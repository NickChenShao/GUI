#ifndef HMI_VIDEO_H
#define HMI_VIDEO_H

#include "hmi_common.h"
#include <stdio.h>
extern const MenuImage_t sgc_VideoImage;

void Hmi_VideoLoad(uint8_t menuState);
void Hmi_VideoExit(void);
void Hmi_VideoTask(void* pExtendInputData, uint8_t menuState, MenuShow_s* pstShowInfo);

#endif