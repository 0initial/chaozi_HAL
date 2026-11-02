#include "key.h"
#include "led.h"

static uint8_t key1_state;
static uint8_t key2_state;

void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeStruct.Pin = KEY1_PIN | KEY2_PIN;
	GPIO_InitTypeStruct.Mode = GPIO_MODE_INPUT;

	GPIO_InitTypeStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(KEY1_PROT, &GPIO_InitTypeStruct);
	HAL_GPIO_Init(KEY2_PROT, &GPIO_InitTypeStruct);
}

void KEY_Init_IT(void)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeStruct.Pin = KEY1_PIN;
	GPIO_InitTypeStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitTypeStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(KEY1_PROT, &GPIO_InitTypeStruct);

	GPIO_InitTypeStruct.Pin = KEY2_PIN;
	GPIO_InitTypeStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitTypeStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(KEY2_PROT, &GPIO_InitTypeStruct);

	// 设置外部中断
	HAL_NVIC_SetPriority(EXTI0_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
 * @brief  按键1扫描函数
 * @param  mode ：0 按下执行 1：抬起执行
 * @retval 0：无按键按下 1：按键1按下
 */
uint8_t Key_Scan(uint8_t mode)
{
	if ((KEY1_IN() == GPIO_PIN_SET) && (key1_state == 0))
	{
		// 如果在循环内，检测到按键松开，则认为是干扰
		for (uint32_t i = 0; i < 0x7FFF; i++)
		{
			if (KEY1_IN() == GPIO_PIN_RESET)
			{
				return 0;
			}
		}
		key1_state = 1;

		// 如果推出了for循环，表面按键一直处于按下，可以进行按键按下逻辑
		if (mode == 0)
		{
			return 1;
		}
	}
	else if ((KEY1_IN() == GPIO_PIN_RESET) && (key1_state == 1))
	{
		for (uint32_t i = 0; i < 0x7FFF; i++)
		{
			if (KEY1_IN() == GPIO_PIN_SET)
			{
				return 0;
			}
		}
		key1_state = 0;
		if (mode == 1)
		{
			return 1;
		}
	}

	if ((KEY2_IN() == GPIO_PIN_SET) && (key2_state == 0))
	{
		// 如果在循环内，检测到按键松开，则认为是干扰
		for (uint32_t i = 0; i < 0x7FFF; i++)
		{
			if (KEY2_IN() == GPIO_PIN_RESET)
			{
				return 0;
			}
		}
		key2_state = 1;

		// 如果推出了for循环，表面按键一直处于按下，可以进行按键按下逻辑
		if (mode == 0)
		{
			return 2;
		}
	}
	else if ((KEY2_IN() == GPIO_PIN_RESET) && (key2_state == 1))
	{
		for (uint32_t i = 0; i < 0x7FFF; i++)
		{
			if (KEY2_IN() == GPIO_PIN_SET)
			{
				return 0;
			}
		}
		key2_state = 0;
		if (mode == 1)
		{
			return 2;
		}
	}

	return 0;
}

void KEY_Init_Event(void)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeStruct.Pin = KEY1_PIN | KEY2_PIN;
	GPIO_InitTypeStruct.Mode = GPIO_MODE_EVT_RISING;
	GPIO_InitTypeStruct.Pull = GPIO_PULLDOWN;
	
	HAL_GPIO_Init(KEY1_PROT, &GPIO_InitTypeStruct);
	HAL_GPIO_Init(KEY2_PROT, &GPIO_InitTypeStruct);
}

void KEY_Init_EventOut(void)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_AFIO_CLK_ENABLE();

	GPIO_InitTypeStruct.Pin = GPIO_PIN_3;
	GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitTypeStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOA, &GPIO_InitTypeStruct);
	
	// 配置哪个端口，和端口的几号
	HAL_GPIOEx_ConfigEventout(AFIO_EVENTOUT_PORT_A, AFIO_EVENTOUT_PIN_3);
	// 然后要记得开启外部唤醒
	HAL_GPIOEx_EnableEventout();
}

// 中断回调函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch (GPIO_Pin)
	{
	case KEY1_PIN:
		if (KEY1_IN() == GPIO_PIN_SET)
		{
			// 如果在循环内，检测到按键松开，则认为是干扰
			for (uint32_t i = 0; i < 0x7FFF; i++)
			{
				if (KEY1_IN() == GPIO_PIN_RESET)
				{
					return;
				}
			}
			HAL_GPIO_TogglePin(LED_RED_PORT, LED_RED_PIN);
		}
		break;

	case KEY2_PIN:
		if (KEY2_IN() == GPIO_PIN_SET)
		{
			// 如果在循环内，检测到按键松开，则认为是干扰
			for (uint32_t i = 0; i < 0x7FFF; i++)
			{
				if (KEY2_IN() == GPIO_PIN_RESET)
				{
					return;
				}
			}
			HAL_GPIO_TogglePin(LED_RED_PORT, LED_RED_PIN);
		}
		break;

	default:
		break;
	}
}
