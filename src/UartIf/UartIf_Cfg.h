/**
 * @file UartIf_Cfg.h
 * @author Mohamed Hassanin Mohamed
 * @brief configuration for PDU to UART controller mapping array.
 * @version 0.1
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef UART_IF_CFG_H
#define UART_IF_CFG_H
/******************************************************************************
* Includes
******************************************************************************/
#include "PduR.h"
#include "UartIf.h"
/******************************************************************************
* Defines
******************************************************************************/
//TODO: Configure number of UART PDUS
#define UART_PDUS_TX_NO 1
#define UART_PDUS_RX_NO 1
/******************************************************************************
* Typedefs
******************************************************************************/
typedef struct {
  PduId_t id;
  UartHw_t uart;
} UartIfTxPduCfg_t;

typedef PduId_t UartIfRxPduCfg_t;
/******************************************************************************
* Function Prototypes
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

const UartIfTxPduCfg_t* UartIf_TxGetConfig(void);
const UartIfRxPduCfg_t* UartIf_RxGetConfig(void);

#ifdef __cplusplus
}
#endif

#endif
/***************************** END OF FILE ***********************************/