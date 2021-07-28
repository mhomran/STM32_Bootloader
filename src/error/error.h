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
  ERR_NONE,
  ERR_DEV_GEN_NULL_PTR,
  ERR_BOOT_IF_CRC,
  ERR_BOOT_IF_WRONG_TYPE,
  ERR_BOOT_IF_SECTOR_FORMAT,
  ERR_BOOT_IF_PROGRAM_FORMAT,
  ERR_BOOT_IF_PROGRAM_ADDR_ALIGN,
  ERR_BOOT_IF_FLASH_UNLOCK,
  ERR_BOOT_IF_SECTOR_ERASE,
  ERR_BOOT_IF_IMAGE_ERASE,
  ERR_BOOT_IF_DATA_IS_NOT_WRITTEN
} Error_t;

#endif