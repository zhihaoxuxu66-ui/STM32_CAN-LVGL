#include "canif.h"
#include "can.h"
#include "usart.h"

void Can_Filter_Init(void)
{
    
	
  CAN_FilterTypeDef CAN_FilterConfig;
	
  CAN_FilterConfig.FilterBank = 0;
  CAN_FilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  CAN_FilterConfig.FilterScale = CAN_FILTERSCALE_16BIT ;
  CAN_FilterConfig.FilterIdHigh = 0x0;
  CAN_FilterConfig.FilterIdLow = 0x0;
  CAN_FilterConfig.FilterMaskIdHigh = 0x0;
  CAN_FilterConfig.FilterMaskIdLow = 0x0;
  CAN_FilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  CAN_FilterConfig.FilterActivation = CAN_FILTER_ENABLE;
  CAN_FilterConfig.SlaveStartFilterBank = 14;

if (HAL_CAN_ConfigFilter(&hcan, &CAN_FilterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_CAN_Start(&hcan);
}


uint32_t Can_Send_Msg(uint32_t id,uint32_t len,uint8_t *buf)
{

   CAN_TxHeaderTypeDef TxMessage;
		
	 uint32_t Tx_Mail = CAN_TX_MAILBOX0;
		TxMessage.StdId = id;
		TxMessage.IDE = CAN_ID_STD;
		TxMessage.DLC = 8;
		TxMessage.RTR = CAN_RTR_DATA;
		
		HAL_CAN_AddTxMessage(&hcan, &TxMessage,buf, &Tx_Mail);
		printf(" fend ok\r\n ");

}

uint32_t Can_Recv_Msg(uint32_t *id,uint8_t *buf)
{
   CAN_RxHeaderTypeDef RxMessage;
		
		HAL_CAN_GetRxMessage(&hcan, CAN_FILTER_FIFO0,&RxMessage,buf);
     *id = RxMessage.StdId;
	   return RxMessage.DLC;

}



