/*!
 * \file	davang_gpio.cpp
 * \details UART abstraction class declaration for ESP-IDF, adds c++ checks at compile time.
 * \author	Davang
 * \version	1.0.0
 * \date	09/06/2024
 * \copyright MIT License
 */

/* C includes */

/* C++ includes */

/* 3rd party includes */
#include "esp_err.h"
#include "freertos/FreeRTOS.h"

/* custom includes*/
#include "davang_uart.hpp"



namespace dvng
{
c_uart::~c_uart()
{
    ( void )uart_driver_delete( m_port );
}



int c_uart::send( const void * t_data, size_t & t_length )
{
    int error = ESP_OK;

    if ( ( nullptr == t_data ) || ( 0 == t_length ) )
    {
        error = ESP_ERR_INVALID_ARG;
    }
    else
    {
        error = uart_write_bytes( m_port, t_data, t_length );

        if ( ESP_FAIL == error )
        {
            t_length = 0;
        }
        else
        {
            t_length = static_cast< size_t >( error );
            error    = ESP_OK;
        }
    }

    return error;
}



int c_uart::send( const void * t_data, const size_t & t_length )
{
    int error = ESP_OK;

    if ( ( nullptr == t_data ) || ( 0 == t_length ) )
    {
        error = ESP_ERR_INVALID_ARG;
    }
    else
    {
        error = uart_write_bytes( m_port, t_data, t_length );

        if ( ESP_FAIL != error )
        {
            error = ESP_OK;
        }
    }

    return error;
}



int c_uart::receive( TickType_t t_timeout, void * t_data, size_t & t_length )
{
    int error = ESP_OK;

    if ( ( nullptr == t_data ) || ( 0 == t_length ) )
    {
        error = ESP_ERR_INVALID_ARG;
    }
    else
    {
        error = uart_read_bytes( m_port, t_data, static_cast< uint32_t >( t_length ), t_timeout );

        if ( ESP_FAIL == error )
        {
            t_length = 0;
        }
        else
        {
            t_length = static_cast< size_t >( error );
            error    = ESP_OK;
        }
    }

    return error;
}



int c_uart::receive( void * t_data, size_t & t_length )
{
    return receive( 100 / portTICK_PERIOD_MS, t_data, t_length );
}
}