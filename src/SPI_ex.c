

#include "SPI_ex.h"

uint32_t count=1;
uint16_t dataIN=0x00,dataout=0x00,pageflag;
int i;




int32_t _compose_int32(int16_t a_hiword, int16_t a_loword) {
	uint32_t res = (static_cast<int32_t>(a_hiword)<<16)|(static_cast<uint16_t>(a_loword));
	return res;
	}
	
uint32_t _compose_uint32(int16_t a_hiword, int16_t a_loword) {
	return (static_cast<uint32_t>(a_hiword)<<16)|(static_cast<uint16_t>(a_loword));
	}	


uint16_t adis16480_spi::_compose_write_cmd(uint8_t a_register, uint8_t a_data) {
	return (static_cast<uint16_t>(a_register | ADIS_WRITE_FLAG)<<8) | a_data;
	}

uint16_t adis16480_spi::_compose_read_cmd(uint8_t a_register) {
	return static_cast<uint16_t>(a_register)<<8;
	}

int16_t adis16480_spi::_write_spi(uint16_t a) {
	select();
	int16_t res = spi.write(a);
	deselect();
	wait_us(STALL_PERIOD_MCS);	
	return res;
	}
	
int32_t adis16480_spi::_read_int32_val(uint8_t a_regLo, uint8_t a_regHi) {
	_write_spi(_compose_read_cmd(a_regLo));
	int16_t lo = _write_spi(_compose_read_cmd(a_regHi));
	int16_t hi = _write_spi(0x00);
	return _compose_int32(hi, lo);
	}
	
int16_t adis16480_spi::_read_int16_val(uint8_t a_reg) {
	_write_spi(_compose_read_cmd(a_reg));
	return _write_spi(0x00);
	}	

uint16_t adis16480_spi::_read_uint16_val(uint8_t a_reg) {
	return static_cast<uint16_t>(_read_int16_val(a_reg));
	}

void adis16480_spi::_write_uint16_cmd(uint8_t a_reg, uint16_t a_cmd) {
	_write_spi(_compose_write_cmd(a_reg, a_cmd & 0xff));
	_write_spi(_compose_write_cmd(a_reg+1, a_cmd >> 8));
	}	

void adis16480_spi::_SwitchToPage(uint8_t a_pageNum) {
	_write_spi(_compose_write_cmd(ADISREG_PAGE_ID, a_pageNum));
	}


bool adis16480_spi::_self_test(){ 
	_SwitchToPage(3);
	_write_uint16_cmd(ADISREG_GLOB_CMD, FLAG_GLOB_CMD_SELF_TEST);
	
    wait_ms(15);
	
	// read diag results and test againgst all possilbe 
	// error bits (0..5 and 8..11)
	_SwitchToPage(0);
	uint16_t selfTestRes = _read_uint16_val(ADISREG_DIAG_STS);

	return ((selfTestRes & 0x0F3F) == 0);
	}

void adis16480_spi::_software_reset() {
	// initialize software reset
	_SwitchToPage(3);
	_write_uint16_cmd(ADISREG_GLOB_CMD, FLAG_GLOB_CMD_SOFTWARE_RESET);
	
	// according to ADIS16480 manual
    wait_ms(1800);
	}

bool adis16480_spi::restart() {
	_software_reset();
	
	if (!init())
		return false;
		
	// only report we were successful if 
	// restart did fix the problem 
	return (need_restart() == false);
	}

// make sure compiler won't optimize this delay generator
#pragma push
#pragma O0

void adis16480_spi::_pause_6nop() const {
	// TODO: not guaranteed to work for hardware cores
	// faster than 200 MHz
	// 
	// 6 NOP's of at least 5 ns each to ensure (taken togeter with
	// function call overhead) 31+ ns pause 
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	}	

#pragma pop

void adis16480_spi::_pause_x_ns(uint32_t x) const {
	const uint32_t n = (x+29)/30;
	for (uint32_t i=0; i<n; ++i)
		_pause_6nop();
	}

uint32_t adis16480_spi::_bits2ns(uint32_t a_spiFreq, uint32_t a_bitsCount) const {
	const uint32_t nsPerKHz = 1000000;
	const uint32_t spiFreqKHz = a_spiFreq/1000;
	return nsPerKHz*a_bitsCount/spiFreqKHz;	
	}

bool adis16480_spi::_ensure_spi_correct(uint32_t a_spiFreq) {
	// assume there was no error
	bool res = true;
	
	while(_read_uint16_val(ADISREG_SYS_E_FLAG) & FLAG_SYS_E_SPI_ERROR) {
		_write_spi(0x00);
		// alas there was an error so some 
		// received data might be incorrect
		res = false;
		// make a 32bit-long pause
		_pause_x_ns(_bits2ns(a_spiFreq, 32));
		}
	
	return res;
	}
	
adis16480_spi::adis16480_spi() 
	: _spi(PC_3, PC_2, PB_10)	//define the SPI (mosi, miso, sclk)
	, //DigitalOut cs(PB_5); 
	{}
		
bool adis16480_spi::init(){
	deselect();

  spi.format(SPI_BITS_PER_SENDING, SPI_MODE);
  spi.frequency(SPI_STARTING_FREQ);
	
	_software_reset();
	
	//if (!_self_test())
		//return false;

	_SwitchToPage(3);
	

	wait_ms(570);
	

	wait_ms(100);
	
	uint16_t mode = _read_uint16_val(ADISREG_EKF_CNFG);


	// switch to registers bank 0 
	_SwitchToPage(0);
	
	// make a 16bit-long pause at SPI_STARTING_FREQ
	_pause_x_ns(_bits2ns(SPI_STARTING_FREQ, 16));
	
	spi.frequency(SPI_WORKING_FREQ);

	_ensure_spi_correct(SPI_WORKING_FREQ);

  return true;
	}

	
bool adis16480_spi::need_restart() {
	return (_read_uint16_val(ADISREG_SYS_E_FLAG) & 
		FLAG_SYS_E_PROCESSING_OVERRUN);
	}

unsigned int adis16480_spi::whoami(){
    return _read_uint16_val(ADISREG_PROD_ID);
	}



/*-----------------------------------------------------------------------------------------------
                                SPI SELECT AND DESELECT
usage: enable and disable ADIS16480 communication bus
-----------------------------------------------------------------------------------------------*/
void adis16480_spi::select() {
    //Set CS low to start transmission (interrupts conversion)
    cs = 0;
	// Must have a 32ns pause between "select" and "clock" signals edges.
	// _pause_30ns() itself plus its calling overhead will guarantee it.
//	_pause_6nop();	
	}

void adis16480_spi::deselect() {
    //Set CS high to stop transmission (restarts conversion)
	while(spi.is_busy()){}
//	_pause_6nop();	
    cs = 1;
	}
