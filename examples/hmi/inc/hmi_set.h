#ifndef HMI_SET_H
#define HMI_SET_H

#include "hmi_common.h"

extern const MenuImage_t sgc_SettingImage;

void Hmi_SetEnter(void);
void Hmi_SetExit(void);
void Hmi_SetLoad(uint8_t menuState);
void Hmi_SetTask(void* pExtendInputData, uint8_t menuState, MenuShow_s* pstShowInfo);

#endif