/**
  ******************************************************************************
  * File Name          : ControlTask.c
  * Description        : 主控制任务
  ******************************************************************************
  *
  * Copyright (c) 2019 Team Jiao Long-Shanghai Jiao Tong University
  * All rights reserved.
  *
  ******************************************************************************
  */
#include "includes.h"

WorkState_e WorkState = PREPARE_STATE;
uint16_t prepare_time = 0;
MusicNote Start[] = {
	{H1, 150}, {0, 30}, 
	{H2, 150}, {0, 30}, 
	{H3, 150}, {0, 30}, 
	{H4, 150}, {0, 30},  
	{H5, 250}, {0, 30},
};

PID_Regulator_t CMRotatePID = CHASSIS_MOTOR_ROTATE_PID_DEFAULT; 
extern int32_t auto_counter;

void playMusicSuperMario(void){
	HAL_TIM_PWM_Start(BUZZER_TIM, TIM_CHANNEL_1);
	for(int i = 0; i < sizeof(Start) / sizeof(MusicNote); i++){
			PLAY(Start[i].note, Start[i].time);
	}
	HAL_TIM_PWM_Stop(BUZZER_TIM, TIM_CHANNEL_1);
}

//状态机切换
void WorkStateFSM(void)
{
	static uint8_t as2_first_enter = 0;
	
	switch (WorkState)
	{
		case PREPARE_STATE:				//准备模式
		{
			
			if(prepare_time < 1000) prepare_time++;	
			if(prepare_time == 1000)//开机一秒进入正常模式
			{
				playMusicSuperMario();
				CMRotatePID.Reset(&CMRotatePID);
				WorkState = NORMAL_STATE;
				prepare_time = 0;
			}
		}break;
		case NORMAL_STATE:				//正常模式
		{
			if (inputmode == STOP) WorkState = STOP_STATE;
			if (inputmode == REMOTE_INPUT)
			{
				if(functionmode == MIDDLE_POS) WorkState = ADDITIONAL_STATE_ONE;
				if(functionmode == LOWER_POS) WorkState = ADDITIONAL_STATE_TWO;
			}
			
			as2_first_enter = 1;
		}break;
		case ADDITIONAL_STATE_ONE:		//附加模式一
		{			
			if (inputmode == STOP) WorkState = STOP_STATE;
			if (inputmode == REMOTE_INPUT)
			{
				if(functionmode == UPPER_POS) WorkState = NORMAL_STATE;
				if(functionmode == LOWER_POS) WorkState = ADDITIONAL_STATE_TWO;
			}
			
			as2_first_enter = 1;
		}break;
		case ADDITIONAL_STATE_TWO:		//附加模式二
		{
			if (inputmode == STOP) WorkState = STOP_STATE;
			if (inputmode == REMOTE_INPUT)
			{
				if(functionmode == UPPER_POS) WorkState = NORMAL_STATE;
				if(functionmode == MIDDLE_POS) WorkState = ADDITIONAL_STATE_ONE;
			}
			
			if(as2_first_enter)
			{
				as2_first_enter = 0;
			}
		}break;
		case STOP_STATE:				//紧急停止
		{
			for(int i=0;i<8;i++) {InitMotor(can1[i]);InitMotor(can2[i]);}
			setCAN11();setCAN12();setCAN21();setCAN22();
			if (inputmode == REMOTE_INPUT || inputmode == KEY_MOUSE_INPUT)
			{
				WorkState = NORMAL_STATE;
				FunctionTaskInit();
			}
		}break;
	}
}
void ControlRotate(void)
{	
	CMRotatePID.ref = 0;
	CMRotatePID.fdb = rotate_speed;
	CMRotatePID.Calc(&CMRotatePID);   
	ChassisSpeedRef.rotate_ref = CMRotatePID.output * 13 + ChassisSpeedRef.forward_back_ref * 0.01 + ChassisSpeedRef.left_right_ref * 0.01;
}

void Chassis_Data_Decoding(uint16_t tim6_count)
{
//	ControlRotate();
	switch(inputmode)
	{
		case REMOTE_INPUT: 
			switch(WorkState)
			{
				case NORMAL_STATE:
				   CMFL.TargetAngle += 0.06; break;
				case ADDITIONAL_STATE_ONE:
					 CMFL.TargetAngle += (0.785 * sin(1.884 * tim6_count / 1000) + 1.305) * 360 / 6.2832 / 1000; break;
			  default: ;
			}
			break;
		case KEY_MOUSE_INPUT:
			switch(WorkState)
			{
				case NORMAL_STATE:
				   CMFL.TargetAngle -= 0.06; break;
				case ADDITIONAL_STATE_ONE:
					 CMFL.TargetAngle -= (0.785 * sin(1.884 * tim6_count / 1000) + 1.305) * 360 / 6.2832 / 1000; break;
			  default: ;
			}
			break;
		default : ;
	}
	
	/*(  -ChassisSpeedRef.forward_back_ref	*0.075 
						+ ChassisSpeedRef.left_right_ref	*0.075 
						+ ChassisSpeedRef.rotate_ref		*0.075)*160;
	CMFR.TargetAngle = ( ChassisSpeedRef.forward_back_ref	*0.075 
						+ ChassisSpeedRef.left_right_ref	*0.075 
						+ ChassisSpeedRef.rotate_ref		*0.075)*160;
	CMBL.TargetAngle = (  ChassisSpeedRef.forward_back_ref	*0.075 
						- ChassisSpeedRef.left_right_ref	*0.075 
						+ ChassisSpeedRef.rotate_ref		*0.075)*160;
	CMBR.TargetAngle = ( -ChassisSpeedRef.forward_back_ref	*0.075 
						- ChassisSpeedRef.left_right_ref	*0.075 
						+ ChassisSpeedRef.rotate_ref		*0.075)*160;*/
}

//主控制循环
void controlLoop(uint16_t tim6_count)
{
	WorkStateFSM();
	
	if(WorkState > 0)
	{
		Chassis_Data_Decoding(tim6_count);
		
		#ifdef CAN11
		for(int i=0;i<4;i++) if(can1[i]!=0) (can1[i]->Handle)(can1[i]);
		setCAN11();
		#endif
		#ifdef CAN12
		for(int i=4;i<8;i++) if(can1[i]!=0) (can1[i]->Handle)(can1[i]);
		setCAN12();
		#endif
		#ifdef CAN21
		for(int i=0;i<4;i++) if(can2[i]!=0) (can2[i]->Handle)(can2[i]);
		setCAN21();
		#endif
		#ifdef CAN22
		for(int i=4;i<8;i++) if(can2[i]!=0) (can2[i]->Handle)(can2[i]);
		setCAN22();
		#endif
	}
}

//时间中断入口函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t tim6_count=0;
	
	if (htim->Instance == htim6.Instance)//1ms时钟`
	{
		HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
		
		tim6_count++;
		tim6_count %= 3335; //3335=2π/1.884*1000
		//主循环在时间中断中启动
		controlLoop(tim6_count);
		
		HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
	}
	else if (htim->Instance == htim7.Instance)//ims时钟
	{
		rc_cnt++;
		if(auto_counter > 0) auto_counter--;
		if (rc_update)
		{
			if( (rc_cnt <= 17) && (rc_first_frame == 1))
			{
				RemoteDataProcess(rc_data);				//遥控器数据解算
				HAL_UART_AbortReceive(&RC_UART);
				HAL_UART_Receive_DMA(&RC_UART, rc_data, 18);
				rc_cnt = 0;
			}
			else
			{
				if(rc_first_frame) 
					WorkState = PREPARE_STATE;
				HAL_UART_AbortReceive(&RC_UART);
				HAL_UART_Receive_DMA(&RC_UART, rc_data, 18);
				rc_cnt = 0;
				rc_first_frame = 1;
			}
			rc_update = 0;
		}
	}
	else if (htim->Instance == htim10.Instance)  //10ms，处理上位机数据，优先级不高
	{
		
		#ifdef DEBUG_MODE
		//zykProcessData();
		//dataCallBack();
		#endif
	}
}
