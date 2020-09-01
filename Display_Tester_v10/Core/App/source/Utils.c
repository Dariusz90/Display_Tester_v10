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

static const u8 seven_seg_digits_decode_gfedcba[78]= {
/*  -     .     /     */
	0x40, 0x80, 0x00,
/*  0     1     2     3     4     5     6     7     8     9     :     ;     */
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x00,
/*  <     =     >     ?     @     A     B     C     D     E     F     G     */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x00, 0x39, 0x00, 0x79, 0x71, 0x3D,
/*  H     I     J     K     L     M     N     O     P     Q     R     S     */
    0x76, 0x30, 0x1E, 0x00, 0x38, 0x00, 0x00, 0x3F, 0x73, 0x00, 0x00, 0x6D,
/*  T     U     V     W     X     Y     Z     [     \     ]     ^     _     */
    0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
/*  `     a     b     c     d     e     f     g     h     i     j     k     */
    0x00, 0x5F, 0x7C, 0x58, 0x5E, 0x00, 0x00, 0x00, 0x74, 0x00, 0x00, 0x00,
/*  l     m     n     o     p     q     r     s     t     u     v     w     */
    0x00, 0x00, 0x54, 0x5C, 0x00, 0x67, 0x50, 0x00, 0x78, 0x1C, 0x00, 0x00,
/*  x     y     z     */
    0x00, 0x6E, 0x00
};

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

/**
 * @brief Decode_7seg.
 * @note
 * @param [in]_Char:
 * @retval decode data
 */
u8 Decode_7seg(char _Char)
{
    if (_Char < (u8)'-')
        return 0x00;
    if (_Char > (u8)'z')  /* assuming implementation uses ASCII */
        return 0x00;

    u8 outData = seven_seg_digits_decode_gfedcba[_Char - '-'];

    return outData;
}

/* Private functions ---------------------------------------------------------*/




//#endif /* LIB_MODULE_ENABLED */ // plik.c
