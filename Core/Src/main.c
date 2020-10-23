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
int main(void)
{
  /* USER CODE BEGIN 1 */

	char time_string[] = {'0', '0', ':', '0', '0', '\0'};
  unsigned long time_start_ms;
  unsigned long time_now_s;
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

EPD epd;
  if (EPD_Init(&epd, lut_full_update) != 0) {  //����ȫ��ˢ�»��Ǿֲ�ˢ��  lut_full_update
//    printf("e-Paper init failed\n");
    return -1;
  }
  
  EPD_ClearFrameMemory(&epd, 0xFF);
  
  Paint paint;
  Paint_Init (&paint, frame_buffer, 120, 250);
 // Paint_Clear(&paint, UNCOLORED);
  
  
  
  /* For simplicity, the argum ents are explicit numerical coordinates */
  /* Write strings to the buffer */
//  Paint_DrawFilledRectangle(&paint, 0, 0, 96, 12, COLORED);
//  EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, 96, 12);
/*   Ĭ�ϳ�����ʾ
  Paint_Clear(&paint, UNCOLORED);
  Paint_DrawStringAt(&paint, 0, 0, "23:59", &Font16, COLORED);
  EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, 128, 16);

  Paint_Clear(&paint, UNCOLORED);
  Paint_DrawStringAt(&paint, 0, 0, "Hello Kit!", &Font16, COLORED);
  EPD_SetFrameMemory(&epd, frame_buffer, 0, 16, 128, 16);
  
  */
  
//	time_string[0] = 1 + '0';
//    time_string[1] = 2 + '0';
//    time_string[3] = 5 + '0';
//    time_string[4] = 9 + '0';
//    Paint_SetWidth(&paint, 80);
//    Paint_SetHeight(&paint, 200);
    Paint_SetRotate(&paint, ROTATE_90);
    Paint_Clear(&paint, UNCOLORED);
	Paint_DrawStringAt(&paint, 0, 95, "2", &Font16, COLORED);  //2
	Paint_DrawStringAt(&paint, 9, 95, "0", &Font16, COLORED);  //0
	Paint_DrawStringAt(&paint, 18, 95, "2", &Font16, COLORED);  //2
	Paint_DrawStringAt(&paint, 27, 95, "0", &Font16, COLORED);  //0   9
	
	  Paint_DrawStringAt(&paint, 36, 90, "!", &Font21, COLORED);  //��  16
	  Paint_DrawStringAt(&paint, 52, 95, "1", &Font16, COLORED);  //2
	Paint_DrawStringAt(&paint, 61, 95, "0", &Font16, COLORED);  //0
	  Paint_DrawStringAt(&paint, 70, 90, "\"", &Font21, COLORED);		//��
	  Paint_DrawStringAt(&paint, 86, 95, "2", &Font16, COLORED);  //2
	Paint_DrawStringAt(&paint, 95, 95, "0", &Font16, COLORED);  //0
	  Paint_DrawStringAt(&paint, 104, 90, "#", &Font21, COLORED);    //��
	  
	  Paint_DrawStringAt(&paint, 128, 90,  " ", &Font21, COLORED);    //ʮ
	  Paint_DrawStringAt(&paint, 144, 90, "\"", &Font21, COLORED);		//��
	  Paint_DrawStringAt(&paint, 160, 90, "(", &Font21, COLORED);		//��
	  Paint_DrawStringAt(&paint, 176, 90, " ", &Font21, COLORED);    //ʮ
	  
	  Paint_DrawStringAt(&paint, 200, 90,  "*", &Font21, COLORED);    //��
	  Paint_DrawStringAt(&paint, 216, 90,  "+", &Font21, COLORED);    //��
	  Paint_DrawStringAt(&paint, 232, 90, "6", &Font21, COLORED);    //�� 
	  
	  Paint_DrawStringAt(&paint, 187, 2,  ".", &Font21, COLORED);    //��
	  Paint_DrawStringAt(&paint, 207, 2,  "0", &Font21, COLORED);    //��
	  
	  Paint_DrawStringAt(&paint, 187, 25,  "2", &Font20, COLORED);    //2
	  Paint_DrawStringAt(&paint, 200, 25,  "5", &Font20, COLORED);    //5
	  Paint_DrawStringAt(&paint, 210, 25,  ".", &Font20, COLORED);    //.
	  Paint_DrawStringAt(&paint, 220, 25,  "9", &Font20, COLORED);    //0
	  Paint_DrawStringAt(&paint, 234, 25,  "C", &Font20, COLORED);    //��
	  
	  Paint_DrawStringAt(&paint, 187, 41,  "/", &Font21, COLORED);    //��
	  Paint_DrawStringAt(&paint, 207, 41,  "0", &Font21, COLORED);    //��
	  
	  Paint_DrawStringAt(&paint, 187, 62,  "2", &Font20, COLORED);    //2
	  Paint_DrawStringAt(&paint, 200, 62,  "9", &Font20, COLORED);    //5
	  Paint_DrawStringAt(&paint, 210, 62,  ".", &Font20, COLORED);    //.
	  Paint_DrawStringAt(&paint, 220, 62,  "9", &Font20, COLORED);    //0
	  Paint_DrawStringAt(&paint, 234, 62,  "%", &Font20, COLORED);    //%
	  
	  
  EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, 72, 80);
  Paint_DrawRectangle(&paint, 0, 86, 250, 84, COLORED);
  Paint_DrawRectangle(&paint, 0, 85, 250, 83, COLORED);
	Paint_DrawStringAt(&paint, 0, 0, "12", &Font80, COLORED);
	Paint_DrawStringAt(&paint, 70, 0, ":", &Font80, COLORED);
	Paint_DrawStringAt(&paint, 100, 0, "34", &Font80, COLORED);
  EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, 120, 250);


//Paint_SetRotate(&paint, ROTATE_90);
//    Paint_Clear(&paint, UNCOLORED);
//	Paint_DrawStringAt(&paint, 0, 0, "59", &Font80, COLORED);
//  EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, 80, 200);
  
//  Paint_Clear(&paint, UNCOLORED);
//  Paint_DrawStringAt(&paint, 0, 0, "Hello Kit!", &Font16, COLORED);
//  EPD_SetFrameMemory(&epd, frame_buffer, 0, 16, 128, 16);
  
//    time_string[0] = 1 + '0';
//    time_string[1] = 2 + '0';
//    time_string[3] = 5 + '0';
//    time_string[4] = 9 + '0';
//    Paint_SetWidth(&paint, 80);
//    Paint_SetHeight(&paint, 200);
//    Paint_SetRotate(&paint, ROTATE_90);

//    Paint_Clear(&paint, UNCOLORED);
//    Paint_DrawStringAt(&paint, 0, 4, time_string, &Font80, COLORED);
//    EPD_SetFrameMemory(&epd, frame_buffer, 32, 0, Paint_GetWidth(&paint), Paint_GetHeight(&paint));
//    EPD_DisplayFrame(&epd);

  
//  EPD_DisplayFrame(&epd);
  
  
//  Paint_Init (&paint, frame_buffer, 72, 72);
//  Paint_Clear(&paint, UNCOLORED);
//  Paint_Init (&paint, frame_buffer, 64, 64);
  // Draw something to the frame buffer 
//  Paint_DrawRectangle(&paint, 0, 0, 40, 50, COLORED);
//  Paint_DrawLine(&paint, 0, 0, 40, 50, COLORED);
//  Paint_DrawLine(&paint, 40, 0, 0, 50, COLORED);
//  Paint_DrawCircle(&paint, 88, 80, 30, COLORED);
//  Paint_DrawFilledRectangle(&paint, 10, 120, 50, 180, COLORED);
//  Paint_DrawFilledCircle(&paint, 88, 150, 30, COLORED);
  
//  EPD_SetFrameMemory(&epd, frame_buffer, 0, 32, 72, 72);
  
  EPD_DisplayFrame(&epd);
  
  
//  
//  /* Display the frame_buffer */
//  EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, Paint_GetWidth(&paint), Paint_GetHeight(&paint));
//  EPD_DisplayFrame(&epd);
  EPD_DelayMs(&epd, 2000);

  /**
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to set the frame memory and refresh the display twice.
   */
//  EPD_ClearFrameMemory(&epd, 0xFF);
//  EPD_DisplayFrame(&epd);
//  EPD_ClearFrameMemory(&epd, 0xFF);
//  EPD_DisplayFrame(&epd);

  /* EPD_or partial update */
////  if (EPD_Init(&epd, lut_partial_update) != 0) {     //����ȫ��ˢ�»��Ǿֲ�ˢ��
//////    printf("e-Paper init failed\n");
////    return -1;
////  }

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

  time_start_ms = HAL_GetTick();

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

////    Paint_SetWidth(&paint, 32);
////    Paint_SetHeight(&paint, 96);
////    Paint_SetRotate(&paint, ROTATE_90);

////    Paint_Clear(&paint, UNCOLORED);
////    Paint_DrawStringAt(&paint, 0, 4, time_string, &Font24, COLORED);
////    EPD_SetFrameMemory(&epd, frame_buffer, 80, 72, Paint_GetWidth(&paint), Paint_GetHeight(&paint));
////    EPD_DisplayFrame(&epd);
	  
	  HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
	  HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);

	HAL_Delay(500);
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
