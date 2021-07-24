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

#define PACKET_HEADER_SIZE 5
#define PACKET_MAX_DATA_SIZE 255
#define PACKET_MAX_SIZE (PACKET_MAX_DATA_SIZE+PACKET_HEADER_SIZE) 

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "serial.h"
#include "port.h"

typedef enum
{
  HEX_STATE_LEN,
  HEX_STATE_ADDR,
  HEX_STATE_TYPE,
  HEX_STATE_DATA,
  HEX_STATE_CC
}HexState_t;

inline static void StateMachine(void);
static void (*Packet_Handler)(HexPacket_t*);

UART_HandleTypeDef gUart1Handle;
DMA_HandleTypeDef gDma2Usart1TxHandle;
DMA_HandleTypeDef gDma2Usart1RxHandle;
static HexPacket_t gPacket;

static uint8_t gpPacketBuff[PACKET_MAX_SIZE];

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
Serial_Init(void)
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

  if(HAL_OK != HAL_UART_Receive_DMA(&gUart1Handle, &(gPacket.len), sizeof(gPacket.len)))
    {
    }

  gPacket.data = gpPacketBuff;
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void 
HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART1)
    {
      StateMachine();   
    }
}

/**
 * @brief state machine for the HEX file parsing
 * 
 */
inline static void
StateMachine(void)
{
  static HexState_t state = HEX_STATE_LEN;
  
  switch(state) 
  {
    case HEX_STATE_LEN:
      {
        state = HEX_STATE_ADDR;
        if(HAL_OK != HAL_UART_Receive_DMA(&gUart1Handle, (uint8_t*)&(gPacket.addr),
         sizeof(gPacket.addr))) 
          {
          }
      }
      break;
    case HEX_STATE_ADDR:
      {
        state = HEX_STATE_TYPE;
        if(HAL_OK != HAL_UART_Receive_DMA(&gUart1Handle, &(gPacket.type),
         sizeof(gPacket.type)))
          {
          }
      }
      break;
    case HEX_STATE_TYPE:
      {
        state = HEX_STATE_DATA;
        if(HAL_OK != HAL_UART_Receive_DMA(&gUart1Handle, gPacket.data, gPacket.len))
          {
          }
      }
      break;
    case HEX_STATE_DATA:
      {
        state = HEX_STATE_CC;
        if(HAL_OK != HAL_UART_Receive_DMA(&gUart1Handle, &(gPacket.checksum),
         sizeof(gPacket.checksum)))
          {
          }
      }
      break;
    case HEX_STATE_CC:
      {
        state = HEX_STATE_LEN;
        Packet_Handler(&gPacket);
        if(HAL_OK != HAL_UART_Receive_DMA(&gUart1Handle, &(gPacket.len), sizeof(gPacket.len)))
          {
          }
      }
      break;
    default:
      {
      }
      break;
  }
}

void 
Serial_RegesterPacketCallback(void(*pPacket_Handler)(HexPacket_t*))
{
  Packet_Handler = pPacket_Handler;
}

void 
Serial_Send(HexPacket_t* Packet)
{
  gpPacketBuff[0] = Packet->len;
  gpPacketBuff[1] = (uint8_t)((Packet->addr & 0xFF00) >> 8);
  gpPacketBuff[2] = (uint8_t)(Packet->addr & 0xFF);
  gpPacketBuff[3] = Packet->type;
  for(uint8_t i = 0; i < Packet->len; i++)
    {
      gpPacketBuff[4+i] = Packet->data[i];
    }
  gpPacketBuff[4+Packet->len] = Packet->checksum;

  HAL_UART_Transmit_DMA(&gUart1Handle, gpPacketBuff, PACKET_HEADER_SIZE+Packet->len);
}