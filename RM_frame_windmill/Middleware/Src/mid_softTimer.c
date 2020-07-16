/**
  ******************************************************************************
  * 文件名             : mid_softTimer.c
  * 文件描述           : 软件定时器-实现
	* 版本               : V0.01
  ******************************************************************************
  * 2020,上海交通大学交龙战队版权所有。
  * 保留所有权利。
  *	
	* 说明：编写软件定时器功能。提供了一个挂载在timer3上的1ms软定时器。
	* 更新日志：
	* V0.02（负责人：席望）：
	* 改到timer7上，用于完成音乐播放。
	* V0.01（负责人：唐欣阳）：
	* 创建文件。
	* 
  ******************************************************************************
  */
	
#include "includes.h"
#include <string.h>

soft_timer_s timer_tim7_1ms;

void soft_timer_init(soft_timer_s *soft_timer, uint16_t tick_interval) {
    memset(soft_timer, 0, sizeof(soft_timer_s));
    soft_timer->tick_interval = tick_interval;
}

void soft_timer_schedule(soft_timer_s *soft_timer) {
    uint16_t tick = soft_timer->tick_interval;
    for (task_s *task = soft_timer->tasks; task < soft_timer->tasks + MAX_TASK_AMOUNT; task++) {
        if (task->enable && ((task->delay -= tick) <= 0)) {
            task->func(task, task->args);
            task->delay = task->interval;
        }
    }
}

uint8_t soft_timer_add_task(soft_timer_s *soft_timer, func_t func, void *args, uint16_t interval, task_s **new_task) {
    for (task_s *task = soft_timer->tasks; task < soft_timer->tasks + MAX_TASK_AMOUNT; task++) {
        if (task->enable) continue;
        task->enable = 1;
        task->func = func;
        task->args = args;
        task->interval = interval;
        task->delay = interval;
        if (new_task) *new_task = task;
        return 1;
    }
    return 0;
}

void soft_timer_delete_task(task_s *task) {
    task->enable = 0;
}

void soft_timer_reset_interval(task_s *task, uint16_t interval) {
    task->interval = interval;
}
