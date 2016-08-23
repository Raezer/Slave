#ifndef IPULSECONSUMER_H
#define IPULSECONSUMER_H

#include "defines.h"

interface IPulseConsumer {
	virtual void OnRisingFrontDetected() = 0;
	virtual void OnFallingFrontDetected() = 0;
	};
	
#endif // IPULSECONSUMER_H
