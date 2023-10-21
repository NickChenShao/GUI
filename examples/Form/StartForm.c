
#include "StartForm.h"
#include "SystemManager.h"
#include "peripheral_fun_share.h"


#define STARTFORM_10_TICK_100MS 10		//100ms
#define STARTFORM_20_TICK_200MS 20		//100ms
#define STARTFORM_20_TICK_2000MS 200		//100ms

/*********************************
	y=1  16     第一行

	y=21  22    第二行

	y=47  16      第三行
*********************************/

Open_Lock_Info_Def g_Open_Lock_Info;

unsigned char g_DoubleCheck_Flag;

//待实现接口
#define SHOW_CLOCK_FUNC()					//显示时间
#define PLAY_MUSIC_ON(MUSIC_ID)				//播放相关的音乐
#define PLAY_MUSIC(MUSIC_ID)

#define TOUCH_LED_ENABLE()
#define LED_FLASHES(FLASH_NUM)

#define LCD_ON()

#define RUN_MOTOR()
#define PLAY_BAOJING_MUSIC()
#define REVERS_MOTOR()
#define	CLEAN_USER_ALL()	//用户清空
#define SYSTEM2FACTORY()	//系统设置参数恢复

#define OLED_HZ_ShowMixStr_XCentre(...)
#define OLED_ShowChar(...)
#define OLED_ShowNum(...)
#define OLED_ShowMixStr(...)
#define OLED_YesOrNO(...)
//#define OLED_ShowMixStr(...)LCD_Lock_Icon
//#define OLED_ShowMixStr(...)LCD_Lock_Icon



#define progress_bar(...)
#define LCD_Open_Icon(...)
#define LCD_Lock_Icon(...)
#define Err_Alarm_Icon(...)
#define Lower_Power_Icon(...)
#define LCD_Finger_Input_Icon(...)
#define LCD_Admin_Icon(...)
#define LCD_Error_Icon(...)
#define LCD_Ring_Icon(...)
#define Dir4_Logo_Icon(...)
#define Dir6_Logo_Icon(...)






extern uint16_t RC522_Operation(unsigned char *iccard_sn);
extern uint8_t XmMcu_ICCard_judgeICCard( unsigned char *iccard_sn,Open_Lock_Info_Def *user_info);
extern uint8_t XmMcu_Passwd_judgePasswd( unsigned char *passwd_input,unsigned short passwd_num,Open_Lock_Info_Def *user_info);

extern uint8_t XmMcu_setAdminLoginInfo(unsigned short userId, unsigned short userType, unsigned short userMode);
//{
//	g_Admin_Login_Info.User_ID_Num=0x00;
//	g_Admin_Login_Info.User_Type=Admin_User;
//	g_Admin_Login_Info.User_Mode=PasswdMode;
//}
extern uint8_t XmMcu_setOperateLogUserId(unsigned short userId);
//{
//	OperateLog_Info.admin_num= userId;
//}

extern uint8_t XmMcu_saveAlarmLog(unsigned short AlarmType, time_t clockSec);
//{
//	AlarmLog_Info.alarm_type = alarmType;
//	AlarmLog_Info.clock_sec = Clock2Time(&clock_env);
//	XmMcu_Log_saveAlarmLog(&AlarmLog_Info);
//}

extern void XmMcu_saveOpenLog(time_t clockSec, unsigned short userId, unsigned short userType);
//{
//	OpenLog_Info.clock_sec=Clock2Time(&clock_env);
//	OpenLog_Info.user_num=g_Open_Lock_Info.User_ID_Num;
//	OpenLog_Info.user_type=g_Open_Lock_Info.User_Type;
//	XmMcu_Log_saveOpenLog(&s_stDouboleCheckOpenLogInfo);
//}


extern int AutoIdentify(unsigned short *nIndex);
unsigned char g_restore_bar_value = 0;	//进度条
unsigned int g_Wait_Time_Count = 0; 	//100ms的速率递减
unsigned char g_RingBells_Times = 0;	//门铃计数动画	
unsigned char g_Yes_Or_No = 0;



//unsigned char passwd_ret;
//PassWord_TypeDef  passwd_buff;
//unsigned char rfid_ret;
//unsigned char finger_ret;
//unsigned short finger_index = 0;
//unsigned short iccard_ret;
//unsigned char iccard_sn[4];
//unsigned int	int_gpio_last_value = 0;


//typedef struct StartMenuShow
//{
//	uint8_t titleNum; 
//	char* (*paapTitleStr)[2][5];
//} StartMenuShow_s;

const uint8_t* g_aaapTitleStr[SelectModeTypeDefMax][1][3] = 
{
	{{NULL}},
	{{NULL}},
	{{"密码开门","输入密码"}},
	{{"开门验证","验证指纹","请按手指"}},
	{{NULL}},								//EnterIC
	{{"菜单验证","请验证","管理员"}},
	{{"菜单验证","输入密码"}},
	{{"系统提示","密码验证失败"}},
	{{"系统提示","指纹验证失败"}},
	{{"系统提示","感应卡验证失败"}},
	{{"系统提示","密码验证失败"}},
	{{"系统提示","指纹验证失败"}},
	{{"系统提示","感应卡验证失败"}},
	{{"体验用户\r\n管理用户\r\n普通用户\r\n宾客用户\r\n临时用户","上锁时间:%02d秒"}},
	{{NULL}},
	{{"系统提示","锁已关闭"}},
	{{"系统提示","常开模式"}},
	{{"系统提示","锁已设置常开","请按把手开门"}},
	{{"系统提示","门铃已启动"}},
	{{"系统提示","确定要初始化吗\?"}},
	{{NULL}},									//RestoreBar
	{{NULL}},
	{{NULL}},
	{{"系统提示","系统已锁死","时间%03d 秒"}},	//Err_Alarm
	{{"电池电量低","请更换电池"}},
	{{"系统提示","请换另一个","用户验证"}},
	{{"系统提示","同一个用户","请重新验证"}},
};


typedef struct _InPutBuf
{
	unsigned char KeyFlag;
	unsigned char KeyNum;
	unsigned char KeyBuf[30];	
}InputBuf_TypeDef;



InputBuf_TypeDef InputBuf;
#define KEY_NUM_MAX  30

InputErrTypeDef  Input_Err_Record;


//按键输入
void XmMcu_StartFormKey_setKeyNumber(uint8_t keyVal)
{
	unsigned char key_buf_temp[KEY_NUM_MAX] = "";
	if(InputBuf.KeyNum < KEY_NUM_MAX)
	{
		if(11 == keyVal)InputBuf.KeyBuf[InputBuf.KeyNum] = 0;
		else InputBuf.KeyBuf[InputBuf.KeyNum] = keyVal;
		InputBuf.KeyNum++;
	}
	else
	{
		memcpy(key_buf_temp, InputBuf.KeyBuf, KEY_NUM_MAX);
		memcpy(InputBuf.KeyBuf, &key_buf_temp[1], KEY_NUM_MAX - 1);
		if(11 == keyVal)InputBuf.KeyBuf[InputBuf.KeyNum] = 0;
		else InputBuf.KeyBuf[InputBuf.KeyNum] = keyVal;
		InputBuf.KeyBuf[KEY_NUM_MAX - 1] = keyVal;
	}
	InputBuf.KeyFlag = 1;

}
void XmMcu_StartFormKey_getKeyNumber()
{

}
void XmMcu_StartFormKey_startKeyNumber(uint8_t keyVal)
{
	if(11 == keyVal)InputBuf.KeyBuf[0] = 0;
	else InputBuf.KeyBuf[0] = keyVal;
	InputBuf.KeyNum = 1;
	InputBuf.KeyFlag = 1;

}

void XmMcu_StartFormKey_clearKeyNumber()
{
	memset(&InputBuf, 0x00, sizeof(InputBuf_TypeDef));
}



unsigned char  XmMcu_OpenDoorDect(void)
{
	static OpenLogStruts  s_stDouboleCheckOpenLogInfo;
	//先判断是否是管理员
	//不是管理员要判定是否是双人验证
	if(g_System_Para_Info.double_check_enable)
	{
		if(g_Open_Lock_Info.User_Type != Admin_User)
		{
			if(1 == g_DoubleCheck_Flag)
			{
				//先验证是否重复
				if(s_stDouboleCheckOpenLogInfo.user_num == g_Open_Lock_Info.User_ID_Num)
				{
					cotMenu_changeMenuState(DoubleCheckErrTips);
					return 1;
				}
				else
				{
					g_DoubleCheck_Flag = 0;
					cotMenu_changeMenuState(LockAlreadyOpen);
					XmMcu_saveOpenLog(s_stDouboleCheckOpenLogInfo.clock_sec, s_stDouboleCheckOpenLogInfo.user_num, s_stDouboleCheckOpenLogInfo.user_type);
					XmMcu_saveOpenLog(Clock2Time(&clock_env), g_Open_Lock_Info.User_ID_Num, g_Open_Lock_Info.User_Type);
					XmMcu_StartFormKey_clearKeyNumber();
					return 1;
				}
			}
			else
			{
				g_DoubleCheck_Flag = 1;
				s_stDouboleCheckOpenLogInfo.clock_sec = Clock2Time(&clock_env);
				s_stDouboleCheckOpenLogInfo.user_num = g_Open_Lock_Info.User_ID_Num;
				s_stDouboleCheckOpenLogInfo.user_type = g_Open_Lock_Info.User_Type;
				cotMenu_changeMenuState(DoubleCheckTips);
				return 1;
			}
		}
	}
	else
	{
		g_DoubleCheck_Flag=0;
		cotMenu_changeMenuState(LockAlreadyOpen);
		System_OpenLogInfo_Save(&clock_env,g_Open_Lock_Info.User_ID_Num,g_Open_Lock_Info.User_Type);
		memset(&InputBuf,0x00,sizeof(InputBuf_TypeDef));
	}
	return 1;
}


void XmMcu_openDoorScan(uint8_t firstOpenFlag)
{

	unsigned char rfid_ret,iccard_ret,finger_ret;
	unsigned short finger_index = 0;
	unsigned char iccard_sn[4];
	unsigned char finger_chip_sn[32];

	rfid_ret = RC522_Operation(iccard_sn);
	if(rfid_ret == 1)
	{
		Input_Type = ICCardMode;
		g_Wake_Time_Count = 0;
		if(g_System_Para_Info.always_open_flag && (firstOpenFlag == 3 || firstOpenFlag == 1))
		{
			cotMenu_changeMenuState(LockAlwaysOpenTip);
			return ;
		}
		iccard_ret = XmMcu_ICCard_judgeICCard(iccard_sn, &g_Open_Lock_Info) ;  //查找密码
		if(iccard_ret)
		{
			XmMcu_OpenDoorDect();
		}
		else
		{
			cotMenu_changeMenuState(ICCard_Fail);
			return ;
		}
	}
	else if(rfid_ret == 0)
	{
		cotMenu_changeMenuState(ICCard_Fail);
		return ;
	}
	// FIXME:没插外设，这里会阻塞900ms,返回ERR_NOFINGER
//	finger_ret = AutoIdentify(&finger_index);
//	if(ERR_NOFINGER == finger_ret)
//	{
//	
//	}
//	else if(ERR_OK == finger_ret)
//	{
//		g_Wake_Time_Count = 0;
//		Input_Type = FingerPrintMode;
//		if(!g_System_Para_Info.restore_factory)
//		{
//			cotMenu_changeMenuState(LockAlreadyOpen);
//			break;
//		}
//		else
//		{
//			if(g_System_Para_Info.always_open_flag && (firstOpenFlag == 3))
//			{
//				cotMenu_changeMenuState(LockAlwaysOpenTip);
//				break;
//			}
//	
//			memset(finger_chip_sn, 0, 32);
//			cmd_GetChipSN(finger_chip_sn, 32);
//	
//			if(!memcmp(finger_chip_sn, g_System_Para_Info.finger_chip_sn, 32))
//			{
//				finger_ret = get_finger_info(finger_index, &g_Open_Lock_Info);	 //查找密码
//				XmMcu_OpenDoorDect();
//				break;
//			}
//			else
//			{
//				cotMenu_changeMenuState(FingerPrintNG);
//			}
//			break;
//		}
//	}
//	else
//	{
//		g_Wake_Time_Count = 0;
//		Input_Type = FingerPrintMode;
//		if(!g_System_Para_Info.restore_factory)
//		{
//			cotMenu_changeMenuState(LockAlreadyOpen);
//			break;
//		}
//		else
//		{
//			if(g_System_Para_Info.always_open_flag && (firstOpenFlag == 3))
//			{
//				cotMenu_changeMenuState(LockAlwaysOpenTip);
//			}
//			else
//			{
//				cotMenu_changeMenuState(FingerPrintNG);			
//			}
//		}
//	}
}


//管理登入验证
void XmMcu_LoginMenuScan()
{
	unsigned char rfid_ret,iccard_ret,finger_ret;
	unsigned char iccard_sn[4];
	rfid_ret = RC522_Operation(iccard_sn);
	if(rfid_ret == 1)
	{
		g_Wake_Time_Count = 0;
		iccard_ret = XmMcu_ICCard_judgeICCard(iccard_sn, &g_Open_Lock_Info) ;  //查找密码
		if(iccard_ret)
		{
			if(Admin_User == g_Open_Lock_Info.User_Type) //管理员用户
			{
				PLAY_MUSIC(47);  //验证成功
				XmMcu_setOperateLogUserId(g_Open_Lock_Info.User_ID_Num);
				XmMcu_setAdminLoginInfo(g_Open_Lock_Info.User_ID_Num,Admin_User,ICCardMode);
				cotMenu_Enter();
				cotMenu_changeMenuState(StartStates);
				XmMcu_StartFormKey_clearKeyNumber();;
				return ;
			}
			else
			{
				cotMenu_changeMenuState(ICCardLoginFail);
				PLAY_MUSIC(37);
				LED_FLASHES(3);
				XmMcu_StartFormKey_clearKeyNumber();
				return ;
			}
		}
		else
		{
			cotMenu_changeMenuState(ICCardLoginFail);
			return ;

		}
	}

	else if(rfid_ret == 0)
	{
		cotMenu_changeMenuState(ICCard_Fail);
		return ;
	}

//	unsigned char finger_chip_sn[32];

//	finger_ret = AutoIdentify(&finger_index);
//	if(ERR_NOFINGER == finger_ret)
//	{
//
//	}
//	else if(ERR_OK == finger_ret)
//	{
//		g_Wake_Time_Count = 0;
//		memset(finger_chip_sn, 0, 32);
////		cmd_GetChipSN(finger_chip_sn, 32);
//		if(!memcmp(finger_chip_sn, g_System_Para_Info.finger_chip_sn, 32))
//		{
//			finger_ret = get_finger_info(finger_index, &g_Open_Lock_Info);	 //查找密码
//			if(finger_ret)
//			{
//				if(Admin_User == g_Open_Lock_Info.User_Type) //管理员用户
//				{
//					PLAY_MUSIC(47);  //验证成功
//					XmMcu_setOperateLogUserId(g_Open_Lock_Info.User_ID_Num);
//					XmMcu_setAdminLoginInfo(g_Open_Lock_Info.User_ID_Num,Admin_User,FingerPrintMode);
//					cotMenu_Enter();
//					cotMenu_changeMenuState(StartStates);
//					XmMcu_StartFormKey_clearKeyNumber();
//					fns_Form_redraw();
//					break;
//				}
//				else
//				{
//					cotMenu_changeMenuState(FingerPrintLoginFail);
//					PLAY_MUSIC(37);
//					LED_FLASHES(3);
//					XmMcu_StartFormKey_clearKeyNumber();
//					fns_Form_redraw();
//					break;
//				}
//			}
//			else   //这个地方应该跑不到
//			{
//				g_Wake_Time_Count = 0;
//				cotMenu_changeMenuState(FingerPrintLoginFail);
//				fns_Form_redraw();
//			}
//		}
//		else
//		{
//			cotMenu_changeMenuState(FingerPrintLoginFail);
//			PLAY_MUSIC(37);
//			LED_FLASHES(3);
//			XmMcu_StartFormKey_clearKeyNumber();
//		}
//	}
//	else
//	{
//		g_Wake_Time_Count = 0;
//		cotMenu_changeMenuState(FingerPrintLoginFail);
//
//	}
}
uint8_t XmMcu_SaveAlarmLog(uint8_t alarmType)
{
	uint8_t alarmFlag = 0;
	//可以错的次数
	if(alarmType = PASSWD_ALARM)
	{
		Input_Err_Record.passwd_error++;
		alarmFlag = (Input_Err_Record.passwd_error > 2) ? 1 : 0 ;
	}
	else if(alarmType = FINGER_ALARM)
	{
		Input_Err_Record.finger_error++;
		alarmFlag = (Input_Err_Record.finger_error > 4) ? 1 : 0 ;
	}
	else if(alarmType = FINGER_ALARM)
	{
		Input_Err_Record.iccard_error++;
		alarmFlag = (Input_Err_Record.iccard_error > 2) ? 1 : 0 ;
	}
	if(alarmFlag)
	{
		XmMcu_saveAlarmLog(alarmType,Clock2Time(&clock_env));
		g_Wait_Time_Count = 1800;		//等待的时间
		memset(&Input_Err_Record, 0x00, sizeof(InputErrTypeDef));
	}
	return alarmFlag;
}


void XmMcu_StartForm_checkPassWd(void)
{
	unsigned char passwd_ret;
	if(InputBuf.KeyNum < 6)
	{
		cotMenu_changeMenuState(PassWordNG);
		PLAY_MUSIC(37);
		LED_FLASHES(3);
	}
	else
	{
		passwd_ret = XmMcu_Passwd_judgePasswd(InputBuf.KeyBuf, InputBuf.KeyNum, &g_Open_Lock_Info); //返回用户类型
		if(passwd_ret)
		{
			if(Admin_User == g_Open_Lock_Info.User_Type) //管理员用户
			{
				PLAY_MUSIC(47);  //验证成功
				XmMcu_setOperateLogUserId(g_Open_Lock_Info.User_ID_Num);
				XmMcu_setAdminLoginInfo(g_Open_Lock_Info.User_ID_Num,Admin_User,PasswdMode);
				cotMenu_Enter();
				cotMenu_changeMenuState(StartStates);
				XmMcu_StartFormKey_clearKeyNumber();
				return ;
			}
			else if(passwd_ret)   //非管理员用户
			{
				cotMenu_changeMenuState(PassWordLoginFail);
				XmMcu_StartFormKey_clearKeyNumber();
				return ;
			}
			else
			{
				cotMenu_changeMenuState(PassWordLoginFail);
				XmMcu_StartFormKey_clearKeyNumber();
			}
		}
		else
		{
			cotMenu_changeMenuState(PassWordLoginFail);
			XmMcu_StartFormKey_clearKeyNumber();
		}
	}

}

void XmMcu_StartForm_checkMenuScanIcAndFinger()
{
	unsigned char rfid_ret,iccard_ret,finger_ret;
	unsigned char iccard_sn[4];
	rfid_ret = RC522_Operation(iccard_sn);
	if(rfid_ret == 1)
	{
		g_Wake_Time_Count = 0;
		iccard_ret = XmMcu_ICCard_judgeICCard(iccard_sn, &g_Open_Lock_Info) ;  //查找密码
		if(iccard_ret)
		{
			if(Admin_User == g_Open_Lock_Info.User_Type) //管理员用户
			{
				PLAY_MUSIC(47);  //验证成功

				
				XmMcu_setOperateLogUserId(g_Open_Lock_Info.User_ID_Num);
				XmMcu_setAdminLoginInfo(g_Open_Lock_Info.User_ID_Num,Admin_User,ICCardMode);
				cotMenu_Enter();
				cotMenu_changeMenuState(StartStates);
				XmMcu_StartFormKey_clearKeyNumber();
				return ;
			}
			else
			{
				cotMenu_changeMenuState(ICCardLoginFail);
				PLAY_MUSIC(37);
				LED_FLASHES(3);
				XmMcu_StartFormKey_clearKeyNumber();
			}
		}
		else
		{
			cotMenu_changeMenuState(ICCardLoginFail);
			return ;
		}
	}
	else if(rfid_ret == 0)
	{
		cotMenu_changeMenuState(ICCard_Fail);
		return ;
	}

	
//	unsigned char finger_chip_sn[32];
//	finger_ret = AutoIdentify(&finger_index);
//	if(ERR_NOFINGER == finger_ret)
//	{
//
//	}
//	else if(ERR_OK == finger_ret)
//	{
//		g_Wake_Time_Count = 0;
//		memset(finger_chip_sn, 0, 32);
//		cmd_GetChipSN(finger_chip_sn, 32);
//		if(!memcmp(finger_chip_sn, g_System_Para_Info.finger_chip_sn, 32))
//		{
//			finger_ret = get_finger_info(finger_index, &g_Open_Lock_Info);	 //查找密码
//			if(finger_ret)
//			{
//				if(Admin_User == g_Open_Lock_Info.User_Type) //管理员用户
//				{
//					PLAY_MUSIC(47);  //验证成功
//					OperateLog_Info.admin_num = g_Open_Lock_Info.User_ID_Num;
//					g_Admin_Login_Info.User_ID_Num = g_Open_Lock_Info.User_ID_Num;
//					g_Admin_Login_Info.User_Type = Admin_User;
//					g_Admin_Login_Info.User_Mode = FingerPrintMode;
//					cotMenu_Enter();
//					cotMenu_changeMenuState(StartStates);
//					XmMcu_StartFormKey_clearKeyNumber();
//					fns_Form_redraw();
//					break;
//				}
//				else
//				{
//					cotMenu_changeMenuState(FingerPrintLoginFail);
//					PLAY_MUSIC(37);
//					LED_FLASHES(3);
//					XmMcu_StartFormKey_clearKeyNumber();
//					fns_Form_redraw();
//					break;
//				}
//			}
//			else   //这个地方应该跑不到
//			{
//				g_Wake_Time_Count = 0;
//				cotMenu_changeMenuState(FingerPrintLoginFail);
//				fns_Form_redraw();
//			}
//		}
//		else
//		{
//			cotMenu_changeMenuState(FingerPrintLoginFail);
//			PLAY_MUSIC(37);
//			LED_FLASHES(3);
//			XmMcu_StartFormKey_clearKeyNumber();
//			fns_Form_redraw();
//		}
//	}
//	else
//	{
//		g_Wake_Time_Count = 0;
//		cotMenu_changeMenuState(FingerPrintLoginFail);
//		fns_Form_redraw();
//	}

}

void XmMcu_StartForm_keyCodeSwitch(void)
{
	unsigned char test_config1[3] = {8, 8, 8};
	unsigned char passwd_ret;
	if(InputBuf.KeyNum < 6)
	{
		if(!g_System_Para_Info.restore_factory)
		{
			if(InputBuf.KeyNum == 1 && 9 == InputBuf.KeyBuf[0])
			{
				Self_Check_Mode();
			}
			else if(InputBuf.KeyNum == 3)
			{
				if(!memcmp(InputBuf.KeyBuf, test_config1, 3))
				{
					Burn_In_Test();
					XmMcu_StartFormKey_clearKeyNumber();
				}
			}
		}
		else
		{
			cotMenu_changeMenuState(PassWordNG);
			PLAY_MUSIC(ShiBaiYin);
			LED_FLASHES(3);		//LED闪3下
			XmMcu_StartFormKey_clearKeyNumber();
		}

	}
	else
	{
		if(!g_System_Para_Info.restore_factory)
		{
			if(InputBuf.KeyNum == 6)
			{
//				if(!memcmp(InputBuf.KeyBuf,test_config2,6))
//				{
//					frmMaintenanceMode_Menu();
//					memset(&InputBuf,0x00,sizeof(InputBuf_TypeDef));
//					menuState=StartStates;
//					break;
//				}
			}
			else
			{
				cotMenu_changeMenuState(LockAlreadyOpen);
				XmMcu_StartFormKey_clearKeyNumber();
			}


			return ;
		}
		else
		{
			passwd_ret = XmMcu_Passwd_judgePasswd(InputBuf.KeyBuf, InputBuf.KeyNum, &g_Open_Lock_Info);
			if(passwd_ret)
			{
				XmMcu_OpenDoorDect();
			}
			else
			{

				if(8 == InputBuf.KeyNum)
				{
//					passwd_ret = OTP_SearchPassword(&passwd_buff, InputBuf.KeyBuf);
//					if(passwd_ret)
//					{
//						g_Open_Lock_Info.User_ID_Num = 9998;
//						g_Open_Lock_Info.User_Type = Temporary_User;
//						cotMenu_changeMenuState(LockAlreadyOpen);
//						OpenLog_Info.clock_sec = Clock2Time(&clock_env);
//						OpenLog_Info.user_num = 9998;
//						OpenLog_Info.user_type = Temporary_User;
//						XmMcu_Log_saveOpenLog(&OpenLog_Info);
//						XmMcu_StartFormKey_clearKeyNumber();
//						break;
//					}
				}
				else if(12 == InputBuf.KeyNum)
				{
//					passwd_ret = OTP_SearchApartmentPassword(&passwd_buff, InputBuf.KeyBuf);
//					if(passwd_ret)
//					{
//						g_Open_Lock_Info.User_ID_Num = 9998;
//						g_Open_Lock_Info.User_Type = Temporary_User;
//						cotMenu_changeMenuState(LockAlreadyOpen);
//						OpenLog_Info.clock_sec = Clock2Time(&clock_env);
//						OpenLog_Info.user_num = 9998;
//						OpenLog_Info.user_type = Temporary_User;
//						XmMcu_Log_saveOpenLog(&OpenLog_Info);
//						XmMcu_StartFormKey_clearKeyNumber();
//						break;
//					}
				}
				cotMenu_changeMenuState(PassWordNG);
				PLAY_MUSIC(37);
				LED_FLASHES(3);
				XmMcu_StartFormKey_clearKeyNumber();
			}
		}
	}

}
/*判断是不是恢复出厂设置*/
uint8_t XmMcu_StartForm_LoginIsFactory(void)
{
	if(g_System_Para_Info.restore_factory)
	{
		PLAY_MUSIC(YanZhengGuanLiYuan);//请验证管理员
	}
	else
	{
		XmMcu_setOperateLogUserId(0x00);
		XmMcu_setAdminLoginInfo(0x00,Admin_User,PasswdMode);
	}

}
//Dect_Mode：电量检测，LockAlwaysOpenTip：常开模式
uint8_t XmMcu_StartForm_changeStateByInpuType(uint8_t menuState)
{
	if(menuState == Dect_Mode)
	{
		if(FingerPrintMode == Input_Type)
		{
			cotMenu_changeMenuState(EnterFingerPrint);
		}
		else if(PasswdMode == Input_Type)
		{
			TOUCH_LED_ENABLE();
			PLAY_MUSIC(KeyWake);
			cotMenu_changeMenuState(StartStates);
		}
		else if(ICCardMode == Input_Type)
		{
			cotMenu_changeMenuState(EnterIC);
		}
		else if(RingTrig == Input_Type)
		{
			cotMenu_changeMenuState(DoorRingTip);
		}
		else
		{
			cotMenu_changeMenuState(StartStates);
		}
		LCD_ON();
	}
	else if(menuState == LockAlwaysOpenTip)	//常开模式
	{
		if(FingerPrintMode == Input_Type)
		{
			cotMenu_changeMenuState(EnterFingerPrint);
		}
		else if(PasswdMode == Input_Type)
		{
			cotMenu_changeMenuState(EnterPassword);
		}
		else if(ICCardMode == Input_Type)
		{
			cotMenu_changeMenuState(LockAlwaysICTip);
		}
	}
	
}
uint8_t XmMcu_Lock_getDoorIsOpenAlways()
{
	return g_System_Para_Info.always_open_flag;
}

void XmMcu_FrmStart_resetKeyCheck(void)
{
#if 0
		if(g_Restore_Flag)
		{
			int_gpio_last_value = get_int_gpio_value();
			if(int_gpio_last_value & GPIO_PA3) //恢复出厂设置
			{
				if(g_Restore_Times < 6)
				{
					if(g_System_Para_Info.restore_factory)
					{
						cotMenu_changeMenuState(CheckMenu);
						PLAY_MUSIC(27);  //请验证管理员
						fns_Form_redraw();
					}
					else
					{
						g_Admin_Login_Info.User_ID_Num = 0x00;
						g_Admin_Login_Info.User_Type = Admin_User;
						g_Admin_Login_Info.User_Mode = PasswdMode;
						cotMenu_Enter();
						OperateLog_Info.admin_num = 0;
						cotMenu_changeMenuState(StartStates);
						fns_Form_redraw();
					}
				}
				g_Restore_Flag = 0;
				g_Restore_Times = 0;
			}
			else
			{
				g_Wake_Time_Count = 0;
				if(g_Restore_Times < 20)
				{
					if(!(g_Restore_Times % 6))
					{
						COMMAND_PLAY(5);
					}
					g_Restore_Times++;
				}
				else
				{
					cotMenu_changeMenuState(RestoreConfirm);
					g_Yes_Or_No = 0;
					g_Restore_Flag = 0;
					g_Restore_Times = 0;
					fns_Form_redraw();
				}
			}
		}
#endif

}
void XmMcu_timeoutReturnMainMenu(uint8_t menuState)
{
	if(menuState == StartStates)
	{
		if(g_Wake_Time_Count > 20)
		{
			g_Wake_Time_Count = 0;
			g_DoubleCheck_Flag = 0;
			system_getinto_sleep();
		}
	}
	else
	{
		if(g_Wake_Time_Count > 13)
		{
			cotMenu_changeMenuState(StartStates);		//超时自动回到主页面
			g_Wake_Time_Count = 0;
		}
	}

}


void XmMcu_FrmStart_showEnterPassword(const uint8_t** pptitleStr)
{
	uint8_t x,y,start_num,i;
	y = 1;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC, "密码开门", 1, y, FONT_16X16);
	y = 21;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC, "输入密码", 1, y, FONT_24X22);
	y = 47;
	x = 24;
	if(InputBuf.KeyFlag)
	{
		InputBuf.KeyFlag++;
		if(InputBuf.KeyFlag > 3)InputBuf.KeyFlag = 0;
		if(InputBuf.KeyNum < 11)
		{
			start_num = 1;
		}
		else
		{
			start_num = InputBuf.KeyNum - 9; //显示9个
		}
		for(i = start_num; i < InputBuf.KeyNum; i++)
		{
			OLED_ShowChar(pFormCDC, x, y, '*', 16, 1, FONT_16X16);
			x += 8;
		}
		OLED_ShowNum(pFormCDC, x, y, InputBuf.KeyBuf[InputBuf.KeyNum - 1], 1, 24, 1, FONT_16X16);
		x += 8;
		i++;
		for(i; i < 11; i++)
		{
			OLED_ShowChar(pFormCDC, x, y, '_', 16, 1, FONT_16X16);
			x += 8;
		}
	}
	else
	{
		if(InputBuf.KeyNum < 10)
		{
			for(i = 0; i < InputBuf.KeyNum; i++)
			{
				OLED_ShowChar(pFormCDC, x, y, '*', 16, 1, FONT_16X16);
				x += 8;
			}
			for(i; i < 10; i++)
			{
				OLED_ShowChar(pFormCDC, x, y, '_', 16, 1, FONT_16X16);
				x += 8;
			}
		}
		else
		{
			for(i = 0; i < 10; i++)
			{
				OLED_ShowChar(pFormCDC, x, y, '*', 16, 1, FONT_16X16);
				x += 8;
			}
		}
	}

}

void XmMcu_FrmStart_showFingerPrint(const uint8_t** pptitleStr)
{
	uint8_t x,y;
	y = 1;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC, "开门验证", 1, y, FONT_16X16);
	y = 21;
	x = 12;
	LCD_Finger_Input_Icon(pFormCDC, x, y);
	y = 21;
	x = 52;
	OLED_ShowMixStr(pFormCDC, x, y, "验证指纹", 1, 1, FONT_16X16);
	y = 37;
	OLED_ShowMixStr(pFormCDC, x, y, "请按手指", 1, 1, FONT_16X16);
}
void XmMcu_FrmStart_showCheckMenu(const uint8_t** pptitleStr)
{
	uint8_t x,y;
	y = 1;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC, "菜单验证", 1, y, FONT_16X16);
	y = 21;
	x = 12;
	LCD_Admin_Icon(pFormCDC, x, y);
	y = 21;
	x = 68;
	OLED_ShowMixStr(pFormCDC, x, y, "请验证", 1, 1, FONT_16X16);
	y = 37;
	OLED_ShowMixStr(pFormCDC, x, y, "管理员", 1, 1, FONT_16X16);

}


void XmMcu_FrmStart_showCheckMenuByPasswd(const uint8_t** pptitleStr)
{
	uint8_t x,  y, i, start_num;
	y = 1;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC, "菜单验证", 1, y, FONT_16X16);
	y = 21;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC, "输入密码", 1, y, FONT_24X22);
	y = 47;
	x = 24;
	if(InputBuf.KeyFlag)
	{
		InputBuf.KeyFlag++;
		if(InputBuf.KeyFlag > 3)InputBuf.KeyFlag = 0;
		if(InputBuf.KeyNum < 11)
		{
			start_num = 1;
		}
		else
		{
			start_num = InputBuf.KeyNum - 9; //显示9个
		}
		for(i = start_num; i < InputBuf.KeyNum; i++)
		{
			OLED_ShowChar(pFormCDC, x, y, '*', 16, 1, FONT_16X16);
			x += 8;
		}
		OLED_ShowNum(pFormCDC, x, y, InputBuf.KeyBuf[InputBuf.KeyNum - 1], 1, 24, 1, FONT_16X16);
		x += 8;
		i++;
		for(i; i < 11; i++)
		{
			OLED_ShowChar(pFormCDC, x, y, '_', 16, 1, FONT_16X16);
			x += 8;
		}
	}
	else
	{
		if(InputBuf.KeyNum < 10)
		{
			for(i = 0; i < InputBuf.KeyNum; i++)
			{
				OLED_ShowChar(pFormCDC, x, y, '*', 16, 1, FONT_16X16);
				x += 8;
			}
			for(i; i < 10; i++)
			{
				OLED_ShowChar(pFormCDC, x, y, '_', 16, 1, FONT_16X16);
				x += 8;
			}
		}
		else
		{
			for(i = 0; i < 10; i++)
			{
				OLED_ShowChar(pFormCDC, x, y, '*', 16, 1, FONT_16X16);
				x += 8;
			}
		}
	}


}

void XmMcu_FrmStart_showCheckFail(const uint8_t** pptitleStr)
{
	uint8_t x,  y;
	y = 1;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC, "系统提示", 1, y, FONT_16X16);
	x = 52;
	y = 21;
	LCD_Error_Icon(pFormCDC, x, y);
	y = 47;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC, "密码验证失败", 1, y, FONT_16X16);


}

static void FrmStart_showCheckMenuAlreadyOpen(const uint8_t* ptitleStr, uint8_t numID)
{
	uint8_t x,  y;
	y = 0;
	x = 12;
	OLED_ShowMixStr(pFormCDC, x, y, ptitleStr, 1, 1, FONT_16X16);
	x += 72;
	OLED_ShowNum(pFormCDC, x, y, numID, 4, 16, 1, FONT_16X16);

}

void XmMcu_FrmStart_showCheckMenuAlreadyOpen(const uint8_t** pptitleStr)
{
	const uint8_t* pTempStr;
	unsigned char str_buf[12];
	uint8_t x, y;
	pTempStr = (uint8_t*)strtok((char*)pptitleStr[0], "\n");
	if(!g_System_Para_Info.restore_factory)
	{
		FrmStart_showCheckMenuAlreadyOpen(pTempStr, 0);

	}
	else
	{
		for(uint8_t i = 1; i <g_Open_Lock_Info.User_Type;i++)
		{
			pTempStr = (uint8_t*)strtok(NULL, "\n");
		}
		FrmStart_showCheckMenuAlreadyOpen(pTempStr, g_Open_Lock_Info.User_ID_Num);
	}
	memset(str_buf, 0x00, 12);
	y = 16;
	x = 44;
	LCD_Open_Icon(pFormCDC, x, y);
	sprintf((char*)str_buf, pptitleStr[1], (g_Wait_Time_Count / 10));
	y = 48;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC, str_buf, 1, y, FONT_16X16);


}

void XmMcu_FrmStart_showLockAlreadyClose(const uint8_t** pptitleStr)
{
	uint8_t x, y;
	y=0;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16); 			
	x=48;
	y=16;
	LCD_Lock_Icon(pFormCDC,x,y);
	y=48;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC,"锁已关闭",1,y,FONT_16X16); 			

}

void XmMcu_FrmStart_showLockAlwaysOpenTip(const uint8_t** pptitleStr)
{
	uint8_t x, y;
	y=1;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
	y=32;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC,"常开模式",1,y,FONT_16X16);
}


void XmMcu_FrmStart_showLockAlwaysICTip(const uint8_t** pptitleStr)
{
	uint8_t x, y;
	y=1;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
	y+=20;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC,"锁已设置常开",1,y,FONT_16X16);
	y+=16;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC,"请按把手开门",1,y,FONT_16X16);
}

void XmMcu_FrmStart_showDoorRingTip(const uint8_t** pptitleStr)
{
	uint8_t x, y;
	y=0;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
	x=24;
	y+=16;
	LCD_Ring_Icon(pFormCDC,(g_RingBells_Times%4), x,  y) ;
	y+=32;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC,"门铃已启动",1,y,FONT_16X16);

}

void XmMcu_FrmStart_showRestoreConfirm(const uint8_t** pptitleStr)
{
	uint8_t x, y;
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
	if(g_Yes_Or_No) 	OLED_YesOrNO(pFormCDC,x,y,1);
	else	OLED_YesOrNO(pFormCDC,x,y,0);

}

void XmMcu_FrmStart_showRestoreBar()
{
	uint8_t x, y;
	x = 14;
	y = 20;
	if(g_restore_bar_value < 100) progress_bar(pFormCDC, g_restore_bar_value, x, y);
	else progress_bar(pFormCDC, 100, x, y);

}


void XmMcu_FrmStart_showErrAlarm(const uint8_t** pptitleStr)
{
	uint8_t x, y;
	unsigned char str_buf[24];

	y=1;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
	y+=16;
	y+=4;
	x=2;
	Err_Alarm_Icon(pFormCDC, x,y);
	y+=2;
	x=+44;
	OLED_ShowMixStr(pFormCDC,x,y,"系统已锁死",1,1,FONT_16X16);
	y+=18;
	sprintf((char *)str_buf,"时间%03d 秒",(g_Wait_Time_Count/10));
	OLED_ShowMixStr(pFormCDC,x,y,str_buf,1,1,FONT_16X16);

}


void XmMcu_FrmStart_showDectMode(const uint8_t** pptitleStr)
{
	uint8_t x, y;
	x=8;
	y=14;
	Lower_Power_Icon( pFormCDC, x, y);
	x+=32;
	OLED_ShowMixStr(pFormCDC,x,y,"电池电量低",1,1,FONT_16X16);
	y+=18;
	OLED_ShowMixStr(pFormCDC,x,y,"请更换电池",1,1,FONT_16X16);

}

void XmMcu_FrmStart_showDoubleCheckTips(const uint8_t** pptitleStr)
{
	uint8_t x, y;
	y = 1;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC, "系统提示", 1, y, FONT_16X16);
	y = 20;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC, "同一个用户", 1, y, FONT_16X16);
	y += 16;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC, "请重新验证", 1, y, FONT_16X16);
}



void System_SetSuccess_Tip()
{
	unsigned char y;
	y=1;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
	y=32;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC,"设置成功",1,y,FONT_16X16);
}


void System_SetFail_Tip()
{
	unsigned char y;
	y=1;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
	y=32;
	OLED_HZ_ShowMixStr_XCentre(pFormCDC,"设置失败",1,y,FONT_16X16);
}

extern void XmMcu_MainForm_enter(void);
extern void XmMcu_MainForm_exit(void);

extern void XmMcu_MainForm_load(uint8_t menuState);

extern void XmMcu_MainForm_task(void* pExtendInputData,uint8_t menuState, MenuShow_s* pstShowInfo);

/* 主菜单 */
MenuList_s sg_MainMenuTable[] = 
{
    {{"主菜单", "main menu"},  XmMcu_MainForm_enter, XmMcu_MainForm_exit, XmMcu_MainForm_load, XmMcu_MainForm_task, NULL},
};

void XmMcu_FrmStart_enter(void)
{
	//加载下一页的
	cotMenu_Bind(sg_MainMenuTable, GET_MENU_NUM(sg_MainMenuTable), XmMcu_FrmStart_onShow,g_aaapTitleStr);
	
	cotMenu_changeMenuState(StartStates);
	XmMcu_StartFormKey_clearKeyNumber();
	LCD_ON();
}

void XmMcu_FrmStart_exit(void)
{

}


void XmMcu_FrmStart_load(uint8_t menuState)
{
	unsigned char x, y;
	unsigned char i;
	unsigned char start_num = 0;

	switch(menuState)
	{
		// FIXME:这段放到enter
		case StartStates:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, STARTFORM_20_TICK_200MS, 0);
			break;
		case EnterPassword:
		case CheckMenuByPasswd:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 50, 0);
			break;

		case EnterFingerPrint:
		case EnterIC:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, STARTFORM_20_TICK_200MS, 0);
			break;
		case CheckMenu:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, STARTFORM_20_TICK_200MS, 0);
			break;
		case PassWordNG:
		case FingerPrintNG:
		case ICCard_Fail:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, STARTFORM_20_TICK_2000MS, 1);
			PLAY_MUSIC(37);
			LED_FLASHES(3);	
			break;
		case PassWordLoginFail:			//验证失败
		case FingerPrintLoginFail:		//验证失败
		case ICCardLoginFail:			//验证失败
			/*播放音频2秒后跳转*/
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, STARTFORM_20_TICK_2000MS, 1);
			PLAY_MUSIC(37);
			LED_FLASHES(3);
			break;
		case LockAlreadyOpen:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, STARTFORM_20_TICK_200MS, 0);
			g_Wake_Time_Count = 0;
			PLAY_MUSIC(39);  //锁已开启请按把手
			g_Wait_Time_Count = 50;		//打开的时间
			RUN_MOTOR();
//			if(g_System_Para_Info.always_open_flag)
//			{
//				g_System_Para_Info.always_open_flag = 0;
//				System_Para_Save();
//			}
//			memset(&Input_Err_Record, 0x00, sizeof(InputErrTypeDef));
			break;
		case LockAlreadyClose:   //已经关闭
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, STARTFORM_20_TICK_2000MS, 1);
			REVERS_MOTOR();
			break;		
		case LockAlwaysOpenTip:   //已经关闭
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, STARTFORM_20_TICK_2000MS, 1);
			PLAY_MUSIC(38);  //门已设置常开
			break;
		case LockAlwaysICTip:   //已经关闭	
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, STARTFORM_20_TICK_2000MS, 1);
			PLAY_MUSIC(12);  //门已设置常开
			break;

		case DoorRingTip:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 30,1);
			PLAY_MUSIC(MenLing);  //门已设置常开
			g_RingBells_Times = 0;
			break;
		case RestoreBar:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, STARTFORM_20_TICK_200MS, 1);
			g_restore_bar_value = 10;
			break;
		case RestoreSuccess:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, STARTFORM_20_TICK_2000MS, 1);
			break;
		case Err_Alarm:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 50, 1);
			PLAY_BAOJING_MUSIC();
			break;
		case Dect_Mode:
			if(!g_Voltage_Level)
			{
				PLAY_MUSIC(DianLiangBuZu);
				cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, STARTFORM_20_TICK_2000MS, 1);
				LCD_ON();
			}
			else
			{
				cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, STARTFORM_20_TICK_200MS, 1);
			}
		case DoubleCheckTips:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 50, 1);
			PLAY_MUSIC(HuanYongHuYanZheng);
			break;
		case DoubleCheckErrTips:
			cotMenu_setTickAtLoad(MENU_TICK_TYPE_ONTICK, 50, 1);
			PLAY_MUSIC(TongYiGeYongHu41_30); 
			break;
		default:
			break;
	}
}

void FrmStart_onChar(uint8_t menuState, int keyVal);
void FrmStart_onTick(uint8_t menuState);


void XmMcu_FrmStart_task(void* pExtendInputData, uint8_t menuState, MenuShow_s* pstShowInfo)
{
	int keyVal;
	FrmStart_onChar(menuState, keyVal);
	if(!cotMenu_getTick())
	{
		FrmStart_onTick(menuState);
	}
}


void XmMcu_FrmStart_onShow(MenuShow_s *pstShowInfo,uint8_t menuState)
{
	unsigned char x, y;
	unsigned char i;
	unsigned char start_num = 0;
	
	const uint8_t* (* paapTitleStr)[1][5];
//	const uint8_t* paapTitleStr;
	paapTitleStr = (const uint8_t* (*)[1][5])pstShowInfo->pTileExData;
	uint8_t chosenLanguage = pstShowInfo->language;
//	switch(menuState)
//	{
//		case StartStates:
//			SHOW_CLOCK_FUNC();
//			break;
//		case EnterPassword:
//			XmMcu_FrmStart_showEnterPassword(paapTitleStr[menuState][chosenLanguage]);
//			break;
//		case EnterFingerPrint:
//			XmMcu_FrmStart_showFingerPrint(paapTitleStr[menuState][chosenLanguage]);
//			break;
//		case EnterIC:
//			break;
//		case CheckMenu:
//			XmMcu_FrmStart_showCheckMenu(paapTitleStr[menuState][chosenLanguage]);
//			break;
//
//		case CheckMenuByPasswd:
//			XmMcu_FrmStart_showCheckMenuByPasswd(paapTitleStr[menuState][chosenLanguage]);
//			break;
//		case PassWordNG:
//		case FingerPrintNG:
//		case ICCard_Fail:
//		case PassWordLoginFail://验证失败
//		case FingerPrintLoginFail://验证失败
//		case ICCardLoginFail://验证失败
//			XmMcu_FrmStart_showCheckFail(paapTitleStr[menuState][chosenLanguage]);
//		case LockAlreadyOpen:    //锁已打开
//			XmMcu_FrmStart_showCheckMenuAlreadyOpen(paapTitleStr[menuState][chosenLanguage]);
//			break;
//		case LockAlreadyClose:   //已经关闭
//			XmMcu_FrmStart_showLockAlreadyClose(paapTitleStr[menuState][chosenLanguage]);
//
//			break;
//		case LockAlwaysOpenTip:
//			XmMcu_FrmStart_showLockAlwaysOpenTip(paapTitleStr[menuState][chosenLanguage]);
//			break;
//		case LockAlwaysICTip:
//			XmMcu_FrmStart_showLockAlwaysICTip(paapTitleStr[menuState][chosenLanguage]);
//			break;
//		case DoorRingTip:
//			XmMcu_FrmStart_showDoorRingTip(paapTitleStr[menuState][chosenLanguage]);
//			break;
//		case RestoreConfirm:
//			XmMcu_FrmStart_showRestoreConfirm(paapTitleStr[menuState][chosenLanguage]);
//			break;
//		case RestoreBar:
//			XmMcu_FrmStart_showRestoreBar(paapTitleStr[menuState][chosenLanguage]);
//			break;
//		case RestoreSuccess:
//			System_SetSuccess_Tip();
//			break;
//		case Err_Alarm:
//			XmMcu_FrmStart_showErrAlarm(paapTitleStr[menuState][chosenLanguage]);
//			break;
//		case RestoreFail:
//			System_SetFail_Tip();
//			break;
//		case Dect_Mode:
//			if(!g_Voltage_Level)
//			{
//				XmMcu_FrmStart_showDectMode(paapTitleStr[menuState][chosenLanguage]);
//			}
//			break;
//		case DoubleCheckTips:
//		case DoubleCheckErrTips:
//			XmMcu_FrmStart_showDoubleCheckTips(paapTitleStr[menuState][chosenLanguage]);
//			break;
//		default :
//			break;
//	}
}


/*有按键按下*/
void FrmStart_onChar(uint8_t menuState, int keyVal)
{
	switch(menuState)
	{
		case StartStates:   //开机状态
			/*第一次触摸唤醒,一些特殊处理*/
//			Input_Type = PasswdMode;
			PLAY_MUSIC_ON(DI);
			switch(keyVal)
			{
				case  12: //# 门铃提示
					cotMenu_changeMenuState(DoorRingTip);
					break;
				case 10: //* 菜单界面
					if(XmMcu_StartForm_LoginIsFactory())
					{
						cotMenu_Enter();	//显示菜单界面
					}
					else
					{
						cotMenu_changeMenuState(CheckMenu);	
					}
					break;
				default:	//输入密码
					if(XmMcu_Lock_getDoorIsOpenAlways())
					{
						cotMenu_changeMenuState(LockAlwaysOpenTip);
					}
					else
					{
						/*密码输入记录*/
						XmMcu_StartFormKey_startKeyNumber(keyVal);
						cotMenu_changeMenuState(EnterPassword);
					}
					break;//进入密码开锁
			}
			break;
		case EnterPassword:	//输入密码
			PLAY_MUSIC_ON(DI);	//按键音
			switch(keyVal)
			{
				case 12:   //确认
					/*根据不同的键盘码分类*/
					XmMcu_StartForm_keyCodeSwitch();
					break;
				case 10:  //取消
					XmMcu_StartFormKey_clearKeyNumber();
					cotMenu_changeMenuState(StartStates);
					break;
				default:
					XmMcu_StartFormKey_setKeyNumber(keyVal);
					break;
			}
			break;
		case EnterFingerPrint:
//			Input_Type = PasswdMode;
			PLAY_MUSIC_ON(DI);
			switch(keyVal)
			{
				case 12:
					cotMenu_changeMenuState(DoorRingTip);
					break;
				case 10:
					cotMenu_changeMenuState(StartStates);
					break;
				default:
					XmMcu_StartFormKey_startKeyNumber(keyVal);
					break;//进入密码开锁
			}
			break;
		case EnterIC:
			break;
		case CheckMenu:         //验证界面
			PLAY_MUSIC_ON(DI);
			switch(keyVal)
			{
				case 10:  //取消
					XmMcu_StartFormKey_clearKeyNumber();
					cotMenu_changeMenuState(StartStates);
					break;
				default:
					XmMcu_StartFormKey_startKeyNumber(keyVal);
					cotMenu_changeMenuState(CheckMenuByPasswd);
					break;
			}
			break;
		case CheckMenuByPasswd:
			PLAY_MUSIC_ON(DI);
			switch(keyVal)
			{
				case 12:   //确认
					XmMcu_StartForm_checkPassWd();
					break;
				case 10:  //取消
					XmMcu_StartFormKey_clearKeyNumber();
					cotMenu_changeMenuState(StartStates);
					break;
				default:
					XmMcu_StartFormKey_setKeyNumber(keyVal);
					break;
			}
			break;

		case PassWordNG:        //验证失败
			break;
		case FingerPrintNG:     //验证失败
			break;
		case ICCard_Fail:        //验证失败
			break;
		case LockAlreadyOpen:    //锁已打开
			break;
		case WaitLockClose:      //等待关闭
			break;
		case LockAlreadyClose:  //已经关闭
			break;
		case LockAlwaysOpenTip:
			break;
		case RestoreConfirm:
			PLAY_MUSIC(DI);
			switch(keyVal)
			{
				case 4:  //确认
					g_Yes_Or_No = 1;
					break;
				case 6:  //否
					g_Yes_Or_No = 0;
					break;
				case 12:
					if(g_Yes_Or_No)
					{
						g_Yes_Or_No = 0;
						SYSTEM2FACTORY();	//系统设置参数恢复
						CLEAN_USER_ALL();	//用户清空
						cotMenu_changeMenuState(RestoreBar);
					}
					else
					{
						cotMenu_changeMenuState(StartStates);
					}
					break;
				case 10:
					cotMenu_changeMenuState(StartStates);
					break;
				default:
					break;
			}
			break;
		case RestoreBar:
			break;
		case RestoreSuccess:
			break;
		case RestoreFail:
			break;
		case Err_Alarm:
			break;
		case Dect_Mode:
			TOUCH_LED_ENABLE();
			PLAY_MUSIC(KeyWake);
			cotMenu_changeMenuState(StartStates);
			LCD_ON();
			break;
		case DoubleCheckTips:
			break;
		case DoubleCheckErrTips:
			break;
		default:
			break;
	}
}


void FrmStart_onTick(uint8_t menuState)
{
	XmMcu_timeoutReturnMainMenu(menuState);
	switch(menuState)
	{
		case StartStates:
			XmMcu_openDoorScan(3);
			break;
		case EnterPassword:
			XmMcu_openDoorScan(0);
			break;
		case EnterFingerPrint:
			XmMcu_openDoorScan(1);
			break;
		case EnterIC:
			break;
		case CheckMenu:
			XmMcu_StartForm_checkMenuScanIcAndFinger();
			break;
		case CheckMenuByPasswd:
			XmMcu_LoginMenuScan();
			break;
		case PassWordNG:
			/*3次错误则报警*/
			if(XmMcu_SaveAlarmLog(PASSWD_ALARM))
			{
				cotMenu_changeMenuState(Err_Alarm);
			}
			else
			{
				cotMenu_changeMenuState(StartStates);
			}
			break;
		case FingerPrintNG:
			if(XmMcu_SaveAlarmLog(FINGER_ALARM))
			{
				cotMenu_changeMenuState(Err_Alarm);
			}
			else
			{
				cotMenu_changeMenuState(EnterFingerPrint);
			}
			break;
		case ICCard_Fail:
			if(XmMcu_SaveAlarmLog(RFID_ALARM))
			{
				cotMenu_changeMenuState(Err_Alarm);
			}
			else
			{
				cotMenu_changeMenuState(StartStates);
			}
			break;

		case PassWordLoginFail:        //验证失败
			if(XmMcu_SaveAlarmLog(PASSWD_ALARM))
			{
				cotMenu_changeMenuState(Err_Alarm);
			}
			else
			{
				cotMenu_changeMenuState(CheckMenu);
			}
			break;
		case FingerPrintLoginFail:         //验证失败
			if(XmMcu_SaveAlarmLog(FINGER_ALARM))
			{
				cotMenu_changeMenuState(Err_Alarm);
			}
			else
			{
				cotMenu_changeMenuState(CheckMenu);
			}
			break;
		case ICCardLoginFail:         //验证失败
			if(XmMcu_SaveAlarmLog(RFID_ALARM))
			{
				cotMenu_changeMenuState(Err_Alarm);
			}
			else
			{
				cotMenu_changeMenuState(CheckMenu);
			}
			break;
		case LockAlreadyOpen:
			if(!g_Wait_Time_Count)
			{
				cotMenu_changeMenuState(LockAlreadyClose);
			}
			break;
		case LockAlreadyClose:

			cotMenu_changeMenuState(StartStates);

			break;
		case LockAlwaysOpenTip:
			XmMcu_StartForm_changeStateByInpuType(LockAlwaysOpenTip);

			break;
		case LockAlwaysICTip:
			cotMenu_changeMenuState(StartStates);
			break;
		case DoorRingTip:
			g_RingBells_Times++;	//门铃动画计数
			if(g_RingBells_Times > 11)
			{
				cotMenu_changeMenuState(StartStates);
			}
			break;
		case RestoreConfirm:
			break;
		case RestoreBar:
			g_restore_bar_value += 2;
			g_Wake_Time_Count = 0;
			if(g_restore_bar_value > 120)
			{
				cotMenu_changeMenuState(RestoreSuccess);
			}
			break;
		case RestoreSuccess:
			cotMenu_changeMenuState(StartStates);
			break;
		case RestoreFail:
			break;
		case Err_Alarm:
			g_Wake_Time_Count = 0;		//触发睡眠
			if(!g_Wait_Time_Count)		//锁死等待时间
			{
				cotMenu_changeMenuState(StartStates);
			}
			break;
		case Dect_Mode:
			XmMcu_StartForm_changeStateByInpuType(Dect_Mode);

			break;
		case DoubleCheckTips:
			cotMenu_changeMenuState(EnterFingerPrint);
			break;
		case DoubleCheckErrTips:
			cotMenu_changeMenuState(EnterFingerPrint);
			break;
		default:
			break;
	}
	XmMcu_FrmStart_resetKeyCheck();
}






