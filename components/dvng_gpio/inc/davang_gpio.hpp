/*!
 * \file	davang_gpio.hpp
 * \details GPIO abstraction class declaration for ESP-IDF.
 * \author	Davang
 * \version	1.0.0
 * \date	2023/09/25
 * \copyright MIT License
 */

#ifndef ESP_DAVANG_COMPONENTS_DVNG_GPIO_DAVANG_GPIO_H
#define ESP_DAVANG_COMPONENTS_DVNG_GPIO_DAVANG_GPIO_H

/* C includes */
#include <cstdint>
#include <cassert>

/* C++ includes */
#include <utility>
#include <functional>

/* 3rd party includes */
#include "driver/gpio.h"
#include "esp_system.h"

/* custom includes*/



/*! Specific davang namespace for gpio data types related. */
namespace  dvng::gpio
{

/* data types */

using pin_t = uint32_t;		/*! \brief data type representing   */
using isr_t = std::function<void(void*)>;	/*! \brief data type representing   */

/* isr_t functions are : static void IRAM_ATTR gpio_isr_handler(void* arg); */

/* enumerators */

enum class MODE : uint64_t
{
	INPUT = GPIO_MODE_INPUT,
	OUTPUT = GPIO_MODE_OUTPUT,
	OUTPUT_OPEN_DRAIN = GPIO_MODE_OUTPUT_OD,
	TOTAL,
};


enum class PULL_UP : uint32_t
{
	NONE = GPIO_PULLUP_DISABLE,
	ACTIVE = GPIO_PULLUP_ENABLE,
	TOTAL,
};

enum class PULL_DOWN : uint32_t
{
	NONE = GPIO_PULLDOWN_DISABLE,
	ACTIVE = GPIO_PULLDOWN_ENABLE,
	TOTAL,
};

enum class INTERRUPT_CHANGE : uint32_t
{
	NONE = GPIO_INTR_DISABLE,
	RAISE = GPIO_INTR_POSEDGE,
	FALL = GPIO_INTR_NEGEDGE,
	EDGE = GPIO_INTR_ANYEDGE,
	TOTAL,
};

enum class LEVEL : uint32_t
{
	LOW = 0,
	HIGH,
	TOTAL,
};

/* constants */

/*!< Specific davang namespace for gpio data types related. */
constexpr pin_t MAX_PIN = GPIO_PIN_COUNT;


/* asssertion structures */
template< typename T, T VAL >
struct s_enum_validator
{
    static_assert( ( VAL < T::TOTAL ) );
};


template< pin_t T_PIN >
struct s_pin
{
	static constexpr pin_t M_PIN = T_PIN;
	static_assert( ( ( 0 <= M_PIN ) && ( 0 != ( ( 1ULL << M_PIN) & SOC_GPIO_VALID_GPIO_MASK ) ) ), "Not a valid gpio pin number" );
};



template< MODE T_MODE >
struct s_mode : s_enum_validator<MODE, T_MODE>
{
	static constexpr MODE M_MODE = T_MODE;
};


template< PULL_UP T_PULL_UP >
struct s_pull_up : s_enum_validator<PULL_UP, T_PULL_UP>
{
	static constexpr PULL_UP M_PULL_UP = T_PULL_UP;
};

template< PULL_DOWN T_PULL_DOWN >
struct s_pull_down 
{
	static constexpr PULL_DOWN M_PULL_DOWN = T_PULL_DOWN;
};


template< INTERRUPT_CHANGE T_INTERRUPT = INTERRUPT_CHANGE::NONE >
struct s_interrupt : s_enum_validator<INTERRUPT_CHANGE, T_INTERRUPT>
{
	static constexpr INTERRUPT_CHANGE M_INTERRUPT = T_INTERRUPT;
};

template< pin_t T_PIN, MODE T_MODE, 
	PULL_UP T_PULL_UP = PULL_UP::NONE,
	PULL_DOWN T_PULL_DOWN = PULL_DOWN::NONE, 
	INTERRUPT_CHANGE T_INTERRUPT = INTERRUPT_CHANGE::NONE >
struct s_config : public s_pin<T_PIN>, s_mode<T_MODE>, s_pull_up<T_PULL_UP>, s_pull_down<T_PULL_DOWN>, s_interrupt<T_INTERRUPT>
{
	static constexpr bool IS_OUTPUT_SUPPORTED = ( 0 != ( ( 1ULL << T_PIN ) & SOC_GPIO_VALID_OUTPUT_GPIO_MASK ) );
	static_assert( ( T_MODE == MODE::INPUT ) || ( ( T_MODE == MODE::OUTPUT ) && ( true == IS_OUTPUT_SUPPORTED ) ) , "Output mode not supported, this pin may only be an input" ); 
};


} /* namespace dvng::gpio */

/*!< Generic davang namespace */
namespace dvng
{


class c_gpio
{
/* constants */
private:
	const gpio_config_t m_config;
	const gpio_num_t m_pin;
	const gpio::MODE m_mode;
	const bool m_is_interrupt;
/* data members */
private:
	gpio::LEVEL m_level;

/* constructors and destructor */
public:

c_gpio( ) = delete;

c_gpio( const c_gpio & ) = delete;

c_gpio( const c_gpio && ) = delete;

/*!
 * \ brief c_gpio constructor 
 */
template< auto ... ARGS_T >
c_gpio( const gpio::s_config< ARGS_T... > & t_config) :
	m_config {
		.pin_bit_mask = 1ULL << gpio::s_config< ARGS_T... > ::M_PIN,
		.mode = static_cast< gpio_mode_t >( gpio::s_config< ARGS_T... > ::M_MODE ),
		.pull_up_en = static_cast< gpio_pullup_t  >( gpio::s_config< ARGS_T... > ::M_PULL_UP ),
		.pull_down_en = static_cast< gpio_pulldown_t  >( gpio::s_config< ARGS_T... > ::M_PULL_DOWN ),
		.intr_type = static_cast< gpio_int_type_t  >( gpio::s_config< ARGS_T... > ::M_INTERRUPT ),
	},
	m_pin { static_cast<gpio_num_t>( gpio::s_config< ARGS_T... >::M_PIN ) },
	m_mode { gpio::s_config< ARGS_T... >::M_MODE },
	m_is_interrupt{ gpio::s_config< ARGS_T... >::M_INTERRUPT !=  gpio::INTERRUPT_CHANGE::NONE },
	m_level { gpio::LEVEL::LOW }
{
	int error = gpio_config( &m_config );

	if( ( true == m_is_interrupt ) && ( ESP_OK == error ) )
	{
		error = gpio_set_intr_type( m_pin, static_cast< gpio_int_type_t >( gpio::s_config< ARGS_T... >::M_INTERRUPT ));

		if( ESP_OK == error )
		{
			error = gpio_install_isr_service( ESP_INTR_FLAG_EDGE | ESP_INTR_FLAG_IRAM | ESP_INTR_FLAG_LOWMED );
		}
	}

	if( ESP_OK != error )
	{
		esp_restart( );
	}
}

virtual ~c_gpio( );


/* methods */
public:	
	
	[[nodiscard("Why get the level of a pin if not using it?")]]
	gpio::LEVEL get_level( );

	int set_level( const gpio::LEVEL & t_level );
	
	inline int set_high( )
	{
		return set_level( gpio::LEVEL::HIGH );
	}
	
	inline int set_low( )
	{
		return set_level( gpio::LEVEL::LOW );
	}
	
	inline int toggle( )
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

	[[nodiscard("Always ensure correct isr registration ")]]
	int register_isr( gpio::isr_t t_isr, void * t_arguments );
	
	void deregister_isr( );

};


} /* namespace dvng */

#endif /* ESP_DAVANG_COMPONENTS_DVNG_GPIO_DAVANG_GPIO_H */