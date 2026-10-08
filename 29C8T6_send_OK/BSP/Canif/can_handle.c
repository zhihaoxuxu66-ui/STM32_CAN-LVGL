#include "main.h"
#include "canif.h"
#include "can.h"
#include "can_handle.h"

	
	ui_Typedef ui_water_temp;
	ui_Typedef ui_rpm;
	ui_Typedef ui_safty_belt;
	ui_Typedef ui_light;
	ui_Typedef ui_turn_light;
void can_handle(void)
{
   uint32_t recv_len;
	 uint32_t recv_id;
	 uint8_t rx_data[8] = {0};
	recv_len = Can_Recv_Msg(&recv_id, rx_data);
	 if(recv_len >0)
	 {
	    if(recv_id == 0x0B4)
			{
			 if(rx_data[0] == 0xAF)
			 {
			   if(rx_data[1] == 0x1)
				 {
//				   ui_water_temp = rx_data[2];
//				   ui_water_temp_freshflash = 1;
				   
					   ui_rpm.ui_value =rx_data[2];
					   ui_rpm.ui_freshflag = 1;
				 }
				 else if(rx_data[1] == 0x2)
				 {
				    ui_water_temp.ui_value =rx_data[2];
					 ui_water_temp.ui_freshflag = 1;
				 }
			   
				  else if(rx_data[1] == 0x3)
				 {
				    ui_safty_belt.ui_value =rx_data[2];
					 ui_safty_belt.ui_freshflag = 1;
				 }
				 
				   else if(rx_data[1] == 0x4)
				 {
				    ui_light.ui_value =rx_data[2];
					  ui_light.ui_freshflag = 1;
				 }
				 
				   else if(rx_data[1] == 0x5)
				 {
				    ui_turn_light.ui_value =rx_data[2];
					  ui_turn_light.ui_freshflag = 1;
				 }
				 
				    else if(rx_data[1] == 0x6)
				 {
				    ui_turn_light.ui_value =rx_data[2];
					  ui_turn_light.ui_freshflag = 1;
				 }
			 }
			}
	 }

}