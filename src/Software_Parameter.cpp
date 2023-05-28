/*
 * Software_Parameter.cpp
 *
 *  Created on: Nov 13, 2021
 *      Author: mhn
 */


#include "main.h"


	//--------------------SCADA--------------------
	//Class0:
	//---BinaryPoint Class0
	const int Class0[]={ 1 , 0 , 4 , 0 , 0 ,  1 , 0, 2 ,
	//---AnalogPoint Class0
   0 ,0,0,0,0,0,0,0,0,0,
	//---AnalogFloatingPoint Class0
	0  , 0 ,
	//---CounterPoint Class0
	 0  , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
	//Class1:
	//---BinaryPoint Class1
	const int Class1[]={ 1  , 0  , 4  , 0 ,
	//---AnalogPoint Class1
		0 ,
	//---AnalogFloatingPoint Class1
		0 ,
	//---CounterPoint Class1
		0 };
	//Class2:
	//---BinaryPoint Class2
	const int Class2[]={ 0 ,
	//---AnalogPoint Class2
	//  17 , 0 , 3 , 2 , 3 , 3 , 5 , 6 , 3 , 2 , 9  , 3 , 5 , 12 , 3
	//	, 2 , 15 , 3 ,5, 18, 3 , 2 , 21 , 3 , 5 , 24 , 3 , 2 , 27 , 3
	//	, 5 , 30 , 3 , 2 , 33 , 3 , 5 , 36 , 3 , 2 , 39 , 3 , 5 , 42 , 3 , 2 , 45 , 9 , 50 , 54 , 1 , 2 ,
		0,0,0,0,
	//---AnalogFloatingPoint Class2
		0 ,
	//---CounterPoint Class2
		0 };
	//Class3:
	//---BinaryPoint Class3
	const int Class3[]={ 0,
	//---AnalogPoint Class3
		0 ,
	//---AnalogFloatingPoint Class3
		0 ,
	//---CounterPoint Class3
		0 };

//#endif






//---SCADAEvent
const int Unsolicitednotificationdelay[]={1};

const int Binaryeventobject[]={1};
const int Analogeventobject[]={3};
const int Floateventobject[]={0};
const int Converteventobject[]={3};
const int Analogeventstoragemode[]={0};
const int Class1minevents[]={10};
const int Class2minevents[]={10};
const int Class3minevents[]={10};
const int Minunsoleventtxdelay[]={1};
const int Appllayerconfirmeventtimeout[]={2};
const int Defaultenableeventclasses[]={0};


const unsigned short  Memory_setting_card = 1;
const unsigned short  St_add_number_din = Memory_setting_card;
const unsigned short  St_add_number_dout = Memory_setting_card + 9;          //20
const unsigned short  St_add_number_dio = Memory_setting_card + 18;          //37
const unsigned short  St_add_number_ain = Memory_setting_card + 27;          //70
const unsigned short  St_add_number_aout = Memory_setting_card + 34;         //32         '77

const unsigned short  Memory_setting_port = 278;                              //38
const unsigned short  St_add_config_com2_3 = Memory_setting_port;
const unsigned short  St_add_config_com4_gprs = Memory_setting_port + 1;
const unsigned short  St_add_num_d485 = Memory_setting_port + 2 ;            //85
const unsigned short  St_add_num_d232 = Memory_setting_port + 131;            //150





void idle(void)
{

}
