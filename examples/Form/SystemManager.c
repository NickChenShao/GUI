#if 1
#include "StartForm.h"
#include "SystemManager.h"
#include <string.h>
#include <stdio.h>
#include "peripheral_fun_share.h"
#include "xmcore_menu_gui.h"


static unsigned char g_SystemSet_Pri_Display;
/*************************************************/

#define FNS_ALL_FORM_CLOSE()
#define FNS_MENUFORM_ONCHAR(...)

/*语音开关*/
#define COMMAND_GET_VOL(...)			//获取当前音量
#define SET_VOICEVALUE(..)				//设置音量

#define PLAY_MUSIC(...)					//播放
#define PLAY_MUSIC_ON(...)
#define WAIT_MUSIC(...)

/*常开模式*/
#define RUN_MOTOR()
#define REVERS_MOTOR()

/*防拆设置*/

/*语言设置*/

/*恢复出厂*/
#define	CLEAN_USER_ALL()	//用户清空
#define SYSTEM2FACTORY()	//系统设置参数恢复

/*配网设置*/

extern void XmMcu_ShowMainMenu(MenuShow_s *pstShowInfo,uint8_t menuState);

extern void XmMcu_SystemManager_enter(void);
extern void XmMcu_SystemManager_exit(void);
extern void XmMcu_SystemManager_load(uint8_t menuState);
extern void XmMcu_SystemManager_task(void* pExtendInputData, uint8_t menuState, MenuShow_s* pstShowInfo);

/*时间设置*/
void SystemTime_enter(void);
void SystemTime_load(uint8_t menuState);
void SystemTime_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo);

/*语音开关*/
void VoiceVolume_enter(void);
void VoiceVolume_exit(void);
void VoiceVolume_load(uint8_t menuState);
void VoiceVolume_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo);

/*常开模式*/
void AlwaysOpenSet_enter(void);
void AlwaysOpenSet_load(uint8_t menuState);
void AlwaysOpenSet_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo);
/*防拆设置*/
void TamperSet_enter(void);
void TamperSet_exit(void);
void TamperSet_load(uint8_t menuState);
void TamperSet_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo);

/*语言设置*/
void LanguageSet_enter(void);
void LanguageSet_exit(void);
void LanguageSet_load(uint8_t menuState);
void LanguageSet_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo);
/*恢复出厂*/
void ResetDefaults_enter(void);
//void ResetDefaults_exit(void);
void ResetDefaults_load(uint8_t menuState);
void ResetDefaults_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo);
/*配网设置*/
void VoiceVolume_enter(void);
void VoiceVolume_exit(void);
void VoiceVolume_load(uint8_t menuState);
void VoiceVolume_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo);

/* 主菜单 */
MenuList_s s_SystemManagerSetMenu[] = 
{
	{{"时间设置", "user management"},		SystemTime_enter,	NULL,				SystemTime_load,	SystemTime_task,	NULL},
	{{"语音开关", " Setting"},				VoiceVolume_enter,	VoiceVolume_exit,	VoiceVolume_load,	VoiceVolume_task,	NULL},
	{{"常开模式", "user management"},  	AlwaysOpenSet_enter,	NULL,				AlwaysOpenSet_load,	AlwaysOpenSet_task,	NULL},
	{{"防拆设置", " Camera "},				TamperSet_enter,	TamperSet_exit,		TamperSet_load,		TamperSet_task,		NULL},
	{{"语言设置", " Camera "},				LanguageSet_enter,	LanguageSet_exit,	LanguageSet_load,	LanguageSet_task,	NULL},
	{{"恢复出厂", " Setting"},  			ResetDefaults_enter,NULL,				ResetDefaults_load,	ResetDefaults_task,	NULL},
	{{"配网设置", " Camera "},				VoiceVolume_enter,	VoiceVolume_exit,	VoiceVolume_load,	VoiceVolume_task,	NULL},
};

void XmMcu_SystemManager_enter(void)
{

	cotMenu_Bind(s_SystemManagerSetMenu, GET_MENU_NUM(s_SystemManagerSetMenu), XmMcu_ShowMainMenu,NULL);
	g_Wake_Time_Count=0;

}

void XmMcu_SystemManager_exit(void)
{

}


void XmMcu_SystemManager_load(uint8_t menuState)
{
	cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 20, 0);

}

void XmMcu_SystemManager_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo)
{
	FNS_MENUFORM_ONCHAR(pThis,keyVal);
	if(!cotMenu_getTick())
	{
		if(g_Wake_Time_Count>SYSTEM_DELAY_TIME){
			FNS_ALL_FORM_CLOSE();
			g_Wake_Time_Count=0;
		}

	}
}



void SystemTimeSet(MENUFORM* pMenuForm);
void LockTimeSet(MENUFORM* pMenuForm);
void VarificationMode(MENUFORM* pMenuForm);
void VoiceVolumeSet(MENUFORM* pMenuForm);
void AlwaysOpen(MENUFORM* pMenuForm);
void GuestAuthority(MENUFORM* pMenuForm);
void LanguageSet(MENUFORM* pMenuForm);
void ButtonSound(MENUFORM* pMenuForm);
void TamperConfig(MENUFORM* pMenuForm);
void ResetDefaults(MENUFORM* pMenuForm);
void NetworkSet(MENUFORM* pMenuForm);
void RemoteUnlock(MENUFORM* pMenuForm);


void SystemManagerSet_onCreate(BASEFORM* pThis){   
    MENUFORM* pMenuForm=(MENUFORM*)pThis;
    pMenuForm->menuItems=s_SystemManagerSetMenu;
    pMenuForm->itemCount=sizeof(s_SystemManagerSetMenu)/sizeof(MENUITEM);
    pMenuForm->firstIndex=0;
		pMenuForm->curIndex=0;
    pMenuForm->timeTick=2;
		g_Wake_Time_Count=0;
	
	
}
/****************************时间设置***************************************/

uint8_t Lock_Time_Set_Pos=0;
clock_param_t clock_env_temp;

void SystemTime_onShow(MenuShow_s *pstShowInfo,uint8_t menuState)
{
		uint8_t x,y = 1;
		CDC* pFormCDC;
		System_UDOperation_Tip(pFormCDC, pstShowInfo->apszDesc, y);

		switch(menuState)
		{
			case SysTimeSet:   
				y+=16;
				y+=4;
				x=7;
				OLED_ShowMixStr(pFormCDC,x,y,"日期:",1,1,FONT_16X16);
				x+=40;
				if(0==Lock_Time_Set_Pos) OLED_ShowNum(pFormCDC,x,y,(clock_env_temp.year-2000),2,16,0,FONT_16X16);
				else OLED_ShowNum(pFormCDC,x,y,clock_env_temp.year,2,16,1,FONT_16X16);
				x+=16;
				OLED_ShowMixStr(pFormCDC,x,y,"/",1,1,FONT_16X16);
				x+=8;
				if(1==Lock_Time_Set_Pos) OLED_ShowNum(pFormCDC,x,y,clock_env_temp.month,2,16,0,FONT_16X16);
				else OLED_ShowNum(pFormCDC,x,y,clock_env_temp.month,2,16,1,FONT_16X16);
				x+=16;
				OLED_ShowMixStr(pFormCDC,x,y,"/",1,1,FONT_16X16);
				x+=8;
				if(2==Lock_Time_Set_Pos) OLED_ShowNum(pFormCDC,x,y,clock_env_temp.day,2,16,0,FONT_16X16);
				else OLED_ShowNum(pFormCDC,x,y,clock_env_temp.day,2,16,1,FONT_16X16);
				y+=2;
				x=7;
				y+=16;
				OLED_ShowMixStr(pFormCDC,x,y,"时间:",1,1,FONT_16X16);
				x+=40;
				if(3==Lock_Time_Set_Pos) OLED_ShowNum(pFormCDC,x,y,clock_env_temp.hour,2,16,0,FONT_16X16);
				else OLED_ShowNum(pFormCDC,x,y,clock_env_temp.hour,2,16,1,FONT_16X16);
				x+=16;
				OLED_ShowMixStr(pFormCDC,x,y,":",1,1,FONT_16X16);
				x+=8;
				if(4==Lock_Time_Set_Pos) OLED_ShowNum(pFormCDC,x,y,clock_env_temp.min,2,16,0,FONT_16X16);
				else OLED_ShowNum(pFormCDC,x,y,clock_env_temp.min,2,16,1,FONT_16X16);
				x+=16;
				OLED_ShowMixStr(pFormCDC,x,y,":",1,1,FONT_16X16);
				x+=8;
				if(5==Lock_Time_Set_Pos) OLED_ShowNum(pFormCDC,x,y,clock_env_temp.sec,2,16,0,FONT_16X16);
				else OLED_ShowNum(pFormCDC,x,y,clock_env_temp.sec,2,16,1,FONT_16X16);				
				break;				
			case SysTimeSetConfirm:
				y=1;
				x=4;
				Dir4_Logo_Icon(pFormCDC,x,y);
				x=12;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
				x=108;
				Dir6_Logo_Icon(pFormCDC,x,y);
				y=24;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"确定更新时间吗\?",1,y,FONT_16X16);
				y=47;
				x=18;
					if(g_Yes_Or_No) OLED_YesOrNO(pFormCDC,x,y,1);
					else OLED_YesOrNO(pFormCDC,x,y,0);
				break;
			case SysTimeSetSuccess: 
				System_SetSuccess_Tip(pFormCDC);
				break;				
			case SysTimeSetFail:
				y=1;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"时间设置错误",1,y,FONT_16X16);
				break;
			default:
				break;
		}
}

void SystemTime_onChar(uint8_t menuState, uint8_t keyVal)
{
	 unsigned char time_ret;
		g_Wake_Time_Count=0;
		switch(menuState)
		{
			case SysTimeSet:
				PLAY_MUSIC(7);
				switch(keyVal)
				{
					case 2:
						switch(Lock_Time_Set_Pos)
						{
							case 0:
								if(clock_env_temp.year>2098)clock_env_temp.year=2000;
								else clock_env_temp.year++;
								break;
							case 1:
								if(clock_env_temp.month>11)clock_env_temp.month=1;
								else clock_env_temp.month++;
								break;
							case 2:
								if(clock_env_temp.day>30)clock_env_temp.day=1;
								else clock_env_temp.day++;
								break;
							case 3:
								if(clock_env_temp.hour>22)clock_env_temp.hour=0;
								else clock_env_temp.hour++;
								break;
							case 4:
								if(clock_env_temp.min>58)clock_env_temp.min=0;
								else clock_env_temp.min++;
								break;
							case 5:
								if(clock_env_temp.sec>58)clock_env_temp.sec =0;
								else clock_env_temp.sec++;
								break;
							default:
								break;
						}
						break;
					case 4:
						if(Lock_Time_Set_Pos) Lock_Time_Set_Pos--;
					else Lock_Time_Set_Pos=5;
						break;
					case 6:
						if(Lock_Time_Set_Pos>4) Lock_Time_Set_Pos=0;
					else Lock_Time_Set_Pos++;
						break;
					case 8:
						switch(Lock_Time_Set_Pos)
						{
							case 0:
								if(clock_env_temp.year==2000)clock_env_temp.year=2099;
								else clock_env_temp.year--;
								break;
							case 1:
								if(clock_env_temp.month==1)clock_env_temp.month=12;
								else clock_env_temp.month--;
								break;
							case 2:
								if(clock_env_temp.day==1)clock_env_temp.day=31;
								else clock_env_temp.day--;
								break;
							case 3:
								if(clock_env_temp.hour==0)clock_env_temp.hour=23;
								else clock_env_temp.hour--;
								break;
							case 4:
								if(clock_env_temp.min==0)clock_env_temp.min=59;
								else clock_env_temp.min--;
								break;
							case 5:
								if(clock_env_temp.sec==0)clock_env_temp.sec =59;
								else clock_env_temp.sec--;
								break;
							default:
								break;
						}
						break;
					case 12:   //确认
						cotMenu_changeMenuState(SysTimeSetConfirm);
						break;
					case 10:  //取消
						cotMenu_Exit();	
						break;
					default:
						break;
				}
				break;
			case SysTimeSetConfirm:
					PLAY_MUSIC(7);
				switch(keyVal)
				{
					case 4:  //确认
						g_Yes_Or_No=1;
					break;
					case 6:  //否
						g_Yes_Or_No=0;
						break;
					case 12:
						if(g_Yes_Or_No){
							//增加对日期的判定
							
							time_ret=Clock_Check(&clock_env_temp);
							if(0==time_ret)
							{
								cotMenu_changeMenuState(SysTimeSetFail);
								g_Yes_Or_No=0;
							}
							else
							{
								memcpy(&clock_env,&clock_env_temp,sizeof(clock_param_t));
								g_Yes_Or_No=0;
								fns_Form_redraw(); 
								cotMenu_changeMenuState(SysTimeSetSuccess);
								break;
							}
						}
						else{
							g_Yes_Or_No=0;
							cotMenu_Exit();	
							fns_Form_redraw(); 
						}
						break;
					case 10:
						cotMenu_Exit();	
					break;
					default:
						break;
				}
				fns_Form_redraw();
			break;
						
			case SysTimeSetSuccess: 
				break;
			case SysTimeSetFail: 
				break;
				default:	
				break;					
		}

}

void SystemTime_onTick(uint8_t menuState)
{
		if(g_Wake_Time_Count>SYSTEM_DELAY_TIME)
		{
			FNS_ALL_FORM_CLOSE();
			g_Wake_Time_Count=0;
		}
		switch(menuState)
		{
			case SysTimeSet :   
				break;
			case SysTimeSetConfirm:
				break;
			case SysTimeSetSuccess:  
				cotMenu_Exit();
				break;
			case SysTimeSetFail: 
//				cotMenu_Exit();
				cotMenu_changeMenuState(SysTimeSet);
				break;
			default:	
				break;					
		}
}

void SystemTime_enter(void)
{

	memcpy(&clock_env_temp,&clock_env,sizeof(clock_param_t));
	Lock_Time_Set_Pos=0;
	g_Yes_Or_No=0;
	g_SystemSet_Pri_Display=0;
	cotMenu_changeMenuState(SysTimeSet);
	g_Wake_Time_Count=0;
}


void SystemTime_load(uint8_t menuState)
{
	switch(menuState)
	{
		case SysTimeSet :
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 20, 0);
			break;
		case SysTimeSetConfirm:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 20, 0);
			break;
		case SysTimeSetSuccess:
		case SysTimeSetFail: 
				cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 200, 0);
				break;
		default:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 20, 0);
			break;					
	}

}

void SystemTime_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo)
{
	uitn8_t keyVal;
	SystemTime_onChar(menuState,keyVal);
	if(!cotMenu_getTick())
	{
		SystemTime_onTick(menuState);


	}
}


/*****************语音开关****************************/
static uint8_t g_Voice_Volume=0;
static uint8_t g_Voice_Level=0;
static uint8_t g_VolumeSetFlag=0;

void VoiceVolumeSet_onShow(MenuShow_s *pstShowInfo,uint8_t menuState)
{
	
	unsigned char y;
	
	switch(menuState)
	{
		case VoiceLevelSet:
			CDC* pFormCDC;
			y = 1;
			System_UDOperation_Tip(pFormCDC, pstShowInfo->apszDesc, y);
			y+=16;
			y+=8;
			if(2<g_Voice_Level){
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"音量最高",0,y,FONT_24X22);
			}
			else if(2==g_Voice_Level){
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"音量中等",0,y,FONT_24X22);
			}
			else if(1==g_Voice_Level){
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"音量最低",0,y,FONT_24X22);
			}
			else{
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"语音关闭",0,y,FONT_24X22);
			}
			break;
		case VoiceLevelSetConfirm:
			System_UpdateConfirm_Tip(pFormCDC,g_Yes_Or_No);
			break;
		case VoiceLevelSetSuccess:
			System_SetSuccess_Tip(pFormCDC);
			break;	
		case VoiceLevelSetFail:
			System_SetFail_Tip(pFormCDC);
			break;
	}
}

void VoiceVolumeSet_onChar(uint8_t menuState, uint8_t keyVal)
{
		g_Wake_Time_Count=0;
		switch(menuState)
		{
			case VoiceLevelSet:
				PLAY_MUSIC_ON(7);
				switch(keyVal)
				{
					case 2:
						if(g_Voice_Level<3)g_Voice_Level++;
						g_VolumeSetFlag = 1;
						break;
					case 8:
						if(g_Voice_Level)g_Voice_Level--;
						g_VolumeSetFlag = 1;
						break;
					case 12:   //确认
						cotMenu_changeMenuState(VoiceLevelSetConfirm);
						break;
					case 10:  //取消
						cotMenu_Exit();	
						break;
					default:
						break;
				}
				break;
			case VoiceLevelSetConfirm:
				PLAY_MUSIC(7);
				switch(keyVal)
				{
					case 4:  //确认
						g_Yes_Or_No=1;
						break;
					case 6:  //否
						g_Yes_Or_No=0;
						break;
					case 12:
						if(g_Yes_Or_No){
								g_System_Para_Info.voice_level=g_Voice_Level; 
								System_Para_Save();
								
								g_Yes_Or_No=0;
								cotMenu_changeMenuState(VoiceLevelSetSuccess);
						}
						else
						{
							g_Yes_Or_No=0;
							cotMenu_Exit();	
						}
						break;
					case 10:
						cotMenu_Exit();	
						break;
					default:
						break;
				}
				break;
			case VoiceLevelSetSuccess: 
				break;
			case VoiceLevelSetFail: 
				break;
				default:	
				break;					
		}
}
	
void VoiceVolumeSet_onTick(uint8_t menuState)
{
	unsigned char ret;
	
	if(g_Wake_Time_Count>SYSTEM_DELAY_TIME)
	{
		FNS_ALL_FORM_CLOSE();
		g_Wake_Time_Count=0;
	}	
		
	switch(menuState)
	{
		case VoiceLevelSet:
			//等待音乐播完,调整音量后需要播放开门音乐
			ret=WAIT_MUSIC();
			if(!ret)
			{
				if(g_VolumeSetFlag)
				{
					g_VolumeSetFlag=0;
					SET_VOICEVALUE(g_Voice_Level);
					PLAY_MUSIC(KaiMenYin11);
				}
			}
				
			break;
		case VoiceLevelSetConfirm:
			break;
		case VoiceLevelSetSuccess:
		case VoiceLevelSetFail:
			cotMenu_Exit();
			break;
		default :
			break;
	}
}

void VoiceVolume_enter(void)
{
	g_Voice_Volume = COMMAND_GET_VOL();		
	Xprintf("voice value%d \r\n",g_Voice_Volume);
	if(g_Voice_Volume>85){
		g_Voice_Level=3;
	}
	else if(g_Voice_Volume>42){
		g_Voice_Level=2;
	}
	else if(g_Voice_Volume){
		g_Voice_Level=1;
	}
	else{
		g_Voice_Level=0;
	}

	cotMenu_changeMenuState(VoiceLevelSet);
	g_SystemSet_Pri_Display=0;
	g_Wake_Time_Count=0;

}

void VoiceVolume_exit(void)
{
	unsigned int i=0xfffff;
	while(i) i--;
	SET_VOICEVALUE(g_Voice_Level);
}


void VoiceVolume_load(uint8_t menuState)
{
	switch(menuState)
	{
		case VoiceLevelSet:
		case VoiceLevelSetConfirm:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 20, 0);
			break;
		case VoiceLevelSetSuccess:
		case VoiceLevelSetFail:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 200, 1);
			break;
		default :
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 20, 0);
			break;
	}
}

void VoiceVolume_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo)
{
	uitn8_t keyVal;
	SystemTime_onChar(keyVal);
	if(!cotMenu_getTick())
	{
		SystemTime_onTick(menuState);
	}
}


/**********************常开模式***********************/
static unsigned char g_Always_Open_Flag;
AlwaysOpenDef AlwaysOpenSetStatus;

void AlwaysOpenSet_onShow(MenuShow_s *pstShowInfo,uint8_t menuState)
{
	unsigned char y;
	CDC* pFormCDC;
	switch(menuState)
	{
		case AlwaysOpenSet:
			y=1;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,pstShowInfo->apszDesc,1,y,FONT_16X16);
			y+=16;
			y+=8;
			if(g_Always_Open_Flag)
			{
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"启动常开",0,y,FONT_24X22);
			}
			else
			{
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"关闭常开",0,y,FONT_24X22);
			}
			break;
		case AlwaysOpenSetConfirm:
			System_UpdateConfirm_Tip(pFormCDC,g_Yes_Or_No);
			break;
		case AlwaysOpenSetSuccess:
			System_SetSuccess_Tip(pFormCDC);
			break;	
		case AlwaysOpenSetFail:  
			System_SetFail_Tip(pFormCDC);
			break;
	}
}

void AlwaysOpenSet_onChar(uint8_t menuState, uint8_t keyVal)
{
		g_Wake_Time_Count=0;
		switch(menuState)
		{
			case AlwaysOpenSet:
				PLAY_MUSIC(7);
				switch(keyVal)
				{
					case 2:
						if(!g_Always_Open_Flag)g_Always_Open_Flag=1;
						break;

					case 8:
						if(g_Always_Open_Flag)g_Always_Open_Flag=0;
						break;
					case 12:   //确认
						cotMenu_changeMenuState(AlwaysOpenSetConfirm);
						break;
					case 10:  //取消
						cotMenu_Exit();	
						break;
					default:
						break;
				}
				break;
			case AlwaysOpenSetConfirm:
				PLAY_MUSIC(7);
				switch(keyVal)
				{
					case 4:  //确认
						g_Yes_Or_No=1;
						break;
					case 6:  //否
						g_Yes_Or_No=0;
						break;
					case 12:
						if(g_Yes_Or_No)
						{
							g_Yes_Or_No=0;
							if(g_Always_Open_Flag){
									g_System_Para_Info.always_open_flag=1;
									RUN_MOTOR();
									System_Para_Save();
							}
							else{
									g_System_Para_Info.always_open_flag=0;
									REVERS_MOTOR();
									System_Para_Save();
							}
							cotMenu_changeMenuState(AlwaysOpenSetSuccess);
						}
						else
						{
							g_Yes_Or_No=0;
							cotMenu_Exit();	

						}
						break;
					case 10:
						cotMenu_Exit();	
						break;
					default:
						break;
				}
				break;
			case AlwaysOpenSetSuccess: 
				break;
			case AlwaysOpenSetFail: 
				break;
				default:	
				break;					
		}
}

void AlwaysOpenSet_onTick(uint8_t menuState)
{
		if(g_Wake_Time_Count>SYSTEM_DELAY_TIME)
		{
			FNS_ALL_FORM_CLOSE();
			g_Wake_Time_Count=0;
			fns_Form_redraw();
		}

		switch(menuState)
		{
			case AlwaysOpenSet:
				break;
			case AlwaysOpenSetConfirm:
				break;
			case AlwaysOpenSetSuccess:
			case AlwaysOpenSetFail:
				cotMenu_Exit();
				break;
			default :
				break;
		}
		
}



void AlwaysOpenSet_enter(void)
{
	if(g_System_Para_Info.always_open_flag)
		g_Always_Open_Flag=1;
	else
		g_Always_Open_Flag=0;
	g_Yes_Or_No=0;
	g_SystemSet_Pri_Display=0;
	AlwaysOpenSetStatus= AlwaysOpenSet;
	g_Wake_Time_Count=0;
}

void AlwaysOpenSet_load(uint8_t menuState)
{
	switch(menuState)
	{
		case AlwaysOpenSet:
		case AlwaysOpenSetConfirm:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 20, 0);
			break;
		case AlwaysOpenSetSuccess:
		case AlwaysOpenSetFail:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 200, 1);
			break;
		default :
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 20, 0);
			break;
	}

}

void AlwaysOpenSet_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo)
{
	uitn8_t keyVal;
	AlwaysOpenSet_onChar(menuState, keyVal);
	if(!cotMenu_getTick())
	{
		AlwaysOpenSet_onTick(menuState);
	}
}



/*****************************防拆设置******************************************/


uint8_t TamperSetStr[]={"防拆设置"};
unsigned char  g_TamperSet_Flag;
TamperStateDef TamperSetStatus;


void TamperSet_onShow(MenuShow_s *pstShowInfo,uint8_t menuState)
{
	CDC* pFormCDC;
	unsigned char x,y;
	switch(menuState)
	{
		case TamperSet:
			y = 1;
			System_UDOperation_Tip(pFormCDC, pstShowInfo->apszDesc, y);
			y+=16;
			y+=8;
			if(g_TamperSet_Flag){
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"开启防拆",0,y,FONT_24X22);
			}
			else{
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"关闭防拆",0,y,FONT_24X22);
			}
			break;
		case TamperSetConfirm:
			System_UpdateConfirm_Tip(pFormCDC,g_Yes_Or_No);
			break;
		case TamperSetSuccess:
			System_SetSuccess_Tip(pFormCDC);
			break;	
		case TamperSetFail:  
			System_SetFail_Tip(pFormCDC);
			break;
	}
}

void TamperSet_onChar(uint8_t menuState, uint8_t keyVal)
{
	CDC* pFormCDC;
	g_Wake_Time_Count=0;
	switch(menuState)
	{
		case TamperSet:
			PLAY_MUSIC(7);
			switch(keyVal)
			{
				case 2:
					if(!g_TamperSet_Flag)g_TamperSet_Flag=1;
					break;
				case 8:
					if(g_TamperSet_Flag)g_TamperSet_Flag=0;
					break;
				case 12:   //确认
					cotMenu_changeMenuState(TamperSetConfirm);
					break;
				case 10:  //取消
					cotMenu_Exit();	
					break;
				default:
					break;
			}
			break;
		case TamperSetConfirm:
			PLAY_MUSIC(7);
			switch(keyVal)
			{
				case 4:  //确认
					g_Yes_Or_No=1;
					break;
				case 6:  //否
					g_Yes_Or_No=0;
					break;
				case 12:
					if(g_Yes_Or_No)
					{
						g_Yes_Or_No=0;
						
						if(g_TamperSet_Flag){
							g_Force_Disable_Flag=0;
							g_System_Para_Info.tamper_enable=1;
							System_Para_Save();
						}
						else
						{
							g_Force_Disable_Flag=1;
							g_System_Para_Info.tamper_enable=0;
							System_Para_Save();
						}
						cotMenu_changeMenuState(TamperSetSuccess);
					}
					else
					{
						g_Yes_Or_No=0;
						cotMenu_Exit();
					}
					break;
				case 10:
					cotMenu_Exit();	
					break;
				default:
					break;
			}
			break;
		case TamperSetSuccess: 
			break;
		case TamperSetFail: 
			break;
			default:	
			break;					
	}
}

void TamperSet_onTick(uint8_t menuState)
{
		if(g_Wake_Time_Count>SYSTEM_DELAY_TIME)
		{
				FNS_ALL_FORM_CLOSE();
				g_Wake_Time_Count=0;
				fns_Form_redraw();
		}
		switch(menuState)
		{
			case TamperSet:
			case TamperSetConfirm:
				break;
			case TamperSetSuccess:
			case TamperSetFail:

					cotMenu_Exit();

				break;
			default :
				break;
		}
}


void TamperSet_enter(void)
{
	g_Yes_Or_No=0;
	g_SystemSet_Pri_Display=0;
	TamperSetStatus=TamperSet;
		
	if(g_System_Para_Info.tamper_enable)
	g_TamperSet_Flag=1; 
	else	g_TamperSet_Flag=0; 
	g_Wake_Time_Count=0;

}

void TamperSet_load(uint8_t menuState)
{
	switch(menuState)
	{
		case TamperSet:
		case TamperSetConfirm:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 20, 0);
			break;
		case TamperSetSuccess:
		case TamperSetFail:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 200, 1);
			break;
		default :
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 20, 0);
			break;
	}


}

void TamperSet_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo)
{
	uitn8_t keyVal;
	TamperSet_onChar(menuState, keyVal);
	if(!cotMenu_getTick())
	{
		TamperSet_onTick(menuState);
	}
}

/****************************语言设置*******************************************/

void LanguageSet_enter(void)
{

}
void LanguageSet_exit(void)
{

}
void LanguageSet_load(uint8_t menuState)
{

}
void LanguageSet_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo)
{

}


/******************************恢复出厂*****************************************/

unsigned char g_progress_bar_value=0;
RestoreStateDef RestoreSetStatus;

void ResetDefaults_onShow(MenuShow_s *pstShowInfo,uint8_t menuState)
{
		unsigned char x,y;
		CDC* pFormCDC;
		switch(menuState)
		{
			case RestoreSetConfirm:
				y=1;
				x=4;
				Dir4_Logo_Icon(pFormCDC,x,y);
				x=12;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
				x=108;
				Dir6_Logo_Icon(pFormCDC,x,y);
				y=24;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"确定要初始化吗\?",1,y,FONT_16X16);
				y=47;
				x=18;
				if(g_Yes_Or_No) OLED_YesOrNO(pFormCDC,x,y,1);
				else OLED_YesOrNO(pFormCDC,x,y,0);
				break;
			case RestoreSetBar:
				x=14;
				y=20;
				if(g_progress_bar_value<100) progress_bar(pFormCDC,g_progress_bar_value,x,y);
				else progress_bar(pFormCDC,100,x,y);
				break;
			case RestoreSetSuccess: 
				y=1;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"初始化成功",1,y,FONT_16X16);
				break;
			case RestoreSetFail:
				System_SetFail_Tip(pFormCDC);
				break;
		}
}


void ResetDefaults_onChar(uint8_t menuState, uint8_t keyVal)
{
	
	switch(menuState)
	{
		case RestoreSetConfirm:
			PLAY_MUSIC(7);
			switch(keyVal)
			{
				case 4:  //确认
					g_Yes_Or_No=1;
					break;
				case 6:  //否
					g_Yes_Or_No=0;
					break;
				case 12:
					if(g_Yes_Or_No)
					{
						g_Yes_Or_No=0;
						SYSTEM2FACTORY();           //系统设置参数恢复
						CLEAN_USER_ALL();						//用户清空
						RestoreSetStatus=RestoreSetBar;																		
					}
					else{
						g_Yes_Or_No=0;
						cotMenu_Exit();	
					}
					break;
				case 10:
					cotMenu_Exit();	
					break;
				default:
					break;
			}
			break;
		case RestoreSetBar:
			break;
		case RestoreSetSuccess:
			break;
		case	RestoreSetFail:
			break;
	}
}


void ResetDefaults_onTick(uint8_t menuState)
{
	if(g_Wake_Time_Count>SYSTEM_DELAY_TIME)
	{
		FNS_ALL_FORM_CLOSE();
		g_Wake_Time_Count=0;
	}
	
	switch(menuState)
	{
		case RestoreSetConfirm:
			break;
		case RestoreSetBar:
			g_progress_bar_value+=2;
			g_Wake_Time_Count=0;
			if(g_progress_bar_value>120)
			{
				RestoreSetStatus=RestoreSetSuccess;													
			}
			break;
		case RestoreSetSuccess:
		case RestoreSetFail:
			cotMenu_Exit();
			break;
		default :
			break;
	}
}


void ResetDefaults_enter(void)
{
	g_Yes_Or_No=0;
	g_SystemSet_Pri_Display=0;
	RestoreSetStatus= RestoreSetConfirm;
	g_progress_bar_value=0;
	g_Wake_Time_Count=0;
}

void ResetDefaults_load(uint8_t menuState)
{
	switch(menuState)
	{
		case RestoreSetConfirm:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 20, 0);
			break;
		case RestoreSetBar:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 20, 0);
			PLAY_MUSIC(HuiFuChuChangSheZhi);
			g_progress_bar_value=10;
			break;
		case RestoreSetSuccess:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 200, 1);
			PLAY_MUSIC(CaoZuoChengGong);
			break;
		case RestoreSetFail:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 200, 1);
			break;
		default :
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 20, 0);
			break;
	}
}

void ResetDefaults_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo)
{
	uitn8_t keyVal;
	ResetDefaults_onChar(menuState, keyVal);
	if(!cotMenu_getTick())
	{
		ResetDefaults_onTick(menuState);
	}
}

/*****************************配网设置******************************************/

//void NetworkSet_onShow(MenuShow_s *pstShowInfo,uint8_t menuState);
//void NetworkSet_onChar(uint8_t menuState, uint8_t keyVal);
//void NetworkSet_onTick(BASEFORM* pThis);
//void NetworkSet_onCreate(BASEFORM* pThis);
//
//const FORM_HANDLERS s_formcode_NetworkSet={
//		sizeof(MENUFORM),
//		NetworkSetStr,
//		fns_NullForm_onDefault,
//		NetworkSet_onShow,
//		NetworkSet_onChar,
//		NetworkSet_onTick,
//		NetworkSet_onCreate,
//		fns_NullForm_onDestroy,
//		fns_NullForm_onFocus	
//};
//void NetworkSet(MENUFORM* pMenuForm)
//{
//		fns_Form_create(&s_formcode_NetworkSet);
//}
//
//
//void NetworkSet_onShow(MenuShow_s *pstShowInfo,uint8_t menuState)
//{
//
//}
//
//void NetworkSet_onChar(uint8_t menuState, uint8_t keyVal)
//{
//
//
//}
//
//void NetworkSet_onTick(uint8_t menuState)
//{
//		pThis->timeTick=2;
//		if(g_Wake_Time_Count>SYSTEM_DELAY_TIME){
//					FNS_ALL_FORM_CLOSE();
//					g_Wake_Time_Count=0;
//					fns_Form_redraw();
//		}
//}
//
//
//
//void NetworkSet_onCreate(BASEFORM* pThis)
//{
//		pThis->timeTick=2;
//		g_Yes_Or_No=0;
//		g_SystemSet_Pri_Display=0;
//
//}



#if 0
		
/***********************************************************************/		
uint8_t GuestAuthorityStr[]={"宾客权限"};

void GuestAuthority_onShow(MenuShow_s *pstShowInfo,uint8_t menuState);
void GuestAuthority_onChar(uint8_t menuState, uint8_t keyVal);
void GuestAuthority_onTick(BASEFORM* pThis);
void GuestAuthority_onCreate(BASEFORM* pThis);

const FORM_HANDLERS s_formcode_GuestAuthoritySet={
		sizeof(MENUFORM),
		GuestAuthorityStr,
		fns_NullForm_onDefault,
		GuestAuthority_onShow,
		GuestAuthority_onChar,
		GuestAuthority_onTick,
		GuestAuthority_onCreate,
		fns_NullForm_onDestroy,
		fns_NullForm_onFocus	
};
void GuestAuthority(MENUFORM* pMenuForm)
{
		fns_Form_create(&s_formcode_GuestAuthoritySet);
}


void GuestAuthority_onShow(MenuShow_s *pstShowInfo,uint8_t menuState)
{

}

void GuestAuthority_onChar(uint8_t menuState, uint8_t keyVal)
{


}

void GuestAuthority_onTick(BASEFORM* pThis)
{
	pThis->timeTick=2;
if(g_Wake_Time_Count>SYSTEM_DELAY_TIME){
					FNS_ALL_FORM_CLOSE();
					g_Wake_Time_Count=0;
					fns_Form_redraw();
		}
}

void GuestAuthority_onCreate(BASEFORM* pThis)
{

}


/***********************************************************************/



uint8_t ButtonSoundSetStr[]={"按键音效"};

void ButtonSoundSet_onShow(MenuShow_s *pstShowInfo,uint8_t menuState);
void ButtonSoundSet_onChar(uint8_t menuState, uint8_t keyVal);
void ButtonSoundSet_onTick(BASEFORM* pThis);
void ButtonSoundSet_onCreate(BASEFORM* pThis);

const FORM_HANDLERS s_formcode_ButtonSoundSet={
		sizeof(MENUFORM),
		ButtonSoundSetStr,
		fns_NullForm_onDefault,
		ButtonSoundSet_onShow,
		ButtonSoundSet_onChar,
		ButtonSoundSet_onTick,
		ButtonSoundSet_onCreate,
		fns_NullForm_onDestroy,
		fns_NullForm_onFocus	
};
void ButtonSound(MENUFORM* pMenuForm)
{
		fns_Form_create(&s_formcode_ButtonSoundSet);
}


void ButtonSoundSet_onShow(MenuShow_s *pstShowInfo,uint8_t menuState)
{

}

void ButtonSoundSet_onChar(uint8_t menuState, uint8_t keyVal)
{


}

void ButtonSoundSet_onTick(BASEFORM* pThis)
{
		pThis->timeTick=2;
		if(g_Wake_Time_Count>SYSTEM_DELAY_TIME){
					FNS_ALL_FORM_CLOSE();
					g_Wake_Time_Count=0;
					fns_Form_redraw();
		}
}



void ButtonSoundSet_onCreate(BASEFORM* pThis)
{

}


#endif






/***********************************************************************/

#if 0


/***********************************************************************/


uint8_t RemoteUnlockStr[]={"远程开锁"};

void RemoteUnlock_onShow(MenuShow_s *pstShowInfo,uint8_t menuState);
void RemoteUnlock_onChar(uint8_t menuState, uint8_t keyVal);
void RemoteUnlock_onTick(BASEFORM* pThis);
void RemoteUnlock_onCreate(BASEFORM* pThis);

const FORM_HANDLERS s_formcode_RemoteUnlock={
		sizeof(MENUFORM),
		RemoteUnlockStr,
		fns_NullForm_onDefault,
		RemoteUnlock_onShow,
		RemoteUnlock_onChar,
		RemoteUnlock_onTick,
		RemoteUnlock_onCreate,
		fns_NullForm_onDestroy,
		fns_NullForm_onFocus	
};
void RemoteUnlock(MENUFORM* pMenuForm)
{
		fns_Form_create(&s_formcode_RemoteUnlock);
}


void RemoteUnlock_onShow(MenuShow_s *pstShowInfo,uint8_t menuState)
{

}

void RemoteUnlock_onChar(uint8_t menuState, uint8_t keyVal)
{


}

void RemoteUnlock_onTick(BASEFORM* pThis)
{
	pThis->timeTick=2;
if(g_Wake_Time_Count>SYSTEM_DELAY_TIME){
					FNS_ALL_FORM_CLOSE();
					g_Wake_Time_Count=0;
					fns_Form_redraw();
		}
}



void RemoteUnlock_onCreate(BASEFORM* pThis)
{
		pThis->timeTick=2;
		g_Yes_Or_No=0;
		g_SystemSet_Pri_Display=0;

}
#endif
/***********************************************************************/

uint8_t SoftVersionInfoStr[]={"版本号"};

void SoftVersionInfo_onShow(MenuShow_s *pstShowInfo,uint8_t menuState);
void SoftVersionInfo_onChar(uint8_t menuState, uint8_t keyVal);
void SoftVersionInfo_onTick(BASEFORM* pThis);
void SoftVersionInfo_onCreate(BASEFORM* pThis);

const FORM_HANDLERS s_formcode_SoftVersionInfo={
		sizeof(MENUFORM),
		SoftVersionInfoStr,
		fns_NullForm_onDefault,
		SoftVersionInfo_onShow,
		SoftVersionInfo_onChar,
		SoftVersionInfo_onTick,
		SoftVersionInfo_onCreate,
		fns_NullForm_onDestroy,
		fns_NullForm_onFocus	
};


void SoftVersionInfo_onShow(MenuShow_s *pstShowInfo,uint8_t menuState)
{
		unsigned char x,y;
		if(pThis->handlers->titleId)
		{
				y=0;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,pThis->handlers->titleId,1,y,FONT_16X16);
		}
		x=4;
		y+=16;
		OLED_ShowMixStr(pFormCDC,x,y,g_System_Para_Info.soft_version,1,1,FONT_16X16);
		y+=16;

		OLED_HZ_ShowMixStr_XCentre(pFormCDC,g_System_Para_Info.soft_date,1,y,FONT_16X16);
		y+=16;
		OLED_HZ_ShowMixStr_XCentre(pFormCDC,g_System_Para_Info.soft_time,1,y,FONT_16X16);
}

void SoftVersionInfo_onChar(uint8_t menuState, uint8_t keyVal)
{
		PLAY_MUSIC(7);
		switch(keyVal){
			case 12:
				break;
			case 10:
				cotMenu_Exit();	
				fns_Form_redraw(); 
				break;
			default:
				break;
		}
}

void SoftVersionInfo_onTick(BASEFORM* pThis)
{
		pThis->timeTick=2;
		if(g_Wake_Time_Count>SYSTEM_DELAY_TIME){
					FNS_ALL_FORM_CLOSE();
					g_Wake_Time_Count=0;
					fns_Form_redraw();
		}
}



void SoftVersionInfo_onCreate(BASEFORM* pThis)
{
		pThis->timeTick=2;
		g_Yes_Or_No=0;
		g_SystemSet_Pri_Display=0;
		g_Wake_Time_Count=0;
}

/**************************************************************/


//uint8_t MaintenanceModeStr[] ={"维护模式"};

//void MaintenanceMode_onCreate(BASEFORM* pThis);
//void MaintenanceMode_onShow(MenuShow_s *pstShowInfo,uint8_t menuState);
//void MaintenanceMode_onChar(uint8_t menuState, uint8_t keyVal);
//void MaintenanceMode_onTick(BASEFORM* pThis);

//const FORM_HANDLERS s_formcode_MaintenanceModeSet={
//		sizeof(MENUFORM),
//		MaintenanceModeStr,
//    fns_NullForm_onDefault,
//    MaintenanceMode_onShow,
//    MaintenanceMode_onChar,
//    MaintenanceMode_onTick,
//		MaintenanceMode_onCreate,
//    fns_NullForm_onDestroy,
//    fns_NullForm_onFocus	
//};

//void frmMaintenanceMode_Menu(void)
//{
//	fns_Form_create(&s_formcode_MaintenanceModeSet);
//}


//void MaintenanceMode_onShow(MenuShow_s *pstShowInfo,uint8_t menuState){
//    uint8_t x,y;
//		if(pThis->handlers->titleId)
//		{
//				y=1;
//				x=4;
//				Dir2_Logo_Icon(pFormCDC,x,y);
//				x=108;
//				Dir8_Logo_Icon(pFormCDC,x,y);
//				OLED_HZ_ShowMixStr_XCentre(pFormCDC,pThis->handlers->titleId,1,y,FONT_16X16);
//		}
//    fns_MenuForm_onShow(pThis,pFormCDC);
//}

//void MaintenanceMode_onChar(uint8_t menuState, uint8_t keyVal){
//    fns_MenuForm_onChar(pThis,keyVal);
//}

//void MaintenanceMode_onTick(BASEFORM* pThis){
//    pThis->timeTick=2;
//	if(g_Wake_Time_Count>20){
//			FNS_ALL_FORM_CLOSE();
//			g_Wake_Time_Count=0;
//			fns_Form_redraw();
//		}
//    fns_Form_redraw();
//}


//void SafetyLevel(MENUFORM* pMenuForm);
//void ICCardCheck(MENUFORM* pMenuForm);
//void ICEncryption(MENUFORM* pMenuForm);
//void SysLogoSwitch(MENUFORM* pMenuForm);

//const MENUITEM s_MaintenanceModeMenu[]={
////		{(uint8_t*)"安全等级",SafetyLevel,NULL}, 
////		{(uint8_t*)"寻卡模式",ICCardCheck,NULL}, 
////		{(uint8_t*)"卡片加密",ICEncryption,NULL},
////		{(uint8_t*)"开机LOGO",SysLogoSwitch,NULL},

//};

//void MaintenanceMode_onCreate(BASEFORM* pThis){   
//    MENUFORM* pMenuForm=(MENUFORM*)pThis;
//    pMenuForm->menuItems=s_MaintenanceModeMenu;
//    pMenuForm->itemCount=sizeof(s_MaintenanceModeMenu)/sizeof(MENUITEM);
//    pMenuForm->firstIndex=0;
//		pMenuForm->curIndex=0;
//    pMenuForm->timeTick=2;
//	g_Wake_Time_Count=0;
//}

/*********************************************************/

//uint8_t SafetyLevelSetStr[]={"指纹安全等级"};
//static unsigned char g_SafetyLevelSet_Flag;
//SafetyLevelSetDef SafetyLevelStatus;

//void SafetyLevelSet_onShow(MenuShow_s *pstShowInfo,uint8_t menuState);
//void SafetyLevelSet_onChar(uint8_t menuState, uint8_t keyVal);
//void SafetyLevelSet_onTick(BASEFORM* pThis);
//void SafetyLevelSet_onCreate(BASEFORM* pThis);

//const FORM_HANDLERS s_formcode_SafetyLevelSet={
//		sizeof(MENUFORM),
//		SafetyLevelSetStr,
//		fns_NullForm_onDefault,
//		SafetyLevelSet_onShow,
//		SafetyLevelSet_onChar,
//		SafetyLevelSet_onTick,
//		SafetyLevelSet_onCreate,
//		fns_NullForm_onDestroy,
//		fns_NullForm_onFocus	
//};

//void SafetyLevel(MENUFORM* pMenuForm)
//{
//fns_Form_create(&s_formcode_SafetyLevelSet);
//}


//void SafetyLevelSet_onShow(MenuShow_s *pstShowInfo,uint8_t menuState)
//{
//	
//	unsigned char x,y;
//	
//	switch(SafetyLevelStatus)
//	{
//		case SafetyLevelSet:
//			
//			if(pThis->handlers->titleId){
//					y=1;
//					OLED_HZ_ShowMixStr_XCentre(pFormCDC,pThis->handlers->titleId,1,y,FONT_16X16);
//			}
//			y=28;
//			if(0==g_SafetyLevelSet_Flag){
//					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"等级低",0,y,FONT_24X22);
//			}
//			else if(1==g_SafetyLevelSet_Flag){
//					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"等级中",0,y,FONT_24X22);
//			}
//			else{
//					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"等级高",0,y,FONT_24X22);
//			}
//			break;
//		case	SafetyLevelSetConfirm:
//			System_UpdateConfirm_Tip(pFormCDC,g_Yes_Or_No);
//			break;
//		case SafetyLevelSetSuccess:
//			System_SetSuccess_Tip(pFormCDC);
//			break;	
//		case SafetyLevelSetFail:  
//			System_SetFail_Tip(pFormCDC);
//			break;
//	}
//}

//void SafetyLevelSet_onChar(uint8_t menuState, uint8_t keyVal)
//{
//		g_Wake_Time_Count=0;
//		switch(SafetyLevelStatus){
//			case SafetyLevelSet:
//				PLAY_MUSIC(7);
//				switch(keyVal)
//				{
//					case 2:
//						if(g_SafetyLevelSet_Flag<2)g_SafetyLevelSet_Flag++;
//						fns_Form_redraw(); 
//						break;
//					case 8:
//						if(g_SafetyLevelSet_Flag)g_SafetyLevelSet_Flag--;
//						fns_Form_redraw(); 
//						break;
//					case 12:   //确认
//						SafetyLevelStatus=SafetyLevelSetConfirm;
//						fns_Form_redraw(); 
//						break;
//					case 10:  //取消
//						cotMenu_Exit();	
//						fns_Form_redraw(); 
//						break;
//					default:
//						break;
//				}
//				break;
//			case SafetyLevelSetConfirm:
//				PLAY_MUSIC(7);
//				switch(keyVal){
//					case 4:  //确认
//						g_Yes_Or_No=1;
//						fns_Form_redraw();
//						break;
//					case 6:  //否
//						g_Yes_Or_No=0;
//						fns_Form_redraw();
//						break;
//					case 12:
//						if(g_Yes_Or_No){
//							g_Yes_Or_No=0;
//							if(g_SafetyLevelSet_Flag){
//									g_System_Para_Info.safetylevel=1;
//									System_Para_Save();
//							}
//							else{
//									g_System_Para_Info.safetylevel=0;
//									System_Para_Save();
//							}
//							SafetyLevelStatus=SafetyLevelSetSuccess;
//							fns_Form_redraw();
//						}
//						else{
//							g_Yes_Or_No=0;
//							cotMenu_Exit();	
//							fns_Form_redraw(); 
//						}
//						break;
//					case 10:
//						cotMenu_Exit();	
//						break;
//					default:
//						break;
//				}
//				break;
//			case SafetyLevelSetSuccess: 
//				break;
//			case SafetyLevelSetFail: 
//				break;
//				default:	
//				break;					
//		}
//}

//void SafetyLevelSet_onTick(BASEFORM* pThis)
//{
//	pThis->timeTick=2;
//		if(g_Wake_Time_Count>20){
//					FNS_ALL_FORM_CLOSE();
//					g_Wake_Time_Count=0;
//					fns_Form_redraw();
//		}

//		switch(SafetyLevelStatus){
//			case SafetyLevelSet:
//					fns_Form_redraw();
//				break;
//			case SafetyLevelSetConfirm:
//					fns_Form_redraw();
//				break;
//			case SafetyLevelSetSuccess:
//					if(	g_SystemSet_Pri_Display!=SafetyLevelSetSuccess){
//					g_SystemSet_Pri_Display=SafetyLevelSetSuccess;
//					pThis->timeTick=20;//2秒
//					fns_Form_redraw();
//				}
//			else{
//					cotMenu_Exit();
//					fns_Form_redraw();
//				}
//				break;
//			case SafetyLevelSetFail:
//				if(	g_SystemSet_Pri_Display!=SafetyLevelSetFail){
//					g_SystemSet_Pri_Display=SafetyLevelSetFail;
//					pThis->timeTick=20;//2秒
//					fns_Form_redraw();
//				}
//			else{
//					cotMenu_Exit();
//					fns_Form_redraw();
//				}
//				break;
//			default :
//				break;
//		}
//		
//}



//void SafetyLevelSet_onCreate(BASEFORM* pThis)
//{
//		if(g_System_Para_Info.safetylevel)
//			g_SafetyLevelSet_Flag=1;
//		else
//			g_SafetyLevelSet_Flag=0;
//		pThis->timeTick=2;
//		g_Yes_Or_No=0;
//		g_SystemSet_Pri_Display=0;
//		SafetyLevelStatus= SafetyLevelSet;
//	
//}

/***********************************************************/

//uint8_t ICCheckSetStr[]={"寻卡模式"};
//static unsigned char g_ICCheckSet_Flag;
//ICCheckSetDef ICCheckSetStatus;

//void ICCheckSet_onShow(MenuShow_s *pstShowInfo,uint8_t menuState);
//void ICCheckSet_onChar(uint8_t menuState, uint8_t keyVal);
//void ICCheckSet_onTick(BASEFORM* pThis);
//void ICCheckSet_onCreate(BASEFORM* pThis);

//const FORM_HANDLERS s_formcode_ICCheckSet={
//		sizeof(MENUFORM),
//		ICCheckSetStr,
//		fns_NullForm_onDefault,
//		ICCheckSet_onShow,
//		ICCheckSet_onChar,
//		ICCheckSet_onTick,
//		ICCheckSet_onCreate,
//		fns_NullForm_onDestroy,
//		fns_NullForm_onFocus	
//};

//void ICCardCheck(MENUFORM* pMenuForm)
//{
//		fns_Form_create(&s_formcode_ICCheckSet);
//}

//void ICCheckSet_onShow(MenuShow_s *pstShowInfo,uint8_t menuState)
//{
//	
//	unsigned char x,y;
//	
//	switch(ICCheckSetStatus)
//	{
//		case ICCheckSet:
//			
//			if(pThis->handlers->titleId){
//					y=1;
//					OLED_HZ_ShowMixStr_XCentre(pFormCDC,pThis->handlers->titleId,1,y,FONT_16X16);
//			}
//			y=28;
//			if(0==g_ICCheckSet_Flag){
//					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"关闭寻卡",0,y,FONT_24X22);
//			}
//			else if(1==g_ICCheckSet_Flag){
//					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"场强侦测",0,y,FONT_24X22);
//			}
//			else{
//					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"普通寻卡",0,y,FONT_24X22);
//			}
//			break;
//		case	ICCheckSetConfirm:
//			System_UpdateConfirm_Tip(pFormCDC,g_Yes_Or_No);
//			break;
//		case ICCheckSetSuccess:
//			System_SetSuccess_Tip(pFormCDC);
//			break;	
//		case ICCheckSetFail:  
//			System_SetFail_Tip(pFormCDC);
//			break;
//	}
//}

//void ICCheckSet_onChar(uint8_t menuState, uint8_t keyVal)
//{
//		g_Wake_Time_Count=0;
//		switch(ICCheckSetStatus)
//		{
//			case ICCheckSet:
//				PLAY_MUSIC(7);
//				switch(keyVal)
//				{

//						
//					case 2:
//						if(g_ICCheckSet_Flag<2)g_ICCheckSet_Flag++;
//						fns_Form_redraw(); 
//						break;
//					case 8:
//						if(g_ICCheckSet_Flag)g_ICCheckSet_Flag--;
//						fns_Form_redraw(); 
//						break;
//					case 12:   //确认
//						ICCheckSetStatus=ICCheckSetConfirm;
//						fns_Form_redraw(); 
//						break;
//					case 10:  //取消
//						cotMenu_Exit();	
//						fns_Form_redraw(); 
//						break;
//					default:
//						break;
//				}
//				break;
//			case ICCheckSetConfirm:
//				PLAY_MUSIC(7);
//				switch(keyVal){
//					case 4:  //确认
//						g_Yes_Or_No=1;
//						fns_Form_redraw();
//						break;
//					case 6:  //否
//						g_Yes_Or_No=0;
//						fns_Form_redraw();
//						break;
//					case 12:
//						if(g_Yes_Or_No){
//							g_Yes_Or_No=0;
//							if(g_ICCheckSet_Flag){
//									g_System_Para_Info.ic_check_enable=1;
//									System_Para_Save();
//							}
//							else{
//									g_System_Para_Info.ic_check_enable=0;
//									System_Para_Save();
//							}
//							ICCheckSetStatus=ICCheckSetSuccess;
//							fns_Form_redraw();
//						}
//						else{
//							g_Yes_Or_No=0;
//							cotMenu_Exit();	
//							fns_Form_redraw(); 
//						}
//						break;
//					case 10:
//						cotMenu_Exit();	
//						break;
//					default:
//						break;
//				}
//				break;
//			case ICCheckSetSuccess: 
//				break;
//			case ICCheckSetFail: 
//				break;
//				default:	
//				break;					
//		}
//}

//void ICCheckSet_onTick(BASEFORM* pThis)
//{
//	pThis->timeTick=2;
//		if(g_Wake_Time_Count>SYSTEM_DELAY_TIME){
//					FNS_ALL_FORM_CLOSE();
//					g_Wake_Time_Count=0;
//					fns_Form_redraw();
//		}

//		switch(ICCheckSetStatus)
//		{
//			case ICCheckSet:
//					fns_Form_redraw();
//				break;
//			case ICCheckSetConfirm:
//					fns_Form_redraw();
//				break;
//			case ICCheckSetSuccess:
//					if(	g_SystemSet_Pri_Display!=ICCheckSetSuccess){
//					g_SystemSet_Pri_Display=ICCheckSetSuccess;
//					pThis->timeTick=20;//2秒
//					fns_Form_redraw();
//				}
//			else{
//					cotMenu_Exit();
//					fns_Form_redraw();
//				}
//				break;
//			case ICCheckSetFail:
//				if(	g_SystemSet_Pri_Display!=ICCheckSetFail){
//					g_SystemSet_Pri_Display=ICCheckSetFail;
//					pThis->timeTick=20;//2秒
//					fns_Form_redraw();
//				}
//			else{
//					cotMenu_Exit();
//					fns_Form_redraw();
//				}
//				break;
//			default :
//				break;
//		}
//		
//}



//void ICCheckSet_onCreate(BASEFORM* pThis)
//{
//		if(g_System_Para_Info.ic_check_enable)
//			g_ICCheckSet_Flag=1;
//		else
//			g_ICCheckSet_Flag=0;
//		pThis->timeTick=2;
//		g_Yes_Or_No=0;
//		g_SystemSet_Pri_Display=0;
//		ICCheckSetStatus= ICCheckSet;
//		g_Wake_Time_Count=0;
//	
//}
/********************************************************/


//uint8_t ICEncryptionSetStr[]={"卡片加密设置"};
//static unsigned char g_ICEncryptionSet_Flag;
//ICEncryptionSetDef ICEncryptionSetStatus;

//void ICEncryptionSet_onShow(MenuShow_s *pstShowInfo,uint8_t menuState);
//void ICEncryptionSet_onChar(uint8_t menuState, uint8_t keyVal);
//void ICEncryptionSet_onTick(BASEFORM* pThis);
//void ICEncryptionSet_onCreate(BASEFORM* pThis);

//const FORM_HANDLERS s_formcode_ICEncryptionSet={
//		sizeof(MENUFORM),
//		ICEncryptionSetStr,
//		fns_NullForm_onDefault,
//		ICEncryptionSet_onShow,
//		ICEncryptionSet_onChar,
//		ICEncryptionSet_onTick,
//		ICEncryptionSet_onCreate,
//		fns_NullForm_onDestroy,
//		fns_NullForm_onFocus	
//};

//void ICEncryption(MENUFORM* pMenuForm)
//{
//		fns_Form_create(&s_formcode_ICEncryptionSet);
//}

//void ICEncryptionSet_onShow(MenuShow_s *pstShowInfo,uint8_t menuState)
//{
//	
//	unsigned char x,y;
//	
//	switch(ICEncryptionSetStatus)
//	{
//		case ICCheckSet:
//			
//			if(pThis->handlers->titleId){
//					y=1;
//					OLED_HZ_ShowMixStr_XCentre(pFormCDC,pThis->handlers->titleId,1,y,FONT_16X16);
//			}
//			y=28;
//			if(g_ICEncryptionSet_Flag){
//					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"打开加密",0,y,FONT_24X22);
//			}
//			else{
//					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"关闭加密",0,y,FONT_24X22);
//			}
//			break;
//		case	ICEncryptionSetConfirm:
//			System_UpdateConfirm_Tip(pFormCDC,g_Yes_Or_No);
//			break;
//		case ICEncryptionSetSuccess:
//			System_SetSuccess_Tip(pFormCDC);
//			break;	
//		case ICEncryptionSetFail:  
//			System_SetFail_Tip(pFormCDC);
//			break;
//	}
//}

//void ICEncryptionSet_onChar(uint8_t menuState, uint8_t keyVal)
//{
//		g_Wake_Time_Count=0;
//		switch(ICEncryptionSetStatus)
//		{
//			case ICEncryptionSet:
//				PLAY_MUSIC(7);
//				switch(keyVal)
//				{
//					case 2:
//						if(!g_ICEncryptionSet_Flag)g_ICEncryptionSet_Flag=1;
//						break;

//					case 8:
//						if(g_ICEncryptionSet_Flag)g_ICEncryptionSet_Flag=0;
//						break;
//					case 12:   //确认
//						ICEncryptionSetStatus=ICEncryptionSetConfirm;
//						fns_Form_redraw(); 
//						break;
//					case 10:  //取消
//						cotMenu_Exit();	
//						fns_Form_redraw(); 
//						break;
//					default:
//						break;
//				}
//				break;
//			case ICEncryptionSetConfirm:
//				PLAY_MUSIC(7);
//				switch(keyVal){
//					case 4:  //确认
//						g_Yes_Or_No=1;
//						fns_Form_redraw();
//						break;
//					case 6:  //否
//						g_Yes_Or_No=0;
//						fns_Form_redraw();
//						break;
//					case 12:
//						if(g_Yes_Or_No){
//							g_Yes_Or_No=0;
//							if(g_ICEncryptionSet_Flag){
//									g_System_Para_Info.ic_encryption_enable=1;
//									System_Para_Save();
//							}
//							else{
//									g_System_Para_Info.ic_encryption_enable=0;
//									System_Para_Save();
//							}
//							ICEncryptionSetStatus=ICEncryptionSetSuccess;
//							fns_Form_redraw();
//						}
//						else{
//							g_Yes_Or_No=0;
//							cotMenu_Exit();	
//							fns_Form_redraw(); 
//						}
//						break;
//					case 10:
//						cotMenu_Exit();	
//						break;
//					default:
//						break;
//				}
//				break;
//			case ICEncryptionSetSuccess: 
//				break;
//			case ICEncryptionSetFail: 
//				break;
//				default:	
//				break;					
//		}
//}

//void ICEncryptionSet_onTick(BASEFORM* pThis)
//{
//	pThis->timeTick=2;
//		if(g_Wake_Time_Count>SYSTEM_DELAY_TIME){
//					FNS_ALL_FORM_CLOSE();
//					g_Wake_Time_Count=0;
//					fns_Form_redraw();
//		}

//		switch(ICEncryptionSetStatus)
//		{
//			case ICEncryptionSet:
//					fns_Form_redraw();
//				break;
//			case ICEncryptionSetConfirm:
//					fns_Form_redraw();
//				break;
//			case ICEncryptionSetSuccess:
//					if(	g_SystemSet_Pri_Display!=ICEncryptionSetSuccess){
//					g_SystemSet_Pri_Display=ICEncryptionSetSuccess;
//					pThis->timeTick=20;//2秒
//					fns_Form_redraw();
//				}
//			else{
//					cotMenu_Exit();
//					fns_Form_redraw();
//				}
//				break;
//			case ICEncryptionSetFail:
//				if(	g_SystemSet_Pri_Display!=ICEncryptionSetFail){
//					g_SystemSet_Pri_Display=ICEncryptionSetFail;
//					pThis->timeTick=20;//2秒
//					fns_Form_redraw();
//				}
//			else{
//					cotMenu_Exit();
//					fns_Form_redraw();
//				}
//				break;
//			default :
//				break;
//		}
//		
//}



//void ICEncryptionSet_onCreate(BASEFORM* pThis)
//{
//		if(g_System_Para_Info.ic_encryption_enable)
//			g_ICEncryptionSet_Flag=1;
//		else
//			g_ICEncryptionSet_Flag=0;
//		pThis->timeTick=2;
//		g_Yes_Or_No=0;
//		g_SystemSet_Pri_Display=0;
//		ICEncryptionSetStatus= ICEncryptionSet;
//	
//}

/******************************************************/

//uint8_t SysLogoSetStr[]={"开机LOGO显示"};
//static unsigned char g_SysLogoSet_Flag;
//SysLogoSetDef SysLogoSetStatus;

//void SysLogoSet_onShow(MenuShow_s *pstShowInfo,uint8_t menuState);
//void SysLogoSet_onChar(uint8_t menuState, uint8_t keyVal);
//void SysLogoSet_onTick(BASEFORM* pThis);
//void SysLogoSet_onCreate(BASEFORM* pThis);

//const FORM_HANDLERS s_formcode_SysLogoSet={
//		sizeof(MENUFORM),
//		SysLogoSetStr,
//		fns_NullForm_onDefault,
//		SysLogoSet_onShow,
//		SysLogoSet_onChar,
//		SysLogoSet_onTick,
//		SysLogoSet_onCreate,
//		fns_NullForm_onDestroy,
//		fns_NullForm_onFocus	
//};



//void SysLogoSwitch(MENUFORM* pMenuForm)
//{
//		fns_Form_create(&s_formcode_SysLogoSet);

//}


//void SysLogoSet_onShow(MenuShow_s *pstShowInfo,uint8_t menuState)
//{
//	
//	unsigned char x,y;
//	
//	switch(SysLogoSetStatus)
//	{
//		case SysLogoSet:
//			
//			if(pThis->handlers->titleId){
//					y=1;
//					OLED_HZ_ShowMixStr_XCentre(pFormCDC,pThis->handlers->titleId,1,y,FONT_16X16);
//			}
//			y=28;
//			if(g_SysLogoSet_Flag){
//					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"开启LOGO",0,y,FONT_24X22);
//			}
//			else{
//					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"关闭LOGO",0,y,FONT_24X22);
//			}
//			break;
//		case	SysLogoSetConfirm:
//			System_UpdateConfirm_Tip(pFormCDC,g_Yes_Or_No);
//			break;
//		case SysLogoSetSuccess:
//			System_SetSuccess_Tip(pFormCDC);
//			break;	
//		case SysLogoSetFail:  
//			System_SetFail_Tip(pFormCDC);
//			break;
//	}
//}

//void SysLogoSet_onChar(uint8_t menuState, uint8_t keyVal)
//{
//		g_Wake_Time_Count=0;
//		switch(SysLogoSetStatus){
//			case SysLogoSet:
//				PLAY_MUSIC(7);
//				switch(keyVal){
//					case 2:
//						if(!g_SysLogoSet_Flag)g_SysLogoSet_Flag=1;
//						break;

//					case 8:
//						if(g_SysLogoSet_Flag)g_SysLogoSet_Flag=0;
//						break;
//					case 12:   //确认
//						SysLogoSetStatus=SysLogoSetConfirm;
//						fns_Form_redraw(); 
//						break;
//					case 10:  //取消
//						cotMenu_Exit();	
//						fns_Form_redraw(); 
//						break;
//					default:
//						break;
//				}
//				break;
//			case SysLogoSetConfirm:
//				PLAY_MUSIC(7);
//				switch(keyVal){
//					case 4:  //确认
//						g_Yes_Or_No=1;
//						fns_Form_redraw();
//						break;
//					case 6:  //否
//						g_Yes_Or_No=0;
//						fns_Form_redraw();
//						break;
//					case 12:
//						if(g_Yes_Or_No){
//							g_Yes_Or_No=0;
//							if(g_SysLogoSet_Flag){
//									g_System_Para_Info.sys_logo_switch=1;
//									System_Para_Save();
//							}
//							else{
//									g_System_Para_Info.sys_logo_switch=0;
//									System_Para_Save();
//							}
//							SysLogoSetStatus=SysLogoSetSuccess;
//							fns_Form_redraw();
//						}
//						else{
//							g_Yes_Or_No=0;
//							cotMenu_Exit();	
//							fns_Form_redraw(); 
//						}
//						break;
//					case 10:
//						cotMenu_Exit();	
//						break;
//					default:
//						break;
//				}
//				break;
//			case SysLogoSetSuccess: 
//				break;
//			case SysLogoSetFail: 
//				break;
//				default:	
//				break;					
//		}
//}

//void SysLogoSet_onTick(BASEFORM* pThis)
//{
//	pThis->timeTick=2;
//		if(g_Wake_Time_Count>SYSTEM_DELAY_TIME){
//			FNS_ALL_FORM_CLOSE();
//			g_Wake_Time_Count=0;
//			fns_Form_redraw();
//		}

//		switch(SysLogoSetStatus)
//		{
//			case SysLogoSet:
//					fns_Form_redraw();
//				break;
//			case SysLogoSetConfirm:
//					fns_Form_redraw();
//				break;
//			case SysLogoSetSuccess:
//					if(	g_SystemSet_Pri_Display!=SysLogoSetSuccess){
//					g_SystemSet_Pri_Display=SysLogoSetSuccess;
//					pThis->timeTick=20;//2秒
//					fns_Form_redraw();
//				}
//			else{
//					cotMenu_Exit();
//					fns_Form_redraw();
//				}
//				break;
//			case SysLogoSetFail:
//				if(	g_SystemSet_Pri_Display!=SysLogoSetFail){
//					g_SystemSet_Pri_Display=SysLogoSetFail;
//					pThis->timeTick=20;//2秒
//					fns_Form_redraw();
//				}
//			else{
//					cotMenu_Exit();
//					fns_Form_redraw();
//				}
//				break;
//			default :
//				break;
//		}
//}

//void SysLogoSet_onCreate(BASEFORM* pThis)
//{
//		if(g_System_Para_Info.sys_logo_switch)
//			g_SysLogoSet_Flag=1;
//		else
//			g_SysLogoSet_Flag=0;
//		pThis->timeTick=2;
//		g_Yes_Or_No=0;
//		g_SystemSet_Pri_Display=0;
//		SysLogoSetStatus= SysLogoSet;
//		g_Wake_Time_Count=0;
//}

#endif
