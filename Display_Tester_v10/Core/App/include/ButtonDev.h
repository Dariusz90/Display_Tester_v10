/*
 * ButtonDev.h
 *
 *  Created on: 2 wrz 2020
 *      Author: DBabraj
 * 		Version: 1.0
 */

#ifndef _BUTTONDEV_H_
#define _BUTTONDEV_H_

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
	BUTTON_NONE,

	BUTTON_S1_END,
	BUTTON_S2_END,

	BUTTON_S1_SHORT,
	BUTTON_S2_SHORT,

	BUTTON_S1_LONG,
	BUTTON_S2_LONG,

	BUTTON_S1_BURST,
	BUTTON_S2_BURST
}ButtonType_e;

/* Public constants --------------------------------------------------------*/

/* Public variables --------------------------------------------------------*/

/* Public function prototypes ----------------------------------------------*/

void ButtonDev_ProcessingTask(uc32 _TaskPeriod_ms);
ButtonType_e ButtonDev_GetButton(void);




#ifdef __cplusplus // plik.h
}
#endif


#endif /* _BUTTONDEV_H_ */
