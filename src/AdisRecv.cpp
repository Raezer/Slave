#include "AdisRecv.h"
#include "mbed.h"
#include "stm32f4xx_spi.h"
int cmdHistory[100];
int cmdHandled[100];
	int buf,pageNum,ans,cmdHandledN=0,i=0;

	SPISlave device(PB_5, PA_6, PA_5,PA_4); //mosi,miso,sclk,ssel

void init(){
	device.format(SPI_BITS_PER_SENDING, SPI_MODE);
	device.frequency(SPI_STARTING_FREQ);
  device.reply(0x00); 
}
int checkREG(int buf)
{
	ans=0x00;
	
			 if (buf ==0x8003)
			 {
			 pageNum=3;
				
			 }
			 if (buf ==0x8000)
			 {
			 pageNum=0;
				
			 }
			//selftest
			if (buf == 0x0A00)
			 {
				cmdHandled[cmdHandledN]=buf;
				ans=0x00;
				cmdHandledN++;
				
			 }
			//mode
			if (buf ==0x5000)
			 {
				cmdHandled[cmdHandledN]=buf;
				ans=0x000a;
				cmdHandledN++;
				
			 }
			//whoami
			if (buf ==0x7E00)
			 {
				cmdHandled[cmdHandledN]=buf;
				ans=0x4060;
				cmdHandledN++;
				
			 }
		
			if (buf ==ADISREG_X_ACCL_OUT)
			 {
				cmdHandled[cmdHandledN]=buf;
				ans=0x30D4;
				cmdHandledN++;
				
			 }
		
		  if (buf ==ADISREG_Y_ACCL_OUT)
		   {
				cmdHandled[cmdHandledN]=buf;
				ans=0x0010;
				cmdHandledN++;
				
			 }
		  if (buf ==ADISREG_Z_ACCL_OUT)
		   {
				cmdHandled[cmdHandledN]=buf;
				ans=0x0030;
				cmdHandledN++;
				
			 }
		  if (buf ==ADISREG_X_GYRO_OUT)
		   {
				cmdHandled[cmdHandledN]=buf;
				ans=0x57E4;
				cmdHandledN++;
				
			 }
		  if (buf ==ADISREG_Y_GYRO_OUT)
		   {
				cmdHandled[cmdHandledN]=buf;
				ans=0x0010;
				cmdHandledN++;
				
			 }
		   if (buf ==ADISREG_Z_GYRO_OUT)
		   {
				cmdHandled[cmdHandledN]=buf;
				ans=0x0030;
				cmdHandledN++;
				
			 }
		 if (buf ==ADISREG_X_MAGN_OUT)
			 {
				cmdHandled[cmdHandledN]=buf;
				ans=0x7FFF;
				cmdHandledN++;
				
			 }
		   if (buf ==ADISREG_Y_MAGN_OUT)
			 {
				cmdHandled[cmdHandledN]=buf;
				ans=0x0010;
				cmdHandledN++;
				
			 }
		   if (buf ==ADISREG_Z_MAGN_OUT)
       {
				cmdHandled[cmdHandledN]=buf;
				ans=0x0030;
				cmdHandledN++;
				
			 }
		  if (buf ==ADISREG_BAROM_OUT)
			 {
				cmdHandled[cmdHandledN]=buf;
				ans=0x7FFF;
				cmdHandledN++;
				
			 }
		  if (buf ==ADISREG_BAROM_LOW)
		   {
				cmdHandled[cmdHandledN]=buf;
				ans=0x0001;
				cmdHandledN++;
				
			 }
		  if (buf ==ADISREG_TEMP_OUT)
			 {
				cmdHandled[cmdHandledN]=buf;
				ans=0x0000;
				cmdHandledN++;
				
			 }
	 
			
			//prod_id=0x4060
		 cmdHistory[i]=buf;
		 i++;	
	return ans;
}
void adisRecieve(){
	 if(device.receive()) {
     buf = device.read();   // Read byte from master
     device.reply(checkREG(buf));
		 }
}
