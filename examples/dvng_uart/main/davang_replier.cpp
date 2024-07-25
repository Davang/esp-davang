#include "davang_uart.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern "C" void app_main()
{
	//vTaskDelay( 25 / portTICK_PERIOD_MS );
	dvng::c_uart replier = dvng::c_uart( dvng::uart::s_asserter< 0 >( ) );
	
	int error = ESP_OK;
	const char msg[255] = "Hello world!\n";

	while( ESP_OK == error )
	{
		size_t size = strlen(msg);
		error = replier.send( msg, size );
		vTaskDelay( 1'000 / portTICK_PERIOD_MS );
	}
}



