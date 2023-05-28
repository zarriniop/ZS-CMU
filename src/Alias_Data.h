/*
 * Alias_Data.h
 *
 *  Created on: Nov 13, 2021
 *      Author: mhn
 */



#ifndef ALIAS_DATA_H_
#define ALIAS_DATA_H_


//-------------------- Digital_Input : Total = 14----------------------------------


//point 0 DNP3
#define IN1_Status                  DI->BIT080
#define IN2_Status                  DI->BIT081
#define IN3_Status				    DI->BIT082
#define IN4_Status  			    DI->BIT083
//-------------------- Memory_Word : Total = 388
#define _B1currenta     							Main_buffer[2000]
#define _B1currentb     							Main_buffer[2001]
//------------------------------------------------------------------------
#define _DL_Retry     								Main_buffer[2431]
#define _DL_Timeout     							Main_buffer[2432]
#define _DL_Confirm     							Main_buffer[2433]
#define _AL_Retry     								Main_buffer[2434]
#define _AL_Timeout     							Main_buffer[2435]
#define _InitialUnsol     						Main_buffer[2436]
#define _Unsoldelay     							Main_buffer[2437]
#define _Sbotimeout     							Main_buffer[2438]
//#define _Unsoladdress     						Main_buffer[2439]
#define _Frameintv     								Main_buffer[2440]
//#define _Slaveaddress     						Main_buffer[2441]
#define _Comm_Speed     							Main_buffer[2442]
#define _Test_Battery     						Main_buffer[2443]
#define _Last_Data     								Main_buffer[2444]
#define _Reset_Factory     						Main_buffer[2445]
#define _Active_timer10ms     				Main_buffer[2446]
#define _Modem_Led     								Main_buffer[2447]
#define _Password    									Main_buffer[2448]
#define _Phase_Delaytime     					Main_buffer[2449]
#define _Phasefunctioninuse     			Main_buffer[2450]
#define _Unsolclass1     							Main_buffer[2451]
#define _Unsolclass2     							Main_buffer[2452]
#define _Unsolclass3     							Main_buffer[2453]
#define	_BaseTime1										Main_buffer[2454]
#define	_BaseTime2										Main_buffer[2455]
#define	_BaseTime3										Main_buffer[2456]
#define	_BaseTime4										Main_buffer[2457]
#define	_TimeDateSet									Main_buffer[2458]
/*---------------Digital Output----------------------------------------*/
#define _B1_Keyclose     							Main_buffer[2800]
#define _B1_Keyopen     							Main_buffer[2801]
/*----------------------------------------------------------------------*/
//#define _Sysbatterytest     					Main_buffer[2900]
#define _Faultindicator_Reset     		Main_buffer[2901]
//-----------------------------------------------------------------------
#define 	Timer_Count				24
#define _Timertestbattry   			7
#define _Timeraccharge   			8
#define _Timerbreaker4open   		9
#define _Timerbreaker4close   		10
#define _Timerbreaker6close   		16
#define _Timerbatterytest   		0
#define _Timerbreaker2open   		3
#define _Timerbreaker2close   		4
#define _Timerbreaker3open   		5
#define _Timerbreaker3close   		6
#define _Timerbreaker1open   		1
#define _Timerbreaker1close   		2
#define _Timer_open_linedetect      11
#define _Timer_groundfault   		12
#define _Timer300ms   				13
#define _Timerbreaker6open   		17
#define _Timerbreaker5open          14
#define _Timerbreaker7open          18
#define _Timerbreaker8open          20
#define _Timerbreaker5close         15
#define _Timerbreaker8close         21
#define _Timerbreaker7close         19
#define _Timerbreaker9open          22
#define _Timerbreaker9close         23
//---------------------------------------------------------------
#define St_add_di_plc  185
#define St_add_ai_plc  2000
#define St_add_do_plc  2800//340
#define St_add_do_plc_index    2900//340
#define St_add_soft_dio_plc    4000
#define St_add_cnt_plc         3000
#define St_add_freeze_cnt_plc  1600
#define St_add_ao_plc        2500
#define St_add_soft_aio_plc  2000 //196 //2000
#define Stop_index_dio_plc   1255
#define Stop_index_do_plc    255
#define Stop_index_aio_plc   3255
#define Stop_index_ao_plc    255
#define Stop_index_cnt_plc   99
#define Stop_index_freeze_cnt_plc  99
#define On  1
#define Off  0
#define Binary_chang_no_time  1
#define Binary_chang_with_time  2
#define a32_bit_analog_chang_no_time  3
#define a16_bit_analog_chang_no_time  4
#define a32_bit_analog_chang_with_time  5
#define a16_bit_analog_chang_with_time  6
#define a32_bit_counter_chang_no_time  7
#define a16_bit_counter_chang_no_time  8
#define a32_bit_counter_chang_with_time  9
#define a16_bit_counter_chang_with_time  10
#define Float_chang_no_time  11
#define Di_event_obj  1
#define Ai_event_obj  2
#define Cnt_event_obj  3
#define Float_event_obj 4
#define St_add_last_di_plc  1
#define St_add_last_ai_plc  80 //196
#define St_add_last_cnt_plc  3150
#define Event  1
#define Static  2
//-----------------------------------------------------------------
#define TripOutOn  100
#define CloseOutoff  200
#define Outon  214
#define Outoff  150












#endif /* ALIAS_DATA_H_ */
