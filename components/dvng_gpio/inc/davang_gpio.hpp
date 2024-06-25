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

/* C++ includes */
#include <utility>

/* 3rd party includes */
#include "driver/gpio.h"
#include "esp_err.h"

/* custom includes*/



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

enum class GPIO_INTERRUPT : uint32_t
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
};

/* constants */

/*!< Specific davang namespace for gpio data types related. */
constexpr pin_t MAX_PIN = GPIO_PIN_COUNT;

/* asssertion structures */
template< gpio::pin_t T_PIN, gpio::MODE T_MODE, 
	gpio::PULL_UP T_PULL_UP = gpio::PULL_UP::NONE,
	gpio::PULL_DOWN T_PULL_DOWN = gpio::PULL_DOWN::NONE, 
	gpio::GPIO_INTERRUPT T_INTERRUPT = gpio::GPIO_INTERRUPT::NONE >
struct s_asserter
{

static constexpr uint64_t OUTPUT_MASK = SOC_GPIO_VALID_OUTPUT_GPIO_MASK;
static constexpr uint64_t GPIO_MASK = SOC_GPIO_VALID_GPIO_MASK;

static constexpr bool IS_OUTPUT_SUPPORTED = ( 0 != ( ( 1ULL << T_PIN ) & OUTPUT_MASK ) );
static constexpr bool IS_MODE_OUTPUT = ( T_MODE == gpio::MODE::OUTPUT );
static constexpr bool IS_MODE_INPUT = ( T_MODE == gpio::MODE::INPUT );

static_assert( ( ( 0 <= T_PIN ) && ( 0 != ( ( 1ULL << T_PIN) & GPIO_MASK ) ) ), "Not a valid gpio pin number, gpio pin should be less than dvng::gpio::MAX_PIN" );

static_assert( ( T_MODE < gpio::MODE::TOTAL ), "Gpio mode not supported" );

static_assert( ( true == IS_MODE_INPUT ) || ( ( true == IS_MODE_OUTPUT ) && ( true == IS_OUTPUT_SUPPORTED ) ) , "Output mode not supported, this pin may only be an input" ); 

static_assert( ( T_MODE < gpio::MODE::TOTAL ), "Gpio mode not supported" );
static_assert( ( T_PULL_UP < gpio::PULL_UP::TOTAL ), "Gpio pull up not supported" );
static_assert( ( T_PULL_DOWN < gpio::PULL_DOWN::TOTAL ), "Gpio pulldown not supported" );
static_assert( ( T_INTERRUPT < gpio::GPIO_INTERRUPT::TOTAL ), "Gpio interrupt not supported" );



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
	const gpio::MODE m_mode;
	const gpio::GPIO_INTERRUPT m_interrupt;

/* constructors and destructor */
public:

c_gpio( ) = delete;

c_gpio( const c_gpio & ) = delete;

c_gpio( const c_gpio && ) = delete;

/*!
 * \ brief c_gpio constructor 
 */
template< gpio::pin_t T_PIN, gpio::MODE T_MODE, 
	gpio::PULL_UP T_PULL_UP = gpio::PULL_UP::NONE,
	gpio::PULL_DOWN T_PULL_DOWN = gpio::PULL_DOWN::NONE, 
	gpio::GPIO_INTERRUPT T_INTERRUPT = gpio::GPIO_INTERRUPT::NONE >
c_gpio( const gpio::s_asserter< T_PIN, T_MODE, T_PULL_UP, T_PULL_DOWN, T_INTERRUPT > ) :  m_gpio_config ( /* pin_bit_mask */ 1ULL << T_PIN,
	/* mode */ static_cast< gpio_mode_t >( T_MODE ),
	/* pull_up_en */ static_cast< gpio_pullup_t  >( T_PULL_UP ),
	/* pull_down_en */ static_cast< gpio_pulldown_t  >( T_PULL_DOWN ),
	/* intr_type */ static_cast< gpio_int_type_t  >( T_INTERRUPT ) ),
	m_level ( gpio::LEVEL::LOW ),
	m_pin ( T_PIN ),
	m_mode ( T_MODE ),
	m_interrupt ( T_INTERRUPT )	
{

}


virtual ~c_gpio( );


/* methods */
public:	
	
	[[nodiscard("Always ensure valid pin initialization")]]
	int init( );

	int deinit( );

	[[nodiscard("Why get the level of a pin if not using it?")]]
	gpio::LEVEL get_level( );

	int set_level( const gpio::LEVEL & t_level );
	
	int set_high( ) ;
	
	int set_low( );
	
	int toggle( );
	
	[[nodiscard("Always ensure correct isr registration ")]]
	int register_isr( gpio::isr_t t_isr, void * t_arguments );


};


} /* namespace dvng */

#endif /* ESP_DAVANG_COMPONENTS_DVNG_GPIO_DAVANG_GPIO_H */