#ifndef HMI_MUSIC_H
#define HMI_MUSIC_H

#include "hmi_common.h"


extern const MenuImage_t sgc_MusicImage;

void Hmi_MusicEnter(void);
void Hmi_MusicExit(void);
void Hmi_MusicLoad(uint8_t);
void Hmi_MusicTask(void* pExtendInputData, uint8_t menuState, MenuShow_s* pstShowInfo);

#endif