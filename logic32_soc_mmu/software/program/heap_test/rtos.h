#ifndef _RTOS_H_INCLUDE_
#define _RTOS_H_INCLUDE_

#include "os_type.h"

#define THREAD_MAGIC 0x4321abcd
#define SEM_RESERVED_COUNT 2
#define INFO_COUNT 4

#define OS_WAIT_FOREVER -1

typedef void (*OS_FuncPtr_t)(void *arg);
typedef struct OS_Thread_s OS_Thread_t;
typedef struct OS_Semaphore_s OS_Semaphore_t;
typedef struct OS_Mutex_s OS_Mutex_t;
typedef struct OS_MQueue_s OS_MQueue_t;
typedef struct OS_Timer_s OS_Timer_t;

typedef void (*OS_TimerFuncPtr_t)(OS_Timer_t *timer, uint32 info);

typedef enum {
   THREAD_PEND    = 0,       //Thread in semaphore's linked list
   THREAD_READY   = 1,       //Thread in ThreadHead linked list
   THREAD_RUNNING = 2        //Thread == ThreadCurrent[cpu]
} OS_ThreadState_e;

struct OS_Thread_s {
   const char *name;         //Name of thread
   OS_ThreadState_e state;   //Pending, ready, or running
   int cpuIndex;             //Which CPU is running the thread
   int cpuLock;              //Lock the thread to a specific CPU
   jmp_buf env;              //Registers saved during context swap
   OS_FuncPtr_t funcPtr;     //First function called
   void *arg;                //Argument to first function called
   uint32 priority;          //Priority of thread (0=low, 255=high)
   uint32 ticksTimeout;      //Tick value when semaphore pend times out
   void *info[INFO_COUNT];   //User storage
   OS_Semaphore_t *semaphorePending;  //Semaphore thread is blocked on
   int returnCode;           //Return value from semaphore pend
   uint32 processId;         //Process ID if using MMU
   OS_Heap_t *heap;          //Heap used if no heap specified
   struct OS_Thread_s *next; //Linked list of threads by priority
   struct OS_Thread_s *prev;  
   struct OS_Thread_s *nextTimeout; //Linked list of threads by timeout
   struct OS_Thread_s *prevTimeout; 
   uint32 magic[1];          //Bottom of stack to detect stack overflow
};

struct OS_Semaphore_s {
   const char *name;
   struct OS_Thread_s *threadHead; //threads pending on semaphore
   int count;
};

struct OS_Mutex_s {
   OS_Semaphore_t *semaphore;
   OS_Thread_t *thread;
   uint32 priorityRestore;
   int count;
}; 

struct OS_MQueue_s {
   const char *name;
   OS_Semaphore_t *semaphore;
   int count, size, used, read, write;
};

struct OS_Timer_s {
   const char *name;
   struct OS_Timer_s *next, *prev;
   uint32 ticksTimeout;
   uint32 ticksRestart;
   int active;
   OS_TimerFuncPtr_t callback;
   OS_MQueue_t *mqueue;
   uint32 info;
}; 

#endif
