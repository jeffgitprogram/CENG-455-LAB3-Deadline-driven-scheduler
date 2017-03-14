/* ###################################################################
**     Filename    : os_tasks.h
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
** @file os_tasks.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup os_tasks_module os_tasks module documentation
**  @{
*/         

#ifndef __os_tasks_H
#define __os_tasks_H
/* MODULE os_tasks */

#include "fsl_device_registers.h"
#include "clockMan1.h"
#include "pin_init.h"
#include "osa1.h"
#include "mqx_ksdk.h"
#include "uart1.h"
#include "fsl_mpu1.h"
#include "fsl_hwtimer1.h"
#include "MainTask.h"
#include "DDSTask.h"
#include "HandlerTask.h"
#include "GeneratorTask.h"
#include "UserTask.h"
#include "IdleTask.h"
#include "myUART.h"
#include "message.h"
#include "dd_functions.h"

/* Message Queue */
#define SCHEDULER_MESSAGE_QUEUE 8

/* Message Pool */
typedef struct schedule_message {
	MESSAGE_HEADER_STRUCT 	HEADER;
	unsigned char 			DATA[32];
} SCHEDULE_MESSAGE, *SCHEDULE_MESSAGE_PTR;

/*Task Link list, maintained by */
typedef struct task_list {
	uint32_t tid;
	uint32_t deadline;
	uint32_t task_type;
	uint32_t creation_time;
	struct task_list *next_cell;
	struct task_list *previous_cell;
} TASK_LIST, *TASK_LIST_PTR;

typedef struct overdue_tasks {
	uint32_t tid;
	uint32_t deadline;
	uint32_t task_type;
	uint32_t creation_time;
	struct overdue_tasks *next_cell;
	struct overdue_tasks *previous_cell;
} OVERDUE_TASKS, *OVERDUE_TASKS_PTR;


#ifdef __cplusplus
extern "C" {
#endif 


void idle_task(os_task_param_t task_init_data);


void user_task(os_task_param_t task_init_data);


void generator_task(os_task_param_t task_init_data);


void handler_task(os_task_param_t task_init_data);


void dds_task(os_task_param_t task_init_data);


/* END os_tasks */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif /* ifndef __os_tasks_H*/

/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
