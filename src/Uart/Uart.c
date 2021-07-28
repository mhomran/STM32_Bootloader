/**
 * @file Uart.c
 * @author Mohamed Hassanin
 * @brief A Uart driver to pack the hex formated packet and send it to
 * UartIf.
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#define UART_HW_NO 1

#define FRAME_HEADER_SIZE 7 //2 addr, 1 type, 4 CRC
#define FRAME_MAX_DATA_BUFF_SIZE 16
#define FRAME_MAX_SIZE (FRAME_HEADER_SIZE + FRAME_MAX_DATA_BUFF_SIZE)
#define FRAME_CRC_SIZE 4

#define ADDR_OFFSET 0
#define TYPE_OFFSET 2
#define DATA_OFFSET 3

#include "stm32f4xx_hal.h"
#include "Uart.h"
#include "UartIf.h"
#include "UartIf_Cfg.h"
#include "port.h"
#include "PduR_Cfg.h"

typedef struct {
  uint8_t len;
  uint16_t addr;
  uint8_t* data;
  uint8_t type;
  uint32_t crc; 
} HexFormat_t;

typedef enum
{
  STATE_LEN,
  STATE_ADDR,
  STATE_TYPE,
  STATE_DATA,
  STATE_CRC
}HexState_t;

UART_HandleTypeDef gUart1Handle;
DMA_HandleTypeDef gDma2Usart1TxHandle;
DMA_HandleTypeDef gDma2Usart1RxHandle;
CRC_HandleTypeDef gCrcHandle;

static HexFormat_t gRxHexFrame;
static uint8_t gRxHexFrameBuff[FRAME_MAX_SIZE];
static uint8_t gRxHexFrameDataBuff[FRAME_MAX_DATA_BUFF_SIZE];
static uint8_t gTxHexFrameDataBuff[FRAME_MAX_DATA_BUFF_SIZE];
static uint32_t gCrcBuff[FRAME_MAX_SIZE-FRAME_CRC_SIZE]; //

static PduInfo_t gUartIfPdu;
static uint8_t gUartIfPduDataBuff[FRAME_MAX_SIZE];

static uint8_t gUartHwTxPduId[UART_HW_NO];

static uint32_t Uart_CalculateCRC(uint8_t*, uint8_t);

void 
HAL_UART_MspInit(UART_HandleTypeDef *hsart)
{
  if(hsart->Instance==USART1)
    {
      __HAL_RCC_USART1_CLK_ENABLE();
      __HAL_RCC_GPIOB_CLK_ENABLE();
      __HAL_RCC_DMA2_CLK_ENABLE();

      GPIO_InitTypeDef GPIO_InitStruct = {0};

      GPIO_InitStruct.Pin = USART1_TX_PIN | USART1_RX_PIN;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

      gDma2Usart1TxHandle.Instance = DMA2_Stream7;
      gDma2Usart1TxHandle.Init.Channel = DMA_CHANNEL_4;
      gDma2Usart1TxHandle.Init.Direction = DMA_MEMORY_TO_PERIPH;
      gDma2Usart1TxHandle.Init.PeriphInc = DMA_PINC_DISABLE;
      gDma2Usart1TxHandle.Init.MemInc = DMA_MINC_ENABLE;
      gDma2Usart1TxHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      gDma2Usart1TxHandle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
      gDma2Usart1TxHandle.Init.Mode = DMA_NORMAL;
      gDma2Usart1TxHandle.Init.Priority = DMA_PRIORITY_LOW;
      gDma2Usart1TxHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
      if (HAL_DMA_Init(&gDma2Usart1TxHandle) != HAL_OK)
        {
        }

      __HAL_LINKDMA(&gUart1Handle,hdmatx,gDma2Usart1TxHandle);

      /* USART1_RX Init */
      gDma2Usart1RxHandle.Instance = DMA2_Stream2;
      gDma2Usart1RxHandle.Init.Channel = DMA_CHANNEL_4;
      gDma2Usart1RxHandle.Init.Direction = DMA_PERIPH_TO_MEMORY;
      gDma2Usart1RxHandle.Init.PeriphInc = DMA_PINC_DISABLE;
      gDma2Usart1RxHandle.Init.MemInc = DMA_MINC_ENABLE;
      gDma2Usart1RxHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      gDma2Usart1RxHandle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
      gDma2Usart1RxHandle.Init.Mode = DMA_NORMAL;
      gDma2Usart1RxHandle.Init.Priority = DMA_PRIORITY_LOW;
      gDma2Usart1RxHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
      if (HAL_DMA_Init(&gDma2Usart1RxHandle) != HAL_OK)
        {
        }

      __HAL_LINKDMA(&gUart1Handle,hdmarx,gDma2Usart1RxHandle);

      /* USART1 interrupt Init */
      HAL_NVIC_SetPriority(USART1_IRQn, TICK_INT_PRIORITY+1, 0);
      HAL_NVIC_EnableIRQ(USART1_IRQn);
      HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, TICK_INT_PRIORITY+2, 0);
      HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
      HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, TICK_INT_PRIORITY+3, 0);
      HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
    }
}

void 
Uart_Init(void)
{
  gUart1Handle.Instance = USART1;

  gUart1Handle.Init.BaudRate = 9600;
  gUart1Handle.Init.WordLength = UART_WORDLENGTH_8B;
  gUart1Handle.Init.StopBits = UART_STOPBITS_1;
  gUart1Handle.Init.Parity = UART_PARITY_NONE;
  gUart1Handle.Init.Mode = UART_MODE_TX_RX;

  if (HAL_UART_Init(&gUart1Handle) != HAL_OK)
    {
    }

  __HAL_UART_ENABLE_IT(&gUart1Handle, UART_IT_IDLE);

  if(HAL_OK != HAL_UART_Receive_DMA(&gUart1Handle, gRxHexFrameBuff, FRAME_MAX_SIZE))
    {
    }

  __HAL_RCC_CRC_CLK_ENABLE();
  gCrcHandle.Instance = CRC;
  HAL_CRC_Init(&gCrcHandle);

  gRxHexFrame.data = gRxHexFrameDataBuff;
  gUartIfPdu.data = gUartIfPduDataBuff;
}

StdReturn_t 
Uart_Write(UartHw_t Uart, UartPdu_t* UartPdu)
{
  uint8_t i;
  uint32_t crc = 0x01020304;

  switch(Uart)
  {
    case UART_HW_1:
      {
        *((uint16_t*)&gTxHexFrameDataBuff[ADDR_OFFSET]) = *((uint16_t*)&UartPdu->data[ADDR_OFFSET]);
        gCrcBuff[ADDR_OFFSET] = UartPdu->data[ADDR_OFFSET];
        gCrcBuff[ADDR_OFFSET+1] = UartPdu->data[ADDR_OFFSET+1];
        gTxHexFrameDataBuff[TYPE_OFFSET] = UartPdu->data[TYPE_OFFSET];
        gCrcBuff[TYPE_OFFSET] = UartPdu->data[TYPE_OFFSET];
        for(i = DATA_OFFSET; i < UartPdu->len; i++)
          {
            gTxHexFrameDataBuff[i] = UartPdu->data[i];
            gCrcBuff[i] = (uint32_t)(UartPdu->data[i]);
          }

        crc = HAL_CRC_Calculate(&gCrcHandle, gCrcBuff, UartPdu->len);
        *((uint32_t*)&gTxHexFrameDataBuff[i]) = crc;

        if(HAL_OK != HAL_UART_Transmit_DMA(&gUart1Handle, gTxHexFrameDataBuff, 
        UartPdu->len + sizeof(crc)))
          {
            return E_NOT_OK;
          }
        gUartHwTxPduId[UART_HW_1] = UartPdu->pduid;
      }
      break;
    default:
      {
        return E_NOT_OK;
      }
      break;
  } 

  return E_OK;
}

/**
  * @brief  Tx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void 
HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART1)
    {
      UartIf_TxConfirmation(gUartHwTxPduId[UART_HW_1]);
    }
}

void 
UART1_IDLE_IRQHandler(UART_HandleTypeDef *huart)
{
  uint8_t len;
  uint8_t i;
  uint32_t rx_crc;
  uint32_t gen_crc;
  StdReturn_t state = E_OK;
  
  len = FRAME_MAX_SIZE - huart->hdmarx->Instance->NDTR;
  
  gUartIfPdu.len = len - sizeof(gRxHexFrame.crc);

  *((uint16_t*)&gUartIfPdu.data[ADDR_OFFSET]) = *((uint16_t*)&gRxHexFrameBuff[ADDR_OFFSET]);
  gUartIfPdu.data[TYPE_OFFSET] = gRxHexFrameBuff[TYPE_OFFSET];
  for(i = DATA_OFFSET; i < len - sizeof(rx_crc); i++)
    {
      gUartIfPdu.data[i] = gRxHexFrameBuff[i];
      gCrcBuff[i - DATA_OFFSET] = (uint32_t)gUartIfPdu.data[i];
    }
  
  rx_crc = *((uint32_t*)&gRxHexFrameBuff[i]);
  gen_crc = Uart_CalculateCRC(gUartIfPdu.data, gUartIfPdu.len);

  if(rx_crc != gen_crc)
    {
      state = E_NOT_OK;
    }
  
  if(huart->Instance == USART1)
    {
      if(state == E_OK)
        {
          UartIf_RxIndication(UART_HW_1, &gUartIfPdu);
        }
      if(HAL_UART_Receive_DMA(&gUart1Handle, gRxHexFrameBuff, FRAME_MAX_SIZE) != HAL_OK)
        {
          state = E_NOT_OK;
        }
    }
}

static uint32_t 
Uart_CalculateCRC(uint8_t* pData, uint8_t Size)
{
  for(uint8_t i = 0; i < Size; i++)
    {
      gCrcBuff[i] = (uint32_t)pData[i];
    }
  return HAL_CRC_Calculate(&gCrcHandle, gCrcBuff, Size);
}