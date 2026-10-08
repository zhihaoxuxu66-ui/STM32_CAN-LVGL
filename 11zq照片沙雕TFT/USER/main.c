//////////////////////////////////////////////////////////////////////////////////
//  文 件 名   : main.c
//  功能描述   : TFT屏只显示文字 "你好"（去掉图片显示）
//  屏幕       : 中景园3.5寸 ST7796 320x480 竖屏
//  文字       : lcdfont.h 中 tfont16 字库 ("你" "好" 16号字)
//******************************************************************************/
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "lcd_init.h"
#include "lcd.h"

#define NAME_X  144   // (320-2*16)/2  "你好" 两字 16号字水平居中
#define NAME_Y  200

int main(void)
{
	delay_init();
	LED_Init();//LED初始化
	LCD_Init();//LCD初始化
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);//清屏为白色
	LED=0;

	while(1)
	{
		LCD_ShowChinese(NAME_X,NAME_Y,"你好",RED,WHITE,16,0);
		delay_ms(1000);//仅文字显示，刷新间隔由你定，可去掉循环只显示一次
	}
}
