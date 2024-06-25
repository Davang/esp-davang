#include <array>
#include "davang_gpio.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern "C" void app_main()
{
	std::array<dvng::c_gpio, 2> output_pins {
		dvng::c_gpio( dvng::gpio::s_asserter< 18, dvng::gpio::MODE::OUTPUT >( ) ),
		dvng::c_gpio( dvng::gpio::s_asserter< 41, dvng::gpio::MODE::OUTPUT >( ) )
	};
	
	dvng::c_gpio input_pin = dvng::c_gpio( dvng::gpio::s_asserter<35, dvng::gpio::MODE::INPUT >( ) );
	
	int error = input_pin.init();
	for ( dvng::c_gpio & pin : output_pins )
	{
		error = pin.init();
	}

	unsigned int i = 0;

	while( ESP_OK == error )
	{
		output_pins.at(0).toggle( );


		++i;
		if( 0 == ( i%5 ) )
		{
			output_pins.at(1).set_level( input_pin.get_level( ) );
		}

		vTaskDelay( 400 / portTICK_PERIOD_MS);
	}
}