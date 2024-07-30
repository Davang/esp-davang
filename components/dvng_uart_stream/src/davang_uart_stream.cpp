

#include "davang_uart_stream.hpp"

namespace dvng
{


c_uart_stream::c_uart_stream( )
{

}


c_uart_stream::~c_uart_stream( )
{

}


c_uart_stream & c_uart_stream::operator<<( const uart_stream::BIN_FMT & t_bin_fmt )
{

	return *this;
}


c_uart_stream & c_uart_stream::operator<<( const uart_stream::HEX_FMT & t_hex_fmt )
{

	return *this;
}


c_uart_stream & c_uart_stream::operator<<( const uart_stream::SEP_FMT & t_dec_fmt )
{

	return *this;
}


c_uart_stream & c_uart_stream::operator<<( const uart_stream::FMT & t_fmt )
{

	return *this;
}


c_uart_stream & c_uart_stream::operator<<( const uart_stream::s_bool_fmt & t_bol_fmt )
{

	return *this;
}


c_uart_stream & c_uart_stream::operator<<( const uart_stream::s_float_point_fmt & t_float_point_fmt )
{

	return *this;
}


c_uart_stream & c_uart_stream::operator<<( const void * Text )
{

	return *this;
}


c_uart_stream & c_uart_stream::operator<<( const char * Text )
{

	return *this;
}


c_uart_stream & c_uart_stream::operator<<( const std::string & Text )
{

	return *this;
}


c_uart_stream & c_uart_stream::operator<<( const bool & Text )
{

	return *this;
}


c_uart_stream & c_uart_stream::operator<<( const std::integral auto & Text )
{

	return *this;
}


c_uart_stream & c_uart_stream::operator<<( const std::floating_point auto & Text )
{

	return *this;
}

}