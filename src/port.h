/**
 * @file port.h
 * @author Mohamed Hassanin Mohamed
 * @brief This file contains all the mapping of the used GPIO pins.
 * @version 0.1
 * @date 2021-07-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef PORT_H
#define PORT_H
/******************************************************************************
* includes
******************************************************************************/
#include <inttypes.h>
/******************************************************************************
* Definitions
******************************************************************************/
//Ports and pins
#define BLUE_LED_PIN GPIO_PIN_15
#define BLUE_LED_PORT GPIOD

#define USART1_TX_PIN GPIO_PIN_6
#define USART1_TX_PORT GPIOB

#define USART1_RX_PIN GPIO_PIN_7  
#define USART1_RX_PORT GPIOB


#endif
/***************************** END OF FILE ***********************************/