#include <cstdint>
#include "davang_uart.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern "C" void app_main()
{
	dvng::c_uart replier = dvng::c_uart( dvng::uart::s_asserter< 0 >( ) );
	
	int error = ESP_OK;
	char msg[255] = "Replier ready!\n";
	size_t size = strlen(msg);
	error = replier.send( msg, size );

	while( ESP_OK == error )
	{
		size = 255;
		memset(msg,0,size);
		if( ESP_OK == replier.receive( msg, size ) )
		{
			if( 0 != size )
			{
				error = replier.send( msg, size );				
			}
		}
	}
}