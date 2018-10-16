/**
  ******************************************************************************
  * @file    ICM20948.c
  * @author  liun@xlnav.com
  * @version V1.0
  * @date    21-June-2018
  * @brief   This file includes the ICM20948 driver functions
  
  ******************************************************************************
  */


#ifndef __ICM20948_H
#define __ICM20948_H
#include "Global.h"
// define ICM20948 register address
//****************************************
#define ADDRESS_AD0_LOW     0xD0 //address pin low (GND), default for InvenSense evaluation board
#define ADDRESS_AD0_HIGH    0xD1 //address pin high (VCC)

#define	GYRO_ADDRESS   ADDRESS_AD0_LOW	  //Gyro and Accel device address
#define ACCEL_ADDRESS  ADDRESS_AD0_LOW 
#define DEFAULT_ADDRESS  GYRO_ADDRESS

#define REG_BANK_SEL    0x7F
#define SELECT_USER_BANK_0 0x00
#define SELECT_USER_BANK_1 0x10
#define SELECT_USER_BANK_2 0x20
#define SELECT_USER_BANK_3 0x30


// USER　Bank 0 Register map
#define	WHO_AM_I		0x00	//identity of the device
#define WHO_AM_I_VAL	0xEA //identity of ICM20948 is 0xEA.

#define USER_CTRL 0x03
#define I2C_IF_DIS 0x10


#define INT_PIN_CFG 0x0F

#define	ACCEL_XOUT_H	0x2D
#define	ACCEL_XOUT_L	0x2E
#define	ACCEL_YOUT_H	0x2F
#define	ACCEL_YOUT_L	0x30
#define	ACCEL_ZOUT_H	0x31
#define	ACCEL_ZOUT_L	0x32

#define	TEMP_OUT_H		0x39
#define	TEMP_OUT_L		0x3A

#define	GYRO_XOUT_H		0x33
#define	GYRO_XOUT_L		0x34	
#define	GYRO_YOUT_H		0x35
#define	GYRO_YOUT_L		0x36
#define	GYRO_ZOUT_H		0x37
#define	GYRO_ZOUT_L		0x38

#define EXT_SLV_SENS_DATA_00  0x3B

// USER　Bank 1 Register map

#define	PWR_MGMT_1		0x06	//Power Management 1. Typical values:0x00(run mode)
#define	PWR_MGMT_2		0x07	//Power Management 2. Typical values:0x00(run mode)
#define PWR1_RUN_MODE 		0x01
#define PWN2_DISABLE_ACCE 	0x38
#define PWN2_DISABLE_GYRO 	0x07
#define PWN2_ENABLE_ACCE	0x00
#define PWN2_ENABLE_GYRO	0x00

// USER　Bank 2 Register map
#define	GYRO_CONFIG_1	0x01	//Gyro Full Scale Select. Typical values:0x10(1000dps)
#define	GYRO_250DPS     0x00
#define	GYRO_500DPS     0x02
#define	GYRO_1000DPS    0x04
#define	GYRO_2000DPS    0x06
#define GYRO_DLPF7_NBW377HZ		0x39
#define GYRO_DLPF6_NBW6HZ		0x31
#define GYRO_DLPF5_NBW17HZ		0x29
#define GYRO_DLPF4_NBW36HZ		0x21
#define GYRO_DLPF3_NBW73HZ		0x19
#define GYRO_DLPF2_NBW154HZ		0x11
#define GYRO_DLPF1_NBW188HZ		0x09
#define GYRO_DLPF0_NBW230HZ		0x01
#define GYRO_DLPF_DISABLE       0x00

#define	ACCEL_CONFIG	0x14	//Accel Full Scale Select. Typical values:0x01(2g)
#define ACCEL_2G      	0x00
#define ACCEL_4G      	0x02
#define ACCEL_8G		0x04
#define ACCEL_16G       0x06
#define ACCEL_DLPF_DISABLE		0x00
#define ACCEL_DLPF1_NBW265HZ	0x09
#define ACCEL_DLPF2_NBW136HZ	0x11
#define ACCEL_DLPF3_NBW69HZ		0x19
#define ACCEL_DLPF4_NBW34HZ		0x21
#define ACCEL_DLPF5_NBW17HZ		0x29
#define ACCEL_DLPF6_NBW8HZ		0x31
#define ACCEL_DLPF7_NBW499HZ	0x39

#define	GYRO_SMPLRT_DIV		0x00	//Sample Rate Divider. Typical values:0x07(125Hz) 1KHz internal sample rate
#define GYRO_SAMPLE_RATE_1100HZ	0x00
#define GYRO_SAMPLE_RATE_550HZ	0x02
#define GYRO_SAMPLE_RATE_225HZ	0x04
#define GYRO_SAMPLE_RATE_125HZ	0x07
#define GYRO_SAMPLE_RATE_100HZ	0x0A
#define GYRO_SAMPLE_RATE_50HZ	0x15
#define GYRO_SAMPLE_RATE_20HZ	0x36
#define ACCEL_SMPLRT_DIV_1 0x10
#define ACCEL_SMPLRT_HIGH  0x00
#define ACCEL_SMPLRT_DIV_2 0x11
#define ACCEL_SMPLRT_LOW_1100HZ   0x00
#define ACCEL_SMPLRT_LOW_550HZ   0x02
#define ACCEL_SMPLRT_LOW_225HZ   0x04
#define ACCEL_SMPLRT_LOW_125HZ   0x07
#define ACCEL_SMPLRT_LOW_100HZ   0x0A
#define ACCEL_SMPLRT_LOW_50HZ 0x15
#define ACCEL_SMPLRT_LOW_20HZ 0x36
// USER　Bank 3 Register map
#define I2C_MST_CTRL                        0x01
#define I2C_MST_DELAY_CTRL                  0x02
#define I2C_SLV0_ADDR                       0x03  
#define I2C_SLV0_REG                        0x04
#define I2C_SLV0_CTRL                       0x05 
#define I2C_SLV0_DO                         0x06 //output reg







 
#define MAG_ADDRESS    0x0C   //compass device address
#define	ASAX			0x10	//MAG X -axis sensitivity adjustment value(Fuse ROM)
#define	ASAY			0x11	//MAG Y -axis sensitivity adjustment value(Fuse ROM)
#define	ASAZ			0x12	//MAG Y -axis sensitivity adjustment value(Fuse ROM)

//#define MAG_XOUT_L		0x03
//#define MAG_XOUT_H		0x04
//#define MAG_YOUT_L		0x05
//#define MAG_YOUT_H		0x06
//#define MAG_ZOUT_L		0x07
//#define MAG_ZOUT_H		0x08
#define MAG_XOUT_L		0x11
#define MAG_XOUT_H		0x12
#define MAG_YOUT_L		0x13
#define MAG_YOUT_H		0x14
#define MAG_ZOUT_L		0x15
#define MAG_ZOUT_H		0x16
#define AK09916_CNTL2_REG 0x31
#define AK09916_CNTL3_REG 0x32

void ICM20948_Init(void);
//void ICM20948_READ_MAG(int16_t magn[3]);
void ICM20948_READ_ACCEL_AND_GYRO(int16_t accel[3], int16_t gyro[3]);
void ICM20948_READ_TEMP(int16_t* tempreture);
void ICM20948_ACCEL_AND_GYRO_AND_MAGN_datadeal(int16_t accel[3], int16_t gyro[3],int16_t magn[3]);
void ICM20948_READ_MAG(int16_t magn[3]);
#endif
