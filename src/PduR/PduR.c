
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
#include <stdio.h>
#include "PduR.h"
#include "PduR_Cfg.h"
#include "port.h"
#include "error.h"
#include "UartIf.h"
#include "BootIf.h"

typedef enum {
  PDU_DIR_TX,
  PDU_DIR_RX
} PduDirection_t;


const PduRCfg_t* gpTxPduCfg;
const PduRCfg_t* gpRxPduCfg;

static StdReturn_t PduR_GetModuleIf(PduDirection_t dir, PduModuleIf_t* res, PduId_t id);

void 
PduR_Init(void)
{
  gpTxPduCfg = PduR_TxGetConfig();
  gpRxPduCfg = PduR_RxGetConfig();
}

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

