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

/* custom includes*/
#include "davang_gpio.hpp"


namespace dvng
{

c_gpio::~c_gpio( )
{
	if( true == m_initalized_on_constructor )
	{
		if( true == m_is_interrupt )
		{
			deregister_isr( );
		}
		deinit( );
	}
}

int c_gpio::init( )
{
	int error =  gpio_config( &m_config );

	if( ( true == m_is_interrupt ) && ( ESP_OK == error ) )
	{
		error = gpio_set_intr_type(static_cast< gpio_num_t >( m_pin ), static_cast< gpio_int_type_t >( m_interrupt ));

		if( ESP_OK == error )
		{
			error = gpio_install_isr_service( ESP_INTR_FLAG_EDGE | ESP_INTR_FLAG_IRAM | ESP_INTR_FLAG_LOWMED );
		}
	}

	if( ESP_OK != error )
	{
		deinit( );
	}

	return error;
}


void c_gpio::deinit( )
{
	gpio_reset_pin( static_cast< gpio_num_t >( m_pin ) );
	gpio_intr_disable( static_cast< gpio_num_t >( m_pin ) );
	gpio_uninstall_isr_service( );
}

gpio::LEVEL c_gpio::get_level( )
{
	return  static_cast< gpio::LEVEL >( gpio_get_level( static_cast< gpio_num_t >( m_pin ) ) );
}

int c_gpio::set_level( const gpio::LEVEL & t_level )
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
		error = gpio_set_level( static_cast< gpio_num_t >( m_pin ), static_cast<uint32_t>( m_level ) );
	}

	if( ESP_OK == error )
	{
		m_level = t_level;
	}

	return error;
}

int c_gpio::set_high( )
{
	return set_level( gpio::LEVEL::HIGH );
}

int c_gpio::set_low( )
{
	return set_level( gpio::LEVEL::LOW );
}

int c_gpio::toggle( )
{
	if( gpio::LEVEL::LOW == m_level )
	{
		return set_high( );
	}
	else
	{
		return set_low( );	
	}
}

int c_gpio::register_isr( gpio::isr_t t_isr, void * t_arguments )
{
	esp_err_t error;
	
	if( true == m_is_interrupt )
	{
		if( true == static_cast<bool>(t_isr) )
		{
			gpio_isr_t isr = *(t_isr.target<void(*)(void*)>());
			error = gpio_isr_handler_add( static_cast< gpio_num_t >( m_pin ), isr, t_arguments );
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
	if( true == m_is_interrupt )
	{
		(void)gpio_isr_handler_remove( static_cast< gpio_num_t >( m_pin ) );
	}
}

}