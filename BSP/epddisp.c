#include "epddisp.h"
#include "epd2in13.h"
#include "epdif.h"
#include "epdpaint.h"
#include "imagedata.h"
#include <stdlib.h>
#include "pcf8563/pcf8563.h"
#include "si7020/si7020_iic.h"
#include <rtthread.h>
#include "usart.h"
unsigned char frame_buffer[4096] = {0};
EPD epd;
Paint paint;
uint8_t time_buffer[8]={0};
uint8_t time_buffer1[8]={0};
char time_string[] = {'0', '\0'};
char* time_table[]={"0","1","2","3","4","5","6","7","8","9"};
char* userchar[]={"",};
double si7020_temperature;
double si7020_humidity;
uint8_t setPartialUpdate(void)
{
	if (EPD_Init(&epd, lut_partial_update) != 0) 
	{     //设置局部刷新
		return -1;
	}
}
uint8_t setFullUpdate(void)
{
	if (EPD_Init(&epd, lut_full_update) != 0) 
	{  //设置全局刷新还是局部刷新  lut_full_update
		return -1;
	}
}
uint8_t epdDispFullUpdate(void)
{
	
	uint16_t u16_si7020_temperature;
	uint16_t u16_si7020_humidity;
	
	setFullUpdate();  //设置为全局刷新
	EPD_ClearFrameMemory(&epd, 0xFF);

	Paint_Init (&paint, frame_buffer, 120, 250);
	Paint_SetRotate(&paint, ROTATE_90);
	Paint_Clear(&paint, UNCOLORED);
	PCF8563_ReadTime(time_buffer1);
	si7020Measure(&si7020_temperature,&si7020_humidity);
	Paint_DrawStringAt(&paint, 0, 95, "2", &Font16, COLORED);  //2
	Paint_DrawStringAt(&paint, 9, 95, "0", &Font16, COLORED);  //0
	Paint_DrawStringAt(&paint, 18, 95, time_table[time_buffer[1]  / 10], &Font16, COLORED);  //2
	Paint_DrawStringAt(&paint, 27, 95, time_table[time_buffer[1] % 10], &Font16, COLORED);  //0   9

	Paint_DrawStringAt(&paint, 37, 90, "!", &Font21, COLORED);  //年  16
	Paint_DrawStringAt(&paint, 52, 95, time_table[time_buffer[2]  / 10] , &Font16, COLORED);  //2
	Paint_DrawStringAt(&paint, 61, 95, time_table[time_buffer[2]  % 10], &Font16, COLORED);  //0
	Paint_DrawStringAt(&paint, 70, 90, "\"", &Font21, COLORED);		//月
	Paint_DrawStringAt(&paint, 86, 95, time_table[time_buffer[3]  / 10], &Font16, COLORED);  //2
	Paint_DrawStringAt(&paint, 95, 95, time_table[time_buffer[3]  % 10], &Font16, COLORED);  //0
	Paint_DrawStringAt(&paint, 104, 90, "#", &Font21, COLORED);    //日

	Paint_DrawStringAt(&paint, 128, 90,  " ", &Font21, COLORED);    //十
	Paint_DrawStringAt(&paint, 144, 90, "\"", &Font21, COLORED);		//月
	Paint_DrawStringAt(&paint, 160, 90, "(", &Font21, COLORED);		//初
	Paint_DrawStringAt(&paint, 176, 90, " ", &Font21, COLORED);    //十

	Paint_DrawStringAt(&paint, 200, 90,  "*", &Font21, COLORED);    //星
	Paint_DrawStringAt(&paint, 216, 90,  "+", &Font21, COLORED);    //期
	time_string[0]=(time_buffer[7]  % 10)+ '0';
	Paint_DrawStringAt(&paint, 232, 90, time_string, &Font21, COLORED);    //六 

	Paint_DrawStringAt(&paint, 187, 2,  ".", &Font21, COLORED);    //温
	Paint_DrawStringAt(&paint, 207, 2,  "0", &Font21, COLORED);    //度

	if(si7020_temperature>=0)
	{
		u16_si7020_temperature=(uint16_t)(si7020_temperature*10);
		u16_si7020_humidity=(uint16_t)(si7020_humidity*10);
		Paint_DrawStringAt(&paint, 187, 25,  time_table[u16_si7020_temperature  / 100] , &Font20, COLORED);    //2
		Paint_DrawStringAt(&paint, 200, 25,  time_table[u16_si7020_temperature  % 100 /10], &Font20, COLORED);    //5
		Paint_DrawStringAt(&paint, 210, 25,  ".", &Font20, COLORED);    //.
		Paint_DrawStringAt(&paint, 220, 25,  time_table[u16_si7020_temperature  % 10], &Font20, COLORED);    //0
		Paint_DrawStringAt(&paint, 234, 25,  "C", &Font20, COLORED);    //℃
	}
	

	Paint_DrawStringAt(&paint, 187, 41,  "/", &Font21, COLORED);    //湿
	Paint_DrawStringAt(&paint, 207, 41,  "0", &Font21, COLORED);    //度


	Paint_DrawStringAt(&paint, 187, 62,  time_table[u16_si7020_humidity  / 100], &Font20, COLORED);    //2
	Paint_DrawStringAt(&paint, 200, 62,  time_table[u16_si7020_humidity  % 100/10], &Font20, COLORED);    //5
	Paint_DrawStringAt(&paint, 210, 62,  ".", &Font20, COLORED);    //.
	Paint_DrawStringAt(&paint, 220, 62,  time_table[u16_si7020_humidity  % 10], &Font20, COLORED);    //0
	Paint_DrawStringAt(&paint, 234, 62,  "%", &Font20, COLORED);    //%

	Paint_DrawStringAt(&paint, 0, 0, time_table[time_buffer[4]  / 10], &Font80, COLORED);   //小时
	Paint_DrawStringAt(&paint, 40, 0, time_table[time_buffer[4]  % 10], &Font80, COLORED);   //小时
	Paint_DrawStringAt(&paint, 70, 0, ":", &Font80, COLORED);	//：
	Paint_DrawStringAt(&paint, 100, 0, time_table[time_buffer[5]  / 10], &Font80, COLORED);   //小时
	Paint_DrawStringAt(&paint, 140, 0, time_table[time_buffer[5]  % 10], &Font80, COLORED);   //小时
//	Paint_DrawStringAt(&paint, 100, 0, "34", &Font80, COLORED);//分钟

	Paint_DrawRectangle(&paint, 0, 86, 250, 84, COLORED);   //画两个矩形显示成黑色条
	Paint_DrawRectangle(&paint, 0, 85, 250, 83, COLORED);	
	EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, 120, 250);
	EPD_DisplayFrame(&epd);
	rt_kprintf("%d%d:%d%d,%d%d:%d%d\n",time_buffer[0]/10,time_buffer[0]%10,time_buffer[1]/10,time_buffer[1]%10,time_buffer[2]/10,time_buffer[2]%10,time_buffer[6]/10,time_buffer[6]%10);
	rt_kprintf("%dC,%dRH\n",u16_si7020_temperature,u16_si7020_humidity);
}
void epdSleep(void)
{
	EPD_Sleep(&epd);
}
void updateMinute(void)
{
	PCF8563_ReadTime(time_buffer1);
	Paint_SetRotate(&paint, ROTATE_90);

	Paint_Clear(&paint, UNCOLORED);
	Paint_DrawStringAt(&paint, 0, 0, time_table[time_buffer[4]  / 10], &Font80, COLORED);   //小时
	Paint_DrawStringAt(&paint, 40, 0, time_table[time_buffer[4]  % 10], &Font80, COLORED);   //小时
	Paint_DrawStringAt(&paint, 70, 0, ":", &Font80, COLORED);	//：
	Paint_DrawStringAt(&paint, 100, 0, time_table[time_buffer[6]  / 10], &Font80, COLORED);   //小时
	Paint_DrawStringAt(&paint, 140, 0, time_table[time_buffer[6]  % 10], &Font80, COLORED);   //小时
	EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, Paint_GetWidth(&paint), Paint_GetHeight(&paint));
	EPD_DisplayFrame(&epd);
}
