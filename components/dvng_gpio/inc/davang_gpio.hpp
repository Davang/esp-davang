/*!
 * \file	davang_gpio.h
 * \details GPIO abstraction class declaration for ESP-IDF.
 * \author	Davang
 * \version	1.0.0
 * \date	2023/09/25
 * \copyright MIT License
 */

#ifndef ESP_DAVANG_COMPONENTS_DVNG_GPIO_DAVANG_GPIO_H
#define ESP_DAVANG_COMPONENTS_DVNG_GPIO_DAVANG_GPIO_H

/* C/C++ includes */
#include <utility>
#include <cstdint>

/* System specific libaries */
#include "driver/gpio.h"
#include "esp_err.h"

/* Davang includes */

/* 3rd party includes */


/*! Specific davang namespace for gpio data types related. */
namespace  dvng::gpio
{

/* data types */

using pin_t = uint32_t;		/*! \brief data type representing   */
using isr_t = gpio_isr_t;	/*! \brief data type representing   */

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
	DISABLE = GPIO_PULLUP_DISABLE,
	ENABLE = GPIO_PULLUP_ENABLE,
	TOTAL,
};

enum class PULL_DOWN : uint32_t
{
	DISABLE = GPIO_PULLDOWN_DISABLE,
	ENABLE = GPIO_PULLDOWN_ENABLE,
	TOTAL,
};

enum class GPIO_INTERRUPT : uint32_t
{
	NONE = GPIO_INTR_DISABLE,
	RAISE = GPIO_INTR_POSEDGE,
	FALL = GPIO_INTR_NEGEDGE,
	EDGE = GPIO_INTR_ANYEDGE,
	LOW = GPIO_INTR_LOW_LEVEL,
	HIGH = GPIO_INTR_HIGH_LEVEL,
	TOTAL,
};

enum class LEVEL : uint32_t
{
	LOW = 0,
	HIGH = 1,
};

/* constants */

/*!< Specific davang namespace for gpio data types related. */
constexpr pin MAX_PIN = GPIO_PIN_COUNT;

/* asssertion structures */
template< gpio::pin_T T_PIN, gpio::MODE T_MODE, gpio::PULL_UP T_PULLUP, const gpio::PULL_DOWN T_PULL_DOWN, gpio::GPIO_INTERRUPT T_INTERRUPT >
struct s_asserter
{	
static constexpr uint64_t OUTPUT_MASK = SOC_GPIO_VALID_OUTPUT_GPIO_MASK;
static constexpr uint64_t GPIO_MASK = SOC_GPIO_VALID_GPIO_MASK;

static constexpr bool IS_OUTPUT_SUPPORTED = ( 0 != ( ( 1ULL << T_PIN ) & OUTPUT_MASK ) );
static constexpr bool IS_PIN_VALID = ;
static constexpr bool IS_MODE_OUTPUT =  ( T_MODE == gpio::MODE::OUTPUT );

static_assert( ( ( 0 <= T_PIN ) && ( 0 != ( ( 1ULL << T_PIN) & GPIO_MASK ) ) ), "Not a valid gpio pin number, gpio pin should be less than dvng::gpio::MAX_PIN" );

static_assert( ( T_MODE < gpio::MODE::TOTAL ), "Gpio mode not supported" );

static_assert( ( IS_MODE_OUTPUT == IS_OUTPUT_SUPPORTED ), "Output mode not supported, this pin may only be an input" ); 

static_assert( ( T_MODE < gpio::PULL_DOWN::TOTAL ), "Gpio pull mode not supported" );
static_assert( ( T_MODE < gpio::GPIO_INTERRUPT::TOTAL ), "Gpio interrupt mode not supported" );

};


} /* namespace dvng::gpio */

/*!< Generic davang namespace */
namespace dvng
{


class c_gpio
{
/* constants */
private:
	const gpio_config_t m_gpio_config;

/* data members */
private:
	gpio::LEVEL m_level;
	gpio::pin_t m_pin;

/* constructors and destructor */
public:

c_gpio( ) = delete;

c_gpio( const c_gpio & ) = delete;

c_gpio( const c_gpio && ) = delete;

/*!
 * \ brief c_gpio constructor 
 */
template< gpio::pin_T T_PIN, gpio::MODE T_MODE, 
	gpio::PULL_UP T_PULLUP = gpio::PULL_UP::GPIO_PULLUP_DISABLE,
	gpio::PULL_DOWN T_PULL_DOWN = gpio::PULL_DOWN::GPIO_PULLDOWN_DISABLE, 
	gpio::GPIO_INTERRUPT T_INTERRUPT = gpio::GPIO_INTERRUPT::GPIO_INTR_DISABLE >
constexpr c_gpio( const gpio::::asseter< T_PIN, T_MODE, T_PULLUP, T_PULL_DOWN, T_INTERRUPT > ) :  m_gpio_config ( /* pin_bit_mask */ 1ULL << T_PIN,
	/* mode */ static_cast< gpio_mode_t >( T_MODE ),
	/* pull_up_en */ static_cast< gpio_pullup_t  >( T_PULLUP ),
	/* pull_down_en */ static_cast< gpio_pulldown_t  >( T_PULLDOWN ),
	/* intr_type */ static_cast< gpio_int_type_t  >( T_INTERRUPT ) ),
	m_level ( gpio::LEVEL::LOW ),
	m_pin ( T_PIN )
{

}


virtual ~c_gpio( );


/* methods */
public:	
	
	[[nodiscard("Always ensure valid pin initialization")]]
	int init( );

	[[nodiscard("Why get the level of a pin if not using it?")]]
	gpio::LEVEL get_level( );

	int set_level( const gpio::LEVEL & t_level );
	
	int set_high( ) ;
	
	int set_low( );
	
	int toggle( );
	
	[[nodiscard("Always ensure correct isr registration ")]]
	int register_isr( gpio::isr t_isr, void * t_arguments );


};


} /* namespace dvng */

#endif /* ESP_DAVANG_COMPONENTS_DVNG_GPIO_DAVANG_GPIO_H */