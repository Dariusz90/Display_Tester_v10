/*
 * Button.c
 *
 *  Created on: 2 wrz 2020
 *      Author: DBabraj
 * 		Version: 1.0
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32inc.h"
#include "Button.h"

/********************** NOTES **********************************************
...
*******************************************************************************/

//#ifdef LIB_MODULE_ENABLED // plik.c


/* Private define ------------------------------------------------------------*/

#define _SHORT_PRESSED_SEQUENCE_ms				(50)	// short pressing sequence length, given in BUTTON_TASK_MS_PERIOD times
#define _LONG_PRESSED_SEQUENCE_ms				(500)   // long pressing sequence length, given in BUTTON_TASK_MS_PERIOD times
#define _START_BUSRT_LONG_PRESS_NB				(3)

//	Size of the circular receive buffer, must be power of 2
#define _BUTTON_QUEUE_SIZE				(8)

#if ( _BUTTON_QUEUE_SIZE & (_BUTTON_QUEUE_SIZE - 1) )
#	error RX buffer size is not a power of 2
#endif

/* Private macros ------------------------------------------------------------*/

//#define _FIFO_INCR(x, FIFO_SIZE)				(((x)+1)&((FIFO_SIZE)-1)) // Size of the circular receive buffer, must be power of 2
#define _FIFO_INCR(x, FIFO_SIZE)				(((x)+1)%(FIFO_SIZE))

//#define _READ_BUTTON_MASK(_Button_)				(u16)(1<<(_Button_))
/* Private typedef -----------------------------------------------------------*/

// Structure of FIFO
typedef struct _FIFO
{
	uc32 BufferSize; // must be power of 2
    u16 *pBuffer;
	u32 Tail;
	u32 Head;
}FIFO_TypeDef;

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

static u16 Button_RxBuff[_BUTTON_QUEUE_SIZE] = {0};
static FIFO_TypeDef ButtonFifoRxData = { .BufferSize = _BUTTON_QUEUE_SIZE, .pBuffer = Button_RxBuff, .Tail = 0, .Head = 0 };

/* Private function prototypes -----------------------------------------------*/
static u8 _fifo_getData(FIFO_TypeDef *_pFifo, u16 *_dataOut);

static void _PutPressedButtonIntoFifio(ButtonData_t _pBtnData[], u16 _NbRead, FlagStatus _PressedButton, uc32 _TaskPeriod_ms);

/* Public functions --------------------------------------------------------*/

/**
 * @brief Button_Init
 * @note Initialization of BUTTON's sequence catching task.
 * @param [in]_pBtnData:
 * @param [in]_ButtonNum: Number of buttons to read
 * @retval none
 */
void Button_Init(ButtonData_t _pBtnData[], uc32 _ButtonNum)
{
	for(u32 i = 0; i < _ButtonNum; i++)
	{
		_pBtnData[i].Timer = 0;
		_pBtnData[i].LongPressCounter = 0;
	}
}

/**
 * @brief Button_ProcessingTask
 * @note Initialization of BUTTON's sequence catching task.
 * @param [in]_pBtnData:
 * @param [in]_ButtonNum: Number of buttons to read
 * @param [in]_TaskPeriod_ms:
 * @retval none
 */
void Button_ProcessingTask(ButtonData_t _pBtnData[], uc32 _ButtonNum, uc32 _TaskPeriod_ms)
{
	// read all pins
	for(u32 i = 0; i < _ButtonNum; i++)
	{
		if( _pBtnData[i].ReadButtonPinFun != NULL )
		{
			_PutPressedButtonIntoFifio(_pBtnData, i, _pBtnData[i].ReadButtonPinFun(), _TaskPeriod_ms);
		}
	}
}

/**
 * @brief Button_GetLastPressed
 * @note
 * @param none
 * @retval
 */
u16 Button_GetLastPressed(void)
{
	u16 Button = 0;

	if( _fifo_getData(&ButtonFifoRxData, &Button) ) {
		return Button;
	}

	return Button;
}

#if 0 // Example:
/**
 * @brief _GetButton
 * @note Return pressed button, according to device orientation.
 * @param None
 * @retval ButtonType_e
 */
static ButtonType_e _GetButton(void)
{
	u16 key = Button_GetLastPressed();

	switch( key )
	{
		case (BUTTON_END_SEQ + 0x0000):
			return BUTTON_MODE_START_END;
		case (BUTTON_END_SEQ + 0x0001):
			return BUTTON_PROG_SET_END;

		//*****************************

		case (BUTTON_SHORT_SEQ + 0x0000):
			return BUTTON_MODE_START_SHORT;
		case (BUTTON_SHORT_SEQ + 0x0001):
			return BUTTON_PROG_SET_SHORT;

		//*****************************

		case (BUTTON_LONG_SEQ + 0x0000):
			return BUTTON_MODE_START_LONG;
		case (BUTTON_LONG_SEQ + 0x0001):
			return BUTTON_PROG_SET_LONG;

		//*****************************

		case (BUTTON_BURST_SEQ + 0x0000):
			return BUTTON_MODE_START_BURST;
		case (BUTTON_BURST_SEQ + 0x0001):
			return BUTTON_PROG_SET_BURST;
	}
	return BUTTON_NONE;
}
#endif

/* Private functions ---------------------------------------------------------*/

/**
 * @brief fifo_putData
 * @note add data to FIFO
 * @param [in/out]*_pFifo:
 * @param [in]_dataIn:
 * @retval 0 -> buffer overrun, 1 -> success
 */
static u8 _fifo_putData(FIFO_TypeDef *_pFifo, u16 _dataIn)
{
	if( _FIFO_INCR(_pFifo->Head, _pFifo->BufferSize) == _pFifo->Tail )
		return 0;  // overrun
	else
    {
		if( _pFifo->pBuffer != NULL )
			_pFifo->pBuffer[_pFifo->Head] = _dataIn;
		_pFifo->Head = _FIFO_INCR(_pFifo->Head, _pFifo->BufferSize);
    }
	return 1;
}

/**
 * @brief fifo_getData
 * @note get data from FIFO
 * @param [in/out]*_pFifo:
 * @param [out]*_dataOut:
 * @retval 0 -> buffer overrun, 1 -> success
 */
static u8 _fifo_getData(FIFO_TypeDef *_pFifo, u16 *_dataOut)
{
	if( _pFifo->Head == _pFifo->Tail )
		return 0;
	if( _pFifo->pBuffer != NULL )
		*_dataOut = _pFifo->pBuffer[_pFifo->Tail];
	_pFifo->Tail = _FIFO_INCR(_pFifo->Tail, _pFifo->BufferSize);
	return 1;
}

/**
 * @brief fifo_getNumAvailableData
 * @note
 * @param [in]*_pFifo:
 * @retval numbers of bytes available  in FIFO buffer
 */
//static u16 _fifo_getNumAvailableData(FIFO_TypeDef *_pFifo)
//{
//	const u16 _BUFFER_MASK = _pFifo->BufferSize - 1;
//	return ((u16)(_BUFFER_MASK + _pFifo->Head - _pFifo->Tail)) % _BUFFER_MASK;
//}

/**
 * @brief _WriteKey
 * @note
 * @param [in] _Button:
 * @retval none
 */
static void _WriteKey(u16 _Button)
{
	_fifo_putData(&ButtonFifoRxData, _Button);
}

/**
 * @brief _PutPressedButtonIntoFifio
 * @note This function puts pressed button into fifo.
 * @param [in]_pBtnData:
 * @param [in]_NbRead:
 * @param [in]_PressedButton:
 * @param [in]_TaskPeriod_ms:
 * @retval none
 */
static void _PutPressedButtonIntoFifio(ButtonData_t _pBtnData[], u16 _NbRead, FlagStatus _PressedButton, uc32 _TaskPeriod_ms)
{
	// check if button pressed
	if(_PressedButton == SET)
	{
		_pBtnData[_NbRead].Timer += _TaskPeriod_ms;
		if(_pBtnData[_NbRead].Timer >= _LONG_PRESSED_SEQUENCE_ms)
		{
			if(_pBtnData[_NbRead].LongPressCounter >= _START_BUSRT_LONG_PRESS_NB)
			{
				_WriteKey(_NbRead | BUTTON_BURST_SEQ);
			}
			else
			{
				_pBtnData[_NbRead].LongPressCounter++;
				_WriteKey(_NbRead | BUTTON_LONG_SEQ);
			}
			_pBtnData[_NbRead].Timer = 0;
		}
	}
	else
	{
		// check if long was pressed, if button released and long was pressed then no press state
		if(_pBtnData[_NbRead].LongPressCounter == 0)
		{
			if(_pBtnData[_NbRead].Timer >= _SHORT_PRESSED_SEQUENCE_ms)
				_WriteKey(_NbRead | BUTTON_SHORT_SEQ);
		}

		if(_pBtnData[_NbRead].Timer >= _SHORT_PRESSED_SEQUENCE_ms)
			_WriteKey(_NbRead | BUTTON_END_SEQ);

		if((_pBtnData[_NbRead].LongPressCounter != 0) && (_pBtnData[_NbRead].Timer == 0))
			_WriteKey(_NbRead | BUTTON_END_SEQ);

		_pBtnData[_NbRead].Timer = 0;
		_pBtnData[_NbRead].LongPressCounter = 0;
	}
}

//#endif /* LIB_MODULE_ENABLED */ // plik.c
