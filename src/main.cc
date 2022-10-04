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
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_main.h"

#if CLI_ONLY_INFERENCE
#include "esp_cli.h"
#endif


// tflite task
void tf_main(void) {
  
  printf("\n========== BEGINS TASK\n");
  setup();
  
  // vTaskDelay(portMAX_DELAY);
  inference_handler(9);

  // vTaskDelay(portMAX_DELAY);
  printf("\n========== image0\n");
  inference_handler(0);
  
  // vTaskDelay(portMAX_DELAY);
  printf("\n\n");
  printf("\n========== image1\n");
  inference_handler(1);

  printf("\n\n======================== FINISHED TASK =========");
  fflush(stdout);

  vTaskDelay(portMAX_DELAY);
}


extern "C" void app_main() {

  
  // printf("\n========== BEGINS TASK\n");
  // setup();
  
  // inference_handler(9);

  // printf("\n========== image0\n");
  // inference_handler(0);
  
  // printf("\n\n");
  // printf("\n========== image1\n");
  // inference_handler(1);

  // printf("\n\n======================== FINISHED TASK =========");
  // fflush(stdout);

  // vTaskDelay(portMAX_DELAY);

  //                            function, name,    stack size, -, priority
  xTaskCreate((TaskFunction_t)&tf_main, "tf_main", 4 * 1024, NULL, 0, NULL);
  vTaskDelete(NULL);

}
