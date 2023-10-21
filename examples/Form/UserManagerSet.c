#if 1
#include "MenuForm.h"
#include "StartForm.h"
#include "CDC.h"
#include <string.h>
#include <stdio.h>
#include "UserManagerSet.h"
#include "peripheral_fun_share.h"
#include "GUI_Form_Fun.h"
#include "Form.h"


RegistKeyStateDef RegistPasswdState;
RegistICCardStateDef RegistICCardState;
RegFingerStateDef RegistFingerState;

unsigned char DeleteState;
unsigned char g_Add_Passwd_Type;
unsigned char g_Add_Finger_Type;
unsigned char g_Add_ICCard_Type;

unsigned char g_Delete_Mode_Type;
unsigned char g_Delete_By_SN;
unsigned char g_Delete_By_Verify;

unsigned char g_FingerPrint_Acq_Times;

InputBuf_TypeDef Input_Key_Buf,Input_Key_Buf_Temp;

static unsigned char g_UserManager_Pri_Display;


PassWord_TypeDef PassWord_Buff;  //用于记录密码用户操作
FingerPrint_TypeDef g_FingerPrint_Buff;
ICCard_TypeDef  g_ICCard_Buff;


/*************************************************/
uint8_t UserManagerStr[] ={ "用户管理"};

void UserManagerSet_onCreate(BASEFORM* pThis);
void UserManagerSet_onShow(BASEFORM* pThis,CDC* pFormCDC);
void UserManagerSet_onChar(BASEFORM* pThis,int keyVal);
void UserManagerSet_onTick(BASEFORM* pThis);

const FORM_HANDLERS s_formcode_UserManagerSet={
		sizeof(MENUFORM),
		UserManagerStr,
    fns_NullForm_onDefault,
    UserManagerSet_onShow,
    UserManagerSet_onChar,
    UserManagerSet_onTick,
		UserManagerSet_onCreate,
    fns_NullForm_onDestroy,
    fns_NullForm_onFocus	
};



void UserManagerSet_onShow(BASEFORM* pThis,CDC* pFormCDC){
    uint8_t x,y;
		if(pThis->handlers->titleId){
				y=1;
				x=4;
				Dir2_Logo_Icon(pFormCDC,x,y);
				x=108;
				Dir8_Logo_Icon(pFormCDC,x,y);
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,pThis->handlers->titleId,1,y,FONT_16X16);
		}
    fns_MenuForm_onShow(pThis,pFormCDC);
}

void UserManagerSet_onChar(BASEFORM* pThis,int keyVal){
    fns_MenuForm_onChar(pThis,keyVal);
}

void UserManagerSet_onTick(BASEFORM* pThis){
    pThis->timeTick=1;
		if(g_Wake_Time_Count>USER_MANAGER_DELAY_TIME){
					FNS_ALL_FORM_CLOSE();
					g_Wake_Time_Count=0;
					fns_Form_redraw();
		}
    fns_Form_redraw();
}


void RegistAdminUser(MENUFORM* pMenuForm);
void RegistNormalUser(MENUFORM* pMenuForm);
void RegistGuestUser(MENUFORM* pMenuForm);
//void RegistThreatUser(MENUFORM* pMenuForm);
void DeleteUser(MENUFORM* pMenuForm);


const MENUITEM s_UserManagerSetMenu[]={
		{(uint8_t*)"添加管理",RegistAdminUser,NULL}, 
		{(uint8_t*)"添加用户",RegistNormalUser,NULL}, 
//		{(uint8_t*)"添加宾客",RegistGuestUser,NULL}, 
//		{(uint8_t*)"添加胁迫",RegistThreatUser,NULL},
		{(uint8_t*)"删除用户",DeleteUser,NULL},
  
};

void UserManagerSet_onCreate(BASEFORM* pThis){   
    MENUFORM* pMenuForm=(MENUFORM*)pThis;
    pMenuForm->menuItems=s_UserManagerSetMenu;
    pMenuForm->itemCount=sizeof(s_UserManagerSetMenu)/sizeof(MENUITEM);
    pMenuForm->firstIndex=0;
		pMenuForm->curIndex=0;
    pMenuForm->timeTick=2;
	g_Wake_Time_Count=0;
}
/*******************************************************************/

uint8_t RegistAdminUserStr[]={"添加管理"};

void RegistAdminUser_onShow(BASEFORM* pThis,CDC* pFormCDC);
void RegistAdminUser_onChar(BASEFORM* pThis,int keyVal);
void RegistAdminUser_onTick(BASEFORM* pThis);
void RegistAdminUser_onCreate(BASEFORM* pThis);
const FORM_HANDLERS s_formcode_RegistAdminUser={
		sizeof(MENUFORM),
		RegistAdminUserStr,
		fns_NullForm_onDefault,
		RegistAdminUser_onShow,
		RegistAdminUser_onChar,
		RegistAdminUser_onTick,
		RegistAdminUser_onCreate,
		fns_NullForm_onDestroy,
		fns_NullForm_onFocus	
};
void RegistAdminUser(MENUFORM* pMenuForm)
{
 fns_Form_create(&s_formcode_RegistAdminUser);
}


void RegistAdminUser_onShow(BASEFORM* pThis,CDC* pFormCDC)
{
		uint8_t x,y;
		if(pThis->handlers->titleId)
		{
				y=1;
				x=4;
				Dir2_Logo_Icon(pFormCDC,x,y);
				x=108;
				Dir8_Logo_Icon(pFormCDC,x,y);
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,pThis->handlers->titleId,1,y,FONT_16X16);
		}
		fns_MenuForm_onShow(pThis,pFormCDC);
}

void RegistAdminUser_onChar(BASEFORM* pThis,int keyVal)
{
    fns_MenuForm_onChar(pThis,keyVal);

}

void RegistAdminUser_onTick(BASEFORM* pThis)
{
    pThis->timeTick=2;
		if(g_Wake_Time_Count>USER_MANAGER_DELAY_TIME){
					FNS_ALL_FORM_CLOSE();
					g_Wake_Time_Count=0;
					fns_Form_redraw();
		}
    fns_Form_redraw();
}


void RegistAdminFingerPrintUser(MENUFORM* pMenuForm);
void RegistAdminPassWordUser(MENUFORM* pMenuForm);
void RegistAdminICUser(MENUFORM* pMenuForm);


const MENUITEM s_AdminUserAddMenu[]={
		{(uint8_t*)"添加指纹",RegistAdminFingerPrintUser,NULL}, 
		{(uint8_t*)"添加密码",RegistAdminPassWordUser,NULL}, 
		{(uint8_t*)"添加IC卡",RegistAdminICUser,NULL}, 
};

void RegistAdminUser_onCreate(BASEFORM* pThis)
{

    MENUFORM* pMenuForm=(MENUFORM*)pThis;
    pMenuForm->menuItems=s_AdminUserAddMenu;
    pMenuForm->itemCount=sizeof(s_AdminUserAddMenu)/sizeof(MENUITEM);
    pMenuForm->firstIndex=0;
		pMenuForm->curIndex=0;
    pMenuForm->timeTick=2;
	g_Wake_Time_Count=0;
}


/*********************************************************/

uint8_t RegistNormalUserStr[]={"添加用户"};

void RegistNormalUser_onShow(BASEFORM* pThis,CDC* pFormCDC);
void RegistNormalUser_onChar(BASEFORM* pThis,int keyVal);
void RegistNormalUser_onTick(BASEFORM* pThis);
void RegistNormalUser_onCreate(BASEFORM* pThis);
const FORM_HANDLERS s_formcode_RegistNormalUser={
		sizeof(MENUFORM),
		RegistNormalUserStr,
		fns_NullForm_onDefault,
		RegistNormalUser_onShow,
		RegistNormalUser_onChar,
		RegistNormalUser_onTick,
		RegistNormalUser_onCreate,
		fns_NullForm_onDestroy,
		fns_NullForm_onFocus	
};
void RegistNormalUser(MENUFORM* pMenuForm)
{
		fns_Form_create(&s_formcode_RegistNormalUser);

}

void RegistNormalUser_onShow(BASEFORM* pThis,CDC* pFormCDC)
{
		uint8_t x,y;
		if(pThis->handlers->titleId)
		{
				y=1;
				x=4;
				Dir2_Logo_Icon(pFormCDC,x,y);
				x=108;
				Dir8_Logo_Icon(pFormCDC,x,y);
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,pThis->handlers->titleId,1,y,FONT_16X16);
		}
		fns_MenuForm_onShow(pThis,pFormCDC);
}
void RegistNormalUser_onChar(BASEFORM* pThis,int keyVal)
{
    fns_MenuForm_onChar(pThis,keyVal);

}
void RegistNormalUser_onTick(BASEFORM* pThis)
{
    pThis->timeTick=2;
		if(g_Wake_Time_Count>USER_MANAGER_DELAY_TIME){
					FNS_ALL_FORM_CLOSE();
					g_Wake_Time_Count=0;
					fns_Form_redraw();
		}
    fns_Form_redraw();
}


void RegistNormalFingerPrintUser(MENUFORM* pMenuForm);
void RegistNormalPassWordUser(MENUFORM* pMenuForm);
void RegistNormalICUser(MENUFORM* pMenuForm);





const MENUITEM s_NormalUserAddMenu[]={
		{(uint8_t*)"添加指纹",RegistNormalFingerPrintUser,NULL}, 
		{(uint8_t*)"添加密码",RegistNormalPassWordUser,NULL}, 
		{(uint8_t*)"添加IC卡",RegistNormalICUser,NULL}, 
};

void RegistNormalUser_onCreate(BASEFORM* pThis)
{

    MENUFORM* pMenuForm=(MENUFORM*)pThis;
    pMenuForm->menuItems=s_NormalUserAddMenu;
    pMenuForm->itemCount=sizeof(s_NormalUserAddMenu)/sizeof(MENUITEM);
    pMenuForm->firstIndex=0;
		pMenuForm->curIndex=0;
    pMenuForm->timeTick=2;
	g_Wake_Time_Count=0;
}


/******************************************************************/


uint8_t RegistGuestUserStr[]={"添加宾客"};

void RegistGuestUser_onShow(BASEFORM* pThis,CDC* pFormCDC);
void RegistGuestUser_onChar(BASEFORM* pThis,int keyVal);
void RegistGuestUser_onTick(BASEFORM* pThis);
void RegistGuestUser_onCreate(BASEFORM* pThis);
const FORM_HANDLERS s_formcode_RegistGuestUser={
		sizeof(MENUFORM),
		RegistGuestUserStr,
		fns_NullForm_onDefault,
		RegistGuestUser_onShow,
		RegistGuestUser_onChar,
		RegistGuestUser_onTick,
		RegistGuestUser_onCreate,
		fns_NullForm_onDestroy,
		fns_NullForm_onFocus	
};
void RegistGuestUser(MENUFORM* pMenuForm)
{
		fns_Form_create(&s_formcode_RegistGuestUser);

}

void RegistGuestUser_onShow(BASEFORM* pThis,CDC* pFormCDC)
{
		uint8_t x,y;
		if(pThis->handlers->titleId)
		{
				y=1;
				x=4;
				Dir2_Logo_Icon(pFormCDC,x,y);
				x=108;
				Dir8_Logo_Icon(pFormCDC,x,y);
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,pThis->handlers->titleId,1,y,FONT_16X16);
		}
		fns_MenuForm_onShow(pThis,pFormCDC);
}
void RegistGuestUser_onChar(BASEFORM* pThis,int keyVal)
{
    fns_MenuForm_onChar(pThis,keyVal);

}
void RegistGuestUser_onTick(BASEFORM* pThis)
{
    pThis->timeTick=2;
	if(g_Wake_Time_Count>USER_MANAGER_DELAY_TIME){
					FNS_ALL_FORM_CLOSE();
					g_Wake_Time_Count=0;
					fns_Form_redraw();
		}
    fns_Form_redraw();
}






void RegistGuestFingerPrintUser(MENUFORM* pMenuForm);
void RegistGuestPassWordUser(MENUFORM* pMenuForm);
void RegistGuestICUser(MENUFORM* pMenuForm);


const MENUITEM s_GuestUserAddMenu[]={
		{(uint8_t*)"添加指纹",RegistGuestFingerPrintUser,NULL}, 
		{(uint8_t*)"添加密码",RegistGuestPassWordUser,NULL}, 
		{(uint8_t*)"添加IC卡",RegistGuestICUser,NULL}, 
};

void RegistGuestUser_onCreate(BASEFORM* pThis)
{

    MENUFORM* pMenuForm=(MENUFORM*)pThis;
    pMenuForm->menuItems=s_GuestUserAddMenu;
    pMenuForm->itemCount=sizeof(s_GuestUserAddMenu)/sizeof(MENUITEM);
    pMenuForm->firstIndex=0;
		pMenuForm->curIndex=0;
    pMenuForm->timeTick=2;
	g_Wake_Time_Count=0;
}


/*********************************************/

//uint8_t RegistThreatUserStr[]={"添加胁迫"};

//void RegistThreatUser_onShow(BASEFORM* pThis,CDC* pFormCDC);
//void RegistThreatUser_onChar(BASEFORM* pThis,int keyVal);
//void RegistThreatUser_onTick(BASEFORM* pThis);
//void RegistThreatUser_onCreate(BASEFORM* pThis);
//const FORM_HANDLERS s_formcode_RegistThreatUser={
//		sizeof(MENUFORM),
//		RegistThreatUserStr,
//		fns_NullForm_onDefault,
//		RegistThreatUser_onShow,
//		RegistThreatUser_onChar,
//		RegistThreatUser_onTick,
//		RegistThreatUser_onCreate,
//		fns_NullForm_onDestroy,
//		fns_NullForm_onFocus	
//};
//void RegistThreatUser(MENUFORM* pMenuForm)
//{
// fns_Form_create(&s_formcode_RegistThreatUser);

//}

//void RegistThreatUser_onShow(BASEFORM* pThis,CDC* pFormCDC)
//{
//		uint8_t y;
//		if(pThis->handlers->titleId)
//		{
//				y=1;
//				OLED_HZ_ShowMixStr_XCentre(pFormCDC,pThis->handlers->titleId,1,y,FONT_16X16);
//		}
//		fns_MenuForm_onShow(pThis,pFormCDC);
//}
//void RegistThreatUser_onChar(BASEFORM* pThis,int keyVal)
//{
//    fns_MenuForm_onChar(pThis,keyVal);

//}
//void RegistThreatUser_onTick(BASEFORM* pThis)
//{
//    pThis->timeTick=2;
//    fns_Form_redraw();
//}




//void RegistThreatFingerPrintUser(MENUFORM* pMenuForm);
//void RegistThreatPassWordUser(MENUFORM* pMenuForm);
//void RegistThreatICUser(MENUFORM* pMenuForm);


//const MENUITEM s_ThreatUserAddMenu[]={
//		{(uint8_t*)"添加指纹",RegistThreatFingerPrintUser,NULL}, 
//		{(uint8_t*)"添加密码",RegistThreatPassWordUser,NULL}, 
//		{(uint8_t*)"添加IC卡",RegistThreatICUser,NULL}, 
//};

//void RegistThreatUser_onCreate(BASEFORM* pThis)
//{
//    MENUFORM* pMenuForm=(MENUFORM*)pThis;
//    pMenuForm->menuItems=s_ThreatUserAddMenu;
//    pMenuForm->itemCount=sizeof(s_ThreatUserAddMenu)/sizeof(MENUITEM);
//    pMenuForm->firstIndex=0;
//		pMenuForm->curIndex=0;
//    pMenuForm->timeTick=2;
//}



/*********************************************/




/***********************************************************************/


uint8_t RegistFingerPrintStr[]={"添加指纹"};

unsigned char g_finger_register_ret;

void RegistFingerPrint_onShow(BASEFORM* pThis,CDC* pFormCDC);
void RegistFingerPrint_onChar(BASEFORM* pThis,int keyVal);
void RegistFingerPrint_onTick(BASEFORM* pThis);
void RegistFingerPrint_onCreate(BASEFORM* pThis);
const FORM_HANDLERS s_formcode_RegistFingerPrint={
		sizeof(MENUFORM),
		RegistFingerPrintStr,
    fns_NullForm_onDefault,
    RegistFingerPrint_onShow,
    RegistFingerPrint_onChar,
    RegistFingerPrint_onTick,
		RegistFingerPrint_onCreate,
    fns_NullForm_onDestroy,
    fns_NullForm_onFocus	
};




void RegistAdminFingerPrintUser(MENUFORM* pMenuForm)
{
	g_Add_Finger_Type=Admin_User;
		fns_Form_create(&s_formcode_RegistFingerPrint);
}

void RegistNormalFingerPrintUser(MENUFORM* pMenuForm)
{
		g_Add_Finger_Type=Normal_User;

		fns_Form_create(&s_formcode_RegistFingerPrint);
}
void RegistGuestFingerPrintUser(MENUFORM* pMenuForm)
{
		g_Add_Finger_Type=Guest_User;
		fns_Form_create(&s_formcode_RegistFingerPrint);
}

void RegistFingerPrint_onShow(BASEFORM* pThis,CDC* pFormCDC)
{
	unsigned short x,y;
	
	switch(RegistFingerState)
	{
		case RegistFinger:   //输入指纹
		y=4;
		x=4;
		Finger_Collect_Icon(pFormCDC,0,x,y);
		x=56;
		y=1;
		if(Admin_User==g_Add_Finger_Type){
				OLED_ShowMixStr(pFormCDC,x,y,"添加管理",1,1,FONT_16X16);
		}
		else if(Normal_User==g_Add_Finger_Type){
				OLED_ShowMixStr(pFormCDC,x,y,"添加用户",1,1,FONT_16X16);
		}
		else if(Guest_User==g_Add_Finger_Type){
				OLED_ShowMixStr(pFormCDC,x,y,"添加宾客",1,1,FONT_16X16);
		}
		y+=20;
		x=64;
		OLED_ShowMixStr(pFormCDC,x,y,"  0%",1,1,FONT_24X22);
		y+=26;
		x=56;
		OLED_ShowMixStr(pFormCDC,x,y,"请放手指",1,1,FONT_16X16);
		break;
		case Raise_Finger:
		case ReRegistFinger:  //重新输入指纹
			y=4;
			x=4;
			if(2==g_FingerPrint_Acq_Times)
				Finger_Collect_Icon(pFormCDC,1,x,y);
			else if(3==g_FingerPrint_Acq_Times)
				Finger_Collect_Icon(pFormCDC,2,x,y);
			else if(4==g_FingerPrint_Acq_Times)
				Finger_Collect_Icon(pFormCDC,3,x,y);
			else if(5==g_FingerPrint_Acq_Times)
				Finger_Collect_Icon(pFormCDC,4,x,y);
			x=56;
			y=1;
			if(Admin_User==g_Add_Finger_Type){
					OLED_ShowMixStr(pFormCDC,x,y,"添加管理",1,1,FONT_16X16);
			}
			else if(Normal_User==g_Add_Finger_Type){
					OLED_ShowMixStr(pFormCDC,x,y,"添加用户",1,1,FONT_16X16);
			}
			else if(Guest_User==g_Add_Finger_Type){
					OLED_ShowMixStr(pFormCDC,x,y,"添加宾客",1,1,FONT_16X16);
			}
			y+=20;
			x=64;
			if(2==g_FingerPrint_Acq_Times)
					OLED_ShowMixStr(pFormCDC,x,y," 25%",1,1,FONT_24X22);
			else if(3==g_FingerPrint_Acq_Times)
					OLED_ShowMixStr(pFormCDC,x,y," 50%",1,1,FONT_24X22);
			else if(4==g_FingerPrint_Acq_Times)
					OLED_ShowMixStr(pFormCDC,x,y," 75%",1,1,FONT_24X22);
			else if(5==g_FingerPrint_Acq_Times)
					OLED_ShowMixStr(pFormCDC,x,y,"100%",1,1,FONT_24X22);
			y+=26;
			x=56;
			OLED_ShowMixStr(pFormCDC,x,y,"请放手指",1,1,FONT_16X16);

			if(5==g_FingerPrint_Acq_Times){
					pThis->timeTick=5;
					if(g_finger_register_ret){
							RegistFingerState=RegistFingerSuccess;  //指纹添加成功
					}
					else{
							RegistFingerState=RegistFingerFail;  //指纹添加成功
					}
			}							
			break;
		case 	RegistFingerSuccess:	  //指纹登记成功
			y=1;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);		
			y=32;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"指纹添加成功",1,y,FONT_16X16);			
			break;
		case	RegistFingerFail:      //指纹登记失败
			y=1;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);		
			y=32;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"指纹登记失败",1,y,FONT_16X16);			
			break;
		case FingerFull:          // 指纹已满
			y=1;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);		
			y=32;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"指纹用户已满",1,y,FONT_16X16);			
			break;
		case RegistFingerNoAdmin:
			y=1;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);		
			y+=20;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"为了您的安全",1,y,FONT_16X16);	
			y+=18;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"请先添加管理员",1,y,FONT_16X16);	
			break;
		default:
			break;	
	}		
}


void RegistFingerPrint_onChar(BASEFORM* pThis,int keyVal)
{
	switch(RegistFingerState)
	{
			case DelFingerSuccess:            // 删除成功
			break;
			case DelFingerFail:               // 删除失败
			break;
			case InputFingerError:            // 输入出错
			break;
			case DelFailAdmin:         				// 管理员指纹最后一枚指纹不能删除
			break;
			case RegistFinger:         				// 输入指纹
			switch(keyVal)
			{
					case 10:  						 				//取消键和确认键都是返回
					case 12:
					cotMenu_MainExit();	
					fns_Form_redraw();
					break;
					default:
					break;
			}
			break;
			case ReRegistFinger:      // 重新输入指纹
			switch(keyVal){
					case 10:  						 //取消键和确认键都是返回
					case 12:
					cotMenu_MainExit();	
					fns_Form_redraw();
					break;
					default:
					break;
			}
			break;
			case RegistFingerFailAdmin:        // 请先登记管理员指纹
			break;
			case FingerFull:          // 指纹已满
			break;
			case FingerExists:          // 该指纹已登记
			break;
			case RegistFingerFail:             // 指纹登记失败
			break;
			case RegistFingerSuccess:           // 指纹登记成功
			break;
			case RegistFingerNoAdmin:
				break;
			default:
			break;
	}
}


void RegistFingerPrint_onTick(BASEFORM* pThis)
{
		unsigned char finger_ret;
	  pThis->timeTick=5;
		if(g_Wake_Time_Count>USER_MANAGER_DELAY_TIME){
					FNS_ALL_FORM_CLOSE();
					g_Wake_Time_Count=0;
					fns_Form_redraw();
		}
		switch(RegistFingerState)
		{
			case InputFingerError:     // 输入出错
				if(	g_UserManager_Pri_Display!=InputFingerError){
						g_UserManager_Pri_Display=InputFingerError;
						pThis->timeTick=20;//2秒
						fns_Form_redraw();
				}
				else{
							cotMenu_MainExit();
							fns_Form_redraw();
				}
				break;
			case DelFailAdmin:         // 管理员指纹最后一枚指纹不能删除
				if(	g_UserManager_Pri_Display!=DelFailAdmin){
							g_UserManager_Pri_Display=DelFailAdmin;
							pThis->timeTick=20;//2秒
							fns_Form_redraw();
				}
				else{
							cotMenu_MainExit();
							fns_Form_redraw();
				}
				break;
			case RegistFinger:         // 输入指纹
						if(	g_UserManager_Pri_Display!=RegistFinger){
							g_UserManager_Pri_Display=RegistFinger;
							fns_Form_redraw();
					}
					if((finger_ret = cmd_GetEnrollImage()) == ERR_OK){
							PLAY_MUSIC(7);
							g_Wake_Time_Count=0;
							finger_ret = cmd_GenChar(g_FingerPrint_Acq_Times);
							if(ERR_OK==finger_ret){
								PLAY_MUSIC(125);
								g_FingerPrint_Acq_Times=2;
								RegistFingerState=ReRegistFinger;
								fns_Form_redraw();
							}
							else{
								PLAY_MUSIC(125);
								fns_Form_redraw();
							}
					}
					if(ERR_NOFINGER==finger_ret){
						
					}
					else{
							
					}
			break;
			case ReRegistFinger:       // 重新输入指纹
				if(	g_UserManager_Pri_Display!=ReRegistFinger){
							g_UserManager_Pri_Display=ReRegistFinger;
							fns_Form_redraw();
					}
					if((finger_ret = cmd_GetEnrollImage()) == ERR_OK){
						PLAY_MUSIC(7);
						g_Wake_Time_Count=0;
						finger_ret = cmd_GenChar(g_FingerPrint_Acq_Times);
						
						if((finger_ret != ERR_OK)	&& (finger_ret != ERR_TEMPLATERELATED)){
							fns_Form_redraw();
								break;
						}
						g_FingerPrint_Acq_Times++;
//						if(0){
//								if(cmd_Search(g_FingerPrint_Acq_Times, 0, MODULE_DBSize, &uMatchIndex, &uMatchScore) == ERR_OK){
//									fns_Form_redraw();
//										break;
//								}
//						}
						 if(5==g_FingerPrint_Acq_Times){
								if((finger_ret = cmd_RegModel()) != ERR_OK){
							
									g_finger_register_ret=0;
									fns_Form_redraw();
									break;
								}
								if((finger_ret = cmd_StoreChar(1, g_FingerPrint_Buff.Finger_Index_Table)) != ERR_OK){
								
									fns_Form_redraw();
										break;
								}
								if(Admin_User==g_Add_Finger_Type){
										g_System_Para_Info.restore_factory=1;
										System_Para_Save();
										g_FingerPrint_Buff.User_Type=Admin_User;
								}
								else if(Normal_User==g_Add_Finger_Type){
										g_FingerPrint_Buff.User_Type=Normal_User;
								}
								else if(Guest_User==g_Add_Finger_Type){
										g_FingerPrint_Buff.User_Type=Guest_User;
								}

								g_finger_register_ret=1;
								Finger_save(&g_FingerPrint_Buff,g_FingerPrint_Buff.Finger_Index_Table);
								OperateLog_Info.clock_sec=Clock2Time(&clock_env);
								OperateLog_Info.operatetype=ADD_USER;
								OperateLog_Info.user_num=g_FingerPrint_Buff.Finger_ID_Num;
								OperateLog_Info.user_type=FingerPrintMode;
								save_OperateLog(&OperateLog_Info);
								fns_Form_redraw();
						}
						else{
							PLAY_MUSIC(125);
							fns_Form_redraw();
						}
					}
					if(ERR_NOFINGER==finger_ret){
					     
					}
					else{
						
					}
			break;

			case RegistFingerFailAdmin:        // 请先登记管理员指纹
				if(	g_UserManager_Pri_Display!=RegistFingerFailAdmin){
							g_UserManager_Pri_Display=RegistFingerFailAdmin;
							pThis->timeTick=20;//2秒
							fns_Form_redraw();
					}
				else{
							cotMenu_MainExit();
							fns_Form_redraw();
				}
			break;
			case FingerFull:           // 指纹已满
					if(	g_UserManager_Pri_Display!=FingerFull){
							g_UserManager_Pri_Display=FingerFull;
							pThis->timeTick=20;//2秒
							fns_Form_redraw();
					}
				else{
							cotMenu_MainExit();
							fns_Form_redraw();
				}
				break;
			case FingerExists:          // 该指纹已登记
				if(	g_UserManager_Pri_Display!=FingerExists){
						g_UserManager_Pri_Display=FingerExists;
						pThis->timeTick=20;//2秒
						fns_Form_redraw();
				}
				else{
							cotMenu_MainExit();
							fns_Form_redraw();
				}
			break;
			case RegistFingerFail:             // 指纹登记失败
				if(	g_UserManager_Pri_Display!=RegistFingerFail){
				g_UserManager_Pri_Display=RegistFingerFail;
				pThis->timeTick=20;//2秒
					fns_Form_redraw();
				}
				else{
						cotMenu_MainExit();
						fns_Form_redraw();
				}
			break;
			case RegistFingerSuccess:           // 指纹登记成功
				if(	g_UserManager_Pri_Display!=RegistFingerSuccess){
						g_UserManager_Pri_Display=RegistFingerSuccess;
						PLAY_MUSIC(TianJiaChengGong);
						pThis->timeTick=20;//2秒
						fns_Form_redraw();
				}
				else{
						cotMenu_MainExit();
						fns_Form_redraw();
				}
			break;
			case RegistFingerNoAdmin:
				if(	g_UserManager_Pri_Display!=RegistFingerNoAdmin){
						g_UserManager_Pri_Display=RegistFingerNoAdmin;
						PLAY_MUSIC(WeiLeAnQuan43_25_09);
						pThis->timeTick=28;//2秒
						fns_Form_redraw();
				}
				else{
						cotMenu_MainExit();
						fns_Form_redraw();
				}
				break;
			default:
			break;
		}
}

void RegistFingerPrint_onCreate(BASEFORM* pThis)
{
		unsigned char ret;
		unsigned short finger_index;
		unsigned short admin_num;
//	用户密码已满
		g_UserManager_Pri_Display=0;

		if(Admin_User==g_Add_Finger_Type){
				finger_index=get_finger_user_num(Admin_User);
		}
		else{
				if(Normal_User==g_Add_Finger_Type){
						finger_index=get_finger_user_num(Normal_User);
				}
				else if(Guest_User==g_Add_Finger_Type){
						finger_index=get_finger_user_num(Guest_User);
				}
				admin_num=get_all_admin_num();
		}

		g_FingerPrint_Acq_Times=1;
		if((!admin_num)&&(Admin_User!=g_Add_Finger_Type)){
				RegistFingerState=RegistFingerNoAdmin;
				g_UserManager_Pri_Display=0;
				pThis->timeTick=1;//2秒
				fns_Form_redraw();
		}
		else if(finger_index==0xffff){
				RegistFingerState=FingerFull;
				fns_Form_redraw();
				pThis->timeTick=2;   //1.5秒
				PLAY_MUSIC(YongHuYiMan);
		}
		else{
				ret = Get_EnrollIndex(&finger_index);
				g_FingerPrint_Buff.Finger_ID_Num=finger_index+100;
				g_FingerPrint_Buff.Finger_Index_Table=finger_index;
				if(ret == ERR_OK){
						RegistFingerState=RegistFinger;
						pThis->timeTick=10;   //1.5秒
						fns_Form_redraw();
						PLAY_MUSIC(FangShouZhi);
				}
				else{
						RegistFingerState=FingerFull;
						fns_Form_redraw();
						pThis->timeTick=20;   //1.5秒
				}
		}
		g_Wake_Time_Count=0;
}




/*************************************************/

uint8_t RegistPassWordStr[]={"添加管理员密码"};

void RegistPassWord_onShow(BASEFORM* pThis,CDC* pFormCDC);
void RegistPassWord_onChar(BASEFORM* pThis,int keyVal);
void RegistPassWord_onTick(BASEFORM* pThis);
void RegistPassWord_onCreate(BASEFORM* pThis);
const FORM_HANDLERS s_formcode_RegistPassWord={
	sizeof(MENUFORM),
		RegistPassWordStr,
    fns_NullForm_onDefault,
    RegistPassWord_onShow,
    RegistPassWord_onChar,
    RegistPassWord_onTick,
		RegistPassWord_onCreate,
    fns_NullForm_onDestroy,
    fns_NullForm_onFocus	
};



void RegistAdminPassWordUser(MENUFORM* pMenuForm)
{
	g_Add_Passwd_Type=Admin_User;
		fns_Form_create(&s_formcode_RegistPassWord);

}

void RegistNormalPassWordUser(MENUFORM* pMenuForm)  //添加普通用户
{
	g_Add_Passwd_Type=Normal_User;
		fns_Form_create(&s_formcode_RegistPassWord);
}

void RegistGuestPassWordUser(MENUFORM* pMenuForm)  //添加普通用户
{
		g_Add_Passwd_Type=Guest_User;
		fns_Form_create(&s_formcode_RegistPassWord);

}
void RegistPassWord_onShow(BASEFORM* pThis,CDC* pFormCDC)
{
	unsigned char DisplayBuf[50]={0};
	unsigned short i,x,y;
	

	switch(RegistPasswdState)
	{
		case InputPassWord:
			y=1;
		
			sprintf((char *)DisplayBuf,"用户编号:%04d",PassWord_Buff.PassWd_ID_Num);
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,DisplayBuf,1,y,FONT_16X16);
			y=21;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"输入新密码:",1,y,FONT_16X16);
			y=41;
			x=24;
			if(Input_Key_Buf.KeyFlag)
			{
					Input_Key_Buf.KeyFlag=0;
					if(Input_Key_Buf.KeyNum)
					{
							for(i=0;i<(Input_Key_Buf.KeyNum-1);i++)
							{
									OLED_ShowChar(pFormCDC,x,y,'*',16,1,FONT_16X16);
									x+=8;
							}
								OLED_ShowNum(pFormCDC, x, y,Input_Key_Buf.KeyBuf[Input_Key_Buf.KeyNum-1],1,24,1,FONT_16X16);
								x+=8;
								i++;
							for(i;i<10;i++)
							{
									OLED_ShowChar(pFormCDC,x,y,'_',16,1,FONT_16X16);
									x+=8;
							}
					}
					else{
							for(i=0;i<10;i++)
							{
									OLED_ShowChar(pFormCDC,x,y,'_',16,1,FONT_16X16);
									x+=8;
							}
					}
			}
			else{
					if(Input_Key_Buf.KeyNum<11)
					{
							for(i=0;i<Input_Key_Buf.KeyNum;i++)
							{
									OLED_ShowChar(pFormCDC,x,y,'*',16,1,FONT_16X16);
									x+=8;
							}
							for(i;i<10;i++)
							{
									OLED_ShowChar(pFormCDC,x,y,'_',16,1,FONT_16X16);
									x+=8;
							}
					}
					else{
							for(i=0;i<10;i++)
							{
									OLED_ShowChar(pFormCDC,x,y,'*',16,1,FONT_16X16);
									x+=8;
							}
					}
			}
			break;
		case ReInputPassWord:
			y=1;
			sprintf((char *)DisplayBuf,"用户编号:%04d",PassWord_Buff.PassWd_ID_Num);
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,DisplayBuf,1,y,FONT_16X16);
			y=21;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"确认新密码:",1,y,FONT_16X16);
			y=41;
			x=24;
			if(Input_Key_Buf_Temp.KeyFlag)
			{
					Input_Key_Buf_Temp.KeyFlag++;
					if(Input_Key_Buf_Temp.KeyFlag>3)Input_Key_Buf_Temp.KeyFlag=0;
					if(Input_Key_Buf_Temp.KeyNum)
					{
						for(i=0;i<(Input_Key_Buf_Temp.KeyNum-1);i++)
						{
								OLED_ShowChar(pFormCDC,x,y,'*',16,1,FONT_16X16);
								x+=8;
						}
						OLED_ShowNum(pFormCDC, x, y,Input_Key_Buf_Temp.KeyBuf[Input_Key_Buf_Temp.KeyNum-1],1,24,1,FONT_16X16);
						x+=8;
						i++;
						for(i;i<10;i++)
						{
								OLED_ShowChar(pFormCDC,x,y,'_',16,1,FONT_16X16);
								x+=8;
						}
					}
					else{
						for(i=0;i<10;i++)
						{
								OLED_ShowChar(pFormCDC,x,y,'_',16,1,FONT_16X16);
								x+=8;
						}
					}
			}
			else{
					if(Input_Key_Buf_Temp.KeyNum<11)
					{
							for(i=0;i<Input_Key_Buf_Temp.KeyNum;i++)
							{
									OLED_ShowChar(pFormCDC,x,y,'*',16,1,FONT_16X16);
									x+=8;
							}
							for(i;i<10;i++)
							{
									OLED_ShowChar(pFormCDC,x,y,'_',16,1,FONT_16X16);
									x+=8;
							}
					}
					else
					{
							for(i=0;i<10;i++)
							{
									OLED_ShowChar(pFormCDC,x,y,'*',16,1,FONT_16X16);
									x+=8;
							}
					}		
			}
			break;
		
		case PassWordFull:
		
				
			y=1;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);		
			y=32;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"密码用户已满",1,y,FONT_16X16);			
//			
			break;
		case RegistPasswdSuccess:	
					
			y=1;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);		
			y=32;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"密码添加成功",1,y,FONT_16X16);			
//			pThis->timeTick=20;//1.5秒后返回上一级目录
			break;
		case PasswdErrLess:        	//密码位数不够
		
			y=1;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);		
			y=21;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"密码位数错误",1,y,FONT_16X16);
			y=38;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"请重新输入",1,y,FONT_16X16);			
//			pThis->timeTick=15;//1.5秒后返回上一级目录
			break;
		case PasswdErrDiff:          //密码不止
			
			y=1;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);		
			y=21;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"密码不一致",1,y,FONT_16X16);
			y=38;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"请重新输入",1,y,FONT_16X16);			
//			pThis->timeTick=15;//1.5秒后返回上一级目录
			break;
		case	PasswdErrDupicate:      //密码输入重复

			y=1;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);		
			y=21;
			x=52;
			LCD_Error_Icon(pFormCDC, x, y);	
			y=47;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"密码添加失败",1,y,FONT_16X16);			
//			pThis->timeTick=15;//1.5秒后返回上一级目录
			break;	
		case PasswdNoAdmin:
			y=1;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);		
			y+=20;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"为了您的安全",1,y,FONT_16X16);	
			y+=18;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"请先添加管理员",1,y,FONT_16X16);	
			break;		
		default:
			break;	
	}		
}


void RegistPassWord_onChar(BASEFORM* pThis,int keyVal)
{
	unsigned char ret;

	switch(RegistPasswdState)
	{
		case InputPassWord:
			PLAY_MUSIC(7);
			switch(keyVal)
			{
				case 12:   //确认
				if(Input_Key_Buf.KeyNum>10){
							memset(&Input_Key_Buf,0x00,sizeof(InputBuf_TypeDef));
							RegistPasswdState=PasswdErrLess;
							fns_Form_redraw(); 
							PLAY_MUSIC(MiMaWeiShu16_29);
				}
				if(Input_Key_Buf.KeyNum<6){
							memset(&Input_Key_Buf,0x00,sizeof(InputBuf_TypeDef));
							RegistPasswdState=PasswdErrLess;
							fns_Form_redraw(); 
							PLAY_MUSIC(MiMaWeiShu16_29);
				}
				else{
							RegistPasswdState=ReInputPassWord;
							fns_Form_redraw(); 
							PLAY_MUSIC(QueRenXinMiMa);
				}
				break;
				case 10:  //取消
					if(Input_Key_Buf.KeyNum){
							Input_Key_Buf.KeyNum--;
							Input_Key_Buf.KeyFlag=1;
					}
					else		
						cotMenu_MainExit();	
					fns_Form_redraw(); 
					break;
				default:
					if(Input_Key_Buf.KeyNum<11)   //密码最大10位，寄到11位用于判定超出位数
					{
							if(11==keyVal)Input_Key_Buf.KeyBuf[Input_Key_Buf.KeyNum]=0;
							else Input_Key_Buf.KeyBuf[Input_Key_Buf.KeyNum]=keyVal;
							Input_Key_Buf.KeyNum++;
					}
					Input_Key_Buf.KeyFlag=1;
					fns_Form_redraw(); 
					break;
			}
			break;
		case ReInputPassWord:
			PLAY_MUSIC(7);
			switch(keyVal)
			{
					case 12:
					if(Input_Key_Buf_Temp.KeyNum!=Input_Key_Buf.KeyNum)//密码小于6位，或超过10位
					{
							memset(&Input_Key_Buf_Temp,0x00,sizeof(InputBuf_TypeDef));
							RegistPasswdState=PasswdErrDiff;
							fns_Form_redraw(); 
							PLAY_MUSIC(MiMaBuYiZhi15_29);
					}
					else if(memcmp(Input_Key_Buf_Temp.KeyBuf,Input_Key_Buf.KeyBuf,Input_Key_Buf.KeyNum))//两次密码不一致
					{
							memset(&Input_Key_Buf_Temp,0x00,sizeof(InputBuf_TypeDef));
							RegistPasswdState=PasswdErrDiff;
							fns_Form_redraw(); 
							PLAY_MUSIC(MiMaBuYiZhi15_29);
					}else 
					{
							//密码重复
							PassWord_Buff.PassWd_Num=Input_Key_Buf_Temp.KeyNum;   //  复制给PassWord_TypeDef 的值
						
							if(Admin_User==g_Add_Passwd_Type)
							{
									PassWord_Buff.User_Type=Admin_User;
							}
							else if(Normal_User==g_Add_Passwd_Type)
							{
									PassWord_Buff.User_Type=Normal_User;
							}
							else if(Guest_User==g_Add_Passwd_Type)
							{
									PassWord_Buff.User_Type=Guest_User;
							}
							memcpy(PassWord_Buff.PassWd_Buff,Input_Key_Buf_Temp.KeyBuf,PassWord_Buff.PassWd_Num);
							ret=passwd_check(&PassWord_Buff) ;
							if(PassWd_Err_Invalid==ret)  //应该不会进
							{
									memset(&Input_Key_Buf,0x00,sizeof(InputBuf_TypeDef));
									memset(&Input_Key_Buf_Temp,0x00,sizeof(InputBuf_TypeDef));
									RegistPasswdState=PasswdErrDupicate;
									fns_Form_redraw(); 
									PLAY_MUSIC(MiMaTianJiaShiBai);
							}
							else if(PassWd_Err_Length==ret)//应该不会进
							{
							
							}
							else if(PassWd_Err_Dupicate==ret)
							{
									memset(&Input_Key_Buf,0x00,sizeof(InputBuf_TypeDef));
									memset(&Input_Key_Buf_Temp,0x00,sizeof(InputBuf_TypeDef));
									RegistPasswdState=PasswdErrDupicate;
									fns_Form_redraw(); 
									PLAY_MUSIC(MiMaTianJiaShiBai);
							}
							else
							{
									if(Admin_User==g_Add_Passwd_Type)
									{
											g_System_Para_Info.restore_factory=1;
											System_Para_Save();
									}
									
									passwd_save( &PassWord_Buff,  PassWord_Buff.PassWd_Save_Num);
									OperateLog_Info.clock_sec=Clock2Time(&clock_env);
									OperateLog_Info.operatetype=ADD_USER;
									OperateLog_Info.user_num=PassWord_Buff.PassWd_ID_Num;
									OperateLog_Info.user_type=PasswdMode;
									save_OperateLog(&OperateLog_Info);
									
									RegistPasswdState=RegistPasswdSuccess;
									fns_Form_redraw(); 
									PLAY_MUSIC(MiMaTianJiaChengGong);
							}
					}			
					break;
					case 10:  //取消
					if(Input_Key_Buf_Temp.KeyNum)
					{
							Input_Key_Buf_Temp.KeyNum--;
							Input_Key_Buf_Temp.KeyFlag=1;
					}
					else		cotMenu_MainExit();	
					fns_Form_redraw(); 
					break;
					default:
					if(Input_Key_Buf_Temp.KeyNum<11)
					{
							if(11==keyVal)Input_Key_Buf_Temp.KeyBuf[Input_Key_Buf_Temp.KeyNum]=0;
							else Input_Key_Buf_Temp.KeyBuf[Input_Key_Buf_Temp.KeyNum]=keyVal;
							Input_Key_Buf_Temp.KeyNum++;
					}
					Input_Key_Buf_Temp.KeyFlag=1;
					fns_Form_redraw(); 
					break;
			}
				break;														
			case PassWordFull:          // 密码已满
				break;
			case RegistPasswdSuccess:         // 密码注册成功
				break;
			case PasswdErrLess:       	//密码位数不够
				break;
			case PasswdErrDiff:          //密码不止
				break;
			case PasswdErrDupicate:     //密码输入重复
				break;
			case PasswdNoAdmin:
				switch(keyVal){
					case 10:
					cotMenu_MainExit();	
					fns_Form_redraw(); 
						break;
					default:
						break;
				}
				break;
				default:
			break;
	}
}


void RegistPassWord_onTick(BASEFORM* pThis)
{
	  pThis->timeTick=5;
if(g_Wake_Time_Count>USER_MANAGER_DELAY_TIME){
					FNS_ALL_FORM_CLOSE();
					g_Wake_Time_Count=0;
					fns_Form_redraw();
		}
		switch(RegistPasswdState)
		{
				case InputPassWord:         // 输入密码
					g_UserManager_Pri_Display=InputPassWord;
					fns_Form_redraw();
					break;
				case ReInputPassWord:       // 重新输入密码
					g_UserManager_Pri_Display=ReInputPassWord;
					fns_Form_redraw();
					break;
				case PassWordFull:          // 密码已满
						if(	g_UserManager_Pri_Display!=PassWordFull){
								g_UserManager_Pri_Display=PassWordFull;
								pThis->timeTick=20;//2秒
						}
							else
						{
								cotMenu_MainExit();
								fns_Form_redraw();
						} 
						break;
				case RegistPasswdSuccess:         // 密码注册成功
						if(	g_UserManager_Pri_Display!=RegistPasswdSuccess){
								g_UserManager_Pri_Display=RegistPasswdSuccess;
								pThis->timeTick=20;//2秒
							fns_Form_redraw();
						}
							else
						{
								cotMenu_MainExit();
								fns_Form_redraw();
						} 
						break;
				case PasswdErrLess:       	// 密码位数不够
						if(	g_UserManager_Pri_Display!=PasswdErrLess){
								g_UserManager_Pri_Display=PasswdErrLess;
								pThis->timeTick=20;//2秒
							fns_Form_redraw();
						}
							else
						{
								cotMenu_MainExit();
								fns_Form_redraw();
						}
						break;
				case PasswdErrDiff:         // 密码不一致
						if(	g_UserManager_Pri_Display!=PasswdErrDiff){
								g_UserManager_Pri_Display=PasswdErrDiff;
								pThis->timeTick=20;//2秒
							fns_Form_redraw();
						}
							else
						{
								cotMenu_MainExit();
								fns_Form_redraw();
						}
						break;
				case PasswdErrDupicate:     // 密码输入重复
					if(	g_UserManager_Pri_Display!=PasswdErrDupicate){
								g_UserManager_Pri_Display=PasswdErrDupicate;
								pThis->timeTick=20;//2秒
								fns_Form_redraw();
						
						}
							else
						{
								cotMenu_MainExit();
								fns_Form_redraw();
						}
						break;
				case PasswdNoAdmin:
					if(	g_UserManager_Pri_Display!=PasswdNoAdmin){
								g_UserManager_Pri_Display=PasswdNoAdmin;
								PLAY_MUSIC(WeiLeAnQuan43_25_09);
								pThis->timeTick=28;//2秒
								fns_Form_redraw();
						}
						else{
								cotMenu_MainExit();
								fns_Form_redraw();
						}
					break;
				default :
					fns_Form_redraw(); 
						break;
		}

}

void RegistPassWord_onCreate(BASEFORM* pThis)
{
		//用户密码已满
		unsigned short user_valid_num;
		unsigned short admin_num;
		g_UserManager_Pri_Display=0;

		if(Admin_User==g_Add_Passwd_Type){
				user_valid_num=get_passwd_user_all_num(Admin_User);		
		}
		else{
			if(Normal_User==g_Add_Passwd_Type){
					user_valid_num=get_passwd_user_all_num(Normal_User);		
			}
			else if(Guest_User==g_Add_Passwd_Type){
					user_valid_num=get_passwd_user_all_num(Guest_User);		
			}
			admin_num=get_all_admin_num();
		}
		
		if((!admin_num)&&(Admin_User!=g_Add_Passwd_Type)){
				RegistPasswdState=PasswdNoAdmin;
			g_UserManager_Pri_Display=0;
				pThis->timeTick=1;//2秒
				fns_Form_redraw();
		}
		else if(user_valid_num==0xffff)
		{				
				RegistPasswdState=PassWordFull;
				pThis->timeTick=20;   //1.5秒
				fns_Form_redraw();
				PLAY_MUSIC(YongHuYiMan);
		}
		else
		{
				PassWord_Buff.PassWd_ID_Num=user_valid_num+200;
				PassWord_Buff.PassWd_Save_Num=user_valid_num;
			
				PLAY_MUSIC(ShuRu21_09_14_58);
				pThis->timeTick=10;   //1.5秒
				RegistPasswdState=InputPassWord;
				memset(&Input_Key_Buf,0x00,sizeof(InputBuf_TypeDef));
				memset(&Input_Key_Buf_Temp,0x00,sizeof(InputBuf_TypeDef));
				fns_Form_redraw();
		}
		g_Wake_Time_Count=0;
}

/**********************************************************/

uint8_t RegistICCardStr[]={"添加ID卡"};

void RegistICCard_onShow(BASEFORM* pThis,CDC* pFormCDC);
void RegistICCard_onChar(BASEFORM* pThis,int keyVal);
void RegistICCard_onTick(BASEFORM* pThis);
void RegistICCard_onCreate(BASEFORM* pThis);
void RegistICCard_onDestroy(BASEFORM* pThis);
const FORM_HANDLERS s_formcode_RegistICCard={
		sizeof(MENUFORM),
		RegistICCardStr,
    fns_NullForm_onDefault,
    RegistICCard_onShow,
    RegistICCard_onChar,
    RegistICCard_onTick,
		RegistICCard_onCreate,
    RegistICCard_onDestroy,
    fns_NullForm_onFocus	
};


void RegistAdminICUser(MENUFORM* pMenuForm)
{
	g_Add_ICCard_Type=Admin_User;
		fns_Form_create(&s_formcode_RegistICCard);
}

void RegistNormalICUser(MENUFORM* pMenuForm)
{
		g_Add_ICCard_Type=Normal_User;

		fns_Form_create(&s_formcode_RegistICCard);
}
void RegistGuestICUser(MENUFORM* pMenuForm)
{
		g_Add_ICCard_Type=Guest_User;
		fns_Form_create(&s_formcode_RegistICCard);
}

void RegistICCard_onShow(BASEFORM* pThis,CDC* pFormCDC)
{
	unsigned char DisplayBuf[50]={0};
	unsigned short x,y;
	
	switch(RegistICCardState)
	{
		case putICCard:
			y=1;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"添加感应卡",1,y,FONT_16X16);
			y=21;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"请刷感应卡",0,y,FONT_16X16);
			y=41;
			x=24;
		
			sprintf((char *)DisplayBuf,"用户编号:%04d",g_ICCard_Buff.ICCard_ID_Num);
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,DisplayBuf,1,y,FONT_16X16);
			break;
	
		case ICCardFull:
			y=1;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);		
			y=32;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"感应卡已满",1,y,FONT_16X16);			//语音提示用户已满
				
			break;
		case ICCardFail:
				y=1;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);		
				x=52;
				y=21;
				LCD_Error_Icon(pFormCDC, x, y);
				y=47;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"感应卡添加失败",1,y,FONT_16X16);						
				break;
			break;
		case RegistICSuccess:	
			y=1;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);		
			y=32;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"感应卡添加成功",1,y,FONT_16X16);			
			break;

		case	CardExists:      //密码输入重复
			y=1;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);		
			y=32;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"感应卡已存在",1,y,FONT_16X16);			
			break;		
		case RegistICNoAdmin:
			y=1;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);		
			y+=20;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"为了您的安全",1,y,FONT_16X16);	
			y+=18;
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"请先添加管理员",1,y,FONT_16X16);	
			break;		
		default:
			
			break;	
	}		
}


void RegistICCard_onChar(BASEFORM* pThis,int keyVal)
{
	switch(RegistICCardState)
	{
		case putICCard:             // 放置IC卡
			switch(keyVal)
			{
				case 10:  						 //取消键和确认键都是返回
				case 12:
				cotMenu_MainExit();	
				fns_Form_redraw();
					break;
				default:
					break;
			}
			break;
		case  CardExists:             // IC卡已登记
			break;
		case ICCardFull:            	// IC卡已满
			break;
		case ICCardFail:
			break;
		case RegistICSuccess:         // IC卡注册成功
			break;
		case RegistICNoAdmin:
			switch(keyVal){
				case 10:  						 //取消键和确认键都是返回
				cotMenu_MainExit();	
				fns_Form_redraw();
				case 12:
				cotMenu_MainExit();	
				fns_Form_redraw();
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}


void RegistICCard_onTick(BASEFORM* pThis)
{
		unsigned char ret;
	  pThis->timeTick=5;
		if(g_Wake_Time_Count>USER_MANAGER_DELAY_TIME){
					FNS_ALL_FORM_CLOSE();
					g_Wake_Time_Count=0;
					fns_Form_redraw();
		}
		switch(RegistICCardState)
		{
				case putICCard:
					g_UserManager_Pri_Display=putICCard;
					ret= RF_ENROLL(g_ICCard_Buff.ICCard_buf);
					macRC522_Reset_0;
					if(ret==1){
						g_Wake_Time_Count=0;
						ret=ICCard_check(&g_ICCard_Buff);
						if(ret){
							if(Admin_User==g_Add_ICCard_Type){
									g_System_Para_Info.restore_factory=1;
									System_Para_Save();
									g_ICCard_Buff.User_Type=Admin_User;
							}
							else if(Normal_User==g_Add_ICCard_Type){
									g_ICCard_Buff.User_Type=Normal_User;
							}
							else if(Guest_User==g_Add_ICCard_Type){
									g_ICCard_Buff.User_Type=Guest_User;
							}
							iccard_save(&g_ICCard_Buff,g_ICCard_Buff.ICCard_Save_Num);
							OperateLog_Info.clock_sec=Clock2Time(&clock_env);
							OperateLog_Info.operatetype=ADD_USER;
							OperateLog_Info.user_num=g_ICCard_Buff.ICCard_ID_Num;
							OperateLog_Info.user_type=ICCardMode;
							save_OperateLog(&OperateLog_Info);
							RegistICCardState=RegistICSuccess;
							PLAY_MUSIC(TianJiaChengGong);
							fns_Form_redraw(); 
						break;
						}
						else{
							RegistICCardState=CardExists;
							PLAY_MUSIC(GanYingKaYiCunZai);
							fns_Form_redraw(); 
							break;
						}
					}
					else if(ret==0){
						RegistICCardState=ICCardFail;
						PLAY_MUSIC(TianJiaShiBai);
						fns_Form_redraw(); 
						break;
					}
						break;
				case ICCardFull:          // ID卡已满
						if(	g_UserManager_Pri_Display!=ICCardFull){
							g_UserManager_Pri_Display=ICCardFull;
							pThis->timeTick=20;//2秒
							fns_Form_redraw();
						}
						else{
							cotMenu_MainExit();
							fns_Form_redraw();
						}
						break;
				case ICCardFail:
					if(	g_UserManager_Pri_Display!=ICCardFail){
							g_UserManager_Pri_Display=ICCardFail;
							pThis->timeTick=20;//2秒
							fns_Form_redraw();
						}
						else{
							RegistICCardState=putICCard;
							fns_Form_redraw();
						}
				case RegistICSuccess:   	// ID卡注册成功
						if(	g_UserManager_Pri_Display!=RegistICSuccess){
							g_UserManager_Pri_Display=RegistICSuccess;
							pThis->timeTick=20;//2秒
							fns_Form_redraw();
						}
						else{
							cotMenu_MainExit();
							fns_Form_redraw();
						}
						break;
				case CardExists:    // 卡片已存在
						if(	g_UserManager_Pri_Display!=CardExists){
							g_UserManager_Pri_Display=CardExists;
							pThis->timeTick=20;//2秒
							fns_Form_redraw();
						}
						else{
							cotMenu_MainExit();
							fns_Form_redraw();
						}
						break;
				case 	RegistICNoAdmin:
						if(	g_UserManager_Pri_Display!=RegistICNoAdmin){
							g_UserManager_Pri_Display=RegistICNoAdmin;
							PLAY_MUSIC(WeiLeAnQuan43_25_09);
							pThis->timeTick=28;//2秒
							fns_Form_redraw();
						}
						else{
							cotMenu_MainExit();
							fns_Form_redraw();
						}
					break;
				default :
						fns_Form_redraw(); 
						break;
		}

}

void RegistICCard_onCreate(BASEFORM* pThis)
{
		unsigned short iccard_valid_num;
		unsigned short admin_num;
//	用户密码已满
		g_UserManager_Pri_Display=0;

		if(Admin_User==g_Add_ICCard_Type)
		{
				iccard_valid_num=get_ic_user_num(Admin_User);
		}
		else
		{
			if(Normal_User==g_Add_ICCard_Type){
					iccard_valid_num=get_ic_user_num(Normal_User);
			}
			else if(Guest_User==g_Add_ICCard_Type){
					iccard_valid_num=get_ic_user_num(Guest_User);
			}
			admin_num=get_all_admin_num();
		}
		
		RC522_Init();
		if((!admin_num)&&(Admin_User!=g_Add_ICCard_Type)){
				RegistICCardState=RegistICNoAdmin;
				g_UserManager_Pri_Display=0;
				pThis->timeTick=1;//2秒
				fns_Form_redraw();
		}
		else if(iccard_valid_num==0xffff){
				RegistICCardState=ICCardFull;
				fns_Form_redraw();
				pThis->timeTick=2;   //1.5秒
				PLAY_MUSIC(YongHuYiMan);
		}
		else{
				g_ICCard_Buff.ICCard_Save_Num=iccard_valid_num;
				g_ICCard_Buff.ICCard_ID_Num=300+iccard_valid_num;
				RegistICCardState=putICCard;
				pThis->timeTick=10;   //1.5秒
				fns_Form_redraw();
				PLAY_MUSIC(FangGanYingKa);
		}
		g_Wake_Time_Count=0;
}


void RegistICCard_onDestroy(BASEFORM* pThis)
{
		RC522_Init();
}

/*****************************************************************/
void RegistThreatFingerPrintUser(MENUFORM* pMenuForm)
{


}
void RegistThreatPassWordUser(MENUFORM* pMenuForm)
{


}
void RegistThreatICUser(MENUFORM* pMenuForm)
{


}
/***********************************************/
uint8_t DeleteUserStr[]={"删除用户"};

void DeleteUser_onShow(BASEFORM* pThis,CDC* pFormCDC);
void DeleteUser_onChar(BASEFORM* pThis,int keyVal);
void DeleteUser_onTick(BASEFORM* pThis);
void DeleteUser_onCreate(BASEFORM* pThis);
const FORM_HANDLERS s_formcode_DeleteUser={
		sizeof(MENUFORM),
		DeleteUserStr,
		fns_NullForm_onDefault,
		DeleteUser_onShow,
		DeleteUser_onChar,
		DeleteUser_onTick,
		DeleteUser_onCreate,
		fns_NullForm_onDestroy,
		fns_NullForm_onFocus	
};

void DeleteUser(MENUFORM* pMenuForm)
{
		fns_Form_create(&s_formcode_DeleteUser);
}

void DeleteUser_onShow(BASEFORM* pThis,CDC* pFormCDC)
{
		uint8_t x,y;
		if(pThis->handlers->titleId)
		{
				y=1;
				x=4;
				Dir2_Logo_Icon(pFormCDC,x,y);
				x=108;
				Dir8_Logo_Icon(pFormCDC,x,y);
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,pThis->handlers->titleId,1,y,FONT_16X16);
		}
		fns_MenuForm_onShow(pThis,pFormCDC);
}

void DeleteUser_onChar(BASEFORM* pThis,int keyVal)
{
    fns_MenuForm_onChar(pThis,keyVal);
}

void DeleteUser_onTick(BASEFORM* pThis)
{
    pThis->timeTick=2;
	if(g_Wake_Time_Count>USER_MANAGER_DELAY_TIME){
					FNS_ALL_FORM_CLOSE();
					g_Wake_Time_Count=0;
					fns_Form_redraw();
		}
    fns_Form_redraw();
}

void DeleteFingerPrintUser(MENUFORM* pMenuForm);
void DeletePassWordUser(MENUFORM* pMenuForm);
void DeleteICUser(MENUFORM* pMenuForm);

const MENUITEM s_DeleteUserMenu[]={
		{(uint8_t*)"删除指纹",DeleteFingerPrintUser,NULL}, 
		{(uint8_t*)"删除密码",DeletePassWordUser,NULL}, 
		{(uint8_t*)"删除IC卡",DeleteICUser,NULL}, 
};

void DeleteUser_onCreate(BASEFORM* pThis)
{
    MENUFORM* pMenuForm=(MENUFORM*)pThis;
    pMenuForm->menuItems=s_DeleteUserMenu;
    pMenuForm->itemCount=sizeof(s_DeleteUserMenu)/sizeof(MENUITEM);
    pMenuForm->firstIndex=0;
		pMenuForm->curIndex=0;
    pMenuForm->timeTick=2;
	g_Wake_Time_Count=0;
}
/***************************************************/
AllTypeUserNum_Def g_all_user_num;

uint8_t DeleteUserModeStr[]={"删除用户"};   //删除用户的分类

void DeleteUserMode_onShow(BASEFORM* pThis,CDC* pFormCDC);
void DeleteUserMode_onChar(BASEFORM* pThis,int keyVal);
void DeleteUserMode_onTick(BASEFORM* pThis);
void DeleteUserMode_onCreate(BASEFORM* pThis);

const FORM_HANDLERS s_formcode_DeleteUserMode={
		sizeof(MENUFORM),
		DeleteUserModeStr,
		fns_NullForm_onDefault,
		DeleteUserMode_onShow,
		DeleteUserMode_onChar,
		DeleteUserMode_onTick,
		DeleteUserMode_onCreate,
		fns_NullForm_onDestroy,
		fns_NullForm_onFocus	
};

void DeleteICUser(MENUFORM* pMenuForm)
{
		g_Delete_Mode_Type=DeleteICCardUser;
		fns_Form_create(&s_formcode_DeleteUserMode);
}

void DeleteFingerPrintUser(MENUFORM* pMenuForm)
{
		g_Delete_Mode_Type=DeleteFingerUser;
		fns_Form_create(&s_formcode_DeleteUserMode);
}


void DeletePassWordUser(MENUFORM* pMenuForm)
{
		 g_Delete_Mode_Type=DeletePasswdUser;
		 fns_Form_create(&s_formcode_DeleteUserMode);
}

void DeleteUserMode_onShow(BASEFORM* pThis,CDC* pFormCDC)
{
		uint8_t y;
		y=1;
		if(DeleteFingerUser==g_Delete_Mode_Type){
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"删除指纹用户",1,y,FONT_16X16);
		}
		else if(DeletePasswdUser==g_Delete_Mode_Type){
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"删除密码用户",1,y,FONT_16X16);
		}
		else if(DeleteICCardUser==g_Delete_Mode_Type){
			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"删除卡用户",1,y,FONT_16X16);
		}
		fns_MenuForm_onShow(pThis,pFormCDC);
}

void DeleteUserMode_onChar(BASEFORM* pThis,int keyVal)
{
    fns_MenuForm_onChar(pThis,keyVal);
}

void DeleteUserMode_onTick(BASEFORM* pThis)
{
    pThis->timeTick=2;
	if(g_Wake_Time_Count>USER_MANAGER_DELAY_TIME){
					FNS_ALL_FORM_CLOSE();
					g_Wake_Time_Count=0;
					fns_Form_redraw();
		}
    fns_Form_redraw();
}

void DeleteUserModeByType(MENUFORM* pMenuForm);
void DeleteUserModeBySN(MENUFORM* pMenuForm);
void DeleteUserModeByVerify(MENUFORM* pMenuForm);

const MENUITEM s_DeleteUserModeMenu[]={
		{(uint8_t*)"分组删除",DeleteUserModeByType,NULL}, 
		{(uint8_t*)"编号删除",DeleteUserModeBySN,NULL}, 
		{(uint8_t*)"验证删除",DeleteUserModeByVerify,NULL}, 
};

void DeleteUserMode_onCreate(BASEFORM* pThis)
{
    MENUFORM* pMenuForm=(MENUFORM*)pThis;
    pMenuForm->menuItems=s_DeleteUserModeMenu;
    pMenuForm->itemCount=sizeof(s_DeleteUserModeMenu)/sizeof(MENUITEM);
    pMenuForm->firstIndex=0;
		pMenuForm->curIndex=0;
    pMenuForm->timeTick=2;
	g_Wake_Time_Count=0;
}
/********************************************************/
uint8_t DeleteUserByTypeStr[]={"分组删除"};

void DeleteUserByType_onShow(BASEFORM* pThis,CDC* pFormCDC);
void DeleteUserByType_onChar(BASEFORM* pThis,int keyVal);
void DeleteUserByType_onTick(BASEFORM* pThis);
void DeleteUserByType_onCreate(BASEFORM* pThis);
const FORM_HANDLERS s_formcode_DeleteUserByType={
		sizeof(MENUFORM),
		DeleteUserByTypeStr,
		fns_NullForm_onDefault,
		DeleteUserByType_onShow,
		DeleteUserByType_onChar,
		DeleteUserByType_onTick,
		DeleteUserByType_onCreate,
		fns_NullForm_onDestroy,
		fns_NullForm_onFocus	
};


//	DeleteICCardByType=1,      
//	DeletePasswdByType,  //
//	DeleteFingerByType,   //长度错误


void DeleteUserModeByType(MENUFORM* pMenuForm)
{
	 fns_Form_create(&s_formcode_DeleteUserByType);
}

void DeleteUserByType_onShow(BASEFORM* pThis,CDC* pFormCDC)
{ 
		uint16_t x;
		uint8_t y;
		int rowIndex;
		const MENUITEM* pItem;
		int firstIndex;
		MENUFORM* pMenu=(MENUFORM*)pThis;
		unsigned short user_num_buff[4];
	
		user_num_buff[0]=g_all_user_num.normal_user;  //这个地方将结构体转成数组，数组排序与显示一致
		user_num_buff[1]=g_all_user_num.admin_user;
//		user_num_buff[2]=g_all_user_num.guest_user;
//		user_num_buff[3]=g_all_user_num.force_user;

			switch(DeleteState)
			{
					case DeleteMenuByGroup:            // 删除界面
							y=0;
							if(DeleteFingerUser==g_Delete_Mode_Type){
								OLED_HZ_ShowMixStr_XCentre(pFormCDC,"分组删除指纹",1,y,FONT_16X16);
							}
							else if(DeletePasswdUser==g_Delete_Mode_Type){
								OLED_HZ_ShowMixStr_XCentre(pFormCDC,"分组删除密码",1,y,FONT_16X16);
							}
							else if(DeleteICCardUser==g_Delete_Mode_Type){
								OLED_HZ_ShowMixStr_XCentre(pFormCDC,"分组删除卡",1,y,FONT_16X16);
							}
							fns_MenuForm_adjustIndex(pMenu,3);//设置屏幕上显示的行数
							firstIndex=pMenu->firstIndex;
							for(rowIndex=0;rowIndex<3 && ((pMenu->firstIndex+rowIndex)<pMenu->itemCount) ;rowIndex++){
										pItem=&pMenu->menuItems[pMenu->firstIndex+rowIndex];
										if(pItem->onShow==NULL){																		//如果MENUITEM中的onShow成员为空，
												if((pMenu->curIndex)==(pMenu->firstIndex+rowIndex)){
													y=24+rowIndex*16;
													OLED_ShowMixStr(pFormCDC,12,y,(unsigned char *)pItem->titleId,0,1,FONT_16X16);
													OLED_ShowNum(pFormCDC,88,y,user_num_buff[pMenu->firstIndex+rowIndex], 3, 16,0,FONT_16X16);
												}
												else{
													y=24+rowIndex*16;
													OLED_ShowMixStr(pFormCDC,12,y,(unsigned char *)pItem->titleId,1,1,FONT_16X16);
													OLED_ShowNum(pFormCDC,88,y,user_num_buff[pMenu->firstIndex+rowIndex], 3, 16, 1,FONT_16X16);
												}
										}
										else   //这个地方跑不到
										{
												pItem->onShow(pMenu,pFormCDC,pItem,rowIndex);
										}
							}
					break;
				case	DeleteMenuByGroupNormal:
				case	DeleteMenuByGroupAdmin:
				case	DeleteMenuByGroupGuest:
					
					y=1;
					x=4;
					Dir4_Logo_Icon(pFormCDC,x,y);
					x=12;
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
					x=108;
					Dir6_Logo_Icon(pFormCDC,x,y);
					y=24;
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"确定要删除吗\?",1,y,FONT_16X16);
					y=47;
					x=18;
					if(g_Yes_Or_No) 	OLED_YesOrNO(pFormCDC,x,y,1);
					else 	OLED_YesOrNO(pFormCDC,x,y,0);
					break;
				case	PendingDeletionByGroup:	 			 //正在删除
					
					y=1;
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
					y=32;
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"正在删除...",1,y,FONT_16X16);
					break;
				case DeleteMenuByGoupSuccess:   //删除成功
					
					System_DelSuccess_Tip(pFormCDC);
					break;
				case	DeleteFailByGroup:
					System_DelFail_Tip(pFormCDC);
					break;// 
				case	DeleteInvalidByGroup:
					break;// 
				default :
					break;
			}
}


void DeleteUserByType_onChar(BASEFORM* pThis,int keyVal)
{

			switch(DeleteState)
			{
				case DeleteMenuByGroup:
					fns_MenuForm_onChar(pThis,keyVal);
					fns_Form_redraw();
					break;
				case	DeleteMenuByGroupNormal:
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
									if(DeleteFingerUser==g_Delete_Mode_Type){
										  Delete_Finger_ByType(Normal_User);
											get_finger_all_type_num(&g_all_user_num);
										OperateLog_Info.user_type=FingerPrintMode;
									}
									else if(DeletePasswdUser==g_Delete_Mode_Type){
											Delete_Passwd_ByType(Normal_User);
											get_passwd_all_type_num(&g_all_user_num);
										OperateLog_Info.user_type=PasswdMode;
									}
									else if(DeleteICCardUser==g_Delete_Mode_Type){
											Delete_ICCard_ByType(Normal_User);
											get_iccard_all_type_num(&g_all_user_num);
											OperateLog_Info.user_type=ICCardMode;
									}
										DeleteState=PendingDeletionByGroup;
									OperateLog_Info.clock_sec=Clock2Time(&clock_env);
									OperateLog_Info.operatetype=DELETE_NORMAL;
									save_OperateLog(&OperateLog_Info);
										
								}
								else{
										DeleteState=DeleteMenuByGroup;
								}
								break;
							case 10:
								DeleteState=DeleteMenuByGroup;
								break;
							default:
								break;
						}
						fns_Form_redraw();
					break;
				case	DeleteMenuByGroupAdmin:
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
									if(DeleteFingerUser==g_Delete_Mode_Type){
											Delete_Finger_ByType(Admin_User);
											get_finger_all_type_num(&g_all_user_num);
											OperateLog_Info.user_type=FingerPrintMode;
											if(FingerPrintMode==g_Admin_Login_Info.User_Mode)
											{
											
											}
									}
									else if(DeletePasswdUser==g_Delete_Mode_Type){
											Delete_Passwd_ByType(Admin_User);
											get_passwd_all_type_num(&g_all_user_num);
											OperateLog_Info.user_type=PasswdMode;
											if(PasswdMode==g_Admin_Login_Info.User_Mode)
											{
											
											}
									}
									else if(DeleteICCardUser==g_Delete_Mode_Type){
											Delete_ICCard_ByType(Admin_User);
											get_iccard_all_type_num(&g_all_user_num);
											OperateLog_Info.user_type=ICCardMode;
										if(ICCardMode==g_Admin_Login_Info.User_Mode)
											{
											
											}
									}
									DeleteState=PendingDeletionByGroup;
									OperateLog_Info.clock_sec=Clock2Time(&clock_env);
									OperateLog_Info.operatetype=DELETE_ADMIN;
									save_OperateLog(&OperateLog_Info);
								}
								else{
										DeleteState=DeleteMenuByGroup;
								}
								break;
							case 10:
								DeleteState=DeleteMenuByGroup;
							break;
							default:
								break;
						}
						fns_Form_redraw();
					break;
				case	DeleteMenuByGroupGuest:
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
										if(DeleteFingerUser==g_Delete_Mode_Type){
													Delete_Finger_ByType(Guest_User);
													get_finger_all_type_num(&g_all_user_num);
													OperateLog_Info.user_type=FingerPrintMode;

										}
										else if(DeletePasswdUser==g_Delete_Mode_Type){
													Delete_Passwd_ByType(Guest_User);
													get_passwd_all_type_num(&g_all_user_num);
													OperateLog_Info.user_type=PasswdMode;
										}
										else if(DeleteICCardUser==g_Delete_Mode_Type){
													Delete_ICCard_ByType(Guest_User);
													get_iccard_all_type_num(&g_all_user_num);
													OperateLog_Info.user_type=ICCardMode;
										}
										DeleteState=PendingDeletionByGroup;
										OperateLog_Info.clock_sec=Clock2Time(&clock_env);
										OperateLog_Info.operatetype=DELETE_GUEST;
										save_OperateLog(&OperateLog_Info);
										
								}
								else{
										DeleteState=DeleteMenuByGroup;
								}
								break;
							case 10:
								DeleteState=DeleteMenuByGroup;
							break;
							default:
								break;
						}
						fns_Form_redraw();
					break;
				case	DeleteFailByGroup:
					break;
				case	DeleteInvalidByGroup:
					break;
				default :
					break;
			}
}
void DeleteUserByType_onTick(BASEFORM* pThis)
{
			pThis->timeTick=5;
	if(g_Wake_Time_Count>USER_MANAGER_DELAY_TIME){
					FNS_ALL_FORM_CLOSE();
					g_Wake_Time_Count=0;
					fns_Form_redraw();
		}
			fns_Form_redraw();
			switch(DeleteState)
			{
				case DeleteMenuByGroup:
					g_UserManager_Pri_Display=DeleteMenuByGroup;
					fns_Form_redraw();
					break;
				case	DeleteMenuByGroupNormal:
					g_UserManager_Pri_Display=DeleteMenuByGroupNormal;
					break;

				case	DeleteMenuByGroupAdmin:
					g_UserManager_Pri_Display=DeleteMenuByGroupAdmin;
					break;
				case	DeleteMenuByGroupGuest:
					g_UserManager_Pri_Display=DeleteMenuByGroupGuest;
					break;
				case PendingDeletionByGroup:  	//正在删除
					if(	g_UserManager_Pri_Display!=PendingDeletionByGroup){
							g_UserManager_Pri_Display=PendingDeletionByGroup;
							pThis->timeTick=20;//2秒
						fns_Form_redraw();
					}
						else
					{
								cotMenu_MainExit();
								fns_Form_redraw();
					}
					break;
				case DeleteMenuByGoupSuccess:   //删除成功
					if(	g_UserManager_Pri_Display!=DeleteMenuByGoupSuccess){
							g_UserManager_Pri_Display=DeleteMenuByGoupSuccess;
							pThis->timeTick=20;//2秒
						fns_Form_redraw();
					}
						else
					{
								cotMenu_MainExit();
								fns_Form_redraw();
					}
					break;
				case	DeleteFailByGroup:
					if(	g_UserManager_Pri_Display!=DeleteFailByGroup){
							g_UserManager_Pri_Display=DeleteFailByGroup;
							pThis->timeTick=20;//2秒
						fns_Form_redraw();
					}
						else
					{
								cotMenu_MainExit();
								fns_Form_redraw();
					}
					break;// 
				case	DeleteInvalidByGroup:
					if(	g_UserManager_Pri_Display!=DeleteInvalidByGroup){
							g_UserManager_Pri_Display=DeleteInvalidByGroup;
							pThis->timeTick=20;//2秒
						fns_Form_redraw();
					}
						else
					{
								cotMenu_MainExit();
								fns_Form_redraw();
					}
					break;// 
				default :
					break;
			}
}

void DeleteUserByTypeNormal(MENUFORM* pMenuForm);
void DeleteUserByTypeAdmin(MENUFORM* pMenuForm);
void DeleteUserByTypeGuest(MENUFORM* pMenuForm);
void DeleteUserByTypeForce(MENUFORM* pMenuForm);

const MENUITEM s_DeleteUserByTypeMenu[]={
		{(uint8_t*)"普通用户",DeleteUserByTypeNormal,NULL}, 
		{(uint8_t*)"管理用户",DeleteUserByTypeAdmin,NULL}, 
//		{(uint8_t*)"宾客用户",DeleteUserByTypeGuest,NULL}, 
//		{(uint8_t*)"胁迫用户",DeleteUserByTypeForce,NULL}, 
};


void DeleteUserByType_onCreate(BASEFORM* pThis)
{
    MENUFORM* pMenuForm=(MENUFORM*)pThis;
    pMenuForm->menuItems=s_DeleteUserByTypeMenu;
    pMenuForm->itemCount=sizeof(s_DeleteUserByTypeMenu)/sizeof(MENUITEM);
    pMenuForm->firstIndex=0;
		pMenuForm->curIndex=0;
    pMenuForm->timeTick=2;
	
		g_UserManager_Pri_Display=0;
		if(DeleteFingerUser==g_Delete_Mode_Type)
		{
				get_finger_all_type_num(&g_all_user_num);
		}
		else if(DeletePasswdUser==g_Delete_Mode_Type)
		{
				get_passwd_all_type_num(&g_all_user_num);
		}
		else if(DeleteICCardUser==g_Delete_Mode_Type)
		{
				get_iccard_all_type_num(&g_all_user_num);
		}
										
		DeleteState= DeleteMenuByGroup;
		fns_Form_redraw();
g_Wake_Time_Count=0;
}

void DeleteUserByTypeNormal(MENUFORM* pMenuForm)
{
	//写删除的代码
		DeleteState=DeleteMenuByGroupNormal;
		g_Yes_Or_No=0;
}
void DeleteUserByTypeAdmin(MENUFORM* pMenuForm)
{
	//写删除的代码
		DeleteState=DeleteMenuByGroupAdmin;
		g_Yes_Or_No=0;
}


void DeleteUserByTypeGuest(MENUFORM* pMenuForm)
{
//写删除的代码
		DeleteState=DeleteMenuByGroupGuest;
		g_Yes_Or_No=0;

}


void DeleteUserByTypeForce(MENUFORM* pMenuForm)
{
		DeleteState=DeleteMenuByGoupForce;
}





/*************************************************************/

uint8_t DeleteUserModeBySnStr[]={"编号删除"};

void DeleteUserModeBySn_onShow(BASEFORM* pThis,CDC* pFormCDC);
void DeleteUserModeBySn_onChar(BASEFORM* pThis,int keyVal);
void DeleteUserModeBySn_onTick(BASEFORM* pThis);
void DeleteUserModeBySn_onCreate(BASEFORM* pThis);
const FORM_HANDLERS s_formcode_DeleteUserModeBySn={
		sizeof(MENUFORM),
		DeleteUserModeBySnStr,
		fns_NullForm_onDefault,
		DeleteUserModeBySn_onShow,
		DeleteUserModeBySn_onChar,
		DeleteUserModeBySn_onTick,
		DeleteUserModeBySn_onCreate,
		fns_NullForm_onDestroy,
		fns_NullForm_onFocus	
};
void DeleteUserModeBySN(MENUFORM* pMenuForm)
{
	 fns_Form_create(&s_formcode_DeleteUserModeBySn);
}

void DeleteUserModeBySn_onShow(BASEFORM* pThis,CDC* pFormCDC)
{
		unsigned short i;
		uint16_t x;
		uint8_t y;
	
			switch(DeleteState)
			{
				case DeleteMenuBySn:           // 删除界面
						y=1;
						if(DeleteFingerUser==g_Delete_Mode_Type){
								OLED_HZ_ShowMixStr_XCentre(pFormCDC,"编号指纹删除",1,y,FONT_16X16);
						}
						else if(DeletePasswdUser==g_Delete_Mode_Type){
								OLED_HZ_ShowMixStr_XCentre(pFormCDC,"编号密码删除",1,y,FONT_16X16);
						}
						else if(DeleteICCardUser==g_Delete_Mode_Type){
								OLED_HZ_ShowMixStr_XCentre(pFormCDC,"编号卡片删除",1,y,FONT_16X16);
						}
						y+=16;
						y+=4;
						OLED_HZ_ShowMixStr_XCentre(pFormCDC,"请输入删除的编号",1,y,FONT_16X16);
						x=12;
						y+=16;
						y+=2;
						OLED_ShowMixStr(pFormCDC,x,y,"用户编号:",1,1,FONT_16X16);
						x+=72;
						if(Input_Key_Buf.KeyNum){
								for(i=0;i<Input_Key_Buf.KeyNum;i++){
										OLED_ShowNum(pFormCDC, x, y,Input_Key_Buf.KeyBuf[i],1,16,0,FONT_16X16);
										x+=8;
								}
								for(i;i<4;i++){
										OLED_ShowChar(pFormCDC,x,y,'_',16,1,FONT_16X16);
										x+=8;
								}
						}
						else{
								for(i=0;i<4;i++){
										OLED_ShowChar(pFormCDC,x,y,'_',16,1,FONT_16X16);
										x+=8;
								}
						}
					break;
				case DeleteMenuBySnConfirm:
					y=1;
					x=4;
					Dir4_Logo_Icon(pFormCDC,x,y);
					x=12;
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
					x=108;
					Dir6_Logo_Icon(pFormCDC,x,y);
					y=24;
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"确定要删除吗\?",1,y,FONT_16X16);
					y=47;
					x=18;
					if(g_Yes_Or_No) 	OLED_YesOrNO(pFormCDC,x,y,1);
					else 	OLED_YesOrNO(pFormCDC,x,y,0);
					break;
		
				case DeleteMenuBySnSuccess:   //删除成功
					System_DelSuccess_Tip(pFormCDC);
					break;
				case DeleteMenuBySnFail:// 
					System_DelFail_Tip(pFormCDC);
					break;
				case DeleteMenuBySnNoUser:
					y=1;
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
					y=32;
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"此用户不存在",1,y,FONT_16X16);
					break;
				case DeleteMenuBySnInvalid:
					break;
				default :
					break;
			}
}
void DeleteUserModeBySn_onChar(BASEFORM* pThis,int keyVal)
{
		uint16_t i;
		uint16_t user_sn;
		uint16_t finger_index;
		uint8_t ret;
		switch(DeleteState)
		{
			case DeleteMenuBySn:           // 删除界面
				PLAY_MUSIC(7);
				switch(keyVal){
					case 12:   //确认
					if(Input_Key_Buf.KeyNum){
							DeleteState=DeleteMenuBySnConfirm;
							fns_Form_redraw(); 
					}
					else{
							memset(&Input_Key_Buf,0x00,sizeof(InputBuf_TypeDef));
							cotMenu_MainExit();	
					}

					break;
					case 10:  //取消
						memset(&Input_Key_Buf,0x00,sizeof(InputBuf_TypeDef));
						cotMenu_MainExit();	
						fns_Form_redraw(); 
						break;
					default:
						if(Input_Key_Buf.KeyNum<4)  
						{
								if(11==keyVal)Input_Key_Buf.KeyBuf[Input_Key_Buf.KeyNum]=0;
								else Input_Key_Buf.KeyBuf[Input_Key_Buf.KeyNum]=keyVal;
								Input_Key_Buf.KeyNum++;
						}
						else{
								Input_Key_Buf.KeyNum=0;
								if(11==keyVal)Input_Key_Buf.KeyBuf[Input_Key_Buf.KeyNum]=0;
								else Input_Key_Buf.KeyBuf[Input_Key_Buf.KeyNum]=keyVal;
								Input_Key_Buf.KeyNum++;
						}
						Input_Key_Buf.KeyFlag=1;
						fns_Form_redraw(); 
						break;
				}
					break;
				case DeleteMenuBySnConfirm:
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
											user_sn=0;
											for(i=0;i<Input_Key_Buf.KeyNum;i++)   //计算输入的编号
											{
													user_sn=user_sn*10+Input_Key_Buf.KeyBuf[i];
											}
											if(DeleteFingerUser==g_Delete_Mode_Type)
											{
													if(user_sn>=100){
															if((g_Admin_Login_Info.User_Mode==FingerPrintMode)&&(g_Admin_Login_Info.User_ID_Num==user_sn)){
																ret=2;
															}
															else{
																	finger_index=user_sn-100;
																	ret=Delete_Finger_BySn(finger_index);
																	OperateLog_Info.user_type=FingerPrintMode;
															}
													}
													else ret=0;
														
											}
											else if(DeletePasswdUser==g_Delete_Mode_Type){
												if((g_Admin_Login_Info.User_Mode==PasswdMode)&&(g_Admin_Login_Info.User_ID_Num==user_sn)){
													ret=2;
												}
												else{
														ret=Delete_Passwd_BySn(user_sn);
														OperateLog_Info.user_type=PasswdMode;
												}														
											}
											else if(DeleteICCardUser==g_Delete_Mode_Type){
												
												if((g_Admin_Login_Info.User_Mode==ICCardMode)&&(g_Admin_Login_Info.User_ID_Num==user_sn)){
													ret=2;
												}
												else{
													ret=Delete_ICCard_BySn(user_sn);
													OperateLog_Info.user_type=ICCardMode;
												}
											}

											//判定删除是否成功
											if(1==ret)  //删除成功
											{
													OperateLog_Info.clock_sec=Clock2Time(&clock_env);
													OperateLog_Info.operatetype=DELETE_USER;
													OperateLog_Info.user_num=user_sn;
													save_OperateLog(&OperateLog_Info);
													DeleteState=DeleteMenuBySnSuccess;
											}
											else  if(2==ret) 
											{
													DeleteState=DeleteMenuBySnFail;
											}
											else{
													DeleteState=DeleteMenuBySnNoUser;
											}
											g_Yes_Or_No=0;
											memset(&Input_Key_Buf,0x00,sizeof(InputBuf_TypeDef));
											fns_Form_redraw(); 
										}
									else{
										cotMenu_MainExit();	
										fns_Form_redraw(); 								
									}
								break;
							case 10:
								cotMenu_MainExit();	
						fns_Form_redraw(); 
								break;
							default:
								break;
						}
					break;
				case DeleteMenuBySnSuccess:   //删除成功
					break;
				case DeleteMenuBySnFail:// 
					break;
				case DeleteMenuBySnNoUser:
					break;
				case DeleteMenuBySnInvalid:
					break;
				default :
					break;
			}
}


void DeleteUserModeBySn_onTick(BASEFORM* pThis)
{
		pThis->timeTick=5;
	if(g_Wake_Time_Count>USER_MANAGER_DELAY_TIME){
					FNS_ALL_FORM_CLOSE();
					g_Wake_Time_Count=0;
					fns_Form_redraw();
		}
		fns_Form_redraw();
		switch(DeleteState)
		{
				case DeleteMenuBySn:         // 删除界面
						break;
				case DeleteMenuBySnConfirm:
						break;
				case DeleteMenuBySnSuccess:  //删除成功
					if(	g_UserManager_Pri_Display!=DeleteMenuBySnSuccess){
							g_UserManager_Pri_Display=DeleteMenuBySnSuccess;
							pThis->timeTick=20;//2秒
						PLAY_MUSIC(ShanChuChengGong);
						fns_Form_redraw();
					}
						else
					{
								cotMenu_MainExit();
								fns_Form_redraw();
					}
					break;
				case DeleteMenuBySnFail:            // 
					if(	g_UserManager_Pri_Display!=DeleteMenuBySnFail){
							g_UserManager_Pri_Display=DeleteMenuBySnFail;
							pThis->timeTick=20;//2秒
						PLAY_MUSIC(ShanChuShiBai);
							fns_Form_redraw();
					}
					else{
							cotMenu_MainExit();
							fns_Form_redraw();
					}
					break;
				case DeleteMenuBySnNoUser:
					if(	g_UserManager_Pri_Display!=DeleteMenuBySnNoUser){
							g_UserManager_Pri_Display=DeleteMenuBySnNoUser;
							pThis->timeTick=20;//2秒
							PLAY_MUSIC(YongHuBuCunZai);
							fns_Form_redraw();
					}
					else{
							cotMenu_MainExit();
							fns_Form_redraw();
					}
					break;
				case DeleteMenuBySnInvalid:        // 
					if(	g_UserManager_Pri_Display!=DeleteMenuBySnInvalid){
							g_UserManager_Pri_Display=DeleteMenuBySnInvalid;
							pThis->timeTick=20;//2秒
						fns_Form_redraw();
					}
						else
					{
								cotMenu_MainExit();
								fns_Form_redraw();
					}
					break;
				default :
				break;
		}
}
void DeleteUserModeBySn_onCreate(BASEFORM* pThis)
{
    pThis->timeTick=2;
		g_Yes_Or_No=0;
		memset(&Input_Key_Buf,0x00,sizeof(InputBuf_TypeDef));
		g_UserManager_Pri_Display=0;
		DeleteState= DeleteMenuBySn;
		fns_Form_redraw();
	g_Wake_Time_Count=0;
}

/*****************************************************************/

uint8_t DeleteICUserByVerifyStr[]={"删除感应卡"};

void DeleteICUserByVerify_onShow(BASEFORM* pThis,CDC* pFormCDC);
void DeleteICUserByVerify_onChar(BASEFORM* pThis,int keyVal);
void DeleteICUserByVerify_onTick(BASEFORM* pThis);
void DeleteICUserByVerify_onCreate(BASEFORM* pThis);
void DeleteICUserByVerify_onDestroy(BASEFORM* pThis);
const FORM_HANDLERS s_formcode_DeleteICUserByVerify={
		sizeof(MENUFORM),
		DeleteICUserByVerifyStr,
		fns_NullForm_onDefault,
		DeleteICUserByVerify_onShow,
		DeleteICUserByVerify_onChar,
		DeleteICUserByVerify_onTick,
		DeleteICUserByVerify_onCreate,
		DeleteICUserByVerify_onDestroy,
		fns_NullForm_onFocus	
};



uint8_t DeletePasswdUserByVerifyStr[]={"密码验证删除"};

void DeletePasswdUserByVerify_onShow(BASEFORM* pThis,CDC* pFormCDC);
void DeletePasswdUserByVerify_onChar(BASEFORM* pThis,int keyVal);
void DeletePasswdUserByVerify_onTick(BASEFORM* pThis);
void DeletePasswdUserByVerify_onCreate(BASEFORM* pThis);
const FORM_HANDLERS s_formcode_DeletePasswdUserByVerify={
		sizeof(MENUFORM),
		DeletePasswdUserByVerifyStr,
		fns_NullForm_onDefault,
		DeletePasswdUserByVerify_onShow,
		DeletePasswdUserByVerify_onChar,
		DeletePasswdUserByVerify_onTick,
		DeletePasswdUserByVerify_onCreate,
		fns_NullForm_onDestroy,
		fns_NullForm_onFocus	
};


uint8_t DeleteFingerUserByVerifyStr[]={"删除验证"};

void DeleteFingerUserByVerify_onShow(BASEFORM* pThis,CDC* pFormCDC);
void DeleteFingerUserByVerify_onChar(BASEFORM* pThis,int keyVal);
void DeleteFingerUserByVerify_onTick(BASEFORM* pThis);
void DeleteFingerUserByVerify_onCreate(BASEFORM* pThis);
const FORM_HANDLERS s_formcode_DeleteFingerUserByVerify={
		sizeof(MENUFORM),
		DeleteFingerUserByVerifyStr,
		fns_NullForm_onDefault,
		DeleteFingerUserByVerify_onShow,
		DeleteFingerUserByVerify_onChar,
		DeleteFingerUserByVerify_onTick,
		DeleteFingerUserByVerify_onCreate,
		fns_NullForm_onDestroy,
		fns_NullForm_onFocus	
};



void DeleteUserModeByVerify(MENUFORM* pMenuForm)
{
//这个地方分类型，每种验证删除的方式不一样
		if(DeleteFingerUser==g_Delete_Mode_Type)
		{
				fns_Form_create(&s_formcode_DeleteFingerUserByVerify);
		}
		else if(DeletePasswdUser==g_Delete_Mode_Type){
				fns_Form_create(&s_formcode_DeletePasswdUserByVerify);
		}
		else if(DeleteICCardUser==g_Delete_Mode_Type){
				fns_Form_create(&s_formcode_DeleteICUserByVerify);
		}
}


void DeleteICUserByVerify_onShow(BASEFORM* pThis,CDC* pFormCDC)
{
		uint16_t x;
		uint8_t y;
	
			switch(DeleteState)
			{
				case DeleteMenuByVerify:           // 删除界面
					
					if(pThis->handlers->titleId){
							y=1;
							OLED_HZ_ShowMixStr_XCentre(pFormCDC,pThis->handlers->titleId,1,y,FONT_16X16);
					}
					y=32;
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"请验证感应卡",1,y,FONT_16X16);

					break;
				case DeleteMenuByVerifyConfirm:
						
					y=1;
					x=4;
					Dir4_Logo_Icon(pFormCDC,x,y);
					x=12;
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
					x=108;
					Dir6_Logo_Icon(pFormCDC,x,y);
					y=24;
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"确定要删除吗\?",1,y,FONT_16X16);
					y=47;
					x=18;
					if(g_Yes_Or_No) 	OLED_YesOrNO(pFormCDC,x,y,1);
					else 	OLED_YesOrNO(pFormCDC,x,y,0);
					break;
		
				case DeleteMenuByVerifySuccess:   //删除成功
					
					System_DelSuccess_Tip(pFormCDC);
					break;
				case DeleteMenuByVerifyFail:// 
					System_DelFail_Tip(pFormCDC);
					break;
				case DeleteMenuByVerifyNoUser:
					y=1;
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
					y=32;
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"此用户不存在",1,y,FONT_16X16);
					break;
				case DeleteMenuByVerifyInvalid:
					break;
				default :
					break;
			}
}
void DeleteICUserByVerify_onChar(BASEFORM* pThis,int keyVal)
{

		switch(DeleteState)
		{
			case DeleteMenuByVerify:           // 删除界面
					PLAY_MUSIC(7);
					switch(keyVal)
					{
						case 12:
							break;
						case 10:
							cotMenu_MainExit();	
							fns_Form_redraw(); 
							break;
						default:
							break;
					}
					break;
				case DeleteMenuByVerifyConfirm:
					PLAY_MUSIC(7);
					switch(keyVal){
							case 4:  //确认
									g_Yes_Or_No=1;
									fns_Form_redraw();
									break;
							case 6:  //否
									g_Yes_Or_No=0;
									fns_Form_redraw();
									break;
							case 12:
									if(g_Yes_Or_No){
										if((g_Admin_Login_Info.User_Mode==ICCardMode)&&(g_Admin_Login_Info.User_ID_Num==g_ICCard_Buff.ICCard_ID_Num)){
											DeleteState=DeleteMenuByVerifyFail;
											g_Yes_Or_No=0;
											fns_Form_redraw(); 
											break;
										}
										else{
											Delete_ICCard_BySn(g_ICCard_Buff.ICCard_ID_Num);
											OperateLog_Info.user_type=ICCardMode;
											OperateLog_Info.clock_sec=Clock2Time(&clock_env);
											OperateLog_Info.operatetype=DELETE_USER;
											OperateLog_Info.user_num=g_ICCard_Buff.ICCard_ID_Num;
											save_OperateLog(&OperateLog_Info);
											DeleteState=DeleteMenuByVerifySuccess;
											g_Yes_Or_No=0;
											fns_Form_redraw(); 
											break;
										}
									}
									else{
											cotMenu_MainExit();	
											fns_Form_redraw(); 
									}
									break;
							case 10:
									cotMenu_MainExit();	
									fns_Form_redraw(); 
									break;
							default:
							break;
					}
					break;
				case DeleteMenuByVerifySuccess:   //删除成功
					break;
				case DeleteMenuByVerifyFail:// 
					break;
				case DeleteMenuByVerifyNoUser:
					break;
				case DeleteMenuByVerifyInvalid:
					break;
				default :
					break;
			}
}


void DeleteICUserByVerify_onTick(BASEFORM* pThis)
{
		unsigned char ret=0;
		pThis->timeTick=5;
		if(g_Wake_Time_Count>USER_MANAGER_DELAY_TIME){
					FNS_ALL_FORM_CLOSE();
					g_Wake_Time_Count=0;
					fns_Form_redraw();
		}
		switch(DeleteState)
		{
				case DeleteMenuByVerify:         // 删除界面
				g_UserManager_Pri_Display=DeleteMenuByVerify;

				ret=ICCard_ID_Read(g_ICCard_Buff.ICCard_buf);
					if(ret==1){
						g_Wake_Time_Count=0;
						g_ICCard_Buff.ICCard_ID_Num=get_ICCard_sn(&g_ICCard_Buff);
						if(0xffff!=g_ICCard_Buff.ICCard_ID_Num){
								DeleteState=DeleteMenuByVerifyConfirm;  //确认是否删除
						}
						else{
							DeleteState=DeleteMenuByVerifyNoUser;  //卡片不存在
						}
					}
					fns_Form_redraw();
					break;
				case DeleteMenuByVerifyConfirm:
						g_UserManager_Pri_Display=DeleteMenuByVerifyConfirm;
						fns_Form_redraw();
						break;
				case DeleteMenuByVerifySuccess:  //删除成功
					if(	g_UserManager_Pri_Display!=DeleteMenuByVerifySuccess){
						g_UserManager_Pri_Display=DeleteMenuByVerifySuccess;
						pThis->timeTick=20;//2秒
						PLAY_MUSIC(ShanChuChengGong);
						fns_Form_redraw();
					}
					else{
						cotMenu_MainExit();
						fns_Form_redraw();
					}
					break;
				case DeleteMenuByVerifyFail:            // 
					if(	g_UserManager_Pri_Display!=DeleteMenuByVerifyFail){
							g_UserManager_Pri_Display=DeleteMenuByVerifyFail;
							pThis->timeTick=20;//2秒
						PLAY_MUSIC(ShanChuShiBai);
						fns_Form_redraw();
					}
					else{
								cotMenu_MainExit();
								fns_Form_redraw();
					}
					break;
				case DeleteMenuByVerifyNoUser:
					if(	g_UserManager_Pri_Display!=DeleteMenuByVerifyNoUser){
							g_UserManager_Pri_Display=DeleteMenuByVerifyNoUser;
							PLAY_MUSIC(YongHuBuCunZai);
							pThis->timeTick=20;//2秒
							fns_Form_redraw();
					}
					else{
								cotMenu_MainExit();
								fns_Form_redraw();
					}
					break;
				case DeleteMenuByVerifyInvalid:        // 
					break;
				default :
				break;
		}
}
void DeleteICUserByVerify_onCreate(BASEFORM* pThis)
{

    pThis->timeTick=2;
		g_Yes_Or_No=0;
		g_UserManager_Pri_Display=0;
		DeleteState= DeleteMenuByVerify;
		PLAY_MUSIC(YanZhengShanChuYongHu);   //请验证要删除的用户
		fns_Form_redraw();
	g_Wake_Time_Count=0;
}

void DeleteICUserByVerify_onDestroy(BASEFORM* pThis)
{
		RC522_Init();
}


/*****************************************************/

void DeletePasswdUserByVerify_onShow(BASEFORM* pThis,CDC* pFormCDC)
{
	unsigned short i,x,y;
	

	switch(DeleteState)
	{
		case DeleteMenuByVerify:             // 删除界面
			if(pThis->handlers->titleId)
			{
					y=1;
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,pThis->handlers->titleId,1,y,FONT_16X16);  //密码验证删除
			}
			y+=16;
			y+=4;
						OLED_HZ_ShowMixStr_XCentre(pFormCDC,"输入密码",1,y,FONT_24X22);
			y+=22;
			y+=4;
			x=24;
			if(Input_Key_Buf.KeyFlag)
			{
					Input_Key_Buf.KeyFlag=0;
					if(Input_Key_Buf.KeyNum)
					{
							for(i=0;i<(Input_Key_Buf.KeyNum-1);i++)
							{
									OLED_ShowChar(pFormCDC,x,y,'*',16,1,FONT_16X16);
									x+=8;
							}
								OLED_ShowNum(pFormCDC, x, y,Input_Key_Buf.KeyBuf[Input_Key_Buf.KeyNum-1],1,24,1,FONT_16X16);
								x+=8;
								i++;
							for(i;i<10;i++)
							{
									OLED_ShowChar(pFormCDC,x,y,'_',16,1,FONT_16X16);
									x+=8;
							}
					}
					else{
							for(i=0;i<10;i++){
									OLED_ShowChar(pFormCDC,x,y,'_',16,1,FONT_16X16);
									x+=8;
							}
					}
			}
			else
			{
					if(Input_Key_Buf.KeyNum<11)
					{
							for(i=0;i<Input_Key_Buf.KeyNum;i++)
							{
									OLED_ShowChar(pFormCDC,x,y,'*',16,1,FONT_16X16);
									x+=8;
							}
							for(i;i<10;i++)
							{
									OLED_ShowChar(pFormCDC,x,y,'_',16,1,FONT_16X16);
									x+=8;
							}
					}
					else{
							for(i=0;i<10;i++)
							{
									OLED_ShowChar(pFormCDC,x,y,'*',16,1,FONT_16X16);
									x+=8;
							}
					}
			}
			break;
//		case	DeleteMenuByVerifyInputErr:
//			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
//			OLED_HZ_ShowMixStr_XCentre(pFormCDC,"密码验证错误",1,y,FONT_16X16);

//			break;
		case DeleteMenuByVerifyConfirm:
					y=1;
				x=4;
				Dir4_Logo_Icon(pFormCDC,x,y);
				x=12;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
				x=108;
				Dir6_Logo_Icon(pFormCDC,x,y);
					y=24;
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"确定要删除吗\?",1,y,FONT_16X16);
					y=47;
		x=18;
					if(g_Yes_Or_No) 	OLED_YesOrNO(pFormCDC,x,y,1);
					else 	OLED_YesOrNO(pFormCDC,x,y,0);
			break;
		case DeleteMenuByVerifySuccess:   //删除成功
				y=1;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);		
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"密码删除成功",1,y,FONT_16X16);		
				break;
		case DeleteMenuByVerifyFail:           // 
				y=1;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);	
				y=21;
				x=52;
				LCD_Error_Icon(pFormCDC, x, y);		
				y=47;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"密码验证错误",1,y,FONT_16X16);		
				break;
		case DeleteMenuByVerifyNoUser:
			y=1;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"此用户不存在",1,y,FONT_16X16);
			break;
		case DeleteMenuByVerifyInvalid:       // 
				break;
		default:
				break;
	}		
}


void DeletePasswdUserByVerify_onChar(BASEFORM* pThis,int keyVal)
{
	PassWord_TypeDef passwd_input;
	unsigned short user_sn;

	switch(DeleteState)
	{
		case DeleteMenuByVerify:
			PLAY_MUSIC(7);
			switch(keyVal)
			{
				case 12:   //确认
				if(Input_Key_Buf.KeyNum>10)
				{
							memset(&Input_Key_Buf,0x00,sizeof(InputBuf_TypeDef));
							DeleteState=DeleteMenuByVerifyFail;
							fns_Form_redraw(); 
				}
				if(Input_Key_Buf.KeyNum<6){
							memset(&Input_Key_Buf,0x00,sizeof(InputBuf_TypeDef));
							DeleteState=DeleteMenuByVerifyFail;
							fns_Form_redraw(); 
				}
				else{
							DeleteState=DeleteMenuByVerifyConfirm;
							fns_Form_redraw(); 
				}
				break;
				case 10:  //取消
					if(Input_Key_Buf.KeyNum){
							Input_Key_Buf.KeyNum--;
							Input_Key_Buf.KeyFlag=1;
					}
					else  cotMenu_MainExit();	
					fns_Form_redraw(); 
					break;
				default:
					if(Input_Key_Buf.KeyNum<11)   //密码最大10位，寄到11位用于判定超出位数
					{
							if(11==keyVal)Input_Key_Buf.KeyBuf[Input_Key_Buf.KeyNum]=0;
							else Input_Key_Buf.KeyBuf[Input_Key_Buf.KeyNum]=keyVal;
							Input_Key_Buf.KeyNum++;
					}
					Input_Key_Buf.KeyFlag=1;
					fns_Form_redraw(); 
					break;
			}
			break;
//			case DeleteMenuByVerifyInputErr:
//				break;
			case DeleteMenuByVerifyConfirm:
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
											passwd_input.PassWd_Num=Input_Key_Buf.KeyNum;   //  复制给PassWord_TypeDef 的值
											passwd_input.User_Type=Admin_User;
											memcpy(passwd_input.PassWd_Buff,Input_Key_Buf.KeyBuf,passwd_input.PassWd_Num);
											user_sn=passwd_duplicate_check( &passwd_input) ;
									
											if(user_sn!=0xffff){
														if((g_Admin_Login_Info.User_Mode==PasswdMode)&&(g_Admin_Login_Info.User_ID_Num==user_sn)){
														DeleteState=DeleteMenuByVerifyFail;
														g_Yes_Or_No=0;
														fns_Form_redraw(); 
												}
												else{
														Delete_Passwd_BySn(user_sn);
														OperateLog_Info.user_type=PasswdMode;
														OperateLog_Info.clock_sec=Clock2Time(&clock_env);
														OperateLog_Info.operatetype=DELETE_USER;
														OperateLog_Info.user_num=user_sn;
														save_OperateLog(&OperateLog_Info);
														DeleteState=DeleteMenuByVerifySuccess;
												}
											}
											else{
													DeleteState=DeleteMenuByVerifyNoUser;
											}
											g_Yes_Or_No=0;
											memset(&Input_Key_Buf,0x00,sizeof(InputBuf_TypeDef));
											fns_Form_redraw(); 
									}
									else{
											cotMenu_MainExit();	
											fns_Form_redraw(); 
									}
									break;
							case 10:
									cotMenu_MainExit();	
									fns_Form_redraw(); 
									break;
							default:
							break;
					}
				break;
			
			case DeleteMenuByVerifySuccess:   //删除成功
				break;
			case DeleteMenuByVerifyFail:           // 
				break;
			case DeleteMenuByVerifyInvalid:       // 
				break;
		}
}


void DeletePasswdUserByVerify_onTick(BASEFORM* pThis)
{
	  pThis->timeTick=5;
if(g_Wake_Time_Count>USER_MANAGER_DELAY_TIME){
					FNS_ALL_FORM_CLOSE();
					g_Wake_Time_Count=0;
					fns_Form_redraw();
		}
		switch(DeleteState)
		{
			case DeleteMenuByVerify:            // 删除界面
					g_UserManager_Pri_Display=DeleteMenuByVerify;
					fns_Form_redraw();
					break;
			case DeleteMenuByVerifyConfirm:
					g_UserManager_Pri_Display=DeleteMenuByVerifyConfirm;
					fns_Form_redraw();
					break;
			case DeleteMenuByVerifySuccess:   //删除成功
					if(	g_UserManager_Pri_Display!=DeleteMenuByVerifySuccess){
							g_UserManager_Pri_Display=DeleteMenuByVerifySuccess;
							pThis->timeTick=20;//2秒
						PLAY_MUSIC(ShanChuChengGong);  //密码不存在
						fns_Form_redraw();
					}
						else
					{
								cotMenu_MainExit();
								fns_Form_redraw();
					}
					break;
			case DeleteMenuByVerifyFail:          // 
					if(	g_UserManager_Pri_Display!=DeleteMenuByVerifyFail){
							g_UserManager_Pri_Display=DeleteMenuByVerifyFail;
							pThis->timeTick=20;//2秒
						PLAY_MUSIC(ShanChuShiBai);  //密码不存在
						fns_Form_redraw();
					}
					else{
								cotMenu_MainExit();
								fns_Form_redraw();
					}
					break;
			case DeleteMenuByVerifyNoUser:
					if(	g_UserManager_Pri_Display!=DeleteMenuByVerifyNoUser){
							g_UserManager_Pri_Display=DeleteMenuByVerifyNoUser;
							pThis->timeTick=20;//2秒
							PLAY_MUSIC(YongHuBuCunZai);  //密码不存在
							fns_Form_redraw();
					}
					else{
							cotMenu_MainExit();
							fns_Form_redraw();
					}
					break;
			case DeleteMenuByVerifyInvalid: 
				if(	g_UserManager_Pri_Display!=DeleteMenuByVerifyInvalid){
							g_UserManager_Pri_Display=DeleteMenuByVerifyInvalid;
							pThis->timeTick=20;//2秒
							fns_Form_redraw();
					}
					else
					{
								cotMenu_MainExit();
								fns_Form_redraw();
					}
					
					break;
			default :
					break;
		}
}

void DeletePasswdUserByVerify_onCreate(BASEFORM* pThis)
{
		g_UserManager_Pri_Display=0;
		pThis->timeTick=2;//1.5秒
		DeleteState=DeleteMenuByVerify;
		memset(&Input_Key_Buf,0x00,sizeof(InputBuf_TypeDef));
		fns_Form_redraw();
	g_Wake_Time_Count=0;
}








/***********************************************************************/


void DeleteFingerUserByVerify_onShow(BASEFORM* pThis,CDC* pFormCDC)
{
		uint16_t x;
		uint8_t y;
	
		switch(DeleteState)
		{
			case DeleteMenuByVerify:           // 删除界面
				if(pThis->handlers->titleId)
				{
						y=1;
						OLED_HZ_ShowMixStr_XCentre(pFormCDC,pThis->handlers->titleId,1,y,FONT_16X16);
				}
			y=21;
			x=12;
			LCD_Finger_Input_Icon(pFormCDC, x, y);
			y=21;
			x=52;
			OLED_ShowMixStr(pFormCDC,x,y,"验证指纹",1,1,FONT_16X16);	
			y=37;
			OLED_ShowMixStr(pFormCDC,x,y,"请按手指",1,1,FONT_16X16);
				break;
			case DeleteMenuByVerifyConfirm:
				y=1;
				x=4;
				Dir4_Logo_Icon(pFormCDC,x,y);
				x=12;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
				x=108;
				Dir6_Logo_Icon(pFormCDC,x,y);
				y=24;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"确定要删除吗\?",1,y,FONT_16X16);
				y=47;
			x=18;
				if(g_Yes_Or_No) 	OLED_YesOrNO(pFormCDC,x,y,1);
				else 	OLED_YesOrNO(pFormCDC,x,y,0);
				break;
	
			case DeleteMenuByVerifySuccess:   //删除成功
				System_DelSuccess_Tip(pFormCDC);
				break;
			case DeleteMenuByVerifyFail:// 
				System_DelFail_Tip(pFormCDC);
				break;
			case DeleteMenuByVerifyNoUser:
				y=1;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"此用户不存在",1,y,FONT_16X16);
				break;
			case DeleteMenuByVerifyInvalid:
				break;
			default :
				break;
		}
}
void DeleteFingerUserByVerify_onChar(BASEFORM* pThis,int keyVal)
{
		Open_Lock_Info_Def open_lock_info_temp;
		switch(DeleteState){
			case DeleteMenuByVerify:           // 删除界面
					PLAY_MUSIC(7);
					switch(keyVal){
						case 12:
							break;
						case 10:
							cotMenu_MainExit();	
							fns_Form_redraw(); 
							break;
						default:
							break;
					}
					break;
				case DeleteMenuByVerifyConfirm:
					PLAY_MUSIC(7);
					switch(keyVal){
							case 4:  //确认
									g_Yes_Or_No=1;
									fns_Form_redraw();
									break;
							case 6:  //否
									g_Yes_Or_No=0;
									fns_Form_redraw();
									break;
							case 12:
									if(g_Yes_Or_No){
									get_finger_info( g_FingerPrint_Buff.Finger_Index_Table, &open_lock_info_temp);    //查找密码
										if((g_Admin_Login_Info.User_Mode==FingerPrintMode)&&(g_Admin_Login_Info.User_ID_Num==open_lock_info_temp.User_ID_Num)){
												DeleteState=DeleteMenuByVerifyFail;
												g_Yes_Or_No=0;
												
												fns_Form_redraw(); 
												break;
										}
										else{
											Delete_Finger_BySn( g_FingerPrint_Buff.Finger_Index_Table);
											DeleteState=DeleteMenuByVerifySuccess;
											OperateLog_Info.clock_sec=Clock2Time(&clock_env);
											OperateLog_Info.operatetype=DELETE_USER;
											OperateLog_Info.user_num=open_lock_info_temp.User_ID_Num;
											save_OperateLog(&OperateLog_Info);
											g_Yes_Or_No=0;
											fns_Form_redraw(); 
											break;
										}
									}
									else{
										cotMenu_MainExit();	
										fns_Form_redraw(); 
										break;
									}
							case 10:
									cotMenu_MainExit();	
									fns_Form_redraw(); 
									break;
							default:
							break;
					}
					break;
				case DeleteMenuByVerifySuccess:   //删除成功
					break;
				case DeleteMenuByVerifyFail:// 
					break;
				case DeleteMenuByVerifyNoUser:
					break;
				case DeleteMenuByVerifyInvalid:
					break;
				default :
					break;
			}
}


void DeleteFingerUserByVerify_onTick(BASEFORM* pThis)
{
		unsigned char finger_ret=0;
		
		pThis->timeTick=5;
		if(g_Wake_Time_Count>USER_MANAGER_DELAY_TIME){
					FNS_ALL_FORM_CLOSE();
					g_Wake_Time_Count=0;
					fns_Form_redraw();
		}
		switch(DeleteState)
		{
				case DeleteMenuByVerify:         // 删除界面
					g_UserManager_Pri_Display=DeleteState;
					finger_ret=AutoIdentify(&g_FingerPrint_Buff.Finger_Index_Table);
					if(ERR_NOFINGER==finger_ret){
					
					}
					else if(ERR_OK==finger_ret){
						g_Wake_Time_Count=0;
							DeleteState=DeleteMenuByVerifyConfirm;
							fns_Form_redraw();
					}
					else{
						//指纹验证失败
						g_Wake_Time_Count=0;
							DeleteState=DeleteMenuByVerifyNoUser;
							fns_Form_redraw();
					}
					break;
				case DeleteMenuByVerifyConfirm:
					g_UserManager_Pri_Display=DeleteMenuByVerifyConfirm;
						fns_Form_redraw();
						break;
				case DeleteMenuByVerifySuccess:  //删除成功
						if(	g_UserManager_Pri_Display!=DeleteMenuByVerifySuccess){
							g_UserManager_Pri_Display=DeleteMenuByVerifySuccess;
							pThis->timeTick=20;//2秒
							PLAY_MUSIC(ShanChuChengGong);  //删除成功
							fns_Form_redraw();
					}
					else{
								cotMenu_MainExit();
								fns_Form_redraw();
					}
					break;
				case DeleteMenuByVerifyFail:            // 
						if(	g_UserManager_Pri_Display!=DeleteMenuByVerifyFail){
							g_UserManager_Pri_Display=DeleteMenuByVerifyFail;
							PLAY_MUSIC(ShanChuShiBai);  //删除成功
							pThis->timeTick=20;//2秒
							fns_Form_redraw();
					}
					else{
								cotMenu_MainExit();
								fns_Form_redraw();
					}
					break;
				case	DeleteMenuByVerifyNoUser:
						if(	g_UserManager_Pri_Display!=DeleteMenuByVerifyNoUser){
							g_UserManager_Pri_Display=DeleteMenuByVerifyNoUser;
							pThis->timeTick=20;//2秒
							PLAY_MUSIC(YongHuBuCunZai);  //删除成功
							fns_Form_redraw();
					}
					else{
								cotMenu_MainExit();
								fns_Form_redraw();
					}
				break;		
				case DeleteMenuByVerifyInvalid:        // 
					break;
				default :
				break;
		}
}



void DeleteFingerUserByVerify_onCreate(BASEFORM* pThis)
{
    pThis->timeTick=2;
		g_Yes_Or_No=0;
		g_UserManager_Pri_Display=0;
		DeleteState= DeleteMenuByVerify;
		fns_Form_redraw();
	g_Wake_Time_Count=0;
}





















/***********************************************************************/


















								




/***********************************************************************/

#endif
