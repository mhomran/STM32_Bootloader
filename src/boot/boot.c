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

#define BOOT_OPTION_DATA_RECORD                          ((uint8_t)0)
#define BOOT_OPTION_EOF_RECORD                           ((uint8_t)1)
#define BOOT_OPTION_EXTENDED_LINEAR_ADDR_RECORD          ((uint8_t)4)
#define BOOT_OPTION_ERASE_SECTOR_IN_BANK2                ((uint8_t)6)
#define BOOT_OPTION_RESET                                ((uint8_t)7)
#define BOOT_OPTION_HELP                                 ((uint8_t)8)

#include <stdio.h>
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
  int temp;
  
  switch(Packet->type)
  {
    case BOOT_OPTION_DATA_RECORD: 
      {

      }
      break;
    case BOOT_OPTION_EOF_RECORD:
      {

      }
      break;
    case BOOT_OPTION_EXTENDED_LINEAR_ADDR_RECORD:
      {

      }
      break;
    case BOOT_OPTION_ERASE_SECTOR_IN_BANK2:
      {

      }
      break;
    case BOOT_OPTION_RESET:
      {

      }
      break;
    case BOOT_OPTION_HELP:
      {
        printf(
          "0X%02X - data record\r"
          "0X%02X - EOF record\r"
          "0X%02X - extended linear address record record\r"
          "0X%02X - erase a sector record. sector number is put in data\r"
          "0X%02X - reset the microcontroller\r"
          "0X%02X - for help\r\n",
          BOOT_OPTION_DATA_RECORD,                          
          BOOT_OPTION_EOF_RECORD,                           
          BOOT_OPTION_EXTENDED_LINEAR_ADDR_RECORD,          
          BOOT_OPTION_ERASE_SECTOR_IN_BANK2,                
          BOOT_OPTION_RESET,                                
          BOOT_OPTION_HELP); 
      }
      break;
    default:
      {
        printf("[ERROR] %X Unknown record type, use 0x08 type record for help.\n", Packet->type);
      }
      break;
  }
}