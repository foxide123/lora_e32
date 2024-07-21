idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES lora driver unity nvs_flash wifi bluetooth_le)
