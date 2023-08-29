/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "main_functions.h"

// C libs.
#include <stdio.h>

// TensorFlow Lite Micro lib.
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"

// ESP-IDF libs.
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_heap_caps.h>
#include <esp_timer.h>
#include <esp_log.h>
#include "esp_main.h"

// Project headers.
#include "model_settings.h"
#include "model_data.h"
#include "images.h"
#include "esp_cli.h"

// Global vars.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
// Heap memory to allocate (input, output and intermediate arrays).
constexpr int kTensorArenaSize = 140 * 1024;
static uint8_t *tensor_arena;
}


/*
Get model.
Allocate tensors in heap.
Get interpreter class.
*/
void setup() {
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure.
  // Use int8 for input/output of models (better tensorflow optimization).
  model = tflite::GetModel(cnn_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d, not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // Allocate heap space.
  if (tensor_arena == NULL) {
    tensor_arena = (uint8_t *) heap_caps_malloc(
      kTensorArenaSize,
      MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  }
  // Prevent heap explosion.
  if (tensor_arena == NULL) {
    printf("! malloc FAILED\n"
           "- Could NOT allocate memory of %d bytes\n", kTensorArenaSize);
    return;
  }

  // Reduce code memory usage by choosing what to include.
  static tflite::MicroMutableOpResolver<6> micro_op_resolver;
  micro_op_resolver.AddQuantize();
  micro_op_resolver.AddConv2D();
  micro_op_resolver.AddAveragePool2D();
  micro_op_resolver.AddReshape();
  micro_op_resolver.AddFullyConnected();
  micro_op_resolver.AddLogistic();

  // Build an interpreter to run the model.
  static tflite::MicroInterpreter static_interpreter(
      model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Associate the model interpreter to the heap allocated.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Get the model's input.
  input = interpreter->input(0);
}

/*
Use model to infer results from an input.
*/
float run_inference(void *x_input) {
  // Pass x_input (image) to input data, converting uint8 to int8 (^0x80).
  for (int i = 0; i < kNumCols * kNumRows; i++) {
    input->data.int8[i] = ((uint8_t *) x_input)[i] ^ 0x80;
  }

  // Run the model on this input.
  if (kTfLiteOk != interpreter->Invoke()) {
    error_reporter->Report("Invoke failed.");
  }

  TfLiteTensor* output = interpreter->output(0);

  // Get model output results.
  int8_t result_score = output->data.int8[0];

  // Scale output to float.
  float result_score_f =
      (result_score - output->params.zero_point) * output->params.scale;

  return result_score_f;
}
