#ifndef PORT_H
#define PORT_H

//Ports and pins
#define BLUE_LED_PIN GPIO_PIN_15
#define BLUE_LED_PORT GPIOD

#define USART1_TX_PIN GPIO_PIN_6
#define USART1_TX_PORT GPIOB

#define USART1_RX_PIN GPIO_PIN_7  
#define USART1_RX_PORT GPIOB

//Development error codes
typedef enum {
  ERR_DEV_GEN_NULL_PTR,
  ERR_PACKET_CHECKSUM,
  ERR_PACKET_WRONG_TYPE
} Error_t;

#endif