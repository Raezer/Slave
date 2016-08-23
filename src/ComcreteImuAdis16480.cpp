#include "ConcreteImuAdis16480.h"
#include <stdint.h>
#include <new>
#include "HRC.h"

	
static const uint32_t ADIS16480_WHO_AM_I = 16480;
	
ConcreteImuAdis16480::ConcreteImuAdis16480() 
	: _spi(PC_3, PC_2, PB_10)	//define the SPI (mosi, miso, sclk)
	, _imu(_spi,PB_9)   		//define the Adis16480 object

	{}
		
void ConcreteImuAdis16480::Initialize() {
	// wait 1.6 sec (if needed) to give ADIS time 
	// to initialize internally
	while (::SystemTime() < 1.6){};
		
	if(!_imu.init())
	{
	}
		
	if (_imu.whoami() == ADIS16480_WHO_AM_I) //тут нато поставить потом !=
	{
	}
		
	
	}

bool ConcreteImuAdis16480::read_all_data(MpuDataArray* a_res) { 
	while(1) {
		_imu.read_all_mpu_data(a_res);
		
		if (!_imu.need_restart())
			return true;
			
		if (!_imu.restart())
//		if (!_imu._self_test())
			return false;
		};
	}



bool ConcreteImuAdis16480::WaitForHwReady(uint32_t a_timeoutMs) {
//	if (!_dataReadySignalOp.IsHwBusyBitSet())
//		return true;

	double time = ::SystemTime();
	
	
		
	a_timeoutMs -= (::SystemTime() - time)*1000;
		
	return 0;
	}

bool ConcreteImuAdis16480::IsHwStillReady() {
	return 0;
	}

void ConcreteImuAdis16480::OnRisingFrontDetected() {
	// IRQ on detection of rising front of HwBusy signal
	
	}

void ConcreteImuAdis16480::OnFallingFrontDetected() {
	// IRQ on detection of falling front of HwBusy signal
	
	}




