/**
  ******************************************************************************
  * @file    ICM20948.c
  * @author  liun@xlnav.com
  * @version V1.0
  * @date    21-June-2018
  * @brief   This file includes the ICM20948 driver functions
  
  ******************************************************************************
  */

//#include "Debug.h"
//#include "i2c.h"
#include "dvc_ICM20948.h"
#include "spi.h"
void i2c_Mag_write(uint8_t reg,uint8_t value);
/**
  * @brief  Read bytes by SPI
  * @param  REG_Add:register address will be writen
	*         *pData: the point to the result of reading
	*				  Size:the zize of the reading bytes
  * @retval None
  */
void ICM20948_DevieceSPI_ReadBytes(uint8_t REG_Add, uint8_t *pData, uint16_t Size)
{
  REG_Add=REG_Add|0x80;
	HAL_GPIO_WritePin(CS_20948_GPIO_Port, CS_20948_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS_20948_GPIO_Port, CS_20948_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,&REG_Add,1,100);
	HAL_SPI_Receive(&hspi1,pData,Size,100);
  HAL_GPIO_WritePin(CS_20948_GPIO_Port, CS_20948_Pin, GPIO_PIN_SET);	
}
/**
  * @brief  Write bytes by SPI
  * @param  REG_Add:register address will be writen
	*        *pData: the point to the date which will be writen
	* 				Size:the size of the reading bytes
  * @retval None
  */

void ICM20948_DevieceSPI_WriteBytes(uint8_t REG_Add, uint8_t *pData, uint16_t Size)
{
	REG_Add=REG_Add&0x7F;
	HAL_GPIO_WritePin(CS_20948_GPIO_Port, CS_20948_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,&REG_Add,1,100);
	HAL_SPI_Transmit(&hspi1,pData,Size,100);
	HAL_GPIO_WritePin(CS_20948_GPIO_Port, CS_20948_Pin, GPIO_PIN_SET);
	
}
/**
  * @brief  Read one byte by SPI
  * @param  REG_Add:register address will be writen
	*         *pData: the point to the redult of reading
  * @retval None
  */
void ICM20948_DevieceSPI_ReadOneByte(uint8_t REG_Add, uint8_t* pData)
{
	REG_Add=REG_Add|0x80;
	HAL_GPIO_WritePin(CS_20948_GPIO_Port, CS_20948_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,&REG_Add,1,100);
	HAL_SPI_Receive(&hspi1,pData,1,100);
	HAL_GPIO_WritePin(CS_20948_GPIO_Port, CS_20948_Pin, GPIO_PIN_SET);
}
/**
  * @brief  Write one byte by SPI
  * @param  REG_Add:register address will be writen
	*         *pData: the point to the date which will be writen
  * @retval None
  */
void ICM20948_DevieceSPI_WriteOneByte(uint8_t REG_Add, uint8_t Data)
{
	REG_Add=REG_Add&0x7F;
	HAL_GPIO_WritePin(CS_20948_GPIO_Port, CS_20948_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,&REG_Add,1,100);
	HAL_SPI_Transmit(&hspi1,&Data,1,100);
	HAL_GPIO_WritePin(CS_20948_GPIO_Port, CS_20948_Pin, GPIO_PIN_SET);
}

/**
  * @brief  Initializes ICM20948
  * @param  None
  * @retval None
  */
void ICM20948_Init(void)
{
    unsigned char i2c_dev = 0;
//	  uint8_t recbuf[10];
	// 初始化 ICM2098 电源配置
	
	ICM20948_DevieceSPI_WriteOneByte(REG_BANK_SEL, SELECT_USER_BANK_0);
	HAL_Delay(10);
  ICM20948_DevieceSPI_WriteOneByte(PWR_MGMT_1, 0x01);
	ICM20948_DevieceSPI_WriteOneByte(PWR_MGMT_2, (PWN2_DISABLE_ACCE | PWN2_DISABLE_GYRO));
	HAL_Delay(10);
	ICM20948_DevieceSPI_WriteOneByte(PWR_MGMT_2, PWN2_ENABLE_ACCE | PWN2_ENABLE_GYRO);
	HAL_Delay(10);
	// 检查设备ID是否正确
  ICM20948_DevieceSPI_ReadOneByte( WHO_AM_I, &i2c_dev);
	if (i2c_dev == WHO_AM_I_VAL)
	{

		ICM20948_DevieceSPI_WriteOneByte(REG_BANK_SEL, SELECT_USER_BANK_2);
		HAL_Delay(20);
		// 配置陀螺
		ICM20948_DevieceSPI_WriteOneByte(GYRO_CONFIG_1, (GYRO_250DPS|GYRO_DLPF5_NBW17HZ));
		ICM20948_DevieceSPI_WriteOneByte(GYRO_SMPLRT_DIV,GYRO_SAMPLE_RATE_100HZ);
		
		// 配置加速度表
		ICM20948_DevieceSPI_WriteOneByte(ACCEL_CONFIG, (ACCEL_8G|ACCEL_DLPF2_NBW136HZ));
		// Set Accel sample rate 225Hz
		ICM20948_DevieceSPI_WriteOneByte(ACCEL_SMPLRT_DIV_1, ACCEL_SMPLRT_HIGH);
		ICM20948_DevieceSPI_WriteOneByte(ACCEL_SMPLRT_DIV_2,ACCEL_SMPLRT_LOW_100HZ);
		ICM20948_DevieceSPI_WriteOneByte(REG_BANK_SEL, SELECT_USER_BANK_2);
		/**********************Init SLV0 i2c**********************************/	
     //Use SPI-bus read slave0
		ICM20948_DevieceSPI_WriteOneByte(REG_BANK_SEL, SELECT_USER_BANK_0);
		ICM20948_DevieceSPI_WriteOneByte(INT_PIN_CFG ,0x30);// INT Pin / Bypass Enable Configuration
		ICM20948_DevieceSPI_WriteOneByte(USER_CTRL ,0x20); // I2C_MST _EN 
		ICM20948_DevieceSPI_WriteOneByte(REG_BANK_SEL, SELECT_USER_BANK_3);
		ICM20948_DevieceSPI_WriteOneByte(I2C_MST_CTRL,0x4d);//I2C MAster mode and Speed 400 kHz
		ICM20948_DevieceSPI_WriteOneByte(I2C_MST_DELAY_CTRL ,0x01);//I2C_SLV0 _DLY_ enable 	
	  ICM20948_DevieceSPI_WriteOneByte(I2C_SLV0_CTRL ,0x81); //enable IIC	and EXT_SENS_DATA==1 Bytee
		/**********************Init MAG **********************************/
		i2c_Mag_write(AK09916_CNTL3_REG,0x01); // Reset AK8963
		HAL_Delay(1000);
		i2c_Mag_write(AK09916_CNTL2_REG,0x02); // use i2c to set AK8963 working on Continuous measurement mode1 & 16-bit output
		}
		HAL_Delay(10);
		ICM20948_DevieceSPI_WriteOneByte(REG_BANK_SEL, SELECT_USER_BANK_0);
		HAL_Delay(10);

	
}
/**
  * @brief Get accelerometer and gyroscope datas
  * @param  accel: the value of accelerometer
  *         gyro:the value of gyroscope
  * @retval None
  */
void ICM20948_READ_ACCEL_AND_GYRO(int16_t accel[3], int16_t gyro[3])
{ 
	unsigned char BUF[16];
	//uint16_t j=50000;
	ICM20948_DevieceSPI_WriteOneByte(REG_BANK_SEL, SELECT_USER_BANK_0);
	ICM20948_DevieceSPI_ReadBytes( ACCEL_XOUT_H, BUF, 2);
	accel[0]= (BUF[0]<<8)|BUF[1];
	ICM20948_DevieceSPI_ReadBytes( ACCEL_YOUT_H, BUF, 2);
	accel[1]= (BUF[0]<<8)|BUF[1];
	ICM20948_DevieceSPI_ReadBytes( ACCEL_ZOUT_H, BUF, 2);
	accel[2]= (BUF[0]<<8)|BUF[1];
	ICM20948_DevieceSPI_ReadBytes( GYRO_XOUT_H, BUF, 2);
	gyro[0]= (BUF[0]<<8)|BUF[1];
	ICM20948_DevieceSPI_ReadBytes( GYRO_YOUT_H, BUF, 2);
	gyro[1]= (BUF[0]<<8)|BUF[1];
	ICM20948_DevieceSPI_ReadBytes( GYRO_ZOUT_H, BUF, 2);
	gyro[2]= (BUF[0]<<8)|BUF[1];
	
	//while(j--);	
//	accel[0]= (BUF[0]<<8)|BUF[1];
//	accel[1]= (BUF[2]<<8)|BUF[3];
//	accel[2]= (BUF[4]<<8)|BUF[5];
//	gyro[0] = (BUF[6]<<8)|BUF[7];	
//	gyro[1] = (BUF[8]<<8)|BUF[9];
//	gyro[2] = (BUF[10]<<8)|BUF[11];	
}
float acc_f[3],gyro_f[3],magn_f[3];
/**
  * @brief  Deal with accelerometer 、gyroscope and magnetometer datas
  * @param  accel: the value of accelerometer
  *         gyro:the value of gyroscope
  *         magn：the value of magnetometer
  * @retval None
  */
void ICM20948_ACCEL_AND_GYRO_AND_MAGN_datadeal(int16_t accel[3], int16_t gyro[3],int16_t magn[3])
{
	int i;
	for(i=0;i<3;i++)
	{
		acc_f[i]=(float)((double)accel[i]/4096*9.8);
	}
	for(i=0;i<3;i++)
	{
		gyro_f[i]=(float)((double)gyro[i]/131);
	}
		for(i=0;i<3;i++)
	{
		magn_f[i]=(float)((double)magn[i]*0.15);
	}
}
/**
  * @brief Write to I2C SLV0
  * @param  reg:the regiter address to be writen
  *         value:the value to be writen
  * @retval None
  */
void i2c_Mag_write(uint8_t reg,uint8_t value)
{
	uint16_t j=500;
	ICM20948_DevieceSPI_WriteOneByte(REG_BANK_SEL, SELECT_USER_BANK_3);
	while(j--);
	ICM20948_DevieceSPI_WriteOneByte(I2C_SLV0_ADDR ,MAG_ADDRESS);//mode: write
	while(j--);
	ICM20948_DevieceSPI_WriteOneByte(I2C_SLV0_REG ,reg);//set reg addr
    while(j--);
	ICM20948_DevieceSPI_WriteOneByte(I2C_SLV0_DO ,value);//send value	
	while(j--);//
	
}

/**
  * @brief Read a date from I2C SLV0
  * @param  reg:the regiter address to be read
  * @retval Data:EXT_SLV_SENS_DATA_00 register value
  */
static uint8_t i2c_Mag_read(uint8_t reg)
{
	uint16_t j=5000;
	uint8_t  Data;
	ICM20948_DevieceSPI_WriteOneByte(REG_BANK_SEL, SELECT_USER_BANK_3);
    while(j--);//
	ICM20948_DevieceSPI_WriteOneByte(I2C_SLV0_ADDR ,MAG_ADDRESS|0x80); 
    while(j--);
	ICM20948_DevieceSPI_WriteOneByte(I2C_SLV0_REG ,reg);// set reg addr
    while(j--);
	ICM20948_DevieceSPI_WriteOneByte(I2C_SLV0_DO ,0xff);//read
	while(j--);//
	ICM20948_DevieceSPI_WriteOneByte(REG_BANK_SEL, SELECT_USER_BANK_0);
	ICM20948_DevieceSPI_ReadOneByte(EXT_SLV_SENS_DATA_00,&Data);
    while(j--);
	return Data;
}
/**
  * @brief Read the date of magnetometer
  * @param  magn:the value of the magnetometer
  * @retval none
  */
void ICM20948_READ_MAG(int16_t magn[3])
{ 
  uint8_t BUF[10];	
	uint8_t recbuf[10];
  uint8_t REG_Add;
	uint8_t REG_Add2;
    
   
	
    BUF[0] =i2c_Mag_read(0x01);
	  
    BUF[1] =i2c_Mag_read(0x11);
	  BUF[2] =i2c_Mag_read(0x12);
	  magn[0]=BUF[1]|BUF[2]<<8;
  	BUF[3] =i2c_Mag_read(0x13);
    BUF[4] =i2c_Mag_read(0x14);
  	magn[1]=BUF[3]|BUF[4]<<8;
	 	BUF[5] =i2c_Mag_read(0x15);
    BUF[6] =i2c_Mag_read(0x16);
		magn[2]=BUF[5]|BUF[6]<<8;

   	i2c_Mag_write(AK09916_CNTL2_REG,0x01);
    
  
}
/**
  * @brief Read the date of tempreture
  * @param  tempreture:the value of the tempreture
  * @retval none
  */
void ICM20948_READ_TEMP(int16_t* tempreture)
{ 
	unsigned char BUF[4];
  int16_t InBuffer = 0;
	ICM20948_DevieceSPI_WriteOneByte(REG_BANK_SEL, SELECT_USER_BANK_0);
	ICM20948_DevieceSPI_ReadBytes(TEMP_OUT_H, BUF, 2);
  InBuffer = (BUF[0]<<8)|BUF[1];
	 //*tempreture = InBuffer ;
    *tempreture = (int16_t)((((double)(InBuffer-0)/333.87)+21.0)* 100) ;
	//  *tempreture = (float)(((double)(InBuffer-0)/333.87)+21.0) ;
}
