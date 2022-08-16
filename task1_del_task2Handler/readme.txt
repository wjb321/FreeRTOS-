将stm32f103ze 移植到C8T6中：


1，更改startup_hd.s to startup_md.s. 注意：驱动文件需要在最小系统的cmsis-> arm下面获取。
2，通过魔法棒更改芯片，c/c++下的HD -> MD， ultilities下更改flash size.
3, 由于freeRTOS 中RAM分配大小有问题，第一次通过将其变大，发现没有效果。然后变小后才体现功能。
   其问题分析如此：https://blog.csdn.net/qq_33161479/article/details/90064215


4, FreeRTOSconfig.h 中的文件配置：
（condition compile）INCLUDE_ 开头的宏文件用来表示使能或者除能FreeRTOS中相应的API函数， 作用就是配置FreeRTOS中可选的API函数。
INCLUDE_vTaskPrioritySet 就表示使能函数vTASKPrioritySet() 

5, Config 开头的宏文件和上者相同： configUSE_PREEMPTION等。 co-routine, is not develped anymore, but no plans to remove that or update that.

6, freeRTOS timer/interrupt can be used for ttcan timer configuration. 


7,
#define xPortPendSVHandler 	PendSV_Handler
#define vPortSVCHandler 	SVC_Handler
here are the renamed isrs in freeRTOS 

8, task characteristics:
   a, simple
   b, no usage limation
   c, support preemption 
   d, support priority
   e, every tasks have heap which cases the usage of ram large
   f, if use preemption, then should think about the reload problem.
   
9, task states
   a, running
   b, ready
   c, block
   e, hang-up
   
10, priority control: configMAX_Priorities-1: higher the numer is, higher prioirity is
11, task control block:
    describe the property of task, whcih is described in a struct.
12, task stack
    protect field


16.08.22
1. in FREEstosConfigure.h 中，此值是需要配置为1的，这样就可以统计每个任务的运行时间。
#define configGENERATE_RUN_TIME_STATS	        0                       //为1时启用运行时间统计功能

2. higher number higher priority

3. make sure allocate enough space for stack otherwise the program will stuck somewhere.

4.#define configTOTAL_HEAP_SIZE					((size_t)(46*1024))     //系统所有总的堆大小 too large, wont work
  #define configTOTAL_HEAP_SIZE					((size_t)(10*1024))     //系统所有总的堆大小  //works fine
 
5. START_STK_SIZE: define the size of stack,
for example: if START_STK_SIZE = 50, so the size is 4* 50 = 200Byte  
  this defines in the file task.c
  	/* Allocate space for the stack used by the task being created. */
			pxStack = ( StackType_t * ) pvPortMalloc( ( ( ( size_t ) usStackDepth ) * sizeof( StackType_t ) ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
6. xTaskCreate is dynamic allocation:
			#if( configSUPPORT_DYNAMIC_ALLOCATION == 1 )

	BaseType_t xTaskCreate(	TaskFunction_t pxTaskCode, 
	
7. led0 task and led1 task, led0 task is deleted when led1 task blinks 5 times, the if condition should be if(led1_blink_times == 5), then it works. 
   while if it changes into  if(led1_blink_times > 5 || led1_blink_times == 5), it does not work.