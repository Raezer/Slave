#include "AdisRecv.h"
#include "mbed.h"
#include "stm32f4xx_spi.h"
int v[100];
	int buf;
	int i=0;
SPISlave device(PB_5, PA_6, PA_5,PA_4); 
//SPISlave(PinName mosi, PinName miso, PinName sclk, PinName ssel);
void init(){
	
	device.format(SPI_BITS_PER_SENDING, SPI_MODE);
	device.frequency(SPI_STARTING_FREQ);
  device.reply(0x00); 
}
int pageNum;
void adisRecieve(){
	 if(device.receive()) {
     buf = device.read();   // Read byte from master
            //переключение страниц, ждем 0х8003 
		 if (buf==0x8003)
			{
			 pageNum=3;
			}
		 v[i]=buf;
		 device.reply(v[i]);
		 i++;

						
		 }
}

//     a_pageNum =3;
//ADISREG_PAGE_ID=0x00
//_write_spi(_compose_write_cmd(ADISREG_PAGE_ID, a_pageNum));
//uint16_t adis16480_spi::_compose_write_cmd(uint8_t a_register, uint8_t a_data) {
//	return (static_cast<uint16_t>(a_register | ADIS_WRITE_FLAG)<<8) | a_data;
//	}

//_write_uint16_cmd(ADISREG_GLOB_CMD, FLAG_GLOB_CMD_SOFTWARE_RESET);

//void adis16480_spi::_write_uint16_cmd(uint8_t a_reg, uint16_t a_cmd) {
//	_write_spi(_compose_write_cmd(a_reg, a_cmd & 0xff));
//	_write_spi(_compose_write_cmd(a_reg+1, a_cmd >> 8));
//	}	

//	int16_t adis16480_spi::_write_spi(uint16_t a) {
//	select();
//	int16_t res = spi.write(a);
//	deselect();
//	wait_us(STALL_PERIOD_MCS);	
//	return res;
//	}
