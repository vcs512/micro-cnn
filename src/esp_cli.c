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

#define IMAGE_COUNT 2
static uint8_t *image_database[IMAGE_COUNT];


extern const uint8_t image0_start[]   asm("_binary_image0_start");
extern const uint8_t image1_start[]   asm("_binary_image1_start");


static void image_database_init()
{
    image_database[0] = (uint8_t *) image0_start;
    image_database[1] = (uint8_t *) image1_start;
}

int inference_handler(int argc)
{

    image_database_init();

    int image_number = argc;

    if((image_number < 0) || (image_number >= IMAGE_COUNT)) {
        printf("RETURNING: Selected invalid image");
        return -1;
    }


    unsigned detect_time;
    detect_time = esp_timer_get_time();

    run_inference((void *)image_database[image_number]);
    
    detect_time = (esp_timer_get_time() - detect_time)/1000;
    printf("Time required for the inference is %u ms   ", detect_time);

    return 0;
}