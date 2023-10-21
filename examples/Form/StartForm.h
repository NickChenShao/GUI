#ifndef __StartForm_H
#define __StartForm_H

#define OPENINGTIME     20  //开锁时电机转动时间
#define OPENTIME		6000  //ms,锁开启的时间，
#define CLOSINGTIME     10  //关锁是电机转动时间

#include "mainhmi.h"


#include <time.h>


typedef  struct
{
	uint8_t InputState;// 第0位==1,密码已输入；第1位==1指纹已输入
	uint8_t FirstFinger; //输入的第一枚指纹
	uint8_t SecendFinger;//输入的第二枚指纹
} OpenLockInputStateDef;

//主界面上的所有界面
typedef enum
{
	StartStates=1,      //开机状态
	EnterPassword,      //输入密码
	EnterFingerPrint,   //输入指纹
	EnterIC,
	CheckMenu,          //验证界面
	CheckMenuByPasswd,
	PassWordNG,         //验证失败
	FingerPrintNG,         //验证失败
	ICCard_Fail,         //验证失败
	PassWordLoginFail,   //验证失败
	FingerPrintLoginFail,//验证失败
	ICCardLoginFail,     //验证失败
	LockAlreadyOpen,    //锁已打开
	WaitLockClose,      //等待关闭
	LockAlreadyClose,   //已经关闭
	LockAlwaysOpenTip,  //常开模式提示
	LockAlwaysICTip,
	DoorRingTip,
	RestoreConfirm,
	RestoreBar,
	RestoreSuccess,
	RestoreFail,
	Err_Alarm,
	Dect_Mode,
	DoubleCheckTips,
	DoubleCheckErrTips,
	SelectModeTypeDefMax,
}SelectMode_TypeDef;

typedef  struct
{
	uint8_t passwd_error;// 第0位==1,密码已输入；第1位==1指纹已输入
	uint8_t finger_error; //输入的第一枚指纹
	uint8_t iccard_error;//输入的第二枚指纹
} InputErrTypeDef;

void XmMcu_FrmStart_enter(void);
void XmMcu_FrmStart_exit(void);
void XmMcu_FrmStart_load(uint8_t menuState);
void XmMcu_FrmStart_task(void* pExtendInputData, uint8_t menuState, MenuShow_s* pstShowInfo);
void XmMcu_FrmStart_onShow(MenuShow_s *pstShowInfo,uint8_t menuState);

#endif


