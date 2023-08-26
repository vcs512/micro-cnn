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

// C libs.
#include <stdio.h>

// ESP-IDF libs.
#include "main_functions.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

// Project code headers.
#include "esp_main.h"
#include "esp_cli.h"


/*
Infer class for image in memory database with index "img_idx_arg".
*/
void tf_test(void* img_idx_arg) {
  // Loop through images database and infer them.
  for (int idx = 0; idx < 10; idx++){
    // Delay to ping task watchdog.
    vTaskDelay(200);
    // Start inference.
    printf("\n- Image %d", idx);
    inference_handler(idx);
  }
  
  vTaskDelete(NULL);
}


/*
Main function.
*/
extern "C" void app_main() {
  // Load model, preparing heap.
  setup();
  printf("\n- SETUP READY\n");

  // Thread (task) to infer images in database.
  xTaskCreate(// function.
              (TaskFunction_t)&tf_test,
              // task name.
              "tf_test",
              // stack size.
              4 * 1024,
              // arguments.
              (void *)0,
              // priority.
              10,
              NULL);  

  vTaskDelete(NULL);
}
