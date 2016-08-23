#ifndef IIMU_H
#define IIMU_H

#include "MpuDataArray.h"


struct IImu {
	virtual void Initialize() = 0;
	virtual bool read_all_data(MpuDataArray* a_res) = 0;
	
	virtual bool WaitForHwReady(uint32_t a_timeoutMs) = 0;
	virtual bool IsHwStillReady() = 0;
	};

#endif // IIMU_H
