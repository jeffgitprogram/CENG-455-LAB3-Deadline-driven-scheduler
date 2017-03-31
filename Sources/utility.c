/*
 * utility.c
 *
 *  Created on: Mar 16, 2017
 *      Author: willmeng
 */

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "os_tasks.h"
#include <timer.h>
#include "dd_functions.h"

#include "utility.h"
#include "gpio1.h"

void Delay_tool(uint32_t delay_time){
	MQX_TICK_STRUCT delay_ticks; // delay length
	TIME_STRUCT		curr_time;
	_timer_id runtime_timer;

	uint32_t prev = curr_time.MILLISECONDS+(curr_time.SECONDS)*1000;
	uint32_t curr = 0;

	int flag = 0;

	_time_get_elapsed(&curr_time);


	_time_get_elapsed_ticks(&delay_ticks);
	_time_add_msec_to_ticks(&delay_ticks, delay_time);

	//printf("Delay start\n");
	runtime_timer = _timer_start_oneshot_at_ticks(Delay_tool_flag,&flag,TIMER_ELAPSED_TIME_MODE,&delay_ticks);
	while(flag == 0){
			  _time_get_elapsed(&curr_time);
			  curr = curr_time.MILLISECONDS+(curr_time.SECONDS)*1000;
			  if(curr!= prev){
				  prev = curr;
				  user_task_runtime++;
			   }
			  /*ALL_OFF();
			  GREEN_ON();
			  RED_ON();*/
	}

}
void Delay_tool_flag(_timer_id id, int* data_ptr, MQX_TICK_STRUCT_PTR tick_ptr){
	*data_ptr = 1;
}

void Cancel_timer(_timer_id id, uint32_t data, MQX_TICK_STRUCT_PTR tick_ptr){
	_timer_id cancel_timer;
	cancel_timer = data;
	_timer_cancel(cancel_timer);
}

void System_monitor(_timer_id id, int* data_ptr, MQX_TICK_STRUCT_PTR tick_ptr){
	int user_execution_time = user_task_runtime;
	user_task_runtime = 0;
	int idle_time = idle_task_runtime;
	idle_task_runtime = 0;
	int succeeded = successful_task;
	successful_task = 0;
	uint32_t system_overhead = (uint32_t)(WAKE_UP_INTERVAL-(user_execution_time+idle_time));
	puts("=========================\n*************************");
	printf("The total user runtime is %d;\nTotal idle time is %d;\nSystem overhead is %d;\nUtilization is %d%\n",user_execution_time,idle_time,system_overhead,user_execution_time*100/WAKE_UP_INTERVAL );
	ListNode* Active_return;
	MallocMemInitNode(&Active_return);
	Active_return = NULL;
	ListNode* Overdue_return;
	MallocMemInitNode(&Overdue_return);
	Overdue_return = NULL;
	int active_task = 0;
	int overdue_task = 0;
	dd_return_active_list(&Active_return);
	dd_return_overdue_list(&Overdue_return);
	if(Active_return!=NULL){
		active_task = Get_length(Active_return);
	}
	if(Overdue_return != NULL){
		overdue_task = Get_length(Overdue_return);
	}
	printf(" %d running\n %d failed\n %d succeeded\n\n",active_task,overdue_task,succeeded);


}
void GPIO_INIT() {
	GPIO_DRV_Init(gpio1_InpConfig0, gpio1_OutConfig0);
}

void INIT_RGB() {
	GPIO_DRV_TogglePinOutput(LED_BLUE);
}
void RED_OFF(){
	GPIO_DRV_SetPinOutput(LED_RED);
}
void GREEN_OFF(){
	GPIO_DRV_SetPinOutput(LED_GREEN);
}
void BLUE_OFF(){
	GPIO_DRV_SetPinOutput(LED_BLUE);
}
void ALL_OFF() {
	RED_OFF();
	GREEN_OFF();
	BLUE_OFF();
}

void RED_ON(){
	GPIO_DRV_ClearPinOutput(LED_RED);
}
void GREEN_ON(){
	GPIO_DRV_ClearPinOutput(LED_GREEN);
}
void BLUE_ON(){
	GPIO_DRV_ClearPinOutput(LED_BLUE);
}
void PEARL_WHITE(){
	RED_ON();
	BLUE_ON();
	GREEN_ON();

}

/*_timer_id Deadline_tracer(DELETE_NOTIFI_DATA_PTR delete_notifi_data,MQX_TICK_STRUCT *deadline_ticks){
	_timer_id overdue_timer;
	overdue_timer = _timer_start_oneshot_at_ticks(dd_delete,delete_notifi_data,TIMER_KERNEL_TIME_MODE,deadline_ticks);
	return overdue_timer;
}*/

