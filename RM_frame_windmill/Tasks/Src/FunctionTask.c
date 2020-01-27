/**
  ******************************************************************************
  * File Name          : FunctionTask.c
  * Description        : ���ڼ�¼�����˶��еĹ���
  ******************************************************************************
  *
  * Copyright (c) 2019 Team Jiao Long-Shanghai Jiao Tong University
  * All rights reserved.
  *
  ******************************************************************************
  */
#include "includes.h"

float rotate_speed = 0;
KeyboardMode_e KeyboardMode = NO_CHANGE;
RampGen_t LRSpeedRamp = RAMP_GEN_DAFAULT;
RampGen_t FBSpeedRamp = RAMP_GEN_DAFAULT;
ChassisSpeed_Ref_t ChassisSpeedRef; 

int32_t auto_counter=0;                                                                       //�Զ������������������ÿ�����Լ�1������������ʱ����Զ�������
                                                                                              //��������Ҳ������ControlTask.c��158�д��ҵ��Լ���ԭ�����ɴ�����������ʱ�����
int16_t channelrrow = 0;                                                                      //���ĸ��������ڴ洢ң�������صĲ�����
int16_t channelrcol = 0;                                                                      //��������Ϊ�������ֵ���������ף���600����      
int16_t channellrow = 0;
int16_t channellcol = 0;
uint32_t test=0;
//��ʼ��
void FunctionTaskInit()
{
	LRSpeedRamp.SetScale(&LRSpeedRamp, MOUSE_LR_RAMP_TICK_COUNT);
	FBSpeedRamp.SetScale(&FBSpeedRamp, MOUSR_FB_RAMP_TICK_COUNT);
	LRSpeedRamp.ResetCounter(&LRSpeedRamp);
	FBSpeedRamp.ResetCounter(&FBSpeedRamp);
	
	ChassisSpeedRef.forward_back_ref = 0.0f;
	ChassisSpeedRef.left_right_ref = 0.0f;
	ChassisSpeedRef.rotate_ref = 0.0f;
	
	KeyboardMode=NO_CHANGE;
}

void Limit_and_Synchronization()                                                             //�����������Ӳ�����Ƶ���Ƕȣ���ֹ���ת�������ʵ�λ��
{
	//demo
//	MINMAX(AnyMotor.TargetAngle,-900,270);//limit
	//demo end
}

//******************
//ң����ģʽ���ܱ�д
//******************
void RemoteControlProcess(Remote *rc)
{
	if(WorkState <= 0) return;
	//max=297
	channelrrow = 116;//(rc->ch0 - (int16_t)REMOTE_CONTROLLER_STICK_OFFSET); 
	channelrcol = 116;//(rc->ch1 - (int16_t)REMOTE_CONTROLLER_STICK_OFFSET); 
	channellrow = 116;//(rc->ch2 - (int16_t)REMOTE_CONTROLLER_STICK_OFFSET); 
	channellcol = 116;//(rc->ch3 - (int16_t)REMOTE_CONTROLLER_STICK_OFFSET); 
////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////��Ҫ�����������д
	
	
	if(WorkState == NORMAL_STATE)                                                            //�ϵ�
	{	
		ChassisSpeedRef.forward_back_ref = channelrcol * RC_CHASSIS_SPEED_REF;               //�����Ѿ�Ĭ��д���˵��̵Ŀ��ƺ��� 
		ChassisSpeedRef.left_right_ref   = -channelrrow * RC_CHASSIS_SPEED_REF/2;             //�ұ�ҡ�˿���ǰ�����ҵ�ƽ�� ���ҡ�˿�����ת
		rotate_speed = -channellrow * RC_ROTATE_SPEED_REF;                                   //RC_CHASSIS_SPEED_REF��һ��Ĭ�ϵ���ֵ���������н��ٶȴﵽ����ֵ
		                                                                                     //ChassisSpeedRef.forward_back_ref��һ����װ�õı�����ͨ���ı���
		                                                                                     //����ֱ�ӿ��Ƴ���ǰ�������ٶȣ���������ͬ��
		                                                                                     //�����Ҫ��߳��٣�ֻ��Ҫ�ڵȺ��Ҳ����һ��ϵ������
		                                                                                     //��Ȼ�ٶȵ�����������޵ģ����������Ҫ���е���
		                                                                                        
		//demo
		M2006.TargetAngle += -600 * 0.02;                                             //����2006��������������ת�������еģ�������������������ϵ���
		//demo end
		if(channellcol>200)
		{
		       			    //W  PUL   PI5 C1 
					//       		HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);                          //X  DIR       PI6 C2          direction
            //  	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 10000);      //Y  PUL          PI7 C3
		//	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);                                         //Z  DIR        PI2 C4
       //       	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 10000);	        //w pul PI5 C1  
					HAL_GPIO_WritePin(GPIOI,GPIO_PIN_6,0);
					HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,0);
			HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1); 
              	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 5000);
					       		HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);                        //C1 C3 move   C2 C4 direction
              	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 5000);
                                        //motor up
		}
		if(channellcol<-200)
		{
			
	//				       		HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);         
   //           	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 5000);
		//	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3); 
    //          	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 5000);	
			HAL_GPIO_WritePin(GPIOI,GPIO_PIN_6,1);
					HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,1);
			HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1); 
              	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 5000);	
					       		HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3); 
              	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 5000);
			                                                       //motor down
		}
		if(channellcol>-200&&channellcol<200)
		{
			HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1); 
              	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 0);	
					       		HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3); 
              	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 0);
		}
	}                                                                                          
	if(WorkState == ADDITIONAL_STATE_ONE)                                                   //�е�
	{
		
		HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);   
               		                              //���������������PWM��
	  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 500+1050*channellcol/600);             //���������������ռ�ձȣ�cube���Ѿ����ú�������
		                                                                                      //Ϊ20ms������һ�£���ʱ��,ֻ���޸ĵ�����������
		                                                                                      //��Ӧ��ϵΪ��0��Ӧ0,20000��Ӧ20ms��1500��Ӧ1.5ms��
	                                                                                        //��֪���������0.5-2.5ms���źţ�����1500�൱������ת90�㣬
		                                                                                      //500��0�㣬2500�൱��ת180��
	}
	if(WorkState == ADDITIONAL_STATE_TWO)                                                   //�µ�
	{
			M2006.TargetAngle += 300 * 0.02;   
	}
	
	
////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
	Limit_and_Synchronization();
}


uint16_t KM_FORWORD_BACK_SPEED 	= NORMAL_FORWARD_BACK_SPEED;
uint16_t KM_LEFT_RIGHT_SPEED  	= NORMAL_LEFT_RIGHT_SPEED;
void KeyboardModeFSM(Key *key);

//****************
//����ģʽ���ܱ�д  У�����в����õ�����
//****************
void MouseKeyControlProcess(Mouse *mouse, Key *key)
{	
	if(WorkState <= 0) return;
	
	MINMAX(mouse->x, -150, 150); 
	MINMAX(mouse->y, -150, 150); 

	KeyboardModeFSM(key);
	
	switch (KeyboardMode)
	{
		case SHIFT_CTRL:		//State control
		{
			
			break;
		}
		case CTRL:				//slow
		{
			
		}//DO NOT NEED TO BREAK
		case SHIFT:				//quick
		{
			
		}//DO NOT NEED TO BREAK
		case NO_CHANGE:			//normal
		{//CM Movement Process
			if(key->v & KEY_W)  		//key: w
				ChassisSpeedRef.forward_back_ref =  KM_FORWORD_BACK_SPEED* FBSpeedRamp.Calc(&FBSpeedRamp);
			else if(key->v & KEY_S) 	//key: s
				ChassisSpeedRef.forward_back_ref = -KM_FORWORD_BACK_SPEED* FBSpeedRamp.Calc(&FBSpeedRamp);
			else
			{
				ChassisSpeedRef.forward_back_ref = 0;
				FBSpeedRamp.ResetCounter(&FBSpeedRamp);
			}
			if(key->v & KEY_D)  		//key: d
				ChassisSpeedRef.left_right_ref =  KM_LEFT_RIGHT_SPEED * LRSpeedRamp.Calc(&LRSpeedRamp);
			else if(key->v & KEY_A) 	//key: a
				ChassisSpeedRef.left_right_ref = -KM_LEFT_RIGHT_SPEED * LRSpeedRamp.Calc(&LRSpeedRamp);
			else
			{
				ChassisSpeedRef.left_right_ref = 0;
				LRSpeedRamp.ResetCounter(&LRSpeedRamp);
			}
		}
	}
	Limit_and_Synchronization();
}

void KeyboardModeFSM(Key *key)
{
	if((key->v & 0x30) == 0x30)//Shift_Ctrl
	{
		KM_FORWORD_BACK_SPEED=  LOW_FORWARD_BACK_SPEED;
		KM_LEFT_RIGHT_SPEED = LOW_LEFT_RIGHT_SPEED;
		KeyboardMode=SHIFT_CTRL;
	}
	else if(key->v & KEY_SHIFT)//Shift
	{
		KM_FORWORD_BACK_SPEED=  HIGH_FORWARD_BACK_SPEED;
		KM_LEFT_RIGHT_SPEED = HIGH_LEFT_RIGHT_SPEED;
		KeyboardMode=SHIFT;
	}
	else if(key->v & KEY_CTRL)//Ctrl
	{
		KM_FORWORD_BACK_SPEED=  LOW_FORWARD_BACK_SPEED;
		KM_LEFT_RIGHT_SPEED = LOW_LEFT_RIGHT_SPEED;
		KeyboardMode=CTRL;
	}
	else
	{
		KM_FORWORD_BACK_SPEED=  NORMAL_FORWARD_BACK_SPEED;
		KM_LEFT_RIGHT_SPEED = NORMAL_LEFT_RIGHT_SPEED;
		KeyboardMode=NO_CHANGE;
	}	
}
