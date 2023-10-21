#include "StartForm.h"
#include "peripheral_fun_share.h"


#define FNS_ALL_FORM_CLOSE()
#define FNS_MENUFORM_ONCHAR(...)

extern void XmMcu_ShowMainMenu(MenuShow_s *pstShowInfo,uint8_t menuState);

extern void XmMcu_SystemManager_enter(void);
extern void XmMcu_SystemManager_exit(void);
extern void XmMcu_SystemManager_load(uint8_t menuState);
extern void XmMcu_SystemManager_task(void* pExtendInputData, uint8_t menuState, MenuShow_s* pstShowInfo);


/* 主菜单 */
MenuList_s s_OptionMenu[] = 
{
    {{"用户管理", "user management"},  XmMcu_SystemManager_enter, XmMcu_SystemManager_exit, XmMcu_SystemManager_load, XmMcu_SystemManager_task, NULL},
    
//    {{"系统管理", "  Video "},  NULL, Hmi_VideoExit, Hmi_VideoLoad, Hmi_VideoTask, (MenuImage_t *)&sgc_VideoImage},
//    
//    {{"日志管理", " Camera "},  Hmi_CameraEnter, Hmi_CameraExit, Hmi_CameraLoad, Hmi_CameraTask, (MenuImage_t *)&sgc_CameraImage},
//    
//    {{"软件版本", " Setting"},  Hmi_SetEnter, Hmi_SetExit, Hmi_SetLoad, Hmi_SetTask, (MenuImage_t *)&sgc_SettingImage},
};

void XmMcu_MainForm_enter(void)
{

	cotMenu_Bind(s_OptionMenu, GET_MENU_NUM(s_OptionMenu), XmMcu_ShowMainMenu,NULL);
	g_Wake_Time_Count=0;

}

void XmMcu_MainForm_exit(void)
{

}


void XmMcu_MainForm_load(uint8_t menuState)
{
	cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 50, 0);

}

void XmMcu_MainForm_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo)
{
	FNS_MENUFORM_ONCHAR(pThis,keyVal);
	if(!cotMenu_getTick())
	{
		if(g_Wake_Time_Count>20){
			FNS_ALL_FORM_CLOSE();
			g_Wake_Time_Count=0;
		}	
	}
}


	



