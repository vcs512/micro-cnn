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
#include "esp_task_wdt.h"

#include "esp_main.h"
#include "esp_cli.h"

// placeholder for other functions
uint8_t libera = 0;

// inference for image in database
void tf_test(void* i) {
  
  uint8_t ii = *((int *)i);
  printf("\n========== test image %d\n", ii);
  inference_handler(ii);
  
  libera = 1;
  vTaskDelete(NULL);
}

// main function
extern "C" void app_main() {

  // prepare heap and database
  setup();

  printf("\n======================== SETUP READY\n");

  for (uint8_t i = 0; i < 10; i++) {
    // avoid task watchdog
    vTaskDelay(100);

    libera = 0;
    //                          function,      name,  stack size, parameter, priority
    xTaskCreate((TaskFunction_t)&tf_test, "tf_test",    4 * 1024, (void *)&i, 10, NULL);  
    
    // integrate other functions meanwhile
    while (libera==0) ;    
  }

  printf("\n\n======================== FINISHED TASKS =========");
  vTaskDelete(NULL);
  

}
