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

/* C++ includes */
#include <concepts>
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
	DEC,
};

enum class END_LINE : unsigned int
{
	LF = 0,
	CR,
	CR_LF,
	LF_CR,
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

template< uart::port_t T_PORT >
struct s_uart_port
{
	static constexpr uart::port_t PORT = T_PORT;
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
template< uart::port_t T_PORT >
c_uart_stream( const uart_stream::s_uart_port< T_PORT > & t_port ) : m_numbers_base{ uart_stream::NUM_BASE::DEC },
	m_dec_precision{ 
		.m_decimal_length = DEFAULT_DECIMAL_SIZE,
	},
	m_bool_text{
		.m_true_value{"0"},
		.m_false_value{"1"}
	},
	m_uart_port{ uart::s_asserter< T_PORT >( ) }
{

}


c_uart_stream( const c_uart_stream & ) = delete;


c_uart_stream( const c_uart_stream && ) = delete;


virtual ~c_uart_stream( );

c_uart_stream & operator<<( const uart_stream::END_LINE & t_pointer );

c_uart_stream & operator<<( const uart_stream::NUM_BASE & t_pointer );


c_uart_stream & operator<<( const uart_stream::s_dec_precision & t_pointer );


c_uart_stream & operator<<( const uart_stream::s_bool & t_pointer );


c_uart_stream & operator<<( const void * t_pointer );


c_uart_stream & operator<<( const char * t_text );


c_uart_stream & operator<<( const char t_text );


c_uart_stream & operator<<( const std::string & t_text );


c_uart_stream & operator<<( const bool & t_boolean );


c_uart_stream & operator<<( const size_t & t_size );


c_uart_stream & operator<<( const ssize_t & t_size );


c_uart_stream & operator<<( const std::unsigned_integral auto & t_integral )
{
	char msg_buffer[ MAX_MSG_SIZE ] = {0};
	std::string format_size = "%";

	if constexpr ( sizeof( char ) == sizeof( t_integral ) )
	{
		format_size += "hh"; 
	}
	else if constexpr ( sizeof( uint16_t ) == sizeof( t_integral ) )
	{
		format_size += "h";
	}
	else if constexpr ( sizeof( uint32_t ) == sizeof( t_integral ) )
	{
		format_size += "l";
	}
	else if constexpr ( sizeof( uint64_t ) == sizeof( t_integral ) )
	{
		format_size += "ll"; 
	}
	else
	{
		// do nothing
	}

	if( uart_stream::NUM_BASE::DEC == m_numbers_base )
	{
		format_size += "u";
	}
	else if( uart_stream::NUM_BASE::HEX == m_numbers_base )
	{
		format_size += "x";
	}
	else
	{

	}

	(void)snprintf(msg_buffer, MAX_MSG_SIZE, format_size.c_str(), t_integral ); 
	(void) m_uart_port.send( msg_buffer, strlen(msg_buffer) );
	return *this;
}


c_uart_stream & operator<<( const std::signed_integral auto & t_integral )
{
	char msg_buffer[ MAX_MSG_SIZE ] = {0};
	if constexpr ( sizeof( int8_t ) == sizeof( t_integral ) )
	{
		(void)snprintf(msg_buffer, MAX_MSG_SIZE, "%hhd", t_integral ); 
	}
	else if constexpr ( sizeof( int16_t ) == sizeof( t_integral ) )
	{
		(void)snprintf(msg_buffer, MAX_MSG_SIZE, "%hd", t_integral ); 
	}
	else if constexpr ( sizeof( int32_t ) == sizeof( t_integral ) )
	{
		(void)snprintf(msg_buffer, MAX_MSG_SIZE, "%ld", t_integral ); 
	}
	else if constexpr ( sizeof( int64_t ) == sizeof( t_integral ) )
	{
		(void)snprintf(msg_buffer, MAX_MSG_SIZE, "%lld", t_integral ); 
	}
	else
	{
		// do nothing
	}
	
	(void) m_uart_port.send( msg_buffer, strlen(msg_buffer) );
	return *this;
}


c_uart_stream & operator<<( const std::floating_point auto & t_floatpoint )
{
	char msg_buffer[ MAX_MSG_SIZE ] = {0};
	char float_format [MAX_MSG_SIZE] = {0};
	if constexpr( sizeof( float ) == sizeof( t_floatpoint ) )
	{
		(void)snprintf( float_format, MAX_MSG_SIZE, "%%.%df", m_dec_precision.m_decimal_length );
	}
	else if constexpr( sizeof( double ) == sizeof( t_floatpoint ) )
	{
		(void)snprintf( float_format, MAX_MSG_SIZE, "%%.%dlf", m_dec_precision.m_decimal_length );
	}
	else if constexpr ( sizeof( long double ) == sizeof( t_floatpoint ) )
	{
		(void)snprintf( float_format, MAX_MSG_SIZE, "%%.%dLf", m_dec_precision.m_decimal_length );
	}
	else
	{
		// do nothing
	}
	
	(void)snprintf(msg_buffer, MAX_MSG_SIZE, float_format, t_floatpoint ); 
	(void) m_uart_port.send( msg_buffer, strlen(msg_buffer) );
	return *this;
}


};



}

#endif /* ESP_DAVANG_COMPONENTS_DVNG_UART_STREAM_DAVANG_UART_STREAM_H */