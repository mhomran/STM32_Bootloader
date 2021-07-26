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
#ifndef BOOT_H_
#define BOOT_H_

#include "PduR.h"

#ifdef __cplusplus
extern "C" {
#endif

void BootIf_Init(void);
void BootIf_RxIndication(PduId_t id, PduInfo_t* pdu);
void BootIf_TxConfirmation(PduId_t id);

#ifdef __cplusplus
}
#endif

#endif