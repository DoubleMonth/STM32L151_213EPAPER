#include "key/key.h"
#include "rtthread.h"
void keyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(KEY1_GPIO_Port, KEY1_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : PAPin PAPin */
	GPIO_InitStruct.Pin = KEY1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(KEY1_GPIO_Port, &GPIO_InitStruct);
	  
	//中断
	HAL_NVIC_SetPriority(EXTI3_IRQn,2,2); //抢占优先级为2，子优先级为2
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);   //使能中断线3
}  
void EXTI3_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);		//调用中断处理公用函数
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
//	rt_thread_mdelay(20);
	if(GPIO_PIN_3==GPIO_Pin)
	{
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_15);
	}
}