/*!
 * \file	davang_gpio.hpp
 * \details UART abstraction class declaration for ESP-IDF.
 * \author	Davang
 * \version	1.0.0
 * \date	2023/09/25
 * \copyright MIT License
 */

#ifndef ESP_DAVANG_COMPONENTS_DVNG_GPIO_DAVANG_UART_H
#define ESP_DAVANG_COMPONENTS_DVNG_GPIO_DAVANG_UART_H

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



/*! Specific davang namespace for uart data types related. */
namespace  dvng::uart
{

/* data types */
using port_t = uint32_t;		/*! \brief data type representing  uart port */

using tx_isr_t = std::function<void(void)>;								/*! \brief data type representing tx empty buffer interrupt */
using rx_isr_t = std::function<void(const void*, uint32_t)>;		/*! \brief data type representing rx empty buffer interrupt */


/* enumerators */
enum class BAUD_RATE : int
{
	BS_2400 = 2400,
	BS_4800 = 4800,
	BS_9600 = 9600,
	BS_19200 = 19200,
	BS_38400 = 38400,
	BS_57600 = 57600,
	BS_115200 = 115200,
};

enum class WORD_LENGTH : uint32_t
{
	BITS_5 = UART_DATA_5_BITS,
	BITS_6 = UART_DATA_6_BITS,
	BITS_7 = UART_DATA_7_BITS,
	BITS_8 = UART_DATA_8_BITS,
	TOTAL,
};

enum class STOP_BITS : uint32_t
{
	STOP_1 = UART_STOP_BITS_1,
	STOP_1_5 = UART_STOP_BITS_1_5,
	STOP_2 = UART_STOP_BITS_2,
	TOTAL,
};

enum class PARITY : uint32_t
{
	NONE = UART_PARITY_DISABLE,
	EVEN = UART_PARITY_EVEN,
	ODD = UART_PARITY_ODD,
	TOTAL,
};

enum class HW_CONTROL : uint32_t
{
	NONE = UART_HW_FLOWCTRL_DISABLE,
	RTS = UART_HW_FLOWCTRL_RTS,
	CTS = UART_HW_FLOWCTRL_CTS,
	RTS_CTS = UART_HW_FLOWCTRL_CTS_RTS,
	TOTAL,
};

enum class INTERRUPT_BUFFER : uint32_t
{
	NONE = 0x00,
	RX_FULL = UART_INTR_RXFIFO_FULL,
	TX_EMPTY = UART_INTR_TXFIFO_EMPTY,
	TX_RX = UART_INTR_TXFIFO_EMPTY | UART_INTR_RXFIFO_FULL,
	TOTAL,
};

/* constants */
constexpr port_t MAX_PORT = UART_NUM_MAX;


/* asssertion structures */
template< port_t T_PORT,
	 BAUD_RATE T_BRATE =  BAUD_RATE::BS_115200,
	 WORD_LENGTH T_WORD_LENGTH =  WORD_LENGTH::BITS_8,
	 PARITY T_PARITY =  PARITY::NONE,
	 STOP_BITS T_STOP_BITS =  STOP_BITS::STOP_1,
	 HW_CONTROL T_HW_CONTROL =  HW_CONTROL::NONE,
	 INTERRUPT_BUFFER T_INTERRUPT =  INTERRUPT_BUFFER::NONE >
struct s_asserter
{

static_assert( ( T_PORT < MAX_PORT ), "UART port not supported" );
static_assert( ( T_WORD_LENGTH < WORD_LENGTH::TOTAL ), "UART word length not supported" );
static_assert( ( T_PARITY < PARITY::TOTAL ), "UART parity not supported" );
static_assert( ( T_STOP_BITS < STOP_BITS::TOTAL ), "UART stop bits not supported" );
static_assert( ( T_HW_CONTROL < HW_CONTROL::TOTAL ), "UART hardware control not supported" );
static_assert( ( T_INTERRUPT < INTERRUPT_BUFFER::TOTAL ), "UART interrupt not supported" );

};


} /* namespace dvng::uart */

/*!< Generic davang namespace */
namespace dvng
{


class c_uart
{
/* constants */
public:

static constexpr size_t BUFFER_SIZE = 512;
static constexpr size_t QUEUE_SIZE = 32;
static constexpr UBaseType_t PRIORITY = tskIDLE_PRIORITY + 1;
static constexpr TickType_t MIN_WAIT = 5 / portTICK_PERIOD_MS;
static constexpr TickType_t MAX_WAIT = 5000 / portTICK_PERIOD_MS;
static constexpr TickType_t DEFAULT_WAIT = 100 / portTICK_PERIOD_MS;

private:
const uart_config_t m_config;
const uart::port_t m_port;
const uart::INTERRUPT_BUFFER m_interrupt;
const bool m_is_interrupt;
const bool m_initalized_on_constructor;

/* data members */
private:

std::atomic<bool> m_should_finish;

TaskHandle_t m_isr_task;

SemaphoreHandle_t m_tx_isr_mutex;
SemaphoreHandle_t m_rx_isr_mutex;

QueueHandle_t m_queue;

uart::tx_isr_t m_tx_isr;
uart::rx_isr_t m_rx_isr;

/* constructors and destructor */
public:

c_uart( ) = delete;

c_uart( const c_uart & ) = delete;

c_uart( const c_uart && ) = delete;

/*!
 * \ brief c_uart constructor 
 */
template< uart::port_t T_PORT,
	uart::BAUD_RATE T_BRATE = uart::BAUD_RATE::BS_115200,
	uart::WORD_LENGTH T_WORD_LENGTH = uart::WORD_LENGTH::BITS_8,
	uart::PARITY T_PARITY = uart::PARITY::NONE,
	uart::STOP_BITS T_STOP_BITS = uart::STOP_BITS::STOP_1,
	uart::HW_CONTROL T_HW_CONTROL = uart::HW_CONTROL::NONE,
	uart::INTERRUPT_BUFFER T_INTERRUPT = uart::INTERRUPT_BUFFER::NONE >
c_uart( const uart::s_asserter< T_PORT, T_BRATE, T_WORD_LENGTH, T_PARITY, T_STOP_BITS, T_HW_CONTROL, T_INTERRUPT > & , const bool t_should_init) :
	m_config {
		.baud_rate = static_cast< int >( T_BRATE ),
		.data_bits = static_cast< uart_word_length_t > ( T_WORD_LENGTH ),
		.parity = static_cast< uart_parity_t > ( T_PARITY ),
		.stop_bits = static_cast< uart_stop_bits_t > ( T_STOP_BITS ),
		.flow_ctrl = static_cast< uart_hw_flowcontrol_t > ( T_HW_CONTROL ),
		.rx_flow_ctrl_thresh = 122,
		.source_clk = UART_SCLK_DEFAULT
	},
	m_port{T_PORT},
	m_interrupt{T_INTERRUPT},
	m_is_interrupt{ ( ( uart::INTERRUPT_BUFFER::NONE < T_INTERRUPT ) && ( uart::INTERRUPT_BUFFER::TOTAL > T_INTERRUPT) ) },
	m_initalized_on_constructor{ t_should_init },
	m_should_finish{true},
	m_isr_task{nullptr},
	m_tx_isr_mutex{nullptr},
	m_rx_isr_mutex{nullptr}
{
	if( true == m_initalized_on_constructor )
	{
		if( ESP_OK != init( ) )
		{
			esp_restart( );
		}
	}
}

template< uart::port_t T_PORT,
	uart::BAUD_RATE T_BRATE = uart::BAUD_RATE::BS_115200,
	uart::WORD_LENGTH T_WORD_LENGTH = uart::WORD_LENGTH::BITS_8,
	uart::PARITY T_PARITY = uart::PARITY::NONE,
	uart::STOP_BITS T_STOP_BITS = uart::STOP_BITS::STOP_1,
	uart::HW_CONTROL T_HW_CONTROL = uart::HW_CONTROL::NONE,
	uart::INTERRUPT_BUFFER T_INTERRUPT = uart::INTERRUPT_BUFFER::NONE >
c_uart( const uart::s_asserter< T_PORT, T_BRATE, T_WORD_LENGTH, T_PARITY, T_STOP_BITS, T_HW_CONTROL, T_INTERRUPT > & t_config ) :
	c_uart( t_config , true )
{
	
}


virtual ~c_uart( );


/* methods */
public:	

	[[nodiscard("Always ensure valid uart initialization")]]
	int init( );

	void deinit( );

	[[nodiscard("Always ensure valid uart initialization")]]
	int send( const void * t_data, size_t & t_length );

	[[nodiscard("Always ensure valid uart initialization")]]
	int receive( void * t_data, size_t & t_length );

	[[nodiscard("Always ensure valid uart initialization")]]
	size_t get_input_data_length( );

	[[nodiscard("Always ensure correct isr registration ")]]
	int register_tx_isr( uart::tx_isr_t t_isr );
	
	[[nodiscard("Always ensure correct isr registration ")]]
	int register_rx_isr( uart::rx_isr_t t_isr );

	void deresgister_rx_isr( );

	void deresgister_tx_isr( );

	inline bool CheckIfShouldFinish ( ) const
	{
		return m_should_finish;
	}

};


} /* namespace dvng */

#endif /* ESP_DAVANG_COMPONENTS_DVNG_GPIO_DAVANG_UART_H */