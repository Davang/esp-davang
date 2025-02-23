/*!
 * \file	davang_gpio.cpp
 * \details GPIO abstraction class declaration for ESP-IDF, adds c++ checks at compile time.
 * \author	Davang
 * \version	1.0.0
 * \date	09/06/2024
 * \copyright MIT License
 */

/* C includes */

/* C++ includes */

/* 3rd party includes */
#include "esp_err.h"
#include <string>

/* custom includes*/
#include "davang_gpio.hpp"

namespace dvng
{

c_gpio::~c_gpio( )
{
    gpio_isr_handler_remove( m_pin );
    gpio_reset_pin( m_pin );
    gpio_intr_disable( m_pin );
    gpio_uninstall_isr_service( );
}

gpio::LEVEL c_gpio::get_level( )
{
    return static_cast< gpio::LEVEL >( gpio_get_level( m_pin ) );
}

int c_gpio::set_level( const gpio::LEVEL &t_level )
{
    int error = ESP_OK;

    if ( gpio::MODE::OUTPUT != m_mode )
    {
        error = ESP_ERR_NOT_SUPPORTED;
    }
    else if ( gpio::LEVEL::TOTAL <= t_level )
    {
        error = ESP_ERR_INVALID_ARG;
    }
    else
    {
        error = gpio_set_level( m_pin, static_cast< uint32_t >( t_level ) );
    }

    if ( ESP_OK == error )
    {
        m_level = t_level;
    }

    return error;
}

int c_gpio::register_isr( gpio::isr_t t_isr, void *t_arguments )
{
    esp_err_t error;

    if ( true == m_is_interrupt )
    {
        if ( true == static_cast< bool >( t_isr ) )
        {
            gpio_isr_t isr = *( t_isr.target< void ( * )( void * ) >( ) );
            error          = gpio_isr_handler_add( m_pin, isr, t_arguments );
        }
        else
        {
            error = ESP_ERR_INVALID_ARG;
        }
    }
    else
    {
        error = ESP_ERR_NOT_SUPPORTED;
    }

    return error;
}

void c_gpio::deregister_isr( )
{
    if ( true == m_is_interrupt )
    {
        (void)gpio_isr_handler_remove( m_pin );
    }
}

} //namespace dvng