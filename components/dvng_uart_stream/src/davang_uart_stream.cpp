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

/* 3rd party includes */

/* custom includes*/
#include "davang_uart_stream.hpp"


namespace dvng
{
c_uart_stream::~c_uart_stream( )
{
    // do nohting as uart port is initaizlized on constructo destructor shall deinitalize it.
}



c_uart_stream & c_uart_stream::operator<<( const uart_stream::END_LINE & t_end_line )
{
    char end_line[ MAX_MSG_SIZE ] = { 0 };

    switch ( t_end_line )
    {
        case uart_stream::END_LINE::LF:
        {
            snprintf( end_line, MAX_MSG_SIZE, "\n" );
            break;
        }

        case uart_stream::END_LINE::CR:
        {
            snprintf( end_line, MAX_MSG_SIZE, "\r" );
            break;
        }

        case uart_stream::END_LINE::CR_LF:
        {
            snprintf( end_line, MAX_MSG_SIZE, "\r\n" );
            break;
        }

        case uart_stream::END_LINE::LF_CR:
        {
            snprintf( end_line, MAX_MSG_SIZE, "\n\r" );
            break;
        }
    }

    ( void )m_uart_port.send( &end_line, strlen( end_line ) );

    return *this;
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



c_uart_stream & c_uart_stream::operator<<( const void * t_pointer )
{
    char msg_buffer[ MAX_MSG_SIZE ];

    ( void )snprintf( msg_buffer, MAX_MSG_SIZE, "%p", t_pointer );

    ( void )m_uart_port.send( msg_buffer, strlen( msg_buffer ) );

    return *this;
}



c_uart_stream & c_uart_stream::operator<<( const char * t_text )
{
    ( void )m_uart_port.send( t_text, strlen( t_text ) );
    return *this;
}



c_uart_stream & c_uart_stream::operator<<( const char t_text )
{
    ( void )m_uart_port.send( &t_text, 1 );
    return *this;
}



c_uart_stream & c_uart_stream::operator<<( const std::string & t_text )
{
    ( void )m_uart_port.send( t_text.c_str(), t_text.size( ) );
    return *this;
}



c_uart_stream & c_uart_stream::operator<<( const bool & t_boolean )
{
    if ( true == t_boolean )
    {
        ( void )m_uart_port.send( m_bool_text.m_true_value.c_str(), m_bool_text.m_true_value.size( ) );
    }
    else
    {
        ( void )m_uart_port.send( m_bool_text.m_false_value.c_str(), m_bool_text.m_false_value.size( ) );
    }

    return *this;
}



c_uart_stream & c_uart_stream::operator<<( const size_t & t_size )
{
    char msg_buffer[ MAX_MSG_SIZE ] = { 0 };

    ( void )snprintf( msg_buffer, MAX_MSG_SIZE, "%zu", t_size );
    ( void )m_uart_port.send( msg_buffer, strlen( msg_buffer ) );
    return *this;
}



c_uart_stream & c_uart_stream::operator<<( const ssize_t & t_size )
{
    char msg_buffer[ MAX_MSG_SIZE ] = { 0 };

    ( void )snprintf( msg_buffer, MAX_MSG_SIZE, "%zd", t_size );
    ( void )m_uart_port.send( msg_buffer, strlen( msg_buffer ) );
    return *this;
}
}