/**
 * @file PduR_Cfg.c
 * @author Mohamed Hassanin Mohamed
 * @brief A PDU router configuration for PDU to ModuleIf mapping array.
 * @version 0.1
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "PduR_Cfg.h"

static PduRCfg_t gpTxPduRCfg[PDUS_TX_NO] = {
  {
    PDU_TX_ID_BOOT_UART,
    PDU_MODULE_IF_UART
  }
};

static PduRCfg_t gpRxPduRCfg[PDUS_RX_NO] = {
  {
    PDU_RX_ID_BOOT_UART,
    PDU_MODULE_IF_UART
  }
};

const PduRCfg_t* 
PduR_TxGetConfig(void)
{
  return gpTxPduRCfg;
}

const PduRCfg_t* 
PduR_RxGetConfig(void)
{
  return gpRxPduRCfg;
}
