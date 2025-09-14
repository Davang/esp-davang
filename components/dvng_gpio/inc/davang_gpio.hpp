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
#include <cassert>
#include <cstdint>

/* C++ includes */
#include <functional>
#include <utility>

/* 3rd party includes */
#include "driver/gpio.h"
#include "esp_system.h"


/* custom includes*/

/*! Specific davang namespace for gpio data types related. */
namespace dvng::gpio
{
/* data types */

using pin_t = uint32_t; /*! \brief data type representing   */
using isr_t = std::function< void ( void * ) >; /*! \brief data type representing   */

/* isr_t functions are : static void IRAM_ATTR gpio_isr_handler(void* arg); */

/* enumerators */

enum class MODE : uint64_t
{
    INPUT             = GPIO_MODE_INPUT,
    OUTPUT            = GPIO_MODE_OUTPUT,
    OUTPUT_OPEN_DRAIN = GPIO_MODE_OUTPUT_OD,
    TOTAL
};


enum class PULL_UP : uint32_t
{
    NONE   = GPIO_PULLUP_DISABLE,
    ACTIVE = GPIO_PULLUP_ENABLE,
    TOTAL
};


enum class PULL_DOWN : uint32_t
{
    NONE   = GPIO_PULLDOWN_DISABLE,
    ACTIVE = GPIO_PULLDOWN_ENABLE,
    TOTAL
};


enum class EVENT : uint32_t
{
    NONE  = GPIO_INTR_DISABLE,
    RAISE = GPIO_INTR_POSEDGE,
    FALL  = GPIO_INTR_NEGEDGE,
    EDGE  = GPIO_INTR_ANYEDGE,
    TOTAL
};


enum class LEVEL : uint32_t
{
    LOW = 0,
    HIGH,
    TOTAL
};


/* constants */

/*!< Specific davang namespace for gpio data types related. */
constexpr pin_t MAX_PIN     = GPIO_PIN_COUNT;
constexpr pin_t DEFAULT_PIN = -1;


/* asssertion structures */
template< dvng::gpio::pin_t T_PIN, dvng::gpio::MODE T_MODE >
struct s_pin_config
{
    static constexpr dvng::gpio::pin_t M_PIN  = T_PIN;
    static constexpr dvng::gpio::MODE  M_MODE = T_MODE;

    static constexpr bool IS_OUTPUT_SUPPORTED = ( DEFAULT_PIN == M_PIN ) || ( 0 != ( ( 1ULL << T_PIN ) & SOC_GPIO_VALID_OUTPUT_GPIO_MASK ) );

    static_assert( ( DEFAULT_PIN == M_PIN ) || ( ( 0 <= M_PIN ) && ( 0 != ( ( 1ULL << M_PIN ) & SOC_GPIO_VALID_GPIO_MASK ) ) ), "Not a valid gpio pin number" );
    static_assert( ( M_MODE == MODE::INPUT ) || ( ( M_MODE == MODE::OUTPUT ) && ( true == IS_OUTPUT_SUPPORTED ) ),
        "Output mode not supported, this pin shall only be an input" );

    static_assert( M_MODE < dvng::gpio::MODE::TOTAL, "Not a valid dvng::gpio::MODE value" );
};


template<
    dvng::gpio::pin_t T_PIN,
    dvng::gpio::MODE T_MODE,
    dvng::gpio::PULL_UP T_PULL_UP = PULL_UP::NONE,
    dvng::gpio::PULL_DOWN T_PULL_DOWN = PULL_DOWN::NONE,
    dvng::gpio::EVENT T_EVENT = EVENT::NONE >
struct s_config : public s_pin_config< T_PIN, T_MODE >
{
    static constexpr dvng::gpio::PULL_UP   M_PULL_UP   = T_PULL_UP;
    static constexpr dvng::gpio::PULL_DOWN M_PULL_DOWN = T_PULL_DOWN;
    static constexpr dvng::gpio::EVENT     M_EVENT     = T_EVENT;

    static_assert( M_PULL_UP < dvng::gpio::PULL_UP::TOTAL, "Not a valid dvng::gpio::PULL_UP value" );
    static_assert( M_PULL_DOWN < dvng::gpio::PULL_DOWN::TOTAL, "Not a valid dvng::gpio::PULL_DOWN value" );
    static_assert( M_EVENT < dvng::gpio::EVENT::TOTAL, "Not a valid dvng::gpio::EVENT value" );
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
    const gpio_num_t    m_pin;
    const gpio::MODE    m_mode;
    const bool          m_is_interrupt;
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
    c_gpio( const gpio::s_config< ARGS_T ... > & t_config ) :
        m_config      {
            .pin_bit_mask = 1ULL << gpio::s_config< ARGS_T ... >::M_PIN,
            .mode         = static_cast< gpio_mode_t >( gpio::s_config< ARGS_T ... >::M_MODE ),
            .pull_up_en   = static_cast< gpio_pullup_t >( gpio::s_config< ARGS_T ... >::M_PULL_UP ),
            .pull_down_en = static_cast< gpio_pulldown_t >( gpio::s_config< ARGS_T ... >::M_PULL_DOWN ),
            .intr_type    = static_cast< gpio_int_type_t >( gpio::s_config< ARGS_T ... >::M_EVENT ),
            },
        m_pin         { static_cast< gpio_num_t >( gpio::s_config< ARGS_T ... >::M_PIN ) },
        m_mode        { gpio::s_config< ARGS_T ... >::M_MODE },
        m_is_interrupt{ gpio::s_config< ARGS_T ... >::M_EVENT != gpio::EVENT::NONE },
        m_level       { gpio::LEVEL::LOW }
    {
        int error = gpio_config( &m_config );

        if ( ( true == m_is_interrupt ) && ( ESP_OK == error ) )
        {
            error = gpio_set_intr_type( m_pin, static_cast< gpio_int_type_t >( gpio::s_config< ARGS_T ... >::M_EVENT ) );

            if ( ESP_OK == error )
            {
                error = gpio_install_isr_service( ESP_INTR_FLAG_EDGE | ESP_INTR_FLAG_IRAM | ESP_INTR_FLAG_LOWMED );
            }
        }

        if ( ESP_OK != error )
        {
            esp_restart( );
        }
    }


    virtual ~c_gpio( );



/* methods */
    public:
    [[nodiscard( "Do not dvng::c_gpio::get_level result" )]] gpio::LEVEL get_level( );

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
        if ( gpio::LEVEL::LOW == m_level )
        {
            return set_high( );
        }
        else
        {
            return set_low( );
        }
    }


    [[nodiscard( "Do not dvng::c_gpio::register_isr result" )]] int register_isr( gpio::isr_t t_isr, void * t_arguments );

    void deregister_isr( );
};
} /* namespace dvng */


#endif /* ESP_DAVANG_COMPONENTS_DVNG_GPIO_DAVANG_GPIO_H */