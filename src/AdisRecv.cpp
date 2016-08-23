#include "AdisRecv.h"
#include "mbed.h"
#include "stm32f4xx_spi.h"
int v[100];
	int buf;
	int i=0;
SPISlave device(PB_5, PA_6, PA_5,PA_4); 
void init(){
	
	device.format(SPI_BITS_PER_SENDING, SPI_MODE);
	device.frequency(SPI_STARTING_FREQ);
  device.reply(0x00); 
}
void adisRecieve(){
	
}
void SPI1_IRQHandler(void)
{
    if (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_RXNE) == SET)
    {
      if(device.receive()) {
            buf = device.read();   // Read byte from master
            v[i]=buf;
			 // v = v;     // Add one to it, modulo 256
            device.reply(v[i]);
			      i++;
	
			if (i==99)
			{
				i=0;
			}
		 }
    }
}
