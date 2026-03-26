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

    std::string message = " send quit to terminate ";
    std::array< std::byte, 255 > buffer { std::byte{ 0 } };
    memcpy( buffer.data(), message.c_str(), message.size() );
    (void)replier.send( buffer );

    while ( message != "quit" )
    {
        buffer.fill( std::byte{0} );
        const auto & [error, size] = replier.receive( buffer );
        if ( ESP_OK == error )
        {
            (void)replier.send( buffer );
        }

        std::string message = " received " + std::to_string(size) +" bytes\n";
        memcpy( buffer.data(), message.c_str(), message.size() );
        (void)replier.send( buffer );
        
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }

}