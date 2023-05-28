/*
 * DNP3.cpp
 *
 *  Created on: Nov 13, 2021
 *      Author: mhn
 */


#include "main.h"



#include "math_lib.h"

//#include "integer.h"
#include "Software_Parameter.h"
#include "Alias_Data.h"
#include "DNP3.h"

//#include "mxconstants.h"

extern SerialPort Dnp_Buffer;

uint32_t _Unsoladdress  = 20;
uint32_t _Slaveaddress  = 1;
bool Rcv_dnptcpip_flag,reqDataDnp3;

uint8_t *Dnp_rcv_eth,*Dnp_Send_eth,Send_dnpport = Com_Serial;
extern const int Class0[];
extern const int Class1[];
extern const int Class2[];
extern const int Class3[];
WORD LastAiSetting;
extern WORD Main_buffer[5000];
uint8_t write_ok;
const char  *Database [] =
{
"F1G1V0Q60002-F1G1V0Q00002-F1G1V0Q10002-F1G1V1Q00001-F1G1V1Q10001-F1G1V1Q60001-F1G1V2Q00101",
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
"F21G60V4Q8//-F1G80V1Q00707//-F2G80V1Q00707//-F1G30V0Q00404-F1G30V0Q10404",
};


/*----------------- Table of CRC values for high-order byte -----------------------------------*/
 unsigned short crcLookUpTable[256] = // Look up table values   ///changed
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





//extern const  WORD 	_Local;   //sajedi cpu
//extern const  WORD 	Remote;
 BYTE Accept_flag,Rcv_dnp_port;
 BYTE Ok_flag;
 BYTE Event_send_flag;
 BYTE D_rcv_flag;
 BYTE A_rcv_flag;
 BYTE A_time_out_flag;
 BYTE D_send_flag;
 BYTE A_send_flag;
 BYTE Rcv_flag_dnp;
 BYTE Error_recive;
 BYTE Send_reset_link;
 BYTE Send_unsol_flag;
 BYTE Wait_unsol_flag;
 BYTE Last_event_time;
 BYTE D_time_out_flag;
 BYTE Need_send_confirm;
 BYTE Fcb;
 BYTE Unsol_response_flag;
 BYTE Need_ack;
 BYTE Chang_flag;
 extern bool Startup_PLC;
 BYTE Send_first_fragment;
 BYTE Calc_crc_flag;
 BYTE Sbo_delay_time;
 BYTE Sbo_time_out_flag;
 BYTE Test_flag;
 BYTE Check_flag;
 BYTE Read_buff_flag;
 BYTE Unsol_add_flag;
 BYTE Frame_intv_flag;
 BYTE Reset_flag;
 BYTE Fill_e2prom_event_flag;
 BYTE Error_control_relay_request_flag;
 BYTE Request_event_flag;
 BYTE Send_application_response_flag;
 BYTE Read_binary_output_flag;
 BYTE ab_flag;
 BYTE Reset_di_cpu;
 BYTE Reset_ai_cpu;
 BYTE last_reaet_di;
 BYTE last_reaet_Ai;
 BYTE Rcv_data_count;
 BYTE Temp_rcv_data_count;
 BYTE Rcv_num;
 BYTE Length;
 BYTE D_function;
 BYTE D_function_code;
 BYTE D_retry1;
 BYTE T_header;
 BYTE T_sequence_rcv;
 BYTE Last_t_sequence_rcv;
 BYTE A_header;
 BYTE A_sequence;
 BYTE A_function_code;
 BYTE A_retry1;
 BYTE Hdr;
 BYTE T_sequence_transmit;
 BYTE A_event_sequence;
 BYTE Last_a_event_sequence;
 BYTE Last_a_sequence;
 BYTE Send_length;
 BYTE Read_buff_time;
 BYTE Class;
 BYTE Last_event_type;
 BYTE N_crc;
 BYTE N_crc_trans;
 WORD D_retry;
 WORD D_ack_time_out;
 WORD D_confirm;
 WORD A_retry;
 WORD A_ack_time_out;
 WORD Initial_unsol;
 WORD Unsol_delay;
 WORD Sbo_time_out;
 WORD Frame_intv;
 BYTE COUNT_Error_recive;
 BYTE T_one_event_size;
 BYTE Process_data_count;
 BYTE Main_request_gr_count;
 BYTE One_control_block_size;
 BYTE Temp_data_count;
 BYTE Dnp_Temp_data_count;
 BYTE Tempb_l1;
 BYTE Tempb_l2;
 BYTE Tempb_h1;
 BYTE Ij;
 BYTE Lt;
 BYTE C;
 BYTE Ss;
 BYTE H;
 BYTE S;
 BYTE Baa;
 WORD Unsol_Address;
 WORD Send_unsol_delay;
 WORD Event_notification_delay;
 WORD Frame_count;
 WORD Srcadd;
 WORD Destadd;
 WORD Send_dest_add;
 WORD Crc_dnp;
 WORD Crc_d;
 WORD Num_crc_d;
 WORD _checksum;
 WORD Iin;
 WORD Start_data;
 WORD Stop_data;
 WORD Data_num;
 WORD error_code;
 WORD Sec_time;
 WORD Busy_line;
 WORD Time_delay;
 WORD Time_delay1;
 WORD Time_delay2;
 WORD Time_delay4;
 WORD Frame_intv_time;
 WORD Min_tx_delay;
 WORD Time_base;
 WORD Save_time;
 WORD Calcuclate;
 WORD Last_count_class0_table;
 WORD Start_table;
 WORD Count_class0_table;
 WORD Control_block_rcv_data;
 WORD Select_index_count;
 WORD Time_on;
 WORD Time_off;
 WORD Tempb;
 WORD X;
 WORD Xk;
 WORD I;
 WORD Kh;
 WORD Ba;
int Y;
 WORD Yy;
 WORD Yyy;
 WORD B;
 WORD Wt;
 WORD St_crc;
 WORD Tl;
 WORD A;
 WORD D;
 WORD Rrr;
 WORD Gg;
 DWORD W;
 WORD Ww;
 WORD Temp_count;
 WORD Temp_stop;
 WORD Temp_start;
 WORD Mm;
 WORD Xx;
 WORD Jj;
 WORD Jjj;
 int Calucate1;
 long Long_calucate1;
 int32_t int32_t_calcuclate;
 int32_t Last_32_main_buffer;
 float Single_main_buffer;
 float Last_single_main_buffer;
 float Single_calucate1;
 BYTE Tempa_l1;
 BYTE Tempa_h1;
 BYTE Tempa_l2;
 BYTE Tempa_h2;
 BYTE Tempc_l1;
 BYTE Tempc_h1;
 BYTE rezerv1;
 BYTE rezerv2;
 BYTE T_extra_one_event_size;
// extern WORD Dd;
 WORD Ao;
 WORD Kkk;
 WORD K;
 BYTE Temp1;
 BYTE Num_data;
 BYTE Data_buf;
 BYTE Data_count;
 WORD Array_len1;
 WORD Array_counter1;
 BYTE Db_ain;
 BYTE L1;
 BYTE L2;
 WORD L3;
 WORD L4;
 WORD Gi;
 WORD GK;
 BYTE End_data;
 BYTE Temperature;
 BYTE Battery;
 BYTE Command_card;
 BYTE Plc_protocol;
 BYTE Counter_error;
 WORD Plc_Address;
 BYTE No_time;
 BYTE Total_data_count;
 int Start_count;
 int32_t Base_time1;


 int32_t Control_block;

 WORD Main_count_table[20];
 WORD Time_on_off[14];

 WORD Rcv_index[14];
 WORD Index_table[14];
 WORD Count_table[14];
 WORD Delta_table[14];
 WORD Execute_index[14];
 BYTE Output_flag[14];
 WORD Output_buffer[14];
 BYTE Output_index[14];
 BYTE Enable_event_hmi[14];
 BYTE Enable_event_pc[14];
 BYTE Enable_event[14];
BYTE Do_code[14],Do_Type[14];
 BYTE Detect_event_flag[14];
 BYTE Enable_event_flag[14];
 WORD Counter_event[14];
 WORD Last_event_start[14];
 BYTE Last_event_num[14];
 BYTE Event_num[14];
 WORD Start_event_buf[14];
 WORD Start_event_num[14];
 BYTE Max_event_num[14];
 BYTE Save_one_event_size[15];
 BYTE Default_send_event_type[15];
 BYTE Event_num_gr[15];
 BYTE Index_output_flag[14];
 BYTE Send_quantity[14];
 BYTE Need_search_table_flag[16];
 BYTE Start_gr_class0_table[16];
 BYTE Repeat_gr_class0_table[16];
 BYTE Static_data_type[17];
 WORD Process_recieve_data[11];
 int32_t Ao_process_recieve_data[11];
 BYTE Save_event_type[12];
 BYTE One_event_size[12];
 BYTE Extra_one_event_size[12];
 BYTE Event_gr[12];
 BYTE Event_var[12];
 BYTE Send_event_type[12];
 BYTE Request_data[21];
 WORD Group[11];
 WORD Variation[11];
 WORD Qualifier[11];
 WORD Quantity[11];
 WORD Start_Bit[11];
 WORD Stop_Bit[11];
 WORD Num_crc_a[17];
 WORD Stt_crc[17];
 WORD Crc_a[17];
 BYTE Class_flag[5];
 BYTE Statuss[7];
 WORD Select_index[8];
 WORD Ainbuf[5];
 WORD Output_time[5];
 BYTE Array_temp[6];
 WORD Start_index_table[31];
 WORD Stop_index_table[31];
 WORD Count_gr_class0_table[21];
 BYTE T_group[22];
 BYTE T_variation[22];
 WORD T_addr[22];
 WORD T_addr_stop[22];
 BYTE Event_type[31];
 WORD Event_index[61];
 uint32_t Event_value[61];
 uint8_t Analogflag_value[61],Event_Analogflag[61];
 WORD Last_main_buffer[1301];
 BYTE Recieve_data[241];

 BYTE Modem_rcv[300];
 BYTE Event_data[310];
 BYTE Temp_crc[2060];

 BYTE E_event_num[4];
 WORD E_counter_event[4];
 BYTE E_last_event_start[5];
 BYTE E_last_event_num[6];
 BYTE E_event_type[32];
 BYTE E_event_data[312];
 WORD E_frozen_main_buffer[52];
 BYTE E_event_num_gr[7];
 bool Extension,last_reaet_ai,TrobbleInputFlag,Modem_led_Tx,Modem_led_Rx,Dnp3portBusyFlag;
 //extern unsigned short *Main_buffer;
 bool Dnp3_send_Flag,Dnp3_send_Flag1,_DnpSetTime;
 uint16_t Dnp3SendCnt;




  extern  BYTE Get_crc[640], Modem_rcv1[300];//,Count_ai,Count_di;

 //extern WORD Temp_main_buffer[100];   //sajedi cpu
 extern uint64_t Base_time;


 char E[4];
 char String_data[20];
 char Search[512];

void statrup_dnp3(void)
{
T_group[1]=1;
T_variation[1]=1;
T_addr[1]=St_add_di_plc;
T_addr_stop[1]=Stop_index_dio_plc;
T_group[2]=1;
T_variation[2]=2;
T_addr[2]=St_add_di_plc;
T_addr_stop[2]=Stop_index_dio_plc;
T_group[4]=10;
T_variation[4]=2;
T_addr[4]=St_add_do_plc_index;
T_addr_stop[4]=Stop_index_do_plc;
T_group[5]=30;
T_variation[5]=1;
T_addr[5]=St_add_ai_plc;//2000;
T_addr_stop[5]=Stop_index_aio_plc;
T_group[6]=30;
T_variation[6]=2;
T_addr[6]=St_add_ai_plc;//2000;
T_addr_stop[6]=Stop_index_aio_plc;
T_group[7]=30;
T_variation[7]=3;
T_addr[7]=St_add_ai_plc;//2000;
T_addr_stop[7]=Stop_index_aio_plc;
T_group[8]=30;
T_variation[8]=4;
T_addr[8]=St_add_ai_plc;//2000;
T_addr_stop[8]=Stop_index_aio_plc;
T_group[9]=40;
T_variation[9]=1;
T_addr[9]=St_add_ao_plc;
T_addr_stop[9]=Stop_index_ao_plc;
T_group[10]=40;
T_variation[10]=2;
T_addr[10]=St_add_ao_plc;
T_addr_stop[10]=Stop_index_ao_plc;
T_group[11]=30;
T_variation[11]=5;
T_addr[11]=St_add_ai_plc;//2000;
T_addr_stop[11]=Stop_index_aio_plc;
T_group[12]=40;
T_variation[12]=3;
T_addr[12]=St_add_ao_plc;
T_addr_stop[12]=Stop_index_ao_plc;
T_group[13]=20;
T_variation[13]=1;
T_addr[13]=St_add_cnt_plc;
T_addr_stop[13]=Stop_index_cnt_plc;
T_group[14]=20;
T_variation[14]=2;
T_addr[14]=St_add_cnt_plc;
T_addr_stop[14]=Stop_index_cnt_plc;
T_group[15]=20;
T_variation[15]=5;
T_addr[15]=St_add_cnt_plc;
T_addr_stop[15]=Stop_index_cnt_plc;
T_group[16]=20;
T_variation[16]=6;
T_addr[16]=St_add_cnt_plc;
T_addr_stop[16]=Stop_index_cnt_plc;
T_group[17]=21;
T_variation[17]=1;
T_addr[17]=St_add_freeze_cnt_plc;
T_addr_stop[17]=Stop_index_freeze_cnt_plc;
T_group[18]=21;
T_variation[18]=2;
T_addr[18]=St_add_freeze_cnt_plc;
T_addr_stop[18]=Stop_index_freeze_cnt_plc;
T_group[19]=21;
T_variation[19]=9;
T_addr[19]=St_add_freeze_cnt_plc;
T_addr_stop[19]=Stop_index_freeze_cnt_plc;
T_group[20]=21;
T_variation[20]=10;
T_addr[20]=St_add_freeze_cnt_plc;
T_addr_stop[20]=Stop_index_freeze_cnt_plc;
Data_count=1;
Crc_dnp=0;
Fcb=0;
Iin=0;
Iin=Iin | 128;
Iin=Iin | 16;
A_event_sequence=16;
Data_num=0;
Frame_count=0;
Wait_unsol_flag=0;
D_time_out_flag=1;
A_time_out_flag=1;
Last_event_time=1;
Send_unsol_flag=1;
Check_flag=1;
Test_flag=0;
Max_event_num[1]=10;
Max_event_num[2]=10;
Max_event_num[3]=5;
Start_event_buf[1]=1;
Start_event_buf[2]=121;
Start_event_buf[3]=242;
Start_event_num[1]=1;
Start_event_num[2]=101;
Start_event_num[3]=201;
//Base_time=E_base_time;
//Base_time1=E_base_time1;

for(Y=1; Y<=310; Y+=1)
{
    Event_data[Y]=0;
//    reset_watchdog();	;
}
for(Y=1; Y<=1300; Y+=1)
  {
    Last_main_buffer[Y]=0;
//    reset_watchdog();	;
  }
for(Y=1; Y<=240; Y+=1)
  {
    Recieve_data[Y]=0;
//    reset_watchdog();	;
  }
for(Y=1; Y<=300; Y+=1)
  {
    Modem_rcv1[Y]=0;
    Modem_rcv[Y]=0;
//    reset_watchdog();	;
  }
for(Y=1; Y<=2050; Y+=1)
  {
    Temp_crc[Y]=0;
//    reset_watchdog();	;
  }
for(GK=1; GK<=2000; GK+=1)
  {
    Main_buffer[GK]=0;
  }
for(GK=1; GK<=1300; GK+=1)
  {
    Last_main_buffer[GK]=0;
  }
Save_one_event_size[Di_event_obj]=9;
Save_one_event_size[Ai_event_obj]=13;
Save_one_event_size[Cnt_event_obj]=13;
Save_one_event_size[Float_event_obj]=7;
One_event_size[Binary_chang_no_time]=3;
Extra_one_event_size[Binary_chang_no_time]=6;
Event_gr[Binary_chang_no_time]=2;
Event_var[Binary_chang_no_time]=1;
Save_event_type[Binary_chang_no_time]=1;
One_event_size[Binary_chang_with_time]=9;
Extra_one_event_size[Binary_chang_with_time]=0;
Event_gr[Binary_chang_with_time]=2;
Event_var[Binary_chang_with_time]=2;
Save_event_type[Binary_chang_with_time]=1;
One_event_size[a32_bit_analog_chang_no_time]=7;
Extra_one_event_size[a32_bit_analog_chang_no_time]=6;
Event_gr[a32_bit_analog_chang_no_time]=32;
Event_var[a32_bit_analog_chang_no_time]=1;
Save_event_type[a32_bit_analog_chang_no_time]=2;
One_event_size[a16_bit_analog_chang_no_time]=7;
Extra_one_event_size[a16_bit_analog_chang_no_time]=6;
Event_gr[a16_bit_analog_chang_no_time]=32;
Event_var[a16_bit_analog_chang_no_time]=2;
Save_event_type[a16_bit_analog_chang_no_time]=2;
One_event_size[a32_bit_analog_chang_with_time]=13;
Extra_one_event_size[a32_bit_analog_chang_with_time]=0;
Event_gr[a32_bit_analog_chang_with_time]=32;
Event_var[a32_bit_analog_chang_with_time]=3;
Save_event_type[a32_bit_analog_chang_with_time]=2;
One_event_size[a16_bit_analog_chang_with_time]=13;
Extra_one_event_size[a16_bit_analog_chang_with_time]=0;
Event_gr[a16_bit_analog_chang_with_time]=32;
Event_var[a16_bit_analog_chang_with_time]=4;
Save_event_type[a16_bit_analog_chang_with_time]=2;
One_event_size[Float_chang_no_time]=7;
Extra_one_event_size[Float_chang_no_time]=0;
Event_gr[Float_chang_no_time]=32;
Event_var[Float_chang_no_time]=5;
Save_event_type[Float_chang_no_time]=4;
One_event_size[a32_bit_counter_chang_no_time]=7;
Extra_one_event_size[a32_bit_counter_chang_no_time]=6;
Event_gr[a32_bit_counter_chang_no_time]=22;
Event_var[a32_bit_counter_chang_no_time]=1;
Save_event_type[a32_bit_counter_chang_no_time]=3;
One_event_size[a16_bit_counter_chang_no_time]=7;
Extra_one_event_size[a16_bit_counter_chang_no_time]=6;
Event_gr[a16_bit_counter_chang_no_time]=22;
Event_var[a16_bit_counter_chang_no_time]=2;
Save_event_type[a16_bit_counter_chang_no_time]=3;
One_event_size[a32_bit_counter_chang_with_time]=13;
Extra_one_event_size[a32_bit_counter_chang_with_time]=0;
Event_gr[a32_bit_counter_chang_with_time]=22;
Event_var[a32_bit_counter_chang_with_time]=5;
Save_event_type[a32_bit_counter_chang_with_time]=3;
One_event_size[a16_bit_counter_chang_with_time]=13;
Extra_one_event_size[a16_bit_counter_chang_with_time]=0;
Event_gr[a16_bit_counter_chang_with_time]=22;
Event_var[a16_bit_counter_chang_with_time]=5;
Save_event_type[a16_bit_counter_chang_with_time]=3;
//Y = Com1Porotocol;
Plc_protocol=Y;
//if(Plc_protocol==Dnp3)
{
//    Plc_Address=COM1.Dnp3Config.Dnp3Address.dnp3SrcAdd;
//    Unsol_Address=COM1.Dnp3Config.Dnp3Address.dnp3DestAdd;
//    D_confirm=COM1.Dnp3Config.Dnp3DataLinkSetup.ConfirmMode;
//    D_retry=COM1.Dnp3Config.Dnp3DataLinkSetup.Rtry;
//    D_ack_time_out=COM1.Dnp3Config.Dnp3DataLinkSetup.ConfirmTout;
//    A_retry=COM1.Dnp3Config.Dnp3AppLayerSetup.Rtry;
//    A_ack_time_out=2;
//    Unsol_delay=1;
//    Initial_unsol=1;
//    Sbo_time_out=COM1.Dnp3Config.Dnp3AppLayerSetup.SelectArmTout;
//    Frame_intv=COM1.Dnp3Config.Dnp3AppLayerSetup.Compelete_FragmentationTout;
//		Enable_event_hmi[1]=1;
//		Enable_event_hmi[2]=1;
//		Enable_event_hmi[3]=1;




    Y = 1;//Binaryeventobject;
    if(Y==0){
       Default_send_event_type[Di_event_obj]=Binary_chang_no_time;
      }
    else if(Y==1){
       Default_send_event_type[Di_event_obj]=Binary_chang_with_time;
      }
    Y=3;//Analogeventobject;
    if(Y==0){
       Default_send_event_type[Ai_event_obj]=a32_bit_analog_chang_no_time;
      }
    else if(Y==1){
       Default_send_event_type[Ai_event_obj]=a16_bit_analog_chang_no_time;
      }
    else if(Y==2){
       Default_send_event_type[Ai_event_obj]=a32_bit_analog_chang_with_time;
      }
    else if(Y==3){
       Default_send_event_type[Ai_event_obj]=a16_bit_analog_chang_with_time;
      }
    Y =3;// Converteventobject;
    if(Y==0){
       Default_send_event_type[Cnt_event_obj]=a32_bit_counter_chang_no_time;
      }
    else if(Y==1){
       Default_send_event_type[Cnt_event_obj]=a16_bit_counter_chang_no_time;
      }
    else if(Y==2){
       Default_send_event_type[Cnt_event_obj]=a32_bit_counter_chang_with_time;
      }
    else if(Y==3){
       Default_send_event_type[Cnt_event_obj]=a16_bit_counter_chang_with_time;
      }
    Y = 0;//Floateventobject;
    if(Y==0){
       Default_send_event_type[Float_event_obj]=Float_chang_no_time;
      }
    Class_flag[1]=1;
    Class_flag[2]=1;
    Class_flag[3]=1;
  	Enable_event_pc[1]=1;
  }
//	else{
//   Plc_Address =1;
//  }

//	Reset_di_cpu = 1;
//	Reset_ai_cpu = 1;



 //Read_lookup();
}

//********************************************************************************
void Dnp3_run(void)
{


	unsigned char i;

		_BaseTime1=Base_time>>48 & 0xFFFF;
		_BaseTime2=Base_time>>32 & 0xFFFF;
		_BaseTime3=Base_time>>16 & 0xFFFF;
		_BaseTime4=Base_time     & 0xFFFF;

	  if (Dnp3_send_Flag1){
			Dnp3_send_Flag1=0;
			DNP3_Transmit(Array_len1);
		}

		if(Reset_flag==1)
    {
        Reset_flag=0;
       // goto RESET_MICRO;           ************************
    }
    if(Read_binary_output_flag==1)
    {
        Read_binary_output_flag=0;
        for(H=1; H<=3; H+=1)
        {
					if(Output_flag[H]==1 && (Main_buffer[Output_buffer[H]]==Outon || Main_buffer[Output_buffer[H]]==TripOutOn || Main_buffer[Output_buffer[H]]==CloseOutoff))
						{
							Output_time[H]=Output_time[H]+10;
							if(Output_time[H] >= Time_on_off[H])
							{
									Main_buffer[Output_buffer[H]]=Outoff;
									Output_time[H]=0;
									Output_flag[H]=0;
							}
					}
					else
					{
							Output_flag[H]=0;
							Output_time[H]=0;
					}
        }
    }
//    if(Fill_e2prom_event_flag==1)
//    {
//			Fill_e2prom_event_flag=0;
//			for(Class=1; Class<=3; Class+=1)
//				{
//					 FILL_E2PROM_EVENT();
//				}
//    }
    for(Class=1; Class<=3; Class+=1)
    {
			if(Detect_event_flag[Class]==1)
				{
					Detect_event_flag[Class]=0;
					DETECT_EVENT();
				}
    }
//    if(Read_buff_flag==1)
//      {
//        Read_buff_flag=0;
//        Temp1++;
//        if(Temp1==4)
//          {
//            Temp1=1;
//          }
//          READ_BUFF();
//      }
    if(Rcv_flag_dnp==1 )//|| Rcv_dnptcpip_flag)
    {
      PROCEESS_RCV_DATA();
		  //HAL_UART_RxCpltCallback(&huart3);
			//HAL_UART_Receive_IT(&huart3,&Modem_rcv1[1],1000);
    }
		D_confirm=3;  //madad
    if(D_confirm==3)
    {
			Accept_flag=1;
			if(Frame_count!=0&&Frame_intv_flag==1)
				{
					Send_application_response();
					Frame_intv_flag=0;
				}
    }
    if(Initial_unsol==0)
    {
      Ok_flag=1;
    }
    if(Initial_unsol==1&&Send_unsol_flag==1&&Wait_unsol_flag==1&&Ok_flag==0)
    {
        if(Accept_flag==1)
          {
            if(A_time_out_flag==1)
              {
                Unsol_response_flag=1;
                Frame_count=1;
                Unsol_add_flag=1;
                Send_application_response();
              }
          }
        else
          {
            if(D_time_out_flag==1)
              {
                Fcb=0;
                Send_reset_link=1;
                Unsol_add_flag=1;
                Send_datalink_response();
              }
          }
    }

    if(Send_application_response_flag==1)
    {
        Send_application_response_flag=0;
        Send_first_fragment=0;
				Clr_bit (&Iin, 11);
        if(Event_num[1]>=Max_event_num[1])
          {
            Set_bit (&Iin, 11);

          }
        if(Event_num[2]>=Max_event_num[2])
          {
            Set_bit (&Iin, 11);
          }
        if(Event_num[3]>=Max_event_num[3])
          {
           Set_bit (&Iin, 11);
          }
        Send_application_response();
        Clr_bit (&Iin, 8);
				Clr_bit (&Iin, 9);
				Clr_bit (&Iin, 10);
   }

    if(D_rcv_flag==1)
    {
        if(D_function_code==128||D_function_code==0)
          {
            if(Accept_flag==0&&Destadd==Send_dest_add)
              {
                Accept_flag=1;
                Send_reset_link=0;
              }
            if(Accept_flag==1)
              {
                D_time_out_flag=1;
                D_retry1=D_retry;
                D_send_flag=0;
                Time_delay=0;
                if(Frame_count>0&&Frame_intv_flag==1)
                  {
                    Send_application_response();
                    Frame_intv_flag=0;
                  }
              }
          }
        else if(D_function_code==129)
          {
            Accept_flag=0;
            D_send_flag=0;
            Time_delay=0;
          }
        else
        {
					if(D_function_code!=196)
					{
						Send_datalink_response();
					}
        }
        D_rcv_flag=0;
    }
    if(A_rcv_flag==1)
    {

        if(Need_send_confirm==1)
          {
            Data_num=0;
            Send_first_fragment=0;
            Send_application_response();
          }
        if(A_function_code==0&&A_send_flag==1&&Destadd==Send_dest_add)
          {
            Error_recive=0;
            Clr_bit (&Iin, 11);
            if(A_sequence==Last_a_sequence)
              {
                A_sequence++;
                if(Send_unsol_flag==0)
                  {
                    Ok_flag=1;
                  }
                goto O1;
              }
            else if(A_sequence==Last_a_event_sequence)
              {
                A_event_sequence++;
                Send_unsol_flag=0;
                if(Ok_flag==1)
                  {
                   goto O1;
                  }
                else
                  {
                   goto O2;
                  }
              }
            else
              {
                goto O2;
              }
O1:
            Wait_unsol_flag=0;
            Chang_flag=0;
            for(Class=1; Class<=3; Class+=1)
              {
                if(Class_flag[Class]==1)
                  {
                    Class_flag[Class]=0;
                    for(H=1; H<=Send_quantity[Class]; H+=1)
                    {
                      S=Start_event_num[Class]+H;
                      S--;
											if(Event_num_gr[Event_type[S]]>0)
												{
													Event_num_gr[Event_type[S]]--;
												}
                    }
                    E_event_num_gr[1]=Event_num_gr[1];
                    E_event_num_gr[2]=Event_num_gr[2];
                    E_event_num_gr[3]=Event_num_gr[3];
                    E_event_num_gr[4]=Event_num_gr[4];
                    Ss=0;
                    for(W=1; W<=Send_quantity[Class]; W+=1)
                    {
                        Y=Start_event_num[Class]+W;
                        Y--;
                        Ss=Ss+One_event_size[Default_send_event_type[Event_type[Y]]];
                    }
                    Ww=1;
                    Y=Start_event_buf[Class]+Ss;
                    Yy=Counter_event[Class];
                    for(Yyy=Y; Yyy<=Yy; Yyy+=1)
                      {
                        Event_data[Ww]=Event_data[Yyy];
                        Ww++;
                      }
                    K=Send_quantity[Class]+1;
                    Ww=1;
                    for(W=K; W<=Event_num[Class]; W+=1)
                    {
											Y=Start_event_num[Class]+W;
											Y--;
											Yy=Start_event_num[Class]+Ww;
											Event_type[Yy]=Event_type[Y];
											Ww++;
                    }
                    Counter_event[Class]=Counter_event[Class]-Ss;
                    Event_num[Class]=Event_num[Class]-Send_quantity[Class];
                    Last_event_num[Class]=Event_num[Class];
                    Last_event_start[Class]=Start_event_buf[Class]+Counter_event[Class];
                    E_event_num[Class]=Event_num[Class];
                    E_counter_event[Class]=Counter_event[Class];
                    E_last_event_start[Class]=Last_event_start[Class];
                    E_last_event_num[Class]=Last_event_num[Class];
                  }
                if(Event_num[Class]==0)
                  {
                    Clr_bit (&Iin,Class);
                  }
              }
            if(Event_num[1]>Max_event_num[Class]&&Event_num[2]>Max_event_num[Class]&&Event_num[3]>Max_event_num[Class])
            {
                 Clr_bit (&Iin, 11);
            }

O2:
            A_time_out_flag=1;
            A_send_flag=0;
            Time_delay1=0;
            A_retry1=A_retry;
            if(A_sequence==Last_a_event_sequence)
              {
                Ok_flag=1;
                Event_send_flag=0;
              }
            A_rcv_flag=0;
            Rcv_data_count=0;
          }
        else
          {
            if(Accept_flag==0)
              {
                if(D_time_out_flag==1)
                  {
                    Fcb=0;
                    Send_reset_link=1;
                    Send_datalink_response();
                  }
              }
            else
              {
                if(A_function_code==13||A_function_code==14)
                  {
                    Data_num=6;
                    Temp_crc[1]=52;
                    Temp_crc[2]=2;
                    Temp_crc[3]=7;
                    Temp_crc[4]=1;
                    Temp_crc[5]=16;
                    Temp_crc[6]=39;
                    Send_application_response_flag=1;
                    if(A_function_code==13)
                      {
                        Reset_flag=1;
                      }
                    else
                      {
                        Accept_flag=0;
                        Fcb=0;
                      }
                    A_rcv_flag=0;
                    Rcv_data_count=0;
                  }
                else
                  {
                    Process_rcv_function();
                    A_rcv_flag=0;
                    Rcv_data_count=0;
                  }
              }
          }
     }
    if(Wait_unsol_flag==1)
      {

        if(Check_flag==1)
          {
            Check_flag=0;
            for(Class=1; Class<=3; Class+=1)
              {
                if(Event_num[Class]>0)
                  {
                    if(Enable_event[Class] == 0||Ok_flag==0||Accept_flag==0)
                      {
                       //FILL_E2PROM_EVENT();
                      }
                  }
              }
          }
        Main_request_gr_count=0;
        if(Ok_flag==1&&Frame_count==0&&A_time_out_flag==1)
          {
            for(Class=1; Class<=3; Class+=1)
              {
                if(Event_num[Class]>0&&Enable_event[Class]==1)
                  {
                    Main_request_gr_count++;
                    Group[Main_request_gr_count]=60;
                    Variation[Main_request_gr_count]=Class+1;
                    Qualifier[Main_request_gr_count]=40;
                    Quantity[Main_request_gr_count]=Event_num[Class];
                    Request_data[Main_request_gr_count]=Event;
                  }
              }
            if(Main_request_gr_count>0)
              {
                if(Accept_flag==1)
                  {
                    Data_num=0;
                    Fill_data();
                    Event_send_flag=1;
                    Send_first_fragment=0;
                    Unsol_add_flag=1;
                    Send_application_response();
                  }
                else if(Accept_flag==0&&D_time_out_flag==1)
                  {
                    Fcb=0;
                    Send_reset_link=1;
                    Unsol_add_flag=1;
                    if(A_retry1==0&&Test_flag==0)
                      {
                        D_retry1=D_retry;
                        A_retry1=A_retry;
                      }
                    Send_datalink_response();
                  }
              }
            else
              {
                Chang_flag=0;
              }
          }
      }
    for(Class=1; Class<=3; Class+=1)
    {
       if (Enable_event_pc[Class]==1)//(Enable_event_hmi[Class]==1&&
       {
         Enable_event[Class]=1;
        }
        else
        {
           Enable_event[Class]=0;
        }
    }

}   //end of dnp3_run




void Dnp3_systick(void)
{

	if (Dnp3_send_Flag){
		Dnp3SendCnt++;
		if (Dnp3SendCnt > 350){
			Dnp3SendCnt=0;
			Dnp3_send_Flag=0;
			Dnp3_send_Flag1=1;
		}
	}

Read_buff_time++;
if(Read_buff_time>10)
  {
    Read_buff_time=0;
  //  Read_buff_flag=1;
		Read_binary_output_flag=1;
	}


//Time_base++;
//if(Time_base>=1000)
  //{
   // Time_base=0;
   // Save_time++;
   // if(Save_time>36000)
   //   {
     //   Save_time=0;
        //E_base_time=Base_time;
       // E_base_time1=Base_time1;
    //  }
 // }


Sec_time++;
if(Sec_time>=1000)
  {
    Sec_time=0;
    if(D_send_flag==1)
      {
        Time_delay++;
        if(Time_delay>=D_ack_time_out)
          {
            Time_delay=0;
            D_send_flag=0;
            D_time_out_flag=1;
            if(D_retry1>0)
              {
                D_retry1--;
              }
            else
              {
                if(Send_reset_link==1&&A_retry1>0)
                  {
                    Send_reset_link=0;
                    D_retry1=A_retry1;
                    A_retry1=0;
                    Accept_flag=0;
                    Test_flag=1;
                  }
                else
                  {
                    Send_unsol_flag=0;
                    Test_flag=0;
                    Ok_flag=1;
                    //Chang_flag=0; Madad
                    Wait_unsol_flag=0;
                  }
              }
          }
      }
    else
      {
        Time_delay=0;
      }
    if(A_send_flag==1)
      {
        Time_delay1++;
        if(Time_delay1>=A_ack_time_out)
          {
            Time_delay1=0;
            A_send_flag=0;
            A_time_out_flag=1;
            if(A_retry1==0)
              {
                for(Class=1; Class<=3; Class+=1)
                  {
                    if(Event_send_flag==1&&Class_flag[Class]==1)
                      {
                        Fill_e2prom_event_flag=1;
                      }
                    Class_flag[Class]=0;
                    Chang_flag=0;
                  }
                Wait_unsol_flag=0;
                A_retry1=A_retry;
                Send_unsol_flag=0;
                Event_send_flag=0;
                A_event_sequence++;
                if(Accept_flag==1)
                  {
                    Ok_flag=1;
                  }
                Accept_flag=0;
              }
            else if(A_retry1>0)
              {
                A_retry1--;
              }
          }
      }
    else
      {
        Time_delay1=0;
      }
    if(Wait_unsol_flag==0)
      {
        if(Chang_flag==1||Ok_flag==0)
          {
            Time_delay2++;
            if(Time_delay2>=Unsol_delay)
              {
                Time_delay2=0;
                Wait_unsol_flag=1;
              }
          }
        else
          {
            Time_delay2=0;
          }
      }
    if(Sbo_delay_time==1)
    {
			Time_delay4++;
			if(Time_delay4>=Sbo_time_out)
			{
					Time_delay4=0;
					Sbo_time_out_flag=1;
					Sbo_delay_time=0;
					Select_index_count=0;
			}
    }
    else
    {
        Time_delay4=0;
        //Sbo_time_out_flag=0;
    }
//    if(Last_event_time==0)
//      {
//        Time_delay3++;
//        if(Time_delay3>Send_unsol_delay)
//          {
//            Last_event_time=1;
//            Time_delay3=0;
//          }
//      }
//    else
//      {
//        Last_event_time=1;
//      }

 }

if(Frame_count>0&&Frame_intv_flag==0)
  {
    Frame_intv_time++;
    if(Frame_intv_time>Frame_intv)
      {
        Frame_intv_time=0;
        Frame_intv_flag=1;
      }
  }
else
  {
    Frame_intv_time=0;
  }
}

//*************************************************************
void READ_BUFF(BYTE classtype)
{
	Class=classtype;
	Main_count_table[Class]=0;
	Y=0;
	for(K=1; K<=4; K+=1)
  {
    Yy=0;
    Yy=Main_count_table[Class]*3;
    if(K>1)
    {
        Yy++;
    }
    Y+=Yy;
    if(Class==1)
    {
        Main_count_table[Class]=Class1[Y];
    }
    else if(Class==2)
    {
        Main_count_table[Class]=Class2[Y];
    }
    else if(Class==3)
    {
        Main_count_table[Class]=Class3[Y];
    }
    Yy=0;
    W=Y;
    for(Yyy=1; Yyy<=Main_count_table[Class]; Yyy+=1)
    {
        W++;
        if(Class==1)
          {
            Index_table[Class]=Class1[W];
            W++;
            Count_table[Class]=Class1[W];
            W++;
            Delta_table[Class]=Class1[W];
          }
        else if(Class==2)
          {
            Index_table[Class]=Class2[W];
            W++;
            Count_table[Class]=Class2[W];
            W++;
            Delta_table[Class]=Class2[W];
          }
        else if(Class==3)
          {
            Index_table[Class]=Class3[W];
            W++;
            Count_table[Class]=Class3[W];
            W++;
           Delta_table[Class]=Class3[W];
          }
        Xx=Index_table[Class]+Count_table[Class];
        if(Count_table[Class]>0)
          {
            Xx--;
          }
        else
          {
            Xx=Index_table[Class];
          }
        for(Ww=Index_table[Class]; Ww<=Xx; Ww+=1)
          {
            for(;;)
            {
              if(K==1)
                {
                  if(Ww<=Stop_index_dio_plc)
                    {
                      Mm=Ww/16;
                      Gg=St_add_last_di_plc+Mm;
                      if(Ww<=255)
                        {
                          X=St_add_di_plc+Mm;
                        }
                      else
                        {
                          Mm=Ww-256;
                          Mm/=16;
                          X=St_add_soft_dio_plc+Mm;
                        }
                        Mm=Ww % 16;
                      if(Chk_bit_16 (&Main_buffer[X], Mm)!=Chk_bit_16 (&Last_main_buffer[Gg], Mm))
                        {
                            Change_bit (&Last_main_buffer[Gg],Mm,Chk_bit_16 (&Main_buffer[X], Mm));
                          last_reaet_di=1;
//                          if(Reset_di_cpu==0)
//                          {
                              Detect_event_flag[Class]=1;
                              Event_num[Class]++;
                              Jj=Start_event_num[Class]+Event_num[Class];
                              Jj--;
                              Event_type[Jj]=Di_event_obj;
                              Event_index[Jj]=Ww;
                              Change_bit_32((uint32_t *)&Event_value[Jj],0,Chk_bit_16(&Main_buffer[X], Mm));
                              Event_num_gr[Di_event_obj]++;
                       //   }
                        }
                    }
                  break;
                }
              if(K==2)
                {
                  if(Ww<=Stop_index_aio_plc)
                    {
                      Temp_count=0;
                      Rrr=0;
                      for(Gg=1; Gg<=5; Gg+=1)
                        {
                          X=Temp_count*2;
                          if(Gg>1)
                            {
                              X++;
                            }
                          Rrr+=X;
                          Temp_count=Class0[Rrr];
                        }
                      for(H=1; H<=6; H+=1)
                        {
                          Temp_count=Class0[Rrr];
                          if(Temp_count==0)
                            {
                              Rrr++;
                            }
                          for(Gg=1; Gg<=Temp_count; Gg+=1)
                            {
                              Rrr++;
                             Temp_start=Class0[Rrr];
                              Rrr++;
                             Temp_stop=Class0[Rrr];
                              Temp_stop=Temp_start+Temp_stop;
                              Temp_stop--;
                              if(Ww>=Temp_start&&Ww<=Temp_stop)
                                  goto _EXIT;
                            }
                          if(H==6)
                              goto _EXIT1;
                        }

_EXIT:
                      Jj=H%2;
                      if(Jj==0)
                        {
                          Rrr=Ww+St_add_last_ai_plc;
                          X=Ww+St_add_soft_aio_plc;
                          for(;;)
                          {
														if(Main_buffer[X]!=Last_main_buffer[Rrr])
															{
																Calucate1=Last_main_buffer[Rrr]-Main_buffer[X];
																Calcuclate=abs(Calucate1);
																if(Calcuclate>=Delta_table[Class])
																	{
																		Last_main_buffer[Rrr]=Main_buffer[X];

NEXT_EVENT:
																		last_reaet_ai=1;
//																		if(Reset_ai_cpu==0)
//																		{
																				Detect_event_flag[Class]=1;
																				Event_num[Class]++;
																				Jj=Start_event_num[Class]+Event_num[Class];
																				Jj--;
																				Event_type[Jj]=Ai_event_obj;
																				Event_index[Jj]=Ww;
																				Event_value[Jj]=Main_buffer[X];
																				//Event_Analogflag[Jj]=Analogflag_value[X-196];
																				Event_num_gr[Ai_event_obj]++;
																		//}
																	}
															}
                            break;
                          }
                        }
                      else
                        {
                        }
                    }
                  break;
                }
              if(K==3)
                {
                  break;
                }
              if(K==4)
_EXIT1:
              break;
            }
          }
      }
  }
//    if ((Reset_di_cpu == 1) && (last_reaet_di ==1) && (Count_di > 30)){
//			Reset_di_cpu = 0;
//			Count_di=0;
//		}
//    if ((Reset_ai_cpu == 1) && (last_reaet_ai==1) && (Count_ai > 30)){
//			Reset_ai_cpu = 0;
//		  Count_ai=0;
//		}
}

void DETECT_EVENT()
{
	DWORD InputTime;
	BYTE *pTemp=(BYTE *)&InputTime;
	//DWORD *Temp_d =(DWORD *)&Temp_main_buffer[2];  //sajedi cpu




if(Event_num[Class]>Max_event_num[Class])
{
	 CHECK_BUFFER_OVERFLOW();
}

Chang_flag=1;
Check_flag=1;
Set_bit(&Iin,Class);
W=Counter_event[Class]+Start_event_buf[Class];
W--;
Yyy=Last_event_num[Class]+1;
for(Y=Yyy; Y<=Event_num[Class]; Y+=1)
  {
    Jj=Start_event_num[Class]+Y;
    Jj--;
    for(;;)
    {
      if(Event_type[Jj]==Di_event_obj)
        {
          W++;
          Event_data[W]=Get_Low_Byte(Event_index[Jj]);
          W++;
          Event_data[W]=Get_High_Byte(Event_index[Jj]);
          W++;
          Event_data[W]= 0X0001;
          Change_bit_8(&Event_data[W],7,Chk_bit_32(&Event_value[Jj],0));
          break;
        }
      if(Event_type[Jj]>=Ai_event_obj&&Event_type[Jj]<=Float_event_obj)
        {
          Yy=Event_value[Jj];
          W++;
          Event_data[W]=Get_Low_Byte(Event_index[Jj]);
          W++;
          Event_data[W]=Get_High_Byte(Event_index[Jj]);
          W++;
          Event_data[W]=1;
          W++;
          Event_data[W]=Get_Low_Byte(Yy);
          W++;
          Event_data[W]=Get_High_Byte(Yy);
          Yy=Get_High_Word(Event_value[Jj]);
          W++;
          Event_data[W]=Get_Low_Byte(Yy);
          W++;
          Event_data[W]=Get_High_Byte(Yy);
        }
      break;
    }
    No_time=1;
    if(Default_send_event_type[Event_type[Jj]]==1)
      {
        No_time=0;
      }
    else if(Default_send_event_type[Event_type[Jj]]==3)
      {
        No_time=0;
      }
    else if(Default_send_event_type[Event_type[Jj]]==4)
      {
        No_time=0;
      }
    else if(Default_send_event_type[Event_type[Jj]]==7)
      {
        No_time=0;
      }
    else if(Default_send_event_type[Event_type[Jj]]==8)
      {
        No_time=0;
      }
    if(No_time==1)
    {
				BYTE *pBase=(BYTE *)&Base_time;
        W++;
        Event_data[W]=*pBase++;
        W++;
        Event_data[W]=*pBase++;
        W++;
        Event_data[W]=*pBase++;
        W++;
        Event_data[W]=*pBase++;
        W++;
        Event_data[W]=*pBase++;
        W++;
        Event_data[W]=*pBase++;
	}
}
Counter_event[Class]=W-Start_event_buf[Class];
Counter_event[Class]++;
if(Event_num[Class]>Max_event_num[Class])
  {
    Event_num[Class]=Max_event_num[Class];
  }
Last_event_num[Class]=Event_num[Class];
A_retry1=_AL_Retry; //madad
}

///**********************************************************************
void CHECK_BUFFER_OVERFLOW()
{
	K=Event_num[Class]-Max_event_num[Class];
	Set_bit(&Iin,11);
	Ss=0;
	for(W=1; W<=K; W+=1)
	{
		Y=Start_event_num[Class]+W;
		Y--;
		Ss=Ss+One_event_size[Default_send_event_type[Event_type[Y]]];
	}
	Ww=1;
	Y=Start_event_buf[Class]+Ss;
	Yy=Counter_event[Class];
	for(Yyy=Y; Yyy<=Yy; Yyy+=1)
	{
		Event_data[Ww]=Event_data[Yyy];
		Ww++;
	}
	K++;
	Ww=0;
	for(W=K; W<=Event_num[Class]; W+=1)
	{
		Y=Start_event_num[Class]+W;
		Y--;
		Yy=Start_event_num[Class]+Ww;
		Event_type[Yy]=Event_type[Y];
		Ww++;
	}
	Counter_event[Class]=Counter_event[Class]-Ss;
}
//**********************************************************************
void PROCEESS_RCV_DATA()
{
  uint8_t i;



if (Rcv_flag_dnp)
{
	Rcv_flag_dnp=0;
	for(Wt=1; Wt<=Dnp_Temp_data_count; Wt+=1)
	{
			Modem_rcv[Wt]=Modem_rcv1[Wt];
			Rcv_flag_dnp=0;
	}
	//Send_dnpport=Com_Serial;
}
//if (Rcv_dnptcpip_flag)
//{
//	Rcv_dnptcpip_flag=0;
//	for(i=1,Wt=0; Wt<Dnp_Temp_data_count; Wt+=1,i++)
//	{
//    Modem_rcv[i]=Dnp_rcv_eth[Wt];
//	}
//	Send_dnpport=Ethernet;
//}

Total_data_count=Dnp_Temp_data_count;
Ij=8;
N_crc=3;
Lt=1;
Temp_rcv_data_count=0;
for(Xk=1; Xk<=16; Xk+=1)
  {
    Num_crc_a[Xk]=0;
  }
Stt_crc[Lt]=11;
if(Modem_rcv[1]==5&&Modem_rcv[2]==100)
{
    Srcadd=makeint(Modem_rcv[5],Modem_rcv[6]);
    Destadd=makeint(Modem_rcv[7],Modem_rcv[8]);
    if(Srcadd==_Slaveaddress&&Destadd==_Unsoladdress)
      {
				//Led3R(1);   // najafi
				Modem_led_Rx = 1  ;
        Length=Modem_rcv[3];
        D_function_code=Modem_rcv[4];
        Crc_d=makeint(Modem_rcv[10],Modem_rcv[9]);
        Num_crc_d=8;
        if(Length==5)
          {
            goto LL2;
          }
        T_header=Modem_rcv[11];
        T_sequence_rcv=T_header & 31;
        A_header=Modem_rcv[12];
        Need_send_confirm=Chk_bit(&A_header,5);
        A_sequence=A_header & 31;
        A_function_code=Modem_rcv[13];
       Ij=13;
        if(A_function_code==0||A_function_code==13||A_function_code==14||A_function_code==23)
          {
						reqDataDnp3=0;
            goto LL1;

          }
        if(Total_data_count>15&&Total_data_count<50)
          {
            Total_data_count-=15;
            while(Total_data_count>0)
              {
                if(N_crc<16)
                  {
                    Ij++;
                    N_crc++;
                    Total_data_count--;
                    Temp_rcv_data_count++;
                    Recieve_data[Temp_rcv_data_count]=Modem_rcv[Ij];
                  }
                else if(N_crc>=16)
                  {
                    Num_crc_a[Lt]=Ij;
                    Ij++;
                    Crc_a[Lt]=makeint(Modem_rcv[Ij+1],Modem_rcv[Ij]);
                    Ij++;
                    N_crc=0;
                    Total_data_count-=2;
                    Lt++;
                    Stt_crc[Lt]=Ij+1;
                  }
              }
            Rcv_data_count=Temp_rcv_data_count;
          }
        else
            goto ENDPROCESS;

LL1:;

        Num_crc_a[Lt]=Ij;
        Ij++;
        Crc_a[Lt]=makeint(Modem_rcv[Ij+1],Modem_rcv[Ij]);
        Ij++;

LL2:;

				Crc_dnp=Crc_calc1(1,Num_crc_d);
       // Crc_dnp=CRC_calculator(&Modem_rcv[1],Num_crc_d);
        if(Get_Low_Byte(Crc_d)==Get_High_Byte(Crc_dnp)&&Get_High_Byte(Crc_d)==Get_Low_Byte(Crc_dnp))
          {
            D_rcv_flag=1;
            if(Length==5)
              {
              }
            else if(Length>5)
              {
                Ij=0;
                for(Xk=1; Xk<=Lt; Xk+=1)
                  {
                    Crc_dnp=Crc_calc1(Stt_crc[Xk],Num_crc_a[Xk]);
                   // Crc_dnp=CRC_calculator(&Modem_rcv[Stt_crc[Xk]],Num_crc_a[Xk]);//
                    if(Get_Low_Byte(Crc_a[Xk])==Get_High_Byte(Crc_dnp)&&Get_High_Byte(Crc_a[Xk])==Get_Low_Byte(Crc_dnp))
                      {
                        Ij++;
                      }
                  }
                if(Ij==Lt)
                  {
                    Last_t_sequence_rcv=T_sequence_rcv;
                    A_rcv_flag=1;
                  }
              }
          }
      }
  }

ENDPROCESS:;
if(A_rcv_flag==0)
  {
    Error_recive=0;
  }
}
//********************************************************************
void FILL_E2PROM_EVENT()
{
Y=Start_event_buf[Class]+Counter_event[Class];
Y--;
for(W=Last_event_start[Class]; W<=Y; W+=1)
  {
    E_event_data[W]=Event_data[W];
  }
for(W=1; W<=Last_event_num[Class]; W+=1)
  {
    Y=Start_event_num[Class]+W;
    Y--;
    E_event_type[Y]=Event_type[Y];
  }
Last_event_start[Class]=Start_event_buf[Class]+Counter_event[Class];
E_counter_event[Class]=Counter_event[Class];
E_last_event_start[Class]=Last_event_start[Class];
E_event_num[Class]=Last_event_num[Class];
E_last_event_num[Class]=Last_event_num[Class];
for(H=1; H<=4; H+=1)
  {
    E_event_num_gr[H]=Event_num_gr[H];
  }
}
//*************************************************************************
void PROCESS_CONTROL_RELAY_FUNCTION()
	{
Error_control_relay_request_flag=1;
Ss=0;
Jj=0;
Jjj=0;
for(Xx=1; Xx<=Quantity[Main_request_gr_count]; Xx+=1)
  {
    for(Jj=1; Jj<=3; Jj+=1)
      {
        if(Output_flag[Jj]==0)
          {
            Ss=Jj;
            Statuss[Ss]=0;
            break;
          }
        else
          {
            if(Jj==3)
              {
                Statuss[Ss]=5;
                goto ERROR_OUTPUT;
              }
          }
      }
    if(Statuss[Ss]!=0)
      {
        goto ERROR_OUTPUT;
      }
    I++;
    if(Qualifier[Main_request_gr_count]==23)
      {
        Rcv_index[Ss]=Recieve_data[I];
      }
    else
      {
      Rcv_index[Ss]=makeint(Recieve_data[I],Recieve_data[I+1]);
        I++;
      }
    if(Chk_bit(&Recieve_data[I+1],4)!=0 || Chk_bit(&Recieve_data[I+1],5)!=0)
      {
        Statuss[Ss]=4;
        goto ERROR_OUTPUT;
      }
    else
      {
        Do_code[Ss]=Recieve_data[I+1] & 15;
	      Do_Type[Ss]=Recieve_data[I+1] & 0xF0;
      }
    for(Jj=1; Jj<=3; Jj+=1)
      {
        if(Rcv_index[Ss]==Execute_index[Jj]&&Output_flag[Jj]==1)
          {
            if(Do_code[Ss]==1||Do_code[Ss]==2)
              {
                Statuss[Ss]=5;
                goto ERROR_OUTPUT;
              }
          }
      }
    if(Recieve_data[I+2]>1)
      {
        Statuss[Ss]=4;
        goto ERROR_OUTPUT;
      }
    Tempb_l1=Recieve_data[I+3];
    Tempb_h1=Recieve_data[I+4];
    Time_on=makeint(Tempb_l1,Tempb_h1);
		Tempb_l1=0;
    Tempb_h1=0;
    Time_off=makeint(Tempb_l1,Tempb_h1);
    if(Do_code[Ss]==1)
      {
        Tl=Time_on;
      }
    else if(Do_code[Ss]==2)
      {
        Tl=Time_off;
      }
    if(Tl<=65000)
      {
        Time_on_off[Ss]=Tl;
      }
    else
      {
        Statuss[Ss]=4;
        goto ERROR_OUTPUT;
      }
    if(A_function_code==3)
      {
        Select_index[Ss]=Rcv_index[Ss];
        Sbo_delay_time=1;
      }
    else if(A_function_code==4)
      {
        if (Sbo_delay_time ==1)
					Sbo_delay_time=0;
				else if (Sbo_time_out_flag==0)
					goto DEF;
				else
					goto ABC;

        for(Jj=1; Jj<=6; Jj+=1)
          {
            if(Rcv_index[Ss]==Select_index[Jj])
              {
                goto ABC;
              }
          }
DEF:
        Select_index_count=0;
        Statuss[Ss]=2;
        goto ERROR_OUTPUT;

ABC:;
        if(Sbo_time_out_flag==1)
          {
            Sbo_time_out_flag=0;
            Statuss[Ss]=1;
            goto ERROR_OUTPUT;
          }
      }

ERROR_OUTPUT:;
    if(Statuss[Ss]==0)
      {
        if(A_function_code!=3)
          {
            Output_flag[Ss]=1;
						Output_time[Ss]=0;
            Index_output_flag[Ss]=Rcv_index[Ss];
          }
        else
          {
            Select_index_count=Xx;
          }
      }
    else
      {
        Error_control_relay_request_flag=0;
        Output_flag[Ss]=0;
      }
    Jjj++;
    Array_temp[Jjj]=Ss;
    I+=11;
  }
	Recieve_data[I+7]=0;
	Recieve_data[I+8]=0;
	Recieve_data[I+9]=0;
	Recieve_data[I+10]=0;
	Recieve_data[I+11]=0;
}
//*****************************************************************
void FILL_EVENTS()
{
Y=Start_event_buf[Class]+Jj;
Yy=Y+T_one_event_size;
Yy--;
Jj+=T_one_event_size;
for(Yyy=Y; Yyy<=Yy; Yyy+=1)
  {
    Data_num++;
    Temp_crc[Data_num]=Event_data[Yyy];
    Xx=Y+4;
    if(Yyy==Xx)
      {
        if(Send_event_type[H]==a16_bit_analog_chang_no_time||Send_event_type[H]==a16_bit_analog_chang_with_time)
          {
            Yyy+=2;
          }
      }
  }
}
//******************************************************************
void Write_response (void)
{

	BYTE *p=(BYTE *)&Base_time;

  for(K=1; K<=Main_request_gr_count; K+=1)
    {
      for(W=1; W<=Process_data_count; W+=1)
        {
          for(;;)
          {
            if(Static_data_type[K]==1)
              {
                Change_bit(&Iin,7,Chk_bit_16(&Process_recieve_data[W],0));
                break;
              }
            if(Static_data_type[K]==2)
              {
                //Base_time=0;
                //Base_time1=0;
								_TimeDateSet=1;
                Clr_bit(&Iin,4);
                *p++=Process_recieve_data[W];
                *p++=Process_recieve_data[W+1];
                *p++=Process_recieve_data[W+2];
                *p++=Process_recieve_data[W+3];
                *p++=Process_recieve_data[W+4];
                *p++=Process_recieve_data[W+5];
                W=Process_data_count+6;
               // E_base_time=Base_time;
              //  E_base_time1=Base_time1;
								//_HMISetTime = 100;
								_DnpSetTime = 1;
                break;
              }
            if(Static_data_type[K]==3)
              {
                for(Ww=Start_Bit[K]; Ww<=Stop_Bit[K]; Ww+=1)
                  {
                    W++;
                    if(Ww<=255)
                      {
                        Jj=Ww/16;
                        Gg=St_add_do_plc_index+Jj;
                      }
                    else
                      {
                        Jj=Ww-256;
                        Jj/=16;
                        Gg=St_add_soft_dio_plc+Jj;
                      }
                    Jj*=16;
                    Jj=Ww-Jj;
                    Change_bit(&Main_buffer[Gg],Jj,Chk_bit_16(&Process_recieve_data[W],0));
                  }
              }
            break;
          }
        }
    }
}


void Freeze_response (void)
{
  for(K=1; K<=Main_request_gr_count; K+=1)
    {
      for(H=1; H<=Process_data_count; H+=1)
        {
          for(W=Start_index_table[H]; W<=Stop_index_table[H]; W+=1)
            {
              Main_buffer[St_add_freeze_cnt_plc+W]=Main_buffer[St_add_cnt_plc+W];
              E_frozen_main_buffer[W+1]=Main_buffer[St_add_freeze_cnt_plc+W];
              if(A_function_code==9||A_function_code==10)
                {
                  Main_buffer[St_add_cnt_plc+W]=0;
                }
            }
        }
    }
}


void Control_relay (void)
{
  Ss=0;
  for(;;)
  {
		//Static_data_type[Main_request_gr_count]=1;
    if(Static_data_type[Main_request_gr_count]==1)
      {
        if(Error_control_relay_request_flag==1)
          {
            Error_control_relay_request_flag=0;
            for(H=1; H<=3; H+=1)
              {
                if(Output_flag[H]==1)
                  {
                    Output_flag[H]=0;
                    if(Rcv_index[H]<=255)
                      {
                        Jj=Rcv_index[H];
                        Gg=St_add_do_plc_index+Jj;
                      }
                    else
                      {
                        Jj=Rcv_index[H]-256;
                        Jj/=16;
                        Gg=St_add_soft_dio_plc+Jj;
                      }
                    Jj*=16;
                    Jj=Rcv_index[H]-Jj;
                    Main_buffer[Gg]=Outoff;
//                    if(Do_code[H]==1||Do_code[H]==3)
//                      {
//                        Main_buffer[Gg]=Outon;
//												write_ok=100;
//                      }
//                    if(Do_code[H]==1||Do_code[H]==2)
//                      {
//                        Output_flag[H]=1;
//												Output_time[H]=0;
//                        Execute_index[H]=Rcv_index[H];
//                        Output_buffer[H]=Gg;
//                        Output_index[H]=Jj;
//                      }

									if (Do_Type[H]==0 && Rcv_index[H] != 0 ){
										if(Do_code[H]==1||Do_code[H]==3)										{
											Main_buffer[Gg]=Outon;
											write_ok=100;
										}
										if(Do_code[H]==1||Do_code[H]==2)										{
											Output_flag[H]=1;
											Execute_index[H]=Rcv_index[H];
											Output_buffer[H]=Gg;
											Output_index[H]=Jj;
										}
									}
									else if (Do_Type[H] == 0x80){// && (Rcv_index[H] >= StartDnpDO && Rcv_index[H] <= StopDnpDO)){
										Main_buffer[Gg]=TripOutOn;
										write_ok=100;
										Output_flag[H]=1;
										Execute_index[H]=Rcv_index[H];
										Output_buffer[H]=Gg;
										Output_index[H]=Jj;
									}
									else if (Do_Type[H] == 0x40){// && (Rcv_index[H] >= StartDnpDO && Rcv_index[H] <= StopDnpDO)){//(Rcv_index[H] == 0 || Rcv_index[H] == 1 || Rcv_index[H] == 2)){
										Main_buffer[Gg]=CloseOutoff;
										write_ok=100;
										Output_flag[H]=1;
										Execute_index[H]=Rcv_index[H];
										Output_buffer[H]=Gg;
										Output_index[H]=Jj;
									}





                  }


              }
          }
        break;
      }
//    if(Static_data_type[Main_request_gr_count]==2)
//      {
//        for(Xx=1; Xx<=Quantity[Main_request_gr_count]; Xx+=1)
//          {
//            W=Rcv_index[Xx];
//            if(W<=255)
//              {
//                W+=St_add_ao_plc;
//              }
//            else
//              {
//                W-=256;
//                W+=St_add_soft_aio_plc;
//              }
//            if(Variation[Main_request_gr_count]==2)
//              {
//                Main_buffer[W]=Ao_process_recieve_data[Xx];
//              }
//            else if(Variation[Main_request_gr_count]==1)
//              {
//                Main_buffer[W+1]=Get_Low_Word(Ao_process_recieve_data[Xx]);
//                Main_buffer[W]=Get_High_Word(Ao_process_recieve_data[Xx]);
//              }
//          }
//					write_ok=100;
//      }
  if(Static_data_type[Main_request_gr_count]==2){
	//		for(Xx=1; Xx<Quantity[Main_request_gr_count]; Xx++)			{
			Xx=1;
			W=Rcv_index[Xx];
			if(W<=255)				{
				W+=St_add_ao_plc;
			}
			else				{
				W-=256;
				W+=St_add_soft_aio_plc;
			}
			//if(Variation[Main_request_gr_count]==1)				{
			//if(_Local_Remote == Remote)   //sajedi cpu

				Main_buffer[W]=Ao_process_recieve_data[Xx];
			//}
			//	else if(Variation[Main_request_gr_count]==0)				{
//					Main_buffer[W+1]=Get_Low_Word(Ao_process_recieve_data[Xx]);
//					Main_buffer[W]=Get_High_Word(Ao_process_recieve_data[Xx]);
//				}
	//		}
			write_ok=100;
		}
    break;
  }
  if(A_function_code!=6)
    {
      Data_num=0;
      S=0;
      if(Qualifier[Main_request_gr_count]==23)
        {
          H=4;
        }
      else
        {
          H=5;
        }
      W=H+One_control_block_size;
      for(I=1; I<=Rcv_data_count; I+=1)
        {
          Data_num++;
          if(I==W)
            {
              S++;
              W=I+One_control_block_size;
              Temp_crc[Data_num]=Statuss[Array_temp[S]];
            }
          else
            {
              Temp_crc[Data_num]=Recieve_data[I];
            }
        }
    }
}


void En_dis_event (void)
{
  for(Class=1; Class<=3; Class+=1)
    {
      if(Enable_event_flag[Class]==1)
        {
          if(A_function_code==20)
            {
              Enable_event_pc[Class]=1;
            }
          else
            {
              Enable_event_pc[Class]=0;
            }
        }
    }
  Send_application_response_flag=1;
}


void Make_header (void)
{
  Get_crc[1]=5;
  Get_crc[2]=100;
  Get_crc[3]=Send_length;
  Get_crc[4]=D_function;
  if(Unsol_add_flag==1)
    {
      Unsol_add_flag=0;
      Get_crc[5]=Get_Low_Byte(Unsol_Address);
      Get_crc[6]=Get_High_Byte(Unsol_Address);
    }
  else
    {
      Get_crc[5]=Get_Low_Byte(Destadd);
      Get_crc[6]=Get_High_Byte(Destadd);
    }
		Get_crc[7]=Get_Low_Byte(_Slaveaddress);
		Get_crc[8]=Get_High_Byte(_Slaveaddress);
		Crc_dnp=Crc_calc(1,8);
		//Crc_dnp=CRC_calculator(&Get_crc[1],8);
		Get_crc[9]=Get_Low_Byte(Crc_dnp);
		Get_crc[10]=Get_High_Byte(Crc_dnp);
		Send_dest_add=makeint(Get_crc[5],Get_crc[6]);
}


void Send_datalink_response (void)
{
  if(Send_reset_link==1)
    {
      D_function=64;
    }
  else if(D_function_code==211||D_function_code==243)
    {
      D_function=0;
    }
  else if(D_function_code==192)
    {
      Fcb=0;
      D_function=0;
      Clr_bit (&Iin, 8);
			Clr_bit (&Iin, 9);
			Clr_bit (&Iin, 10);

    }
  else if(D_function_code==193||D_function_code==225)
    {
      D_function=159;
    }
  else if(D_function_code==0)
    {
      D_function=128;
    }
  else
      goto LAA;
  Send_length=5;
  Make_header();
  D_send_flag=1;
  D_time_out_flag=0;
  Array_len1=10;
  Array_counter1=1;
  //Chip_UART_SendByte(LPC_UART0,Get_crc[1]);//UART0_SendByte(Get_crc[1]);
  //HAL_UART_Transmit_DMA(&huart3,&Get_crc[1],Array_len1);
	DNP3_Transmit(Array_len1);

  Data_count=1;
  Start_count=0;
  //led2(1);
  LAA:;
}



//*******************************************************************
void Process_rcv_function (void)
{
	  uint8_t i;
		char str[3],str1[3],str2[3];
    Process_data_count=0;
    Main_request_gr_count=0;
    for(I=1; I<=Rcv_data_count; I+=1)
      {
        Main_request_gr_count++;
        Group[Main_request_gr_count]=Recieve_data[I];
        Variation[Main_request_gr_count]=Recieve_data[I+1];
        Qualifier[Main_request_gr_count]=Recieve_data[I+2];
        I+=2;
        *String_data=0;
				 //for(i=0;i<=13;i++)String_data[i] = 0X20;
      //   for(i=0;i<=256;i++)Search[i] = 0X20;
				sprintf(str, "%d", A_function_code);
				sprintf(str1, "%d", Group[Main_request_gr_count]);
				sprintf(str2, "%d", Variation[Main_request_gr_count]);
 				strcat(String_data,"F");
				strcat(String_data,str);
				strcat(String_data,"G");
				strcat(String_data,str1);
				strcat(String_data,"V");
				strcat(String_data,str2);


       for(S=0; S<=35; S+=1)
          {

           strcpy(Search,*(S+Database));
					 W= (DWORD) strstr(Search,String_data);
           //W-=(int)Search;

            if(W!=0)//
              {

							sprintf(str, "%d", Qualifier[Main_request_gr_count]);
							strcat((char *)String_data,"Q");
							strcat((char *)String_data,str);
	  					 if(Group[Main_request_gr_count]==80&&Variation[Main_request_gr_count]==1)
                  {
									strcat((char *)String_data,"0707");
                  }
                if(Group[Main_request_gr_count]==50&&Variation[Main_request_gr_count]==1)
                  {
									strcat((char *)String_data,"01");
                  }
//								for(i=0;i<=256;i++)Search[i] = 0X20;
//                *Search=0;

                for(K=S; K<=35; K+=1)
                  {

					strcpy(Search,*(K+Database));

                    W= (DWORD)strstr(Search,String_data);


                    if(W!=0) W-=(int)Search;   //Najafi

					if (W==0)	W++;

                    if(W!=0)
                      {

                    		if (W==1) W--;

                    		Y=W+strlen((char *)String_data);
//											 for(i=0;i<=13;i++)String_data[i] = 0X20;
							*String_data= 0;

							strcpy((char *)String_data,(char const *)mid(Search,Y,2));


                       if(String_data[0] != 0X2F && String_data[1] != 0X2F )  //(String_data != "//")		strcmp((char *)String_data,"//")!=0
                          {

														Need_search_table_flag[Main_request_gr_count]=1;
                            Start_gr_class0_table[Main_request_gr_count]=atoi(String_data);
														if (String_data[0] == 0x30 && String_data[1] == 0x30)Start_gr_class0_table[Main_request_gr_count]=0;
                            Y+=2;
                            strcpy((char *)String_data,(char const *)mid(Search,Y,2));
                            Repeat_gr_class0_table[Main_request_gr_count]=atoi(String_data);
                            Request_data[Main_request_gr_count]=Static;
                          }
                        else
                          {

                            Need_search_table_flag[Main_request_gr_count]=0;
                            Y+=2;
                            strcpy((char *)String_data,(char const *)mid(Search,Y,1));
                            if((String_data[0] == 0X54 )||(String_data[0] == 0X43 ))//"T" "C"
                              {
                                Request_data[Main_request_gr_count]=Event;
                                if(String_data[0] == 0X54)
                                  {
                                    Y++;
                                    strcpy((char *)String_data,(char const *)mid(Search,Y,2));
																				Send_event_type[Main_request_gr_count]=atoi(String_data);
                                    if(Variation[Main_request_gr_count]==0)
                                      {
                                        Send_event_type[Main_request_gr_count]=Default_send_event_type[Send_event_type[Main_request_gr_count]];
                                        Group[Main_request_gr_count]=Event_gr[Send_event_type[Main_request_gr_count]];
                                        Variation[Main_request_gr_count]=Event_var[Send_event_type[Main_request_gr_count]];
                                      }
                                    if(Qualifier[Main_request_gr_count]==6)
                                      {
                                        Quantity[Main_request_gr_count]=Event_num_gr[Save_event_type[Send_event_type[Main_request_gr_count]]];
                                        Qualifier[Main_request_gr_count]=40;
                                      }
                                  }
                                else if(String_data[0] == 0X43)
                                  {
                                    Class=Variation[Main_request_gr_count]-1;
                                    if(Qualifier[Main_request_gr_count]==6)
                                      {
                                        Quantity[Main_request_gr_count]=Event_num[Class];
                                      }
                                  }
                              }
                            else if(String_data[0] == 0X4E)
                              {
                                Y++;

																strcpy((char *)String_data,(char const *)mid(Search,Y,2));
																One_control_block_size=atoi(String_data);//String_data
                                Y+=2;
																strcpy((char *)String_data,(char const *)mid(Search,Y,1));
																/*if (!strncmp((char const *)String_data,(char const *) "1",1))
																	Static_data_type[Main_request_gr_count]=atoi("1");//String_data
																else	*/
																Static_data_type[Main_request_gr_count]=atoi(String_data);//String_data
                                Y++;
																strcpy((char *)String_data,(char const *)mid(Search,Y,1));
																//memcpy((char *)String_data,(char const *)mid(Search,Y,2),2);
/*																if (!strncmp((char const *)String_data,(char const *) "1",1))
																	Start_gr_class0_table[Main_request_gr_count]=atoi("1");//String_data
																else*/
																	Start_gr_class0_table[Main_request_gr_count]=atoi(String_data);//String_data
                                Request_data[Main_request_gr_count]=Static;
                              }
                            else
                              {
                                Request_data[Main_request_gr_count]=Static;
                              }
                          }
                        break;
                      }
                    else if(W==0&&K==35)
                      {
                        Data_num=0;
												Set_bit (&Iin, 10);
                        goto _END;
                      }
                  }
                break;
              }
            else if(W==0&&S==35)
              {
                Data_num=0;
								Set_bit (&Iin, 9);
                goto _END;
              }
          }


        for(;;)
        {
          if(Qualifier[Main_request_gr_count]==0)
            {
              Start_Bit[Main_request_gr_count]=Recieve_data[I+1];
              Stop_Bit[Main_request_gr_count]=Recieve_data[I+2];
              I+=2;
              break;
            }
          if(Qualifier[Main_request_gr_count]==1)
            {
              I++;
            Start_Bit[Main_request_gr_count]=makeint(Recieve_data[I],Recieve_data[I+1]);
            Stop_Bit[Main_request_gr_count]=makeint(Recieve_data[I+2],Recieve_data[I+3]);
              I+=3;
              break;
            }
          if(Qualifier[Main_request_gr_count]==6)
            {
              Jj=Variation[Main_request_gr_count]-1;
              if(A_function_code==20||A_function_code==21)
                {
                  Enable_event_flag[Jj]=1;
                }
              break;
            }
          if(Qualifier[Main_request_gr_count]==7)
            {
              I++;
              Quantity[Main_request_gr_count]=Recieve_data[I];
              break;
            }
          if(Qualifier[Main_request_gr_count]==8)
            {
              Quantity[Main_request_gr_count]=makeint(Recieve_data[I+1],Recieve_data[I+2]);
              I+=2;
              break;
            }
          if(Qualifier[Main_request_gr_count]==23)
            {
              I++;
              Quantity[Main_request_gr_count]=Recieve_data[I];
              break;
            }
          if(Qualifier[Main_request_gr_count]==40)
            {
              if(Request_data[Main_request_gr_count]!=Event)
                {
                Quantity[Main_request_gr_count]=makeint(Recieve_data[I+1],Recieve_data[I+2]);
                  I+=2;
                }
            }
          break;
        }
        if(Request_data[Main_request_gr_count]==Event)
          {
            if(Group[Main_request_gr_count]!=60&&Quantity[Main_request_gr_count]>Event_num_gr[Save_event_type[Send_event_type[Main_request_gr_count]]])
              {
								Set_bit (&Iin, 10);
                Data_num=0;
                goto _END;
              }
            else if(Group[Main_request_gr_count]==60&&Quantity[Main_request_gr_count]>Event_num[Class])
              {
								Set_bit (&Iin, 10);
                Data_num=0;
                goto _END;
              }
            else if(Group[Main_request_gr_count]==60)
              {
                Qualifier[Main_request_gr_count]=40;
              }
          }
        for(;;)
        {
          if(A_function_code==2)
            {
              for(;;)
              {
                if(Group[Main_request_gr_count]==80)
                  {
                    Static_data_type[Main_request_gr_count]=1;
                    I++;
                    Process_data_count++;
                    Process_recieve_data[Process_data_count]=Recieve_data[I];
                    break;
                  }
                if(Group[Main_request_gr_count]==50)
                  {
                    Static_data_type[Main_request_gr_count]=2;
                    for(S=1; S<=6; S+=1)
                      {
                        Process_data_count++;
                        I++;
                        Process_recieve_data[Process_data_count]=Recieve_data[I];
                      }
                    break;
                  }
                if(Group[Main_request_gr_count]==10)
                  {
                    Static_data_type[Main_request_gr_count]=3;
                    for(S=Start_Bit[Main_request_gr_count]; S<=Stop_Bit[Main_request_gr_count]; S+=1)
                      {
                        Process_data_count++;
                        I++;
                        Process_recieve_data[Process_data_count]=Recieve_data[I];
                      }
                  }
                break;
              }
              break;
            }
          if(A_function_code>=3&&A_function_code<=6)
            {
              if(Quantity[Main_request_gr_count]>3)
                {
									Set_bit (&Iin, 10);
                  Data_num=0;

                  goto _END;
                }
              Need_search_table_flag[Main_request_gr_count]=1;
              Repeat_gr_class0_table[Main_request_gr_count]=1;
              if(Static_data_type[Main_request_gr_count]==1)
                {
                  Control_block_rcv_data=One_control_block_size*Quantity[Main_request_gr_count];
                 PROCESS_CONTROL_RELAY_FUNCTION();
                }
              else
                {
                  Ss=0;
                  for(Xx=1; Xx<=Quantity[Main_request_gr_count]; Xx+=1)
                    {
                      Ss++;
                      I++;
                      if(Qualifier[Main_request_gr_count]==23)
                        {
                          Rcv_index[Ss]=Recieve_data[I];
                          I++;
                        }
                      else
                        {
                        Rcv_index[Ss]=makeint(Recieve_data[I],Recieve_data[I+1]);
                          I+=2;
                        }
                      if(Variation[Main_request_gr_count]==1)
                        {
                          Process_recieve_data[Ss]=makeint(Recieve_data[I],Recieve_data[I+1]);
                          Ao_process_recieve_data[Ss]=Process_recieve_data[Ss]*0X10000;
                          Process_recieve_data[Ss]=makeint(Recieve_data[I+2],Recieve_data[I+3]);
                          Ao_process_recieve_data[Ss]=Ao_process_recieve_data[Ss]+Process_recieve_data[Ss];
                          I+=4;
                        }
                      else if(Variation[Main_request_gr_count]==2)
                        {
                          Ao_process_recieve_data[Ss]=makeint(Recieve_data[I],Recieve_data[I+1]);
                          I+=2;
                        }
                      Statuss[Ss]=Recieve_data[I];
                    }
                }
            }
          break;
        }
      }

    for(H=1; H<=Main_request_gr_count; H+=1)
      {
        if(Need_search_table_flag[H]==1)
          {
            Need_search_table_flag[H]=0;
            Last_count_class0_table=0;
            Process_data_count=0;
            Jj=0;
            Jjj=0;
            Ww=Start_gr_class0_table[H];
            for(K=1; K<=Repeat_gr_class0_table[H]; K+=1)
              {
                Ww++;
                if(Ww==3)
                  {
                  goto _DO;
                  }
                Yy=0;
                Last_count_class0_table=0;
                for(Y=1; Y<=Ww; Y+=1)
                  {
                    Count_class0_table=Class0[Yy];
                    Start_table=Yy;
                    W=Count_class0_table*2;
                    W+=1;
                    Last_count_class0_table=W+Last_count_class0_table;
                    Yy=Last_count_class0_table;
                  }
                Count_gr_class0_table[Ww]=Count_class0_table;
                if(Count_gr_class0_table[Ww]>0)
                  {
                    for(X=1; X<=Count_class0_table; X+=1)
                      {
                        Process_data_count++;
                        Start_table++;
                        Start_index_table[Process_data_count]=Class0[Start_table];
                        Start_table++;
                        Stop_index_table[Process_data_count]=Class0[Start_table];
                        Stop_index_table[Process_data_count]=Start_index_table[Process_data_count]+Stop_index_table[Process_data_count];
                        if(Stop_index_table[Process_data_count]>1)
                          {
                            Stop_index_table[Process_data_count]=Stop_index_table[Process_data_count]-1;
                          }
                        if(Qualifier[H]==0||Qualifier[H]==1)
                          {
                            if(Start_index_table[Process_data_count]<=Start_Bit[H]&&Stop_index_table[Process_data_count]>=Stop_Bit[H])
                              {
                                if(T_addr_stop[Start_gr_class0_table[H]+1]>=Start_Bit[H]&&T_addr_stop[Start_gr_class0_table[H]+1]>=Stop_Bit[H])
                                  {
                                    Start_index_table[H]=Start_Bit[H];
                                    Stop_index_table[H]=Stop_Bit[H];
                                    Count_gr_class0_table[Ww]=1;
                                    Jj++;
                                    goto _DO;
                                  }
                                else
                                  {
																		Set_bit (&Iin, 10);
                                    Data_num=0;
                                  goto _END;
                                  }
                              }
                            else
                              {
                                Count_gr_class0_table[Ww]=0;
                                Process_data_count--;
                              }
                          }
                      }
                  }

_DO:;
              }
            if(Qualifier[H]==23||Qualifier[H]==40)
              {
                if(A_function_code>=3&&A_function_code<=6)
                  {
                    for(Xx=1; Xx<=Quantity[H]; Xx+=1)
                      {
                        for(Yyy=1; Yyy<=Process_data_count; Yyy+=1)
                          {
                            if(Start_index_table[Yyy]<=Rcv_index[Xx]&&Rcv_index[Xx]<=Stop_index_table[Yyy])
                              {
                                Jj++;
                                if(Xx==Quantity[H])
                                  {
                                   goto EXITT;
                                  }
                                break;
                              }
                          }
                      }
                  }
              }

EXITT:;
            if(Qualifier[H]!=6&& Jj==0)
            {
                if(A_function_code>=3&&A_function_code<=6)
                  {
                    for(Y=1; Y<=Quantity[H]; Y+=1)
                      {
                        Output_flag[Array_temp[Y]]=0;
                      }
                  }
								Set_bit (&Iin, 10);
                Data_num=0;
                goto _END;
              }
            else if(Qualifier[H]==6)
              {
                Qualifier[H]=1;
              }
          }
      }
    Data_num=0;
    for(;;)
    {
      if(A_function_code>=3&&A_function_code<=6)
      {
        Control_relay();
        break;
      }
      if(A_function_code==1)
        {
					Fill_data();
					break;
        }
      if(A_function_code>=7&&A_function_code<=10)
      {
        Freeze_response();
        break;
      }
      if(A_function_code==2)
        {
					write_ok=100;
          Write_response();
          break;
        }
      if(A_function_code>=20&&A_function_code<=21)
        {
          En_dis_event();

          break;
        }
         else
        {
          if(A_function_code!=0)
            {
							Set_bit (&Iin, 8);
            }
        }
      break;
    }
    Enable_event_flag[1]=0;
    Enable_event_flag[2]=0;
    Enable_event_flag[3]=0;

_END:
    Send_application_response_flag=1;
    if(A_function_code==6||A_function_code==8||A_function_code==10)
      {
        Send_application_response_flag=0;
      }
  }




void Fill_data (void)
{
  for(H=1; H<=Main_request_gr_count; H+=1)
    {
      if(Request_data[H]==Event)
        {
          if(Group[H]!=60&&Event_num_gr[Save_event_type[Send_event_type[H]]]>0)
            {
              Temp_crc[Data_num+1]=Group[H];
              Temp_crc[Data_num+2]=Variation[H];
              Temp_crc[Data_num+3]=Qualifier[H];
              X=Data_num+4;
              Temp_crc[X]=0;
              Temp_crc[Data_num+5]=0;
              Data_num+=5;
              T_one_event_size=One_event_size[Send_event_type[H]];
              T_extra_one_event_size=Extra_one_event_size[Send_event_type[H]];
              for(Class=1; Class<=3; Class+=1)
                {
                  Jj=0;
                  for(S=1; S<=Event_num[Class]; S+=1)
                    {
                      Tempb=Start_event_num[Class]+S;
                      Tempb--;
                      if(Event_type[Tempb]==Save_event_type[Send_event_type[H]])
                        {
                       FILL_EVENTS();
                        }
                      else
                        {
                          Jj=Jj+Save_one_event_size[Event_type[Tempb]];
                        }
                      if(Temp_crc[X]==Quantity[H])
                        {
                       goto END1;
                        }
                    }
                }
            }
          else
            {
              Last_event_type=0;
              Jj=0;
              Class=Variation[H]-1;
              Class_flag[Class]=1;
              Send_quantity[Class]=Quantity[H];
              Jjj=Event_num[Class]-Send_quantity[Class];
              if(Jjj==0)
                {

									Clr_bit (&Iin, Class);
                }
              if(Event_num[Class]>0)
                {
                  for(S=1; S<=Quantity[H]; S+=1)
                    {
                      Tempb=Start_event_num[Class]+S;
                      Tempb--;
                      if(Last_event_type!=Event_type[Tempb])
                        {
                          Last_event_type=Event_type[Tempb];
                          Send_event_type[H]=Default_send_event_type[Event_type[Tempb]];
                          T_one_event_size=One_event_size[Send_event_type[H]];
                          T_extra_one_event_size=Extra_one_event_size[Send_event_type[H]];
                          Temp_crc[Data_num+1]=Event_gr[Send_event_type[H]];
                          Temp_crc[Data_num+2]=Event_var[Send_event_type[H]];
                          Temp_crc[Data_num+3]=Qualifier[H];
                          X=Data_num+4;
                          Temp_crc[X]=0;
                          Temp_crc[Data_num+5]=0;
                          Data_num+=5;
                        }
                      Temp_crc[X]++;
                      FILL_EVENTS();
                    }
                }
            }

END1:;
        }
      else
        {
          if(Group[H]==50)
            {
              Temp_crc[Data_num+1]=Group[H];
              Temp_crc[Data_num+2]=Variation[H];
              Temp_crc[Data_num+3]=Qualifier[H];
              Temp_crc[Data_num+4]=1;

              BYTE *pBase=(BYTE *)&Base_time;

              Temp_crc[Data_num+5]=*pBase++;   // najafi - read time
              Temp_crc[Data_num+6]=*pBase++;
              Temp_crc[Data_num+7]=*pBase++;
              Temp_crc[Data_num+8]=*pBase++;
              Temp_crc[Data_num+9]=*pBase++;
//              if(Tempc_h1>1)
//                {
//                  Tempc_h1=1;
//                }
              Temp_crc[Data_num+10]=*pBase++;
              Data_num+=10;
            }
          else
            {
              W=0;
              Repeat_gr_class0_table[H]=Start_gr_class0_table[H]+Repeat_gr_class0_table[H];
              Start_gr_class0_table[H]++;
              for(Y=Start_gr_class0_table[H]; Y<=Repeat_gr_class0_table[H]; Y+=1)
                {
                  for(Yy=1; Yy<=Count_gr_class0_table[Y]; Yy+=1)
                    {
                      W++;
                      Temp_crc[Data_num+1]=T_group[Y];
                      Temp_crc[Data_num+2]=T_variation[Y];
                      Temp_crc[Data_num+3]=Qualifier[H];
                      Data_num+=4;
                      for(;;)
                      {
                        if(Qualifier[H]==0)
                          {
                         Temp_crc[Data_num]=Get_Low_Byte(Start_index_table[W]);
                            Data_num++;
                        Temp_crc[Data_num]=Get_Low_Byte(Stop_index_table[W]);
                            break;
                          }
                        if(Qualifier[H]==1)
                          {
                            Temp_crc[Data_num]=Get_Low_Byte(Start_index_table[W]);
                            Temp_crc[Data_num+1]=Get_High_Byte(Start_index_table[W]);
                            Temp_crc[Data_num+2]=Get_Low_Byte(Stop_index_table[W]);
                            Temp_crc[Data_num+3]=Get_High_Byte(Stop_index_table[W]);
                            Data_num+=3;
                          }
                        break;
                      }
                      for(;;)
                      {
                        if(Y>=1&&Y<=4)
                          {
                            Ss=0;
                            for(X=Start_index_table[W]; X<=Stop_index_table[W]; X+=1)
                              {
                                if(X<=255)
                                  {
                                    Jj=X/16;
                                    Gg=T_addr[Y]+Jj;
                                  }
                                else
                                  {
                                    Jj=X-256;
                                    Jj/=16;
                                    Gg=St_add_soft_dio_plc+Jj;
                                  }
                                Jj*=16;
                                Jj=X-Jj;
                                if(Y==1)
                                  {
                                    Ss=Ss % 8;
                                    if(Ss==0)
                                      {
                                        Data_num++;
                                        Temp_crc[Data_num]=0;
                                      }
 																			Change_bit_8 (&Temp_crc[Data_num],Ss,Chk_bit_16 (&Main_buffer[Gg],Jj));
                                    Ss++;
                                  }
                                else if(Y==2 )//|| Y==4
                                  {
                                    Data_num++;
                                    Temp_crc[Data_num]=1;
																		Change_bit_8 (&Temp_crc[Data_num],7,Chk_bit_16 (&Main_buffer[Gg],Jj));
                                  }
																else if (Y==4)
																  {
                                    Data_num++;
                                    Temp_crc[Data_num]=1;
																		if (Main_buffer[T_addr[Y]+X]==Outon)
																			Change_bit_8 (&Temp_crc[Data_num],7,1);//Chk_bit_16 (&Main_buffer[Gg],Jj)
																		else
																			Change_bit_8 (&Temp_crc[Data_num],7,0);//Chk_bit_16 (&Main_buffer[Gg],Jj)
                                  }
                              }
                            break;
                          }
                        if(Y>=5&&Y<=20)
                          {
                            for(X=Start_index_table[W]; X<=Stop_index_table[W]; X+=1)
                              {
                                if(X<=255)
                                  {
                                    Gg=X+T_addr[Y];
                                  }
                                else
                                  {
                                    Gg=X-256;
                                    Gg+=St_add_soft_aio_plc;
                                  }
                                if(Y!=11&&Y!=12)
                                  {
                                    Data_num++;
                                    if(Y==5||Y==6||Y==9||Y==10||Y==13||Y==14||Y==17||Y==18)
                                    {
                                        Temp_crc[Data_num]=1;//Analogflag_value[Gg-St_add_ai_plc]; //FLAG_ANALOG
                                        Data_num++;
                                    }
                                  Temp_crc[Data_num]=Get_Low_Byte(Main_buffer[Gg]);
                                    Data_num++;
                                 Temp_crc[Data_num]=Get_High_Byte(Main_buffer[Gg]);
                                    Jj=Y%2;
                                    if(Jj!=0)
                                      {
                                        Data_num++;
                                      Temp_crc[Data_num]=Get_Low_Byte(Main_buffer[Gg+1]);
                                        Data_num++;
                                     Temp_crc[Data_num]=Get_High_Byte(Main_buffer[Gg+1]);
                                      }
                                  }
                                else
                                  {
                                    Temp_crc[Data_num+1]=1;
                                    Temp_crc[Data_num+2]=Get_Low_Byte(Main_buffer[Gg]);
                                   Temp_crc[Data_num+3]=Get_High_Byte(Main_buffer[Gg]);
                                   Temp_crc[Data_num+4]=Get_Low_Byte(Main_buffer[Gg+1]);
                                    Temp_crc[Data_num+5]=Get_High_Byte(Main_buffer[Gg+1]);
                                    Data_num+=5;
                                  }
                              }
                          }
                        break;
                      }
                    }
                }
            }
        }
    }
}


void Send_application_response (void)
{
  Calc_crc_flag=1;
  A_time_out_flag=0;
  X=0;
  St_crc=11;
  N_crc_trans=6;
  I=15;
  Start_data=1;
  if(Send_first_fragment==0)
    {
      Need_ack=0;
      Kh=Data_num/245;
      Ba=Data_num%245;
      Frame_count=Get_Low_Byte(Kh);
      if(Ba!=0)
        {
          Frame_count++;
        }
      if(Data_num==0)
        {
          Frame_count=1;
        }
      if(D_confirm==2&&Frame_count>1)
        {
          Need_ack=1;
        }
    }
  T_sequence_transmit++;
  if(T_sequence_transmit>63)
    {
      T_sequence_transmit=0;
    }
  Hdr=192;
  for(S=1; S<=Main_request_gr_count; S+=1)
    {
      if(Request_data[S]==Event)
        {
          Hdr=224;
        }
    }
  if(Unsol_response_flag==1||Event_send_flag==1)
    {
      Hdr=224;
    }
  if(Unsol_response_flag==1||Event_send_flag==1)
    {
      Request_event_flag=0;
      Unsol_response_flag=0;
      if(A_event_sequence>31)
        {
          A_event_sequence=16;
        }
      A_sequence=A_event_sequence;
      Last_a_event_sequence=A_sequence;
      Get_crc[13]=130;
    }
  else if(Need_send_confirm==1)
    {
      Get_crc[13]=0;
      Need_send_confirm=0;
    }
  else
    {
      Last_a_sequence=A_sequence;
      Get_crc[13]=129;
    }
  Get_crc[12]=A_sequence+Hdr;
	//Iin=0x90;
  Get_crc[14]=Get_Low_Byte(Iin);
  Get_crc[15]=Get_High_Byte(Iin);
  if(Frame_count==1&&Send_first_fragment==0)
    {
      Get_crc[11]=T_sequence_transmit+192;
      Stop_data=Data_num;
      A_send_flag=1;
    }
  else if(Frame_count>1&&Send_first_fragment==0)
    {
      Get_crc[11]=T_sequence_transmit+64;
      Send_first_fragment=1;
      Stop_data=245;
      D_send_flag=1;
    }
  else if(Frame_count>1&&Send_first_fragment==1)
    {
      Get_crc[11]=T_sequence_transmit;
      Start_data=Stop_data+1;
      Stop_data+=249;
      I=11;
      N_crc_trans=2;
      D_send_flag=1;
    }
  else if(Frame_count==1&&Send_first_fragment==1)
    {
      Get_crc[11]=T_sequence_transmit+128;
      Start_data=Stop_data+1;
      Stop_data=Data_num;
      I=11;
      N_crc_trans=2;
      A_send_flag=1;
      D_send_flag=1;
    }
  Frame_count--;
  for(Tempb=Start_data; Tempb<=Stop_data; Tempb+=1)
  {
      if(N_crc_trans<16)
        {
          I++;
          N_crc_trans++;
          Get_crc[I]=Temp_crc[Tempb];
          Calc_crc_flag=1;
        }
      else if(N_crc_trans>=16)
        {
          Calc_crc_flag=0;
          I++;
          Get_crc[I]=Temp_crc[Tempb];
          N_crc_trans=1;
          X++;
         Crc_dnp=Crc_calc(St_crc,I);
					//Crc_dnp=CRC_calculator(&Get_crc[St_crc],I);
          I++;
       Get_crc[I]=Get_Low_Byte(Crc_dnp);
          I++;
        Get_crc[I]=Get_High_Byte(Crc_dnp);
          St_crc=I+1;
        }
  }
  X*=2;
  X+=5;
  X=I-X;
  Send_length=Get_Low_Byte(X);
  if(Calc_crc_flag==1)
    {
      Crc_dnp=Crc_calc(St_crc,I);
			//Crc_dnp=CRC_calculator(&Get_crc[St_crc],I);
      I++;
      Get_crc[I]=Get_Low_Byte(Crc_dnp);
      I++;
      Get_crc[I]=Get_High_Byte(Crc_dnp);
    }
//  if(Need_ack==1||D_confirm==1)   // madad
//    {
//       Fcb=!Fcb;
//      if(Fcb==0)
//        {
//          D_function=83;
//        }
//      else if(Fcb==1)
//        {
//          D_function=115;
//        }
//    }
//  else
//    {
      D_function=68;
  //  }

  Make_header();



//REPEAT_DATA:;

  Array_len1=I;
  Array_counter1=1;
  //Chip_UART_SendByte(LPC_UART0,Get_crc[1]);//UART0_SendByte(Get_crc[1]);
  //HAL_UART_Transmit_DMA(&huart3,&Get_crc[1],Array_len1);
	//DNP3_Transmit(Array_len1);
	Dnp3_send_Flag=1;

 //rintbin5,Get_crc(1);1;
  Start_count=0;
	Data_count=1;
   //led2(1);
}
/*-----------------------------------------------------------------------------
 *                   ---------CRC_calculator-----------
 *----------------------------------------------------------------------------*/

WORD Crc_calc (WORD Start_crc2, WORD Num_crc22)
  {
WORD i;   /* will index into CRC lookup */
WORD crcAccum=0;
   for(D=Start_crc2; D<=Num_crc22; D+=1)
      {
        i = (crcAccum ^ Get_crc[D]) & 0xFF;     /* calculate the CRC  */
        crcAccum = Get_High_Byte(crcAccum) ^ crcLookUpTable[i];
      }
 return (~crcAccum);
}


WORD Crc_calc1 (WORD Start_crc2, WORD Num_crc22)
{
WORD i;   /* will index into CRC lookup */
	WORD crcAccum=0;
    for(D=Start_crc2; D<=Num_crc22; D+=1)
      {
        i = (crcAccum ^ Modem_rcv[D]) & 0xFF;     /* calculate the CRC  */
        crcAccum = Get_High_Byte(crcAccum) ^ crcLookUpTable[i];
      }
return (~crcAccum);
		}
//---------------------------------------------------------------------------
double Abs (double a)
{
  if(a<0) return -a;
  return  a;
}
//-----------------------------------------------------------------------------
void DNP3_Transmit(WORD datalen)
{
		//Dnp3portBusyFlag=1;
		//HAL_UART_Transmit_DMA(&huart6,&Get_crc[1],datalen);			//MEBADI AND SAJJEDI
//	for ( uint16_t S = 0 ; S < datalen ; S++ )
//	{
//		Ucxx.Tcp.Buf[S] = Get_crc[1+S];
//	}
//	Ucxx.Tcp.Buf = &Get_crc[0];
//	Ucxx.Tcp.Len = datalen;
	//UcSetTaskProcess(HIGH_PRIORITY,&Ucxx.TcpTask,UCXX_TCP_SEND_COMMAND_DATA,Ucxx.Tcp.CurrentConnection,datalen,&Get_crc[1]);

	//najafi

//	char buffer[500];
//	memset(buffer,0, sizeof(buffer));
//
//	for(int i=0;i<datalen;i++)
//	{
//		s mbuffer+strlen(buffer),"%x-",Get_crc[1+i]);
//	}
//
//	printf("%s\n",buffer);

	//write(portfd,&Get_crc[1],datalen);

	memcpy(Dnp_Buffer.TX_Buffer,&Get_crc[1],datalen);
	Dnp_Buffer.TX_Count=datalen;


}
//-----------------------------------------------------------------------------
void Sub_rcv_Dnp3(BYTE value_udr0,BYTE port)
{
Data_buf = value_udr0;
Busy_line=1000;
Wait_unsol_flag = 0;
Time_delay2 = 0;
Rcv_dnp_port=port;
//if(Error_recive==0)
//{
    for(;;)
    {
      if(Data_count==1)
			{
				if(Data_buf==5 && Rcv_flag_dnp==0)
					{
						Modem_rcv1[1]=Data_buf;
						Data_count++;
					}
				else
					{
						Data_count=1;
					}
				break;
			}
      if(Data_count==2)
			{
				if(Data_buf==100)
					{
						Modem_rcv1[2]=Data_buf;
						Data_count++;
					}
				else
					{
						Data_count=1;
					}
				break;
			}
      if(Data_count==3)
			{
				Modem_rcv1[Data_count]=Data_buf;
				Data_count++;
				if(Data_buf>5&&Data_buf<50)
				  {
						Data_buf-=5;
						Rcv_num=Data_buf/16;
						Baa=Data_buf % 16;
						if(Baa!=0)
							{
								Rcv_num++;
							}
						Rcv_num*=2;
						Rcv_num+=Data_buf;
						Rcv_num+=10;
					}
          else if(Data_buf==5)
					{
						Rcv_num=10;
					}
          else
          {
            Data_count=1;
          }
          break;
        }
         else
        {
          if(Data_count<Rcv_num)
            {
              Modem_rcv1[Data_count]=Data_buf;
              Data_count++;
            }
          else
            {
              Modem_rcv1[Data_count]=Data_buf;
              Rcv_flag_dnp=1;
              Dnp_Temp_data_count=Data_count;
              Rcv_num=0;
              Data_count=1;
              Error_recive=1;

            }
        }
      break;
    }

		if (Data_count == 1)
		{
//				if (Rcv_dnp_port == Com1Num)  //Com1Num
//					Error_Protocol_fun(&huart6);
//				else
//					Error_Protocol_fun(&huart2);

			//najafi
	  }

//}
}
/*******************Dnp3Slave_PushAiEvent****************/
bool Dnp3Slave_PushAiEvent(uint16_t point,uint8_t calss){

	Detect_event_flag[calss]=1;
	Event_num[calss]++;
	Jj=Start_event_num[calss]+Event_num[calss];
//	if (Jj>0)
		Jj--;
	Event_type[Jj]=Ai_event_obj;
	Event_index[Jj]=point;
	Event_value[Jj]=Main_buffer[point+St_add_soft_aio_plc];
	//Event_Analogflag[Jj]=Analogflag_value[X-196];
	Event_num_gr[Ai_event_obj]++;

}

/*******************Dnp3Slave_PushDiEvent****************/
bool Dnp3Slave_PushDiEvent(uint16_t point,uint8_t calss){


}




