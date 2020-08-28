/*
 * FIFO_Buffer.c
 *
 *  Created on: May 8, 2020
 *      Author: DBabraj
 * 		Version: 1.0
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32inc.h"
#include "FIFO_Buffer.h"

/********************** NOTES **********************************************
...
*******************************************************************************/

//#ifdef LIB_MODULE_ENABLED // plik.c


/* Private define ------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

#define _FIFO_INCR(x, FIFO_SIZE)	(((x)+1)&((FIFO_SIZE)-1))

/* Private typedef -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Public functions --------------------------------------------------------*/

/**
 * @brief fifo_putData
 * @note add data to FIFO
 * @param [in/out]*_pFifo:
 * @param [in]_dataIn:
 * @retval 0 -> buffer overrun, 1 -> success
 */
u8 fifo_putData(FIFO_TypeDef *_pFifo, u8 _dataIn)
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
u8 fifo_getData(FIFO_TypeDef *_pFifo, u8 *_dataOut)
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
u16 fifo_getNumAvailableData(FIFO_TypeDef *_pFifo)
{
	const u16 _BUFFER_MASK = _pFifo->BufferSize - 1;
	return ((u16)(_BUFFER_MASK + _pFifo->Head - _pFifo->Tail)) % _BUFFER_MASK;
}

/* Private functions ---------------------------------------------------------*/


//#endif /* LIB_MODULE_ENABLED */ // plik.c
