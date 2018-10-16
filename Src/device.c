#include "main.h"
#include "stm32f4xx_hal.h"
#include "tim.h"
#include "usart.h"
#include "dvc_ICM20948.h"
int16_t temp;
extern float acc_f[3],gyro_f[3],magn_f[3];
unsigned short FramLength=0;
unsigned char CheckSum8(unsigned char* pBuffer, int nLength);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	int16_t accel[3]={0};
	int16_t gyro[3]={0};
	int16_t mag[3]={0};
	unsigned char Txdata[32];
	unsigned char *pf;
	int i;
	
	if (htim->Instance == htim6.Instance)
	{
		///@todo 在这里补充输出函数
		HAL_GPIO_TogglePin(GPIOB, LED3_Pin|LED2_Pin|LED1_Pin);
		ICM20948_READ_TEMP(&temp);
		ICM20948_READ_ACCEL_AND_GYRO(&accel[0], &gyro[0]);
		//ICM20948_READ_MAG(&mag[0]);
		ICM20948_ACCEL_AND_GYRO_AND_MAGN_datadeal(&accel[0], &gyro[0],&mag[0]);
		
		
		Txdata[0]=0x55;
		Txdata[1]=0xaa;
		Txdata[2]=0x01;
		Txdata[3]=FramLength;
		Txdata[4]=FramLength>>8;
		FramLength++;
		for(i=0;i<3;i++)
		{
			pf=(unsigned char*)&gyro_f[i];
			Txdata[5+i*4]=pf[0];
			Txdata[6+i*4]=pf[1];
		    Txdata[7+i*4]=pf[2];
			Txdata[8+i*4]=pf[3];
		}
        for(i=0;i<3;i++)
		{
			pf=(unsigned char*)&acc_f[i];
			Txdata[17+i*4]=pf[0];
			Txdata[18+i*4]=pf[1];
		    Txdata[19+i*4]=pf[2];
			Txdata[20+i*4]=pf[3];
		};
		Txdata[29]=temp;
		Txdata[30]=temp>>8;
		Txdata[31]=CheckSum8(&Txdata[2],29);
		HAL_UART_Transmit(&huart3,(uint8_t*)&Txdata[0],32,1);
//		printf("\nAABB %f %f %f %f %f %f %d CCDD\n",gyro_f[0],gyro_f[1],gyro_f[2],acc_f[0],acc_f[1],acc_f[2],temp);
//		printf("%d %d %d %d %d %d %d\n",accel[0],accel[1],accel[2],gyro[0],gyro[1],gyro[2],temp);
//	printf("%f %f %f %f %f %f %f %f %f %d\n",acc_f[0],acc_f[1],acc_f[2],gyro_f[0],gyro_f[1],gyro_f[2],magn_f[0],magn_f[1],magn_f[2],temp);		
   // MTiRead();
//		MTidatadeal();
//		HAL_SPI_Receive(&hspi1,(uint8_t*)&buffer[0],8,100);
	}
}
unsigned char CheckSum8(unsigned char* pBuffer, int nLength)
{
	unsigned char retval = 0x00;
	int i;
	
	for(i=0;i<nLength;i++)
	{
		retval += pBuffer[i];
	}
	return retval;
}
/**
  * 函数功能: 重定向c库函数printf到DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

/**
  * 函数功能: 重定向c库函数getchar,scanf到DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fgetc(FILE * f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&huart3,&ch, 1, 0xffff);
  return ch;
}