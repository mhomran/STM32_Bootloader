/**
 * @file ImgHeader.c
 * @author Mohamed Hassanin Mohamed 
 * @brief The header of the current image.
 * @version 0.1
 * @date 2021-07-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "ImgHeader.h"

extern uint32_t _Image_0_Flash_Origin;
extern uint32_t _Image_0_Flash_End;
extern uint32_t _estack;
extern uint32_t Reset_Handler;
extern uint32_t g_pfnVectors;

static volatile const ImgHeader_t __attribute__((section(".img_header"))) gImgHeader =
{
  .Version = 0x00010000,
  .ImageStartAddr = (uint32_t)&_Image_0_Flash_Origin,
  .ImageEndAddr = (uint32_t)&_Image_0_Flash_End,
  .CRC32 = 0X2D377585,
  .StackPointer = (uint32_t)&_estack,
  .EntryPointAddr = (uint32_t)&Reset_Handler,
  .VectorTableAddr = ((uint32_t)&g_pfnVectors)
};

