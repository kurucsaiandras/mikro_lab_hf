/**
*****************************************************************************
** Kommunikációs mérés - mainloop.h
** UART kommunikáció
*****************************************************************************
*/
#pragma once
#ifndef _UART_H__
#define _UART_H__

/** A legutoljára fogadott byte. */
extern UART_HandleTypeDef huart2;

HAL_StatusTypeDef UART_SendString(const char* str);

#endif
