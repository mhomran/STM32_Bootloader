/**
 * @file PduR_Cfg.h
 * @author Mohamed Hassanin Mohamed
 * @brief A PDU router configuration for PDU to ModuleIf mapping array.
 * @version 0.1
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef PDU_R_CFG_H
#define PDU_R_CFG_H
/******************************************************************************
* Includes
******************************************************************************/
#include "PduR.h"
/******************************************************************************
* delarations
******************************************************************************/
//TODO: Configure number of PDUS
#define PDUS_TX_NO 1
#define PDUS_RX_NO 1

#define PDU_TX_ID_BOOT_UART 5
#define PDU_RX_ID_BOOT_UART 5
/******************************************************************************
* typedefs
******************************************************************************/
typedef enum {
  PDU_MODULE_IF_UART,
  PDU_MODULE_IF_MAX
} PduModuleIf_t;

typedef struct {
  PduId_t id;
  PduModuleIf_t HWM;
} PduRCfg_t;

/******************************************************************************
* Function Prototypes
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

const PduRCfg_t* PduR_TxGetConfig(void);
const PduRCfg_t* PduR_RxGetConfig(void);

#ifdef __cplusplus
}
#endif

#endif