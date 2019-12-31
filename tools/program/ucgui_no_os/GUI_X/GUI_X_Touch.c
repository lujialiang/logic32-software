/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUI_TOUCH_X.C
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include "GUI_X.h"

#include <stdio.h>
#include "GUITouchConf.h"
#include "LCDConf.h"

extern int mouse_x, mouse_y;

void GUI_TOUCH_X_ActivateX(void) {
}

void GUI_TOUCH_X_ActivateY(void) {
}

int  GUI_TOUCH_X_MeasureX(void) {
#if 0
	int x = mouse_x;
	int y = mouse_y;
#else
	int x = *(int *)0x87000000;
	int y = *(int *)0x87000004;
#endif
	int X0 = GUI_TOUCH_AD_LEFT;
	int X1 = GUI_TOUCH_AD_RIGHT;
	int Y0 = GUI_TOUCH_AD_TOP;
	int Y1 = GUI_TOUCH_AD_BOTTOM;
	int X = LCD_XSIZE;
	int Y = LCD_YSIZE;
	int rx = ((X1 - X0) * x )/X + X0;
	int ry = ((Y1 - Y0) * y )/Y + Y0;
	return rx;
}

int  GUI_TOUCH_X_MeasureY(void) {
#if 0
	int x = mouse_x;
	int y = mouse_y;
#else
	int x = *(int *)0x87000000;
	int y = *(int *)0x87000004;
#endif
	int X0 = GUI_TOUCH_AD_LEFT;
	int X1 = GUI_TOUCH_AD_RIGHT;
	int Y0 = GUI_TOUCH_AD_TOP;
	int Y1 = GUI_TOUCH_AD_BOTTOM;
	int X = LCD_XSIZE;
	int Y = LCD_YSIZE;
	int rx = ((X1 - X0) * x )/X + X0;
	int ry = ((Y1 - Y0) * y )/Y + Y0;
	return ry;
}


