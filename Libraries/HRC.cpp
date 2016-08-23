#include "HRC.h"
#include "stm32f4xx_rcc.h"

#include "AtomicOps.h"

TIM_TypeDef* HRC::HRC_TIMER = TIM9;
uint32_t HRC::HRC_TIMER_Periph = RCC_APB2Periph_TIM9;
IRQn_Type HRC::HRC_TIMER_IRQn = TIM1_BRK_TIM9_IRQn;

HRC::HRC()
	: _overflowCount(0)
	{}
	
void HRC::InitHardware() {
	::RCC_APB2PeriphClockCmd(HRC_TIMER_Periph, ENABLE);
	
	TIM_TimeBaseInitTypeDef timeBaseInit;
	timeBaseInit.TIM_Prescaler = SystemCoreClock/MAIN_FREQ_HZ-1;
	timeBaseInit.TIM_CounterMode = TIM_CounterMode_Up;
	timeBaseInit.TIM_Period = RELOAD_EVERY_MS*TICKS_PER_MS;
	timeBaseInit.TIM_ClockDivision = TIM_CKD_DIV1;
	::TIM_TimeBaseInit(HRC_TIMER, &timeBaseInit);
	
	::TIM_ITConfig(HRC_TIMER, TIM_IT_Update, ENABLE);
	
	// set timer interrupt the highest possible priority
	// so as to ensure that overflow counter is always
	// correctly reflects actual overflow count 
	::NVIC_SetPriority(HRC_TIMER_IRQn,3);
	::NVIC_EnableIRQ(HRC_TIMER_IRQn);
	
	::TIM_Cmd(HRC_TIMER, ENABLE); 
	};

double HRC::GetSeconds() const {
	uint32_t ovflCount_stable;
	double res;
	
	do {
		ovflCount_stable = _overflowCount;
		
		double secondsViaOvfl = 
			static_cast<double>(ovflCount_stable)*RELOAD_EVERY_MS/1000;
		double secondsViaCntrVal = 
			static_cast<double>(::TIM_GetCounter(HRC_TIMER))/MAIN_FREQ_HZ;
			
		res = secondsViaOvfl + secondsViaCntrVal;
		
		// make sure _overflowCount haven't changed while
		// we were reading timer register and calculating
		} while(!::CompareExchange(&_overflowCount, ovflCount_stable, ovflCount_stable));
	
	return res;
	}

void HRC::OnTimerOverflow() {
	if (::TIM_GetFlagStatus(HRC_TIMER, TIM_FLAG_Update) == SET) {
		::TIM_ClearFlag(HRC_TIMER, TIM_FLAG_Update);
		::AtomicAddAndFetch(&_overflowCount, 1);
		}
	}

extern "C" {

void TIM1_BRK_TIM9_IRQHandler() {
	HRC::Instance()->OnTimerOverflow();
	}

}

double SystemTime() {
	return HRC::Instance()->GetSeconds();
	}
