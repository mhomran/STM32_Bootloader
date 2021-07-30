/**
 * @file PduR.h
 * @author Mohamed Hassanin Mohamed
 * @brief A PDU router to different modules interfaces e.g. UART.
 * @version 0.1
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef PDU_R_H
#define PDU_R_H
/******************************************************************************
* Includes
******************************************************************************/
#include <inttypes.h>
#include "Std_Types.h"
/******************************************************************************
* Defines
******************************************************************************/
#define PDU_TYPE_DATA_RECORD                          ((PDUType_t)0)
#define PDU_TYPE_EOF_RECORD                           ((PDUType_t)1)
#define PDU_TYPE_EXTENDED_LINEAR_ADDR_RECORD          ((PDUType_t)4)
/******************************************************************************
* Typedefs
******************************************************************************/
typedef enum {
  PDU_TYPE_START = 5,
  PDU_TYPE_LOCK_FLASH,
  PDU_TYPE_UNLOCK_FLASH,
  PDU_TYPE_ERASE_SECTOR,
  PDU_TYPE_ERASE_IMAGE,
  PDU_TYPE_RESET,
  PDU_TYPE_ACK,
  PDU_TYPE_ERR,
  PDU_TYPE_END
} PDUType_t;

typedef struct {
  uint8_t len;
  uint8_t* data;
  /**
   * @brief Pointer to the meta data (e.g. CAN ID,
    socket ID, diagnostic addresses) of the
    PDU, consisting of a sequence of meta
    data items. The length and type of the
    meta data items is statically configured
    for each PDU. Meta data items with
    more than 8 bits use platform byte order.
   * 
   */
  uint8_t* metadata;
} PduInfo_t;

typedef uint32_t PduId_t;
/******************************************************************************
* Function Prototypes
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

void PduR_Init(void);
StdReturn_t PduR_Transmit(PduId_t, PduInfo_t*);
void PduR_RxIndication(PduId_t, PduInfo_t*);
void PduR_TxConfirmation(PduId_t);

#ifdef __cplusplus
}
#endif

#endif
/***************************** END OF FILE ***********************************/