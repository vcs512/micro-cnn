// Copyright 2020-2021 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


// ESP-IDF libs.
#include <stdio.h>
#include <string.h>
#include <esp_log.h>
#include <esp_console.h>
#include <esp_heap_caps.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <driver/uart.h>

// Project code headers.
#include "esp_main.h"
#include "esp_cli.h"
#include "esp_timer.h"
#include "images.h"

// Defines.
#define IMAGE_COUNT 10

/*
Bridge to inference function and profiler
*/
int inference_handler(int img_idx) {
  // Check index boundaries.
  if((img_idx < 0) || (img_idx >= IMAGE_COUNT)) {
    printf("! Selected INVALID index for image");
    return -1;
  }

  // Time measurement.
  unsigned inference_time;
  inference_time = esp_timer_get_time();

  // Start indeference for image.
  float result_score_f;
  result_score_f = run_inference((void *)test_img[img_idx]);
  printf("\n-- OUTPUT = %f %%", result_score_f*100);
    
  // Time profiler results.
  inference_time = (esp_timer_get_time() - inference_time) / 1000;
  printf("\n-- Time for inference = %u ms \n", inference_time);

  return 0;
}
