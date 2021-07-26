/**
 * @file serial.c
 * @author Mohamed Hassanin
 * @brief A serial communication used by the bootloader (USART at this case)
 * @version 0.1
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

//1 len, 1 type, 2 address, 4 CRC
#define UART_HEADER_SIZE 8
#define UART_MAX_DATA_SIZE 255
#define UART_MAX_TX_BUFF_SIZE (UART_HEADER_SIZE+UART_MAX_DATA_SIZE) 

#include <stdio.h>
#include "UartIf.h"
#include "UartIf_Cfg.h"
#include "Uart.h"
#include "port.h"

typedef enum {
  PDU_DIR_TX,
  PDU_DIR_RX
} PduDirection_t;

const UartIfTxPduCfg_t* gpUartIfTxPduCfg;
const UartIfRxPduCfg_t* gpUartIfRxPduCfg;

static StdReturn_t UartIf_GetUartHw(PduDirection_t dir, UartHw_t* res, PduId_t id);

void 
UartIf_Init(void)
{  
  gpUartIfTxPduCfg = UartIf_TxGetConfig();
  gpUartIfRxPduCfg = UartIf_RxGetConfig();
}

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

void 
UartIf_RxIndication(UartHw_t Uart, PduInfo_t* Pdu)
{
  PduId_t id = gpUartIfRxPduCfg[Uart];
  PduR_RxIndication(id, Pdu);
}

void 
UartIf_TxConfirmation(PduId_t PduId)
{
  PduR_TxConfirmation(PduId);
}
