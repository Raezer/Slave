#include "ConcreteImuAdis.h"
#include <stdint.h>



	
static const uint32_t ADIS16480_WHO_AM_I = 16480;
	
ConcreteImuAdis16480::ConcreteImuAdis16480() 
	: _spi(PC_3, PC_2, PB_10)	//define the SPI (mosi, miso, sclk)
	, _imu(_spi,PB_5)   		//define the Adis16480 object
	{}
		
void ConcreteImuAdis16480::Initialize() {
	// wait 1.6 sec (if needed) to give ADIS time 
	// to initialize internally

		
	if(!_imu.init())
	{
	}
		
	if (_imu.whoami() == ADIS16480_WHO_AM_I) //тут нато поставить потом !=
	{
	}
		

	}

bool ConcreteImuAdis16480::read_all_data() { 
	while(1) {
		
		
		if (!_imu.need_restart())
			return true;
			
		if (!_imu.restart())
//		if (!_imu._self_test())
			return false;
		};
	}




