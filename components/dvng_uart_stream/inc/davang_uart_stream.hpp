/*!
 * \file	davang_uart_stream.hpp
 * \details Davang's custom uart_stream class based on davang_uart.
 * \author	Davang
 * \version	1.0.0
 * \date	2024/07/30
 * \copyright MIT License
 */

#ifndef ESP_DAVANG_COMPONENTS_DVNG_GPIO_DAVANG_UART_H

/* C includes */

/* C++ includes */

/* 3rd party includes */

/* custom includes*/
#include "davang_uart.hpp"

/*! Specific davang namespace for logging data types related. */
namespace  dvng::uart_stream
{
/* data types */

/* enumerators */
enum class BIN_FMT : unsigned int
{
	SEPARATOR_LOWER = 0,
	SEPARATOR_UPPER,
	NO_SEPARATOR,
};

enum class HEX_FMT : unsigned int
{
	SEPARATOR_LOWER = 0,
	SEPARATOR_UPPER,
	NO_SEPARATOR_LOWER,
	NO_SEPARATOR_UPPER,
};

enum class SEP_FMT : unsigned int
{
	POINT = 0,
	COMMA,
	CUSTOM,
};

enum class FMT : unsigned int
{
	RESET = 0,
	ENDL,
	FLUSH,
	HEX,
	DEC,
	BIN,
	LF,
	CR,
	CR_LF,
	ADDRS,
};


/* constants */

/* structures */

struct s_bool_fmt
{
	std::string m_true_value;
	std::string m_false_value;
};

struct s_float_point_fmt
{
	size_t m_whole_length;
	size_t m_fractional_length;
	SEP_FMT m_decimal_separator_type; 
	char m_decimal_separator;
};



}

namespace dvng
{


class c_uart_stream
{
/* constants */
public:

/* data members */

private:

uart_stream::BIN_FMT           m_bin_fmt;
uart_stream::HEX_FMT           m_hex_fmt;
uart_stream::SEP_FMT           m_decimal_separator;
uart_stream::FMT               m_fmt;
uart_stream::s_bool_fmt        m_bool_fmt;
uart_stream::s_float_point_fmt m_float_point_fmt;
char m_decimal_separator;
public:

/* constructors and destructor */
c_uart_stream( ) ;


c_uart_stream( const c_uart_stream & ) = delete;


c_uart_stream( const c_uart_stream && ) = delete;


virtual ~c_uart_stream( );


c_uart_stream & operator<<( const BIN_FMT & t_bin_fmt );


c_uart_stream & operator<<( const HEX_FMT & t_hex_fmt );


c_uart_stream & operator<<( const SEP_FMT & t_dec_fmt );


c_uart_stream & operator<<( const FMT & t_fmt );


c_uart_stream & operator<<( const s_bool_fmt & t_bol_fmt );


c_uart_stream & operator<<( const s_float_point_fmt & t_float_point_fmt );


c_uart_stream & operator<<( const void * Text );


c_uart_stream & operator<<( const char * Text );


c_uart_stream & operator<<( const std::string & Text );


c_uart_stream & operator<<( const bool & Text );


c_uart_stream & operator<<( const std::integral auto & Text );


c_uart_stream & operator<<( const std::floating_point auto & Text );


};



}