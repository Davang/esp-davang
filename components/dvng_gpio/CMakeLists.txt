idf_component_register(	SRCS "src/davang_gpio.cpp" 
						INCLUDE_DIRS "inc"
						REQUIRES esp_driver_gpio )
target_compile_options(${COMPONENT_LIB} PUBLIC -std=gnu++23 -O0)
