/*
 * GPIOdriver.c
 *
 *  Created on: 28 sie 2020
 *      Author: DBabraj
 * 		Version: 1.0
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32inc.h"
#include "GPIOdriver.h"

#include "main.h"
/********************** NOTES **********************************************
...
*******************************************************************************/

//#ifdef LIB_MODULE_ENABLED // plik.c


/* Private define ------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

typedef struct {
	GPIO_TypeDef *GPIOx;
	u32 PinMask;
}DIO_GPIO_t;

/* Private constants ---------------------------------------------------------*/

static const DIO_GPIO_t _DisSegOut[DIS_SEG_NB] = {
		{.GPIOx = DO_DISP_a_GPIO_Port,	.PinMask = DO_DISP_a_Pin},
		{.GPIOx = DO_DISP_b_GPIO_Port,	.PinMask = DO_DISP_b_Pin},
		{.GPIOx = DO_DISP_c_GPIO_Port,	.PinMask = DO_DISP_c_Pin},
		{.GPIOx = DO_DISP_d_GPIO_Port,	.PinMask = DO_DISP_d_Pin},
		{.GPIOx = DO_DISP_e_GPIO_Port,	.PinMask = DO_DISP_e_Pin},
		{.GPIOx = DO_DISP_f_GPIO_Port,	.PinMask = DO_DISP_f_Pin},
		{.GPIOx = DO_DISP_g_GPIO_Port,	.PinMask = DO_DISP_g_Pin},
		{.GPIOx = DO_DISP_dp_GPIO_Port,	.PinMask = DO_DISP_dp_Pin}
};

static const DIO_GPIO_t _DigIn[DI_NB] = {
		{.GPIOx = DI_SW1_GPIO_Port,	.PinMask = DI_SW1_Pin},
		{.GPIOx = DI_SW2_GPIO_Port,	.PinMask = DI_SW2_Pin},
		{.GPIOx = DI_SW3_GPIO_Port,	.PinMask = DI_SW3_Pin},
		{.GPIOx = DI_SW4_GPIO_Port,	.PinMask = DI_SW4_Pin}
};

static const DIO_GPIO_t _LedOut[LED_NB] = {
		{.GPIOx = DO_Ext_LED_G_GPIO_Port,	.PinMask = DO_Ext_LED_G_Pin},
		{.GPIOx = DO_Ext_LED_R_GPIO_Port,	.PinMask = DO_Ext_LED_R_Pin},
		{.GPIOx = DO_LED1_GPIO_Port,	.PinMask = DO_LED1_Pin},
		{.GPIOx = DO_LED2_GPIO_Port,	.PinMask = DO_LED2_Pin}
};

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Public functions --------------------------------------------------------*/

/**
 * @brief Display_SetSegment
 * @note
 * @param [in]_Seg:
 * @param [in]_State:
 * @param [in]_DisType:
 * @retval none
 */
void Display_SetSegment(DisSegType_e _Seg, OutStateType_e _State, DisType_e _DisType)
{
	if(_State == DO_OFF)
    {   // OFF
		if( _DisType == DIS_COMMON_CATHODE )
			LL_GPIO_ResetOutputPin(_DisSegOut[_Seg].GPIOx, _DisSegOut[_Seg].PinMask);
		else if( _DisType == DIS_COMMON_ANODE )
			LL_GPIO_SetOutputPin(_DisSegOut[_Seg].GPIOx, _DisSegOut[_Seg].PinMask);
    }
    else if (_State == DO_ON)
    {   // ON
    	if( _DisType == DIS_COMMON_CATHODE )
    		LL_GPIO_SetOutputPin(_DisSegOut[_Seg].GPIOx, _DisSegOut[_Seg].PinMask);
		else if( _DisType == DIS_COMMON_ANODE )
			LL_GPIO_ResetOutputPin(_DisSegOut[_Seg].GPIOx, _DisSegOut[_Seg].PinMask);
    }
    else if (_State == DO_TOGGLE)
    {   // TOGGLE
    	LL_GPIO_TogglePin(_DisSegOut[_Seg].GPIOx, _DisSegOut[_Seg].PinMask);
    }
}

/**
 * @brief LED_SetState
 * @note
 * @param [in]_Seg:
 * @param [in]_State:
 * @retval none
 */
void LED_SetState(LedType_e _Led, OutStateType_e _State)
{
	if(_State == DO_OFF)
    {   // OFF
        LL_GPIO_ResetOutputPin(_LedOut[_Led].GPIOx, _LedOut[_Led].PinMask);
//        LL_GPIO_SetOutputPin(_LedOut[_Led].GPIOx, _LedOut[_Led].PinMask);
    }
    else if (_State == DO_ON)
    {   // ON
        LL_GPIO_SetOutputPin(_LedOut[_Led].GPIOx, _LedOut[_Led].PinMask);
//        LL_GPIO_ResetOutputPin(_LedOut[_Led].GPIOx, _LedOut[_Led].PinMask);
    }
    else if (_State == DO_TOGGLE)
    {   // TOGGLE
    	LL_GPIO_TogglePin(_LedOut[_Led].GPIOx, _LedOut[_Led].PinMask);
    }
}

/**
 * @brief GPIO_GetInput
 * @note Input is pull-up
 * @param [in] _DI:
 * @retval FlagStatus: SET/RESET
 */
FlagStatus GPIO_GetInput(DigInType_e _DI)
{
    if( LL_GPIO_IsInputPinSet(_DigIn[_DI].GPIOx, _DigIn[_DI].PinMask) == 0 )
		return SET;
   	return RESET;
}

/* Private functions ---------------------------------------------------------*/




//#endif /* LIB_MODULE_ENABLED */ // plik.c
