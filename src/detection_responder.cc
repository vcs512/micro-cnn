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

#include "detection_responder.h"
#include "esp_main.h"

void RespondToDetection(tflite::ErrorReporter* error_reporter,
                        float person_score, float no_person_score) {
  int no_person_score_int = (no_person_score) * 100 + 0.5;

//   TF_LITE_REPORT_ERROR(error_reporter,
//                         "\nCLASS 0: usable score %d%%"
//                         "\nCLASS 1: defect score:%d%%",
//                         100 - person_score_int,
//                         person_score_int);


  TF_LITE_REPORT_ERROR(error_reporter,
                        "\nSaida = %f%%",
                        no_person_score);                        
}
