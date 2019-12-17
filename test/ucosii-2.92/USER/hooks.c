#include "includes.h"

void App_TaskCreateHook(OS_TCB *ptcb)
{         
	//printf("App_TaskCreateHook(): OSTCBPrio = %d\n",ptcb->OSTCBPrio);
}

void App_TaskDelHook(OS_TCB *ptcb)
{
	//puts("App_TaskDelHook()");
}

void App_TaskIdleHook(void)
{
	//puts("App_TaskIdleHook()");
}

void App_TaskReturnHook(OS_TCB *ptcb)
{
	//puts("App_TaskReturnHook()");
}

void App_TaskStatHook(void)
{
	//puts("App_TaskStatHook()");
}

void App_TaskSwHook(void)
{
	//printf("App_TaskSwHook() ,cur = %d, hi = %d\n", OSPrioCur, OSPrioHighRdy );
}

void App_TCBInitHook(OS_TCB *ptcb)
{
	//printf("App_TCBInitHook(): TaskName = %s\n", ptcb->OSTCBTaskName);
}

void App_TimeTickHook(void)
{
    if (OSRunning == OS_FALSE)
	{
		puts("App_TimeTickHook() : os not running\n");
	}
}
