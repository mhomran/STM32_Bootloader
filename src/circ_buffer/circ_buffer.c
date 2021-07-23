/**
 * @file circ_buffer.c
 * @author Mohamed Hassanin 
 * @brief A circular buffer/queue module. 
 * Note: one space is wasted in the buffer to know if it's empty
 * @version 0.1
 * @date 2021-02-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */
/*********************************************************************
 * definitions
**********************************************************************/
#define NULL 0x0

/*******************************************************************
 * Includes
**********************************************************************/
#include <inttypes.h>
#include "circ_buffer.h"

/*********************************************************************
 * Prototypes
**********************************************************************/
static uint8_t CircBuff_IsEmpty(CircBuff_t* Buff);
static uint8_t CircBuff_IsFull(CircBuff_t* Buff);

/*********************************************************************
 * Private functions definitions
**********************************************************************/
/*********************************************************************
* Function : CircBuff_IsFull()
*//**
* \b Description:
*
* Utility function is used to check if the circuler buffer is full
*
* @param Buff a valid pointer to the circuler buffer
*
* @return uint8_t 1 if the buffer is full, 0 otherwise
*
**********************************************************************/
uint8_t 
CircBuff_IsFull(CircBuff_t* Buff)
{
  return ((Buff->Front + 1) % Buff->Size) == Buff->Rear;
}

/*********************************************************************
* Function : CircBuff_IsEmpty()
*//**
* \b Description:
*
* Utility function is used to check if the circuler buffer is empty
*
* @param Buff a valid pointer to the circuler buffer
*
* @return uint8_t 1 if the buffer is empty, 0 otherwise
*
**********************************************************************/
uint8_t 
CircBuff_IsEmpty(CircBuff_t* Buf)
{
  // define empty as head == tail
  return (Buf->Front == Buf->Rear);
}

/*********************************************************************
 * Public functions definitions
**********************************************************************/
/*********************************************************************
* Function : CircBuff_Create()
*//**
* \b Description:
*
* This function is used to create a circuler buffer.
*
* @param BuffData a valid pointer an allocated piece of memory for the buffer
* @param Size the size of the piece of memory.
*
* @return CircBuff_t The created buffer.
*
* \b Example:
* @code
* uint8_t UartBuffer[MAX_UART_BUFF_SIZE];
* CircBuff_t UartBuff = CircBuff_Create(UartBuffer, MAX_UART_BUFF_SIZE);
* @endcode
*
**********************************************************************/
extern CircBuff_t
CircBuff_Create(uint8_t* BuffData, uint8_t Size) {
  CircBuff_t Buff;
  Buff.Data = BuffData;
  Buff.Size = Size;

  CircBuff_Reset(&Buff);

  return Buff;
}

/*********************************************************************
* Function : CircBuff_Reset()
*//**
* \b Description:
*
* This function is used to reset a circuler buffer.
*
* @param Buff a valid pointer to the buffer
*
* @return void
*
* \b Example:
* @code
* uint8_t UartBuffer[MAX_UART_BUFF_SIZE];
* CircBuff_t UartBuff = CircBuff_Create(UartBuffer, MAX_UART_BUFF_SIZE);
* CircBuff_Enqueue(&UartBuff, 'a');
* CircBuff_Reset(&UartBuff); //now it's empty again.
* @endcode
*
* @see CircBuff_Create
**********************************************************************/
extern void 
CircBuff_Reset(CircBuff_t* Buff)
{
  Buff->Front = 0;
  Buff->Rear = 0;
}

/*********************************************************************
* Function : CircBuff_Dequeue()
*//**
* \b Description:
*
* This function is used to dequeue from a circuler buffer
*
* @param Buff a valid pointer to the circuler buffer
* @param Data a pointer to store the dequeued Data in.
* @return uint8_t 1 if there's a valid Data returned, 0 otherwise
*
*
* \b Example:
* @code
* uint8_t UartBuffer[MAX_UART_BUFF_SIZE];
* CircBuff_t UartBuff = CircBuff_Create(UartBuffer, MAX_UART_BUFF_SIZE);
* CircBuff_Enqueue(&UartBuff, 'a');
* uint8_t RcvData;
* CircBuff_Dequeue(&UartBuff, &RcvData); //RcvData now has 'a' and the buffer is empty
* @endcode
*
* @see CircBuff_Create
* @see CircBuff_Enqueue
**********************************************************************/
extern uint8_t 
CircBuff_Dequeue(CircBuff_t* Buff, uint8_t * Data)
{
  uint8_t r = 0;

  if(Buff != NULL && Data != NULL && CircBuff_IsEmpty(Buff) != 1)
    {
      *Data = Buff->Data[Buff->Rear];
      Buff->Rear = (Buff->Rear + 1) % Buff->Size;

      r = 1;
    }

  return r;
}

/*********************************************************************
* Function : CircBuff_Enqueue()
*//**
* \b Description:
*
* This function is used to enqueue into a circuler buffer
*
* @param Buff a valid pointer to the circuler buffer
* @param Data a byte to add to the queue.
* @return uint8_t 1 if the byte is stored and 0 otherwise.
*
* \b Example:
* @code
* uint8_t UartBuffer[MAX_UART_BUFF_SIZE];
* CircBuff_t UartBuff = CircBuff_Create(UartBuffer, MAX_UART_BUFF_SIZE);
* CircBuff_Enqueue(&UartBuff, 'a'); // now the buffer has 'a'
* @endcode
*
* @see CircBuff_Create
**********************************************************************/
extern uint8_t
CircBuff_Enqueue(CircBuff_t* Buff, uint8_t Data)
{
  uint8_t r = 0;

  if(Buff != NULL && CircBuff_IsFull(Buff) != 1)
    {
      Buff->Data[Buff->Front] = Data;
      Buff->Front = (Buff->Front + 1) % Buff->Size;

      r = 1;
    }

  return r;
}

/*********************************************************************
* Function : CircBuff_PeekLast()
*//**
* \b Description:
*
* This function is used to peak the tail of a circuler buffer
*
* @param Buff a valid pointer to the circuler buffer
* @param Data a pointer to store the peeked byte in.
* @return uint8_t 1 if the byte is stored and 0 otherwise.
*
* \b Example:
* @code
* uint8_t UartBuffer[MAX_UART_BUFF_SIZE];
* CircBuff_t UartBuff = CircBuff_Create(UartBuffer, MAX_UART_BUFF_SIZE);
* CircBuff_Enqueue(&UartBuff, 'a'); // now the buffer has 'a'
* uint8_t x;
* CircBuff_PeekLast(&UartBuff, &x);
* @endcode
*
* @see CircBuff_PeekLast
**********************************************************************/
extern uint8_t 
CircBuff_PeekLast(CircBuff_t* Buff, uint8_t * Data)
{
  uint8_t r = 0;

  if(Buff != NULL && Data != NULL && CircBuff_IsEmpty(Buff) != 1)
    {
      //just read the rear
      uint8_t toPeekOn;

      if(Buff->Front == 0) toPeekOn = Buff->Size - 1;
      else toPeekOn = Buff->Front - 1;

      *Data = Buff->Data[toPeekOn];

      r = 1;
    }

  return r;
}

/************************End Of File ******************************/