/*!
 * \file	davang_uart_stream.cpp
 * \details Davang's custom uart_stream class based on davang_uart.
 * \author	Davang
 * \version	1.0.0
 * \date	2024/07/30
 * \copyright MIT License
 */
/* C includes */
#include <cstdio>

/* C++ includes */
#include <utility>

/* 3rd party includes */

/* custom includes*/
#include "davang_uart_stream.hpp"


namespace dvng
{
c_uart_stream::~c_uart_stream( )
{
    // do nohting as uart port is initaizlized on constructo destructor shall deinitalize it.
}



c_uart_stream & c_uart_stream::operator<<( const uart_stream::NUM_BASE & t_numbers_base )
{
    m_numbers_base = t_numbers_base;
    return *this;
}



c_uart_stream & c_uart_stream::operator<<( const uart_stream::s_dec_precision & t_dec_precision )
{
    m_dec_precision = t_dec_precision;
    return *this;
}



c_uart_stream & c_uart_stream::operator<<( const uart_stream::s_bool & t_bool_text )
{
    m_bool_text = t_bool_text;
    return *this;
}



c_uart_stream & c_uart_stream::operator<<( const uart_stream::END_LINE & t_end_line )
{
    send_to_port( std::string{ std::to_underlying(t_end_line) });
    return *this;
}



c_uart_stream & c_uart_stream::operator<<( const void * t_pointer )
{
    send_to_port(std::format("{:p}", t_pointer));
    return *this;
}



c_uart_stream & c_uart_stream::operator<<( const char * t_text )
{
    send_to_port( t_text );
    return *this;
}



c_uart_stream & c_uart_stream::operator<<( const std::string_view & t_text )
{
    send_to_port( t_text.data() );
    return *this;
}



c_uart_stream & c_uart_stream::operator<<( const bool & t_boolean )
{
    if ( true == t_boolean )
    {
        send_to_port( m_bool_text.m_true_value );
    }
    else
    {
        send_to_port( m_bool_text.m_false_value );
    }

    return *this;
}

}