#include <cstdint>
#include <functional>
#include "davang_uart.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void callback (dvng::c_uart * t_uart, const void* t_data, size_t t_size)
{
	(void)t_uart->send( t_data, t_size );
}


extern "C" void app_main()
{
	dvng::c_uart replier = dvng::c_uart( dvng::uart::s_asserter< 0 ,
		dvng::uart::BAUD_RATE::BS_115200,
		dvng::uart::WORD_LENGTH::BITS_8,
		dvng::uart::PARITY::NONE,
		dvng::uart::STOP_BITS::STOP_1,
		dvng::uart::HW_CONTROL::NONE,
		dvng::uart::INTERRUPT_BUFFER::RX >( ) );
	
	int error = ESP_OK;

	dvng::uart::isr_t callback_func = std::bind(&callback, &replier, std::placeholders::_1, std::placeholders::_2);
	error = replier.register_isr( callback_func );

	if( ESP_OK == error)
	{		
		char msg_start[255] = "\n\nReplier ready!\n";
		size_t size = strlen(msg_start);
		(void)replier.send( msg_start, size );

		while( ESP_OK == error )
		{
			vTaskDelay( 20'000 / portTICK_PERIOD_MS );
		}
	}
	else
	{
		char msg_error[255] = "\n\nFailed initalization!\n";
		size_t size = strlen(msg_error);
		(void)replier.send( msg_error, size );
	}
}