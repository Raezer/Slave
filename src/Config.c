#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"
#include "misc.h"
#include "Config.h"

GPIO_InitTypeDef gpioInitStruct;
	
void Config_PINS(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	gpioInitStruct.GPIO_Pin = GPIO_Pin_15;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOE, &gpioInitStruct);
}

NVIC_InitTypeDef   NVIC_InitStructure;

void Config_NVIC(void){
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
  NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);    
	SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE);
  NVIC_EnableIRQ(SPI1_IRQn);
}
