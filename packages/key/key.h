#ifndef _KEY_H
#define _KEY_H
#include "stm32l1xx_hal.h"

#define KEY1_Pin GPIO_PIN_8
#define KEY1_GPIO_Port GPIOA

void keyInit(void);
#endif
