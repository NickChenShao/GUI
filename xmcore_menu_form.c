/*************************************************
* Copyright (C), Xiongmai Information Technologies Co., Ltd. All rights reserved.
* 文件名: xmcore_menu_form.h
* 作者@编号: chenshaopeng@07500
* 版本: V1.00
* 创建日期: 2023.9.27
* 功能描述:
*
* 修改历史:
** 1.日期: 2023.9.27
** 修改者: chenshaopeng@07500
** 修改内容: 首次创建 。
*************************************************/

/****************************************************************************/
/*		  include files
*****************************************************************************/

#include "xmcore_menu_form.h"
#include "stdio.h"

#ifdef _MENU_USE_MALLOC_
#include <malloc.h>
#endif

#ifdef _MENU_USE_SHORTCUT_
#include <stdarg.h>
#endif

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
static MenuCtrl_s* MenuForm_newMenu(MenuManage_s* pstMenuManage);
static void MenuForm_deleteMenu(MenuManage_s* pstMenuManage, MenuCtrl_s* pstMenu);

/* Private function --------------------------------------------------------------------------------------------------*/
/**
  * @brief      新建菜单层级
  *
  * @return     MenuCtrl_s*
  */
static MenuCtrl_s* MenuForm_newMenu(MenuManage_s* pstMenuManage)
{
	MenuCtrl_s* pstNowMenuCtrl = NULL;

	if(pstMenuManage->currMenuDepth < pstMenuManage->menuCtrlMaxDepth)
	{
#ifdef _MENU_USE_MALLOC_
		pstNowMenuCtrl = (MenuCtrl_s*)malloc(sizeof(MenuCtrl_s));
#else
		pstNowMenuCtrl = &pstMenuManage->pArrMenuCtrlBuf[pstMenuManage->currMenuDepth];
#endif
		pstMenuManage->currMenuDepth++;
	}

	return pstNowMenuCtrl;
}

/**
  * @brief      销毁菜单层级
  *
  * @param      pstMenu
  */
static void MenuForm_deleteMenu(MenuManage_s* pstMenuManage, MenuCtrl_s* pstMenu)
{
#ifdef _MENU_USE_MALLOC_
	free(pstMenu);
#endif
	if(pstMenuManage->currMenuDepth > 0)
	{
		pstMenuManage->currMenuDepth--;
	}
}

/**
  * @brief      菜单初始化
  *
  * @param[in]  pstMainMenu        主菜单注册信息
  * @return     0,成功; -1,失败
  */
  
int LibXmCore_MenuForm_init(MenuManage_s* pstMenuManage, MainMenuCfg_s* pstMainMenu,	MenuCtrl_s*	pArrMenuCtrlBuf, MenuSize menuCtrlMaxDepth,MenuAnyCallFuncPtr menuAnyCallFuncPtr, 	MenuGetInputDataCallFuncPtr	fnGetInputDataCallFuncPtr, void* pExtendData)
{
	int i;
	MenuCtrl_s* pstNewMenuCtrl = NULL;

	if(pstMenuManage->pstNowMenuCtrl != NULL)
	{
		return -1;
	}
	
#ifndef _MENU_USE_MALLOC_
	pstMenuManage->pArrMenuCtrlBuf = pArrMenuCtrlBuf;
	pstMenuManage->menuCtrlMaxDepth = menuCtrlMaxDepth;
#endif

#if MENU_MAX_DEPTH != 0
	pstMenuManage->currMenuDepth = 0;
#endif

	if((pstNewMenuCtrl = MenuForm_newMenu(pstMenuManage)) == NULL)
	{
		return -1;
	}

	pstMenuManage->language = 0;

	for(i = 0; i < MENU_SUPPORT_LANGUAGE; i++)
	{
		pstNewMenuCtrl->apszDesc[i] = (char*)pstMainMenu->apszDesc[i];
	}

	pstNewMenuCtrl->pstParentMenuCtrl = NULL;
	pstNewMenuCtrl->fnLoadCallFuncPtr = pstMainMenu->fnLoadCallFuncPtr;
	pstNewMenuCtrl->fnShowMenuFuncPtr = NULL;
	pstNewMenuCtrl->fnRunCallFuncPtr = pstMainMenu->fnRunCallFuncPtr;

	pstNewMenuCtrl->pstMenuList = NULL;
	pstNewMenuCtrl->itemsNum = 0;
	pstNewMenuCtrl->selectItem = 0;
	pstNewMenuCtrl->showBaseItem = 0;

	pstMenuManage->pstNowMenuCtrl = pstNewMenuCtrl;
	pstMenuManage->isEnterMainMenu = 0;
	pstMenuManage->fnMainEnterCallFuncPtr = pstMainMenu->fnEnterCallFuncPtr;
	pstMenuManage->fnMainExitCallFuncPtr = pstMainMenu->fnExitCallFuncPtr;
	pstMenuManage->fnLoadCallFuncPtr = pstNewMenuCtrl->fnLoadCallFuncPtr;
	pstMenuManage->menuAnyCallFuncPtr = menuAnyCallFuncPtr;
	pstMenuManage->menuGetInputDataCallFuncPtr = fnGetInputDataCallFuncPtr;
	pstMenuManage->pExtendInputData = pExtendData;
	return 0;
}

/**
  * @brief  菜单反初始化
  *
  * @attention  不管处于任何界面都会逐级退出到主菜单后（会调用退出函数），再退出主菜单，最后反初始化
  * @return 0,成功; -1,失败
  */
int LibXmCore_MenuForm_deInit(MenuManage_s* pstMenuManage)
{
	if(pstMenuManage->pstNowMenuCtrl == NULL)
	{
		return -1;
	}

	LibXmCore_MenuForm_exitStopMenu(pstMenuManage);

	MenuForm_deleteMenu(pstMenuManage,pstMenuManage->pstNowMenuCtrl);
	pstMenuManage->pstNowMenuCtrl = NULL;
	pstMenuManage->language = 0;
	pstMenuManage->isEnterMainMenu = 0;
	pstMenuManage->fnMainEnterCallFuncPtr = NULL;
	pstMenuManage->fnMainExitCallFuncPtr = NULL;
	pstMenuManage->fnLoadCallFuncPtr = NULL;

	return 0;
}

/**
  * @brief      子菜单绑定当前菜单选项
  *
  * @param      pstMenuList       新的菜单列表
  * @param      menuNum         新的菜单列表数目
  * @param      fnShowMenuFuncPtr  新的菜单列表显示效果回调函数, 为NULL则延续上级菜单显示效果
  * @return     int
  */
int LibXmCore_MenuForm_bindMenuList(MenuManage_s* pstMenuManage,MenuList_s* pstMenuList, MenuSize menuNum, ShowmenuAnyCallFuncPtr fnShowMenuFuncPtr)
{
	if(pstMenuManage->pstNowMenuCtrl == NULL)
	{
		return -1;
	}

	if(pstMenuManage->pstNowMenuCtrl->pstMenuList != NULL)
	{
		return 0;
	}

	pstMenuManage->pstNowMenuCtrl->pstMenuList = pstMenuList;
	pstMenuManage->pstNowMenuCtrl->itemsNum = menuNum;

	if(fnShowMenuFuncPtr != NULL)	//表示使用上一层的显示方法
	{
		pstMenuManage->pstNowMenuCtrl->fnShowMenuFuncPtr = fnShowMenuFuncPtr;
	}
	return 0;
}

/**
  * @brief      选择语种
  *
  * @param[in]  languageIdx 语种索引
  * @return     0,成功; -1,失败
  */
int LibXmCore_MenuForm_selectLanguage(MenuManage_s* pstMenuManage, uint8_t languageIdx)
{
	if(MENU_SUPPORT_LANGUAGE <= languageIdx)
	{
		return -1;
	}

	pstMenuManage->language = languageIdx;
	return 0;
}

/**
  * @brief      复位菜单, 回到主菜单界面
  *
  * @note       该复位回到主菜单不会执行退出所需要执行的回调函数
  * @return     0,成功; -1,失败
  */
int LibXmCore_MenuForm_reset(MenuManage_s* pstMenuManage)
{
	if(pstMenuManage->pstNowMenuCtrl == NULL || pstMenuManage->isEnterMainMenu == 0)
	{
		return -1;
	}

	while(pstMenuManage->pstNowMenuCtrl->pstParentMenuCtrl != NULL)
	{
		MenuCtrl_s* pstNowMenuCtrl = pstMenuManage->pstNowMenuCtrl;

		pstMenuManage->pstNowMenuCtrl = pstMenuManage->pstNowMenuCtrl->pstParentMenuCtrl;
		MenuForm_deleteMenu(pstMenuManage, pstNowMenuCtrl);
	}

	pstMenuManage->pstNowMenuCtrl->selectItem = 0;

	return 0;
}

/**
  * @brief      主菜单进入
  *
  * @return     0,成功; -1,失败
  */
int LibXmCore_MenuForm_startEnterMenu(MenuManage_s* pstMenuManage)
{
	if(pstMenuManage->pstNowMenuCtrl == NULL || pstMenuManage->isEnterMainMenu == 1)
	{
		return -1;
	}

	if(pstMenuManage->fnMainEnterCallFuncPtr != NULL)
	{
		pstMenuManage->fnMainEnterCallFuncPtr();
	}

	pstMenuManage->isEnterMainMenu = 1;
	pstMenuManage->fnLoadCallFuncPtr = pstMenuManage->pstNowMenuCtrl->fnLoadCallFuncPtr;

	return 0;
}

/**
  * @brief      主菜单退出
  *
  * @attention  不管处于任何界面都会逐级退出到主菜单后（会调用退出函数），再退出主菜单
  * @return     0,成功; -1,失败
  */
int LibXmCore_MenuForm_exitStopMenu(MenuManage_s* pstMenuManage)
{
	if(pstMenuManage->pstNowMenuCtrl == NULL || pstMenuManage->isEnterMainMenu == 0)
	{
		return -1;
	}

	while(LibXmCore_MenuForm_exit(pstMenuManage,1) == 0)
	{

	}

	if(pstMenuManage->fnMainExitCallFuncPtr != NULL)
	{
		pstMenuManage->fnMainExitCallFuncPtr();
	}

	pstMenuManage->isEnterMainMenu = 0;

	return 0;
}

/**
  * @brief      进入当前菜单选项
  *
  * @return     0,成功; -1,失败
  */
int LibXmCore_MenuForm_enter(MenuManage_s* pstMenuManage)
{
	int i;
	MenuCtrl_s* pstNewMenuCtrl = NULL;
	MenuCtrl_s* pCurrMenuCtrl = pstMenuManage->pstNowMenuCtrl;

	if(pstMenuManage->pstNowMenuCtrl == NULL || pstMenuManage->isEnterMainMenu == 0)
	{
		return -1;
	}

	if((pstNewMenuCtrl = MenuForm_newMenu(pstMenuManage)) == NULL)
	{
		return -1;
	}

	for(i = 0; i < MENU_SUPPORT_LANGUAGE; i++)
	{
		pstNewMenuCtrl->apszDesc[i] = (char*)pCurrMenuCtrl->pstMenuList[pCurrMenuCtrl->selectItem].apszDesc[i];
	}

	pstNewMenuCtrl->pstMenuList = NULL;
	pstNewMenuCtrl->itemsNum = 0;
	pstNewMenuCtrl->fnShowMenuFuncPtr = pCurrMenuCtrl->fnShowMenuFuncPtr;		//临时使用上一层界面的显示回调
	pstNewMenuCtrl->fnLoadCallFuncPtr = pCurrMenuCtrl->pstMenuList[pCurrMenuCtrl->selectItem].fnLoadCallFuncPtr;
	pstNewMenuCtrl->fnRunCallFuncPtr = pCurrMenuCtrl->pstMenuList[pCurrMenuCtrl->selectItem].fnRunCallFuncPtr;
	pstNewMenuCtrl->selectItem = 0;
	pstNewMenuCtrl->isSelected = true;
	pstNewMenuCtrl->pstParentMenuCtrl = pCurrMenuCtrl;

	pstMenuManage->pstNowMenuCtrl = pstNewMenuCtrl;
	pstMenuManage->fnLoadCallFuncPtr = pstNewMenuCtrl->fnLoadCallFuncPtr;

	if(pCurrMenuCtrl->pstMenuList[pCurrMenuCtrl->selectItem].fnEnterCallFuncPtr != NULL)
	{
		pCurrMenuCtrl->pstMenuList[pCurrMenuCtrl->selectItem].fnEnterCallFuncPtr();
	}

	return 0;
}

/**
  * @brief      退出当前选项并返回上一层菜单
  *
  * @param[in]  isReset 菜单选项是否从头选择
  * @return     0,成功; -1,失败, 即目前处于主菜单, 无法返回
  */
int LibXmCore_MenuForm_exit(MenuManage_s* pstMenuManage, bool isReset)
{
	MenuCtrl_s* pstNowMenuCtrl = pstMenuManage->pstNowMenuCtrl;

	if(pstMenuManage->pstNowMenuCtrl == NULL || pstMenuManage->isEnterMainMenu == 0)
	{
		printf("error 2\r\n");
		return -1;
	}

	if(pstMenuManage->pstNowMenuCtrl->pstParentMenuCtrl == NULL)
	{
		printf("error 1\r\n");
		return -1;
	}

	pstMenuManage->pstNowMenuCtrl = pstMenuManage->pstNowMenuCtrl->pstParentMenuCtrl;
	pstMenuManage->fnLoadCallFuncPtr = pstMenuManage->pstNowMenuCtrl->fnLoadCallFuncPtr;
	MenuForm_deleteMenu(pstMenuManage, pstNowMenuCtrl);
	pstNowMenuCtrl = NULL;

	if(pstMenuManage->pstNowMenuCtrl->pstMenuList[pstMenuManage->pstNowMenuCtrl->selectItem].fnExitCallFuncPtr != NULL)
	{
		pstMenuManage->pstNowMenuCtrl->isSelected = false;
		pstMenuManage->pstNowMenuCtrl->pstMenuList[pstMenuManage->pstNowMenuCtrl->selectItem].fnExitCallFuncPtr();
	}

	if(isReset)
	{
		pstMenuManage->pstNowMenuCtrl->selectItem = 0;
	}

	return 0;
}

/**
  * @brief      选择上一个菜单选项
  *
  * @param[in]  isAllowRoll 第一个选项时是否从跳转到最后一个选项
  * @return     0,成功; -1,失败
  */
int LibXmCore_MenuForm_selectPrevious(MenuManage_s* pstMenuManage, bool isAllowRoll)
{
	if(pstMenuManage->pstNowMenuCtrl == NULL || pstMenuManage->isEnterMainMenu == 0 || pstMenuManage->pstNowMenuCtrl->itemsNum == 0)
	{
		return -1;
	}

	if(pstMenuManage->pstNowMenuCtrl->selectItem > 0)
	{
		pstMenuManage->pstNowMenuCtrl->selectItem--;
	}
	else
	{
		if(isAllowRoll)
		{
			pstMenuManage->pstNowMenuCtrl->selectItem = pstMenuManage->pstNowMenuCtrl->itemsNum - 1;
		}
		else
		{
			pstMenuManage->pstNowMenuCtrl->selectItem = 0;
			return -1;
		}
	}

	return 0;
}

/**
  * @brief      选择下一个菜单选项
  *
  * @param[in]  isAllowRoll 最后一个选项时是否跳转到第一个选项
  * @return     0,成功; -1,失败
  */
int LibXmCore_MenuForm_selectNext(MenuManage_s* pstMenuManage, bool isAllowRoll)
{
	if(pstMenuManage->pstNowMenuCtrl == NULL || pstMenuManage->isEnterMainMenu == 0 || pstMenuManage->pstNowMenuCtrl->itemsNum == 0)
	{
		return -1;
	}

	if(pstMenuManage->pstNowMenuCtrl->selectItem < (pstMenuManage->pstNowMenuCtrl->itemsNum - 1))
	{
		pstMenuManage->pstNowMenuCtrl->selectItem++;
	}
	else
	{
		if(isAllowRoll)
		{
			pstMenuManage->pstNowMenuCtrl->selectItem = 0;
		}
		else
		{
			pstMenuManage->pstNowMenuCtrl->selectItem = pstMenuManage->pstNowMenuCtrl->itemsNum - 1;
			return -1;
		}
	}

	return 0;
}

#ifdef _MENU_USE_SHORTCUT_

/**
 * @brief      相对主菜单或当前菜单通过下级各菜单索引快速进入指定选项
 *
 * @param[in]  isAbsolute 是否采用绝对菜单索引（从主菜单开始）
 * @param[in]  deep 菜单深度，大于 0
 * @param[in]  ...  各级菜单索引值(从0开始), 入参个数由 deep 的值决定
 * @return     0,成功; -1,失败
 */
int LibXmCore_MenuForm_shortcutEnter(MenuManage_s* pstMenuManage, bool isAbsolute, uint8_t deep, ...)
{
	uint8_t selectDeep = 0;
	va_list pItemList;
	MenuSize selectItem;

	if(pstMenuManage->pstNowMenuCtrl == NULL || pstMenuManage->isEnterMainMenu == 0)
	{
		return -1;
	}

	if(isAbsolute)
	{
		LibXmCore_MenuForm_reset(pstMenuManage);
	}

	va_start(pItemList, deep);

	while(selectDeep < deep)
	{
		selectItem = va_arg(pItemList, int);

		if(selectItem >= pstMenuManage->pstNowMenuCtrl->itemsNum)
		{
			va_end(pItemList);
			return -1;
		}

		pstMenuManage->pstNowMenuCtrl->selectItem = selectItem;
		LibXmCore_MenuForm_enter(pstMenuManage);
		selectDeep++;
	}

	va_end(pItemList);

	return 0;
}

#endif

/**
  * @brief      限制当前菜单界面最多显示的菜单数目
  *
  * @note       在菜单显示效果回调函数中使用, 使用成员变量 showBaseItem 得到显示界面的第一个选项索引
  * @param[in,out]  tMenuShow   当前菜单显示信息
  * @param[in,out]  showNum     当前菜单中需要显示的选项数目, 根据当前菜单选项的总数得到最终的显示的选项数目
  * @return     0,成功; -1,失败
  */
int LibXmCore_MenuForm_limitShowListNum(MenuShow_s* ptMenuShow, MenuSize* pShowNum)
{
	if(ptMenuShow == NULL || pShowNum == NULL)
	{
		return -1;
	}

	if(*pShowNum > ptMenuShow->itemsNum)
	{
		*pShowNum = ptMenuShow->itemsNum;
	}

	if(ptMenuShow->selectItem < ptMenuShow->showBaseItem)
	{
		ptMenuShow->showBaseItem = ptMenuShow->selectItem;
	}
	else if((ptMenuShow->selectItem - ptMenuShow->showBaseItem) >= *pShowNum)
	{
		ptMenuShow->showBaseItem = ptMenuShow->selectItem - *pShowNum + 1;
	}
	else
	{
		// 保持
	}

	return 0;
}

/**
 * @brief       获取当前父菜单显示信息
 *              如获取当前菜单的二级父菜单信息，level 为2
 *
 * @param[out]  ptMenuShow 父 n 级菜单显示信息
 * @param[in]   level      n 级, 大于 0
 * @return int
 */
int LibXmCore_MenuForm_queryParentMenu(MenuManage_s* pstMenuManage, MenuShow_s* ptMenuShow, uint8_t level)
{
	int i;
	MenuList_s* pstMenu;
	MenuCtrl_s* pstNowMenuCtrl = NULL;

	if(pstMenuManage->pstNowMenuCtrl == NULL || pstMenuManage->isEnterMainMenu == 0)
	{
		return -1;
	}

	pstNowMenuCtrl = pstMenuManage->pstNowMenuCtrl->pstParentMenuCtrl;

	while(level && pstNowMenuCtrl != NULL)
	{
		pstMenu = pstNowMenuCtrl->pstMenuList;
		ptMenuShow->itemsNum = pstNowMenuCtrl->itemsNum;
		ptMenuShow->selectItem = pstNowMenuCtrl->selectItem;
		ptMenuShow->showBaseItem = pstNowMenuCtrl->showBaseItem;

		ptMenuShow->apszDesc = pstMenuManage->pstNowMenuCtrl->apszDesc[pstMenuManage->language];

		for(i = 0; i < ptMenuShow->itemsNum && i < MENU_MAX_NUM; i++)
		{
			ptMenuShow->pszItemsDesc[i] = (char*)pstMenu[i].apszDesc[pstMenuManage->language];
			ptMenuShow->pItemsExData[i] = pstMenu[i].pExtendData;
		}

		pstNowMenuCtrl = pstNowMenuCtrl->pstParentMenuCtrl;
		level--;
	}

	if(level != 0 && pstNowMenuCtrl == NULL)
	{
		return -1;
	}

	return 0;
}
/**
  * @brief      获取当前菜单的地址
  *
  * @param[in out]  pContentAddr 地址数组
  * @param[in out]  pLevel		获取当前嵌入级别,取传入的和当前的最小的那个数值传出
  *
  */
void LibXmCore_MenuForm_getMenuAddr(MenuManage_s* pstMenuManage, uint8_t* pContentAddr, uint8_t* pLevel)
{
	MenuList_s* pstMenu;
	MenuCtrl_s* pstTempMenuCtrl = pstMenuManage->pArrMenuCtrlBuf;
	uint8_t i = 0;
	if(pstMenuManage->currMenuDepth <= 1)
	{
		*pLevel = 0;
		return ;
	}
	for(i = 0;(i < pstMenuManage->currMenuDepth) && (i < *pLevel);i++)	//取较小的一个
	{
		pContentAddr[i] = pstTempMenuCtrl[i].selectItem;
	}
	*pLevel = i-1;	//向上降一级表示是固定的
}

/**
  * @brief  菜单任务
  *
  * @return 0,成功, 处于菜单模式下; -1,失败, 未处于菜单模式下
  */
int LibXmCore_MenuForm_processTask(MenuManage_s* pstMenuManage)
{
	int i;
	MenuList_s* pstMenuList;
	MenuShow_s tMenuShow;
	uint8_t aContentAddr[pstMenuManage->menuCtrlMaxDepth];
	uint8_t level = pstMenuManage->menuCtrlMaxDepth;
	if(pstMenuManage->pstNowMenuCtrl == NULL || pstMenuManage->isEnterMainMenu == 0)
	{
		return -1;
	}

	if(pstMenuManage->fnLoadCallFuncPtr != NULL)
	{
		pstMenuManage->fnLoadCallFuncPtr();
		pstMenuManage->fnLoadCallFuncPtr = NULL;
	}
	if((pstMenuManage->onShowTimeTick == 0) || (pstMenuManage->needFastRefresh))
	{
		if(pstMenuManage->pstNowMenuCtrl->pstMenuList != NULL)
		{
			pstMenuList = pstMenuManage->pstNowMenuCtrl->pstMenuList;
			tMenuShow.itemsNum = pstMenuManage->pstNowMenuCtrl->itemsNum;
			tMenuShow.selectItem = pstMenuManage->pstNowMenuCtrl->selectItem;
			tMenuShow.showBaseItem = pstMenuManage->pstNowMenuCtrl->showBaseItem;
		
			tMenuShow.apszDesc = pstMenuManage->pstNowMenuCtrl->apszDesc[pstMenuManage->language];
		
			for(i = 0; i < tMenuShow.itemsNum && i < MENU_MAX_NUM; i++)
			{
				tMenuShow.pszItemsDesc[i] = (char*)pstMenuList[i].apszDesc[pstMenuManage->language];
				tMenuShow.pItemsExData[i] = pstMenuList[i].pExtendData;
			}
		
			if(pstMenuManage->pstNowMenuCtrl->fnShowMenuFuncPtr != NULL)
			{
				pstMenuManage->pstNowMenuCtrl->fnShowMenuFuncPtr(&tMenuShow);
			}
		
			pstMenuManage->pstNowMenuCtrl->showBaseItem = tMenuShow.showBaseItem;
		}
		pstMenuManage->needFastRefresh = 0;
		if(pstMenuManage->onShowConfTimeTick)
		{
			pstMenuManage->onShowTimeTick = pstMenuManage->onShowConfTimeTick - 1;
		}
	}
	else
	{
			pstMenuManage->onShowTimeTick--;
	}

	if(pstMenuManage->pExtendInputData && pstMenuManage->menuGetInputDataCallFuncPtr)
	{	//获取输入的数据
		pstMenuManage->menuGetInputDataCallFuncPtr(pstMenuManage->pExtendInputData);
	}

	if(pstMenuManage->menuAnyCallFuncPtr)
	{
		LibXmCore_MenuForm_getMenuAddr(pstMenuManage,aContentAddr,&level);
		// FIXME:考虑添加断言ELOG_A(level == pstMenuManage->menuCtrlMaxDepth)
		pstMenuManage->menuAnyCallFuncPtr(aContentAddr,level, pstMenuManage->pExtendInputData);
	}
#if 0	//刷新tick后延迟触发
	if(pstMenuManage->timeTick)
	{
		pstMenuManage->timeTick--;
	}
	else
	{
		if(pstMenuManage->confTimeTick)
		{
			pstMenuManage->timeTick = pstMenuManage->confTimeTick - 1;
		}
	}
#else		//刷新tick后直接触发
	pstMenuManage->timeTick++;
	if(pstMenuManage->timeTick < pstMenuManage->confTimeTick)
	{
		
	}
	else
	{
		pstMenuManage->timeTick = 0;
	}
#endif		
	if(pstMenuManage->pstNowMenuCtrl->fnRunCallFuncPtr != NULL)
	{
		pstMenuManage->pstNowMenuCtrl->fnRunCallFuncPtr(pstMenuManage->pExtendInputData);
	}

	return 0;
}
uint16_t LibXmCore_MenuForm_getTick(MenuManage_s* pstMenuManage)
{
	return pstMenuManage->timeTick;
}
void LibXmCore_MenuForm_setTick(MenuManage_s* pstMenuManage, uint16_t timeTick)
{
	pstMenuManage->confTimeTick = timeTick;
	pstMenuManage->timeTick = pstMenuManage->confTimeTick;
}
void LibXmCore_MenuForm_setRefreshFreqDiv(MenuManage_s* pstMenuManage, uint16_t refreshFreqDiv)
{
	pstMenuManage->onShowConfTimeTick = refreshFreqDiv;
}
void LibXmCore_MenuForm_refreshMenu(MenuManage_s* pstMenuManage)
{
	pstMenuManage->needFastRefresh = 1;

}


