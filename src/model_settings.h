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

#ifndef MODEL_SETTINGS_H_
#define MODEL_SETTINGS_H_

// UPDATE for new models.
// constant expressions == allocated in fixed-sized arrays.
// Input image properties.
constexpr int kNumCols = 128;
constexpr int kNumRows = 128;
constexpr int kNumChannels = 1;

constexpr int kMaxImageSize = kNumCols * kNumRows * kNumChannels;

// ML model properties.
constexpr int kCategoryCount = 2;
constexpr int kClass_1 = 1;
constexpr int kClass_0 = 0;
extern const char* kCategoryLabels[kCategoryCount];

#endif  // MODEL_SETTINGS_H_
