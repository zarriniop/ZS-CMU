/*
 * math_lib.cpp
 *
 *  Created on: Nov 13, 2021
 *      Author: mhn
 */


#include "main.h"


extern uint16_t Main_buffer[3000];

char *mid(char *str,int start,int len)
{
	int id,k,j;
	static char temp_mid[3];
	j=start;
	temp_mid[0]=0,temp_mid[1]=0,temp_mid[2]=0;
	for(id=j,k=0; id<(j+len);k++, id++)
	{
		//printf(" mid function k = %d  , id=%d  \n",k,id);
		temp_mid[k]=*(str+id);
	}
	return temp_mid;
}

uint16_t makeint(uint8_t lsb,uint8_t msb)
{
	uint16_t _resualt;
	_resualt = msb;
	_resualt = _resualt << 8 ;
	_resualt = _resualt | lsb ;
	return _resualt;
}
//====================================
uint8_t Get_High_Byte(uint16_t data)
{
	uint8_t d;
	d = data >> 8;
	return d;
}
//====================================
uint8_t Get_Low_Byte(uint16_t data)
{
	uint8_t d;
	d = data  & 0x00ff;
	return d;
}
//====================================

uint16_t Get_High_Word(long data)
{
	uint16_t d;
	d = data >> 16;
	return d;
}

//=======================================

uint16_t Get_Low_Word(long data)
{
	uint16_t d;
	d = data  & 0x0000ffff;
	return d;
}
//========================================
void U8_Set_bit (uint8_t *p, uint8_t bit)
{
	*p |= 1 << bit;
}
//=======================================
void Set_bit (uint16_t *p, uint16_t bit)
{
	*p |= 1 << bit;
}

//=======================================
void U8_Clr_bit (uint8_t *p, uint8_t bit)
{
	*p &=~(1 << bit);
}


//=======================================
void Clr_bit (uint16_t *p, uint16_t bit)
{
	*p &=~(1 << bit);
}


//=======================================
void Toggle_bit (uint16_t *p, uint16_t bit)
{
	*p ^= 1 << bit;
}

//======================================
bool Chk_bit (uint8_t *p, uint16_t bit)
{
	bool val;
	val = (*p >> bit) & 1;
	return val;
}

//======================================
bool Chk_bit_16 (uint16_t *p, uint16_t bit)
{
	bool val;
 	val = (*p >> bit) & 1;
	return val;
}
//======================================
bool Chk_bit_32 (uint32_t *p, uint16_t bit)
{
	bool val;
 	val = (*p >> bit) & 1;
	return val;
}
//=====================================
void Change_bit (uint16_t *p, uint16_t bit, bool val)
{
	*p ^= (-val ^ *p) & (1 << bit);
}
//=================================================
void Change_bit_8 (uint8_t *p, uint16_t bit, bool val)
{
	*p ^= (-val ^ *p) & (1 << bit);
}
//=================================================
void Change_bit_32 (uint32_t *p, uint16_t bit, bool val)
{
	*p ^= (-val ^ *p) & (1 << bit);
}

//=================================================
uint8_t Rot_l(uint8_t value, uint8_t shift)
{
    if ((shift &= sizeof(value)*8 - 1) == 0)
      return value;
    return (value << shift) | (value >> (sizeof(value)*8 - shift));
}
//==================================================
uint8_t Rot_r(uint8_t value, uint8_t shift)
{
    if ((shift &= sizeof(value)*8 - 1) == 0)
      return value;
    return (value >> shift) | (value << (sizeof(value)*8 - shift));
}
//==================================================
