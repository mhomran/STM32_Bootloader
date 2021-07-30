/**
 * @file ImgHeader.h
 * @author Mohamed Hassanin Mohamed 
 * @brief The header of the current image.
 * @version 0.1
 * @date 2021-07-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef IMG_HEADER_H
#define IMG_HEADER_H
/******************************************************************************
* includes
******************************************************************************/
#include <inttypes.h>
/******************************************************************************
* typedefs
******************************************************************************/
typedef struct {
  uint32_t Version;
  uint32_t ImageStartAddr;
  uint32_t ImageEndAddr;
  uint32_t CRC32;
  uint32_t StackPointer; 
  uint32_t EntryPointAddr;
  uint32_t VectorTableOffset;
} ImgHeader_t;


#endif