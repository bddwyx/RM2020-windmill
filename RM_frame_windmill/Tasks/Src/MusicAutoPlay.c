/**
 ***********************************(C) COPYRIGHT 2020 JiaoDragon************************************
 * @file       MusicAutoPlay.c
 * @brief      I'm insane 
 * @note       
 * @Version    V1.0.0
 * @Date       July-13-2020      
 ***********************************(C) COPYRIGHT 2020 JiaoDragon************************************
 */
 
#include "includes.h"
#include "Lib_songs.h"

uint8_t Music_init(soft_timer_s* timer_1ms, T_Song* t_song){
	if(t_song->first_enter){
		soft_timer_init(timer_1ms, 1);
		t_song->first_enter = 0;
	}	//Init soft_timer
	
	HAL_TIM_PWM_Start(&BUZZER_TIM, TIM_CHANNEL_1);
	
	return soft_timer_add_task(timer_1ms, &Music_auto_play, (void*)(&Astronomia), 1, (task_s**)(0));
}

void Music_deinit(task_s* auto_play_task, T_Song* t_song){
	t_song->note_index = 0;
	t_song->first_enter = 1;
	
	HAL_TIM_PWM_Stop(&BUZZER_TIM, TIM_CHANNEL_1);
	soft_timer_delete_task(auto_play_task);
}

void Music_auto_play(task_s* auto_play_task, void* args){
	static uint16_t note_interval = 0;
	T_Song* t_song=(T_Song*)args;
	
	if(0 == note_interval){
		uint16_t note = t_song->t_script[t_song->note_index].Note;
		note_interval = t_song->t_script[t_song->note_index].Time;
		
		if(0 == note){
			__HAL_TIM_SET_AUTORELOAD(&BUZZER_TIM, 0);
		} else {
			__HAL_TIM_SET_AUTORELOAD(&BUZZER_TIM, 1000000 / note);
			__HAL_TIM_SET_COMPARE(&BUZZER_TIM, TIM_CHANNEL_1, 500000 / note);
		}
		
		if(++(t_song->note_index) >= t_song->note_num) Music_deinit(auto_play_task, t_song);
	} else {
		note_interval--;
		if(t_song->note_index == 20) {
			note_interval--;
		}
	}
	
}
