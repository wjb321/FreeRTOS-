��stm32f103ze ��ֲ��C8T6�У�


1������startup_hd.s to startup_md.s. ע�⣺�����ļ���Ҫ����Сϵͳ��cmsis-> arm�����ȡ��
2��ͨ��ħ��������оƬ��c/c++�µ�HD -> MD�� ultilities�¸���flash size.
3, ����freeRTOS ��RAM�����С�����⣬��һ��ͨ�������󣬷���û��Ч����Ȼ���С������ֹ��ܡ�
   �����������ˣ�https://blog.csdn.net/qq_33161479/article/details/90064215


4, FreeRTOSconfig.h �е��ļ����ã�
��condition compile��INCLUDE_ ��ͷ�ĺ��ļ�������ʾʹ�ܻ��߳���FreeRTOS����Ӧ��API������ ���þ�������FreeRTOS�п�ѡ��API������
INCLUDE_vTaskPrioritySet �ͱ�ʾʹ�ܺ���vTASKPrioritySet() 

5, Config ��ͷ�ĺ��ļ���������ͬ�� configUSE_PREEMPTION�ȡ� co-routine, is not develped anymore, but no plans to remove that or update that.

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
1. in FREEstosConfigure.h �У���ֵ����Ҫ����Ϊ1�ģ������Ϳ���ͳ��ÿ�����������ʱ�䡣
#define configGENERATE_RUN_TIME_STATS	        0                       //Ϊ1ʱ��������ʱ��ͳ�ƹ���

2. higher number higher priority

3. make sure allocate enough space for stack otherwise the program will stuck somewhere.

4.#define configTOTAL_HEAP_SIZE					((size_t)(46*1024))     //ϵͳ�����ܵĶѴ�С too large, wont work
  #define configTOTAL_HEAP_SIZE					((size_t)(10*1024))     //ϵͳ�����ܵĶѴ�С  //works fine
 
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