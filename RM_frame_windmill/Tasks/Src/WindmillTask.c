/**
 ***********************************(C) COPYRIGHT 2020 JiaoDragon************************************
 * @file       WindmillTask.c
 * @brief      Tasks for windmill
 * @note       
 * @Version    V1.0.0
 * @Date       July-16-2020      
 ***********************************(C) COPYRIGHT 2020 JiaoDragon************************************
 */

#include "includes.h"

struct T_blade blades[] =  {
	{&huart3, },
	{&huart6, },
	{&huart7, },
	{&huart8, },
};

struct T_blade* blades_random_list[4] = {blades, blades+1, blades+2, blades+3};

struct T_mill mill = {
	blades, (sizeof(blades)/sizeof(struct T_blade)), 0, LED_RED, PREPARE
};

uint8_t current_blade = 0;
uint8_t rec_buf;

void random_blade_generate(){
	
}

HAL_StatusTypeDef windmill_init(){
	for(current_blade = 0; current_blade < mill.blade_num; current_blade++) HAL_UART_Receive_IT(mill.blade_Info[current_blade].huart, &rec_buf, 1);
	for(current_blade = 0; current_blade < mill.blade_num; current_blade++){
		if(HAL_OK != HAL_UART_Transmit_IT(mill.blade_Info[current_blade].huart, "0", 1)) return HAL_ERROR;
	}
	
	current_blade = 0;
	mill.e_state = PREPARE;
	
	random_blade_generate();
	
	windmill_action((UART_HandleTypeDef*)0);
	
	return HAL_OK;
}

uint8_t blade_search(UART_HandleTypeDef* huart){
	for(int i = 0; i < mill.blade_num; i++){
		if(mill.blade_Info[i].huart == huart) return i;
	}
	
	return 100;
}

void windmill_action(UART_HandleTypeDef* huart){
	if(huart != (UART_HandleTypeDef*)0){
		HAL_UART_Receive_IT(huart, &rec_buf, 1);	//Renable UART receive
	}
	
	switch(mill.e_state){
		case PREPARE:
			HAL_UART_Transmit_IT(blades_random_list[0]->huart, "1", 1);
			mill.e_state = WAITING;
			break;
		
		case WAITING:
			if(huart == blades_random_list[current_blade]->huart){	//Received successfully
				/***	Next blade	***/
				current_blade++;
				
				if(current_blade >= mill.blade_num){	//Finished all blades
					mill.e_state = COMPLETE;
					windmill_action((UART_HandleTypeDef*)0);
					current_blade = 0;
				}
				
				else {
					HAL_UART_Transmit_IT(blades_random_list[current_blade]->huart, "1", 1);
				}
			} 
			
			else {	//Failed
				/***	Renable windmill	***/
				mill.e_state = FAIL;
				current_blade = 0;
				windmill_action((UART_HandleTypeDef*)0);
			}
			
			break;
			
		case FAIL:
			for(current_blade = 0; current_blade < mill.blade_num; current_blade++){
				HAL_UART_Transmit_IT(mill.blade_Info[current_blade].huart, "0", 1);
			}
			
			random_blade_generate();
			mill.e_state = PREPARE;
			
			break;
		
		case COMPLETE:
			for(current_blade = 0; current_blade < mill.blade_num; current_blade++){
				HAL_UART_Transmit_IT(mill.blade_Info[current_blade].huart, "3", 1);
			}
			
			break;
		
		default: break;
	}
}
