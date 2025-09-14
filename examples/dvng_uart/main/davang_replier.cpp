#include <cstring>
#include <cstdint>
#include <functional>

#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "davang_uart.hpp"


extern "C" void app_main()
{
    dvng::c_uart replier = dvng::c_uart( dvng::uart::s_config< 0, dvng::uart::BAUD_RATE::BR_115200 >( ) );

    int error = ESP_OK;
    if ( ESP_OK == error )
    {
        std::string message = " send quit to terminate ";
        ( void )replier.send( message.c_str(), message.size() );

        message.reserve( 255 );
        while ( message != "quit" )
        {
            char msg[255] = {0};
            size_t size = 255;
            if( ESP_OK == replier.receive( msg, size ) )
            {
                message = msg;
                ( void )replier.send( message.c_str(), message.size() );
            }

            vTaskDelay( 100 / portTICK_PERIOD_MS );
        }
    }
}