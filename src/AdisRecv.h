#ifndef ADISRECV_H
#define ADISRECV_H
void init();
void adisRecieve();
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
		
		ADISREG_X_ACCL_OUT = 	0x00001E00,
		ADISREG_X_ACCL_LOW = 	0x00001C00,
		ADISREG_Y_ACCL_OUT = 	0x00002200,
		ADISREG_Y_ACCL_LOW = 	0x00002000,
		ADISREG_Z_ACCL_OUT = 	0x00002600,
		ADISREG_Z_ACCL_LOW = 	0x00002400,
		
		ADISREG_X_GYRO_OUT = 	0x00001200,
		ADISREG_X_GYRO_LOW = 	0x00001000,
		ADISREG_Y_GYRO_OUT = 	0x00001600,
		ADISREG_Y_GYRO_LOW = 	0x00001400,
		ADISREG_Z_GYRO_OUT = 	0x00001A00,
		ADISREG_Z_GYRO_LOW = 	0x00001800,
		
		ADISREG_X_MAGN_OUT = 	0x00002800,
		ADISREG_Y_MAGN_OUT = 	0x00002A00,
		ADISREG_Z_MAGN_OUT = 	0x00002C00,

		ADISREG_BAROM_OUT = 	0x00003000,
		ADISREG_BAROM_LOW = 	0x00002E00,
		
		ADISREG_TEMP_OUT =		0x00000E00,
		
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
#endif
