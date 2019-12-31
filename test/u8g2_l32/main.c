
#include <stdio.h>
#include <string.h>

#include "u8g2.h"
#include "screen.h"

int kbhit(void);

u8g2_t u8g2;

void wait_key(void)
{
	uart_getc();
}

uint8_t u8x8_GetMenuEvent(u8x8_t *u8x8)
{
	if ( kbhit() ) {
		switch( uart_getc() ) {
			case '3':
				return  U8X8_MSG_GPIO_MENU_NEXT;
			case '2':
				return  U8X8_MSG_GPIO_MENU_SELECT;
			case '1':
				return  U8X8_MSG_GPIO_MENU_PREV;
			case '0':
				return  U8X8_MSG_GPIO_MENU_HOME;
			default:
				return 0;
		}
    }
    return 0;
}

int main(void)
{
#if (LCD_WIDTH==640 && LCD_HEIGHT==480)
	u8g2_SetupBuffer_SDL_640x480(&u8g2, &u8g2_cb_r0);
#elif (LCD_WIDTH==240 && LCD_HEIGHT==160)
	u8g2_SetupBuffer_SDL_240x160(&u8g2, &u8g2_cb_r0);
#elif (LCD_WIDTH==320 && LCD_HEIGHT==240)
	u8g2_SetupBuffer_SDL_320x240(&u8g2, &u8g2_cb_r0);
#elif (LCD_WIDTH==128 && LCD_HEIGHT==64)
	u8g2_SetupBuffer_SDL_128x64(&u8g2, &u8g2_cb_r0);
#else
	#error "bad LCD_WIDTH value"
#endif

	u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
	u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  

	u8g2_ClearBuffer(&u8g2);  
	u8g2_SetFontMode(&u8g2, 1);  
	u8g2_SetFontDirection(&u8g2, 0);  
    u8g2_SetFont(&u8g2, u8g2_font_wqy16_t_chinese2);
	u8g2_DrawUTF8(&u8g2,  0, 24, "中国你好");
	u8g2_SetFont(&u8g2, u8g2_font_helvB18_te);  
	u8g2_DrawStr(&u8g2,  0, 50, "I am LJL.");  
#if (LCD_HEIGHT<100)
	u8g2_SetFont(&u8g2, u8g2_font_u8glib_4_tf);  
	u8g2_DrawStr(&u8g2,  0, 60, __DATE__ " " __TIME__);  
#else
	u8g2_DrawStr(&u8g2,  0, 80, __DATE__ " " __TIME__);  
#endif
	u8g2_SendBuffer(&u8g2);  

	wait_key();

    u8g2_SetFont(&u8g2, u8g2_font_wqy16_t_gb2312);
	u8g2_SetFontDirection(&u8g2, 0);
	u8g2_SetFontRefHeightAll(&u8g2);
    u8g2_UserInterfaceMessage(&u8g2, "苹果", "香蕉", "雪梨", " Yes \n No ");
	u8g2_SendBuffer(&u8g2);  

	wait_key();

    u8g2_UserInterfaceSelectionList(&u8g2, "七大地理地区", 4, 
        "东北\n"
        "华东\n"
        "华北\n"
        "华中\n"
        "华南\n"
        "西南\n"
        "西北"
    );

	u8g2_ClearBuffer(&u8g2);  
	u8g2_DrawStr(&u8g2,  0, 50, "Good Bye!");  
	u8g2_SendBuffer(&u8g2);  

	wait_key();

	printf("done\n");

	return 0;
}
