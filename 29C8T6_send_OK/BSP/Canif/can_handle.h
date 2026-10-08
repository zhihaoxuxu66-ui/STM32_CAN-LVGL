#ifndef __CAN_HANDLE_H__
#define __CAN_HANDLE_H__
#include "main.h"

	typedef struct
	{
	 uint32_t ui_value;
		uint8_t ui_freshflag;
	}ui_Typedef;
	
  extern ui_Typedef ui_water_temp;
	extern ui_Typedef ui_rpm;
	extern ui_Typedef ui_safty_belt;
	extern ui_Typedef ui_light;
	extern ui_Typedef ui_turn_light;
void can_handle(void);

#endif

