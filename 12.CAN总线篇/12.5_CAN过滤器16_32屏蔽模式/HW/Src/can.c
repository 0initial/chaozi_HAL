#include "can.h"
#include "uart.h"

CAN_HandleTypeDef can1;				 // 控制结构体
CAN_FilterTypeDef can1_filter; // 过滤器控制结构体
CAN_TxHeaderTypeDef can1_tx;	 // 发送控制结构体
CAN_RxHeaderTypeDef can1_rx;	 // 接收控制结构体

void CAN1_Init(void)
{
	can1.Instance = CAN1;											// 指定使用CAN1外设
	can1.Init.Prescaler = 24;									// 波特率预分频值，配合Tq配置实现100kbps
	can1.Init.Mode = CAN_MODE_NORMAL;					// 工作模式：正常通信模式
	can1.Init.SyncJumpWidth = CAN_SJW_2TQ;		// 重同步跳转宽度：2个时间量子（Tq）
	can1.Init.TimeSeg1 = CAN_BS1_8TQ;					// 位段1（含Prop_Seg + Phase_Seg1）：8 Tq
	can1.Init.TimeSeg2 = CAN_BS2_6TQ;					// 位段2（Phase_Seg2）：6 Tq
	can1.Init.TimeTriggeredMode = DISABLE;		// 禁用时间触发通信模式
	can1.Init.AutoBusOff = DISABLE;						// 禁用自动离线管理
	can1.Init.AutoWakeUp = DISABLE;						// 禁用自动唤醒
	can1.Init.AutoRetransmission = DISABLE;		// 禁用自动重传（适用于单次发送）
	can1.Init.ReceiveFifoLocked = DISABLE;		// 接收FIFO未锁定，溢出时覆盖旧数据
	can1.Init.TransmitFifoPriority = DISABLE; // 发送FIFO按标识符优先级，非请求顺序优先级
	HAL_CAN_Init(&can1);											// 调用HAL库初始化CAN1
	CAN_FilterInit();
	HAL_CAN_ActivateNotification(&can1, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_Start(&can1);
}

void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;

	if (hcan->Instance == CAN1)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_CAN1_CLK_ENABLE();
		__HAL_RCC_AFIO_CLK_ENABLE();

		// 重映射操作
		__HAL_AFIO_REMAP_CAN1_2();

		GPIO_InitTypeStruct.Pin = CAN1_TX_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitTypeStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(CAN1_TX_PORT, &GPIO_InitTypeStruct);

		GPIO_InitTypeStruct.Pin = CAN1_RX_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_INPUT;
		GPIO_InitTypeStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(CAN1_RX_PORT, &GPIO_InitTypeStruct);

		// 开启CAN接收中断
		HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
	}
}

void CAN_FilterInit(void)
{
	// 第一个滤波器设置
	can1_filter.FilterIdHigh = 0x100 << 5;    // Filter ID 高16位：标准ID=0x100，左移5位对齐寄存器
	can1_filter.FilterMaskIdHigh = 0xE018;    // 掩码高16位：数据为1的二进制位是ID范围中必须匹配的位
	
	can1_filter.FilterIdLow = 0x200 << 5;       // Filter ID 低16位：标准ID=0x200，左移5位对齐寄存器
	can1_filter.FilterMaskIdLow = 0xE018;       // 掩码高16位：数据为1的二进制位是ID范围中必须匹配的位
	
	can1_filter.FilterFIFOAssignment = CAN_FILTER_FIFO0; // 匹配的报文放入 FIFO0
	can1_filter.FilterBank = 0;													 // 使用第0号滤波器组（bank 0）
	can1_filter.FilterMode = CAN_FILTERMODE_IDMASK;			 // 工作在屏蔽模式
	can1_filter.FilterScale = CAN_FILTERSCALE_16BIT;		 // 每个滤波器组使用16位宽（支持完整29位扩展ID）
	can1_filter.FilterActivation = CAN_FILTER_ENABLE;		 // 启用该滤波器
	can1_filter.SlaveStartFilterBank = 14;							 // 单CAN芯片下此参数无效
																											 // 仅在双CAN控制器（如STM32F4/F7带CAN1+CAN2）时用于划分滤波器资源；
																											 // 单CAN芯片（如STM32F1）忽略此字段，可任意设置（通常保留默认值14）
	HAL_CAN_ConfigFilter(&can1, &can1_filter);					 // 应用滤波器配置
	
	// 第二个滤波器设置
	can1_filter.FilterIdHigh = ((((uint32_t)0x12345670 << 3) | (1 << 2)) & 0xFFFF0000) >> 16;		// 扩展ID：0x12345670 放入高位段
	can1_filter.FilterIdLow = (((uint32_t)0x12345670 << 3) | (1 << 2)) & 0x0000FFFF;						// 扩展ID：0x12345670 放入低位段
	
	can1_filter.FilterMaskIdHigh = (0xFFFFFF86 & 0xFFFF0000) >> 16;		// 掩码：0xFFFFFF86 放入高位段
	can1_filter.FilterMaskIdLow = 0xFFFFFF86 & 0x0000FFFF;						// 掩码：0xFFFFFF86 放入低位段
	
	can1_filter.FilterFIFOAssignment = CAN_FILTER_FIFO0; // 匹配的报文放入 FIFO0
	can1_filter.FilterBank = 1;													 // 使用第1号滤波器组（bank 1）
	can1_filter.FilterMode = CAN_FILTERMODE_IDMASK;			 // 工作在屏蔽模式
	can1_filter.FilterScale = CAN_FILTERSCALE_32BIT;		 // 32位模式：支持完整的29位扩展ID
	can1_filter.FilterActivation = CAN_FILTER_ENABLE;		 // 启用该滤波器
	can1_filter.SlaveStartFilterBank = 14;							 // 单CAN芯片下此参数无效
																											 // 仅在双CAN控制器（如STM32F4/F7带CAN1+CAN2）时用于划分滤波器资源；
																											 // 单CAN芯片（如STM32F1）忽略此字段，可任意设置（通常保留默认值14）
	HAL_CAN_ConfigFilter(&can1, &can1_filter);					 // 应用滤波器配置
}

void CAN_TxData(uint8_t *Tdata, uint8_t len)
{
	uint32_t pTxMailbox_Num;
	uint8_t res;

	can1_tx.StdId = 0x123;
	can1_tx.ExtId = 0x12345673;
	can1_tx.IDE = CAN_ID_STD;
	can1_tx.RTR = CAN_RTR_DATA;
	can1_tx.DLC = len;
	can1_tx.TransmitGlobalTime = DISABLE;
	res = HAL_CAN_AddTxMessage(&can1, &can1_tx, Tdata, &pTxMailbox_Num);
	u1_printf("res = %d 邮箱编号 = %d\r\n", res, pTxMailbox_Num);
}

uint8_t CAN_RxData(uint8_t *Rdata)
{
	uint32_t id;
	uint8_t res;

	res = HAL_CAN_GetRxMessage(&can1, CAN_RX_FIFO0, &can1_rx, Rdata);
	if (res == HAL_ERROR)
	{
		return 0;
	}

	if (can1_rx.IDE == CAN_ID_STD)
	{
		u1_printf("标准\r\n");
		id = can1_rx.StdId;
	}
	else
	{
		u1_printf("扩展\r\n");
		id = can1_rx.ExtId;
	}

	if (can1_rx.RTR == CAN_RTR_DATA)
	{
		u1_printf("数据帧 ID(%x)\r\n", id);
		for (uint8_t i = 0; i < can1_rx.DLC; i++)
		{
			u1_printf("%c \r\n", Rdata[i]);
		}
		u1_printf("\r\n");
	}
	else
	{
		u1_printf("远程帧 ID(%x)\r\n", id);
		u1_printf("希望请求%d字节数据\r\n", can1_rx.DLC);
	}

	return can1_rx.DLC;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if (hcan->Instance == CAN1)
	{
		uint32_t id;
		uint8_t Rxbuff[16];

		HAL_CAN_GetRxMessage(&can1, CAN_RX_FIFO0, &can1_rx, Rxbuff);

		if (can1_rx.IDE == CAN_ID_STD)
		{
			u1_printf("标准\r\n");
			id = can1_rx.StdId;
		}
		else
		{
			u1_printf("扩展\r\n");
			id = can1_rx.ExtId;
		}

		if (can1_rx.RTR == CAN_RTR_DATA)
		{
			u1_printf("数据帧 ID(%x)\r\n", id);
			for (uint8_t i = 0; i < can1_rx.DLC; i++)
			{
				u1_printf("%c \r\n", Rxbuff[i]);
			}
			u1_printf("\r\n");
		}
		else
		{
			u1_printf("远程帧 ID(%x)\r\n", id);
			u1_printf("希望请求%d字节数据\r\n", can1_rx.DLC);
		}
	}
}
