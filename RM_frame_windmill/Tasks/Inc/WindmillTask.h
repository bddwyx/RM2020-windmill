/**
 ***********************************(C) COPYRIGHT 2020 JiaoDragon************************************
 * @file       WindmillTask.h
 * @brief      Tasks for windmill
 * @note       
 * @Version    V1.0.0
 * @Date       July-16-2020      
 ***********************************(C) COPYRIGHT 2020 JiaoDragon************************************
 */

#ifndef __WINDMILLTASK_H
#define __WINDMILLTASK_H
 
	#include <stdint.h>
	#include "usart.h"
	
	/***	MAP	***/

	/***	CONSTANT VALUE	***/

	/***	TYPE DEFINE	***/
	typedef enum{
		LED_RED = 0,
		LED_BLUE,
	} E_millColor;
	
	typedef enum{
		PREPARE = 0,
		WAITING,
		FAIL,
		COMPLETE,
	} E_millState;
	
	struct T_blade{
		UART_HandleTypeDef* huart;
		
	};
	
	struct T_mill{
		struct T_blade* blade_Info;
		uint8_t blade_num;
		uint8_t blade_index;
		E_millColor e_color;
		E_millState e_state;
	};
	
	/***	EXTERNAL VARIABLES	***/
	
	/***	APIs	***/
	HAL_StatusTypeDef windmill_init(void);
	void windmill_action(UART_HandleTypeDef* huart);
	
	/***	SPECIFIC INIT CONFIGURATION	***/
	
#endif //__WINDMILLTASK_H
