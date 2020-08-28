/*
 * Utils.c
 *
 *  Created on: Aug 27, 2020
 *      Author: DBabraj
 * 		Version: 1.0
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32inc.h"
#include "Utils.h"

/********************** NOTES **********************************************
...
*******************************************************************************/

//#ifdef LIB_MODULE_ENABLED // plik.c


/* Private define ------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Public functions --------------------------------------------------------*/

/**
 * @brief SoftTimer_TickHandler
 * @note Soft timer handler. Handler must be running on hardware timer interrupt.
 * @param  *_soft_timer: Pointer to timer data table.
 * @param  _st_size: Size of timer data table.
 * @retval void
 */
inline void SoftTimer_TickHandler(volatile u32 *_soft_timer, const u32 _st_size)
{
	static u32 x = 0;	//	auxiliary variable

	for(u32 i = 0; i < _st_size; i++)
	{
		x = _soft_timer[i];
		if(x) _soft_timer[i] = --x;
	}
}

/**
 * @brief Filter_LPFfixedPointArithmetic
 * @note
 * @param [in/out]*_pMem: memory of filter state
 * @param [in]_rawData: new value of data
 * @param [in]_filterBeta:	Length of the filter. Example: _filterBeta = 4 -> 16 length of the filter
 * @retval filtered value
 */
s32 Filter_LPFfixedPointArithmetic(s32 *_pMem, s32 _rawData, const u8 _filterBeta)
{
    (*_pMem) = ((*_pMem) << _filterBeta) - (*_pMem);
    (*_pMem) += _rawData;
    (*_pMem) >>= _filterBeta;

	return (*_pMem);
}

/* Private functions ---------------------------------------------------------*/




//#endif /* LIB_MODULE_ENABLED */ // plik.c
