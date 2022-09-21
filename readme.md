# Estrutura do projeto

## ./src/
    app_camera_esp.c
    app_camera_esp.h
    CMakeLists.txt
    detection_responder.cc          -- 3. Formata saída                        
    detection_responder.h
    esp_cli.c
    esp_cli.h
    esp_lcd.c
    esp_lcd.h
    esp_main.h                      -- 0. seleciona CLI_only
    image_provider.cc
    image_provider.h
    Kconfig.projbuild
    logo_en_240x240_lcd.h
    main.cc                         -- 1. loop infinito
    main_functions.cc               -- 2. inicia tflite, modelo e faz inferencia (linha 92)
    main_functions.h
    model_settings.cc
    model_settings.h
    person_detect_model_data.cc
    person_detect_model_data.h