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
	HIGH = 1,
	TOTAL,
};

/* constants */

/*!< Specific davang namespace for gpio data types related. */
constexpr pin_t MAX_PIN = GPIO_PIN_COUNT;

/* asssertion structures */
template< pin_t T_PIN, MODE T_MODE, 
	PULL_UP T_PULL_UP = PULL_UP::NONE,
	PULL_DOWN T_PULL_DOWN = PULL_DOWN::NONE, 
	INTERRUPT_CHANGE T_INTERRUPT = INTERRUPT_CHANGE::NONE >
struct s_asserter
{

static constexpr uint64_t OUTPUT_MASK = SOC_GPIO_VALID_OUTPUT_GPIO_MASK;
static constexpr uint64_t GPIO_MASK = SOC_GPIO_VALID_GPIO_MASK;

static constexpr bool IS_OUTPUT_SUPPORTED = ( 0 != ( ( 1ULL << T_PIN ) & OUTPUT_MASK ) );
static constexpr bool IS_MODE_OUTPUT = ( T_MODE == MODE::OUTPUT );
static constexpr bool IS_MODE_INPUT = ( T_MODE == MODE::INPUT );

static_assert( ( ( 0 <= T_PIN ) && ( 0 != ( ( 1ULL << T_PIN) & GPIO_MASK ) ) ), "Not a valid gpio pin number, gpio pin should be less than dvng::MAX_PIN" );

static_assert( ( T_MODE < MODE::TOTAL ), "Gpio mode not supported" );

static_assert( ( true == IS_MODE_INPUT ) || ( ( true == IS_MODE_OUTPUT ) && ( true == IS_OUTPUT_SUPPORTED ) ) , "Output mode not supported, this pin may only be an input" ); 

static_assert( ( T_MODE < MODE::TOTAL ), "GPIO mode not supported" );
static_assert( ( T_PULL_UP < PULL_UP::TOTAL ), "GPIO pull up not supported" );
static_assert( ( T_PULL_DOWN < PULL_DOWN::TOTAL ), "GPIO pulldown not supported" );
static_assert( ( T_INTERRUPT < INTERRUPT_CHANGE::TOTAL ), "GPIO interrupt not supported" );

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
	const gpio::pin_t m_pin;
	const gpio::MODE m_mode;
	const gpio::INTERRUPT_CHANGE m_interrupt;
	const bool m_is_interrupt;
	const bool m_initalized_on_constructor;
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
template< gpio::pin_t T_PIN, gpio::MODE T_MODE, 
	gpio::PULL_UP T_PULL_UP, gpio::PULL_DOWN T_PULL_DOWN, 
	gpio::INTERRUPT_CHANGE T_INTERRUPT >
c_gpio( const gpio::s_asserter< T_PIN, T_MODE, T_PULL_UP, T_PULL_DOWN, T_INTERRUPT > &, const bool t_should_init ) :
	m_config {
		.pin_bit_mask = 1ULL << T_PIN,
		.mode = static_cast< gpio_mode_t >( T_MODE ),
		.pull_up_en = static_cast< gpio_pullup_t  >( T_PULL_UP ),
		.pull_down_en = static_cast< gpio_pulldown_t  >( T_PULL_DOWN ),
		.intr_type = static_cast< gpio_int_type_t  >( T_INTERRUPT )
	},
	m_pin { T_PIN },
	m_mode { T_MODE },
	m_interrupt { T_INTERRUPT },
	m_is_interrupt{ ( ( gpio::INTERRUPT_CHANGE::NONE < T_INTERRUPT ) && ( gpio::INTERRUPT_CHANGE::TOTAL > T_INTERRUPT) ) },
	m_initalized_on_constructor{t_should_init},
	m_level { gpio::LEVEL::LOW }
{
	if( true == m_initalized_on_constructor )
	{
		if( ESP_OK != init( ) )
		{
			esp_restart( );
		}
	}
}

template< gpio::pin_t T_PIN, gpio::MODE T_MODE, 
	gpio::PULL_UP T_PULL_UP, gpio::PULL_DOWN T_PULL_DOWN, 
	gpio::INTERRUPT_CHANGE T_INTERRUPT >
c_gpio( const gpio::s_asserter< T_PIN, T_MODE, T_PULL_UP, T_PULL_DOWN, T_INTERRUPT > & t_config ) :
	c_gpio( t_config, true )
{

}



virtual ~c_gpio( );


/* methods */
public:	
	
	[[nodiscard("Always ensure valid pin initialization")]]
	int init( );

	void deinit( );

	[[nodiscard("Why get the level of a pin if not using it?")]]
	gpio::LEVEL get_level( );

	int set_level( const gpio::LEVEL & t_level );
	
	int set_high( ) ;
	
	int set_low( );
	
	int toggle( );

	[[nodiscard("Always ensure correct isr registration ")]]
	int register_isr( gpio::isr_t t_isr, void * t_arguments );
	
	void deregister_isr( );


};


} /* namespace dvng */

#endif /* ESP_DAVANG_COMPONENTS_DVNG_GPIO_DAVANG_GPIO_H */