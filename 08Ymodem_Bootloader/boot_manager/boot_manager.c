#include "boot_manager.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"

#define AC5_MODE			1

	
typedef void (*APP_FUNC)(void); 				//函数指针类型定义

#define CLOSE_ALL_INT()  __set_PRIMASK(1)	//关闭所有中断


uint8_t boot_check_stack2jump_app(uint32_t addr) // APP_ADDR
{
#if (AC5_MODE == 1)			//TODO:https://www.armbbs.cn/forum.php?mod=viewthread&tid=110358
		APP_FUNC jump2app; // 定义一个函数指针	
#else
		static APP_FUNC jump2app; // 定义一个函数指针
#endif	

    /* 栈顶地址是否合法 */
    if (((*(uint32_t *)addr) & 0x2FFE0000) == 0x20000000)
    {

        /* 获取复位地址 */
        jump2app = (APP_FUNC) * (volatile uint32_t *)(addr + 4);
        /* 设置栈指针 */
        __set_MSP(*(volatile uint32_t *)addr);

        /* 跳转之前关闭相应的中断 */
        CLOSE_ALL_INT();
        /* 跳转至APP */
        jump2app();

        printf("Bootloader end load app\r\n");
    }
    else
    {
        printf("APP Not Found!\r\n");
    }

    return 0;
}

#if 0

#endif
