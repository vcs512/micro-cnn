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

#include <stdio.h>
#include <string.h>
#include <esp_log.h>
#include <esp_console.h>
#include <esp_heap_caps.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <driver/uart.h>

#include "esp_main.h"
#include "esp_cli.h"
#include "esp_timer.h"
#include "images.h"

#define IMAGE_COUNT 10
// static uint8_t *image_database[IMAGE_COUNT];

// // Assembly to refer to bin images in code memory
// extern const uint8_t image0_start[]   asm("_binary_image0_start");
// extern const uint8_t image1_start[]   asm("_binary_image1_start");

// // Assign pointer to bin images
// static void image_database_init() {
//     image_database[0] = (uint8_t *) image0_start;
//     image_database[1] = (uint8_t *) image1_start;
// }

// Bridge to inference function and profiler
int inference_handler(int argc) {

    int image_number = argc;

    if((image_number < 0) || (image_number >= IMAGE_COUNT)) {
        printf("RETURNING: Selected invalid image");
        return -1;
    }


    unsigned detect_time;
    detect_time = esp_timer_get_time();

    // calls tf inference (main_functions.cc)
    // run_inference((void *)image_database[image_number]);
    run_inference((void *)test_img[image_number]);
    
    // get profiler results
    detect_time = (esp_timer_get_time() - detect_time)/1000;
    printf("\nTime required for the inference is %u ms \n\n", detect_time);

    return 0;
}


// // Inits image database
// void image_data_init() {
//     image_database_init();
// }