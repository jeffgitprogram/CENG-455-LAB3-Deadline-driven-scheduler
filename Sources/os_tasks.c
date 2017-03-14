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

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

#include "dd_functions.h"



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

	// Create a message pool
	_pool_id  message_pool;
	message_pool = _msgpool_create(sizeof(SCHEDULE_MESSAGE), 32, 0, 0);
	if (message_pool == MSGPOOL_NULL_POOL_ID) {
		printf("\nCould not create a message pool\n");
		_task_block();
	}

	// Open a message queue
	_queue_id  schedule_qid;
	schedule_qid = _msgq_open(SCHEDULER_MESSAGE_QUEUE, 0);
	printf("schedule task qid: %d\n",schedule_qid);
	if (schedule_qid == 0) {
		printf("\nCould not open the system message queue\n");
		_task_block();
	}

#ifdef PEX_USE_RTOS
  while (1) {
#endif
    OSA_TimeDelay(10);                 /* Example code (for task release) */
    
    // Check schedule queue
	/*msg_ptr = _msgq_receive(schedule_qid, 0);
	if (msg_ptr == NULL) {
		_task_block();
	}
    
	int command_num = msg_ptr->DATA[0]; // First char of each message is a command int.
	char *schedule_input = msg_ptr->DATA;

	char tokens[5][20];
	while ((tokens[i] = strsep(&schedule_input, " "))) {
		i++;
	}

	switch(command_num) {
		case 0: // create task

			break;

		case 1: // delete task
			dd_delete(tokens[1]);
			break;

		case 2: // deadline TODO

			break;

		case 3: // return active
			dd_return_active_list(); // TODO what is list pointer for?
			break;

		case 4: // return overdue
			dd_return_overdue_list(); // TODO
			break;
	}
    */
    
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
**     Callback    : user_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void user_task(os_task_param_t task_init_data)
{


  
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
**     Callback    : generator_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void generator_task(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */

	//dd_tcreate(templateindex, tokens[1]); // TODO get task template index
  
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
  
#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */
    
    
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
