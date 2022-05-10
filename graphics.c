/*
 * graphics.c
 *
 *  Created on: 2022. máj. 3.
 *      Author: Andris
 */
#include "graphics.h"

volatile float bank_angle; //oldaliranyu elfordulas
volatile float pitch_angle; //elore/hatra doles
uint8_t pixels[8][128];
line bank_indicators[7];
line pitch_indicators[5];

void Graphics_Init(){
	GLCD_Init();
	bank_angle = 0;
	pitch_angle = 0;
	for (int i = 0; i < 8; i++){
		for(int j = 0; j < 128; j++){
			pixels[i][j] = 0;
		}
	}
	//generate bank indicator lines
	float phi = 0;
	float r0 = 26;
	float r1 = 31;
	for (int i = 0; i < 7; i++){
		bank_indicators[i].x0 = r0 * cosf(phi);
		bank_indicators[i].y0 = r0 * sinf(phi);
		bank_indicators[i].x1 = r1 * cosf(phi);
		bank_indicators[i].y1 = r1 * sinf(phi);
		phi += M_PI/6.0;
	}

	//generate pitch indicator lines
	float l_0 = 25;
	float l_1 = 8;
	float space = 8;
	for (int i = 0; i < 5; i++){
		float height = -2.0 * space + i * space;
		pitch_indicators[i].y0 = height;
		pitch_indicators[i].y1 = height;
		if (i == 2){
			pitch_indicators[i].x0 = -l_0;
			pitch_indicators[i].x1 = l_0;
		}
		else {
			pitch_indicators[i].x0 = -l_1;
			pitch_indicators[i].x1 = l_1;
		}
	}
}

void writePixel(int x, int y){
	x += 64;
	y = 32 - y;
	if(x>=0 && x<=127 && y>=0 && y<=63){
		uint8_t bit = y%8;
		uint8_t row = (y - bit) / 8;
		pixels[row][x] |= 0x01<<bit;
	}
}

void displayResetIcon(){
	pixels[7][4]  = 0x00;
	pixels[7][5]  = 0x38;
	pixels[7][6]  = 0x44;
	pixels[7][7]  = 0x82;
	pixels[7][8]  = 0x82;
	pixels[7][9]  = 0x82;
	pixels[7][10] = 0x54;
	pixels[7][11] = 0x18;
	pixels[7][12] = 0x1C;
	pixels[7][13] = 0x00;
}

void displayBrightnessIcon(){
	pixels[7][113] = 0x00;
	pixels[7][114] = 0x10;
	pixels[7][115] = 0x92;
	pixels[7][116] = 0x44;
	pixels[7][117] = 0x10;
	pixels[7][118] = 0xBA;
	pixels[7][119] = 0x10;
	pixels[7][120] = 0x44;
	pixels[7][121] = 0x92;
	pixels[7][122] = 0x10;
	pixels[7][123] = 0x00;
}

void displayBar(){
	int br = round(TIM1->CCR1 * 100.0 / 65535.0);
	pixels[4][13] = 0xFF;
	for (int i = 1; i <= 100; i++){
		if (br>=i)
			pixels[4][13+i] = 0xFF;
		else
			pixels[4][13+i] = 0x81;
	}
	pixels[4][114] = 0xFF;
}

void displayBackIcon(){
	pixels[7][6]  = 0x00;
	pixels[7][7]  = 0x10;
	pixels[7][8]  = 0x38;
	pixels[7][9]  = 0x7C;
	pixels[7][10] = 0xFE;
	pixels[7][11] = 0x00;
}

void displayMinusIcon(){
	pixels[7][44] = 0x00;
	pixels[7][45] = 0x10;
	pixels[7][46] = 0x10;
	pixels[7][47] = 0x10;
	pixels[7][48] = 0x10;
	pixels[7][49] = 0x10;
	pixels[7][50] = 0x10;
	pixels[7][51] = 0x10;
	pixels[7][52] = 0x00;
}

void displayPlusIcon(){
	pixels[7][75] = 0x00;
	pixels[7][76] = 0x10;
	pixels[7][77] = 0x10;
	pixels[7][78] = 0x10;
	pixels[7][79] = 0xFE;
	pixels[7][80] = 0x10;
	pixels[7][81] = 0x10;
	pixels[7][82] = 0x10;
	pixels[7][83] = 0x00;
}

void reDraw(){
	//clear local pixel array
	for (int i = 0; i < 8; i++){
		for(int j = 0; j < 128; j++){
			pixels[i][j] = 0;
		}
	}

	if(state == STATE_DISPLAY){
		//animate and draw bank indicator lines to local pixel array
		for (int i = 0; i < 7; i++){
			line tf_line = rotateLine(bank_indicators[i]); //transform
			plotLine(tf_line);
		}
		//animate and draw pitch indicator lines to local pixel array
		for (int i = 0; i < 5; i++){
			line tf_line = shiftLine(pitch_indicators[i]); //transform
			tf_line = rotateLine(tf_line); //transform
			plotLine(tf_line);
		}
		//display icons
		displayResetIcon();
		displayBrightnessIcon();
	}

	if(state == STATE_BRIGHTNESS){
		displayBar();
		displayBackIcon();
		displayMinusIcon();
		displayPlusIcon();
	}

	//copy local pixel array to the screen
	for (int i = 0; i < 8; i++){
		for(int j = 0; j < 128; j++){
			GLCD_Write_Block(pixels[i][j], i, j);
		}
	}
}

line rotateLine(line _line){
	line ret;
	ret.x0  = _line.x0 * cosf(bank_angle) - _line.y0 * sinf(bank_angle);
	ret.y0  = _line.x0 * sinf(bank_angle) + _line.y0 * cosf(bank_angle);
	ret.x1  = _line.x1 * cosf(bank_angle) - _line.y1 * sinf(bank_angle);
	ret.y1  = _line.x1 * sinf(bank_angle) + _line.y1 * cosf(bank_angle);
	return ret;
}

line shiftLine(line _line){
	line ret;
	//10 fok (pi/18 rad) felel meg 8 pixelnek -> 1 rad = 144/pi pixel
	ret.x0 = _line.x0;
	ret.y0 = _line.y0 + pitch_angle * 144 / M_PI;
	ret.x1 = _line.x1;
	ret.y1 = _line.y1 + pitch_angle * 144 / M_PI;
	return ret;
}

void plotLineLow(int x0, int y0, int x1, int y1){
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;
    if (dy < 0){
        yi = -1;
        dy = -dy;
    }
    int D = (2 * dy) - dx;
    int y = y0;

    for (int x = x0; x <= x1; x++){
        writePixel(x, y);
        if (D > 0){
            y = y + yi;
            D = D + (2 * (dy - dx));
        }
        else
            D = D + 2*dy;
    }
}

void plotLineHigh(int x0, int y0, int x1, int y1){
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;
    if (dx < 0){
        xi = -1;
        dx = -dx;
    }
    int D = (2 * dx) - dy;
    int x = x0;

    for (int y = y0; y <= y1; y++){
    	writePixel(x, y);
        if (D > 0){
            x = x + xi;
            D = D + (2 * (dx - dy));
        }
        else
            D = D + 2*dx;
    }
}

void plotLine(line _line){
	int x0 = round(_line.x0);
	int y0 = round(_line.y0);
	int x1 = round(_line.x1);
	int y1 = round(_line.y1);
    if (abs(y1 - y0) < abs(x1 - x0)){
        if (x0 > x1)
            plotLineLow(x1, y1, x0, y0);
        else
            plotLineLow(x0, y0, x1, y1);
    }
    else{
        if (y0 > y1)
            plotLineHigh(x1, y1, x0, y0);
        else
            plotLineHigh(x0, y0, x1, y1);
    }
}
