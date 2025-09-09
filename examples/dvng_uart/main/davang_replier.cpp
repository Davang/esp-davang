#include <cstdint>
#include <functional>
#include "davang_uart.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"



extern "C" void app_main()
{
    dvng::c_uart replier = dvng::c_uart(
        dvng::uart::s_config< 0,
        dvng::uart::BAUD_RATE::BS_115200 >( ) );

    int error = ESP_OK;
    if ( ESP_OK == error )
    {
        char   msg_start[ 255 ] = "\n\nReplier ready!\n";
        size_t size             = strlen( msg_start );
        ( void )replier.send( msg_start, size );

        while ( ESP_OK == error )
        {
            vTaskDelay( 20'000 / portTICK_PERIOD_MS );
        }
    }
    else
    {
        char   msg_error[ 255 ] = "\n\nFailed initalization!\n";
        size_t size             = strlen( msg_error );
        ( void )replier.send( msg_error, size );
    }
}