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


}