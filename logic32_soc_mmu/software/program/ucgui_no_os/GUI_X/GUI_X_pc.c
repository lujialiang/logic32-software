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
---Author-Explanation
* 
* 1.00.00 020519 JJL    First release of uC/GUI to uC/OS-II interface
* 
*
* Known problems or limitations with current version
*
*    None.
*
*
* Open issues
*
*    None
*********************************************************************************************************
*/

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

#include <stdio.h>
#include <stdbool.h>

#include "GUI_Private.h"

/*
*********************************************************************************************************
*                                         GLOBAL VARIABLES
*********************************************************************************************************
*/
static SDL_sem *videoLock = NULL;
static SDL_sem *KeySem = NULL;
static SDL_sem *EventMbox;
static int KeyPressed;
static char KeyIsInited;

void GUI_X_Log(const char *s) 
{
	//printf("log:%s\n",s); 
}
void GUI_X_Warn(const char *s) 
{ 
	//printf("warn:%s\n",s); 
}
void GUI_X_ErrorOut(const char *s) 
{ 
	//printf("error:%s\n",s); 
}

/*
*********************************************************************************************************
*                                        TIMING FUNCTIONS
*
* Notes: Some timing dependent routines of uC/GUI require a GetTime and delay funtion. 
*        Default time unit (tick), normally is 1 ms.
*********************************************************************************************************
*/

int GUI_X_GetTime(void) 
{
    return SDL_GetTicks();
}


void GUI_X_Delay(int period) 
{
    SDL_Delay( period);
}


/*
*********************************************************************************************************
*                                          GUI_X_ExecIdle()
*********************************************************************************************************
*/
void GUI_X_ExecIdle(void) 
{
    SDL_Delay(10);
}


/*
*********************************************************************************************************
*                                    MULTITASKING INTERFACE FUNCTIONS
*
* Note(1): 1) The following routines are required only if uC/GUI is used in a true multi task environment, 
*             which means you have more than one thread using the uC/GUI API.  In this case the #define 
*             GUI_OS 1   needs to be in GUIConf.h
*********************************************************************************************************
*/

void GUI_X_InitOS(void)
{
    videoLock = SDL_CreateSemaphore(1);
}


void GUI_X_Lock(void)
{ 
    SDL_SemWait( videoLock );
}


void GUI_X_Unlock(void)
{ 
    SDL_SemPost( videoLock );
}


U32 GUI_X_GetTaskId(void) 
{
    return SDL_ThreadID();
}

/*
*********************************************************************************************************
*                                        GUI_X_WaitEvent()
*                                        GUI_X_SignalEvent()
*********************************************************************************************************
*/

#if 0
void GUI_X_WaitEvent(void) 
{
}


void GUI_X_SignalEvent(void) 
{
}
#endif

/*
*********************************************************************************************************
*                                      KEYBOARD INTERFACE FUNCTIONS
*
* Purpose: The keyboard routines are required only by some widgets.
*          If widgets are not used, they may be eliminated.
*
* Note(s): If uC/OS-II is used, characters typed into the log window will be placed	in the keyboard buffer. 
*          This is a neat feature which allows you to operate your target system without having to use or 
*          even to have a keyboard connected to it. (useful for demos !)
*********************************************************************************************************
*/

static void CheckInit(void) 
{
    if (KeyIsInited == false) {
        KeyIsInited = true;
        GUI_X_Init();
    }
}


void GUI_X_Init(void) 
{
    KeySem = SDL_CreateSemaphore(1);
}


int GUI_X_GetKey(void) 
{
    int r;
    r = KeyPressed;
    CheckInit();
    KeyPressed = 0;
    return (r);
}


int GUI_X_WaitKey(void) 
{
    int r;
    CheckInit();
    if (KeyPressed == 0) {
        SDL_SemWait(KeySem);
    }
    r = KeyPressed;
    KeyPressed = 0;
    return (r);
}


void GUI_X_StoreKey(int k) 
{
    KeyPressed = k;
    SDL_SemPost(KeySem);
}
