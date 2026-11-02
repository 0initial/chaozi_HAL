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
	}
}

void CAN_FilterInit(void)
{
	can1_filter.FilterIdHigh = 0x0000;									 // 滤波器ID高16位：设为0（配合掩码模式，表示不关心高16位）
	can1_filter.FilterIdLow = 0x0000;										 // 滤波器ID低16位：设为0（实际匹配时由掩码决定哪些位有效）
	can1_filter.FilterMaskIdHigh = 0x0000;							 // 掩码高16位：全0 → 所有高16位“不参与匹配”（即任意值都通过）
	can1_filter.FilterMaskIdLow = 0x0000;								 // 掩码低16位：全0 → 所有低16位“不参与匹配”  效果：接收所有CAN报文（ID通配，无过滤）
	can1_filter.FilterFIFOAssignment = CAN_FILTER_FIFO0; // 匹配的报文放入 FIFO0
	can1_filter.FilterBank = 0;													 // 使用第0号滤波器组（bank 0）
	can1_filter.FilterMode = CAN_FILTERMODE_IDMASK;			 // 工作在掩码模式（ID + Mask 方式过滤）
	can1_filter.FilterScale = CAN_FILTERSCALE_32BIT;		 // 每个滤波器组使用32位宽（支持完整29位扩展ID）
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

	can1_tx.StdId = 0x123;								// 设置标准帧标识符（11位ID），范围 0x000 ~ 0x7FF
	can1_tx.ExtId = 0x12345673;						// 设置扩展帧标识符（29位ID），范围 0x00000000 ~ 0x1FFFFFFF
	can1_tx.IDE = CAN_ID_STD;							// 指定帧格式：使用标准帧（11位ID）
	can1_tx.RTR = CAN_RTR_DATA;						// 指定帧类型：发送数据帧（携带有效数据）
	can1_tx.DLC = len;										// 指定数据长度（Data Length Code），单位：字节  必须在 0~8 范围内，超出部分将被截断或导致发送失败
	can1_tx.TransmitGlobalTime = DISABLE;	// 是否启用全局时间戳功能（Time Triggered Communication）
	res = HAL_CAN_AddTxMessage(&can1, &can1_tx, Tdata, &pTxMailbox_Num);// 将报文加入CAN发送邮箱（非阻塞）
	u1_printf("res = %d 邮箱编号 = %d\r\n", res, pTxMailbox_Num);				// 打印发送结果：
																																			// - res = HAL_OK (0) 表示成功入队
																																			// - pTxMailbox_Num = 0/1/2，表示使用了哪个硬件发送邮箱
}

uint8_t CAN_RxData(uint8_t *Rdata)
{
	uint32_t id;
	uint8_t res;

	res = HAL_CAN_GetRxMessage(&can1, CAN_RX_FIFO0, &can1_rx, Rdata);
	if(res == HAL_ERROR)
	{
		return 0;
	}

	if(can1_rx.IDE == CAN_ID_STD)
	{
		u1_printf("标准\r\n");
		id = can1_rx.StdId;
	}
	else
	{
		u1_printf("扩展\r\n");
		id = can1_rx.ExtId;
	}

	if(can1_rx.RTR == CAN_RTR_DATA)
	{
		u1_printf("数据帧 ID(%x)\r\n", id);
		for(uint8_t i = 0; i < can1_rx.DLC; i++)
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













