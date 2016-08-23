#ifndef CONCRETEIMUADIS16480_H
#define CONCRETEIMUADIS16480_H


#include <SPI.h>
#include "SPI_ex.h"



class ConcreteImuAdis16480 
{
	mbed::SPI						_spi;
	adis16480_spi 					_imu;	
	ConcreteImuAdis16480();
public:	
	// IImu
	virtual void Initialize() ;
	virtual bool read_all_data();
	
	};

#endif // CONCRETEIMUADIS16480_H
