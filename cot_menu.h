/**
  **********************************************************************************************************************
  * @file    cot_menu.h
  * @brief   该文件提供菜单框架所有函数原型
  * @author  const_zpc    any question please send mail to const_zpc@163.com
  * @date    2023-06-21
  **********************************************************************************************************************
  *
  **********************************************************************************************************************
  */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef MENU_H
#define MENU_H


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

/******************************************* 配置项 ********************************************************************/


/* Exported types ----------------------------------------------------------------------------------------------------*/

#if MENU_MAX_NUM < 255
typedef uint8_t menusize_t;
#else
typedef uint16_t menusize_t;
#endif

typedef void (*MenuCallFun_f)(void);

typedef struct
{
    menusize_t itemsNum;                /*!< 当前菜单中选项的总数目 */

    menusize_t selectItem;              /*!< 当前菜单中被选中的选项 */

    menusize_t showBaseItem;            /*!< 当前菜单首个显示的选项 */
    
    char     *pszDesc;                  /*!< 当前菜单的字符串描述 */

    char *pszItemsDesc[MENU_MAX_NUM];   /*!< 当前菜单中所有选项的字符串描述 */

    void *pItemsExData[MENU_MAX_NUM];   /*!< 当前菜单中所有选项注册时的扩展数据 */
} MenuShow_t;

typedef void (*ShowMenuCallFun_f)(MenuShow_t *ptShowInfo); 

/**
  * @brief 菜单信息注册结构体
  * 
  */
typedef struct
{
    const char     *pszDesc;            /*!< 当前选项的中文字符串描述 */

    const char     *pszEnDesc;          /*!< 当前选项的英文字符串描述 */

    MenuCallFun_f     pfnLoadCallFun;   /*!< 当前菜单选项每次加载时需要执行一次的函数, 为NULL不执行 */

    MenuCallFun_f     pfnExitCallFun;   /*!< 当前菜单选项进入退出时需要执行一次的函数, 为NULL不执行 */

    MenuCallFun_f     pfnRunCallFun;    /*!< 当前菜单选项的周期调度函数 */

    void             *pExtendData;      /*!< 当前选项的菜单显示效果函数扩展数据入参, 可自行设置该内容 */
} MenuList_t, MenuItem_t;

/**
  * @brief 菜单信息注册结构体
  * 
  */
typedef struct
{
    const char     *pszDesc;            /*!< 主菜单的中文字符串描述 */

    const char     *pszEnDesc;          /*!< 主菜单的英文字符串描述 */

    MenuCallFun_f   pfnLoadCallFun;     /*!< 主菜单每次加载时需要执行一次的函数, 为NULL不执行 */

    MenuCallFun_f   pfnRunCallFun;      /*!< 主菜单周期调度函数 */
}MainMenuCfg_t;

/* Exported constants ------------------------------------------------------------------------------------------------*/
/* Exported macro ----------------------------------------------------------------------------------------------------*/

#define GET_MENU_NUM(X)    (sizeof(X) / sizeof(MenuList_t))

/* Exported functions ------------------------------------------------------------------------------------------------*/

/* 菜单初始化和反初始化 */

extern int cotMenu_Init(MainMenuCfg_t *pMainMenu);
extern int cotMenu_DeInit(void);

extern int cotMenu_Bind(MenuList_t *pMenuList, menusize_t menuNum, ShowMenuCallFun_f pfnShowMenuFun);

/* 菜单功能设置 */

extern int cotMenu_EnableEnglish(bool isEnable);

/* 菜单选项显示时需要使用的功能扩展函数 */

extern int cotMenu_LimitShowListNum(MenuShow_t *ptMenuShow, menusize_t *pShowNum);
extern int cotMenu_QueryParentMenu(MenuShow_t *ptMenuShow, uint8_t level);

/* 菜单状态获取函数 */

extern bool cotMenu_IsRun(void);
extern bool cotMenu_IsEnglish(void);

/* 菜单操作 */

extern int cotMenu_Reset(void);
extern int cotMenu_Enter(void);
extern int cotMenu_Exit(bool isReset);
extern int cotMenu_SelectPrevious(bool isAllowRoll);
extern int cotMenu_SelectNext(bool isAllowRoll);

extern int cotMenu_ShortcutEnter(bool isAbsolute, uint8_t deep, ...);

/* 菜单轮询处理任务 */

extern int cotMenu_Task(void);

#ifdef __cplusplus
 }
#endif

#endif // MENU_H