#include "hmi_set.h"
#include "hmi_more_set.h"
#include "cot_menu.h"
#include <stdio.h>
#include <string.h>


const MenuImage_t sgc_SettingImage = {
"$$$$$$$$$$",
"%"
};

static void OnLanguageFunction(void);
static void OnBluetoothFunction(void);
static void OnBatteryFunction(void);
static void OnStorageFunction(void);

/* ���ò˵� */
MenuList_t sg_SetMenuTable[] = 
{
    {{"����", "Language"},   NULL, NULL, NULL, OnLanguageFunction, NULL},
    {{"����", "Bluetooth"},  NULL, NULL, NULL, OnBluetoothFunction, NULL},
    {{"���", "Battery"},    NULL, NULL, NULL, OnBatteryFunction, NULL},
    {{"����", "Store"},      NULL, NULL, NULL, OnStorageFunction, NULL},
    {{"����", "More"},       Hmi_MoreSetEnter, Hmi_MoreSetExit, Hmi_MoreSetLoad, Hmi_MoreSetTask, NULL},
};

/* ���ò˵���ʾЧ�� */
static void ShowSetMenu(MenuShow_t *ptShowInfo)
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
            printf("\e[0;30;47m %d. %-16s\e[0m |\n", tmpselect + 1, ptShowInfo->pszItemsDesc[tmpselect]);
        }
        else
        {
            printf("\e[7;30;47m %d. %-16s\e[0m |\n", tmpselect + 1, ptShowInfo->pszItemsDesc[tmpselect]);
        }
    }
}

void Hmi_SetEnter(void)
{
    cotMenu_Bind(sg_SetMenuTable, GET_MENU_NUM(sg_SetMenuTable), ShowSetMenu);
}

void Hmi_SetLoad(void)
{
    
}

void Hmi_SetExit(void)
{

}

void Hmi_SetTask(void)
{
    int cmd;

    printf("ѡ�����(0-����; 1-�������˵�; 2-����; 3-��һ��; 4-��һ��): ");
    scanf(" %d", &cmd); // �ո������Ǻ����ϴεĻس�
 
    switch (cmd)
    {
    case 0:
        cotMenu_Exit(false);
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


static void OnLanguageFunction(void)
{
    int cmd;

    printf("--------------------------\n");
    printf("     ���Թ��ܲ��Խ���\n");
    printf("--------------------------\n");
    
    printf("ѡ�����(0-����; 1-English): ");
    scanf(" %d", &cmd); // �ո������Ǻ����ϴεĻس�

    if (cmd == 0)
    {
        cotMenu_SelectLanguage(0);
    }
    else
    {
        cotMenu_SelectLanguage(1);
    }

    cotMenu_Exit(0); // �л����Զ��˳�
}

static void OnBluetoothFunction(void)
{
    int cmd;

    printf("--------------------------\n");
    printf("     �������ܲ��Խ���\n");
    printf("--------------------------\n");

    printf("ѡ�����(0-�˳�): ");
    scanf(" %d", &cmd); // �ո������Ǻ����ϴεĻس�

    if (cmd == 0)
    {
        cotMenu_Exit(false);
    }
}

static void OnBatteryFunction(void)
{
    int cmd;

    printf("--------------------------\n");
    printf("     ��ع��ܲ��Խ���\n");
    printf("--------------------------\n");

    printf("ѡ�����(0-�˳�): ");
    scanf(" %d", &cmd); // �ո������Ǻ����ϴεĻس�

    if (cmd == 0)
    {
        cotMenu_Exit(false);
    }
}

static void OnStorageFunction(void)
{
    int cmd;

    printf("--------------------------\n");
    printf("     ���湦�ܲ��Խ���\n");
    printf("--------------------------\n");

    printf("ѡ�����(0-�˳�): ");
    scanf(" %d", &cmd); // �ո������Ǻ����ϴεĻس�

    if (cmd == 0)
    {
        cotMenu_Exit(false);
    }
}