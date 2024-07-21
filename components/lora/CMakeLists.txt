idf_component_register(SRCS "src/lora_e32.c" "test/lora_e32_test.c"
                       INCLUDE_DIRS "include" "../../external/fff"
                       REQUIRES driver unity)
target_link_libraries(${COMPONENT_LIB} PUBLIC fff)