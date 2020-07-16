/**
  ******************************************************************************
  * �ļ���             : mid_softTimer.c
  * �ļ�����           : �����ʱ��-ʵ��
	* �汾               : V0.01
  ******************************************************************************
  * 2020,�Ϻ���ͨ��ѧ����ս�Ӱ�Ȩ���С�
  * ��������Ȩ����
  *	
	* ˵������д�����ʱ�����ܡ��ṩ��һ��������timer3�ϵ�1ms��ʱ����
	* ������־��
	* V0.02�������ˣ�ϯ������
	* �ĵ�timer7�ϣ�����������ֲ��š�
	* V0.01�������ˣ�����������
	* �����ļ���
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
