#include "hmi_more_set.h"
#include "cot_menu.h"
#include <stdio.h>
#include <string.h>


static void OnUpgradeFunction(void);
static void OnAboutMenuFunction(void);


/* �������� */
MenuList_t sg_MoreSetMenuTable[] = 
{
    {{"����", "Upgrade"}, NULL, NULL, NULL, OnUpgradeFunction, NULL},
    {{"����", "About"},   NULL, NULL, NULL, OnAboutMenuFunction, NULL},
};

/* �������ò˵���ʾЧ��: �Ҳ൯���˵�Ч�� */
static void ShowMoreSetMenu(MenuShow_t *ptShowInfo)
{
    uint8_t showNum = 3;
    uint8_t showsubNum = 3;
    menusize_t  tmpselect;

    MenuShow_t tParentMenuShowInfo;

    if (cotMenu_QueryParentMenu(&tParentMenuShowInfo, 1) != 0)
    {
        return;
    }

    cotMenu_LimitShowListNum(&tParentMenuShowInfo, &showNum);
    cotMenu_LimitShowListNum(ptShowInfo, &showsubNum);

    printf("\e[0;30;46m ------------- %s ------------- \e[0m\n", tParentMenuShowInfo.pszDesc);

    for (int i = 0; i < showNum; i++)
    {
        tmpselect = i + tParentMenuShowInfo.showBaseItem;

        if (tmpselect == tParentMenuShowInfo.selectItem)
        {
            printf("\e[0;30;47m %d. %-16s\e[0m |", tmpselect + 1, tParentMenuShowInfo.pszItemsDesc[tmpselect]);
        }
        else
        {
            printf("\e[7;30;47m %d. %-16s\e[0m |", tmpselect + 1, tParentMenuShowInfo.pszItemsDesc[tmpselect]);
        }

        if (i < showsubNum)
        {
            tmpselect = i + ptShowInfo->showBaseItem;

            if (tmpselect == ptShowInfo->selectItem)
            {
                printf(" \e[0;30;47m %-14s\e[0m", ptShowInfo->pszItemsDesc[tmpselect]);
            }
            else
            {
                printf(" \e[7;30;47m %-14s\e[0m", ptShowInfo->pszItemsDesc[tmpselect]);
            }
        }

        printf("\n");
    }   
}

void Hmi_MoreSetEnter(void)
{
    cotMenu_Bind(sg_MoreSetMenuTable, GET_MENU_NUM(sg_MoreSetMenuTable), ShowMoreSetMenu);
}

void Hmi_MoreSetLoad(void)
{

}

void Hmi_MoreSetExit(void)
{

}

void Hmi_MoreSetTask(void)
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


static void OnUpgradeFunction(void)
{
    int cmd;

    printf("--------------------------\n");
    printf("     �����������Խ���\n");
    printf("--------------------------\n");
    
    printf("ѡ�����(0-�˳�): ");
    scanf(" %d", &cmd); // �ո������Ǻ����ϴεĻس�

    if (cmd == 0)
    {
        cotMenu_Exit(false);
    }
}

static void OnAboutMenuFunction(void)
{
    int cmd;

    printf("--------------------------\n");
    printf("     ���ڲ˵���ܽ���\n");
    printf("--------------------------\n");

    printf("ѡ�����(0-�˳�): ");
    scanf(" %d", &cmd); // �ո������Ǻ����ϴεĻس�

    if (cmd == 0)
    {
        cotMenu_Exit(false);
    }
}
