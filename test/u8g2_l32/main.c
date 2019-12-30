
#include <stdio.h>
#include <string.h>

#include "u8g2.h"
#include "screen.h"

u8g2_t u8g2;

int main(void)
{
	u8g2_SetupBuffer_SDL_128x64(&u8g2, &u8g2_cb_r0);
	u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
	u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  

	u8g2_ClearBuffer(&u8g2);  
	u8g2_SetFontMode(&u8g2, 1);  
	u8g2_SetFontDirection(&u8g2, 0);  
    u8g2_SetFont(&u8g2, u8g2_font_wqy16_t_chinese2);
	u8g2_DrawUTF8(&u8g2,  0, 24, "中国你好");
	u8g2_SetFont(&u8g2, u8g2_font_helvB18_te);  
	u8g2_DrawStr(&u8g2,  0, 50, "I am LJL.");  
	u8g2_SetFont(&u8g2, u8g2_font_u8glib_4_tf);  
	u8g2_DrawStr(&u8g2,  0, 60, __DATE__);  
	u8g2_SendBuffer(&u8g2);  


#if 1
	while(1) {
		/*
		if( u8g_sdl_get_key() == 'q' ) {
			break;
		}
		*/
	}
#endif

	return 0;
}
