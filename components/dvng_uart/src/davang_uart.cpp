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


c_uart::~c_uart()
{
	if( true == m_initalized_on_constructor )
	{
		if( true == m_is_interrupt )  
		{
			deregister_isr( );
			m_should_finish = true;
			uart_event_t uart_event;
			uart_event.type = UART_EVENT_MAX;
			xQueueSend( m_queue, &uart_event, MIN_WAIT );
			vTaskDelay( MIN_WAIT );
			vSemaphoreDelete( m_isr_mutex );
			}
		deinit( );
	}
}



int c_uart::init( )
{
	int error = ESP_OK;
	if( true == m_is_interrupt )
	{
		
		error = uart_driver_install(  static_cast<uart_port_t>(m_port), BUFFER_SIZE, BUFFER_SIZE, QUEUE_SIZE, &m_queue, 0);
			
		if( ESP_OK == error )
		{
			m_isr_mutex = xSemaphoreCreateMutex( );
			if( nullptr == m_isr_mutex )
			{
				error = ESP_ERR_NOT_ALLOWED;
			}
		}
		if( ESP_OK == error )
		{
			m_should_finish = false;
			if( pdPASS != xTaskCreate( isr_task, "isr_task", BUFFER_SIZE * 2, this, PRIORITY, &m_isr_task) )
			{
				error = ESP_ERR_NOT_ALLOWED;
			}

			vTaskDelay( MIN_WAIT );
			if( false == m_is_task_running )
			{
				error = ESP_FAIL;
			}
		}
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
	
}

int c_uart::send( const void *t_data, size_t & t_length )
{
	int error = ESP_OK;

	if( ( nullptr == t_data ) || ( 0 >= t_length) )
	{
		error = ESP_ERR_INVALID_ARG;
	}
	else
	{
		error = uart_write_bytes(static_cast<uart_port_t>(m_port), t_data, t_length );

		if( ESP_FAIL == error )
		{
			t_length = 0;
		}
		else
		{
			t_length = static_cast<size_t>(error);
			error = ESP_OK;
		}
	}

	return error;
}

int c_uart::receive( void * t_data, size_t & t_length )
{
	int error = ESP_OK;
	
	if( ( nullptr == t_data ) || ( 0 >= t_length) )
	{
		error = ESP_ERR_INVALID_ARG;
	}
	else
	{
		error = uart_read_bytes(static_cast<uart_port_t>(m_port), t_data, static_cast<uint32_t>(t_length), DEFAULT_WAIT);
		
		if( ESP_FAIL == error )
		{
			t_length = 0;
		}
		else
		{
			t_length = static_cast<size_t>(error);
			error = ESP_OK;
		}
	}

	return error;
}

int c_uart::register_isr( uart::isr_t t_isr )
{
	int error = ESP_OK;
	
	if( true == static_cast<bool>(t_isr) )
	{
		if( pdTRUE == xSemaphoreTake( m_isr_mutex, DEFAULT_WAIT ) )
		{
			m_isr_callback = t_isr;
        	(void)xSemaphoreGive( m_isr_mutex );
		}
		else
		{
			error = ESP_ERR_NOT_ALLOWED;
		}
	}
	else
	{
		error = ESP_ERR_INVALID_ARG;
	}

	return error;
}


void c_uart::deregister_isr( )
{
	if( pdTRUE == xSemaphoreTake( m_isr_mutex, DEFAULT_WAIT ) )
	{
		m_isr_callback = nullptr;
    	(void)xSemaphoreGive( m_isr_mutex );
	}
}

void c_uart::wait_for_message_isr( )
{
	while ( false == m_should_finish )
	{
		uart_event_t event;
		if ( pdTRUE == xQueueReceive( m_queue, &event, MAX_WAIT ) )
		{
		 	if ( UART_DATA == event.type )
	 		{	 
	 			size_t event_size = event.size;
	 			while( 0 < event_size )
	 			{
					uint8_t buffer[BUFFER_SIZE] = {};
	 				size_t size = ( ( BUFFER_SIZE < event_size ) ? BUFFER_SIZE : event_size );
					size = uart_read_bytes(static_cast<uart_port_t>(m_port), buffer, static_cast<uint32_t>(size), DEFAULT_WAIT);
					if( ( ESP_FAIL != size ) && ( true == static_cast<bool>(m_isr_callback)) )
					{
						if( pdTRUE == xSemaphoreTake( m_isr_mutex, DEFAULT_WAIT ) )
						{
							m_isr_callback( buffer, size );
    						(void)xSemaphoreGive( m_isr_mutex );
						}
					}				
					event_size -= size;
	 			}
	 		}
		}
	} // end of while
}

void c_uart::isr_task ( void * t_uart )
{
	if ( nullptr != t_uart) 
	{
		c_uart * uart = reinterpret_cast<c_uart*>( t_uart );

		uart->m_is_task_running = true;
	
		uart->wait_for_message_isr( );
		
		uart->m_is_task_running = false;
	}

	vTaskDelete( nullptr );
}


}