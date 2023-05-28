/*
 * Software_Parameter.h
 *
 *  Created on: Nov 13, 2021
 *      Author: mhn
 */

#ifndef SOFTWARE_PARAMETER_H_
#define SOFTWARE_PARAMETER_H_


//extern const uint8_t	Com1Porotocol; /*1=ModbusRTU 2=ModbusAscii  3=DNP3  4=IEC*/
//extern const uint8_t  Com1PorotocolMode;
//extern const uint32_t	Com1Baudrate;
//extern const uint8_t	Com1Databit;
//extern const uint8_t	Com1Pariety;    /*0=None 1=odd 2=even*/
//extern const uint8_t	Com1Stopbit;
//
//extern const uint8_t	Com2Porotocol;
//extern const uint8_t  Com2PorotocolMode;
//extern const uint32_t	Com2Baudrate;
//extern const uint8_t	Com2Databit;
//extern const uint8_t	Com2Pariety;    /*0=None 1=odd 2=even*/
//extern const uint8_t	Com2Stopbit;
//
//extern const uint8_t	Com3Porotocol;
//extern const uint8_t  Com3PorotocolMode;
//extern const uint32_t	Com3Baudrate;
//extern const uint8_t	Com3Databit;
//extern const uint8_t	Com3Pariety;    /*0=None 1=odd 2=even*/
//extern const uint8_t	Com3Stopbit;

//extern const uint8_t	Com4Porotocol;
//extern const uint8_t  Com4PorotocolMode;
//extern const uint32_t	Com4Baudrate;
//extern const uint8_t	Com4Databit;
//extern const uint8_t	Com4Pariety;    /*0=None 1=odd 2=even*/
//extern const uint8_t	Com4Stopbit;
//
//extern const uint8_t	PlcMbusMode;  /*Master 1 ; Slave 2*/
//extern const uint8_t	PlcMbusSlaveAddress;
//extern const uint8_t	PlcMbusMyAddress;
//extern const uint8_t	PlcDnp3SlaveAddress;
//extern const uint8_t	PlcDnp3Masteraddress;
//
//extern const uint8_t	ApplicationLayerSetupRtry;
//extern const uint8_t	ApplicationLayerSetupConfirm;
//extern const uint16_t	ApplicationLayerSetupCompleteFragTout;
//extern const uint8_t	ApplicationLayerSetupSelectArmTout;
//
//extern const uint8_t	DataLinkSetupRtry;
//extern const uint8_t	DataLinkConfirmMode;
//extern const uint8_t DataLinkSetupConfirmTout;


///*****************ETH Parameter*******************/
//extern unsigned char Physicaladdress[6];//const
////--------------------IPAddress--------------------
//extern const unsigned char PLCIpaddress[4];

////--------------------SubnetMask--------------------
//extern const unsigned char PLCSubnetmask[4];

////--------------------Gateway--------------------
//extern const unsigned char PLCGateway[4];

////--------------------PLCPort--------------------
//extern const int PlcEthport[5];

////--------------------Master1 IPAddress----------------------
//extern const unsigned char Master1Ipaddress[4];

////--------------------Master2 IPAddress----------------------
//extern const unsigned char Master2Ipaddress[4];

////--------------------Server Soucket Mode--------------------
//extern const int ServerSoucketMode[];/*1:TCP_MODE 0:UDP_MODE*/

////--------------------ServerIP--------------------
//extern const unsigned char ServerSoucketip[5][4];

////--------------------ServerPort--------------------
//extern const int ServerSoucketport[5];

////--------------------Client soucket Mode--------------------
//extern const unsigned char ClientSoucketMode[];/*1:TCP_MODE 0:UDP_MODE*/

//extern const uint8_t BkpArrange[18];



//Device Number%,Command|Device Address%,Start Address%,Num Register%.Pull Time%,...
//extern const uint16_t Device485_number[];//{0};
//extern const uint16_t Device485_Index[];
////Device Number%,Command|Device Address%,Start Address%,Num Register%.Pull Time%,...
//extern const int Device232_number[];//{0};
////--------------------------------------
//extern const int Device485_tag[];//{0};
////--------------------------------------
//extern const int Device232_tag[];//{0};



//{Board Number,Add ,Kind of card,Frist Point Add,...,Add ,Kind of card,Frist Point Add}
//extern const int Seting_di_table[];//{2,0,23,1,1,23,24};
////{Board Number,Add ,Kind of card,Frist Point Add,...,Add ,Kind of card,Frist Point Add}
//extern const int Seting_do_table[] ;// {1,1,7,0};
////{Board Number,Add ,Kind of card,Frist Point Add,...,Add ,Kind of card,Frist Point Add}
//extern const int Seting_ai_table[] ;// {2,1,0,1,2,0,51};

//extern const int Setting_dio_table[];


//--------------------SCADA--------------------
//Class0:
//---BinaryPoint Class0
extern const int Class0[];/*={ 1 , 0 , 55 , 0 , 0, 1 , 0 , 10,
//---AnalogPoint Class0
0, 8, 0 , 4 , 4 , 6, 10 , 4 , 14 , 3 , 17  , 4  , 21  , 6  , 27  , 4  , 31  , 6  , 0  , 0  , 0  , 1  , 0  , 20 ,
//---AnalogFloatingPoint Class0
0  , 0 ,
//---CounterPoint Class0
 0  , 1  , 0  , 15  , 0  , 0  , 0  , 0  , 0  , 0 };
*/
//Class1:
//---BinaryPoint Class1
extern const int Class1[];/*={ 1  , 0  , 55  , 0 ,
//---AnalogPoint Class1
  0 ,
//---AnalogFloatingPoint Class1
  0 ,
//---CounterPoint Class1
  0 };
 */
//Class2:
//---BinaryPoint Class2
extern  const int Class2[];/*={ 0 ,
//---AnalogPoint Class2
  4  , 0  , 4  , 10  , 10  , 4  , 10  , 17  , 4  , 10  , 27  , 4  , 10 ,
//---AnalogFloatingPoint Class2
  0 ,
//---CounterPoint Class2
  0 };
//Class3:
//---BinaryPoint Class3
*/
extern const int Class3[];/*={ 0 ,
//---AnalogPoint Class3
  0 ,
//---AnalogFloatingPoint Class3
  0 ,
//---CounterPoint Class3
  0 };
*/



	//---SCADAEvent
extern const int Unsolicitednotificationdelay[];//={1};

extern const int Binaryeventobject[];//={1};
extern const int Analogeventobject[];//={3};
extern const int Floateventobject[];//={0};
extern const int Converteventobject[];//={3};
extern const int Analogeventstoragemode[];//={0};
extern const int Class1minevents[];//={10};
extern const int Class2minevents[];//={10};
extern const int Class3minevents[];//={10};
extern const int Minunsoleventtxdelay[];//={1};
extern const int Appllayerconfirmeventtimeout[];//={2};
extern const int Defaultenableeventclasses[];//={0};





extern const char  *Database [];// =
/*
{
"F1G1V0Q60002-F1G1V0Q00002-F1G1V0Q10002-F1G1V1Q00001-F1G1V1Q10001-F1G1V2Q00101",
"F1G1V2Q10101-F1G2V0Q6//T01-F1G2V0Q7//T01-F1G2V0Q8//T01-F1G2V1Q6//T01-F1G2V1Q7//T01",
"F1G2V1Q8//T01-F1G2V2Q6//T02-F1G2V2Q7//T02-F1G2V2Q8//T02-F1G2V3Q6//-F1G2V3Q7//",
"F1G2V3Q8//-F1G10V0Q60301-F2G10V1Q00301-F2G10V1Q10301-F1G10V2Q00301-F1G10V2Q10301",
"F3G12V1Q23//N1213-F3G12V1Q40//N1313-F4G12V1Q23//N1213-F4G12V1Q40//N1313",
"F5G12V1Q23//N1213-F5G12V1Q40//N1313-F6G12V1Q23//N1213-F6G12V1Q40//N1313-F1G20V0Q61204",
"F1G20V0Q01204-F1G20V0Q11204-F7G20V0Q61204-F7G20V0Q01204-F7G20V0Q11204-F8G20V0Q61204",
"F8G20V0Q01204-F8G20V0Q11204-F9G20V0Q61204-F9G20V0Q01204-F9G20V0Q11204-",
"F10G20V0Q61204-F10G20V0Q01204-F10G20V0Q11204-F1G20V1Q01201-F1G20V1Q11201",
"F7G20V1Q01201-F7G20V1Q11201-F8G20V1Q01201-F8G20V1Q11201-F9G20V1Q01201-F9G20V1Q11201",
"F1G20V5Q01401-F1G20V5Q11401-F7G20V5Q01401-F10G20V1Q01201-F10G20V1Q11201-F1G20V2Q01301",
"F1G20V2Q11301-F7G20V2Q01301-F7G20V2Q11301-F8G20V2Q01301-F8G20V2Q11301-",
"F9G20V2Q01301-F9G20V2Q11301-F10G20V2Q01301-F10G20V2Q11301-F7G20V5Q11401",
"F8G20V5Q01401-F8G20V5Q11401-F9G20V5Q01401-F9G20V5Q11401-F10G20V5Q01401-F10G20V5Q11401",
"F1G20V6Q01501-F1G20V6Q11501-F7G20V6Q01501-F7G20V6Q11501-F8G20V6Q01501-F8G20V6Q11501",
"F9G20V6Q01501-F9G20V6Q11501-F10G20V6Q01501-F10G20V6Q11501-F1G21V0Q61604",
"F1G21V1Q01601-F1G21V1Q11601-F1G21V2Q01701-F1G21V2Q11701-F1G21V9Q01801-F1G21V9Q11801",
"F1G21V10Q01901-F1G21V10Q11901-F1G22V0Q6//T03-F1G22V0Q7//T03-F1G22V0Q8//T03",
"F1G22V5Q6//T10-F1G22V5Q7//T10-F1G22V5Q8//T10-F1G30V0Q60404-F1G30V1Q00401",
"F1G30V1Q10401-F1G30V2Q00501-F1G30V2Q10501-F1G30V3Q00601-F1G30V3Q10601-F1G30V4Q00701",
"F1G30V4Q10701-F1G30V5Q01001-F1G30V5Q11001-F1G32V0Q6//T02-F1G32V0Q7//T02",
"F1G32V0Q8//T02-F1G32V1Q6//T03-F1G32V1Q7//T03-F1G32V1Q8//T03-F1G32V2Q6//T04-F1G32V2Q7//T04",
"F1G32V2Q8//T04-F1G32V3Q6//T05-F1G32V3Q7//T05-F1G32V3Q8//T05-F1G32V4Q6//T06",
"F1G32V4Q7//T06-F1G32V4Q8//T06-F1G32V5Q6//T11-F1G32V5Q7//T11-F1G32V5Q8//T11-F1G40V0Q60802",
"F1G40V0Q00802-F1G40V0Q10802-F1G40V1Q00801-F1G40V1Q10801-F1G40V2Q00901-F1G40V2Q10901",
"F1G40V3Q01101-F3G41V1Q23//N0328-F3G41V1Q40//N0428-F4G41V1Q23//N0328",
"F4G41V1Q40//N0428-F5G41V1Q23//N0328-F5G41V1Q40//N0428-F6G41V1Q23//N0328",
"F6G41V1Q40//N0428-F3G41V2Q23//N0429-F3G41V2Q40//N0529-F4G41V2Q23//N0429-F4G41V2Q40//N0529-",
"F5G41V2Q23//N0429-F5G41V2Q40//N0529-F6G41V2Q23//N0429-F6G41V2Q40//N0529-F3G41V3Q23//",
"F3G41V3Q40//-F4G41V3Q23//-F4G41V3Q40//-F5G41V3Q23//-F5G41V3Q40//-F6G41V3Q23//-",
"F6G41V3Q40//-F1G50V1Q701//-F2G50V1Q701//-F1G60V1Q60019-F1G60V2Q6//C-F20G60V2Q6//",
"F21G60V2Q6//-F1G60V3Q6//C-F1G60V3Q7//C-F1G60V3Q8//C-F20G60V3Q6//-F20G60V3Q7//-",
"F20G60V3Q8//-F21G60V3Q6//-F21G60V3Q7//-F21G60V3Q8//-F1G60V4Q6//C-F1G60V4Q7//C",
"F1G60V4Q8//C-F20G60V4Q6//-F20G60V4Q7//-F20G60V4Q8//-F21G60V4Q6//-F21G60V4Q7//",
"F21G60V4Q8//-F1G80V1Q00707//-F2G80V1Q00707//-F1G30V0Q00404-F1G30V0Q10404",
};
*/

/*----------------- Table of CRC values for high-order byte -----------------------------------*/
extern  unsigned short crcLookUpTable[256]; // Look up table values
/*
{
0x0000, 0x365E, 0x6CBC, 0x5AE2, 0xD978, 0xEF26, 0xB5C4, 0x839A,
0xFF89, 0xC9D7, 0x9335, 0xA56B, 0x26F1, 0x10AF, 0x4A4D, 0x7C13,
0xB26B, 0x8435, 0xDED7, 0xE889, 0x6B13, 0x5D4D, 0x07AF, 0x31F1,
0x4DE2, 0x7BBC, 0x215E, 0x1700, 0x949A, 0xA2C4, 0xF826, 0xCE78,
0x29AF, 0x1FF1, 0x4513, 0x734D, 0xF0D7, 0xC689, 0x9C6B, 0xAA35,
0xD626, 0xE078, 0xBA9A, 0x8CC4, 0x0F5E, 0x3900, 0x63E2, 0x55BC,
0x9BC4, 0xAD9A, 0xF778, 0xC126, 0x42BC, 0x74E2, 0x2E00, 0x185E,
0x644D, 0x5213, 0x08F1, 0x3EAF, 0xBD35, 0x8B6B, 0xD189, 0xE7D7,
0x535E, 0x6500, 0x3FE2, 0x09BC, 0x8A26, 0xBC78, 0xE69A, 0xD0C4,
0xACD7, 0x9A89, 0xC06B, 0xF635, 0x75AF, 0x43F1, 0x1913, 0x2F4D,
0xE135, 0xD76B, 0x8D89, 0xBBD7, 0x384D, 0x0E13, 0x54F1, 0x62AF,
0x1EBC, 0x28E2, 0x7200, 0x445E, 0xC7C4, 0xF19A, 0xAB78, 0x9D26,
0x7AF1, 0x4CAF, 0x164D, 0x2013, 0xA389, 0x95D7, 0xCF35, 0xF96B,
0x8578, 0xB326, 0xE9C4, 0xDF9A, 0x5C00, 0x6A5E, 0x30BC, 0x06E2,
0xC89A, 0xFEC4, 0xA426, 0x9278, 0x11E2, 0x27BC, 0x7D5E, 0x4B00,
0x3713, 0x014D, 0x5BAF, 0x6DF1, 0xEE6B, 0xD835, 0x82D7, 0xB489,
0xA6BC, 0x90E2, 0xCA00, 0xFC5E, 0x7FC4, 0x499A, 0x1378, 0x2526,
0x5935, 0x6F6B, 0x3589, 0x03D7, 0x804D, 0xB613, 0xECF1, 0xDAAF,
0x14D7, 0x2289, 0x786B, 0x4E35, 0xCDAF, 0xFBF1, 0xA113, 0x974D,
0xEB5E, 0xDD00, 0x87E2, 0xB1BC, 0x3226, 0x0478, 0x5E9A, 0x68C4,
0x8F13, 0xB94D, 0xE3AF, 0xD5F1, 0x566B, 0x6035, 0x3AD7, 0x0C89,
0x709A, 0x46C4, 0x1C26, 0x2A78, 0xA9E2, 0x9FBC, 0xC55E, 0xF300,
0x3D78, 0x0B26, 0x51C4, 0x679A, 0xE400, 0xD25E, 0x88BC, 0xBEE2,
0xC2F1, 0xF4AF, 0xAE4D, 0x9813, 0x1B89, 0x2DD7, 0x7735, 0x416B,
0xF5E2, 0xC3BC, 0x995E, 0xAF00, 0x2C9A, 0x1AC4, 0x4026, 0x7678,
0x0A6B, 0x3C35, 0x66D7, 0x5089, 0xD313, 0xE54D, 0xBFAF, 0x89F1,
0x4789, 0x71D7, 0x2B35, 0x1D6B, 0x9EF1, 0xA8AF, 0xF24D, 0xC413,
0xB800, 0x8E5E, 0xD4BC, 0xE2E2, 0x6178, 0x5726, 0x0DC4, 0x3B9A,
0xDC4D, 0xEA13, 0xB0F1, 0x86AF, 0x0535, 0x336B, 0x6989, 0x5FD7,
0x23C4, 0x159A, 0x4F78, 0x7926, 0xFABC, 0xCCE2, 0x9600, 0xA05E,
0x6E26, 0x5878, 0x029A, 0x34C4, 0xB75E, 0x8100, 0xDBE2, 0xEDBC,
0x91AF, 0xA7F1, 0xFD13, 0xCB4D, 0x48D7, 0x7E89, 0x246B, 0x1235
};
*/


extern const  unsigned short  Memory_setting_card;
extern const  unsigned short  St_add_number_din;
extern const  unsigned short  St_add_number_dout;
extern const  unsigned short  St_add_number_dio;
extern const  unsigned short  St_add_number_ain;
extern const  unsigned short  St_add_number_aout;

extern const  unsigned short  Memory_setting_port;
extern const  unsigned short  St_add_config_com2_3;
extern const  unsigned short  St_add_config_com4_gprs;
extern const  unsigned short  St_add_num_d485;
extern const  unsigned short  St_add_num_d232;


//extern const unsigned short St_add_di_plc;
//extern const unsigned short St_add_ai_plc;
//extern const unsigned short St_add_do_plc;
//extern const unsigned short St_add_do_plc_index;
//extern const unsigned short St_add_soft_dio_plc;
//extern const unsigned short St_add_cnt_plc;
//extern const unsigned short St_add_freeze_cnt_plc;
//extern const unsigned short St_add_ao_plc;
//extern const unsigned short St_add_soft_aio_plc;
//extern const unsigned short Stop_index_dio_plc;
//extern const unsigned short Stop_index_do_plc;
//extern const unsigned short Stop_index_aio_plc;
//extern const unsigned short Stop_index_ao_plc;
//extern const unsigned short Stop_index_cnt_plc;
//extern const unsigned short Stop_index_freeze_cnt_plc;
//extern const unsigned short On;
//extern const unsigned short Off;
//extern const unsigned short Binary_chang_no_time;
//extern const unsigned short Binary_chang_with_time;
//extern const unsigned short a32_bit_analog_chang_no_time;
//extern const unsigned short a16_bit_analog_chang_no_time;
//extern const unsigned short a32_bit_analog_chang_with_time;
//extern const unsigned short a16_bit_analog_chang_with_time;
//extern const unsigned short a32_bit_counter_chang_no_time;
//extern const unsigned short a16_bit_counter_chang_no_time;
//extern const unsigned short a32_bit_counter_chang_with_time;
//extern const unsigned short a16_bit_counter_chang_with_time;
//extern const unsigned short Float_chang_no_time;
//extern const unsigned short Di_event_obj;
//extern const unsigned short Ai_event_obj;
//extern const unsigned short Cnt_event_obj;
//extern const unsigned short Float_event_obj;
//extern const unsigned short St_add_last_di_plc;
//extern const unsigned short St_add_last_ai_plc;
//extern const unsigned short St_add_last_cnt_plc;
//extern const unsigned short Event;
//extern const unsigned short Static;
//extern const unsigned short Outon;
//extern const unsigned short Outoff;





#endif /* SOFTWARE_PARAMETER_H_ */
