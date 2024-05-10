#include "task.h"

typedef struct 
{
    uint8_t  active_tasks;
} task_ctrl_st;

task_st *task[TASK_RESERVED_NBR]; 
task_ctrl_st task_ctrl;

void dummy_task(void)
{

};

task_st undef_task = {"Undefined      ", 0, 0, 0, 255, 0, 0, dummy_task };

void task_initialize(uint8_t active_tasks)
{
    for (uint8_t i = 0; i < TASK_RESERVED_NBR; i++)
    {
        task[i] = &undef_task;
    }
    task_ctrl.active_tasks = active_tasks;
}


void task_set_task(uint8_t task_indx, task_st *task_p) 
{
    if ( task_indx < TASK_RESERVED_NBR )
    {
        task[task_indx] = task_p;  
    }
}

task_st *task_get_task(uint8_t task_indx) 
{
    if ( task_indx < TASK_RESERVED_NBR )
        return task[task_indx];  
	else return &undef_task;
}

void task_delay(uint8_t tindx, uint32_t delay_ms)
{
  task[tindx]->interval = millis() + delay_ms;
}

void task_run(void)
{
    static uint8_t task_indx = 0;

    uint32_t now_ms = millis();
    if (task_indx < task_ctrl.active_tasks)
    {
        if (now_ms >= task[task_indx]->next_run )
        {
            task[task_indx]->prev_state = task[task_indx]->state;
            task[task_indx]->next_run = now_ms + task[task_indx]->interval;
            (*task[task_indx]->cb)();
			task[task_indx]->wd_cntr++;
        }
    } 
    if (++task_indx >= task_ctrl.active_tasks) task_indx = 0;
}

void task_print_status(bool force)
{   char buffer[64];
    // bool do_print = false;
    // Serial.printf("now = %d\n", millis());
    for (uint8_t i = 0; i < task_ctrl.active_tasks; i++)
    {
        if ((task[i]->prev_state != task[i]->state) || force)
        {
            sprintf(buffer,"%s: %d -> %d @ %d # %d %d <? %d\n",
              task[i]->name, 
              task[i]->prev_state, 
              task[i]->state, 
              millis(),
              task[i]->next_run,
			  task[i]->wd_cntr,
			  task[i]->wd_limit);
			Serial.print(buffer);  
        }
    }
}

void task_clear_cntr(uint8_t tindx)
{
	task[tindx]->wd_cntr = 0;
}

uint8_t task_check_all(void)
{
	uint8_t tindx = 255;
	for (uint8_t i = 0; i < task_ctrl.active_tasks; i++)
    {
        if ((task[i]->wd_limit > 0) && (task[i]->wd_cntr >= task[i]->wd_limit))
        {
			tindx = i;
			break;
        }
    }
	return tindx;
}


