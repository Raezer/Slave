#include "stm32f4xx.h"
#include "AdisRecv.h"
#include "Config.h"
#include "main.h"

int main(void){
	SystemInit();
	init();
	Config_NVIC();
	while(1) 
	{
		 
	}
}
