/*
 * GPIOdriver.h
 *
 *  Created on: 28 sie 2020
 *      Author: DBabraj
 * 		Version: 1.0
 */

#ifndef _GPIODRIVER_H_
#define _GPIODRIVER_H_

#ifdef __cplusplus // plik.h
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/


/********************** NOTES *************************************************
...
*******************************************************************************/

/* Public define -----------------------------------------------------------*/

/* Public macros -----------------------------------------------------------*/

/* Public typedef ----------------------------------------------------------*/

typedef enum {
	DO_OFF,
	DO_ON,
	DO_TOGGLE
}OutStateType_e;

typedef enum {
	DIS_SEG_A,
	DIS_SEG_B,
	DIS_SEG_C,
	DIS_SEG_D,
	DIS_SEG_E,
	DIS_SEG_F,
	DIS_SEG_G,
	DIS_SEG_DOT,

	DIS_SEG_NB
}DisSegType_e;

typedef enum {
	DIS_COMMON_ANODE,
	DIS_COMMON_CATHODE,

	DIS_COMMON_NB
}DisType_e;

typedef enum {
	LED_Ext_G,
	LED_Ext_R,
	LED_1,
	LED_2,

	LED_NB
}LedType_e;

typedef enum {
	DI_DipSwitch1,
	DI_DipSwitch2,
	DI_Button1,
	DI_Button2,

	DI_NB
}DigInType_e;

/* Public constants --------------------------------------------------------*/

/* Public variables --------------------------------------------------------*/

/* Public function prototypes ----------------------------------------------*/

void Display_SetSegment(DisSegType_e _Seg, OutStateType_e _State, DisType_e _DisType);
void LED_SetState(LedType_e _Led, OutStateType_e _State);
FlagStatus GPIO_GetInput(DigInType_e _DI);




#ifdef __cplusplus // plik.h
}
#endif


#endif /* _GPIODRIVER_H_ */
