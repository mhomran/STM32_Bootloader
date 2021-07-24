/**
 * @file boot.h
 * @author Mohamed Hassanin
 * @brief This module decides which action to perform on the received packet.
 * @version 0.1
 * @date 2021-07-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "boot.h"
#include "packet.h"

static void Boot_Handler(HexPacket_t*);

void 
Boot_Init(void) 
{
  Packet_RegesterBootCallback(Boot_Handler);
}

/**
 * @brief Handle the received packet action according to the packet
 * type
 * 
 */
static void 
Boot_Handler(HexPacket_t* Packet)
{
  switch(Packet->type)
  {
    case PACKET_TYPE_DATA_RECORD: 
      {

      }
      break;
    case PACKET_TYPE_EOF_RECORD:
      {

      }
      break;
    case PACKET_TYPE_EXTENDED_LINEAR_ADDR_RECORD:
      {

      }
      break;
    case PACKET_TYPE_ERASE_SECTOR_IN_BANK2:
      {

      }
      break;
    case PACKET_TYPE_RESET:
      {
        //Stop the tick, so the wdg isn't fed.
        HAL_SuspendTick();
      }
      break;
    default:
      {
        //DO NOTHING
      }
      break;
  }
}