idf_component_register(	SRCS "src/davang_uart_stream.cpp" 
						INCLUDE_DIRS "inc"
						REQUIRES dvng_uart )
target_compile_options(${COMPONENT_LIB} PUBLIC -std=gnu++20)
