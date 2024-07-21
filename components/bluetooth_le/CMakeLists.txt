idf_component_register(SRCS "src/ble.c"
                       INCLUDE_DIRS 
                        "include"
                        "../../external/fff"
                       REQUIRES bt nvs_flash)
target_link_libraries(${COMPONENT_LIB} PUBLIC fff)