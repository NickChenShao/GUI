#if 1
#include "StartForm.h"
#include "SystemManager.h"
#include <string.h>
#include <stdio.h>
#include "peripheral_fun_share.h"
#include "xmcore_menu_gui.h"



typedef enum
{
	SelfCheckModeFinger=1,  
	SelfCheckModeFingerOK,	
	SelfCheckModeIC,
	SelfCheckModeICOK,
	selfCheckModeTouch,
	selfCheckModeTouchOK,
	SelfCheckModeInit,
	SelfCheckModeResult,	
} SelfCheckModeDef;

//老化测试
void BurnInTest_enter(void);
void BurnInTest_exit(void);
void BurnInTest_load(uint8_t menuState);
void BurnInTest_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo);
void BurnInTest_onShow(MenuShow_s *pstShowInfo,uint8_t menuState);


//自检模式
void SelfCheckMode_enter(void);
void SelfCheckMode_exit(void);
void SelfCheckMode_load(uint8_t menuState);
void SelfCheckMode_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo);
void SelfCheckMode_onShow(MenuShow_s *pstShowInfo,uint8_t menuState);

/*语音开关*/

#define PLAY_MUSIC(...)					//播放
#define PLAY_MUSIC_ON(...)
#define WAIT_MUSIC(...)

/*常开模式*/
#define RUN_MOTOR()
#define REVERS_MOTOR()

//指纹检测，卡检测接口


MenuList_s g_stBurnInTestMenu =		{{"老化测试", "user management"},	 BurnInTest_enter, BurnInTest_exit, BurnInTest_load, BurnInTest_task, NULL};
MenuList_s g_stSelfCheckModeMenu =	{{"自检模式", "user management"},	 SelfCheckMode_enter, SelfCheckMode_exit, SelfCheckMode_load, SelfCheckMode_task, NULL};



unsigned short g_Touch_Test_Mask=0;
unsigned int g_Motor_Run_Times=0;
unsigned short g_Rfid_Test=0;
unsigned short g_Finger_Test=0;
unsigned int g_Time_Count=0; 
InputBuf_TypeDef TestInputBuf;



void XmMcu_SelfCheckMode_onCreate()
{
	//进入自检模式的入口
		//&g_stBurnInTestMenu
}
void XmMcu_SelfCheckMode_onCreate()
{
	//进入自检模式的入口
		//&g_stSelfCheckModeMenu
}


/********************老化检测***************************/
void BurnInTest_onShow(MenuShow_s *pstShowInfo,uint8_t menuState)
{
		unsigned char x,y,i;
		unsigned char date_str[20];
		unsigned char touch_num;
		unsigned short touch_mask_temp;
		CDC* pFormCDC;
		x=1;
		y=0;
		LCD_Battery_Level(pFormCDC,g_Voltage_Level,x,y);
		OLED_HZ_ShowMixStr_XCentre(pFormCDC,"老化测试",1,y,FONT_16X16);

		y=0;
		touch_mask_temp=g_Touch_Test_Mask;
		for(i=0;i<12;i++)
		{
			if(touch_mask_temp&0x01)
			{
					touch_num++;
			}
			touch_mask_temp>>=1;
		}

		x=104;
		if(touch_num)
		OLED_ShowNum(pFormCDC,x,y,touch_num,2,16,1,FONT_16X16);		
		
		y=24;
		
		memset(date_str,0x00,sizeof(date_str));
		sprintf((char *)date_str,"运行%08d次",g_Motor_Run_Times);
		OLED_HZ_ShowMixStr_XCentre(pFormCDC,date_str,1,y,FONT_16X16);	
		
		y=48;
//		time_count_temp=g_Time_Count/10;  //200ms一次，2秒切换一次

		if(!(g_Rfid_Test||g_Finger_Test)){
			if(g_Motor_Run_Times%2){
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"读卡模块未检",1,y,FONT_16X16);	
			}
			else{
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"指纹模块未检",1,y,FONT_16X16);	
					
			}
		}
		else if(!g_Finger_Test){
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"指纹模块未检",1,y,FONT_16X16);		
		}
		else if(!g_Rfid_Test){
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"读卡模块未检",1,y,FONT_16X16);	
				
		}
}



void BurnInTest_onChar(uint8_t menuState, uint8_t keyVal)
{
		unsigned char exit_cmd[4]={8,8,8,12};
		if(keyVal<13)
		g_Touch_Test_Mask&=~(1<<(keyVal-1));	
		PLAY_MUSIC(7);
		switch(keyVal){
			case 12:
				TestInputBuf.KeyBuf[TestInputBuf.KeyNum]=keyVal;
				TestInputBuf.KeyNum++;
				if(TestInputBuf.KeyNum>=4){
						if(!memcmp(&TestInputBuf.KeyBuf[TestInputBuf.KeyNum-4],exit_cmd,4)){
							cotMenu_Exit();
							memset(&TestInputBuf,0,sizeof(InputBuf_TypeDef));
							fns_Form_redraw(); 
						}
				}
				fns_Form_redraw(); 
				break;
			case 10:
				memset(&TestInputBuf,0,sizeof(InputBuf_TypeDef));
				fns_Form_redraw(); 
				break;
			default:
				if(TestInputBuf.KeyNum>20){
					TestInputBuf.KeyBuf[TestInputBuf.KeyNum]=keyVal;
					TestInputBuf.KeyNum++;
					memcpy(TestInputBuf.KeyBuf,&TestInputBuf.KeyBuf[TestInputBuf.KeyNum-4],4);
					TestInputBuf.KeyNum=4;
				}
				else{
					TestInputBuf.KeyBuf[TestInputBuf.KeyNum]=keyVal;
					TestInputBuf.KeyNum++;
				}
				break;
		}
}

void BurnInTest_onTick(uint8_t menuState)
{
	unsigned char rfid_ret;
	unsigned char finger_ret;
	unsigned char iccard_sn[4];
	unsigned short finger_index=0;
	g_Time_Count++;
	

	 if(!(g_Time_Count%40)){
		RUN_MOTOR();
	}
	 else	if(!(g_Time_Count%20)){
		REVERS_MOTOR();
		 g_Motor_Run_Times++;

	}
	if(!g_Rfid_Test){
		rfid_ret=RC522_Operation(iccard_sn);
		if(rfid_ret==1){
			g_Rfid_Test=1;
		}
		else if(rfid_ret==0){		

		}
	}
	
	if(!g_Finger_Test)
	{
		if(g_Finger_Int_Flag)
		{
			g_Finger_Int_Flag=0;
			finger_ret=AutoIdentify(&finger_index);
			if(ERR_NOFINGER==finger_ret)
			{

			}
			else if(ERR_OK==finger_ret)
			{
					g_Finger_Test=1;
			}
			else
			{
					g_Finger_Test=1;
			}
		}
	}
}





void BurnInTest_enter(void)
{
	g_Yes_Or_No=0;
	g_SystemSet_Pri_Display=0;
	g_Touch_Test_Mask=0xfff;
	g_Rfid_Test=0;
	g_Finger_Test=0;
	g_Wake_Time_Count=0;
}

void BurnInTest_exit(void)
{
	REVERS_MOTOR();
}


void BurnInTest_load(uint8_t menuState)
{

	cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 20, 0);
}

void BurnInTest_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo)
{
	uitn8_t keyVal;
	BurnInTest_onChar(menuState, keyVal);
	if(!cotMenu_getTick())
	{
		BurnInTest_onTick(menuState);
	}

}


/********************自检模式*************************/
void SelfCheckMode_onShow(MenuShow_s *pstShowInfo,uint8_t menuState)
{
	CDC* pFormCDC;
	unsigned char x,y;
		switch(menuState)
		{
			case SelfCheckModeFinger:
				y=0;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"指纹模块自检",1,y,FONT_16X16);
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"请按手指",1,y,FONT_16X16);
				break;
			case SelfCheckModeFingerOK:
				y=0;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"指纹模块自检",1,y,FONT_16X16);
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"验证通过",1,y,FONT_16X16);
				break;
			case SelfCheckModeIC:
				y=0;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"卡模块自检",1,y,FONT_16X16);
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"请刷感应卡",1,y,FONT_16X16);
				break;
			case SelfCheckModeICOK:
				y=0;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"卡模块自检",1,y,FONT_16X16);
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"验证通过",1,y,FONT_16X16);
				break;
			case selfCheckModeTouch:
				y=0;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"触摸按键自检",1,y,FONT_16X16);
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"请触摸按键",1,y,FONT_16X16);
				break;
			case selfCheckModeTouchOK:
				y=0;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"触摸按键自检",1,y,FONT_16X16);
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"验证通过",1,y,FONT_16X16);
				break;
			case SelfCheckModeInit:
				x=0;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"初始化中..",1,y,FONT_16X16);
				break;
			case SelfCheckModeResult:
				y=0;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"自检结果",1,y,FONT_16X16);
				y+=16;
				x=0;
				if(g_Finger_Test)  
				OLED_ShowMixStr(pFormCDC,x,y,"指纹 OK",1,1,FONT_16X16);
				else  OLED_ShowMixStr(pFormCDC,x,y,"指纹 NG",1,1,FONT_16X16);
				x=64;
				OLED_ShowMixStr(pFormCDC,x,y,"时钟 OK",1,1,FONT_16X16);
				y+=16;
				x=0;
				if(g_Rfid_Test) OLED_ShowMixStr(pFormCDC,x,y,"读卡 OK",1,1,FONT_16X16);
				else  OLED_ShowMixStr(pFormCDC,x,y,"读卡 NG",1,1,FONT_16X16);
				x=64;
				OLED_ShowMixStr(pFormCDC,x,y,"电量 OK",1,1,FONT_16X16);
				y+=16;
				x=0;
				if(!g_Touch_Test_Mask) OLED_ShowMixStr(pFormCDC,x,y,"触摸 OK",1,1,FONT_16X16);
				else  OLED_ShowMixStr(pFormCDC,x,y,"触摸 NG",1,1,FONT_16X16);
				x=64;
				OLED_ShowMixStr(pFormCDC,x,y,"存储 OK",1,1,FONT_16X16);
			break;
			default:
				
			break;
		}
}

void SelfCheckMode_onChar(uint8_t menuState, uint8_t keyVal)
{
	switch(menuState)
	{
		case SelfCheckModeFinger:
			break;
		case SelfCheckModeFingerOK:
			break;
		case SelfCheckModeIC:
			break;
		case SelfCheckModeICOK:
			break;
		case selfCheckModeTouch:
			PLAY_MUSIC(7);
			g_Time_Count=0;
			if(keyVal<13)
			g_Touch_Test_Mask&=~(1<<(keyVal-1));
			if(0==g_Touch_Test_Mask){
				cotMenu_changeMenuState(selfCheckModeTouchOK);
				g_Time_Count=0;
				fns_Form_redraw();
			}

			break;
		case selfCheckModeTouchOK:
			break;
		case SelfCheckModeInit:
			break;
		case SelfCheckModeResult:
			break;
	}
}

void SelfCheckMode_onTick(uint8_t menuState)
{
	unsigned char rfid_ret;
	unsigned char finger_ret;
	unsigned char iccard_sn[4];
	unsigned short finger_index=0;
	g_Time_Count++;
	switch(menuState)
	{
		case SelfCheckModeFinger:
			if(!(g_Time_Count%20))
			{
				cotMenu_changeMenuState(SelfCheckModeIC);
				g_Time_Count=0;
			}
			g_Finger_Int_Flag=0;
			finger_ret=AutoIdentify(&finger_index);
			if(ERR_NOFINGER==finger_ret)
			{

			}
			else if(ERR_OK==finger_ret)
			{
				Xprintf("finger test OK\r\n");
				g_Finger_Test=1;
				g_Time_Count=0;
				cotMenu_changeMenuState(SelfCheckModeFingerOK);
			}
			else
			{
				Xprintf("finger test err\r\n");
				if(9==finger_ret)  //没搜索到
				{
					g_Finger_Test=1;
					g_Time_Count=0;
					cotMenu_changeMenuState(SelfCheckModeFingerOK);
				}
			}
			break;
		case SelfCheckModeFingerOK:
				cotMenu_changeMenuState(SelfCheckModeIC);
				g_Time_Count=0;
			break;
		case SelfCheckModeIC:
			if(!(g_Time_Count%20)){
				cotMenu_changeMenuState(selfCheckModeTouch);
				g_Time_Count=0;
			}

			if(!g_Rfid_Test){
				rfid_ret=RC522_Operation(iccard_sn);
				if(rfid_ret==1){
					g_Rfid_Test=1;
					cotMenu_changeMenuState(SelfCheckModeICOK);
					g_Time_Count=0;
				}
				else if(rfid_ret==0){		
	
				}
			}
			break;
		case SelfCheckModeICOK:
			cotMenu_changeMenuState(selfCheckModeTouch);
			g_Time_Count=0;
			break;
		case selfCheckModeTouch:
			if(!(g_Time_Count%15)){
				cotMenu_changeMenuState(SelfCheckModeInit);
				g_Time_Count=0;

			}
			break;
		case selfCheckModeTouchOK:
			cotMenu_changeMenuState(SelfCheckModeInit);
			g_Time_Count=0;
			break;
		case SelfCheckModeInit:
			cotMenu_changeMenuState(SelfCheckModeResult);
			g_Time_Count=0;
			break;
		case SelfCheckModeResult:
			cotMenu_Exit();
			break;
	}
}

void SelfCheckMode_enter(void)
{
	g_Yes_Or_No=0;
	g_SystemSet_Pri_Display=0;
	g_Touch_Test_Mask=0xfff;
	g_Touch_Test_Mask=0xfff;
	g_Rfid_Test=0;
	g_Finger_Test=0;
	g_Wake_Time_Count=0;
	cotMenu_changeMenuState(SelfCheckModeFinger);
}

void SelfCheckMode_exit(void)
{

}


void SelfCheckMode_load(uint8_t menuState)
{
	
	switch(menuState)
	{
		case SelfCheckModeFinger:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 20, 0);
			break;
		case SelfCheckModeFingerOK:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 200, 1);
			break;
		case SelfCheckModeIC:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 20, 0);
			break;
		case SelfCheckModeICOK:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 200, 1);

			break;
		case selfCheckModeTouch:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 20, 0);
			break;
		case selfCheckModeTouchOK:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 200, 1);
			break;
		case SelfCheckModeInit:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 200, 1);
			break;
		case SelfCheckModeResult:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 200, 1);
			break;
	}

}

void SelfCheckMode_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo)
{
	uitn8_t keyVal;
	SelfCheckMode_onChar(menuState, keyVal);
	if(!cotMenu_getTick())
	{
		SelfCheckMode_onTick(menuState);
	}

}

#endif
