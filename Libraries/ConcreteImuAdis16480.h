#ifndef CONCRETEIMUADIS16480_H
#define CONCRETEIMUADIS16480_H

#include "Singleton.h"
#include "IImu.h"
#include <SPI.h>
#include "adis16480emu.h"

#include "IPulseConsumer.h"


class ConcreteImuAdis16480 
	: public Singleton<ConcreteImuAdis16480>
	, public IImu
	, public IPulseConsumer	{

	friend class Singleton<ConcreteImuAdis16480>;

	mbed::SPI						_spi;
	adis16480_spi 					_imu;	
	

	ConcreteImuAdis16480();
public:	
	// IImu
	virtual void Initialize();
	virtual bool read_all_data(MpuDataArray* a_res);
	virtual bool WaitForHwReady(uint32_t a_timeoutMs);
	virtual bool IsHwStillReady();
	
	// IPulseConsumer
	virtual void OnRisingFrontDetected();
	virtual void OnFallingFrontDetected();
	};

#endif // CONCRETEIMUADIS16480_H
