#include "hmi_music.h"
#include "cot_menu.h"
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

void Hmi_MusicEnter(void)
{

}

void Hmi_MusicExit(void)
{

}

void Hmi_MusicLoad(void)
{
    sg_isInit = true;
}

void Hmi_MusicTask(void)
{
    int cmd = 0;

    if (sg_isInit)
    {
        sg_isInit = false;
        printf("--------------------------\n");
        printf("    ���ֹ��ܲ��Խ���\n");
        printf("--------------------------\n");
#ifdef _WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
    }

    printf("ѡ�����(0-�˳�): ");
    scanf(" %d", &cmd); // �ո������Ǻ����ϴεĻس�

    if (cmd == 0)
    {
        cotMenu_Exit(false);
    }
}