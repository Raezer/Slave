#ifndef SPI_EX_H
#define SPI_EX_H
#include "stm32f4xx.h"
#include "spi.h"
#include "mbed.h"

class adis16480_spi {
	SPI& spi;
public:
	adis16480_spi(SPI& _spi);
	bool init();
	bool need_restart();
	bool restart();
	unsigned int whoami();


	static float lsb_omega();
	static float lsb_accel();
	static float lsb_magn();
	static float lsb_baro();
	
	mbed::SPI						_spi;

	float acc_divider;
	float gyro_divider;
	adis16480_spi();
private:
	enum {
		SPI_BITS_PER_SENDING = 16,
		SPI_MODE = 3,
		SPI_STARTING_FREQ = 400000,
		MHZ = 1000000,
		SPI_WORKING_FREQ = 15*MHZ,
		DELAY_CS2CLK_NS = 32,
		STALL_PERIOD_MCS = 3,
		};

	enum {
		ADISREG_PAGE_ID = 		0x00,
		ADISREG_GLOB_CMD	 =	0x02,
		ADISREG_FNCTIO_CTRL	= 	0x06,
		ADISREG_SYS_E_FLAG	= 	0x08,
		ADISREG_DIAG_STS	 =	0x0A,
		ADISREG_FILTR_BNK_0	= 	0x16,
		ADISREG_FILTR_BNK_1	= 	0x18,
		ADISREG_PROD_ID = 		0x7E,
		
		ADISREG_X_ACCL_OUT = 	0x1E,
		ADISREG_X_ACCL_LOW = 	0x1C,
		ADISREG_Y_ACCL_OUT = 	0x22,
		ADISREG_Y_ACCL_LOW = 	0x20,
		ADISREG_Z_ACCL_OUT = 	0x26,
		ADISREG_Z_ACCL_LOW = 	0x24,
		
		ADISREG_X_GYRO_OUT = 	0x12,
		ADISREG_X_GYRO_LOW = 	0x10,
		ADISREG_Y_GYRO_OUT = 	0x16,
		ADISREG_Y_GYRO_LOW = 	0x14,
		ADISREG_Z_GYRO_OUT = 	0x1A,
		ADISREG_Z_GYRO_LOW = 	0x18,
		
		ADISREG_X_MAGN_OUT = 	0x28,
		ADISREG_Y_MAGN_OUT = 	0x2A,
		ADISREG_Z_MAGN_OUT = 	0x2C,

		ADISREG_BAROM_OUT = 	0x30,
		ADISREG_BAROM_LOW = 	0x2E,
		
		ADISREG_TEMP_OUT =		0x0E,
		
		ADISREG_EKF_CNFG =		0x50,		
		ADISREG_DEC_RATE =		0x0C,		
		
		FLAG_FIOCTL_DATA_READY_EANBLE = 0x08,
		FLAG_FIOCTL_DATA_READY_LINE_DIO2 = 0x01,
		
		FLAG_GLOB_CMD_SELF_TEST 		= 0x01 << 1,
		FLAG_GLOB_CMD_SOFTWARE_RESET 	= 0x01 << 7,
		
		FLAG_SYS_E_SPI_ERROR 			= 0x01 << 3,
		FLAG_SYS_E_SENSOR_OVERRANGE 	= 0x01 << 4,
		FLAG_SYS_E_PROCESSING_OVERRUN 	= 0x01 << 7,
		FLAG_SYS_E_OVERACCELERATION 	= 0x01 << 10,
		FLAG_SYS_E_OVERMAGNITOMETER 	= 0x01 << 11,
		FLAG_SYS_E_OVERGYROSCOPE	 	= 0x01 << 12,
		
		ADIS_WRITE_FLAG = 		0x80
		};
	
	static uint16_t _compose_write_cmd(uint8_t a_register, uint8_t a_data);
	static uint16_t _compose_read_cmd(uint8_t a_register);


	void _software_reset();
	bool _self_test();
	int16_t _write_spi(uint16_t a);
	int32_t _read_int32_val(uint8_t a_regLo, uint8_t a_regHi);
	int16_t _read_int16_val(uint8_t a_reg);
	uint16_t _read_uint16_val(uint8_t a_reg);
	void _write_uint16_cmd(uint8_t a_reg, uint16_t a_cmd);
	void _SwitchToPage(uint8_t a_pageNum);
	void _pause_6nop() const;
	void _pause_x_ns(uint32_t x) const; 
	uint32_t _bits2ns(uint32_t a_spiFreq, uint32_t a_bitsCount) const;
	bool _ensure_spi_correct(uint32_t a_spiFreq);
	void page3_adis(uint16_t c);
	void page0_adis(uint16_t c);
	float _error;
	};
#endif
