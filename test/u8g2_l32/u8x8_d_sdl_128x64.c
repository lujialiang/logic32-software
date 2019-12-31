/*
  u8x8_d_sdl_128x64.c
*/

#include "u8g2.h"
#include "screen.h"

#define W(x,w) (((x)*(w))/100)

int kbhit(void);

int u8g_sdl_height, u8g_sdl_width;

static void u8g_sdl_set_8pixel(int x, int y, uint8_t pixel)
{
	int cnt = 8;
	int bg = 0;
	if ( (x/8 + y/8)  & 1 ) {
		bg = 4;
	}
	while( cnt > 0 ) {
		if ( (pixel & 1) == 0 ) {
			put_pixel_ex(x,y,0x0000);
		}
		else {
			put_pixel_ex(x,y,0xffff);
		}
		pixel >>= 1;
		y++;
		cnt--;
	}
}

static void u8g_sdl_set_multiple_8pixel(int x, int y, int cnt, uint8_t *pixel)
{
	uint8_t b;
	while( cnt > 0 ) {
		b = *pixel;
		u8g_sdl_set_8pixel(x, y, b);
		x++;
		pixel++;
		cnt--;
	}
}

static void u8g_sdl_init(int width, int height)
{
	u8g_sdl_height = height;
	u8g_sdl_width = width;
	screen_init();
	return;
}

static const u8x8_display_info_t u8x8_sdl_128x64_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 0,
  /* pre_chip_disable_wait_ns = */ 0,
  /* reset_pulse_width_ms = */ 0, 
  /* post_reset_wait_ms = */ 0, 
  /* sda_setup_time_ns = */ 0,		
  /* sck_pulse_width_ns = */ 0,
  /* sck_clock_hz = */ 4000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 1,		
  /* i2c_bus_clock_100kHz = */ 0,
  /* data_setup_time_ns = */ 0,
  /* write_pulse_width_ns = */ 0,
  /* tile_width = */ 16,
  /* tile_hight = */ 8,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 128,
  /* pixel_height = */ 64
};

static const u8x8_display_info_t u8x8_sdl_240x160_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 0,
  /* pre_chip_disable_wait_ns = */ 0,
  /* reset_pulse_width_ms = */ 0, 
  /* post_reset_wait_ms = */ 0, 
  /* sda_setup_time_ns = */ 0,		
  /* sck_pulse_width_ns = */ 0,
  /* sck_clock_hz = */ 4000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 1,		
  /* i2c_bus_clock_100kHz = */ 0,
  /* data_setup_time_ns = */ 0,
  /* write_pulse_width_ns = */ 0,
  /* tile_width = */ 30,		/* width of 30*8=240 pixel */
  /* tile_hight = */ 20,		/* height: 160 pixel */
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 240,
  /* pixel_height = */ 160
};

static const u8x8_display_info_t u8x8_sdl_320x240_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 0,
  /* pre_chip_disable_wait_ns = */ 0,
  /* reset_pulse_width_ms = */ 0, 
  /* post_reset_wait_ms = */ 0, 
  /* sda_setup_time_ns = */ 0,		
  /* sck_pulse_width_ns = */ 0,
  /* sck_clock_hz = */ 4000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 1,		
  /* i2c_bus_clock_100kHz = */ 0,
  /* data_setup_time_ns = */ 0,
  /* write_pulse_width_ns = */ 0,
  /* tile_width = */ 40,		/* width of 40*8=320 pixel */
  /* tile_hight = */ 30,		/* height: 240 pixel */
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 320,
  /* pixel_height = */ 240
};
static const u8x8_display_info_t u8x8_sdl_640x480_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 0,
  /* pre_chip_disable_wait_ns = */ 0,
  /* reset_pulse_width_ms = */ 0, 
  /* post_reset_wait_ms = */ 0, 
  /* sda_setup_time_ns = */ 0,		
  /* sck_pulse_width_ns = */ 0,
  /* sck_clock_hz = */ 4000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 1,		
  /* i2c_bus_clock_100kHz = */ 0,
  /* data_setup_time_ns = */ 0,
  /* write_pulse_width_ns = */ 0,
  /* tile_width = */ 80,		/* width of 80*8=640 pixel */
  /* tile_hight = */ 60,		/* height: 480 pixel */
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 640,
  /* pixel_height = */ 480
};

static uint8_t u8x8_d_sdl_gpio(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	static int debounce_cnt = 0;
	static int curr_msg = 0;
	static int db_cnt = 10;
	int event;
  
	if ( curr_msg > 0 )
	{
	  if ( msg == curr_msg )
	  {
		  u8x8_SetGPIOResult(u8x8, 0);
		  if ( debounce_cnt == 0 )
		    curr_msg = 0;
		  else
		    debounce_cnt--;
		  return 1;
	  }
	  
	}
	else
	{
	  event = 0;
	  if ( kbhit() ) {
		event = uart_getc();
	  }
	  
	  switch(event)
	  {
		  case 273:
			  curr_msg = U8X8_MSG_GPIO_MENU_UP;
			  debounce_cnt = db_cnt;
			  break;
		  case 274:
			  curr_msg = U8X8_MSG_GPIO_MENU_DOWN;
			  debounce_cnt = db_cnt;
			  break;
		  case 275:
			  curr_msg = U8X8_MSG_GPIO_MENU_NEXT;
			  debounce_cnt = db_cnt;
			  break;
		  case 276:
			  curr_msg = U8X8_MSG_GPIO_MENU_PREV;
			  debounce_cnt = db_cnt;
			  break;
		  case 's':
			  curr_msg = U8X8_MSG_GPIO_MENU_SELECT;
			  debounce_cnt = db_cnt;
			  break;
		  case 'q':
			  curr_msg = U8X8_MSG_GPIO_MENU_HOME;
			  debounce_cnt = db_cnt;
			  break;
	  }
	}
	u8x8_SetGPIOResult(u8x8, 1);
	return 1;
}

uint8_t u8x8_d_sdl_128x64(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x, y, c;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8g2, &u8x8_sdl_128x64_info);
      u8g_sdl_init(128, 64);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8g2);
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      break;
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      break;
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      x *= 8;
      x += u8g2->x_offset;
    
      y = ((u8x8_tile_t *)arg_ptr)->y_pos;
      y *= 8;
    
      do
      {
        c = ((u8x8_tile_t *)arg_ptr)->cnt;
        ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
        u8g_sdl_set_multiple_8pixel(x, y, c*8, ptr);
        arg_int--;
      } while( arg_int > 0 );
      
      /* update all */
      //SDL_UpdateWindowSurface(u8g_sdl_window);
      
      break;
    default:
      return 0;
  }
  return 1;
}

uint8_t u8x8_d_sdl_240x160(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x, y, c;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8g2, &u8x8_sdl_240x160_info);
      u8g_sdl_init(240, 160);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8g2);
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      break;
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      break;
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      x *= 8;
      x += u8g2->x_offset;
    
      y = ((u8x8_tile_t *)arg_ptr)->y_pos;
      y *= 8;
    
      do
      {
        c = ((u8x8_tile_t *)arg_ptr)->cnt;
        ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
        u8g_sdl_set_multiple_8pixel(x, y, c*8, ptr);
        arg_int--;
      } while( arg_int > 0 );

      /* update all */
      //SDL_UpdateWindowSurface(u8g_sdl_window);
      
      break;
    default:
      return 0;
  }
  return 1;
}

uint8_t u8x8_d_sdl_320x240(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x, y, c;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8g2, &u8x8_sdl_320x240_info);
      u8g_sdl_init(320, 240);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8g2);
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      break;
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      break;
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      x *= 8;
      x += u8g2->x_offset;
    
      y = ((u8x8_tile_t *)arg_ptr)->y_pos;
      y *= 8;
    
      do
      {
        c = ((u8x8_tile_t *)arg_ptr)->cnt;
        ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
        u8g_sdl_set_multiple_8pixel(x, y, c*8, ptr);
        arg_int--;
      } while( arg_int > 0 );

      /* update all */
      //SDL_UpdateWindowSurface(u8g_sdl_window);
      
      break;
    default:
      return 0;
  }
  return 1;
}

uint8_t u8x8_d_sdl_640x480(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint16_t x, y, c;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8g2, &u8x8_sdl_640x480_info);
      u8g_sdl_init(640, 480);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8g2);
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      break;
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      break;
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      x *= 8;
      x += u8g2->x_offset;
    
      y = ((u8x8_tile_t *)arg_ptr)->y_pos;
      y *= 8;
    
      do
      {
        c = ((u8x8_tile_t *)arg_ptr)->cnt;
        ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
        u8g_sdl_set_multiple_8pixel(x, y, c*8, ptr);
        arg_int--;
      } while( arg_int > 0 );

      /* update all */
      //SDL_UpdateWindowSurface(u8g_sdl_window);
      
      break;
    default:
      return 0;
  }
  return 1;
}

void u8x8_Setup_SDL_128x64(u8x8_t *u8x8)
{
  /* setup defaults */
  u8x8_SetupDefaults(u8x8);
  
  /* setup specific callbacks */
  u8x8->display_cb = u8x8_d_sdl_128x64;
	
  u8x8->gpio_and_delay_cb = u8x8_d_sdl_gpio;

  /* setup display info */
  u8x8_SetupMemory(u8x8);  
}

void u8g2_SetupBuffer_SDL_128x64(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb)
{
  
  static uint8_t buf[128*8];
  
  u8x8_Setup_SDL_128x64(u8g2_GetU8x8(u8g2));
  u8g2_SetupBuffer(u8g2, buf, 8, u8g2_ll_hvline_vertical_top_lsb, u8g2_cb);
}

void u8x8_Setup_SDL_240x160(u8x8_t *u8x8)
{
  /* setup defaults */
  u8x8_SetupDefaults(u8x8);
  
  /* setup specific callbacks */
  u8x8->display_cb = u8x8_d_sdl_240x160;
	
  u8x8->gpio_and_delay_cb = u8x8_d_sdl_gpio;

  /* setup display info */
  u8x8_SetupMemory(u8x8);  
}

void u8g2_SetupBuffer_SDL_240x160(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb)
{
  
  static uint8_t buf[240*20];
  
  u8x8_Setup_SDL_240x160(u8g2_GetU8x8(u8g2));
  u8g2_SetupBuffer(u8g2, buf, 20, u8g2_ll_hvline_vertical_top_lsb, u8g2_cb);
}

void u8x8_Setup_SDL_320x240(u8x8_t *u8x8)
{
  /* setup defaults */
  u8x8_SetupDefaults(u8x8);
  
  /* setup specific callbacks */
  u8x8->display_cb = u8x8_d_sdl_320x240;
	
  u8x8->gpio_and_delay_cb = u8x8_d_sdl_gpio;

  /* setup display info */
  u8x8_SetupMemory(u8x8);  
}

void u8g2_SetupBuffer_SDL_320x240(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb)
{
  
  static uint8_t buf[320*30];
  
  u8x8_Setup_SDL_320x240(u8g2_GetU8x8(u8g2));
  u8g2_SetupBuffer(u8g2, buf, 30, u8g2_ll_hvline_vertical_top_lsb, u8g2_cb);
}

void u8x8_Setup_SDL_640x480(u8x8_t *u8x8)
{
  /* setup defaults */
  u8x8_SetupDefaults(u8x8);
  
  /* setup specific callbacks */
  u8x8->display_cb = u8x8_d_sdl_640x480;
	
  u8x8->gpio_and_delay_cb = u8x8_d_sdl_gpio;

  /* setup display info */
  u8x8_SetupMemory(u8x8);  
}

void u8g2_SetupBuffer_SDL_640x480(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb)
{
  
  static uint8_t buf[640*60];
  
  u8x8_Setup_SDL_640x480(u8g2_GetU8x8(u8g2));
  u8g2_SetupBuffer(u8g2, buf, 60, u8g2_ll_hvline_vertical_top_lsb, u8g2_cb);
}
