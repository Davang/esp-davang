/*!
 * \file	davang_gpio.hpp
 * \details UART abstraction class declaration for ESP-IDF.
 * \author	Davang
 * \version	1.0.0
 * \date	2023/09/25
 * \copyright MIT License
 */

#ifndef ESP_DAVANG_COMPONENTS_DVNG_UART_DAVANG_UART_H
#define ESP_DAVANG_COMPONENTS_DVNG_UART_DAVANG_UART_H

/* C includes */
#include <cstdint>
#include <cassert>

/* C++ includes */
#include <atomic>
#include <utility>
#include <functional>

/* 3rd party includes */
#include "driver/uart.h"
#include "hal/uart_ll.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "freertos/semphr.h"


/* custom includes*/
#include "davang_gpio.hpp"


/*! Specific davang namespace for uart data types related. */
namespace  dvng::uart
{
/* data types */
using port_t = uint32_t; /*! \brief data type representing  uart port */

/* enumerators */
enum class BAUD_RATE : int
{
    BS_2400   = 2400,
    BS_4800   = 4800,
    BS_9600   = 9600,
    BS_19200  = 19200,
    BS_38400  = 38400,
    BS_57600  = 57600,
    BS_115200 = 115200
};


enum class WORD_LENGTH : uint32_t
{
    BITS_5 = UART_DATA_5_BITS,
    BITS_6 = UART_DATA_6_BITS,
    BITS_7 = UART_DATA_7_BITS,
    BITS_8 = UART_DATA_8_BITS,
    TOTAL
};


enum class STOP_BITS : uint32_t
{
    STOP_1   = UART_STOP_BITS_1,
    STOP_1_5 = UART_STOP_BITS_1_5,
    STOP_2   = UART_STOP_BITS_2,
    TOTAL
};


enum class PARITY : uint32_t
{
    NONE = UART_PARITY_DISABLE,
    EVEN = UART_PARITY_EVEN,
    ODD  = UART_PARITY_ODD,
    TOTAL
};


enum class HW_CONTROL : uint32_t
{
    NONE    = UART_HW_FLOWCTRL_DISABLE,
    RTS     = UART_HW_FLOWCTRL_RTS,
    CTS     = UART_HW_FLOWCTRL_CTS,
    RTS_CTS = UART_HW_FLOWCTRL_CTS_RTS,
    TOTAL
};


/* constants */
constexpr port_t MAX_PORT = UART_NUM_MAX;


/* asssertion structures */
template< port_t T_PORT,
  dvng::uart::BAUD_RATE T_BRATE = BAUD_RATE::BS_115200,
  dvng::uart::WORD_LENGTH T_WORD_LENGTH = WORD_LENGTH::BITS_8,
  dvng::uart::PARITY T_PARITY = PARITY::NONE,
  dvng::uart::STOP_BITS T_STOP_BITS = STOP_BITS::STOP_1,
  dvng::uart::HW_CONTROL T_HW_CONTROL = HW_CONTROL::NONE
  >
struct s_config
{
    static constexpr dvng::uart::port_t      M_PORT        = T_PORT;
    static constexpr dvng::uart::BAUD_RATE   M_BRATE       = T_BRATE;
    static constexpr dvng::uart::WORD_LENGTH M_WORD_LENGTH = T_WORD_LENGTH;
    static constexpr dvng::uart::PARITY      M_PARITY      = T_PARITY;
    static constexpr dvng::uart::STOP_BITS   M_STOP_BITS   = T_STOP_BITS;
    static constexpr dvng::uart::HW_CONTROL  M_HW_CONTROL  = T_HW_CONTROL;

    // static

    static_assert( ( T_PORT < MAX_PORT ), "UART port not supported" );
    static_assert( ( T_WORD_LENGTH < WORD_LENGTH::TOTAL ), "UART word length not supported" );
    static_assert( ( T_PARITY < PARITY::TOTAL ), "UART parity not supported" );
    static_assert( ( T_STOP_BITS < STOP_BITS::TOTAL ), "UART stop bits not supported" );
    static_assert( ( T_HW_CONTROL < HW_CONTROL::TOTAL ), "UART hardware control not supported" );
};
} /* namespace dvng::uart */


/*!< Generic davang namespace */
namespace dvng
{
class c_uart
{
/* constant expressions */
    public:

    static constexpr size_t BUFFER_SIZE = 1024;
    static constexpr size_t QUEUE_SIZE  = 32;

    protected:
    static constexpr UBaseType_t PRIORITY     = tskIDLE_PRIORITY + 1;
    static constexpr TickType_t  MIN_WAIT     = 5 / portTICK_PERIOD_MS;
    static constexpr TickType_t  MAX_WAIT     = 20'000 / portTICK_PERIOD_MS;
    static constexpr TickType_t  DEFAULT_WAIT = 100 / portTICK_PERIOD_MS;

    private:

/* constants*/

    private:

    const uart_config_t m_config;
    const uart_port_t   m_port;

/* data members */
    private:

    QueueHandle_t m_queue;

/* constructors and destructor */
    public:


    c_uart( ) = delete;


    c_uart( const c_uart & ) = delete;


    c_uart( const c_uart && ) = delete;


/*!
 * \ brief c_uart constructor
 */
    template< auto ... ARGS_T >
    c_uart( const uart::s_config< ARGS_T ... > & t_config ) :
        m_config{
        .baud_rate           = static_cast< int >( uart::s_config< ARGS_T ... >::M_BRATE ),
        .data_bits           = static_cast< uart_word_length_t >( uart::s_config< ARGS_T ... >::M_WORD_LENGTH ),
        .parity              = static_cast< uart_parity_t >( uart::s_config< ARGS_T ... >::M_PARITY ),
        .stop_bits           = static_cast< uart_stop_bits_t >( uart::s_config< ARGS_T ... >::M_STOP_BITS ),
        .flow_ctrl           = static_cast< uart_hw_flowcontrol_t >( uart::s_config< ARGS_T ... >::M_HW_CONTROL ),
        .rx_flow_ctrl_thresh = 122,
        .source_clk          = UART_SCLK_DEFAULT,
        .flags               = { 0, 0 },
        },
        m_port{ static_cast< uart_port_t >( uart::s_config< ARGS_T ... >::M_PORT ) }
    {
        int error = uart_driver_install( static_cast< uart_port_t >( m_port ), BUFFER_SIZE, BUFFER_SIZE, QUEUE_SIZE, &m_queue, 0 );

        if ( ESP_OK == error )
        {
            error = uart_param_config( static_cast< uart_port_t >( m_port ), &m_config );
        }

        if ( ESP_OK == error )
        {
            error = uart_set_pin( static_cast< uart_port_t >( m_port ), UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE );
        }
        else
        {
            esp_restart();
        }
    }


    virtual ~c_uart( );


/* methods */
    public:

    [[nodiscard("")]] int send( const void * t_data, size_t & t_length );


    [[nodiscard("")]] int send( const void * t_data, const size_t & t_length );


    [[nodiscard("")]] int receive( void * t_data, size_t & t_length );
};
} /* namespace dvng */


#endif /* ESP_DAVANG_COMPONENTS_DVNG_UART_DAVANG_UART_H */