/*
 * FIFO_Buffer.h
 *
 *  Created on: May 8, 2020
 *      Author: DBabraj
 * 		Version: 1.0
 */

#ifndef _FIFO_BUFFER_H_
#define _FIFO_BUFFER_H_

#ifdef __cplusplus // plik.h
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/


/********************** NOTES *************************************************
...
//	Size of the circular receive buffer, must be power of 2
#define _FIFO_RX_BUFFER_SIZE					(64)

#if ( _FIFO_RX_BUFFER_SIZE & (_FIFO_RX_BUFFER_SIZE - 1) )
#	error RX buffer size is not a power of 2
#endif

u8 _FifoRxBuff[_FIFO_RX_BUFFER_SIZE] = {0};
FIFO_TypeDef FifoRxData = { .BufferSize = _FIFO_RX_BUFFER_SIZE, .pBuffer = _FifoRxBuff };

*******************************************************************************/

/* Public define -----------------------------------------------------------*/

/* Public macros -----------------------------------------------------------*/

/* Public typedef ----------------------------------------------------------*/

// Structure of FIFO
typedef struct _FIFO
{
	uc32 BufferSize; // must be power of 2
    u8 *pBuffer;
	u32 Tail;
	u32 Head;
}FIFO_TypeDef;


/* Public constants --------------------------------------------------------*/

/* Public variables --------------------------------------------------------*/

/* Public function prototypes ----------------------------------------------*/

u8 fifo_putData(FIFO_TypeDef *_pFifo, u8 _dataIn);
u8 fifo_getData(FIFO_TypeDef *_pFifo, u8 *_dataOut);

u16 fifo_getNumAvailableData(FIFO_TypeDef *_pFifo);



#ifdef __cplusplus // plik.h
}
#endif


#endif /* _FIFO_BUFFER_H_ */
