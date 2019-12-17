// ------------------------------------
// µGUI example project
// http://www.embeddedlightning.com/
// ------------------------------------

#include <stdio.h>

#include "system.h"
#include "screen.h"
#include "image.h"
#include "ugui.h"

/* GUI structure */
UG_GUI gui;

/* Touch structure */
typedef struct {
	uint16_t TouchDetected;
	uint16_t X;
	uint16_t Y;
	uint16_t Z;
} TP_STATE;

static TP_STATE  tp_state;
static TP_STATE* TP_State;

void IOE_TP_SetState(uint16_t detected,uint16_t x,uint16_t y) {
	tp_state.TouchDetected = detected;
	tp_state.X = x;
	tp_state.Y = y;
}

TP_STATE* IOE_TP_GetState(void) {
	return &tp_state;
}

/* Some defines */
#define MAX_OBJECTS        10
#define TOGGLE_GREEN_LED   puts("trogle green led");
#define TOGGLE_RED_LED     puts("trogle red led");

/* Window 1 */
UG_WINDOW window_1;
UG_OBJECT obj_buff_wnd_1[MAX_OBJECTS];
UG_BUTTON button1_1;
UG_BUTTON button1_2;
UG_BUTTON button1_3;
UG_BUTTON button1_4;
UG_BUTTON button1_5;
UG_BUTTON button1_6;

/* Window 2 */
UG_WINDOW window_2;
UG_OBJECT obj_buff_wnd_2[MAX_OBJECTS];
UG_BUTTON button2_1;
UG_TEXTBOX textbox2_1;
UG_TEXTBOX textbox2_2;
UG_IMAGE image2_1;

/* Window 3 */
UG_WINDOW window_3;
UG_OBJECT obj_buff_wnd_3[MAX_OBJECTS];
UG_BUTTON button3_1;
UG_TEXTBOX textbox3_1;

/* FSM */
#define STATE_MAIN_MENU                0
#define STATE_BENCHMARK_RUN            1
#define STATE_BENCHMARK_RESULT         2
volatile UG_U32 state;
volatile UG_U32 next_state;

/* Benchmark */
volatile UG_U32 timer;
volatile UG_U32 hw_acc = 1;
char result_str[30];
UG_S16 xs,ys;
UG_S16 xe,ye;
UG_COLOR c;

void (*timer1_handler)(void);

volatile BOOL quit = FALSE;

#if 0
SDL_Event event;
void handle_sdl_events(void);

void gui_update(void) {
	UG_Update();
	SDL_Flip( screen );  
}

/* Systick interrupt */
void signal_handler(int val)
{
	while( SDL_PollEvent(&event) != 0 ) {
		if( event.type == SDL_QUIT ) {
			quit = TRUE;
		}
		handle_sdl_events();
	}
     if (SIGUSR2 == val)
    {
		  if ( timer ) timer--;
#if 1
		   if ( state == STATE_MAIN_MENU )
		   {
			  TP_State = IOE_TP_GetState();
			  if( TP_State->TouchDetected )
			  {
				 if ( (TP_State->X > 0) && (TP_State->X < 239 ) )
				 {
					if ( (TP_State->Y > 0) && (TP_State->Y < 319 ) )
					{
					   UG_TouchUpdate(TP_State->X,TP_State->Y,TOUCH_STATE_PRESSED);
					}
				 }
			  }
			  else
			  {
				 UG_TouchUpdate(-1,-1,TOUCH_STATE_RELEASED);
			  }
		   }
		   //UG_Update();
#endif
		if( timer1_handler ) {
			timer1_handler();
		}
		else {
			printf("timer1_handler was NULL\n");
		}
    }
}
#endif

void led_init(void)
{
	puts("led init");
}

void systick_init( void )
{
   puts("systick init");
}

/* Callback function for the main menu */
void window_1_callback( UG_MESSAGE* msg )
{
   if ( msg->type == MSG_TYPE_OBJECT )
   {
      if ( msg->id == OBJ_TYPE_BUTTON )
      {
         switch( msg->sub_id )
         {
            case BTN_ID_0: /* Toggle green LED */
            {
               TOGGLE_GREEN_LED;
               break;
            }
            case BTN_ID_1: /* Toggle red LED */
            {
               TOGGLE_RED_LED;
               break;
            }
            case BTN_ID_2: /* Show µGUI info */
            {
               UG_WindowShow( &window_2 );
               break;
            }
            case BTN_ID_3: /* Toggle hardware acceleration */
            {
               if ( !hw_acc )
               {
                  UG_ButtonSetForeColor( &window_1, BTN_ID_3, C_RED );
                  UG_ButtonSetText( &window_1, BTN_ID_3, (char*)"HW_ACC\nOFF" );
                  UG_DriverEnable( DRIVER_DRAW_LINE );
                  UG_DriverEnable( DRIVER_FILL_FRAME );
               }
               else
               {
                  UG_ButtonSetForeColor( &window_1, BTN_ID_3, C_BLUE );
                  UG_ButtonSetText( &window_1, BTN_ID_3, (char*)"HW_ACC\nON" );
                  UG_DriverDisable( DRIVER_DRAW_LINE );
                  UG_DriverDisable( DRIVER_FILL_FRAME );
               }
               hw_acc = !hw_acc;
               break;
            }
            case BTN_ID_4: /* Start benchmark */
            {
               next_state = STATE_BENCHMARK_RUN;
               break;
            }
            case BTN_ID_5: /* Resize window */
            {
               static UG_U32 tog;

               if ( !tog )
               {
                  UG_WindowResize( &window_1, 0, 40, 239, 319-40 );
               }
               else
               {
                  UG_WindowResize( &window_1, 0, 0, 239, 319 );
               }
               tog = ! tog;
               break;
            }
         }
      }
   }
}

/* Callback function for the info window */
void window_2_callback( UG_MESSAGE* msg )
{
   if ( msg->type == MSG_TYPE_OBJECT )
   {
      if ( msg->id == OBJ_TYPE_BUTTON )
      {
         switch( msg->sub_id )
         {
            case BTN_ID_0:
            {
               UG_WindowHide( &window_2 );
               break;
            }
         }
      }
   }
}

/* Callback function for the result window */
void window_3_callback( UG_MESSAGE* msg )
{
   if ( msg->type == MSG_TYPE_OBJECT )
   {
      if ( msg->id == OBJ_TYPE_BUTTON )
      {
         switch( msg->sub_id )
         {
            /* OK button */
            case BTN_ID_0:
            {
               UG_WindowShow( &window_1 );
               break;
            }
         }
      }
   }
}

/* better rand() function */
UG_U32 randx( void )
{
   static UG_U32 z1 = 12345, z2 = 12345, z3 = 12345, z4 = 12345;
   UG_U32 b;
   b  = ((z1 << 6) ^ z1) >> 13;
   z1 = ((z1 & 4294967294U) << 18) ^ b;
   b  = ((z2 << 2) ^ z2) >> 27;
   z2 = ((z2 & 4294967288U) << 2) ^ b;
   b  = ((z3 << 13) ^ z3) >> 21;
   z3 = ((z3 & 4294967280U) << 7) ^ b;
   b  = ((z4 << 3) ^ z4) >> 12;
   z4 = ((z4 & 4294967168U) << 13) ^ b;
   return (z1 ^ z2 ^ z3 ^ z4);
}

void pset( UG_S16 x, UG_S16 y, UG_COLOR c ) {
	x %= SCREEN_WIDTH;
	y %= SCREEN_HEIGHT;
	*(uint32_t*)(screen_start + y*SCREEN_WIDTH + x) = c;
}

int main(void)
{
   screen_init();
   led_init();

   /* Init µGUI */
   UG_Init(&gui,(void(*)(UG_S16,UG_S16,UG_COLOR))pset,240,320);

   /* Init Touch */
   //IOE_Config();

   /* Register hardware acceleration */
   #if 0
   UG_DriverRegister( DRIVER_DRAW_LINE, (void*)_HW_DrawLine );
   UG_DriverRegister( DRIVER_FILL_FRAME, (void*)_HW_FillFrame );
   UG_DriverEnable( DRIVER_DRAW_LINE );
   UG_DriverEnable( DRIVER_FILL_FRAME );
   #endif

   /* Init SysTick (100Hz) */
   systick_init();

   /* Clear Screen */
   //ltdc_draw_layer(LAYER_1);
   //ltdc_show_layer(LAYER_1);
   UG_FillScreen( C_BLUE );

   /* Create Window 1 */
   UG_WindowCreate( &window_1, obj_buff_wnd_1, MAX_OBJECTS, window_1_callback );
   UG_WindowSetTitleText( &window_1, (char*)"µGUI @ Linux" );
   UG_WindowSetTitleTextFont( &window_1, &FONT_12X20 );

   /* Create some Buttons */
   UG_ButtonCreate( &window_1, &button1_1, BTN_ID_0, 10, 10, 110, 60 );
   UG_ButtonCreate( &window_1, &button1_2, BTN_ID_1, 10, 80, 110, 130 );
   UG_ButtonCreate( &window_1, &button1_3, BTN_ID_2, 10, 150, 110,200 );
   UG_ButtonCreate( &window_1, &button1_4, BTN_ID_3, 120, 10, UG_WindowGetInnerWidth( &window_1 ) - 10 , 60 );
   UG_ButtonCreate( &window_1, &button1_5, BTN_ID_4, 120, 80, UG_WindowGetInnerWidth( &window_1 ) - 10, 130 );
   UG_ButtonCreate( &window_1, &button1_6, BTN_ID_5, 120, 150, UG_WindowGetInnerWidth( &window_1 ) - 10, 200 );

   /* Configure Button 1 */
   UG_ButtonSetFont( &window_1, BTN_ID_0, &FONT_12X20 );
   UG_ButtonSetBackColor( &window_1, BTN_ID_0, C_LIME );
   UG_ButtonSetText( &window_1, BTN_ID_0, (char*)"Green\nLED" );
   /* Configure Button 2 */
   UG_ButtonSetFont( &window_1, BTN_ID_1, &FONT_12X20 );
   UG_ButtonSetBackColor( &window_1, BTN_ID_1, C_RED );
   UG_ButtonSetText( &window_1, BTN_ID_1, (char*)"Red\nLED" );
   /* Configure Button 3 */
   UG_ButtonSetFont( &window_1, BTN_ID_2, &FONT_12X20 );
   UG_ButtonSetText( &window_1, BTN_ID_2, (char*)"About\nµGUI" );
   UG_WindowShow( &window_1 );
   /* Configure Button 4 */
   UG_ButtonSetFont( &window_1, BTN_ID_3, &FONT_12X20 );
   UG_ButtonSetForeColor( &window_1, BTN_ID_3, C_RED );
   UG_ButtonSetText( &window_1, BTN_ID_3, (char*)"HW_ACC\nOFF" );
   /* Configure Button 5 */
   UG_ButtonSetFont( &window_1, BTN_ID_4, &FONT_8X14 );
   UG_ButtonSetText( &window_1, BTN_ID_4, (char*)"Start\nBenchmark" );
   /* Configure Button 6 */
   UG_ButtonSetFont( &window_1, BTN_ID_5, &FONT_10X16 );
   UG_ButtonSetText( &window_1, BTN_ID_5, (char*)"Resize\nWindow" );

   /* -------------------------------------------------------------------------------- */
   /* Create Window 2 (µGUI Info)                                                      */
   /* -------------------------------------------------------------------------------- */
   UG_WindowCreate( &window_2, obj_buff_wnd_2, MAX_OBJECTS, window_2_callback );
   UG_WindowSetTitleText( &window_2, (char*)"Info" );
   UG_WindowSetTitleTextFont( &window_2, &FONT_12X20 );
   UG_WindowResize( &window_2, 20, 40, 219, 279 );

   /* Create Button 1 */
   UG_ButtonCreate( &window_2, &button2_1, BTN_ID_0, 40, 150, UG_WindowGetInnerWidth( &window_2 )-40, 200 );
   UG_ButtonSetFont( &window_2, BTN_ID_0, &FONT_22X36 );
   UG_ButtonSetText( &window_2, BTN_ID_0, (char*)"OK!" );

   /* Create Textbox 1 */
   UG_TextboxCreate( &window_2, &textbox2_1, TXB_ID_0, 10, 10, UG_WindowGetInnerWidth( &window_2 )-10, 40 );
   UG_TextboxSetFont( &window_2, TXB_ID_0, &FONT_16X26 );
   UG_TextboxSetText( &window_2, TXB_ID_0, (char*)"µGUI v0.3" );
   UG_TextboxSetAlignment( &window_2, TXB_ID_0, ALIGN_TOP_CENTER );

   /* Create Textbox 2 */
   UG_TextboxCreate( &window_2, &textbox2_2, TXB_ID_1, 10, 125, UG_WindowGetInnerWidth( &window_2 )-10, 135 );
   UG_TextboxSetFont( &window_2, TXB_ID_1, &FONT_6X8 );
   UG_TextboxSetText( &window_2, TXB_ID_1, (char*)"www.embeddedlightning.com" );
   UG_TextboxSetAlignment( &window_2, TXB_ID_1, ALIGN_BOTTOM_CENTER );
   UG_TextboxSetForeColor( &window_2, TXB_ID_1, C_BLUE );
   UG_TextboxSetHSpace( &window_2, TXB_ID_1, 1 );

   /* Create Image 1 */
   UG_ImageCreate( &window_2, &image2_1, IMG_ID_0, (UG_WindowGetInnerWidth( &window_2 )>>1) - (logo.width>>1), 40, 0, 0 );
   UG_ImageSetBMP( &window_2, IMG_ID_0, &logo );

   /* -------------------------------------------------------------------------------- */
   /* Create Window 3 (Benchmark Result)                                               */
   /* -------------------------------------------------------------------------------- */
   UG_WindowCreate( &window_3, obj_buff_wnd_3, MAX_OBJECTS, window_3_callback );
   UG_WindowSetTitleText( &window_3, (char*)"Benchmark Result" );
   UG_WindowSetTitleTextFont( &window_3, &FONT_10X16 );
   UG_WindowResize( &window_3, 20, 90, 219, 230 );

   /* Create Textbox 1 */
   UG_TextboxCreate( &window_3, &textbox3_1, TXB_ID_0, 5, 10, UG_WindowGetInnerWidth( &window_3 )-5, 60 );
   UG_TextboxSetFont( &window_3, TXB_ID_0, &FONT_12X20 );
   UG_TextboxSetText( &window_3, TXB_ID_0, (char*)"Result:\n99999 frm/sec" );
   UG_TextboxSetAlignment( &window_3, TXB_ID_0, ALIGN_TOP_CENTER );

   /* Create Button 1 */
   UG_ButtonCreate( &window_3, &button3_1, BTN_ID_0, 40, 65, UG_WindowGetInnerWidth( &window_3 )-40, 100 );
   UG_ButtonSetFont( &window_3, BTN_ID_0, &FONT_12X16 );
   UG_ButtonSetText( &window_3, BTN_ID_0, (char*)"OK!" );

   /* -------------------------------------------------------------------------------- */
   /* Start demo application                                                           */
   /* -------------------------------------------------------------------------------- */
   /* Show Window 1 */
   UG_WindowShow( &window_1 );
   //timer1_handler = gui_update;
   UG_WaitForUpdate();

   /* Initialize FSM */
   next_state = STATE_MAIN_MENU;
   state = !STATE_MAIN_MENU;

   while(!quit)
   {
      static int frm_cnt;

      /* Do we change the state? */
      if ( next_state != state )
      {
         /* Initialize the next state */
         switch ( next_state )
         {
            case STATE_MAIN_MENU:
            {
               /* Nothing to do */
               break;
            }
            case STATE_BENCHMARK_RUN:
            {
               /* Clear layer 2 */
               //ltdc_draw_layer( LAYER_2 );
               UG_FillScreen( C_BLACK );

               /* Fade to layer 2 */
               //ltdc_fade_to_layer( LAYER_2 );

               /* Reset the frame counter */
               frm_cnt = 0;

               /* Run benchmark for 5 seconds */
               //timer = 500;
			   timer = 50;
               break;
            }
            case STATE_BENCHMARK_RESULT:
            {
               /* Nothing to do */
               break;
            }
         }
         state = next_state;
      }

      /* FSM */
      switch ( state )
      {
         /* Run the benchmark */
         case STATE_BENCHMARK_RUN:
         {
            xs = randx() % 240;
            xe = randx() % 240;
            ys = randx() % 320;
            ye = randx() % 320;
            c = randx() % 0xFFFFFF;
            UG_FillFrame( xs, ys, xe, ye, c );
            frm_cnt++;

            if ( !timer ) next_state = STATE_BENCHMARK_RESULT;
            break;
         }
         /* Show benchmark result */
         case STATE_BENCHMARK_RESULT:
         {
			#if 0
            sprintf( result_str, "Result:\n%u frm/sec", frm_cnt/5 );
			#endif
            UG_TextboxSetText( &window_3, TXB_ID_0, result_str );

            /* Fade to layer 1 */
            //ltdc_draw_layer( LAYER_1 );
            //ltdc_fade_to_layer( LAYER_1 );

            /* Show benchmark result */
            UG_WindowShow( &window_3 );

            next_state = STATE_MAIN_MENU;
            break;
         }
         case STATE_MAIN_MENU:
         {
            /* Let µGUI do the job! */
            break;
         }
      }
   }

    //Free memory  
    //SDL_FreeSurface( screen );  
    //Quit SDL  
	//screen_exit();
	return 0;
}

static int key_pressed = 0;

void handle_sdl_events(void)
{
    //The mouse offsets
    int x = 0, y = 0;
    //If the mouse moved
	#if 0
    if( event.type == SDL_MOUSEMOTION )
    {
		if( key_pressed ) {
			x = event.motion.x;
			y = event.motion.y;
			//UG_TouchUpdate(x,y,TOUCH_STATE_PRESSED);
			IOE_TP_SetState(1,x,y);
		}

    }
    //If a mouse button was pressed
    if( event.type == SDL_MOUSEBUTTONDOWN )
    {
        //If the left mouse button was pressed
        if( event.button.button == SDL_BUTTON_LEFT )
        {
			key_pressed = -1;
            x = event.button.x;
            y = event.button.y;
			//UG_TouchUpdate(x,y,TOUCH_STATE_PRESSED);
			IOE_TP_SetState(1,x,y);
        }
    }
    //If a mouse button was released
    if( event.type == SDL_MOUSEBUTTONUP )
    {
        //If the left mouse button was released
        if( event.button.button == SDL_BUTTON_LEFT )
        {
			key_pressed = 0;
            x = event.button.x;
            y = event.button.y;
			//UG_TouchUpdate(-1,-1,TOUCH_STATE_RELEASED);
			IOE_TP_SetState(0,-1,-1);
        }
    }
	#endif
}
