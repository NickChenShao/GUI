
/* 菜单显示效果图可看: 

https://blog.csdn.net/qq_24130227/article/details/121167276 

*/

#include "xmcore_menu_form.h"
#include "mainhmi.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


static MenuManage_s sg_stMenuManage;

#ifndef _MENU_USE_MALLOC_
static MenuCtrl_s sg_aArrMenuCtrl[MENU_MAX_DEPTH];
#endif


int isEnterMenu = 0;


void EnterMainMenu(void)
{
    isEnterMenu = 1;
}

void ExitMainMenu(void)
{
    isEnterMenu = 0;
}

static void LCD_SystemLogo_Icon(void)
{
	CLEAR();
	MOVETO(0, 0);
	printf("开机动画显示!!.........\r\n");
	sleep(2);
}
int main(int argc, char **argv)
{
    int cmd = 0;
    int8_t musicMenuId, languageMenuId, moreSetMenuId;
    
    MainMenuCfg_s tMainMenu = {{"主菜单", "Main Menu"}, EnterMainMenu, ExitMainMenu, Hmi_LoadMainHmi, Hmi_MainTask};

    cotMenu_Init(&tMainMenu);
    system("stty -icanon");//关闭缓冲区，输入字符无需按回车键直接接受
	LCD_SystemLogo_Icon();
    while (1)
    {
        CLEAR();
        MOVETO(0, 0);

        if (!isEnterMenu)
        {
                cotMenu_MainEnter();
                CLEAR();
                MOVETO(0, 0);
        }
        cotMenu_Task();
    }

    cotMenu_DeInit();

    return 0;
}

typedef struct
{
	uint8_t keyFlag;
	uint8_t keyValue;
} MenuInput_s;
MenuInput_s g_MenuInput;

/* 菜单初始化和反初始化 */
int cotMenu_Init(MainMenuCfg_s *pstMainMenu)
{
	LibXmCore_MenuForm_init(&sg_stMenuManage, pstMainMenu, sg_aArrMenuCtrl,MENU_MAX_DEPTH,XmMcu_Ctrl_MenuTask,XmMcu_cotMenu_getInput,&g_MenuInput);
}
int cotMenu_DeInit(void)
{
	LibXmCore_MenuForm_deInit(&sg_stMenuManage);
}

int cotMenu_Bind(MenuList_s *pstMenuList, MenuSize menuNum, ShowmenuAnyCallFuncPtr fnShowMenuFuncPtrcPtr)
{
	LibXmCore_MenuForm_bindMenuList(&sg_stMenuManage, pstMenuList,menuNum,fnShowMenuFuncPtrcPtr);
}

/* 菜单功能设置 */
int cotMenu_SelectLanguage(uint8_t languageIdx)
{
	LibXmCore_MenuForm_selectLanguage(&sg_stMenuManage, languageIdx);
}

/* 菜单选项显示时需要使用的功能扩展函数 */

int cotMenu_LimitShowListNum(MenuShow_s *ptMenuShow, MenuSize *pShowNum)
{
	LibXmCore_MenuForm_limitShowListNum(ptMenuShow, pShowNum);
}
int cotMenu_QueryParentMenu(MenuShow_s *ptMenuShow, uint8_t level)
{
	LibXmCore_MenuForm_queryParentMenu(&sg_stMenuManage, ptMenuShow, level);
}

/* 菜单操作 */

int cotMenu_MainEnter(void)
{
	LibXmCore_MenuForm_startEnterMenu(&sg_stMenuManage);
}
int cotMenu_MainExit(void)
{	
	LibXmCore_MenuForm_exitStopMenu(&sg_stMenuManage);
}

int cotMenu_Reset(void)
{
	LibXmCore_MenuForm_reset(&sg_stMenuManage);

}
int cotMenu_Enter(void)
{
	LibXmCore_MenuForm_enter(&sg_stMenuManage);

}
int cotMenu_Exit(bool isReset)
{
	LibXmCore_MenuForm_exit(&sg_stMenuManage, isReset);

}
int cotMenu_SelectPrevious(bool isAllowRoll)
{
	LibXmCore_MenuForm_selectPrevious(&sg_stMenuManage, isAllowRoll);

}
int cotMenu_SelectNext(bool isAllowRoll)
{
	LibXmCore_MenuForm_selectNext(&sg_stMenuManage, isAllowRoll);
}

int cotMenu_ShortcutEnter(bool isAbsolute, uint8_t deep, ...)
{
//	int LibXmCore_MenuForm_shortcutEnter(&sg_stMenuManag, isAbsolute, deep, ...);
}

/* 菜单轮询处理任务 */
int cotMenu_Task(void)
{
	LibXmCore_MenuForm_processTask(&sg_stMenuManage);
}

void cotMenu_setTick(uint16_t timeTick)
{
	LibXmCore_MenuForm_setTick(&sg_stMenuManage, timeTick);
}
void cotMenu_refreshMenu(void)
{
	LibXmCore_MenuForm_refreshMenu(&sg_stMenuManage);
}
void cotMenu_setRefreshFreqDiv(uint16_t refreshFreqDiv)
{
	LibXmCore_MenuForm_setRefreshFreqDiv(&sg_stMenuManage,refreshFreqDiv);
}



uint16_t cotMenu_getTick()
{
	return LibXmCore_MenuForm_getTick(&sg_stMenuManage);
}

void XmMcu_cotMenu_getInput(void* pExtendInputData)
{
	uint8_t cmd;
	MenuInput_s* pstMenuInput = pExtendInputData;
    printf("选择操作(h-退出主菜单; l-进入; j-↓; k-↑; 5-快捷音乐; 6-快捷语言; 7-快捷更多设置): cmd:%d\r\n",cmd);
    cmd = getchar();

	pstMenuInput->keyFlag = 1;
	pstMenuInput->keyValue = cmd;
}
uint8_t XmMcu_Ctrl_MenuTask(uint8_t* pContent, uint8_t level, void* pExtendInputData)
{
	MenuInput_s* pstMenuInput = pExtendInputData;
	if(level >= 2)
	{
		return 0;
	}
	static uint8_t cmd;
	uint8_t i = 0;
	if(pstMenuInput->keyFlag)
	{
		cmd = pstMenuInput->keyValue;
		pstMenuInput->keyFlag = 0;
	}
	else
	{
		return 0;
	}

	printf("Content%d:", level);
	for(i = 0;i<level;i++)
	{
		printf("%d",pContent[i]);
	}
	printf("\r\n");
    switch (cmd)
    {
	    case 'h':
	    	if(level == 0)
	    	{
				cotMenu_MainExit();
	    	}
	    	else
	    	{
				cotMenu_Exit(false);
			}

	        break;
	    case 'm':
	        cotMenu_Reset();
	        break;
	    case 'l':
	        cotMenu_Enter();
	        break;
	    case 'j':
	        cotMenu_SelectNext(true);
	        break;
	    case 'k':
	        cotMenu_SelectPrevious(true);
	        break;
	    default:
	    	printf("not found cmd\r\n");
	    	break;
    }

//	sleep(1);
	return 1;
}


