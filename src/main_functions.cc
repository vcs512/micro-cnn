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

#include <stdio.h>

#include "main_functions.h"

#include "detection_responder.h"
#include "model_settings.h"
#include "model_data.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <esp_heap_caps.h>
#include <esp_timer.h>
#include <esp_log.h>
#include "esp_main.h"

#include "images.h"
#include "esp_cli.h"

// Globals
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;

// Memory for input, output and intermediate arrays:
constexpr int kTensorArenaSize = 140 * 1024;
static uint8_t *tensor_arena; //size: [kTensorArenaSize];
}  // namespace



// Get model
// Alocate tensors in heap
// Get interpreter class
void setup() {
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure.
  // Use int8 for input/output of models (better tensorflow optimization)
  model = tflite::GetModel(g_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // prevent heap exploding
  if (tensor_arena == NULL) {
    tensor_arena = (uint8_t *) heap_caps_malloc(kTensorArenaSize, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  }
  if (tensor_arena == NULL) {
    printf("malloc FAILED\n -Could NOT allocate memory of %d bytes\n", kTensorArenaSize);
    return;
  }


  // // AllOpsResolver: use full tflite lib
        // - too much code memory
        // - no need to select operations manually
  // tflite::AllOpsResolver micro_op_resolver;

  // Save code memory choosing what to include
  static tflite::MicroMutableOpResolver<7> micro_op_resolver;
  micro_op_resolver.AddConv2D();
  micro_op_resolver.AddReshape();
  micro_op_resolver.AddLogistic();
  micro_op_resolver.AddQuantize();
  micro_op_resolver.AddFullyConnected();
  micro_op_resolver.AddMaxPool2D();
  micro_op_resolver.AddAveragePool2D();

  // Can be added (tflite asks as runtime error):
  // micro_op_resolver.AddDepthwiseConv2D();
  // micro_op_resolver.AddExpandDims();
  // micro_op_resolver.AddAveragePool2D();
  // micro_op_resolver.AddMaxPool2D();
  // micro_op_resolver.AddSoftmax();
  // micro_op_resolver.Add

  // Build an interpreter to run the model
  static tflite::MicroInterpreter static_interpreter(
      model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors
  // Beware of heap explosion
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Get information about the memory area to use for the model's input.
  input = interpreter->input(0);

  // image_data_init();
}


// Get results from an input
void run_inference(void *ptr) {
  //  image -> data 
  //  uint8 -> int8
  for (int i = 0; i < kNumCols * kNumRows; i++) {
    input->data.int8[i] = ((uint8_t *) ptr)[i] ^ 0x80;
    
    // PoC entries
    // input->data.int8[i] = test_img[i] ^ 0x80;
    // input->data.int8[i] = 50 - 128;
    // if (i==0) {printf("\ninput 0 = %d", input->data.int8[i]);}

  }


  // Run the model on this input and make sure it succeeds.
  if (kTfLiteOk != interpreter->Invoke()) {
    error_reporter->Report("Invoke failed.");
  }

  TfLiteTensor* output = interpreter->output(0);

  // // Dimensions:
  // printf("\n== input size: %d", input->bytes);
  // printf("\n=== output size: %d  ", output->dims->size);

  // Get inference results:
  int8_t result_score = output->data.int8[0];

  // float to show in terminal:
  float result_score_f =
      (result_score - output->params.zero_point) * output->params.scale;

  printf("\n== OUTPUT = %f %%", result_score_f*100);

  // implement output
  // RespondToDetection(error_reporter, result_score_f);
}
