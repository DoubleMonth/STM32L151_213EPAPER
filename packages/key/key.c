#include "key/key.h"
#include "rtthread.h"
#include "main.h"
extern  void SystemClock_Config(void);
void keyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(KEY1_GPIO_Port, KEY1_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : PAPin PAPin */
	GPIO_InitStruct.Pin = KEY1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(KEY1_GPIO_Port, &GPIO_InitStruct);
	  
	//中断
	HAL_NVIC_SetPriority(EXTI9_5_IRQn,2,2); //抢占优先级为2，子优先级为2
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);   //使能中断线3
}  
void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);		//调用中断处理公用函数
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
//	rt_thread_mdelay(20);
//	SystemClock_Config();
	if(GPIO_PIN_8==GPIO_Pin)
	{
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_15);
	}
	
}