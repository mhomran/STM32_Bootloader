/**
 * @file error.h
 * @author Mohamed Hassanin Mohamed
 * @brief header file for the error codes.
 * @version 0.1
 * @date 2021-07-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef ERROR_H
#define ERROR_H

#include <inttypes.h>

extern uint8_t gErrorCode;

typedef enum {
  ERR_DEV_GEN_NULL_PTR,
  ERR_PACKET_CHECKSUM,
  ERR_PACKET_WRONG_TYPE,
  ERR_BOOT_SECTOR_FORMAT,
  ERR_BOOT_FLASH_UNLOCK,
} Error_t;

#endif