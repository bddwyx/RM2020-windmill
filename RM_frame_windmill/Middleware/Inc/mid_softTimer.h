/**
  ******************************************************************************
  * �ļ���             : mid_softTimer.h
  * �ļ�����           : �����ʱ��-�ӿ�
	* �汾               : V0.01
  ******************************************************************************
  * 2020,�Ϻ���ͨ��ѧ����ս�Ӱ�Ȩ���С�
  * ��������Ȩ����
  *	
	* ˵������д�����ʱ�����ܡ��ṩ��һ��������timer3�ϵ�1ms��ʱ����
	* ������־��
	* V0.01�������ˣ�����������
	* �����ļ���
	* 
  ******************************************************************************
  */
	
#ifndef __MID_SOFTTIMER_H_INCLUDED
#define __MID_SOFTTIMER_H_INCLUDED

#include "stm32f4xx.h"

// ʹ�÷���������һ��soft_timer_s��ȫ�ֱ������磺
// soft_timer_s timer1;
// ��ʼ������ʱ����
// soft_timer_init(&timer1, tick_interval);
// ��Ӳ����ʱ���ص������е��ã�
// soft_time_scheduler(&timer1);

#ifndef MAX_TASK_AMOUNT
#define MAX_TASK_AMOUNT (32) //������ʱ�����������
#endif
struct task_s;

typedef void(*func_t)(struct task_s *, void *);

typedef struct task_s {
    func_t func;       // �ص���������
    void *args;        // �������Զ�������ĵ�ַ
    int16_t interval; // �������ĵ��ü�� (ms)
    int16_t delay;    // ��ǰ�����´ε��õ�ʱ�� (ms)
    uint8_t enable;    // ��־��ǰ�����Ƿ�����
} task_s;

typedef struct soft_timer_s {
    task_s tasks[MAX_TASK_AMOUNT];  // �����б�
    uint16_t tick_interval;         // ����ʱ���ĵ��ü�� (ms)
} soft_timer_s;


// ��ʼ��һ�������ʱ��
// ������
//     soft_timer: ָ��һ����ʱ��ʵ����ָ��
//     tick_interval: ����ʱ���ĵ��ü�� (ms)
void soft_timer_init(soft_timer_s *soft_timer, uint16_t tick_interval);

// �����ʱ���ĵ���ִ�к���
// ������
//     soft_timer: ָ��һ����ʱ��ʵ����ָ��
void soft_timer_schedule(soft_timer_s *soft_timer);

// �������ʱ�������һ������
// ������
//     soft_timer: ָ��һ����ʱ��ʵ����ָ��
//     func: ����Ĺ�������
//     args: ���������û��Զ�������ĵ�ַ
//     interval: �����ִ�м��(ms)
//     new_task: ���ڽ����������ַ�Ķ���ָ��
// ����ֵ��
//     0: �������ʧ�ܣ��������б�����
//     1: ������ӳɹ�
uint8_t soft_timer_add_task(soft_timer_s *soft_timer, func_t func, void *args, uint16_t interval, task_s **new_task);

// ɾ��һ������
// ������
//     task: ָ�������ָ��
void soft_timer_delete_task(task_s *task);

// ����һ�������ִ�м��
// ������
//     task: ָ�������ָ��
//     interval: �����ִ�м��(ms)
void soft_timer_reset_interval(task_s *task, uint16_t interval);


// һ�������ʱ����ʵ������
extern soft_timer_s timer_tim7_1ms;

#endif
