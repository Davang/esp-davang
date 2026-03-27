idf_component_register(	SRCS "src/davang_uart.cpp" 
						INCLUDE_DIRS "inc"
						REQUIRES dvng_gpio esp_driver_uart freertos )
target_compile_options(${COMPONENT_LIB} PUBLIC -std=gnu++23 -O0)
