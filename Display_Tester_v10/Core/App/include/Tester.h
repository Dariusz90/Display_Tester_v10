/*
 * Tester.h
 *
 *  Created on: Aug 27, 2020
 *      Author: DBabraj
 * 		Version: 1.0
 */

#ifndef _TESTER_H_
#define _TESTER_H_

#ifdef __cplusplus // plik.h
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32inc.h"

#include "Utils.h"
/********************** NOTES *************************************************
...
*******************************************************************************/

/* Public define -----------------------------------------------------------*/

/* Public macros -----------------------------------------------------------*/

/* Public typedef ----------------------------------------------------------*/

typedef enum{
	ST_MAIN_TASK,
	ST_ADC_MEASURE_TASK,
	ST_DI_READ_TASK,
	ST_NB	// MAX SIZE OF SOFT TIMER DATA
}SoftTimer_e;

/* Public constants --------------------------------------------------------*/

/* Public variables --------------------------------------------------------*/

extern volatile u32 SoftTimerCnt[ST_NB];

/* Public function prototypes ----------------------------------------------*/

void Tester_Init(void);
void Tester_ProcessingTask(void);



#ifdef __cplusplus // plik.h
}
#endif


#endif /* _TESTER_H_ */
