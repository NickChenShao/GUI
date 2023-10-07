/**
  **********************************************************************************************************************
  * @file    xmcore_menu_form.h
  * @brief   该文件提供菜单框架所有函数原型
  * @author  const_zpc    any question please send mail to const_zpc@163.com
  * @date    2023-06-21
  **********************************************************************************************************************
  *
  **********************************************************************************************************************
  */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef COT_MENU_H
#define COT_MENU_H


/* Includes ----------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL 0
#endif

/******************************************* 配置项 ********************************************************************/

/* 定义 _MENU_USE_MALLOC_ 则采用 malloc/free 的方式实现多级菜单, 否则通过数组的形式 */
// #define _MENU_USE_MALLOC_

/* 定义 _MENU_USE_SHORTCUT_ 则启用快捷菜单选项进入功能 */
#define _MENU_USE_SHORTCUT_

/* 多级菜单深度 */
#define MENU_MAX_DEPTH              10

/* 菜单支持的最大选项数目 */
#define MENU_MAX_NUM                20

/* 菜单支持的语种数目 */
#define MENU_SUPPORT_LANGUAGE       2

/******************************************* 配置项 ********************************************************************/


/* Exported types ----------------------------------------------------------------------------------------------------*/

#if MENU_MAX_NUM < 255
typedef uint8_t MenuSize;
#else
typedef uint16_t MenuSize;
#endif

typedef void (*menuAnyCallFuncPtr)(void);
typedef uint8_t (*MenuAnyCallFuncPtr)(uint8_t* pContentAddr,uint8_t level,void* pExtendInputData);
typedef void (*MenuRunCallFuncPtr)(void* pExtendInputData);
typedef void (*MenuGetInputDataCallFuncPtr)(void* pExtendInputData);


typedef struct
{
	MenuSize itemsNum;                /*!< 当前菜单中选项的总数目 */

	MenuSize selectItem;              /*!< 当前菜单中被选中的选项 */

	MenuSize showBaseItem;            /*!< 当前菜单首个显示的选项 */

	char*    apszDesc;                  /*!< 当前菜单的字符串描述 */

	char*    pszItemsDesc[MENU_MAX_NUM];   /*!< 当前菜单中所有选项的字符串描述 */

	void*    pItemsExData[MENU_MAX_NUM];   /*!< 当前菜单中所有选项注册时的扩展数据 */
} MenuShow_s;

typedef void (*ShowmenuAnyCallFuncPtr)(MenuShow_s* pstShowInfo);
typedef void (*RunmenuAnyCallFuncPtr)(MenuShow_s* pstShowInfo);


/**
  * @brief 菜单信息注册结构体
  *
  */
typedef struct
{
	const char*			apszDesc[MENU_SUPPORT_LANGUAGE];/*!< 当前选项的字符串描述(多语种) */

	menuAnyCallFuncPtr		fnEnterCallFuncPtr;  /*!< 当前菜单选项进入时(从父菜单进入)需要执行一次的函数, 为NULL不执行 */

	menuAnyCallFuncPtr		fnExitCallFuncPtr;   /*!< 当前菜单选项进入后退出时(退出至父菜单)需要执行一次的函数, 为NULL不执行 */

	menuAnyCallFuncPtr		fnLoadCallFuncPtr;   /*!< 当前菜单选项每次加载时(从父菜单进入或子菜单退出)需要执行一次的函数, 为NULL不执行 */

	MenuRunCallFuncPtr		fnRunCallFuncPtr;    /*!< 当前菜单选项的周期调度函数 */

	void* 				pExtendData;      /*!< 当前选项的菜单显示效果函数扩展数据入参, 可自行设置该内容 */
} MenuList_s, MenuItem_t;

/**
  * @brief 菜单信息注册结构体
  *
  */
typedef struct
{
	const char*      apszDesc[MENU_SUPPORT_LANGUAGE];/*!< 当前选项的字符串描述(多语种) */

	menuAnyCallFuncPtr   fnEnterCallFuncPtr;    /*!< 主前菜单进入时(进入菜单)需要执行一次的函数, 为NULL不执行 */

	menuAnyCallFuncPtr   fnExitCallFuncPtr;     /*!< 主前菜单进入后退出时(退出菜单)需要执行一次的函数, 为NULL不执行 */

	menuAnyCallFuncPtr   fnLoadCallFuncPtr;     /*!< 主菜单每次加载时需要执行一次的函数, 为NULL不执行 */

	MenuRunCallFuncPtr   fnRunCallFuncPtr;      /*!< 主菜单周期调度函数 */
} MainMenuCfg_s;

/**
  * @brief 菜单控制信息
  *
  */
typedef struct MenuCtrl
{
	struct MenuCtrl*	pstParentMenuCtrl;    /*!< 父菜单控制处理 */
	char*				apszDesc[MENU_SUPPORT_LANGUAGE];/*!< 当前选项的字符串描述(多语种) */
	ShowmenuAnyCallFuncPtr	fnShowMenuFuncPtr;     /*!< 当前菜单显示效果函数 */
	MenuList_s*			pstMenuList;          /*!< 当前菜单列表 */
	menuAnyCallFuncPtr		fnLoadCallFuncPtr;     /*!< 当前菜单加载函数 */
	MenuRunCallFuncPtr	fnRunCallFuncPtr;      /*!< 当前选项的非菜单功能函数 */
	MenuSize			itemsNum;           /*!< 当前菜单选项总数目 */
	MenuSize			showBaseItem;       /*!< 当前菜单首个显示的选项 */
	MenuSize			selectItem;         /*!< 当前菜单选中的选项 */
	bool				isSelected;         /*!< 菜单选项是否已经被选择 */
} MenuCtrl_s;

/**
  * @brief 菜单管理信息
  *
  */

typedef struct
{
	MenuCtrl_s*			pstNowMenuCtrl;           /*!< 当前菜单控制处理 */
	menuAnyCallFuncPtr		fnMainEnterCallFuncPtr; /*!< 主菜单进入时(进入菜单)需要执行一次的函数 */
	menuAnyCallFuncPtr		fnMainExitCallFuncPtr;  /*!< 主菜单进入后退出时(退出菜单)需要执行一次的函数 */
	menuAnyCallFuncPtr		fnLoadCallFuncPtr;      /*!< 重加载函数 */
//	menuAnyCallFuncPtr 	fnLoadCallFuncPtr;		/*!< 重加载函数 */
	uint8_t				language;            /*!< 语种选择 */
	uint8_t				isEnterMainMenu : 1; /*!< 是否进入了主菜单 */
#ifndef _MENU_USE_MALLOC_	
	MenuCtrl_s*			pArrMenuCtrlBuf;
	MenuSize			menuCtrlMaxDepth;
#endif
	MenuGetInputDataCallFuncPtr	menuGetInputDataCallFuncPtr;
	void*				pExtendInputData;
	MenuAnyCallFuncPtr	menuAnyCallFuncPtr;
	MenuSize			currMenuDepth;
	uint16_t			timeTick;
	uint16_t			confTimeTick;
	
	uint16_t			onShowTimeTick;
	uint16_t			onShowConfTimeTick;
	uint16_t			needFastRefresh;
} MenuManage_s;

/* Exported constants ------------------------------------------------------------------------------------------------*/
/* Exported macro ----------------------------------------------------------------------------------------------------*/

#define GET_MENU_NUM(X)    (sizeof(X) / sizeof(MenuList_s))

/* Exported functions ------------------------------------------------------------------------------------------------*/

/* 菜单初始化和反初始化 */

int LibXmCore_MenuForm_init(MenuManage_s* pstMenuManage, MainMenuCfg_s* pstMainMenu,	MenuCtrl_s*	pArrMenuCtrlBuf, MenuSize menuCtrlMaxDepth,MenuAnyCallFuncPtr menuAnyCallFuncPtr, 	MenuGetInputDataCallFuncPtr	fnGetInputDataCallFuncPtr, void* pExtendData);
int LibXmCore_MenuForm_deInit(MenuManage_s* pstMenuManage);

int LibXmCore_MenuForm_bindMenuList(MenuManage_s* pstMenuManage,MenuList_s* pstMenuList, MenuSize menuNum, ShowmenuAnyCallFuncPtr fnShowMenuFuncPtr);

/* 菜单功能设置 */

int LibXmCore_MenuForm_selectLanguage(MenuManage_s* pstMenuManage, uint8_t languageIdx);


/* 菜单选项显示时需要使用的功能扩展函数 */

int LibXmCore_MenuForm_limitShowListNum(MenuShow_s* ptMenuShow, MenuSize* pShowNum);

int LibXmCore_MenuForm_queryParentMenu(MenuManage_s* pstMenuManage, MenuShow_s* ptMenuShow, uint8_t level);

void LibXmCore_MenuForm_getMenuAddr(MenuManage_s* pstMenuManage, uint8_t* pContentAddr, uint8_t* pLevel);

/* 菜单操作 */
//开始和停止菜单
int LibXmCore_MenuForm_startEnterMenu(MenuManage_s* pstMenuManage);
int LibXmCore_MenuForm_exitStopMenu(MenuManage_s* pstMenuManage);


int LibXmCore_MenuForm_reset(MenuManage_s* pstMenuManage);
int LibXmCore_MenuForm_enter(MenuManage_s* pstMenuManage);
int LibXmCore_MenuForm_exit(MenuManage_s* pstMenuManage, bool isReset);
int LibXmCore_MenuForm_selectPrevious(MenuManage_s* pstMenuManage, bool isAllowRoll);
int LibXmCore_MenuForm_selectNext(MenuManage_s* pstMenuManage, bool isAllowRoll);
int LibXmCore_MenuForm_shortcutEnter(MenuManage_s* pstMenuManage, bool isAbsolute, uint8_t deep, ...);

/* 菜单轮询处理任务 */

int LibXmCore_MenuForm_processTask(MenuManage_s* pstMenuManage);
void LibXmCore_MenuForm_setTick(MenuManage_s* pstMenuManage, uint16_t timeTick);
uint16_t LibXmCore_MenuForm_getTick(MenuManage_s* pstMenuManage);
void LibXmCore_MenuForm_refreshMenu(MenuManage_s* pstMenuManage);
	void LibXmCore_MenuForm_setRefreshFreqDiv(MenuManage_s* pstMenuManage, uint16_t refreshFreqDiv);



#ifdef __cplusplus
}
#endif

#endif // MENU_H
