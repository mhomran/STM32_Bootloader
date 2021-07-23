/**
 * @file circ_buffer.h
 * @author Mohamed Hassanin 
 * @brief A circular buffer/queue module. 
 * Note: one space is wasted in the buffer to know if it's empty
 * @version 0.1
 * @date 2021-02-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef CIRC_BUFFER_H
#define CIRC_BUFFER_H

/*******************************************************************
 * Includes
*******************************************************************/
#include <inttypes.h>
/*******************************************************************
 * typedefs
*******************************************************************/
/**
 * @brief Circular Buffer structure to hold the information of it.
 * 
 */
typedef struct CircBuff {
    uint8_t Rear; /*< the Rear of the queue */
    uint8_t Front; /*< the Front of the queue */
    uint8_t* Data; /*< a pointer to the buffer Data */
    uint8_t Size; /*< the Size of the buffer */
}CircBuff_t;
/*******************************************************************
 * Prototypes
*******************************************************************/
extern CircBuff_t CircBuff_Create(uint8_t* BuffData, uint8_t Size);
extern void CircBuff_Reset(CircBuff_t* Buff);
extern uint8_t CircBuff_Dequeue(CircBuff_t* Buff, uint8_t * Data);
extern uint8_t CircBuff_Enqueue(CircBuff_t* Buff, uint8_t Data);
extern uint8_t CircBuff_PeekLast(CircBuff_t* Buff, uint8_t * Data);

#endif /* end CIRC_BUFFER_H */
/************************End Of File ******************************/