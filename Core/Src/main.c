/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "epd2in13.h"
#include "epdif.h"
#include "epdpaint.h"
#include "imagedata.h"
#include <stdlib.h>
#include "pcf8563/pcf8563.h"
#include "si7020/si7020_iic.h"
#include "key/key.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define COLORED      0
#define UNCOLORED    1
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
unsigned char frame_buffer[4096] = {0};
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
uint8_t time_buffer[8]={0};
uint8_t time_buffer1[8]={0};
double si7020_temperature;
double si7020_humidity;
int main(void)
{
  /* USER CODE BEGIN 1 */

	char time_string[] = {'0', '0', ':', '0', '0', '\0'};
	char* time_table[]={"0","1","2","3","4","5","6","7","8","9"};
  unsigned long time_start_ms;
  unsigned long time_now_s;
	char temp;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  PCF8563_IIC_Init();
  PCF8563_SetINT();
  SI7020_IIC_Init();
  MX_USART1_UART_Init();
  PCF8563_WriteTime();
  keyInit();
  /* USER CODE BEGIN 2 */
	rt_kprintf("Init Finsh!\n");

/*	

	EPD epd;
  if (EPD_Init(&epd, lut_full_update) != 0) {  //设置全局刷新还是局部刷新  lut_full_update
    return -1;
  }
  
  EPD_ClearFrameMemory(&epd, 0xFF);
  
  Paint paint;
  Paint_Init (&paint, frame_buffer, 120, 250);
    Paint_SetRotate(&paint, ROTATE_90);
    Paint_Clear(&paint, UNCOLORED);
	Paint_DrawStringAt(&paint, 0, 95, "2", &Font16, COLORED);  //2
	Paint_DrawStringAt(&paint, 9, 95, "0", &Font16, COLORED);  //0
	Paint_DrawStringAt(&paint, 18, 95, "2", &Font16, COLORED);  //2
	Paint_DrawStringAt(&paint, 27, 95, "0", &Font16, COLORED);  //0   9
	
	  Paint_DrawStringAt(&paint, 36, 90, "!", &Font21, COLORED);  //年  16
	  Paint_DrawStringAt(&paint, 52, 95, "1", &Font16, COLORED);  //2
	Paint_DrawStringAt(&paint, 61, 95, "0", &Font16, COLORED);  //0
	  Paint_DrawStringAt(&paint, 70, 90, "\"", &Font21, COLORED);		//月
	  Paint_DrawStringAt(&paint, 86, 95, "2", &Font16, COLORED);  //2
	Paint_DrawStringAt(&paint, 95, 95, "0", &Font16, COLORED);  //0
	  Paint_DrawStringAt(&paint, 104, 90, "#", &Font21, COLORED);    //日
	  
	  Paint_DrawStringAt(&paint, 128, 90,  " ", &Font21, COLORED);    //十
	  Paint_DrawStringAt(&paint, 144, 90, "\"", &Font21, COLORED);		//月
	  Paint_DrawStringAt(&paint, 160, 90, "(", &Font21, COLORED);		//初
	  Paint_DrawStringAt(&paint, 176, 90, " ", &Font21, COLORED);    //十
	  
	  Paint_DrawStringAt(&paint, 200, 90,  "*", &Font21, COLORED);    //星
	  Paint_DrawStringAt(&paint, 216, 90,  "+", &Font21, COLORED);    //期
	  Paint_DrawStringAt(&paint, 232, 90, "6", &Font21, COLORED);    //六 
	  
	  Paint_DrawStringAt(&paint, 187, 2,  ".", &Font21, COLORED);    //温
	  Paint_DrawStringAt(&paint, 207, 2,  "0", &Font21, COLORED);    //度
	  
	  Paint_DrawStringAt(&paint, 187, 25,  "2", &Font20, COLORED);    //2
	  Paint_DrawStringAt(&paint, 200, 25,  "5", &Font20, COLORED);    //5
	  Paint_DrawStringAt(&paint, 210, 25,  ".", &Font20, COLORED);    //.
	  Paint_DrawStringAt(&paint, 220, 25,  "9", &Font20, COLORED);    //0
	  Paint_DrawStringAt(&paint, 234, 25,  "C", &Font20, COLORED);    //℃
	  
	  Paint_DrawStringAt(&paint, 187, 41,  "/", &Font21, COLORED);    //湿
	  Paint_DrawStringAt(&paint, 207, 41,  "0", &Font21, COLORED);    //度
	

	  Paint_DrawStringAt(&paint, 187, 62,  "2", &Font20, COLORED);    //2
	  Paint_DrawStringAt(&paint, 200, 62,  "9", &Font20, COLORED);    //5
	  Paint_DrawStringAt(&paint, 210, 62,  ".", &Font20, COLORED);    //.
	  Paint_DrawStringAt(&paint, 220, 62,  "9", &Font20, COLORED);    //0
	  Paint_DrawStringAt(&paint, 234, 62,  "%", &Font20, COLORED);    //%
	  
	  Paint_DrawStringAt(&paint, 0, 0, "12", &Font80, COLORED);   //小时
	Paint_DrawStringAt(&paint, 70, 0, ":", &Font80, COLORED);	//：
	Paint_DrawStringAt(&paint, 100, 0, "34", &Font80, COLORED);//分钟
	
  Paint_DrawRectangle(&paint, 0, 86, 250, 84, COLORED);   //画两个矩形显示成黑色条
  Paint_DrawRectangle(&paint, 0, 85, 250, 83, COLORED);	
  EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, 120, 250);
  EPD_DisplayFrame(&epd);

rt_thread_mdelay(2000);

*/
//  if (EPD_Init(&epd, lut_partial_update) != 0) {     //设置全局刷新还是局部刷新
//    return -1;
//  }

  /**
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to set the frame memory and refresh the display twice.
   */
  
//  Paint_Init (&paint, frame_buffer, 64, 64);
//  Paint_Clear(&paint, UNCOLORED);
//  EPD_SetFrameMemory(&epd, IMAGE_DATA, 0, 0, epd.width, epd.height);
//  EPD_DisplayFrame(&epd);
//  EPD_SetFrameMemory(&epd, IMAGE_DATA, 0, 0, epd.width, epd.height);
//  EPD_DisplayFrame(&epd);

//  time_start_ms = HAL_GetTick();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
////	  time_now_s = (HAL_GetTick() - time_start_ms) / 1000;
////    time_string[0] = time_now_s / 60 / 10 + '0';
////    time_string[1] = time_now_s / 60 % 10 + '0';
////    time_string[3] = time_now_s % 60 / 10 + '0';
////    time_string[4] = time_now_s % 60 % 10 + '0';

////    Paint_SetRotate(&paint, ROTATE_90);

////    Paint_Clear(&paint, UNCOLORED);
////    Paint_DrawStringAt(&paint, 0, 0, time_table[time_now_s / 60 / 10], &Font80, COLORED);
////	  Paint_DrawStringAt(&paint, 40, 0, time_table[time_now_s / 60% 10], &Font80, COLORED);
////	  Paint_DrawStringAt(&paint, 70, 0, ":", &Font80, COLORED);	//：
////	  Paint_DrawStringAt(&paint, 100, 0, time_table[time_now_s % 60 / 10], &Font80, COLORED);
////	  Paint_DrawStringAt(&paint, 140, 0, time_table[time_now_s % 60 % 10], &Font80, COLORED);
////    EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, Paint_GetWidth(&paint), Paint_GetHeight(&paint));
////    EPD_DisplayFrame(&epd);
////	  
//	  HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
//	  HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
//	  rt_thread_mdelay(500);

	/*    PCF8563 1分钟中断输出 start
	while(PCF85636_ReadINT()==0)
	{;}
	  HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
	  HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
	  PCF8563_ClearINT();
	  
	PCF8563 1分钟中断输出 end   */
		/*
	  PCF8563_ReadTime(time_buffer);
	 rt_kprintf("%d%d:%d%d,%d%d:%d%d\n",time_buffer[0]/10,time_buffer[0]%10,time_buffer[1]/10,time_buffer[1]%10,time_buffer[2]/10,time_buffer[2]%10,time_buffer[3]/10,time_buffer[3]%10);
//	rt_kprintf("%d%d:%d%d\n",time_buffer[4]%10,time_buffer[5]%10,time_buffer[6]%10,time_buffer[7]%10);
	rt_thread_mdelay(500);
	si7020Measure(&si7020_temperature,&si7020_humidity);
	rt_kprintf("temperature=%d\n",(uint16_t)(si7020_temperature*10));
		*/
////	HAL_Delay(1000);
////    EPD_DelayMs(&epd, 500);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
