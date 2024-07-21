idf_component_register(SRCS "src/wifi_scan.c" "test/wifi_test.c"
                       INCLUDE_DIRS "include" "../../external/fff"
                       REQUIRES driver unity esp_wifi)
target_link_libraries(${COMPONENT_LIB} PUBLIC fff)