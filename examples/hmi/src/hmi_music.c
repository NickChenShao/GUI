#include "hmi_music.h"
#include "mainhmi.h"
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif
static bool sg_isInit = false;

const MenuImage_t sgc_MusicImage = {
"mmmmmmmmmm",
"@"
};

void XmMcu_MainForm_enter(void)
{

}

void XmMcu_MainForm_exit(void)
{

}

void Hmi_MusicLoad(uint8_t menuState)
{
    sg_isInit = true;
}

void Hmi_MusicTask(void* pExtendInputData, uint8_t menuState, MenuShow_s* pstShowInfo)
{
    int cmd = 0;

    if (sg_isInit)
    {
        sg_isInit = false;
        printf("--------------------------\n");
        printf("    音乐功能测试界面\n");
        printf("--------------------------\n");
#ifdef _WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
    }

//    printf("选择操作(0-退出): ");
//    scanf(" %d", &cmd); // 空格作用是忽略上次的回车
//
//    if (cmd == 0)
//    {
//        cotMenu_Exit(false);
//    }
}