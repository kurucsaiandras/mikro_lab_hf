/*
 * graphics.h
 *
 *  Created on: 2022. máj. 3.
 *      Author: Andris
 */

#ifndef INC_GRAPHICS_H_
#define INC_GRAPHICS_H_
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>
#include "bsp_glcd.h"
#include "stm32f4xx_hal.h"

extern uint8_t pixels[8][128];

typedef struct{
	float x0, y0, x1, y1;
}line;
extern volatile float bank_angle; //oldaliranyu elfordulas
extern volatile float pitch_angle; //elore/hatra doles

typedef enum{
	STATE_DISPLAY,
	STATE_BRIGHTNESS
} prog_state;

extern prog_state state;

extern line bank_indicators[7];
extern line pitch_indicators[5];

void Graphics_Init();
void writePixel(int x, int y);
void displayResetIcon();
void displayBrightnessIcon();
void displayBar();
void displayBackIcon();
void displayMinusIcon();
void displayPlusIcon();
void reDraw();
line rotateLine(line _line);
line shiftLine(line _line);
void plotLineLow(int x0, int y0, int x1, int y1);
void plotLineHigh(int x0, int y0, int x1, int y1);
void plotLine(line _line);

#endif /* INC_GRAPHICS_H_ */
