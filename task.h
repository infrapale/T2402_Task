#ifndef __TASK_H__
#define __TASK_H__

#define TASK_RESERVED_NBR  16

#include "Arduino.h"

typedef void (*task_cb)(void);

typedef struct 
{
    char      name[16];
    uint32_t  interval;
    uint32_t  next_run;
    uint16_t  state;
    uint16_t  prev_state;
	uint16_t  wd_cntr;
	uint16_t  wd_limit;
    task_cb   cb;
} task_st;


void task_initialize(uint8_t active_tasks);

void task_set_task(uint8_t task_indx, task_st *task_p);

task_st *task_get_task(uint8_t task_indx);

void task_delay(uint8_t tindx, uint32_t delay_ms);

void task_run(void);

void task_print_status(bool force);

uint8_t task_check_all(void);

void task_clear_cntr(uint8_t tindx);


#endif