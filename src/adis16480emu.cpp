#include "adis16480emu.h"
#include "defines.h"




int32_t _compose_int32(int16_t a_hiword, int16_t a_loword) {
	uint32_t res = (static_cast<int32_t>(a_hiword)<<16)|(static_cast<uint16_t>(a_loword));
	return res;
	}
	
uint32_t _compose_uint32(int16_t a_hiword, int16_t a_loword) {
	return (static_cast<uint32_t>(a_hiword)<<16)|(static_cast<uint16_t>(a_loword));
	}	


adis16480_spi::adis16480_spi(SPI& _spi, PinName _cs) 
	: spi(_spi)
	, cs(_cs) 
	{}
		
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
	
//	while(_read_uint16_val(ADISREG_SYS_E_FLAG) & FLAG_SYS_E_SPI_ERROR) {
//		_write_spi(0x00);
//		// alas there was an error so some 
//		// received data might be incorrect
//		res = false;
//		// make a 32bit-long pause
//		_pause_x_ns(_bits2ns(a_spiFreq, 32));
//		}
	
	return res;
	}


bool adis16480_spi::init(){
	deselect();
	
    spi.format(SPI_BITS_PER_SENDING, SPI_MODE);
    spi.frequency(SPI_STARTING_FREQ);
	
	_software_reset();
	
	//if (!_self_test()) это важная штука потом убрать 
		//return false;

	_SwitchToPage(3);
	
	// set "data not ready" to positive polarity
	_write_uint16_cmd(ADISREG_FNCTIO_CTRL, 
		FLAG_FIOCTL_DATA_READY_EANBLE | FLAG_FIOCTL_DATA_READY_LINE_DIO2);

	// set all sensors to use FIR filters bank B
	_write_uint16_cmd(ADISREG_FILTR_BNK_0, 0x5b6d);
	_write_uint16_cmd(ADISREG_FILTR_BNK_1, 0x0005);
	
	// set decimation rate to 12 (11+1) to 
	// have 205 Hz effective sampling rate
	_write_uint16_cmd(ADISREG_DEC_RATE, 11);
	
	// wait 0.57 seconds before sending special commands
	// for ADIS to initialize completely. The time is
	// taken from ADIS 16480 datasheet (0.4+-0.16 sec).
	wait_ms(570);
	
	// turn off "output data in local navigation frame"
	// option for ADIS-16480 ("use local navigation frame"
	// is a default ADIS-16480 power-on setting; but what we 
	// need is a "body-frame" data alignment to correctly 
	// calculate orientaion with our own maths)
	// and "disable magnetometer influence over angle calculations"
	// Wtite 0x0A to address 0x50 (1101 0000 0000 1010b)
	_write_uint16_cmd(ADISREG_EKF_CNFG, 0x000A);
	wait_ms(100);
	uint16_t mode = _read_uint16_val(ADISREG_EKF_CNFG);
	//if (mode != 0x000a) это важная штука, потом убрать
		//return false;

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

float adis16480_spi::lsb_omega() {
	static const float omega_hiword_lsb_val = 0.02f;
	return omega_hiword_lsb_val/65536.0f;
	}

float adis16480_spi::_OmegaCode2Float(int32_t a) {
	return static_cast<float>(a)*lsb_omega();
	}

float adis16480_spi::lsb_accel() {
	static const float hiword_lsb_val = 0.8f;
	static const float accel_hiword_lsb_val = hiword_lsb_val/1000.0f;
	return accel_hiword_lsb_val/65536.0f;
	}

float adis16480_spi::_AccelCode2Float(int32_t a) {
	return static_cast<float>(a)*lsb_accel();
	}

float adis16480_spi::lsb_magn() {
	return 0.1;
	}
	
float adis16480_spi::_MagnetometerCode2Float(int16_t a)	{
	return static_cast<float>(a)*lsb_magn();
	}	

float adis16480_spi::lsb_baro() {
	return 40.0e-3/65536.0;
	}
	
float adis16480_spi::_PressureCode2Float(int32_t a)	{
	return static_cast<float>(a)*lsb_baro();
	}	
	
float adis16480_spi::_TemperatureCode2Float(int16_t a)	{
	static const float lsb_celsius_deg = 0.00565f;
	return (static_cast<float>(a)*lsb_celsius_deg)+25.0f;
	}	
	

void adis16480_spi::read_all_mpu_data(MpuDataArray* a_res) {

	const int signs[] = {-1, 1, -1};
	const int indexes[] = {0, 1, 2};



	int32_t i32val[3];
	
	do {
		i32val[0] = _read_int32_val(ADISREG_X_ACCL_LOW, ADISREG_X_ACCL_OUT);
		i32val[1] = _read_int32_val(ADISREG_Y_ACCL_LOW, ADISREG_Y_ACCL_OUT);
		i32val[2] = _read_int32_val(ADISREG_Z_ACCL_LOW, ADISREG_Z_ACCL_OUT);
	
		
		a_res->Set_Delta_V_x(signs[0]*_AccelCode2Float(i32val[indexes[0]]));
		a_res->Set_Delta_V_y(signs[1]*_AccelCode2Float(i32val[indexes[1]]));
		a_res->Set_Delta_V_z(signs[2]*_AccelCode2Float(i32val[indexes[2]]));
		} while(!_ensure_spi_correct(SPI_WORKING_FREQ));
		
	do {
		// no need to read lower 16 bits
		i32val[0] = static_cast<int32_t>(_read_int16_val(ADISREG_X_GYRO_OUT))<<16;
		i32val[1] = static_cast<int32_t>(_read_int16_val(ADISREG_Y_GYRO_OUT))<<16;
		i32val[2] = static_cast<int32_t>(_read_int16_val(ADISREG_Z_GYRO_OUT))<<16;
		
		a_res->Set_omega_x(signs[0]*_OmegaCode2Float(i32val[indexes[0]]));
		a_res->Set_omega_y(signs[1]*_OmegaCode2Float(i32val[indexes[1]]));
		a_res->Set_omega_z(signs[2]*_OmegaCode2Float(i32val[indexes[2]]));
		} while(!_ensure_spi_correct(SPI_WORKING_FREQ));
	
		
	int16_t i16val[3];
	
	do {
		i16val[0] = _read_int16_val(ADISREG_X_MAGN_OUT);
		i16val[1] = _read_int16_val(ADISREG_Y_MAGN_OUT);
		i16val[2] = _read_int16_val(ADISREG_Z_MAGN_OUT);
	
		
		a_res->Set_Magnetometer_x_mgauss(signs[0]*_MagnetometerCode2Float(i16val[indexes[0]]));
		a_res->Set_Magnetometer_y_mgauss(signs[1]*_MagnetometerCode2Float(i16val[indexes[1]]));
		a_res->Set_Magnetometer_z_mgauss(signs[2]*_MagnetometerCode2Float(i16val[indexes[2]]));
		} while(!_ensure_spi_correct(SPI_WORKING_FREQ));
		
	do {
		i32val[0] = _read_int32_val(ADISREG_BAROM_LOW, ADISREG_BAROM_OUT);
		a_res->Set_Pressure_mbar(_PressureCode2Float(i32val[0]));

		
		i16val[0] = _read_int16_val(ADISREG_TEMP_OUT);
		a_res->Set_Temperature(_TemperatureCode2Float(i16val[0]));
		} while(!_ensure_spi_correct(SPI_WORKING_FREQ));
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
