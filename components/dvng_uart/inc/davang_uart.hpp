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
#include <cstddef>

/* C++ includes */
#include <atomic>
#include <array>
#include <tuple>
#include <utility>
#include <functional>

/* 3rd party includes */
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "esp_err.h"

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
    BR_2400   = 2400,
    BR_4800   = 4800,
    BR_9600   = 9600,
    BR_19200  = 19200,
    BR_38400  = 38400,
    BR_57600  = 57600,
    BR_115200 = 115200
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

constexpr dvng::gpio::pin_t DEFAULT_PIN = UART_PIN_NO_CHANGE;


/* asssertion structures */
template< port_t T_PORT,
  dvng::uart::BAUD_RATE T_BRATE = BAUD_RATE::BR_115200,
  dvng::uart::WORD_LENGTH T_WORD_LENGTH = WORD_LENGTH::BITS_8,
  dvng::uart::PARITY T_PARITY = PARITY::NONE,
  dvng::uart::STOP_BITS T_STOP_BITS = STOP_BITS::STOP_1,
  dvng::uart::HW_CONTROL T_HW_CONTROL = HW_CONTROL::NONE,
  dvng::gpio::pin_t T_TX_PIN = DEFAULT_PIN,
  dvng::gpio::pin_t T_RX_PIN = DEFAULT_PIN,
  dvng::gpio::pin_t T_RTS_PIN = DEFAULT_PIN,
  dvng::gpio::pin_t T_CTS_PIN = DEFAULT_PIN
  >
struct s_config
{
    static constexpr dvng::uart::port_t      M_PORT        = T_PORT;
    static constexpr dvng::uart::BAUD_RATE   M_BRATE       = T_BRATE;
    static constexpr dvng::uart::WORD_LENGTH M_WORD_LENGTH = T_WORD_LENGTH;
    static constexpr dvng::uart::PARITY      M_PARITY      = T_PARITY;
    static constexpr dvng::uart::STOP_BITS   M_STOP_BITS   = T_STOP_BITS;
    static constexpr dvng::uart::HW_CONTROL  M_HW_CONTROL  = T_HW_CONTROL;

    static constexpr dvng::gpio::s_pin_mode_config< T_TX_PIN, dvng::gpio::MODE::OUTPUT > TX{};
    static constexpr dvng::gpio::s_pin_mode_config< T_RX_PIN, dvng::gpio::MODE::INPUT >  RX{};

    static constexpr dvng::gpio::s_pin_mode_config< T_RTS_PIN, dvng::gpio::MODE::OUTPUT > RTS{};
    static constexpr dvng::gpio::s_pin_mode_config< T_CTS_PIN, dvng::gpio::MODE::INPUT >  CTS{};

    static constexpr dvng::gpio::pin_t M_TX_PIN{ T_TX_PIN };
    static constexpr dvng::gpio::pin_t M_RX_PIN{ T_RX_PIN };

    static constexpr dvng::gpio::pin_t M_RTS_PIN{ T_RTS_PIN };
    static constexpr dvng::gpio::pin_t M_CTS_PIN{ T_CTS_PIN };


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

/* constants*/

    private:

    const uart_config_t m_config;
    const uart_port_t   m_port;

/* data members */

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
        int error = uart_driver_install( m_port, 1024, 0, 0, NULL, 0 );

        if ( ESP_OK == error )
        {
            error = uart_param_config( m_port, &m_config );
        }

        if ( ESP_OK == error )
        {
            using uart_config_t = uart::s_config< ARGS_T ... >;
            error               = uart_set_pin( m_port, uart_config_t::M_TX_PIN, uart_config_t::M_RX_PIN, uart_config_t::M_RTS_PIN, uart_config_t::M_CTS_PIN );
        }
        else
        {
            esp_restart();
        }
    }


    virtual ~c_uart( );


/* methods */
    public:

    template< size_t N >
    [[nodiscard("Do not dvng::c_uart::send result")]] std::tuple<int, ssize_t> send( const std::array< std::byte, N > & t_data )
    {
        ssize_t size = N;
        int error   = uart_write_bytes( m_port, t_data.data(), t_data.size() );

        if ( ESP_FAIL == error )
        {
            size = 0;
        }
        else
        {
            size  = static_cast< ssize_t >( error );
            error = ESP_OK;
        }

        return std::tie(error, size);
    }

    template< size_t N >
    [[nodiscard("Do not dvng::c_uart::receive result")]] inline std::tuple<int, ssize_t> receive( std::array< std::byte, N > & t_data  )
    {
        return receive( 100 / portTICK_PERIOD_MS, t_data);
    }

    template< size_t N >
    [[nodiscard("Do not dvng::c_uart::receive result")]] std::tuple<int, ssize_t> receive( TickType_t t_timeout, std::array< std::byte, N > & t_data  )
    {
        ssize_t size = N;
        int error = uart_read_bytes( m_port, t_data.data(), t_data.size(), t_timeout );

        if ( ESP_FAIL == error )
        {
            size = 0;
        }
        else
        {
            size  = static_cast< ssize_t >( error );
            error = ESP_OK;
        }

        return std::tie(error, size);
    }

};
} /* namespace dvng */


#endif /* ESP_DAVANG_COMPONENTS_DVNG_UART_DAVANG_UART_H */