/*
 * gpio.cpp
 *
 *  Created on: Nov 21, 2021
 *      Author: mhn
 */
#include "main.h"
#include "gpio.h"
#include "tools.h"
#include "DNP3.h"
#include "math.h"


extern GPIO gpio;
extern  uint8_t*  gpio_addr ;
extern SerialPort Dnp_Buffer;

uint8_t Inputs[4]={0,0,0,0};
uint8_t Outputs[2]={0,0};

extern uint16_t _Unsoladdress;
extern uint16_t _Slaveaddress;

uint32_t CountRTC;
WORD Main_buffer[5000]; //sajedi cpu
Bit *DI=(Bit *)&Main_buffer[180];
Bit *DO=(Bit *)&Main_buffer[120];
uint64_t Base_time;
BYTE Get_crc[640],Modem_rcv1[300];
extern bool _DnpSetTime;
bool RTCUpdateFlag,RTC_Read_Flag,SMS_SEND_COMPELETE = 0;
extern WORD Iin;
char Second,Month,Date;
extern WORD Plc_Address,Unsol_Address,D_confirm,D_retry,D_ack_time_out,A_retry,
	A_ack_time_out,Unsol_delay,Initial_unsol,Sbo_time_out,Frame_intv;
extern BYTE Enable_event_hmi[14];
#define Const_time1 4294967
#define Const_time2 296
const unsigned int  g_days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const unsigned int  j_days_in_month[12] = {31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};


//----------- Set Time
uint16_t Myear,Hyear;
uint8_t 	Mmonth,Mday,Hmonth,Hday,Hour,Minute;
extern WORD Array_len1;
extern uint8_t Dnp3_send_Flag1;

extern  WORD D_confirm;
extern  WORD D_retry;
extern  WORD D_ack_time_out;
extern  WORD A_retry;
extern  WORD A_ack_time_out;
extern  WORD Unsol_delay;
extern  WORD Initial_unsol;
extern  WORD Sbo_time_out;
extern  WORD Frame_intv;
extern  BYTE Enable_event_hmi[];
extern  uint8_t Dnp3_send_Flag ;
extern  BYTE  Modem_rcv[];
extern  BYTE Rcv_flag_dnp;




void *GPIO_Referesh(void *arg)
{
	char str[10];

	report("s","GPIO --> REFERESH_THREAD_STARTED !");

	char out1=0,out2=0;

	while(1)
	{
		if(Inputs[0]) exec2("uci set inout.input.input1=1");
		else 		  exec2("uci set inout.input.input1=0");

		if(Inputs[1]) exec2("uci set inout.input.input2=1");
		else 		  exec2("uci set inout.input.input2=0");

		if(Inputs[2]) exec2("uci set inout.input.input3=1");
		else 		  exec2("uci set inout.input.input3=0");

		if(Inputs[3]) exec2("uci set inout.input.input4=1");
		else  		  exec2("uci set inout.input.input4=0");

		exec2("uci commit inout");

		exec("uci get inout.output.relay1",str,1);
		if(str[0]=='1' && out1==0)
		{
			gpio.Write(GPIO_OUT_RELAY1, 1);
			out1=1;
		}
		else if(str[0]=='0' && out1==1)
		{
			gpio.Write(GPIO_OUT_RELAY1, 0);
			out1=0;
		}

		exec("uci get inout.output.relay2",str,1);
		if(str[0]=='1' && out2==0)
		{
			gpio.Write(GPIO_OUT_RELAY2, 1);
			out2=1;
		}
		else if(str[0]=='0' && out2==1)
		{
			gpio.Write(GPIO_OUT_RELAY2, 0);
			out2=0;
		}

		sleep(1);

	}


}



inline void DNP_Proccess()
{
	for ( uint8_t S = 0 ; S < Dnp_Buffer.RX_Count ; S++)
	{
		Sub_rcv_Dnp3(Dnp_Buffer.RX_Buffer[S] ,0 );
	}
	 Dnp_Buffer.RX_Count=0;
}


inline void GPIO_Read()
{
	Main_buffer[185]=0;

	if(gpio.Read(GPIO_IN1) == Inputs[0]) 	Main_buffer[185]=Inputs[0];
	else	Inputs[0]=gpio.Read(GPIO_IN1);

	if(gpio.Read(GPIO_IN2) == Inputs[1]) 	Main_buffer[185]+= Inputs[1] ? 0b0010 : 0;
	else	Inputs[1]=gpio.Read(GPIO_IN2);

	if(gpio.Read(GPIO_IN3) == Inputs[2]) 	Main_buffer[185]+=Inputs[2]  ? 0b0100 : 0;
	else	Inputs[2]=gpio.Read(GPIO_IN3);

	if(gpio.Read(GPIO_IN4) == Inputs[3]) 	Main_buffer[185]+=Inputs[3]  ? 0b1000 : 0;
	else	Inputs[3]=gpio.Read(GPIO_IN4);

}


inline void GPIO_Write()
{
	//printf("2900=%d\n2901=%d\n\n",Main_buffer[2900],Main_buffer[2901]);

	if(Main_buffer[2900]==CloseOutoff && Outputs[0]!=CloseOutoff)
	{
		gpio.Write(GPIO_OUT_RELAY1, 1);
		Outputs[0]=CloseOutoff;
	}
	else if(Main_buffer[2900]==Outoff && Outputs[0]!=Outoff)
	{
		gpio.Write(GPIO_OUT_RELAY1, 0);
		Outputs[0]=Outoff;
	}

	if(Main_buffer[2901]==CloseOutoff && Outputs[1]!=CloseOutoff )
	{
		gpio.Write(GPIO_OUT_RELAY2, 1);
		Outputs[1]=CloseOutoff;

	}
	else if(Main_buffer[2901]==Outoff && Outputs[1]!=Outoff)
	{
		gpio.Write(GPIO_OUT_RELAY2, 0);
		Outputs[1]=Outoff;
	}

}


void *DNP_Thread(void *arg)
{
	report("s","GPIO --> DNP3.0_THREAD_STARTED !");

 	statrup_dnp3();
	Main_buffer[2433] = 3;
	D_confirm = 3;
	D_retry =2;
	D_ack_time_out =5;
	A_retry=2;
	A_ack_time_out=5;
	Unsol_delay=4;  //sec
	Initial_unsol=1;  //yes
	Sbo_time_out=15;  //sec
	Frame_intv=700;  //ms
	Enable_event_hmi[1]=1;
	Enable_event_hmi[2]=1;
	Enable_event_hmi[3]=1;
	memset(Get_crc,0,639);

	Dnp_Buffer.TX_Count=0;
	Dnp_Buffer.RX_Count=0;

	Main_buffer[185]=0xFFFF;

	Outputs[0]=Main_buffer[2900];
	Outputs[1]=Main_buffer[2901];


	char str[20];

	memset(str,0,sizeof(str));
	exec(UCI_GET_DNPSRC, str, 1);
	_Unsoladdress=stoi(str);


	memset(str,0,sizeof(str));
	exec(UCI_GET_DNPDIST, str, 1);
	_Slaveaddress=stoi(str);




	struct timeval  tv;
	char cnt=0;

	int diff=0;
	uint32_t start=*(volatile uint32_t *)(gpio.BASE_ADDR + 0x118);

	//Enable Timer0 of MT7628 with 1ms priod (refer to MT7628 datasheet page 67)
	system("devmem 0x10000110 32 0x3e80090\0");

	//Set Timer0 Limit Value to 65535   (refer to MT7628 datasheet page 68)
	system("devmem 0x10000114 32 0xffff\0");


	while(1)
	{

		GPIO_Read();
		GPIO_Write();

		Dnp3_run();
		READ_BUFF(1);
		Dnp3_systick();
		//TimeDate();

		if(Dnp_Buffer.RX_Count>0)	DNP_Proccess();




		diff=start - TIMER0_VALUE ;
		start=TIMER0_VALUE;

		if(diff<0) diff+=0xFFFF;

		Base_time+=diff;


		std::this_thread::sleep_for(std::chrono::milliseconds(1));

	}
}



