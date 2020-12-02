#include "stdio.h" 
#include "main.h"
#include "stm32l1xx_hal.h"
#include "usart.h"
#include "pcf8563/pcf8563.h"
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

unsigned char  time_buf1[8]={20,20,12,02,23,59,50,3};
//uint8_t time_buf1[8]={1,2,3,4,5,6,7,8};
unsigned char time_buf[8];
unsigned char time_buf2[8];
unsigned char *time_buf1_sp;

void pcf8563_delay_us(uint32_t us)
{
	uint32_t i,j;
	for(i=0;i<us;i++)
	{
		for(j=0;j<100;j++)
		{};
	}
}
//IIC��ʼ��
void PCF8563_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();   //ʹ��GPIOCʱ��
    
    //PC11,12��ʼ������
    GPIO_Initure.Pin|=PCF8563_SCL_Pin|PCF8563_SDA_Pin;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_SET);
}

//����IIC��ʼ�ź�
void PCF8563_IIC_Start(void)
{
	SDA_OUT(); 	         //sda�����
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_SET);
	rt_thread_mdelay(1);
 	HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_RESET);//START:when CLK is high,DATA change form high to low 
	rt_thread_mdelay(1);
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void PCF8563_IIC_Stop(void)
{
	SDA_OUT();//sda�����
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_RESET);//STOP:when CLK is high DATA change form low to high
 	rt_thread_mdelay(1);
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_SET);//����I2C���߽����ź�
	rt_thread_mdelay(1);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t PCF8563_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_SET);
	rt_thread_mdelay(1);	   
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_SET);
	rt_thread_mdelay(1);	 
	while(HAL_GPIO_ReadPin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			PCF8563_IIC_Stop();
			return 1;
		}
	}
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void PCF8563_IIC_Ack(void)
{
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);
	SDA_OUT();
	HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_RESET);
	rt_thread_mdelay(1);
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_SET);
	rt_thread_mdelay(1);
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);
}
//������ACKӦ��		    
void PCF8563_IIC_NAck(void)
{
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);
	SDA_OUT();
	HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_SET);
	rt_thread_mdelay(1);
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_SET);
	rt_thread_mdelay(1);
	HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void PCF8563_IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
    HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
			HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin,GPIO_PIN_RESET);
        txd<<=1; 	  
		rt_thread_mdelay(1);   //��TEA5767��������ʱ���Ǳ����
		HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_SET);
		rt_thread_mdelay(1); 
		HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);
		rt_thread_mdelay(1);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t PCF8563_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_RESET);
        rt_thread_mdelay(1);
		HAL_GPIO_WritePin(PCF8563_SCL_GPIO_Port,PCF8563_SCL_Pin,GPIO_PIN_SET);
        receive<<=1;
        if(HAL_GPIO_ReadPin(PCF8563_SDA_GPIO_Port,PCF8563_SDA_Pin))receive++;   
		rt_thread_mdelay(1); 
    }					 
    if (!ack)
        PCF8563_IIC_NAck();//����nACK
    else
        PCF8563_IIC_Ack(); //����ACK   
    return receive;
}


//uint8_t si70xx_buff[1];
//uint8_t si70xx_data[2];
uint8_t PCF8563_ReadOneByte(uint16_t ReadAddr)
{
	uint8_t temp = 0;
	PCF8563_IIC_Start();
	PCF8563_IIC_Send_Byte(PCF8563_ADDR);
	PCF8563_IIC_Wait_Ack();
	PCF8563_IIC_Send_Byte(ReadAddr);
	PCF8563_IIC_Wait_Ack();
	PCF8563_IIC_Start();
	PCF8563_IIC_Send_Byte(PCF8563_ADDR|PCF8563_READ);
	PCF8563_IIC_Wait_Ack();
	temp = PCF8563_IIC_Read_Byte(0);
	
	PCF8563_IIC_Stop();
	return temp;
}
uint8_t PCF8563_WriteOneByte(uint8_t WriteAddr,uint8_t DataToWrite)
{
	PCF8563_IIC_Start();
	PCF8563_IIC_Send_Byte(PCF8563_ADDR);
	PCF8563_IIC_Wait_Ack();
	PCF8563_IIC_Send_Byte(WriteAddr);
	PCF8563_IIC_Wait_Ack();
	PCF8563_IIC_Send_Byte(DataToWrite);
	PCF8563_IIC_Wait_Ack();
	PCF8563_IIC_Stop();
	rt_thread_mdelay(1);
}
void PCF8563_WriteTime(void)
{
	uint8_t i,temp;
	for(i=0;i<8;i++)
	{
		temp = time_buf1[i]/10;
		time_buf[i] = time_buf1[i]%10;
		time_buf[i] = time_buf[i]+temp*16;
	}
	PCF8563_WriteOneByte(CONTROL_STATUS_1,0X20);
	PCF8563_WriteOneByte(PCF8563_YEAR_ADDRESS,time_buf[1]);
	PCF8563_WriteOneByte(PCF8563_MONTH_ADDRESS,time_buf[2]);
	PCF8563_WriteOneByte(PCF8563_DAY_ADDRESS,time_buf[3]);
	PCF8563_WriteOneByte(PCF8563_HOUR_ADDRESS,time_buf[4]);
	PCF8563_WriteOneByte(PCF8563_MINUTE_ADDRESS,time_buf[5]);
	PCF8563_WriteOneByte(PCF8563_SECOND_ADDRESS,time_buf[6]);
	PCF8563_WriteOneByte(PCF8563_WEEK_ADDRESS,time_buf[7]);
	PCF8563_WriteOneByte(CONTROL_STATUS_1,0X00);
}
extern uint8_t time_buffer[8];
void PCF8563_ReadTime(uint8_t *p_time_buf)
{
	uint8_t i,temp;
	time_buf[1] = PCF8563_ReadOneByte(PCF8563_YEAR_ADDRESS)&0XFF;
	time_buf[2] = PCF8563_ReadOneByte(PCF8563_MONTH_ADDRESS)&0X1F;
	time_buf[3] = PCF8563_ReadOneByte(PCF8563_DAY_ADDRESS)&0X3F;
	time_buf[4] = PCF8563_ReadOneByte(PCF8563_HOUR_ADDRESS)&0X3F;
	time_buf[5] = PCF8563_ReadOneByte(PCF8563_MINUTE_ADDRESS)&0X7F;
	time_buf[6] = PCF8563_ReadOneByte(PCF8563_SECOND_ADDRESS)&0X7F;
	time_buf[7] = PCF8563_ReadOneByte(PCF8563_WEEK_ADDRESS)&0X0F;
	for(i=0;i<8;i++)
	{
		temp=time_buf[i]/16;
		time_buffer[i]=time_buf[i]%16;
		time_buffer[i]=time_buffer[i]+temp*10;
	}

}
void PCF8563_SetINT(void)
{
	PCF8563_WriteOneByte(0x01,0x11); //����״̬�Ĵ���2���ã�TI/TP=0;TIE=1
	PCF8563_WriteOneByte(0x0E,0x82);	//���ö�ʱ���������Ĵ��� TE=1,TD1=0,TD0=1(1Hz)
	PCF8563_WriteOneByte(0x0F,0x3C);	//���ö�ʱ������������ֵΪ60������ʱ1min
}
void PCF8563_ClearINT(void)  //����ж�TF��־λ
{
	PCF8563_WriteOneByte(0x01,0x11);
}
uint8_t PCF85636_ReadINT(void)
{
	uint8_t temp;
	temp=PCF8563_ReadOneByte(0x01);
	if(0x04==(0x04&temp))
		return 1;
	else
		return 0;
}