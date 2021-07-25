/**
 * @file img_header.h
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

#include <inttypes.h>

typedef struct {
  uint32_t ImageSize;
  uint8_t Id;
  uint32_t StackPointer; 
  uint32_t EntryPoint;
} ImgHeader_t;

#endif