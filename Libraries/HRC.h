#ifndef HRC_H
#define HRC_H

#include "Singleton.h"
#include <stdint.h>
#include "stm32f4xx_tim.h"

// High Resolution Clock
class HRC
	: public Singleton<HRC>
	{
	friend class Singleton<HRC>;
	
	// configuration values
	static TIM_TypeDef* HRC_TIMER;
	static uint32_t 	HRC_TIMER_Periph;
	static IRQn_Type 	HRC_TIMER_IRQn;
	
	enum {
		MAIN_FREQ_HZ = 1000000,
		TICKS_PER_MS = MAIN_FREQ_HZ/1000,
		RELOAD_EVERY_MS = 60,
		};
	
	// calculation mechanics
	mutable volatile uint32_t 	_overflowCount;

	HRC();
		
public:
	void InitHardware();
	double GetSeconds() const;
	void OnTimerOverflow();
	};
	
double SystemTime();
	
#endif // HRC_H
