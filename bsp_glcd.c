/**
*****************************************************************************
** Kommunikációs mérés - glcd.c
** A grafikus LCD függvényei
*****************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"

#include "bsp_glcd.h"
#include "font.h"

void GLCD_MSP_Init(void);

/* ----------------- Publikus függvények ----------------- */

void GLCD_Init(void)
{
	// Minden alapállapotba
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);	// GLCD_RW = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);		// GLCD_E = 1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);	// GLCD_RESET = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);	// #GLCDEN = 0

	// Init GLCD functions
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);	// GLCD_E = 0
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);	// GLCD_RESET = 1

	GLCD_Write(3,0,0x3F); 	//GLCD bekapcsolása
	GLCD_Write(3,0,0xC0);	//GLCD Start Line
	GLCD_Clear();			//Kijelzõ törlése
}

void GLCDEN(int newState)
{
	if(newState)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);	//GLDC_EN
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);		//GLDC_EN
	}
}

//returns the idx-th bit of the data byte
//idx between 0 and 7
int getBit(char data, int idx){
	data = data >> idx;
	data &= 1;
	return data;
}

/** Beírja a g_data értéket a kiválasztott kijelzõvezérlõbe (cs_s->CS1, CS2) utasítás/adat paraméternek megfelelõen.
* Általánosan felhasználható 8bit (adat/utasítás) beírására a kijelzõ vezérlõjébe.
* 	cs_s, 1 = CS1, 2 = CS2, 3 = CS1&CS2
* 	d_i, 0 = instruction, 1 = data
*/
void GLCD_Write(char cs_s,char d_i,char data)
{
	int bitData = 0;

	switch(cs_s)
	{
	case 1:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);	//GLCD_CS1
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);	//GLCD_CS2
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);	//GLCD_CS1
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);	//GLCD_CS2
		break;
	}
	switch(d_i)
	{
	case 0:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);	//GLCD_DI
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);		//GLCD_DI
		break;
	}

	//Writing the data pins:
	//DB0 C7
	//DB1 B6
	//DB2 A7
	//DB3 A6
	//DB4 B1
	//DB5 B15
	//DB6 B14
	//DB7 B13
	bitData = getBit(data, 0);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, bitData);

	bitData = getBit(data, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, bitData);

	bitData = getBit(data, 2);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, bitData);

	bitData = getBit(data, 3);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, bitData);

	bitData = getBit(data, 4);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, bitData);

	bitData = getBit(data, 5);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, bitData);

	bitData = getBit(data, 6);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, bitData);

	bitData = getBit(data, 7);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, bitData);

	//Sys_DelayUs(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);		//GLCD_E
	//Sys_DelayUs(2);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);	//GLCD_E
	//Sys_DelayUs(4);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);	//GLCD_CS1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);	//GLCD_CS2
}

/** Kijelző törlése. */
void GLCD_Clear(void)
{
	char x,y;
	for(x=0;x<8;x++)
	{
		GLCD_Write(3,0,0x40);
		GLCD_Write(3,0,(0xB8|x));
		for(y=0;y<64;y++)
		{
			GLCD_Write(3,1,0x00);
		}
	}
}

/** A kijelzõ adott sor-oszlop metszetének beállítása az m_data értéknek megfelelõen.
 * 		m_data: adott metszet rajzolata hexába kódolva
 * 		cX: sor (0-7)
 * 		cY: oszlop (0-127) */
void GLCD_Write_Block(char m_data,char cX,char cY)
{
	char chip=1;
	if(cY>=64)
	{
		chip=2;
		cY-=64;
	}
	GLCD_Write(chip,0,(0x40|cY));
	GLCD_Write(chip,0,(0xB8|cX));
	GLCD_Write(chip,1,m_data);
}

/** String kiírása a kijelzőre. */
void GLCD_WriteString(const char* string,char Y, char X)
{
	char temp = 0;
	int i=0;
	while(string[i]!='\0')
	{
		temp = string[i];
		GLCD_Write_Char(temp-32,X,Y+6*i);
		i++;
	}
}

/** Karakter írása a kijelzőre. */
void GLCD_Write_Char(char cPlace,char cX,char cY)
{
	char i=0;
	char chip=1;
	if(cY>=64)
	{
		chip=2;
		cY-=64;
	}
	GLCD_Write(chip,0,(0x40|cY));
	GLCD_Write(chip,0,(0xB8|cX));
	for (i=0;i<5;i++)
	{
	  if (cY+i >= 64)
	  {
		  chip=2;
		  GLCD_Write(chip,0,(0x40|(cY+i-64)));
		  GLCD_Write(chip,0,(0xB8|cX));
	  }
	  GLCD_Write(chip,1,fontdata[cPlace*5+i]);
	}
}
