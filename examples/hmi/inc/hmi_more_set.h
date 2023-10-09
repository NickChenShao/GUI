#ifndef HMI_MORE_SET_H
#define HMI_MORE_SET_H

#include "hmi_common.h"


void Hmi_MoreSetEnter(void);
void Hmi_MoreSetExit(void);
void Hmi_MoreSetLoad(uint8_t menuState);
void Hmi_MoreSetTask(void* pExtendInputData, uint8_t menuState, MenuShow_s* pstShowInfo);

#endif