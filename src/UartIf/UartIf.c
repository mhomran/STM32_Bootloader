/**
 * @file UartIf.h
 * @author Mohamed Hassanin
 * @brief UART interface layer between the driver and the PDU router.
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
#include "UartIf.h"
#include "UartIf_Cfg.h"
#include "Uart.h"
#include "port.h"
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
const UartIfTxPduCfg_t* gpUartIfTxPduCfg;
const UartIfRxPduCfg_t* gpUartIfRxPduCfg;
/******************************************************************************
* Functions declrations
******************************************************************************/
static StdReturn_t UartIf_GetUartHw(PduDirection_t dir, UartHw_t* res, PduId_t id);
/******************************************************************************
* Functions definitions
******************************************************************************/

/**
 * @brief Initialize the UART interface layer
 * 
 */
void 
UartIf_Init(void)
{  
  gpUartIfTxPduCfg = UartIf_TxGetConfig();
  gpUartIfRxPduCfg = UartIf_RxGetConfig();
}


/**
 * @brief utility function to get the UART HW number given the PDU id
 * 
 * @param dir the direction of the PDU (Transmited or Received PDU)
 * @param res the UART HW number
 * @param id the PDU id
 * @return StdReturn_t 
 */
static StdReturn_t 
UartIf_GetUartHw(PduDirection_t dir, UartHw_t* res, PduId_t id)
{
  StdReturn_t state = E_NOT_OK;
  
  if(dir == PDU_DIR_TX)
    {
      for(uint32_t i = 0; i < UART_PDUS_TX_NO; i++)
        {
          if(gpUartIfTxPduCfg[i].id == id)
            {
              *res = gpUartIfTxPduCfg[i].uart;
              state = E_OK;
              break;
            }   
        }
    }
  else
    {
      for(uint32_t i = 0; i < UART_PDUS_RX_NO; i++)
        {
          if(gpUartIfRxPduCfg[i] == id)
            {
              *res = i;
              state = E_OK;
              break;
            }   
        }
    }
  return state;
}

StdReturn_t 
UartIf_Transmit(PduId_t id, PduInfo_t* Pdu)
{
  UartPdu_t UartPdu;
  UartHw_t Uart;

  UartPdu.data = Pdu->data;
  UartPdu.len = Pdu->len;
  UartPdu.pduid = id;

  Uart = UartIf_GetUartHw(PDU_DIR_TX, &Uart, id);

  return Uart_Write(Uart, &UartPdu);
}

/**
 * @brief A call back function to be called from the UART driver upon receiving a PDU
 * 
 * @param Uart the UART hw that received the PDU.
 * @param Pdu The PDU received.
 */
void 
UartIf_RxIndication(UartHw_t Uart, PduInfo_t* Pdu)
{
  PduId_t id = gpUartIfRxPduCfg[Uart];
  PduR_RxIndication(id, Pdu);
}

/**
 * @brief A call back function to be called from the UART driver upon transmitting a PDU
 * 
 * @param PduId The PDU id of the transmitted PDU.
 */
void 
UartIf_TxConfirmation(PduId_t PduId)
{
  PduR_TxConfirmation(PduId);
}
/***************************** END OF FILE ***********************************/