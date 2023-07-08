#include "hmi_camera.h"
#include "cot_menu.h"
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

const MenuImage_t sgc_CameraImage = {
"**********",
"&"
};


static void OnPhotoFunctionTask(void);
static void OnCameraFunctionTask(void);

/* ������˵� */
MenuList_t sg_CameraMenuTable[] = 
{
    {{"����", "Photo"}, NULL, NULL, NULL, OnPhotoFunctionTask, NULL},
    {{"��Ӱ", "Camera"}, NULL, NULL, NULL, OnCameraFunctionTask, NULL},
};

/* ������˵���ʾЧ�� */
static void ShowCameraMenu(MenuShow_t *ptShowInfo)
{
    uint8_t showNum = 3;
    menusize_t  tmpselect;

    cotMenu_LimitShowListNum(ptShowInfo, &showNum);

    printf("\e[0;30;46m ------------- %s ------------- \e[0m\n", ptShowInfo->pszDesc);

    for (int i = 0; i < showNum; i++)
    {
        tmpselect = i + ptShowInfo->showBaseItem;

        if (tmpselect == ptShowInfo->selectItem)
        {
            printf("\e[0;30;47m %d. %-34s\e[0m\n", tmpselect + 1, ptShowInfo->pszItemsDesc[tmpselect]);
        }
        else
        {
            printf("\e[7;30;47m %d. %-34s\e[0m\n", tmpselect + 1, ptShowInfo->pszItemsDesc[tmpselect]);
        }
    }
}

void Hmi_CameraEnter(void)
{
    cotMenu_Bind(sg_CameraMenuTable, GET_MENU_NUM(sg_CameraMenuTable), ShowCameraMenu);
}

void Hmi_CameraLoad(void)
{
    CLEAR();
    MOVETO(0, 0);
    printf("---����-----\n");
}

void Hmi_CameraExit(void)
{
    printf("--------------------------\n");
    printf("     ��Ƶ����ֹͣ����...\n");
    printf("--------------------------\n");
#ifdef _WIN32
    Sleep(1000);
#else
    sleep(1);
#endif
}

void Hmi_CameraTask(void)
{
    int cmd;

    printf("ѡ�����(0-����; 1-�������˵�; 2-����; 3-��һ��; 4-��һ��): ");
    scanf(" %d", &cmd); // �ո������Ǻ����ϴεĻس�
 
    switch (cmd)
    {
    case 0:
        cotMenu_Exit(true);
        break;
    case 1:
        cotMenu_Reset();
        break;
    case 2:
        cotMenu_Enter();
        break;
    case 3:
        cotMenu_SelectNext(true);
        break;
    case 4:
        cotMenu_SelectPrevious(true);
        break;

    default:
        break;    
    }
}



static void OnPhotoFunctionTask(void)
{
    int cmd = 0;

    printf("--------------------------\n");
    printf("     ���չ��ܲ��Խ���\n");
    printf("--------------------------\n");

    printf("ѡ�����(0-�˳�): ");
    scanf(" %d", &cmd); // �ո������Ǻ����ϴεĻس�

    if (cmd == 0)
    {
        cotMenu_Exit(false);
    }
}

static void OnCameraFunctionTask(void)
{
    int cmd = 0;

    printf("--------------------------\n");
    printf("     �����ܲ��Խ���\n");
    printf("--------------------------\n");

    printf("ѡ�����(0-�˳�): ");
    scanf(" %d", &cmd); // �ո������Ǻ����ϴεĻس�

    if (cmd == 0)
    {
        cotMenu_Exit(false);
    }
}