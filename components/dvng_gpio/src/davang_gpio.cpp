/*!
 * \file	davang_gpio.cpp
 * \details GPIO abstraction class declaration for ESP-IDF, adds c++ checks at compile time.
 * \author	Davang
 * \version	1.0.0
 * \date	09/06/2024
 * \copyright MIT License
 */

/* custom includes*/
#include "davang_gpio.hpp"


namespace dvng
{

c_gpio::~c_gpio( )
{
	// do nothing you are responsible of deinitialize the pin
}

int c_gpio::deinit( )
{
	return  gpio_reset_pin( static_cast< gpio_num_t >( m_pin ) );
}

int c_gpio::init( )
{
	return  gpio_config( &m_gpio_config );
}

gpio::LEVEL c_gpio::get_level( )
{
	return  static_cast< gpio::LEVEL >( gpio_get_level( static_cast< gpio_num_t >( m_pin ) ) );
}

int c_gpio::set_level( const gpio::LEVEL & t_level )
{
	int error = ESP_OK;
	
	if ( gpio::MODE::OUTPUT == m_mode )
	{
		error = gpio_set_level( static_cast< gpio_num_t >( m_pin ), std::to_underlying( m_level ) );
		if( ESP_OK == error )
		{
			m_level = t_level;
		}
	}
	else
	{
		error = ESP_ERR_NOT_SUPPORTED;
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
	if ( gpio::GPIO_INTERRUPT::NONE == m_interrupt )
	{
		return ESP_ERR_NOT_SUPPORTED;
	}
	else
	{			
		esp_err_t error = gpio_set_intr_type(static_cast< gpio_num_t >( m_pin ), static_cast< gpio_int_type_t >( m_interrupt ));
		
		if( ESP_OK == error )
		{
			error = gpio_install_isr_service( ESP_INTR_FLAG_EDGE | ESP_INTR_FLAG_IRAM | ESP_INTR_FLAG_LOWMED );
		}
		
		if( ESP_OK == error )
		{
			error = gpio_isr_handler_add( static_cast< gpio_num_t >( m_pin ), t_isr, t_arguments );
		}

		if( ESP_OK != error )
		{
			error = gpio_isr_handler_remove( static_cast< gpio_num_t >( m_pin ) );
			gpio_uninstall_isr_service( );
		}

		
		return error;
	}
}

}