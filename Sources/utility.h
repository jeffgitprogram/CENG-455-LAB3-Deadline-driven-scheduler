/*
 * utility.h
 *
 *  Created on: Mar 16, 2017
 *      Author: willmeng
 */


#ifndef SOURCES_UTILITY_H_
#define SOURCES_UTILITY_H_


#include <timer.h>
#include "dd_functions.h"


#define WAKE_UP_INTERVAL 5000
void Delay_tool(uint32_t delay_time);
void Delay_tool_flag(_timer_id id, int* data_ptr, MQX_TICK_STRUCT_PTR tick_ptr);
void Cancel_timer(_timer_id id, uint32_t data, MQX_TICK_STRUCT_PTR tick_ptr);
void System_monitor(_timer_id id, int* data_ptr, MQX_TICK_STRUCT_PTR tick_ptr);
void GPIO_INIT();
void INIT_RGB();
void RED_OFF();
void GREEN_OFF();
void BLUE_OFF();
void ALL_OFF();
void RED_ON();
void GREEN_ON();
void BLUE_ON();
void PEARL_WHITE();

#endif /* SOURCES_UTILITY_H_ */
