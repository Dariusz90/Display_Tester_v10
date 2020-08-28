/*
 * stm32inc.h
 *
 *  Created on: Aug 27, 2020
 *      Author: DBabraj
 * 		Version: 1.0
 */

#ifndef _STM32INC_H_
#define _STM32INC_H_

#ifdef __cplusplus // plik.h
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/********************** NOTES *************************************************
...
*******************************************************************************/

/* Public define -----------------------------------------------------------*/

#define _PACK_PRAGMA				__attribute__ ((__packed__))

/* Public macros -----------------------------------------------------------*/

/* Public typedef ----------------------------------------------------------*/

typedef signed long long  s64;
typedef signed long s32;
typedef signed short s16;
typedef signed char s8;

typedef const signed long long sc64;  /*!< Read Only */
typedef const signed long sc32;  /*!< Read Only */
typedef const signed short sc16;  /*!< Read Only */
typedef const signed char sc8;   /*!< Read Only */

typedef volatile signed long long  vs64;
typedef volatile signed long vs32;
typedef volatile signed short vs16;
typedef volatile signed char vs8;


typedef unsigned long long u64;
typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef const unsigned long long uc64;  /*!< Read Only */
typedef const unsigned long uc32;  /*!< Read Only */
typedef const unsigned short uc16;  /*!< Read Only */
typedef const unsigned char uc8;   /*!< Read Only */

typedef volatile unsigned long long  vu64;
typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

/* Public constants --------------------------------------------------------*/

/* Public variables --------------------------------------------------------*/

/* Public function prototypes ----------------------------------------------*/




#ifdef __cplusplus // plik.h
}
#endif


#endif /* _STM32INC_H_ */
