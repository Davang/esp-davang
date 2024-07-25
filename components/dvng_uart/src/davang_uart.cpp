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

/* custom includes*/
#include "davang_uart.hpp"


namespace dvng
{

void isr_task ( void * t_uart );

c_uart::~c_uart()
{
	if( true == m_initalized_on_constructor )
	{
		if( true == m_is_interrupt )
		{
			deresgister_rx_isr( );
			deresgister_tx_isr( );
			m_should_finish = true;
			vTaskDelay( MIN_WAIT );
			vTaskDelete( m_isr_task );
			vSemaphoreDelete( m_tx_isr_mutex );
			vSemaphoreDelete( m_rx_isr_mutex );
		}
		deinit( );
	}
}



int c_uart::init( )
{
	int error = ESP_OK;
	if( true == m_is_interrupt )
	{
		error = ESP_FAIL;
		/*
		error = uart_driver_install(  static_cast<uart_port_t>(m_port), BUFFER_SIZE, BUFFER_SIZE, QUEUE_SIZE, &m_queue, 0);
			
		if( ESP_OK == error )
		{
			m_tx_isr_mutex = xSemaphoreCreateBinary( );
			if( nullptr == m_tx_isr_mutex )
			{
				error = ESP_ERR_NOT_ALLOWED;
			}
		}

		if( ESP_OK == error )
		{
			m_rx_isr_mutex = xSemaphoreCreateBinary( );
			if( nullptr == m_tx_isr_mutex )
			{
				error = ESP_ERR_NOT_ALLOWED;
			}
		}

		if( ESP_OK == error )
		{
			
			if( pdPASS != xTaskCreate( isr_task, "isr_task", BUFFER_SIZE, this, PRIORITY, &m_isr_task) )
			{
				error = ESP_ERR_NOT_ALLOWED;
			}
		}*/
	}
	else
	{
		error = uart_driver_install(  static_cast<uart_port_t>(m_port), BUFFER_SIZE, 0, 0, nullptr, 0 );
	}	

	if( ESP_OK == error )
	{
		error = uart_param_config (  static_cast<uart_port_t>(m_port), &m_config );
	}

	if( ESP_OK == error )
	{
		uart_set_pin( static_cast<uart_port_t>(m_port), UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	}
	return error;
}

void c_uart::deinit( )
{
	m_should_finish = true;
}

int c_uart::send( const void *t_data, size_t & t_length )
{
	int result = uart_write_bytes(static_cast<uart_port_t>(m_port), t_data, t_length );

	if( ESP_FAIL == result )
	{
		t_length = 0;
	}
	else
	{
		t_length = static_cast<size_t>(result);
		result = ESP_OK;
	}

	return result;
}

int c_uart::receive( void * t_data, size_t & t_length )
{
	int result = uart_read_bytes(static_cast<uart_port_t>(m_port), t_data, static_cast<uint32_t>(t_length), DEFAULT_WAIT);

	if( ESP_FAIL == result )
	{
		t_length = 0;
	}
	else
	{
		t_length = static_cast<size_t>(result);
		result = ESP_OK;
	}

	return result;
}

size_t c_uart::get_input_data_length( )
{
	return 0;
}

int c_uart::register_tx_isr( uart::tx_isr_t t_isr )
{
	m_should_finish = true;
	return ESP_OK;
}
	
int c_uart::register_rx_isr( uart::rx_isr_t t_isr )
{

	m_should_finish = true;
	return ESP_OK;
}

void c_uart::deresgister_rx_isr( )
{

}

void c_uart::deresgister_tx_isr( )
{

}

void isr_task ( void * t_uart )
{
	if (nullptr == t_uart) 
		return;

	c_uart * uart = reinterpret_cast<c_uart*>( t_uart );

	while ( false == uart->CheckIfShouldFinish( ) )
	{
		vTaskDelay( 100'000 / portTICK_PERIOD_MS );
	}

}


}