idf_component_register(	SRCS "src/davang_uart.cpp" 
						INCLUDE_DIRS "inc"
						REQUIRES esp_common driver hal freertos )
target_compile_options(${COMPONENT_LIB} PUBLIC -std=gnu++20)
