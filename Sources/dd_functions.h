#ifndef __dd_functions_H
#define __dd_functions_H


#include "Events.h"
#include "os_tasks.h"



_task_id dd_tcreate(uint32_t template_index, uint32_t deadline);
uint32_t dd_delete(uint32_t task_id);
uint32_t dd_return_active_list();
uint32_t dd_return_overdue_list();

#endif //__dd_functions_H
