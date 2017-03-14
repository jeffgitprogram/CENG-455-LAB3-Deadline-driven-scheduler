

#include <mqx.h>
#include <message.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <mutex.h>

#include "Cpu.h"
#include "Events.h"
#include "os_tasks.h"
#include "dd_functions.h"

_task_id dd_tcreate(uint32_t template_index, uint32_t deadline) {
	return 0;

}

uint32_t dd_delete(uint32_t task_id) {
	return 0;
}

uint32_t dd_return_active_list() {
	return 0;
}

uint32_t dd_return_overdue_list() {
	return 0;
}
