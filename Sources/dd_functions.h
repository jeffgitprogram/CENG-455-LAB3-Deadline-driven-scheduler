#ifndef __dd_functions_H
#define __dd_functions_H


#include "Events.h"
#include "os_tasks.h"
#include <timer.h>
#include <mqx.h>
#include "linklist.h"

//MESSAGE QUEUE

#define NUM_OF_RUNS 100
typedef struct notifi_data {
	uint32_t task_type;
	uint32_t runtime;
	uint32_t deadline;
	uint32_t creation_delay;

} NOTIFI_DATA,*NOTIFI_DATA_PTR;

typedef struct delete_notifi_data {
	uint32_t task_id;
	} DELETE_NOTIFI_DATA,*DELETE_NOTIFI_DATA_PTR;

void dd_tcreate(_timer_id id, NOTIFI_DATA_PTR data_ptr, MQX_TICK_STRUCT_PTR tick_ptr);
void dd_delete(_task_id task_id);
void dd_return_active_list(ListNode** returned_list);
void dd_return_overdue_list(ListNode** returned_list);
bool generate_task(NOTIFI_DATA_PTR notifi_data);

#endif //__dd_functions_H
