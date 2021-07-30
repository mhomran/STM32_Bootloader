
/**
 * @file PduR.c
 * @author Mohamed Hassanin Mohamed
 * @brief A packet handler for HEX packets, it checks the checksum and
 * call the bootloader to determine the action.
 * @version 0.1
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
/******************************************************************************
* Includes
******************************************************************************/
#include <stdio.h>
#include "PduR.h"
#include "PduR_Cfg.h"
#include "port.h"
#include "error.h"
#include "UartIf.h"
#include "BootIf.h"
/******************************************************************************
* typedefs
******************************************************************************/
typedef enum {
  PDU_DIR_TX,
  PDU_DIR_RX
} PduDirection_t;

/******************************************************************************
* Module Variable Definitions
******************************************************************************/
const PduRCfg_t* gpTxPduCfg;
const PduRCfg_t* gpRxPduCfg;

static StdReturn_t PduR_GetModuleIf(PduDirection_t dir, PduModuleIf_t* res, PduId_t id);
/******************************************************************************
* Functions definitions
******************************************************************************/

/**
 * @brief Initialize the PDU router layer.
 * 
 */
void 
PduR_Init(void)
{
  gpTxPduCfg = PduR_TxGetConfig();
  gpRxPduCfg = PduR_RxGetConfig();
}

/**
 * @brief Get the correct module (UART, CAN, etc.) based on the PDU id
 * and the direction of the message (transmitted or received).
 * 
 * @param dir the direction of the message.
 * @param res The Module 
 * @param id The PDU ID
 * @return StdReturn_t 
 */
static StdReturn_t 
PduR_GetModuleIf(PduDirection_t dir, PduModuleIf_t* res, PduId_t id)
{
  StdReturn_t state = E_NOT_OK;

  if(dir == PDU_DIR_TX)
    {
      for(uint32_t i = 0; i < PDUS_TX_NO; i++)
        {
          if(gpTxPduCfg[i].id == id)
            {
              *res = gpTxPduCfg[i].HWM;
              state = E_OK;
              break;
            }
        }
    }
  else
    {
      for(uint32_t i = 0; i < PDUS_RX_NO; i++)
        {
          if(gpRxPduCfg[i].id == id)
            {
              *res = gpRxPduCfg[i].HWM;
              state = E_OK;
              break;
            }
        }
    }
  return state;
}


/**
 * @brief Transmit a PDU given its ID and info.
 * 
 * @param id The PDU id
 * @param pdu The PDU info
 * @return StdReturn_t 
 */
StdReturn_t 
PduR_Transmit(PduId_t id, PduInfo_t* pdu)
{
  PduModuleIf_t HWM;
  StdReturn_t state = E_OK;

  state = PduR_GetModuleIf(PDU_DIR_TX, &HWM, id);

  if(state == E_NOT_OK)
    {
      return E_NOT_OK;
    }
  
  switch(HWM)
    {
      case PDU_MODULE_IF_UART:
        {
          state = UartIf_Transmit(id, pdu);
        }
        break;
      default:
        {
          state = E_NOT_OK;
        }
        break;
    }

  return state;
}


/**
 * @brief A call back function to be called from the UART interface layer
 *  upon receiving a PDU
 * 
 * @param id the PDU ID of the received the PDU.
 * @param pdu The PDU received.
 */
void 
PduR_RxIndication(PduId_t id, PduInfo_t* pdu)
{
  switch (id)
  {
  case PDU_RX_ID_BOOT_UART:
    {
      BootIf_RxIndication(id, pdu);
    }
    break;

  default:
    break;
  }
}

/**
 * @brief A call back function to be called from the UART interface layer
 *  upon transmitting a PDU
 * 
 * @param id the PDU ID of the transmitted the PDU.
 */
void 
PduR_TxConfirmation(PduId_t id)
{
  switch (id)
  {
  case PDU_RX_ID_BOOT_UART:
    {
      BootIf_TxConfirmation(id);
    }
    break;

  default:
    break;
  }
}
/***************************** END OF FILE ***********************************/