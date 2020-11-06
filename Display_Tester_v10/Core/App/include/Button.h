/*
 * Button.h
 *
 *  Created on: 2 wrz 2020
 *      Author: DBabraj
 * 		Version: 1.0
 */

#ifndef _BUTTON_H_
#define _BUTTON_H_

#ifdef __cplusplus // plik.h
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/


/********************** NOTES *************************************************
...
*******************************************************************************/

/* Public define -----------------------------------------------------------*/

#define BUTTON_END_SEQ				(u16)(0x1000)
#define BUTTON_SHORT_SEQ			(u16)(0x2000)
#define BUTTON_LONG_SEQ				(u16)(0x3000)
#define BUTTON_BURST_SEQ			(u16)(0x4000)

/* Public macros -----------------------------------------------------------*/

/* Public typedef ----------------------------------------------------------*/

typedef struct {
	u32 Timer;
	u32 LongPressCounter;
	FlagStatus(*ReadButtonPinFun)(void);
}ButtonData_t;

/* Public constants --------------------------------------------------------*/

/* Public variables --------------------------------------------------------*/

/* Public function prototypes ----------------------------------------------*/

void Button_Init(ButtonData_t _pBtnData[], uc32 _ButtonNum);
void Button_ProcessingTask(ButtonData_t _pBtnData[], uc32 _ButtonNum, uc32 _TaskPeriod_ms);
u16 Button_GetLastPressed(void);




#ifdef __cplusplus // plik.h
}
#endif


#endif /* _BUTTON_H_ */
