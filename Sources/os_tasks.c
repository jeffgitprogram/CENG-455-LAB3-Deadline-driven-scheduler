/* ###################################################################
**     Filename    : os_tasks.c
**     Project     : deadline_driven_scheduler
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-02-27, 16:25, # CodeGen: 1
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         idle_task      - void idle_task(os_task_param_t task_init_data);
**         user_task      - void user_task(os_task_param_t task_init_data);
**         generator_task - void generator_task(os_task_param_t task_init_data);
**         handler_task    - void handler_task(os_task_param_t task_init_data);
**         dds_task       - void dds_task(os_task_param_t task_init_data);
**
** ###################################################################*/
/*!
** @file os_tasks.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup os_tasks_module os_tasks module documentation
**  @{
*/         
/* MODULE os_tasks */

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "os_tasks.h"
#include <timer.h>
#include "dd_functions.h"
#include "utility.h"
#include "mqx_prv.h"
#ifdef __cplusplus
extern "C" {
#endif 

//Globe Value

_pool_id schedule_message_pool;
long user_task_runtime = 0;
long idle_task_runtime = 0;
int successful_task = 0;
_task_id current_running = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Callback    : dds_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void dds_task(os_task_param_t task_init_data)
{


	//Initialize priority para
	_mqx_uint old_priority;
	_mqx_uint active_priority = 13;
	_mqx_uint inactive_priority = 25;

	//Initialize active/overdue tasks linked list, this list is maintained by the scheduler
	ListNode* ActiveList;
	MallocMemInitNode(&ActiveList);
	ActiveList = CreatMyList(ActiveList);
	ListNode* OverdueList;
	MallocMemInitNode(&OverdueList);
	OverdueList = CreatMyList(OverdueList);
	ListNode* lastnode;
	MallocMemInitNode(&lastnode);

	//Initialize local valuables
	uint32_t info_type;
	_task_id tid;
	//_task_id current_running;
	uint32_t relative_deadline;
	uint32_t task_type;
	uint32_t creation_time_msec;
	uint32_t deadline_time_msec;
	uint32_t setnewdeadline_msec;
	uint32_t current_time_msec;
	//Initiate timer out value
	int global_timeout = 0;
	int timeout = 0;
//	int remaining = 0;
	//current_running =0;
	//parameters used for set active priority
//	int immediateoverdue = 0;
	_task_id  lastnode_task_id;
	int offset = 2;

	//Initialize delete task notification data;
	DELETE_NOTIFI_DATA_PTR delete_notifi_data;
	delete_notifi_data = _mem_alloc( sizeof(DELETE_NOTIFI_DATA));

	//Initialize time structure
	TIME_STRUCT creation_time;
	TIME_STRUCT deadline_time;
	TIME_STRUCT currenttime_time;
	MQX_TICK_STRUCT creation_ticks; // Creation time
	MQX_TICK_STRUCT deadline_ticks; // Period length
	MQX_TICK_STRUCT currenttime_ticks;
	TD_STRUCT_PTR td_ptr;



	// Initialize a message pool
	schedule_message_pool = _msgpool_create(sizeof(SCHEDULE_MESSAGE), 32, 1, 0);
	if (schedule_message_pool == MSGPOOL_NULL_POOL_ID) {
		//printf("\nCould not create a message pool\n");
		_task_block();
	}

	// Open a scheduler message queue
	_queue_id  schedule_qid;
	schedule_qid = _msgq_open(SCHEDULER_MESSAGE_QUEUE, 0);
	if (schedule_qid == 0) {
		//printf("\nCould not open the system message queue\n");
		_task_block();
	}
	//SCHEDULE_MESSAGE_PTR scheduler_receive_msg;





#ifdef PEX_USE_RTOS
  while (1) {
#endif
	     SCHEDULE_MESSAGE_PTR scheduler_receive_msg;
		//wait to receive message

			scheduler_receive_msg = _msgq_receive(schedule_qid,timeout);
			ALL_OFF();


		if(scheduler_receive_msg == NULL){//Task is over due

			lastnode = GetLastNode(ActiveList);
			creation_time_msec = lastnode->creation_time;
		    deadline_time_msec = lastnode->deadline;
			task_type = lastnode->task_type;
			tid = lastnode->tid;
			OverdueList = InsertNode(OverdueList,tid,deadline_time_msec,task_type,creation_time_msec);
			//printf("Task %d Overdued. \n",tid);
			ALL_OFF();
			RED_ON();
			//Delete last node in the active list

			//set this task to an inactive priority
			_task_set_priority(tid,PRIORITY_OSA_TO_RTOS(inactive_priority),&old_priority);
			DeleteTailNode(&ActiveList);
			//TODO Safety
			if(ActiveList != NULL)
			{
				lastnode = GetLastNode(ActiveList);
				lastnode_task_id = lastnode->tid;//Get the new ready to run task
				current_running = lastnode_task_id;//Record the task id

				_task_set_priority(lastnode_task_id,PRIORITY_OSA_TO_RTOS(active_priority),&old_priority);

				td_ptr = _task_get_td(lastnode_task_id);

				if (td_ptr != NULL)	_task_ready(td_ptr);//Try to make the task ready

				setnewdeadline_msec = lastnode->deadline;
				_time_get_elapsed_ticks(&currenttime_ticks);
				_ticks_to_time(&currenttime_ticks,&currenttime_time);
				current_time_msec = currenttime_time.SECONDS*1000+currenttime_time.MILLISECONDS;
				global_timeout = setnewdeadline_msec-current_time_msec;
				if(global_timeout > 0){//Exist a feasible time out value, then convert it to milisecond
				   timeout = global_timeout+offset;
				}else{//If does not exist a feasible time out value
					   timeout = 1; //If timeout value has been changed before
					   _task_abort(lastnode_task_id);

				}
			}else{//List is empty, waiting on new task
				timeout = 0;
				current_running = 0;
			}

		  }
		else{
			info_type = scheduler_receive_msg->DATA[0];

			if(info_type == 1){// new task arrives

				//get task id, relative deadline,task type;
				tid = scheduler_receive_msg->DATA[1];
				relative_deadline = scheduler_receive_msg->DATA[2];
				task_type = scheduler_receive_msg->DATA[3];


				_time_get_elapsed_ticks(&creation_ticks);
				_time_get_elapsed_ticks(&deadline_ticks);
				_time_add_msec_to_ticks(&deadline_ticks, relative_deadline);

				_ticks_to_time(&creation_ticks,&creation_time);
				_ticks_to_time(&deadline_ticks,&deadline_time);

				creation_time_msec = creation_time.SECONDS*1000+creation_time.MILLISECONDS;
				deadline_time_msec = deadline_time.SECONDS*1000+deadline_time.MILLISECONDS;



				ActiveList = InsertNode(ActiveList, tid,deadline_time_msec,task_type,creation_time_msec);//Insert the node and make it sorted


				//Acquire task id which has the earliest deadline
				lastnode = GetLastNode(ActiveList);
				lastnode_task_id = lastnode->tid;

				//TODO: Safety
				if(current_running == 0){//In this case, no task is running
						current_running = lastnode_task_id;//Record the new ready for run task
						_task_set_priority(lastnode_task_id,PRIORITY_OSA_TO_RTOS(active_priority),&old_priority);//Let the new task run
						td_ptr = _task_get_td(lastnode_task_id);

						if(td_ptr != NULL){//Try to make the task ready
									_task_ready(td_ptr);
							}

				}else{//There is task running

						if(current_running != lastnode_task_id){//Preempt happened
							_task_set_priority(current_running,PRIORITY_OSA_TO_RTOS(inactive_priority),&old_priority);//Set the current task priority to low
							current_running = lastnode_task_id;//record down the new ready for run task
							_task_set_priority(lastnode_task_id,PRIORITY_OSA_TO_RTOS(active_priority),&old_priority);//Make the new task ready to run

							td_ptr = _task_get_td(lastnode_task_id);//Try make the task ready, if it is blocked before

							if (td_ptr != NULL){
								_task_ready(td_ptr);
								}
							//printf("Preempt happened. \n");
							ALL_OFF();
							BLUE_ON();

							}//else  the current task is the EDL task, let it keep running ,nothing happened
				}

					setnewdeadline_msec = lastnode->deadline;
					_time_get_elapsed_ticks(&currenttime_ticks);
					_ticks_to_time(&currenttime_ticks,&currenttime_time);
					current_time_msec = currenttime_time.SECONDS*1000+currenttime_time.MILLISECONDS;
					global_timeout = setnewdeadline_msec-current_time_msec;

				if(global_timeout > 0){//Exist a feasible time out value
					timeout = global_timeout+offset;
				}else{//If does not exist a feasible time out value, it is gonna overdue
					   timeout = 1; //If timeout value has been changed before
					   _task_abort(lastnode_task_id);

				}

				//return success message to dd_tcreate
				scheduler_receive_msg->HEADER.TARGET_QID = scheduler_receive_msg->HEADER.SOURCE_QID;
				scheduler_receive_msg->HEADER.SOURCE_QID = schedule_qid;
				scheduler_receive_msg->HEADER.SIZE= sizeof(MESSAGE_HEADER_STRUCT)+sizeof((uint32_t*)scheduler_receive_msg->DATA)+1;
				scheduler_receive_msg->DATA[4] = 1;
				 _msgq_send(scheduler_receive_msg);
				 _msg_free(scheduler_receive_msg);

			}//end if(info_type == 1)

			// task complete
			if(info_type == 2){
					tid = scheduler_receive_msg->DATA[1];

					//Delete last node in the active list
					lastnode = GetLastNode(ActiveList);
					if(lastnode != NULL){
						lastnode_task_id = lastnode->tid;
						if(lastnode_task_id == tid){
							DeleteTailNode(&ActiveList);
							_task_abort(tid);
							//printf("Task %d deleted .\n\n",tid);
							  ALL_OFF();
							  GREEN_ON();

							successful_task++;
						}
					}
					//set this task to an inactive priority
					//Then, acquire task id which has the earliest deadline

					//TODO: Safety
					if(ActiveList != NULL){
						lastnode = GetLastNode(ActiveList);
						lastnode_task_id = lastnode->tid;
						current_running = lastnode_task_id;//Record the next running task
						_task_set_priority(lastnode_task_id,PRIORITY_OSA_TO_RTOS(active_priority),&old_priority);

						td_ptr = _task_get_td(lastnode_task_id);//Try to make the task ready
						if (td_ptr != NULL) _task_ready(td_ptr);

						setnewdeadline_msec = lastnode->deadline;
						_time_get_elapsed_ticks(&currenttime_ticks);
						_ticks_to_time(&currenttime_ticks,&currenttime_time);
						current_time_msec = currenttime_time.SECONDS*1000+currenttime_time.MILLISECONDS;
						global_timeout = setnewdeadline_msec-current_time_msec;
						if(global_timeout > 0){//Exist a feasible time out value, then convert it to millisecond
							timeout = global_timeout+offset;
						}else{//If does not exist a feasible time out value, it is going to overdue
							   timeout = 1; //If timeout value has been changed before
							   _task_abort(lastnode_task_id);

						}

					}else{
							timeout = 0;
							current_running = 0;
					}
					_msg_free(scheduler_receive_msg);
			}



			if(info_type == 3){// request active tasks
				scheduler_receive_msg->HEADER.TARGET_QID = scheduler_receive_msg->HEADER.SOURCE_QID;
				scheduler_receive_msg->HEADER.SOURCE_QID = schedule_qid;
				scheduler_receive_msg->HEADER.SIZE= sizeof(MESSAGE_HEADER_STRUCT)+sizeof((ListNode*)scheduler_receive_msg->tlist)+1;
				scheduler_receive_msg->tlist = ActiveList;
				_msgq_send(scheduler_receive_msg);
				_msg_free(scheduler_receive_msg);
			}



			if(info_type == 4){// request inactive tasks
				scheduler_receive_msg->HEADER.TARGET_QID = scheduler_receive_msg->HEADER.SOURCE_QID;
				scheduler_receive_msg->HEADER.SOURCE_QID = schedule_qid;
				scheduler_receive_msg->HEADER.SIZE= sizeof(MESSAGE_HEADER_STRUCT)+sizeof((ListNode*)scheduler_receive_msg->tlist)+1;
				scheduler_receive_msg->tlist = OverdueList;
				_msgq_send(scheduler_receive_msg);
				_msg_free(scheduler_receive_msg);
			}

		}
   // OSA_TimeDelay(10);                 /* Example code (for task release) */

    
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/*
** ===================================================================
**     Callback    : idle_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void idle_task(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */
	TIME_STRUCT curr_time;
	_time_get_elapsed(&curr_time);
	uint32_t prev = curr_time.MILLISECONDS+(curr_time.SECONDS)*1000;
	uint32_t curr = 0;

#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */
	 // OSA_TimeDelay(10);
	  /* Example code (for task release) */
	  if(current_running == 0){
	  _time_get_elapsed(&curr_time);
	  curr = curr_time.MILLISECONDS+(curr_time.SECONDS)*1000;
	  if(curr!= prev){
		  prev = curr;
		  idle_task_runtime++;
	   }

	  }  else{
		  OSA_TimeDelay(10);
	  }
    
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/*
** ===================================================================
**     Callback    : user_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void user_task(os_task_param_t task_init_data)
{

	_task_id tid = _task_get_id();
	//printf("%d runs for %d \n",tid,task_init_data);
	int runtime = task_init_data;
	Delay_tool(runtime);
    dd_delete(tid);
  
#ifdef PEX_USE_RTOS
  while (0) {
#endif
    /* Write your code here ... */
	  OSA_TimeDelay(10);/* Example code (for task release) */
#ifdef PEX_USE_RTOS   
  }


#endif    
}

/*
** ===================================================================
**     Callback    : generator_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/

//TODO:  switch case to create aperiodic task
//TODO:  change input arguments
void generator_task(os_task_param_t task_init_data)
{


	MQX_TICK_STRUCT monitor_ticks;
	MQX_TICK_STRUCT monitor_dticks;
	GPIO_INIT();
	INIT_RGB();

	NOTIFI_DATA_PTR notifi_data;
	notifi_data = _mem_alloc( sizeof(NOTIFI_DATA));

	_timer_id monitor_task;

	if(_timer_create_component(TIMER_DEFAULT_TASK_PRIORITY, 4096)!= MQX_OK){
		_mqx_exit(1);
	}

	_time_get_elapsed_ticks(&monitor_ticks);
	_time_add_msec_to_ticks(&monitor_ticks, WAKE_UP_INTERVAL);
	_time_init_ticks(&monitor_dticks,0);
	_time_add_msec_to_ticks(&monitor_dticks, WAKE_UP_INTERVAL);

	bool result;
	notifi_data->deadline = 5000;
	notifi_data->runtime = 1000;
	notifi_data->task_type =1;
	notifi_data->creation_delay = 0;
	result = generate_task(notifi_data);
	_time_delay(10);
	notifi_data->deadline = 5000;
	notifi_data->runtime = 1000;
	notifi_data->task_type =1;
	notifi_data->creation_delay = 0;
	result = generate_task(notifi_data);
	_time_delay(10);
	notifi_data->deadline = 5000;
	notifi_data->runtime = 1000;
	notifi_data->task_type =1;
	notifi_data->creation_delay = 0;
	result = generate_task(notifi_data);
	_time_delay(10);
	notifi_data->deadline = 5000;
	notifi_data->runtime = 1000;
	notifi_data->task_type =1;
	notifi_data->creation_delay = 0;
	result = generate_task(notifi_data);
	_time_delay(10);
	monitor_task=_timer_start_periodic_at_ticks(System_monitor,0,TIMER_KERNEL_TIME_MODE,&monitor_ticks,&monitor_dticks);
	_time_delay(MONITOR_WORKTIME*1000);
	_timer_cancel(monitor_task);
	_task_block();



	//Delay_tool(30);
	//_timer_cancel(periodic_task1);
  
#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */
    
    
    OSA_TimeDelay(10);                 /* Example code (for task release) */
   
    
    
    
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/*
** ===================================================================
**     Callback    : handler_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void handler_task(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */
  _task_block();
#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */
    printf("Handle task is running. \n");
    
    OSA_TimeDelay(10);                 /* Example code (for task release) */
   
    
    
    

#ifdef PEX_USE_RTOS   
  }
#endif    
}

/* END os_tasks */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
