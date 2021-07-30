/**
 * @file img_header.c
 * @author Mohamed Hassanin Mohamed 
 * @brief The header of the current image.
 * @version 0.1
 * @date 2021-07-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "img_header.h"

extern uint32_t _Image_0_Flash_Origin;
extern uint32_t _Image_0_Flash_End;
extern uint32_t _estack;
extern uint32_t Reset_Handler;
extern uint32_t g_pfnVectors;

static ImgHeader_t __attribute__((section(".img_header"))) gImgHeader =
{
  .Version = 0x00010000,
  .ImageStartAddr = (uint32_t)&_Image_0_Flash_Origin,
  .ImageEndAddr = (uint32_t)&_Image_0_Flash_End,
  .CRC32 = 0XCDAE4015,
  .StackPointer = (uint32_t)&_estack,
  .EntryPointAddr = (uint32_t)&Reset_Handler,
  .VectorTableOffset = (uint32_t)&g_pfnVectors
};

