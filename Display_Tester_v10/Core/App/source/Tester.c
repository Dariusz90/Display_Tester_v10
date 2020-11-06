/*
 * Tester.c
 *
 *  Created on: Aug 27, 2020
 *      Author: DBabraj
 * 		Version: 1.0
 */

/* Includes ------------------------------------------------------------------*/
#include "Tester.h"

#include "main.h"
#include "GPIOdriver.h"
#include "FIFO_Buffer.h"
#include "ButtonDev.h"

#include <string.h>
/********************** NOTES **********************************************
...
	INFO:
	Odczytany prąd segmentu LED  =  4[mA]
*******************************************************************************/

//#ifdef LIB_MODULE_ENABLED // plik.c


/* Private define ------------------------------------------------------------*/

#define _ADC_REF_mV							((u32)(3300))	// [mV]
#define _ADC_TOTAL_CHANNEL_NB				(2)

#define _CURRENT_SENSOR2_INDEX				(0)
#define _CURRENT_SENSOR1_INDEX				(1)

#define _ADC_MEASURE_TASK_PERIOD_MS			(5) // [ms]
#define _DI_READ_TASK_PERIOD_MS				(20) // [ms]
#define _HEARTBEAT_TASK_PERIOD_MS			(1000) // [ms]
#define _MAIN_TASK_PERIOD_MS				(10) // [ms]

#define _CURRENT_AMP_GAIN					(20)
#define _CURRENT_SHUNT_RES					(3300)	// [mOma]

// Current ADC to amperes factor
//#define FAC_CURRENT							((_ADC_REF_mV / 4095.0) / (_CURRENT_SHUNT_RES * _CURRENT_AMP_GAIN))

//	Size of the circular receive buffer, must be power of 2
#define _BUTTON_RX_BUFFER_SIZE				(8)

#if ( _BUTTON_RX_BUFFER_SIZE & (_BUTTON_RX_BUFFER_SIZE - 1) )
#	error RX buffer size is not a power of 2
#endif

#define _SEGLED_TEST_CURRENT_MIN			(3)	//	5 [mA]
#define _SEGLED_TEST_CURRENT_MAX			(5)	//	5 [mA]

/* Private macros ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

typedef struct {
	vu16 RawAdc[_ADC_TOTAL_CHANNEL_NB];
	s32 MeanAdc[_ADC_TOTAL_CHANNEL_NB];
}RawAdcData_t;

typedef struct {
	struct {
		u32 Voltage_mV;
		s32 Current_mA;
	}CurrSens1;
	struct {
		u32 Voltage_mV;
		s32 Current_mA;
	}CurrSens2;
}AnalogData_t;

typedef enum {
	BTN_S1,
	BTN_S2,
	BTN_NB,
	BTN_NONE
}BtnType_e;

typedef enum {
	TEST_IDLE_STATE,

	TEST_START_STATE,
	TEST_SEG_A_STATE,
	TEST_SEG_B_STATE,
	TEST_SEG_C_STATE,
	TEST_SEG_D_STATE,
	TEST_SEG_E_STATE,
	TEST_SEG_F_STATE,
	TEST_SEG_G_STATE,
	TEST_SEG_DOT_STATE,
	TEST_SUCCESS_STATE,

	TEST_ERROR_STATE,
}TestState_e;

typedef struct {
	u32 SegCurrMeanSum;
	u32 SegCurrMeanCnt;
	u32 SegCurrMean;
	FunctionalState LockTest;
	u16 SegLedCurrMin;
	u16 SegLedCurrMax;
}TestData_t;
/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

extern ADC_HandleTypeDef hadc;

static RawAdcData_t AdcMeasureData = {0};
static AnalogData_t AnalogData = {0};

volatile u32 SoftTimerCnt[ST_NB];

static u8 Button_RxBuff[_BUTTON_RX_BUFFER_SIZE] = {0};
static FIFO_TypeDef ButtonFifoRxData = { .BufferSize = _BUTTON_RX_BUFFER_SIZE, .pBuffer = Button_RxBuff, .Tail = 0, .Head = 0 };

static DisType_e DisplayType = DIS_COMMON_ANODE;
static TestData_t TestData = {0};
/* Private function prototypes -----------------------------------------------*/

static void _ADC_Init(void);
static u32 _AdcRawToVolt_mV(u16 _AdcChannelIndex);

static void _Button_ProcessingTaks(void);
static BtnType_e _Button_GetLastPressed(void);

static ErrorStatus _Current_Check(u32 _Curr_mA, u32 _MinCurr_mA, u32 _MaxCurr_mA);
static FlagStatus _Timeout(u32 _Timeout_ms, u32 _TaskPeriod_ms, FunctionalState _ResetTimeState);

static u32 _TesterSteps_DisplaySegTest(u32 _MainState, DisSegType_e _Seg, u32 _TaskPeriod_ms, ErrorStatus *_pErrStatus, FunctionalState _ResetState);

static void _Display_PrintChar(char _Char, FunctionalState _DotOnOff);
/* Public functions --------------------------------------------------------*/

/**
 * @brief Tester_Init
 * @note
 * @param none
 * @retval none
 */
void Tester_Init(void)
{
	// ADC init
	_ADC_Init();

	// Check display type: Common anode or cathode
	if( GPIO_GetInput(DI_DipSwitch1) == SET )
		DisplayType = DIS_COMMON_CATHODE;
	else
		DisplayType = DIS_COMMON_ANODE;

	LED_SetState(LED_1, DO_OFF);
	LED_SetState(LED_2, DO_OFF);

	LED_SetState(LED_Ext_G, DO_OFF);
	LED_SetState(LED_Ext_R, DO_OFF);

	for (u32 i = 0; i < DIS_SEG_NB; ++i) {
		Display_SetSegment(i, DO_OFF, DisplayType);
	}

	TestData.SegLedCurrMin = _SEGLED_TEST_CURRENT_MIN;
	TestData.SegLedCurrMax = _SEGLED_TEST_CURRENT_MAX;
}

/**
 * @brief Tester_ProcessingTask
 * @note
 * @param none
 * @retval none
 */
void Tester_ProcessingTask(void)
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//  ST_ADC_MEASURE
	if( SOFT_TIMER_EXECUTE(SoftTimerCnt, ST_ADC_MEASURE_TASK, _ADC_MEASURE_TASK_PERIOD_MS) )
	{
		Filter_LPFfixedPointArithmetic(&AdcMeasureData.MeanAdc[_CURRENT_SENSOR1_INDEX], AdcMeasureData.RawAdc[_CURRENT_SENSOR1_INDEX], 2);
		Filter_LPFfixedPointArithmetic(&AdcMeasureData.MeanAdc[_CURRENT_SENSOR2_INDEX], AdcMeasureData.RawAdc[_CURRENT_SENSOR2_INDEX], 2);

		AnalogData.CurrSens1.Voltage_mV = _AdcRawToVolt_mV(_CURRENT_SENSOR1_INDEX);
		AnalogData.CurrSens2.Voltage_mV = _AdcRawToVolt_mV(_CURRENT_SENSOR2_INDEX);

		AnalogData.CurrSens1.Current_mA = (AnalogData.CurrSens1.Voltage_mV * 1000) / (_CURRENT_SHUNT_RES * _CURRENT_AMP_GAIN);
		AnalogData.CurrSens2.Current_mA = (AnalogData.CurrSens2.Voltage_mV * 1000) / (_CURRENT_SHUNT_RES * _CURRENT_AMP_GAIN);
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//  ST_DI_READ_TASK
	if( SOFT_TIMER_EXECUTE(SoftTimerCnt, ST_DI_READ_TASK, _DI_READ_TASK_PERIOD_MS) )
	{
		_Button_ProcessingTaks();
#if 0 // Test Button library
		ButtonDev_ProcessingTask(_DI_READ_TASK_PERIOD_MS);
#endif
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//  ST_HEARTBEAT_TASK
	if( SOFT_TIMER_EXECUTE(SoftTimerCnt, ST_HEARTBEAT_TASK, _HEARTBEAT_TASK_PERIOD_MS) )
	{
		if( GPIO_GetInput(DI_DipSwitch2) == SET )
		{
			LED_SetState(LED_1, DO_TOGGLE);
			LED_SetState(LED_2, DO_TOGGLE);
		}
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//  ST_MAIN_TASK
	if( SOFT_TIMER_EXECUTE(SoftTimerCnt, ST_MAIN_TASK, _MAIN_TASK_PERIOD_MS) )
	{
#if 0 // Test Button library
		ButtonType_e Button = ButtonDev_GetButton();

		if( Button == BUTTON_S1_LONG )
		{
			LED_SetState(LED_Ext_G, DO_ON);
		}
		else if( Button == BUTTON_S2_LONG )
		{
			LED_SetState(LED_2, DO_ON);
		}
		else if( Button == BUTTON_S1_SHORT )
		{
			LED_SetState(LED_Ext_G, DO_OFF);
		}
		else if( Button == BUTTON_S2_SHORT )
		{
			LED_SetState(LED_2, DO_OFF);
		}
		else if( Button == BUTTON_S2_BURST )
		{
			LED_SetState(LED_2, DO_TOGGLE);
		}
		else if( Button == BUTTON_S1_BURST )
		{
			LED_SetState(LED_Ext_G, DO_TOGGLE);
		}
#else
		BtnType_e Button = _Button_GetLastPressed();
		static u32 PrintCurrState = 0;
		char num = '0';

#if 1 // Edit min, max current for segment
		if( Button == BTN_S1 && TestData.LockTest == ENABLE )
		{
			if( PrintCurrState == 1 ) // Edit min. current for segment
			{
				TestData.SegLedCurrMin++;
				if( TestData.SegLedCurrMin > 9 )
					TestData.SegLedCurrMin = 0;
				num += TestData.SegLedCurrMin;
				_Display_PrintChar(num, DISABLE);
			}
			else if( PrintCurrState == 3 ) // Edit max. current for segment
			{
				TestData.SegLedCurrMax++;
				if( TestData.SegLedCurrMax > 9 )
					TestData.SegLedCurrMax = 0;
				num += TestData.SegLedCurrMax;
				_Display_PrintChar(num, DISABLE);
			}
		}
#endif

		if( Button == BTN_S2 )
		{
			// Print mean segment current
			switch (PrintCurrState)
			{
				case 0:
					num += TestData.SegLedCurrMin;
					_Display_PrintChar(num, DISABLE);
					TestData.LockTest = ENABLE;
					PrintCurrState++;
					break;
				case 1:
					if( TestData.SegCurrMean < 10 )
						num += TestData.SegCurrMean;
					else
						num = '-';
					_Display_PrintChar(num, ENABLE);
					TestData.LockTest = ENABLE;
					PrintCurrState++;
					break;
				case 2:
					num += TestData.SegLedCurrMax;
					_Display_PrintChar(num, DISABLE);
					TestData.LockTest = ENABLE;
					PrintCurrState++;
					break;
				case 3:
					_Display_PrintChar('.', ENABLE);
					TestData.LockTest = DISABLE;
					PrintCurrState = 0;
					break;
				default:
					PrintCurrState = 0;
					break;
			}
		}

		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		// Test 7-seg display
		static TestState_e TestState = TEST_IDLE_STATE;
		ErrorStatus TestErrState = SUCCESS;
		switch (TestState)
		{
			case TEST_IDLE_STATE:
				if( Button == BTN_S1 && TestData.LockTest == DISABLE )
					TestState++;
				break;
			case TEST_START_STATE:
				TestData.SegCurrMeanSum = 0;
				TestData.SegCurrMeanCnt = 0;
				LED_SetState(LED_Ext_G, DO_ON);
				LED_SetState(LED_Ext_R, DO_ON);
//				LED_SetState(LED_2, DO_ON);
				for (u32 i = 0; i < DIS_SEG_NB; ++i) { // Off all segments
					Display_SetSegment(i, DO_OFF, DisplayType);
				}
				_TesterSteps_DisplaySegTest(TestState, DIS_SEG_A, _MAIN_TASK_PERIOD_MS, NULL, ENABLE); // Reset step
				TestState++;
				break;
			case TEST_SEG_A_STATE:
				TestState = _TesterSteps_DisplaySegTest(TestState, DIS_SEG_A, _MAIN_TASK_PERIOD_MS, &TestErrState, DISABLE);
				if( TestErrState == ERROR ) {
					TestState = TEST_ERROR_STATE;
				}
				break;
			case TEST_SEG_B_STATE:
				TestState = _TesterSteps_DisplaySegTest(TestState, DIS_SEG_B, _MAIN_TASK_PERIOD_MS, &TestErrState, DISABLE);
				if( TestErrState == ERROR ) {
					TestState = TEST_ERROR_STATE;
				}
				break;
			case TEST_SEG_C_STATE:
				TestState = _TesterSteps_DisplaySegTest(TestState, DIS_SEG_C, _MAIN_TASK_PERIOD_MS, &TestErrState, DISABLE);
				if( TestErrState == ERROR ) {
					TestState = TEST_ERROR_STATE;
				}
				break;
			case TEST_SEG_D_STATE:
				TestState = _TesterSteps_DisplaySegTest(TestState, DIS_SEG_D, _MAIN_TASK_PERIOD_MS, &TestErrState, DISABLE);
				if( TestErrState == ERROR ) {
					TestState = TEST_ERROR_STATE;
				}
				break;
			case TEST_SEG_E_STATE:
				TestState = _TesterSteps_DisplaySegTest(TestState, DIS_SEG_E, _MAIN_TASK_PERIOD_MS, &TestErrState, DISABLE);
				if( TestErrState == ERROR ) {
					TestState = TEST_ERROR_STATE;
				}
				break;
			case TEST_SEG_F_STATE:
				TestState = _TesterSteps_DisplaySegTest(TestState, DIS_SEG_F, _MAIN_TASK_PERIOD_MS, &TestErrState, DISABLE);
				if( TestErrState == ERROR ) {
					TestState = TEST_ERROR_STATE;
				}
				break;
			case TEST_SEG_G_STATE:
				TestState = _TesterSteps_DisplaySegTest(TestState, DIS_SEG_G, _MAIN_TASK_PERIOD_MS, &TestErrState, DISABLE);
				if( TestErrState == ERROR ) {
					TestState = TEST_ERROR_STATE;
				}
				break;
			case TEST_SEG_DOT_STATE:
				if( DisplayType == DIS_COMMON_ANODE )
				{
					TestState = _TesterSteps_DisplaySegTest(TestState, DIS_SEG_DOT, _MAIN_TASK_PERIOD_MS, &TestErrState, DISABLE);
					if( TestErrState == ERROR ) {
						TestState = TEST_ERROR_STATE;
					}
				}
				else
				{
					TestState = TEST_SUCCESS_STATE;
				}
				break;
			case TEST_SUCCESS_STATE:
				TestData.SegCurrMean = TestData.SegCurrMeanSum / TestData.SegCurrMeanCnt;
				LED_SetState(LED_Ext_G, DO_ON);
				LED_SetState(LED_Ext_R, DO_OFF);
//				LED_SetState(LED_2, DO_OFF);
				TestState = TEST_IDLE_STATE;
				break;
			case TEST_ERROR_STATE:
				LED_SetState(LED_Ext_G, DO_OFF);
				LED_SetState(LED_Ext_R, DO_ON);
//				LED_SetState(LED_2, DO_OFF);
				TestState = TEST_IDLE_STATE;
				break;
			default:
				TestState = TEST_IDLE_STATE;
				break;
		}
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#endif
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

}


/* Private functions ---------------------------------------------------------*/

/**
 * @brief _ADC_Init
 * @note
 * @param none
 * @retval none
 */
static void _ADC_Init(void)
{
	// ADC automatic self-calibration
	HAL_ADCEx_Calibration_Start(&hadc);
	// Enables ADC, starts conversion of regular group and transfers result through DMA
	HAL_ADC_Start_DMA(&hadc, (u32*)AdcMeasureData.RawAdc, _ADC_TOTAL_CHANNEL_NB);
}

/**
 * @brief _AdcRawToVolt_mV
 * @note
 * @param [in]_AdcChannelIndex: Max value is _ADC_TOTAL_CHANNEL_NB.
 * @retval [mV]
 */
static u32 _AdcRawToVolt_mV(u16 _AdcChannelIndex)
{
	u32 _Volt;
	// Check channel index
	_AdcChannelIndex = (_AdcChannelIndex > _ADC_TOTAL_CHANNEL_NB) ? 0 : _AdcChannelIndex;

	/* Computation of ADC conversions raw data to physical values */
	_Volt = __LL_ADC_CALC_DATA_TO_VOLTAGE(_ADC_REF_mV, AdcMeasureData.MeanAdc[_AdcChannelIndex], LL_ADC_RESOLUTION_12B);

	return _Volt;
}

/**
 * @brief _Button_ProcessingTaks
 * @note
 * @param none
 * @retval none
 */
static void _Button_ProcessingTaks(void)
{
	FlagStatus ButtonStatus[BTN_NB];
	static FlagStatus ButtonStatus_prev[BTN_NB];

	ButtonStatus[BTN_S1] = GPIO_GetInput(DI_Button1);
	ButtonStatus[BTN_S2] = GPIO_GetInput(DI_Button2);

	// Detect rising edge
	if( ButtonStatus[BTN_S1] == SET &&
		ButtonStatus_prev[BTN_S1] == RESET )
	{
		fifo_putData(&ButtonFifoRxData, (u8)BTN_S1);
	}
	if( ButtonStatus[BTN_S2] == SET &&
		ButtonStatus_prev[BTN_S2] == RESET )
	{
		fifo_putData(&ButtonFifoRxData, (u8)BTN_S2);
	}
	// Remember last buttons states
	memcpy(ButtonStatus_prev, ButtonStatus, sizeof(ButtonStatus));
}

/**
 * @brief _Button_GetLastPressed
 * @note
 * @param none
 * @retval BtnType_e
 */
static BtnType_e _Button_GetLastPressed(void)
{
	u8 Data;
	BtnType_e Button = BTN_NONE;

	if( fifo_getData(&ButtonFifoRxData, &Data) ) {
		Button = (BtnType_e)Data;
	}

	return Button;
}

/**
 * @brief _Current_Check
 * @note
 * @param [in]_Curr_mA:
 * @param [in]_MinCurr_mA:
 * @param [in]_MaxCurr_mA:
 * @retval ErrorStatus
 */
static ErrorStatus _Current_Check(u32 _Curr_mA, u32 _MinCurr_mA, u32 _MaxCurr_mA)
{
	if( _Curr_mA >= _MinCurr_mA && _Curr_mA <= _MaxCurr_mA ) {
		return SUCCESS;
	}
	else {
		return ERROR;
	}
}

/**
 * @brief _Timeout
 * @note
 * @param [in]_Timeout_ms:
 * @param [in]_TaskPeriod_ms:
 * @param [in]_ResetTimeState:
 * @retval FlagStatus
 */
static FlagStatus _Timeout(u32 _Timeout_ms, u32 _TaskPeriod_ms, FunctionalState _ResetTimeState)
{
	FlagStatus TimeoutEnd_Flag = RESET;
	static u32 DelayTimeCnt = 0;

	if( _ResetTimeState == ENABLE )
	{
		DelayTimeCnt = 0;
		return TimeoutEnd_Flag;
	}

	if( DelayTimeCnt == 0 )
	{
		DelayTimeCnt = _Timeout_ms / _TaskPeriod_ms;
	}
	else
	{
		if( (--DelayTimeCnt) == 0 )
		{
			TimeoutEnd_Flag = SET;
		}
	}

	return TimeoutEnd_Flag;
}

/**
 * @brief _TesterSteps_DisplaySegTest
 * @note
 * @param [in]_MainState:
 * @param [in]_Seg:
 * @param [in]_TaskPeriod_ms:
 * @param [in]_ResetState:
 * @retval none
 */
static u32 _TesterSteps_DisplaySegTest(u32 _MainState, DisSegType_e _Seg, u32 _TaskPeriod_ms, ErrorStatus *_pErrStatus, FunctionalState _ResetState)
{
	u32 MainState = _MainState;
	static u32 TestState = 0;
	u16 Current_mA = AnalogData.CurrSens2.Current_mA;
	#define _OUT_CHANGE_TIMEOUT_MS				(50) // [ms]

	if( _ResetState == ENABLE )
	{
		TestState = 0;
		_Timeout(0, _TaskPeriod_ms, ENABLE);
//		Display_SetSegment(_Seg, DO_OFF, DisplayType);
		return MainState;
	}

	switch (TestState)
	{
		case 0: // Turn OFF output
			Display_SetSegment(_Seg, DO_OFF, DisplayType);
			TestState++;
			break;
		case 1: // Wait...
			if( _Timeout(_OUT_CHANGE_TIMEOUT_MS, _TaskPeriod_ms, DISABLE) == SET ) {
				TestState++;
			}
			break;
		case 2: // Check current and turn OFF output.
			if( Current_mA > TestData.SegLedCurrMin)
			{
				if( _pErrStatus != NULL )
				{
					*_pErrStatus = ERROR;
					TestData.SegCurrMean = Current_mA;
				}
				MainState++; // Go to next main state
				TestState = 0;
			}
			else
			{
				TestState++;
			}
			break;
		case 3: // Turn ON output
			Display_SetSegment(_Seg, DO_ON, DisplayType);
			TestState++;
			break;
		case 4: // Wait...
			if( _Timeout(_OUT_CHANGE_TIMEOUT_MS, _TaskPeriod_ms, DISABLE) == SET ) {
				TestState++;
			}
			break;
		case 5: // Check current and turn OFF output

			if( _pErrStatus != NULL )
			{
				*_pErrStatus = _Current_Check(Current_mA, TestData.SegLedCurrMin, TestData.SegLedCurrMax);

				if( *_pErrStatus == SUCCESS )
				{
					TestData.SegCurrMeanSum += Current_mA;
					TestData.SegCurrMeanCnt++;
				}
				else
				{
					TestData.SegCurrMean = Current_mA;
				}
			}

			Display_SetSegment(_Seg, DO_OFF, DisplayType);

			MainState++; // Go to next main state
			TestState = 0;

			break;
		default:
			TestState = 0;
			break;
	}

	#undef _OUT_CHANGE_TIMEOUT_MS
	return MainState;
}

/**
 * @brief _Display_PrintChar.
 * @note
 * @param [in]_Char:
 * @param [in]_DotOnOff:
 * @retval none
 */
static void _Display_PrintChar(char _Char, FunctionalState _DotOnOff)
{
    u8 outData = Decode_7seg(_Char);

    for (u32 i = 0; i < DIS_SEG_NB; ++i)
    {
        if( outData & (1<<i) )
        	Display_SetSegment((DisSegType_e)i, DO_ON, DisplayType);
        else
        	Display_SetSegment((DisSegType_e)i, DO_OFF, DisplayType);

        if( i == DIS_SEG_DOT && _DotOnOff == ENABLE ) {
        	Display_SetSegment(DIS_SEG_DOT, DO_ON, DisplayType);
        }
	}
}

//#endif /* LIB_MODULE_ENABLED */ // plik.c
