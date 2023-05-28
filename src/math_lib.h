/*
 * math_lib.h
 *
 *  Created on: Nov 13, 2021
 *      Author: mhn
 */



#ifndef MATH_LIB_H_
#define MATH_LIB_H_


uint16_t makeint(uint8_t lsb,uint8_t msb);
uint8_t Get_High_Byte(uint16_t data);
uint8_t Get_Low_Byte(uint16_t data);
char * mid(char *str,int start,int len);
uint16_t Get_High_Word(long data);
uint16_t Get_Low_Word(long data);
void U8_Set_bit (uint8_t *p, uint8_t bit);
void Set_bit (uint16_t *p, uint16_t bit);
void Clr_bit (uint16_t *p, uint16_t bit);
void U8_Clr_bit (uint8_t *p, uint8_t bit);
void Toggle_bit (uint16_t *p, uint16_t bit);
bool Chk_bit (uint8_t *p, uint16_t bit);
bool Chk_bit_16 (uint16_t *p, uint16_t bit);
extern bool Chk_bit_32 (uint32_t *p, uint16_t bit);
void Change_bit (uint16_t *p, uint16_t bit, bool val);
void Change_bit_8 (uint8_t *p, uint16_t bit, bool val);
extern void Change_bit_32 (uint32_t *p, uint16_t bit, bool val);
extern uint8_t Rot_l(uint8_t value, uint8_t shift);
extern uint8_t Rot_r(uint8_t value, uint8_t shift);






#endif /* MATH_LIB_H_ */
