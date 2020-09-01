/*
 * Utils.h
 *
 *  Created on: Aug 27, 2020
 *      Author: DBabraj
 * 		Version: 1.0
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#ifdef __cplusplus // plik.h
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/


/********************** NOTES *************************************************
...
*******************************************************************************/

/* Public define -----------------------------------------------------------*/

/** @defgroup ADC_LL_EC_RESOLUTION  ADC instance - Resolution
  * @{
  */
#define LL_ADC_RESOLUTION_12B              0x00000000U                         /*!< ADC resolution 12 bits */

/* Public macros -----------------------------------------------------------*/

/**
  * @brief  Macro for calculate soft timer "interrupt".
  * @param  _soft_timer: 		Pointer to timer data table.
  * @param  _st_number:			Number of the soft timer to be executed.
  * @param  _st_update_rate:	Setpoint time for timer. [ms]
  * @retval TRUE/FALSE.
  */
#define SOFT_TIMER_EXECUTE(_soft_timer, _st_number, _st_update_rate)		( !_soft_timer[(_st_number)] ? (_soft_timer[_st_number] = (_st_update_rate)) : 0 )

/**
  * @brief  Helper macro to define the ADC conversion data full-scale digital
  *         value corresponding to the selected ADC resolution.
  * @note   ADC conversion data full-scale corresponds to voltage range
  *         determined by analog voltage references Vref+ and Vref-
  *         (refer to reference manual).
  * @param  __ADC_RESOLUTION__ This parameter can be one of the following values:
  *         @arg @ref LL_ADC_RESOLUTION_12B
  * @retval ADC conversion data equivalent voltage value (unit: mVolt)
  */
#define __LL_ADC_DIGITAL_SCALE(__ADC_RESOLUTION__)                             \
  (0xFFFU)


/**
  * @brief  Helper macro to calculate the voltage (unit: mVolt)
  *         corresponding to a ADC conversion data (unit: digital value).
  * @note   Analog reference voltage (Vref+) must be known from
  *         user board environment or can be calculated using ADC measurement.
  * @param  __VREFANALOG_VOLTAGE__ Analog reference voltage (unit: mV)
  * @param  __ADC_DATA__ ADC conversion data (resolution 12 bits)
  *                       (unit: digital value).
  * @param  __ADC_RESOLUTION__ This parameter can be one of the following values:
  *         @arg @ref LL_ADC_RESOLUTION_12B
  * @retval ADC conversion data equivalent voltage value (unit: mVolt)
  */
#define __LL_ADC_CALC_DATA_TO_VOLTAGE(__VREFANALOG_VOLTAGE__,\
                                      __ADC_DATA__,\
                                      __ADC_RESOLUTION__)                      \
  ((__ADC_DATA__) * (__VREFANALOG_VOLTAGE__)                                   \
   / __LL_ADC_DIGITAL_SCALE(__ADC_RESOLUTION__)                                \
  )

/* Public typedef ----------------------------------------------------------*/

/* Public constants --------------------------------------------------------*/

/* Public variables --------------------------------------------------------*/

/* Public function prototypes ----------------------------------------------*/

extern void SoftTimer_TickHandler(volatile u32 *_soft_timer, const u32 _st_size);

s32 Filter_LPFfixedPointArithmetic(s32 *_pMem, s32 _rawData, const u8 _filterBeta);

u8 Decode_7seg(char _Char);




#ifdef __cplusplus // plik.h
}
#endif


#endif /* _UTILS_H_ */
