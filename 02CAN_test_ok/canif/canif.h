#ifndef __canif_H__
#define __canif_H__

#include "main.h"

void Can_Filter_Init();
uint32_t Can_Send_Msg(uint32_t id,uint32_t len,uint8_t *buf);
uint32_t Can_Recv_Msg(uint32_t *id,uint8_t *buf);


#endif