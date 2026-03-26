/*!
 * \file	davang_uart_stream.hpp
 * \details Davang's custom uart_stream class based on davang_uart.
 * \author	Davang
 * \version	1.0.0
 * \date	2024/07/30
 * \copyright MIT License
 */

#ifndef ESP_DAVANG_COMPONENTS_DVNG_UART_STREAM_DAVANG_UART_STREAM_H
#define ESP_DAVANG_COMPONENTS_DVNG_UART_STREAM_DAVANG_UART_STREAM_H

/* C includes */
#include <cstdint>
#include <cstring>
#include <cstddef>

/* C++ includes */
#include <concepts>
#include <format>
#include <string>

/* 3rd party includes */

/* custom includes*/
#include "davang_uart.hpp"



/*! Specific davang namespace for logging data types related. */
namespace  dvng::uart_stream
{
/* data types */

/* enumerators */
enum class NUM_BASE : unsigned int
{
    HEX = 0,
    DEC
};


enum class END_LINE : std::underlying_type_t < std::byte >
{
    LF = '\n',
    CR = '\r'
};


/* constants */

/* structures */
struct s_bool
{
    std::string m_true_value;
    std::string m_false_value;
};


struct s_dec_precision
{
    size_t m_decimal_length;
};
}


namespace dvng
{
class c_uart_stream
{
/* constant expressions */
    public:
    protected:
    private:

    static constexpr size_t MAX_MSG_SIZE = 256;

    static constexpr size_t DEFAULT_DECIMAL_SIZE = 3;

/* constants */
    public:
    protected:


/* data members */
    public:
    protected:

    uart_stream::NUM_BASE m_numbers_base;

    uart_stream::s_dec_precision m_dec_precision;

    uart_stream::s_bool m_bool_text;

    private:

    c_uart m_uart_port;


    public:


/* constructors and destructor */
    template< auto ... ARGS_T >
    c_uart_stream( ) :
        m_numbers_base { uart_stream::NUM_BASE::DEC },
        m_dec_precision{.m_decimal_length = DEFAULT_DECIMAL_SIZE },
        m_bool_text    {
        .m_true_value{ "0" },
        .m_false_value{ "1" }
        },
        m_uart_port( dvng::uart::s_config< 0 >() )
    {

    }


    c_uart_stream( const c_uart_stream & ) = delete;


    c_uart_stream( const c_uart_stream && ) = delete;


    virtual ~c_uart_stream( );


    c_uart_stream & operator<<( const uart_stream::NUM_BASE & t_pointer );


    c_uart_stream & operator<<( const uart_stream::s_dec_precision & t_pointer );


    c_uart_stream & operator<<( const uart_stream::s_bool & t_pointer );


    c_uart_stream & operator<<( const uart_stream::END_LINE & t_pointer );


    c_uart_stream & operator<<( const void * t_pointer );


    c_uart_stream & operator<<( const char * t_text );


    c_uart_stream & operator<<( const std::string_view & t_text );


    c_uart_stream & operator<<( const bool & t_boolean );


    c_uart_stream & operator<<( const std::integral auto & t_integral )
    {
        if( uart_stream::NUM_BASE::HEX  == m_numbers_base)
        {
            send_to_port( std::format( "{:x}", t_integral ) );
        }
        else
        {
            send_to_port( std::format( "{}", t_integral ) );
        }
        return *this;
    }


    c_uart_stream & operator<<( const std::floating_point auto & t_floatpoint )
    {
        send_to_port( std::format( "{:.{}f}", t_floatpoint, m_dec_precision.m_decimal_length ) );
        return *this;
    }

private:

    void send_to_port( const std::string & t_msg )
    {
        std::array< std::byte, MAX_MSG_SIZE > buffer { std::byte{ 0 } };
        memcpy( buffer.data(), t_msg.c_str(), t_msg.size() );
        (void)m_uart_port.send( buffer );
    }

};
}


#endif /* ESP_DAVANG_COMPONENTS_DVNG_UART_STREAM_DAVANG_UART_STREAM_H */