

#include <mqx.h>
#include <message.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <mutex.h>


#include "Cpu.h"
#include "Events.h"
#include "dd_functions.h"
#include "utility.h"




void dd_tcreate(_timer_id id, NOTIFI_DATA_PTR data_ptr, MQX_TICK_STRUCT_PTR tick_ptr) {
	//printf("dd_tcreate: %d,%d,%d\n",data_ptr->deadline,data_ptr->runtime,data_ptr->task_template_index);
	uint32_t deadline = data_ptr->deadline;
	uint32_t runtime = data_ptr->runtime;
	uint32_t type = data_ptr->task_type;


	//Open a message queue
	SCHEDULE_MESSAGE_PTR task_create_send;
	_queue_id task_create_send_qid = _msgq_open((_queue_number)DD_CREATE_QUEUE , 0);
	//Allocate a message from a message pool
	task_create_send = (SCHEDULE_MESSAGE_PTR)_msg_alloc(schedule_message_pool);
	_task_id currTask = _task_create_blocked(0, USERTASK_TASK, (uint32_t)runtime);
	//printf("current Task id is: %d\n",currTask);

	task_create_send->HEADER.SOURCE_QID = task_create_send_qid;
	task_create_send->HEADER.TARGET_QID = _msgq_get_id(0,SCHEDULER_MESSAGE_QUEUE);
	task_create_send->HEADER.SIZE= sizeof(MESSAGE_HEADER_STRUCT)+sizeof((uint32_t*)task_create_send->DATA)+1;
	task_create_send->DATA[0]= 1;
	task_create_send->DATA[1]= currTask;
	task_create_send->DATA[2]= deadline;
    task_create_send->DATA[3]= type;
    _msgq_send(task_create_send);

    task_create_send = _msgq_receive(task_create_send_qid,0);
    int result = 0;
    result = task_create_send->DATA[4];
    if(result){
    	_msg_free(task_create_send);
    	_msgq_close(task_create_send_qid);
    }

}

void dd_delete(_task_id task_id) {


	//Allocate a message from a message pool
	SCHEDULE_MESSAGE_PTR task_delete_send;
	task_delete_send = (SCHEDULE_MESSAGE_PTR)_msg_alloc(schedule_message_pool);
	task_delete_send->HEADER.SOURCE_QID = 0;
	task_delete_send->HEADER.TARGET_QID = _msgq_get_id(0,SCHEDULER_MESSAGE_QUEUE);
	task_delete_send->HEADER.SIZE= sizeof(MESSAGE_HEADER_STRUCT)+sizeof((uint32_t*)task_delete_send->DATA)+1;
	task_delete_send->DATA[0]= 2;//task complete
	task_delete_send->DATA[1]= task_id;

    _msgq_send_urgent(task_delete_send);

}

void dd_return_active_list(ListNode** returned_list) {
	SCHEDULE_MESSAGE_PTR active_list_request;
    _queue_id active_list_request_qid = _msgq_open((_queue_number)DD_ACTIVE_REQUEST_QUEUE , 0);
	active_list_request = (SCHEDULE_MESSAGE_PTR)_msg_alloc(schedule_message_pool);
	active_list_request->HEADER.SOURCE_QID = active_list_request_qid;
	active_list_request->HEADER.TARGET_QID = _msgq_get_id(0,SCHEDULER_MESSAGE_QUEUE);
	active_list_request->HEADER.SIZE= sizeof(MESSAGE_HEADER_STRUCT)+sizeof((uint32_t*)active_list_request->DATA)+1;
	active_list_request->DATA[0] = 3;
	_msgq_send(active_list_request);

	active_list_request = _msgq_receive(active_list_request_qid,0);
	*returned_list = active_list_request->tlist;

	_msg_free(active_list_request);
	_msgq_close(active_list_request_qid);
}

void dd_return_overdue_list(ListNode** returned_list) {
	SCHEDULE_MESSAGE_PTR overdue_list_request;
	_queue_id overdue_list_request_qid = _msgq_open((_queue_number)DD_OVERDUE_REQUEST_QUEUE , 0);
	overdue_list_request = (SCHEDULE_MESSAGE_PTR)_msg_alloc(schedule_message_pool);
	overdue_list_request->HEADER.SOURCE_QID = overdue_list_request_qid;
	overdue_list_request->HEADER.TARGET_QID = _msgq_get_id(0,SCHEDULER_MESSAGE_QUEUE);
	overdue_list_request->HEADER.SIZE= sizeof(MESSAGE_HEADER_STRUCT)+sizeof((uint32_t*)overdue_list_request->DATA)+1;
	overdue_list_request->DATA[0] = 4;
	_msgq_send(overdue_list_request);

	overdue_list_request = _msgq_receive(overdue_list_request_qid,0);
	*returned_list = overdue_list_request->tlist;

	_msg_free(overdue_list_request);
	_msgq_close(overdue_list_request_qid);
}

bool generate_task(NOTIFI_DATA_PTR notifi_data){
	NOTIFI_DATA_PTR generate_data;
	generate_data = _mem_alloc( sizeof(NOTIFI_DATA));
	generate_data-> deadline = notifi_data->deadline ;
	generate_data-> runtime = notifi_data->runtime ;
	generate_data-> task_type = notifi_data->task_type;
	generate_data-> creation_delay = notifi_data->creation_delay;

	MQX_TICK_STRUCT pt1_ticks;
	MQX_TICK_STRUCT pt1_dticks;
	MQX_TICK_STRUCT cancel_dticks;
	_timer_id periodic_task1;

	if(generate_data->task_type == 1){//Periodic
		_time_get_elapsed_ticks(&pt1_ticks);
		_time_add_msec_to_ticks(&pt1_ticks, (generate_data-> creation_delay));
		_time_init_ticks(&pt1_dticks,0);
		_time_add_msec_to_ticks(&pt1_dticks, (generate_data-> deadline));
		periodic_task1 = _timer_start_periodic_at_ticks(dd_tcreate,generate_data,TIMER_KERNEL_TIME_MODE,&pt1_ticks,&pt1_dticks);
		if(periodic_task1 != TIMER_NULL_ID){//Cancel the task generating after n cycles
			_time_get_elapsed_ticks(&cancel_dticks);
			_time_add_msec_to_ticks(&cancel_dticks, ((generate_data-> creation_delay)+(generate_data-> deadline)*(NUM_OF_RUNS-1)));//Only allow the task to run 5
			_timer_start_oneshot_at_ticks(Cancel_timer,periodic_task1,TIMER_KERNEL_TIME_MODE,&cancel_dticks);
			return TRUE;
		}else{
			printf("timer failed\n");
		}
	}
	if(generate_data->task_type == 2){//Aperiodic
		_time_get_elapsed_ticks(&pt1_ticks);
		_time_add_msec_to_ticks(&pt1_ticks, (generate_data-> creation_delay));
		periodic_task1 = _timer_start_oneshot_at_ticks(dd_tcreate,generate_data,TIMER_KERNEL_TIME_MODE,&pt1_ticks);
		if(periodic_task1 != TIMER_NULL_ID){
			return TRUE;
		}
	}
	return FALSE;
}



