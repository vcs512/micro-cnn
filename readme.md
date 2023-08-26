# Visual quality inspection using Machine Learning in ESP32 (TM) - vcs512

# Project architecture

## platformio.ini
Configure PlatformIO:
- Serial monitor speed
- Embedded binary images

## CMakeLists.txt
- Include and components paths


## src/
### Machine learning model setup order:

    model_settings.cc               
    model_settings.h                -- ML0. Image and model properties
    
    model_data.cc                   -- ML1. HEX model generated (tflite)
    model_data.h

### Images database for inferences:

    images.cc                       -- Images for PoC inferences.
    images.h

### Program flow order:

    main.cc                         -- 0. Main task

    main_functions.cc               -- 1. (tflite micro) Setup memory and run inference
    main_functions.h

    esp_cli.c                       -- 2. Inference handler
    esp_cli.h

# References
Useful ML pipeline for IoT and MCU devices:
- https://github.com/vcs512/ML_IoT_pipeline

Base code:
- https://github.com/espressif/tflite-micro-esp-examples

Base book:
- WARDEN, Pete; SITUNAYAKE, Daniel. **TinyML**. O'Reilly Media, Incorporated, 2019.

Dataset used:
- https://www.kaggle.com/datasets/ravirajsinh45/real-life-industrial-dataset-of-casting-product
