#ifndef _HOOK_H_INCLUDE_
#define _HOOK_H_INCLUDE_

#include "includes.h"

void App_TaskCreateHook(OS_TCB *ptcb);

void App_TaskDelHook(OS_TCB *ptcb);

void App_TaskIdleHook(void);

void App_TaskReturnHook(OS_TCB *ptcb);

void App_TaskStatHook(void);

void App_TaskSwHook(void);

void App_TCBInitHook(OS_TCB *ptcb);

void App_TimeTickHook(void);

#endif
