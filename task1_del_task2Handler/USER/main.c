#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"


#define START_TASK_PRIO		1
#define START_STK_SIZE 		128   // the size is 128 * 4 bytes
TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);


#define LED0_TASK_PRIO		2	
#define LED0_STK_SIZE 		50  
TaskHandle_t LED0Task_Handler;
void led0_task(void *pvParameters);


#define LED1_TASK_PRIO		3
#define LED1_STK_SIZE 		50  
TaskHandle_t LED1Task_Handler;
void led1_task(void *pvParameters);


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 
	delay_init();	    				//��ʱ������ʼ��	  
	uart_init(115200);					//��ʼ������
	LED_Init();		  					//��ʼ��LED
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
	
	  xTaskCreate((TaskFunction_t )led1_task,     
                (const char*    )"led1_task",   
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler); 
    //����LED0����
    xTaskCreate((TaskFunction_t )led0_task,     	
                (const char*    )"led0_task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&LED0Task_Handler);   
    //����LED1����
        
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

//LED0������ 
void led0_task(void *pvParameters)
{   char led1_blink_times = 0;
	  while(1)
    { 
			led1_blink_times++;
			printf("led1_blink_times %d\r\n",led1_blink_times);
			LED0= ~LED0;
			if(led1_blink_times == 5) 
			{ 
				if(LED1Task_Handler != NULL)
				{
					vTaskDelete(LED1Task_Handler);
				}
				
				printf("led1 task is deleted\r\n");
				//led1_blink_times = 0;
			}
     vTaskDelay(500);
			
    }
}   

//LED1������
void led1_task(void *pvParameters)
{
    while(1)
    {
        LED1=0;
        vTaskDelay(200);
        LED1=1;
        vTaskDelay(800);
    }
}
