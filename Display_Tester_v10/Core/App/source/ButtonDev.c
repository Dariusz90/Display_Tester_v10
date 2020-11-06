/*
 * ButtonDev.c
 *
 *  Created on: 2 wrz 2020
 *      Author: DBabraj
 * 		Version: 1.0
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32inc.h"
#include "ButtonDev.h"

#include "Button.h"
#include "GPIOdriver.h"
/********************** NOTES **********************************************
...
*******************************************************************************/

//#ifdef LIB_MODULE_ENABLED // plik.c


/* Private define ------------------------------------------------------------*/

#define _BUTTON_NUMBER				(2)

/* Private macros ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

static FlagStatus _ReadBtn1(void);
static FlagStatus _ReadBtn2(void);

static ButtonData_t BtnData[_BUTTON_NUMBER] = {
		{.Timer = 0, .LongPressCounter = 0, .ReadButtonPinFun = _ReadBtn1},
		{.Timer = 0, .LongPressCounter = 0, .ReadButtonPinFun = _ReadBtn2},
};

/* Private function prototypes -----------------------------------------------*/

/* Public functions --------------------------------------------------------*/

/**
 * @brief ButtonDev_ProcessingTask
 * @note
 * @param [in]_TaskPeriod_ms:
 * @retval none
 */
void ButtonDev_ProcessingTask(uc32 _TaskPeriod_ms)
{
	Button_ProcessingTask(BtnData, _BUTTON_NUMBER, _TaskPeriod_ms);
}

/**
 * @brief ButtonDev_GetButton
 * @note Return pressed button, according to device orientation.
 * @param None
 * @retval ButtonType_e
 */
ButtonType_e ButtonDev_GetButton(void)
{
	u16 key = Button_GetLastPressed();

	switch( key )
	{
		case (BUTTON_END_SEQ + 0x0000):
			return BUTTON_S1_END;
		case (BUTTON_END_SEQ + 0x0001):
			return BUTTON_S2_END;

		//*****************************

		case (BUTTON_SHORT_SEQ + 0x0000):
			return BUTTON_S1_SHORT;
		case (BUTTON_SHORT_SEQ + 0x0001):
			return BUTTON_S2_SHORT;

		//*****************************

		case (BUTTON_LONG_SEQ + 0x0000):
			return BUTTON_S1_LONG;
		case (BUTTON_LONG_SEQ + 0x0001):
			return BUTTON_S2_LONG;

		//*****************************

		case (BUTTON_BURST_SEQ + 0x0000):
			return BUTTON_S1_BURST;
		case (BUTTON_BURST_SEQ + 0x0001):
			return BUTTON_S2_BURST;
	}
	return BUTTON_NONE;
}

/* Private functions ---------------------------------------------------------*/

/**
 * @brief _ReadBtn1
 * @note
 * @param none
 * @retval none
 */
static FlagStatus _ReadBtn1(void)
{
	return GPIO_GetInput(DI_Button1);
}
/**
 * @brief _ReadBtn2
 * @note
 * @param none
 * @retval none
 */
static FlagStatus _ReadBtn2(void)
{
	return GPIO_GetInput(DI_Button2);
}

//#endif /* LIB_MODULE_ENABLED */ // plik.c
