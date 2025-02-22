#include <array>
#include "davang_gpio.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern "C" void app_main()
{
	std::array<dvng::c_gpio, 3> output_pins {
		dvng::c_gpio( dvng::gpio::s_config< 19, dvng::gpio::MODE::OUTPUT >( ) ),
		dvng::c_gpio( dvng::gpio::s_config< 20, dvng::gpio::MODE::OUTPUT >( ) ),
		dvng::c_gpio( dvng::gpio::s_config< 21, dvng::gpio::MODE::OUTPUT >( ) )
	};
	
	dvng::c_gpio input_pin = dvng::c_gpio( dvng::gpio::s_config<35, 
		dvng::gpio::MODE::INPUT, 
		dvng::gpio::PULL_UP::ACTIVE,
		dvng::gpio::PULL_DOWN::ACTIVE, 
		dvng::gpio::INTERRUPT_CHANGE::RAISE
		>( ) );
	
	unsigned int i = 0;

	while( true )
	{
		output_pins.at(0).toggle( );
		++i;
		if( 0 == ( i%5 ) )
		{
			output_pins.at(2).toggle( );
			output_pins.at(1).set_level( input_pin.get_level( ) );
		}

		vTaskDelay( 100 / portTICK_PERIOD_MS);
	}
}