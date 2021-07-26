/**
 * @file UartIf_Cfg.c
 * @author Mohamed Hassanin Mohamed
 * @brief configuration for PDU to UART controller mapping array.
 * @version 0.1
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "UartIf_Cfg.h"
#include "UartIf.h"
#include "PduR_Cfg.h"

static UartIfTxPduCfg_t gpTxUartIfPduRCfg[UART_PDUS_TX_NO] = {
  {
    PDU_TX_ID_BOOT_UART,
    UART_HW_1
  }
};

static UartIfRxPduCfg_t gpRxUartIfPduRCfg[UART_PDUS_RX_NO] = {
  PDU_RX_ID_BOOT_UART
};

const UartIfTxPduCfg_t* 
UartIf_TxGetConfig(void)
{
  return gpTxUartIfPduRCfg;
}

const UartIfRxPduCfg_t* 
UartIf_RxGetConfig(void)
{
  return gpRxUartIfPduRCfg;
}
