#ifndef MAIN_HMI_H
#define MAIN_HMI_H

#include "hmi_common.h"
#include "xmcore_menu_form.h"
#include <unistd.h>
void Hmi_LoadMainHmi(uint8_t menuState);
void XmMcu_MainForm_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo);



//菜单界面相关对接应用层的GUI
extern int cotMenu_Init(MainMenuCfg_s *pMainMenu);
extern int cotMenu_DeInit(void);

extern int cotMenu_Bind(MenuList_s *pstMenuList, MenuSize menuNum, ShowmenuAnyCallFuncPtr fnShowMenuFuncPtrcPtr, const void* pTileExData);

/* 菜单功能设置 */

extern int cotMenu_SelectLanguage(uint8_t languageIdx);

/* 菜单选项显示时需要使用的功能扩展函数 */

extern int cotMenu_LimitShowListNum(MenuShow_s *ptMenuShow, MenuSize *pShowNum);
extern int cotMenu_QueryParentMenu(MenuShow_s *ptMenuShow, uint8_t level);

/* 菜单操作 */

extern int cotMenu_MainEnter(void);
extern int cotMenu_MainExit(void);

extern int cotMenu_Reset(void);
extern int cotMenu_Enter(void);
extern int cotMenu_Exit(bool isReset);
extern int cotMenu_SelectPrevious(bool isAllowRoll);
extern int cotMenu_SelectNext(bool isAllowRoll);

extern int cotMenu_ShortcutEnter(bool isAbsolute, uint8_t deep, ...);

/* 菜单轮询处理任务 */

extern int cotMenu_Task(void);

extern void cotMenu_setTick(uint16_t timeTick,  uint8_t isIgnoreFirstTick);

extern void cotMenu_setTickAtLoad(MenuSetTickType_e eMenuSetTickType, uint16_t timeTick,uint8_t isIgnoreFirstTick);

extern uint16_t cotMenu_getTick(void);

extern void cotMenu_refreshMenu(void);

extern void cotMenu_setMenuState(uint8_t menuState, uint16_t aTimeTick,uint8_t isIgnoreFirstTick);

extern void cotMenu_changeMenuState(uint8_t menuState);


uint8_t XmMcu_Ctrl_MenuTask(uint8_t* pContent, uint8_t level, void* pExtendInputData);
void XmMcu_cotMenu_getInput(void* pExtendInputData);
#endif
