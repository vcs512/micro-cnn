# Visual quality inspection using Machine Learning in ESP32 (TM)

## Project architecture

### platformio.ini
Configure PlatformIO:
- Serial monitor speed
- Embedded binary images

### CMakeLists.txt
- Include and components paths


### src/
Machine learning model setup order:

    model_settings.cc               
    model_settings.h                -- ML0. Image and model properties
    
    person_detect_model_data.cc     -- ML1. HEX model generated (tflite)
    person_detect_model_data.h

Program flow order:

    esp_main.h                      -- 0. Options of Espressif example code

    main.cc                         -- 1. Main task

    main_functions.cc               -- 2. (tflite micro) Setup memory and run inference
    main_functions.h

    esp_cli.c                       -- 3. Image database and inference handler
    esp_cli.h
    
    detection_responder.cc          -- debug and PoC
    detection_responder.h


### static_images
Binary archives to test the convolutional neural network

# References
Base code:
- https://github.com/espressif/tflite-micro-esp-examples

Base book:
- WARDEN, Pete; SITUNAYAKE, Daniel. **TinyML**. O'Reilly Media, Incorporated, 2019.

Dataset used:
- https://www.kaggle.com/datasets/ravirajsinh45/real-life-industrial-dataset-of-casting-product