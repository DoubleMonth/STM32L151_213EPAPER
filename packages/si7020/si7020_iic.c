#include <rtthread.h>
#include "stdio.h" 
#include "main.h"
#include "stm32l1xx_hal.h"
#include "usart.h"

#include "si7020/si7020_iic.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//IIC��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/6/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
void delay_us(uint32_t us)
{
	uint32_t i,j;
	for(i=0;i<us;i++)
	{
		for(j=0;j<100;j++)
		{};
	}
}
//IIC��ʼ��
void SI7020_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
	__HAL_RCC_GPIOA_CLK_ENABLE();   //ʹ��GPIOCʱ��
    
    //PC11,12��ʼ������
    GPIO_Initure.Pin|=SI7020_SDA_Pin;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;     //����
    HAL_GPIO_Init(SI7020_SDA_GPIO_Port,&GPIO_Initure);
	
	 GPIO_Initure.Pin|=SI7020_SCL_Pin;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;     //����
    HAL_GPIO_Init(SI7020_SCL_GPIO_Port,&GPIO_Initure);
    
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_SET);
}

//����IIC��ʼ�ź�
void SI7020_IIC_Start(void)
{
	SI7020_SDA_OUT(); 	         //sda�����
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_SET);
	rt_thread_mdelay(1);
 	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_RESET);//START:when CLK is high,DATA change form high to low 
	rt_thread_mdelay(1);
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void SI7020_IIC_Stop(void)
{
	SI7020_SDA_OUT();//sda�����
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_RESET);//STOP:when CLK is high DATA change form low to high
 	rt_thread_mdelay(1);
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_SET);//����I2C���߽����ź�
	rt_thread_mdelay(1);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t SI7020_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SI7020_SDA_IN();      //SDA����Ϊ����  
	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_SET);
	rt_thread_mdelay(1);	   
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
	rt_thread_mdelay(1);	 
	while(HAL_GPIO_ReadPin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			SI7020_IIC_Stop();
			return 1;
		}
	}
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void SI7020_IIC_Ack(void)
{
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);
	SI7020_SDA_OUT();
	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_RESET);
	rt_thread_mdelay(1);
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
	rt_thread_mdelay(1);
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);
}
//������ACKӦ��		    
void SI7020_IIC_NAck(void)
{
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);
	SI7020_SDA_OUT();
	HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_SET);
	rt_thread_mdelay(1);
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
	rt_thread_mdelay(1);
	HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void SI7020_IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SI7020_SDA_OUT(); 	    
    HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
			HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin,GPIO_PIN_RESET);
        txd<<=1; 	  
		rt_thread_mdelay(1);   //��TEA5767��������ʱ���Ǳ����
		HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
		rt_thread_mdelay(1); 
		HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);
		rt_thread_mdelay(1);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t SI7020_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SI7020_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_RESET);
        rt_thread_mdelay(1);
		HAL_GPIO_WritePin(SI7020_SCL_GPIO_Port,SI7020_SCL_Pin,GPIO_PIN_SET);
        receive<<=1;
        if(HAL_GPIO_ReadPin(SI7020_SDA_GPIO_Port,SI7020_SDA_Pin))receive++;   
		rt_thread_mdelay(1); 
    }					 
    if (!ack)
        SI7020_IIC_NAck();//����nACK
    else
        SI7020_IIC_Ack(); //����ACK   
    return receive;
}

uint32_t get_be_val(const uint8_t * p, int bytes)
{
    uint32_t ret = 0;
    while (bytes-- > 0)
    {
        ret <<= 8;
        ret |= *p++;
    }

    return ret;
}
uint8_t si70xx_buff[1];
uint8_t si70xx_data[2];
uint8_t si7020Measure(double *temperature, double *humidity)
{
	double value=0.0;
	double temp=0.0;
	SI7020_IIC_Start();
	SI7020_IIC_Send_Byte(SI7020_ADDR);
	SI7020_IIC_Wait_Ack();
//	si70xx_buff[0] = 0xE3;
	SI7020_IIC_Send_Byte(0xF3);
	SI7020_IIC_Wait_Ack();
	SI7020_IIC_Start();
	SI7020_IIC_Send_Byte(SI7020_ADDR|SI7020_READ);
	SI7020_IIC_Wait_Ack();
	rt_thread_mdelay(20);
	si70xx_data[1] = SI7020_IIC_Read_Byte(1);
	si70xx_data[0] = SI7020_IIC_Read_Byte(0);
	SI7020_IIC_Stop();
	value = si70xx_data[0] +(si70xx_data[1]<<8);
	temp = (double)value;
	temp = (temp* 175.72 / 65536.0)  - 46.85;
	
	*temperature = temp;
//	rt_kprintf("temp=%d\n",temp);
	rt_thread_mdelay(10);
	
	SI7020_IIC_Start();
	SI7020_IIC_Send_Byte(SI7020_ADDR);
	SI7020_IIC_Wait_Ack();
//	si70xx_buff[0] = 0xE3;
	SI7020_IIC_Send_Byte(0xE5);
	SI7020_IIC_Wait_Ack();
	SI7020_IIC_Start();
	SI7020_IIC_Send_Byte(SI7020_ADDR|SI7020_READ);
	SI7020_IIC_Wait_Ack();
	rt_thread_mdelay(20);
	si70xx_data[1] = SI7020_IIC_Read_Byte(1);
	si70xx_data[0] = SI7020_IIC_Read_Byte(0);
	SI7020_IIC_Stop();
	value = si70xx_data[0] |(si70xx_data[1]<<8);
	temp = (double)value;
	temp = (temp / 65536.0f) * 125.0f - 6.0f;
	
	*humidity = temp;
//	rt_kprintf("humi=%d\n",temp);
	
}



