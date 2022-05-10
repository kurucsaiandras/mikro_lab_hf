/**
*****************************************************************************
** Kommunikációs mérés - glcd.h
** A grafikus LCD header file-ja
*****************************************************************************
*/
#pragma once
#ifndef _GLCD_H__
#define _GLCD_H__

void GLCD_Init(void);
void GLCDEN(int newState);
int getBit(char data, int idx);
void GLCD_Write(char cs_s,char d_i,char g_data);
void GLCD_Clear(void);
void GLCD_Delay(char value);
void GLCD_Write_Block(char m_data,char cX,char cY);
void GLCD_Write_Char(char cPlace,char cX,char cY);
void GLCD_WriteString(const char* string,char X, char Y);

#endif

