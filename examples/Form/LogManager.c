#if 1

#include "LogManager.h"
#include "MenuForm.h"
#include "CDC.h"
#include <string.h>
#include <stdio.h>
#include "GUI_Form_Fun.h"
#include "peripheral_fun_share.h"
#include "date_time.h"
#include "fcl_flash.h"
#include "Form.h"
#include <time.h>
/*************************************************/

unsigned char g_Log_Pri_Display=0;
InputBuf_TypeDef Log_Input_Key_Buf,Log_Input_Key_Buf_Temp;

uint8_t LogManagerInfoStr[] ={"日志管理"};

void LogManagerInfo_onCreate(BASEFORM* pThis);
void LogManagerInfo_onShow(BASEFORM* pThis,CDC* pFormCDC);
void LogManagerInfo_onChar(BASEFORM* pThis,int keyVal);
void LogManagerInfo_onTick(BASEFORM* pThis);

const FORM_HANDLERS s_formcode_LogManagerInfo={
		sizeof(MENUFORM),
		LogManagerInfoStr,
    fns_NullForm_onDefault,
    LogManagerInfo_onShow,
    LogManagerInfo_onChar,
    LogManagerInfo_onTick,
		LogManagerInfo_onCreate,
    fns_NullForm_onDestroy,
    fns_NullForm_onFocus
};

void LogManagerInfo_onShow(BASEFORM* pThis,CDC* pFormCDC){
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

void LogManagerInfo_onChar(BASEFORM* pThis,int keyVal){
    fns_MenuForm_onChar(pThis,keyVal);
}

void LogManagerInfo_onTick(BASEFORM* pThis){
    pThis->timeTick=2;
		if(g_Wake_Time_Count>LOGD_DELAY_TIME){
			FNS_ALL_FORM_CLOSE();
			g_Wake_Time_Count=0;
			fns_Form_redraw();
		}
    fns_Form_redraw();
}

void OpenLogInfo(MENUFORM* pMenuForm);
void OperateLogInfo(MENUFORM* pMenuForm);
void AlarmLogInfo(MENUFORM* pMenuForm);
void CleanLogInfo(MENUFORM* pMenuForm);

const MENUITEM s_LogManagerInfoMenu[]={
		{(uint8_t*)"开门日志",OpenLogInfo,NULL}, 
		{(uint8_t*)"操作日志",OperateLogInfo,NULL}, 
		{(uint8_t*)"报警日志",AlarmLogInfo,NULL}, 
		{(uint8_t*)"清空日志",CleanLogInfo,NULL},
};

void LogManagerInfo_onCreate(BASEFORM* pThis){   
    MENUFORM* pMenuForm=(MENUFORM*)pThis;
    pMenuForm->menuItems=s_LogManagerInfoMenu;
    pMenuForm->itemCount=sizeof(s_LogManagerInfoMenu)/sizeof(MENUITEM);
    pMenuForm->firstIndex=0;
		pMenuForm->curIndex=0;
    pMenuForm->timeTick=2;
	g_Wake_Time_Count=0;
}
/*******************************************************************/

uint8_t OpenLogInfoStr[]={"开门日志查询"};

clock_param_t clock_env_start,clock_env_end;
OpenLogByDateStateDef OpenLogByDateStatus;

void OpenLogInfo_onShow(BASEFORM* pThis,CDC* pFormCDC);
void OpenLogInfo_onChar(BASEFORM* pThis,int keyVal);
void OpenLogInfo_onTick(BASEFORM* pThis);
void OpenLogInfo_onCreate(BASEFORM* pThis);

const FORM_HANDLERS s_formcode_OpenLogInfo={
		sizeof(MENUFORM),
		OpenLogInfoStr,
		fns_NullForm_onDefault,
		OpenLogInfo_onShow,
		OpenLogInfo_onChar,
		OpenLogInfo_onTick,
		OpenLogInfo_onCreate,
		fns_NullForm_onDestroy,
		fns_NullForm_onFocus	
};


void OpenLogInfo(MENUFORM* pMenuForm)
{
		fns_Form_create(&s_formcode_OpenLogInfo);
}


void OpenLogInfo_onShow(BASEFORM* pThis,CDC* pFormCDC)
{
    uint8_t y;
		if(pThis->handlers->titleId){
				y=1;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,pThis->handlers->titleId,1,y,FONT_16X16);
		}
    fns_MenuForm_onShow(pThis,pFormCDC);
}

void OpenLogInfo_onChar(BASEFORM* pThis,int keyVal)
{
    fns_MenuForm_onChar(pThis,keyVal);
}

void OpenLogInfo_onTick(BASEFORM* pThis)
{
		pThis->timeTick=2;
		if(g_Wake_Time_Count>LOGD_DELAY_TIME){
			FNS_ALL_FORM_CLOSE();
			g_Wake_Time_Count=0;
			fns_Form_redraw();
		}
		fns_Form_redraw();
}




void OpenLogByDate(MENUFORM* pMenuForm);
void OpenLogByNum(MENUFORM* pMenuForm);


const MENUITEM s_OpenLogInfoMenu[]={
		{(uint8_t*)"日期查询",OpenLogByDate,NULL}, 
		{(uint8_t*)"编号查询",OpenLogByNum,NULL}, 
};

void OpenLogInfo_onCreate(BASEFORM* pThis){   
    MENUFORM* pMenuForm=(MENUFORM*)pThis;
    pMenuForm->menuItems=s_OpenLogInfoMenu;
    pMenuForm->itemCount=sizeof(s_OpenLogInfoMenu)/sizeof(MENUITEM);
    pMenuForm->firstIndex=0;
		pMenuForm->curIndex=0;
    pMenuForm->timeTick=2;
	g_Wake_Time_Count=0;
}


uint8_t OpenLogByDateStr[]={"请输入查询日期"};
clock_param_t clock_env_start,clock_env_end; 
OpenLogStruts OpenLog_Buff;
unsigned char OpenLogDateSetPos;
void OpenLogByDate_onShow(BASEFORM* pThis,CDC* pFormCDC);
void OpenLogByDate_onChar(BASEFORM* pThis,int keyVal);
void OpenLogByDate_onTick(BASEFORM* pThis);
void OpenLogByDate_onCreate(BASEFORM* pThis);


const FORM_HANDLERS s_formcode_OpenLogByDate={
		sizeof(MENUFORM),
		OpenLogByDateStr,
		fns_NullForm_onDefault,
		OpenLogByDate_onShow,
		OpenLogByDate_onChar,
		OpenLogByDate_onTick,
		OpenLogByDate_onCreate,
		fns_NullForm_onDestroy,
		fns_NullForm_onFocus	
};


void OpenLogByDate(MENUFORM* pMenuForm)
{
		fns_Form_create(&s_formcode_OpenLogByDate);
}


void OpenLogByDate_onShow(BASEFORM* pThis,CDC* pFormCDC)
{
		unsigned char x,y;
		clock_param_t  clock_temp;
		unsigned char str_buff[10];
	
		switch(OpenLogByDateStatus)
		{
			case OpenLogDateSet:   
				if(pThis->handlers->titleId){
						y=1;
						OLED_HZ_ShowMixStr_XCentre(pFormCDC,pThis->handlers->titleId,1,y,FONT_16X16);
				}
				y+=20;
				x=8;
				if(0==OpenLogDateSetPos) OLED_ShowNum(pFormCDC,x,y,(clock_env_start.year-2000),2,16,0,FONT_16X16);
				else OLED_ShowNum(pFormCDC,x,y,clock_env_start.year,2,16,1,FONT_16X16);
				x+=16;
				OLED_ShowMixStr(pFormCDC,x,y,"年",1,1,FONT_16X16);
				x+=16;
				if(1==OpenLogDateSetPos) OLED_ShowNum(pFormCDC,x,y,clock_env_start.month,2,16,0,FONT_16X16);
				else OLED_ShowNum(pFormCDC,x,y,clock_env_start.month,2,16,1,FONT_16X16);
				x+=16;
				OLED_ShowMixStr(pFormCDC,x,y,"月",1,1,FONT_16X16);
				x+=16;
				if(2==OpenLogDateSetPos) OLED_ShowNum(pFormCDC,x,y,clock_env_start.day,2,16,0,FONT_16X16);
				else OLED_ShowNum(pFormCDC,x,y,clock_env_start.day,2,16,1,FONT_16X16);
				x+=16;
				OLED_ShowMixStr(pFormCDC,x,y,"日",1,1,FONT_16X16);
				x+=16;
				y+=8;
				OLED_ShowMixStr(pFormCDC,x,y,"至",1,1,FONT_16X16);
				x=8;
				y+=10;
				if(3==OpenLogDateSetPos) OLED_ShowNum(pFormCDC,x,y,clock_env_end.year,2,16,0,FONT_16X16);
				else OLED_ShowNum(pFormCDC,x,y,clock_env_end.year,2,16,1,FONT_16X16);
				x+=16;
				OLED_ShowMixStr(pFormCDC,x,y,"年",1,1,FONT_16X16);
				x+=16;
				if(4==OpenLogDateSetPos) OLED_ShowNum(pFormCDC,x,y,clock_env_end.month,2,16,0,FONT_16X16);
				else OLED_ShowNum(pFormCDC,x,y,clock_env_end.month,2,16,1,FONT_16X16);
				x+=16;
				OLED_ShowMixStr(pFormCDC,x,y,"月",1,1,FONT_16X16);
				x+=16;
				if(5==OpenLogDateSetPos) OLED_ShowNum(pFormCDC,x,y,clock_env_end.day,2,16,0,FONT_16X16);
				else OLED_ShowNum(pFormCDC,x,y,clock_env_end.day,2,16,1,FONT_16X16);
				x+=16;
				OLED_ShowMixStr(pFormCDC,x,y,"日",1,1,FONT_16X16);
				break;
			case OpenLogDateQuery:	
				y=1;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"正在查询",1,y,FONT_16X16);
				break;
			case OpenLogDateResult:
				x=8;
				y=0;
				OLED_ShowMixStr(pFormCDC,x,y,"编号:",1,1,FONT_16X16);
				x+=40;
				OLED_ShowNum(pFormCDC,x,y,OpenLog_Buff.user_num,4,16,1,FONT_16X16);
			
				x=8;
				y+=16;
				OLED_ShowMixStr(pFormCDC,x,y,"类型:",1,1,FONT_16X16);
				x+=40;
				switch(OpenLog_Buff.user_type){
					case 	Admin_User:  
						OLED_ShowMixStr(pFormCDC,x,y,"管理员",1,1,FONT_16X16);
						break;
					case  Normal_User:
						OLED_ShowMixStr(pFormCDC,x,y,"普通用户",1,1,FONT_16X16);
						break;
					case  Guest_User:
						OLED_ShowMixStr(pFormCDC,x,y,"宾客用户",1,1,FONT_16X16);
						break;
					case Temporary_User:
						OLED_ShowMixStr(pFormCDC,x,y,"临时",1,1,FONT_16X16);
						break;
				}
				
				x=8;
				y+=16;
				OLED_ShowMixStr(pFormCDC,x,y,"日期:",1,1,FONT_16X16);
				x+=40;
				TimeSec2Clock(&OpenLog_Buff.clock_sec,&clock_temp);
				sprintf((char *)str_buff,"%02d/%02d/%02d",(clock_temp.year-2000),clock_temp.month,clock_temp.day);
				OLED_ShowMixStr(pFormCDC,x,y,str_buff,1,1,FONT_16X16);

				x=8;
				y+=16;
				OLED_ShowMixStr(pFormCDC,x,y,"时间:",1,1,FONT_16X16);
				x+=40;
				sprintf((char *)str_buff,"%02d:%02d:%02d",(clock_temp.hour),clock_temp.min,clock_temp.sec);
				OLED_ShowMixStr(pFormCDC,x,y,str_buff,1,1,FONT_16X16);

				break;
			case OpenLogDateFail:
				y=1;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"无信息查询",1,y,FONT_16X16);
			default :
				break;
		}		
}

void OpenLogByDate_onChar(BASEFORM* pThis,int keyVal)
{
		unsigned char ret;
		g_Wake_Time_Count=0;
		switch(OpenLogByDateStatus)
		{
			case OpenLogDateSet:
				PLAY_MUSIC(7);
				switch(keyVal){
					case 2:
						switch(OpenLogDateSetPos){
							case 0:
								if(clock_env_start.year>2098)clock_env_start.year=2000;
								else clock_env_start.year++;
								break;
							case 1:
								if(clock_env_start.month>11)clock_env_start.month=1;
								else clock_env_start.month++;
								break;
							case 2:
								if(clock_env_start.day>30)clock_env_start.day=1;
								else clock_env_start.day++;
								break;
							case 3:
								if(clock_env_end.year>2098)clock_env_end.year=2000;
								else clock_env_end.year++;
								break;
							case 4:
								if(clock_env_end.month>11)clock_env_end.month=1;
								else clock_env_end.month++;
								break;
							case 5:
								if(clock_env_end.day>30)clock_env_end.day =1;
								else clock_env_end.day++;
								break;
							default:
								break;
						}
						break;
					case 4:
						if(OpenLogDateSetPos) OpenLogDateSetPos--;
					else OpenLogDateSetPos=5;
						break;
					case 6:
						if(OpenLogDateSetPos>4) OpenLogDateSetPos=0;
					else OpenLogDateSetPos++;
						break;
					case 8:
						switch(OpenLogDateSetPos){
							case 0:
								if(clock_env_start.year<2001)clock_env_start.year=2099;
								else clock_env_start.year--;
								break;
							case 1:
								if(clock_env_start.month<2)clock_env_start.month=12;
								else clock_env_start.month--;
								break;
							case 2:
								if(clock_env_start.day<2)clock_env_start.day=31;
								else clock_env_start.day--;
								break;
							case 3:
								if(clock_env_end.year<2001)clock_env_end.year=2099;
								else clock_env_end.year--;
								break;
							case 4:
								if(clock_env_end.month<2)clock_env_end.month=12;
								else clock_env_end.month--;
								break;
							case 5:
								if(clock_env_end.day<2)clock_env_end.day =31;
								else clock_env_end.day--;
								break;
							default:
								break;
						}
						break;
					case 12:   //确认
		
						ret= get_OpenLog_By_Time(&clock_env_start,&clock_env_end,&OpenLog_Buff);
						if(ret){
							OpenLogByDateStatus=OpenLogDateResult;
						}
						else{
							OpenLogByDateStatus=OpenLogDateFail;
						}
						fns_Form_redraw(); 
						break;
					case 10:  //取消
						cotMenu_MainExit();	
						fns_Form_redraw(); 
						break;
					default:
						break;
				}
				
				fns_Form_redraw();
				break;
			case OpenLogDateQuery:
				break;
      case OpenLogDateResult:
				PLAY_MUSIC(7);				
				switch(keyVal)
				{
					case 2:
						g_Wait_Time_Count=50;
						ret=get_OpenLog_ByTime_Pri(&OpenLog_Buff);
						if(ret) OpenLogByDateStatus=OpenLogDateResult;
							else OpenLogByDateStatus=OpenLogDateFail;
						fns_Form_redraw(); 
						break;
					case 8:
						g_Wait_Time_Count=50;
						ret=get_OpenLog_ByTime_Next(&OpenLog_Buff);
						if(ret) OpenLogByDateStatus=OpenLogDateResult;
							else OpenLogByDateStatus=OpenLogDateFail;
						fns_Form_redraw(); 
						break;
					case 10:
						cotMenu_MainExit();	
						fns_Form_redraw();
						break;
					default :
						break;
				}
				break;
			case OpenLogDateFail:
				PLAY_MUSIC(7);
				switch(keyVal)
				{
					case 2:
						ret=get_OpenLog_ByTime_Pri(&OpenLog_Buff);
						if(ret) OpenLogByDateStatus=OpenLogDateResult;
							else OpenLogByDateStatus=OpenLogDateFail;
						fns_Form_redraw(); 
						break;
					case 8:
						ret=get_OpenLog_ByTime_Next(&OpenLog_Buff);
						if(ret) OpenLogByDateStatus=OpenLogDateResult;
							else OpenLogByDateStatus=OpenLogDateFail;
						fns_Form_redraw(); 
						break;
					case 10:
						cotMenu_MainExit();	
						fns_Form_redraw();
						break;
					default :
						break;
				}
				break;
			default :
				break;
		}
}

void OpenLogByDate_onTick(BASEFORM* pThis)
{
		pThis->timeTick=2;
		if(g_Wake_Time_Count>LOGD_DELAY_TIME){
				FNS_ALL_FORM_CLOSE();
				g_Wake_Time_Count=0;
				fns_Form_redraw();
		}
		
		switch(OpenLogByDateStatus)		{
			case OpenLogDateSet:
				fns_Form_redraw();
				break;
			case OpenLogDateQuery:
				break;
			case OpenLogDateResult:
				if(	g_Log_Pri_Display!=OpenLogDateResult){
							g_Log_Pri_Display=OpenLogDateResult;
							pThis->timeTick=20;//2秒
							g_Wake_Time_Count=0;
							g_Wait_Time_Count=50;
							fns_Form_redraw();
					}
					else{
							if(!g_Wait_Time_Count){
								cotMenu_MainExit();
							}
							fns_Form_redraw();
					}
				break;
			case OpenLogDateFail:
				if(	g_Log_Pri_Display!=OpenLogDateFail){
							g_Log_Pri_Display=OpenLogDateFail;
							pThis->timeTick=20;//2秒
							g_Wake_Time_Count=0;
							fns_Form_redraw();
					}
					else{
							if(!g_Wait_Time_Count){
								cotMenu_MainExit();
							}
							fns_Form_redraw();
					}
				break;
			default:
				break;
		}
}

void OpenLogByDate_onCreate(BASEFORM* pThis)
{
		OpenLogDateSetPos=0;
		memset(&clock_env_start,0,sizeof(clock_param_t));
		clock_env_start.year=2021;
		clock_env_start.month=1;
		clock_env_start.day=1;
		clock_env_end.year=clock_env.year;
		clock_env_end.month=clock_env.month;
		clock_env_end.day=clock_env.day;
		clock_env_end.hour=clock_env.hour;
		clock_env_end.min=clock_env.min;
		clock_env_end.sec=clock_env.sec;
		pThis->timeTick=2;
		g_Yes_Or_No=0;
		g_Log_Pri_Display=0;
		OpenLogByDateStatus=OpenLogDateSet;
		fns_Form_redraw();
	g_Wake_Time_Count=0;
}



AlarmLogStruts AlarmLogMsg;

uint8_t OpenLogByNumStr[]={"日志查询"};

OpenLogByNumStateDef OpenLogNmuStatus;
unsigned int g_user_sn;
void OpenLogByNum_onShow(BASEFORM* pThis,CDC* pFormCDC);
void OpenLogByNum_onChar(BASEFORM* pThis,int keyVal);
void OpenLogByNum_onTick(BASEFORM* pThis);
void OpenLogByNum_onCreate(BASEFORM* pThis);

const FORM_HANDLERS s_formcode_OpenLogByNum={
		sizeof(MENUFORM),
		OpenLogByNumStr,
		fns_NullForm_onDefault,
		OpenLogByNum_onShow,
		OpenLogByNum_onChar,
		OpenLogByNum_onTick,
		OpenLogByNum_onCreate,
		fns_NullForm_onDestroy,
		fns_NullForm_onFocus	
};


void OpenLogByNum(MENUFORM* pMenuForm)
{
		fns_Form_create(&s_formcode_OpenLogByNum);
}


void OpenLogByNum_onShow(BASEFORM* pThis,CDC* pFormCDC)
{
		unsigned char x,y,i;
		clock_param_t  clock_temp;
		unsigned char str_buff[10];
		switch(OpenLogNmuStatus)
		{
			case OpenLogNumSet: 
				if(pThis->handlers->titleId){
						y=1;
						OLED_HZ_ShowMixStr_XCentre(pFormCDC,pThis->handlers->titleId,1,y,FONT_16X16);
				}
				y+=20;				
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"请输入查询的编号",1,y,FONT_16X16);
				x=12;
				y+=16;
				y+=2;
				OLED_ShowMixStr(pFormCDC,x,y,"用户编号:",1,1,FONT_16X16);
				x+=72;
				if(Log_Input_Key_Buf.KeyNum){
						for(i=0;i<Log_Input_Key_Buf.KeyNum;i++){
								OLED_ShowNum(pFormCDC, x, y,Log_Input_Key_Buf.KeyBuf[i],1,16,0,FONT_16X16);
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
			case OpenLogNumQuery:	
				y=1;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"正在查询",1,y,FONT_16X16);
				break;
			case OpenLogNumResult:
				x=8;
				y=0;
				OLED_ShowMixStr(pFormCDC,x,y,"编号:",1,1,FONT_16X16);
				x+=40;
				OLED_ShowNum(pFormCDC,x,y,OpenLog_Buff.user_num,4,16,1,FONT_16X16);
			
				x=8;
				y+=16;
				OLED_ShowMixStr(pFormCDC,x,y,"类型:",1,1,FONT_16X16);
				x+=40;
				switch(OpenLog_Buff.user_type){
					case 	Admin_User:  
						OLED_ShowMixStr(pFormCDC,x,y,"管理员",1,1,FONT_16X16);
						break;
					case  Normal_User:
						OLED_ShowMixStr(pFormCDC,x,y,"普通用户",1,1,FONT_16X16);
						break;
					case  Guest_User:
						OLED_ShowMixStr(pFormCDC,x,y,"宾客用户",1,1,FONT_16X16);
						break;
					case Temporary_User:
						OLED_ShowMixStr(pFormCDC,x,y,"临时",1,1,FONT_16X16);
					break;
				}
				
				x=8;
				y+=16;
				OLED_ShowMixStr(pFormCDC,x,y,"日期:",1,1,FONT_16X16);
				x+=40;
				TimeSec2Clock(&OpenLog_Buff.clock_sec,&clock_temp);
			
				sprintf((char *)str_buff,"%02d/%02d/%02d",(clock_temp.year-2000),clock_temp.month,clock_temp.day);
				OLED_ShowMixStr(pFormCDC,x,y,str_buff,1,1,FONT_16X16);

				x=8;
				y+=16;
				OLED_ShowMixStr(pFormCDC,x,y,"时间:",1,1,FONT_16X16);
				x+=40;
				sprintf((char *)str_buff,"%02d:%02d:%02d",(clock_temp.hour),clock_temp.min,clock_temp.sec);
				OLED_ShowMixStr(pFormCDC,x,y,str_buff,1,1,FONT_16X16);
				break;
			case OpenLogNumFail:
				y=1;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"无信息查询",1,y,FONT_16X16);
			default :
				break;
		}		
}

void OpenLogByNum_onChar(BASEFORM* pThis,int keyVal)
{
		unsigned char ret;
		unsigned char i;
		g_Wake_Time_Count=0;
		switch(OpenLogNmuStatus)
		{
				case OpenLogNumSet:
				PLAY_MUSIC(7);
				switch(keyVal)
				{
					case 12:   //确认
					if(Log_Input_Key_Buf.KeyNum){
						g_user_sn=0;
							for(i=0;i<Log_Input_Key_Buf.KeyNum;i++)   //计算输入的编号
							{
									g_user_sn=g_user_sn*10+Log_Input_Key_Buf.KeyBuf[i];
							}
							ret=get_OpenLog_BySN(&OpenLog_Buff,g_user_sn);
							if(ret) OpenLogNmuStatus=OpenLogNumResult;
							else   OpenLogNmuStatus=OpenLogNumFail;
							fns_Form_redraw(); 
							memset(&Log_Input_Key_Buf,0x00,sizeof(InputBuf_TypeDef));
					}
					else{
							memset(&Log_Input_Key_Buf,0x00,sizeof(InputBuf_TypeDef));
							cotMenu_MainExit();	
					}

					break;
					case 10:  //取消
						memset(&Log_Input_Key_Buf,0x00,sizeof(InputBuf_TypeDef));
						cotMenu_MainExit();	
						fns_Form_redraw(); 
						break;
					default:
						if(Log_Input_Key_Buf.KeyNum<4)   //密码最大10位，寄到11位用于判定超出位数
						{
								if(11==keyVal)Log_Input_Key_Buf.KeyBuf[Log_Input_Key_Buf.KeyNum]=0;
								else Log_Input_Key_Buf.KeyBuf[Log_Input_Key_Buf.KeyNum]=keyVal;
								Log_Input_Key_Buf.KeyNum++;
						}
						else{
								Log_Input_Key_Buf.KeyNum=0;
								if(11==keyVal)Log_Input_Key_Buf.KeyBuf[Log_Input_Key_Buf.KeyNum]=0;
								else Log_Input_Key_Buf.KeyBuf[Log_Input_Key_Buf.KeyNum]=keyVal;
								Log_Input_Key_Buf.KeyNum++;
						}
						Log_Input_Key_Buf.KeyFlag=1;
						fns_Form_redraw(); 
						break;
				}
				fns_Form_redraw();
				break;
			case OpenLogNumQuery:
				break;
      case OpenLogNumResult:  
				PLAY_MUSIC(7);				
				switch(keyVal){
					case 2:
						g_Wait_Time_Count=50;
						ret=get_OpenLog_BySN_Pri(&OpenLog_Buff,g_user_sn);
						if(ret) OpenLogNmuStatus=OpenLogNumResult;
							else OpenLogNmuStatus=OpenLogNumFail;
						fns_Form_redraw(); 
						break;
					case 8:
						g_Wait_Time_Count=50;
						ret=get_OpenLog_BySN_Next(&OpenLog_Buff,g_user_sn);
							if(ret) OpenLogNmuStatus=OpenLogNumResult;
							else OpenLogNmuStatus=OpenLogNumFail;
						fns_Form_redraw(); 
						break;
					case 10:
						cotMenu_MainExit();	
						fns_Form_redraw();
						break;
					default :
						break;
				}
				break;
			case OpenLogNumFail:
				PLAY_MUSIC(7);
				switch(keyVal)
				{
					case 2:
						g_Wait_Time_Count=50;
						ret=get_OpenLog_BySN_Pri(&OpenLog_Buff,g_user_sn);
						if(ret) OpenLogNmuStatus=OpenLogNumResult;
							else OpenLogNmuStatus=OpenLogNumFail;
						fns_Form_redraw(); 
						break;
					case 8:
						g_Wait_Time_Count=50;
						ret=get_OpenLog_BySN_Next(&OpenLog_Buff,g_user_sn);
						if(ret) OpenLogNmuStatus=OpenLogNumResult;
							else OpenLogNmuStatus=OpenLogNumFail;
						fns_Form_redraw(); 
						break;
					case 10:
						cotMenu_MainExit();	
						fns_Form_redraw();
						break;
					default :
						break;
				}
				break;
			default :
				break;
		}
}

void OpenLogByNum_onTick(BASEFORM* pThis)
{
	pThis->timeTick=2;
	if(g_Wake_Time_Count>LOGD_DELAY_TIME){
			FNS_ALL_FORM_CLOSE();
			g_Wake_Time_Count=0;
			fns_Form_redraw();
	}
	switch(OpenLogNmuStatus)
	{

		case OpenLogNumSet:
			break;
		case 	OpenLogNumQuery:
			break;
		case OpenLogNumResult:
			if(	g_Log_Pri_Display!=OpenLogNumResult){
							g_Log_Pri_Display=OpenLogNumResult;
							pThis->timeTick=50;//2秒
							g_Wake_Time_Count=0;
							g_Wait_Time_Count=50;
							fns_Form_redraw();
					}
					else{
							if(!g_Wait_Time_Count){
								cotMenu_MainExit();
							}
						fns_Form_redraw();
					}
			break;
		case OpenLogNumFail:
			if(	g_Log_Pri_Display!=OpenLogNumFail){
							g_Log_Pri_Display=OpenLogNumFail;
							pThis->timeTick=50;//2秒
							g_Wake_Time_Count=0;
							fns_Form_redraw();
					}
					else{
							if(!g_Wait_Time_Count){
								cotMenu_MainExit();
							}
							fns_Form_redraw();
					}
		default:
			break;
	
	}
	


}

void OpenLogByNum_onCreate(BASEFORM* pThis)
{

		pThis->timeTick=2;
		g_Yes_Or_No=0;
		g_Log_Pri_Display=0;
		fns_Form_redraw();
	OpenLogNmuStatus=OpenLogNumSet;
	g_Wake_Time_Count=0;
}


/*******************************************************************/
uint8_t OperateLogInfoStr[]={"操作日志"};
OperateLogStateDef OperateLogStatus;
OperateLogStruts OperateLogMsg;
void OperateLogInfo_onShow(BASEFORM* pThis,CDC* pFormCDC);
void OperateLogInfo_onChar(BASEFORM* pThis,int keyVal);
void OperateLogInfo_onTick(BASEFORM* pThis);
void OperateLogInfo_onCreate(BASEFORM* pThis);

const FORM_HANDLERS s_formcode_OperateLogInfo={
		sizeof(MENUFORM),
		OperateLogInfoStr,
		fns_NullForm_onDefault,
		OperateLogInfo_onShow,
		OperateLogInfo_onChar,
		OperateLogInfo_onTick,
		OperateLogInfo_onCreate,
		fns_NullForm_onDestroy,
		fns_NullForm_onFocus	
};


void OperateLogInfo(MENUFORM* pMenuForm)
{
		fns_Form_create(&s_formcode_OperateLogInfo);
}


void OperateLogInfo_onShow(BASEFORM* pThis,CDC* pFormCDC)
{
		unsigned char x,y;
		clock_param_t  clock_temp;
		unsigned char str_buff[10];
		switch(OperateLogStatus)
		{
			case OperateLogQuery:	
				y=1;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"正在查询",1,y,FONT_16X16);
				break;
			case OperateLogResult:
				x=8;
				y=0;
				OLED_ShowMixStr(pFormCDC,x,y,"管理:",1,1,FONT_16X16);
				x+=40;
				OLED_ShowNum(pFormCDC,x,y,OperateLogMsg.admin_num,4,16,1,FONT_16X16);
				
				x=8;
				y+=16;
				OLED_ShowMixStr(pFormCDC,x,y,"日期:",1,1,FONT_16X16);
				x+=40;
				TimeSec2Clock(&OperateLogMsg.clock_sec,&clock_temp);
				sprintf((char *)str_buff,"%02d/%02d/%02d",(clock_temp.year-2000),clock_temp.month,clock_temp.day);
				OLED_ShowMixStr(pFormCDC,x,y,str_buff,1,1,FONT_16X16);

				x=8;
				y+=16;
				OLED_ShowMixStr(pFormCDC,x,y,"时间:",1,1,FONT_16X16);
				x+=40;
			sprintf((char *)str_buff,"%02d:%02d:%02d",(clock_temp.hour),clock_temp.min,clock_temp.sec);
				OLED_ShowMixStr(pFormCDC,x,y,str_buff,1,1,FONT_16X16);
			
				x=8;
				y+=16;

			  if(ADD_USER==OperateLogMsg.operatetype) //添加
				{
						OLED_ShowMixStr(pFormCDC,x,y,"添加:",1,1,FONT_16X16);
						x+=40;
						OLED_ShowNum(pFormCDC,x,y,OperateLogMsg.user_num,4,16,1,FONT_16X16);
						x+=40;

						if(FingerPrintMode==OperateLogMsg.user_type){
								OLED_ShowMixStr(pFormCDC,x,y,"指纹",1,1,FONT_16X16);
						}
						else if(PasswdMode==OperateLogMsg.user_type){
								OLED_ShowMixStr(pFormCDC,x,y,"密码",1,1,FONT_16X16);
						}
						else if(ICCardMode==OperateLogMsg.user_type){
								OLED_ShowMixStr(pFormCDC,x,y,"卡片",1,1,FONT_16X16);
						}
				}
				else if(DELETE_USER==OperateLogMsg.operatetype){
						OLED_ShowMixStr(pFormCDC,x,y,"删除:",1,1,FONT_16X16);
						x+=40;
						OLED_ShowNum(pFormCDC,x,y,OperateLogMsg.user_num,4,16,1,FONT_16X16);
						x+=40;

						if(FingerPrintMode==OperateLogMsg.user_type){
								OLED_ShowMixStr(pFormCDC,x,y,"指纹",1,1,FONT_16X16);
						}
						else if(PasswdMode==OperateLogMsg.user_type){
								OLED_ShowMixStr(pFormCDC,x,y,"密码",1,1,FONT_16X16);
						}
						else if(ICCardMode==OperateLogMsg.user_type){
								OLED_ShowMixStr(pFormCDC,x,y,"卡片",1,1,FONT_16X16);
						}
				}
				else if(DELETE_ADMIN==OperateLogMsg.operatetype){
						OLED_ShowMixStr(pFormCDC,x,y,"删管理区:",1,1,FONT_16X16);
						x+=80;					
						if(FingerPrintMode==OperateLogMsg.user_type){
								OLED_ShowMixStr(pFormCDC,x,y,"指纹",1,1,FONT_16X16);
						}
						else if(PasswdMode==OperateLogMsg.user_type){
								OLED_ShowMixStr(pFormCDC,x,y,"密码",1,1,FONT_16X16);
						}
						else if(ICCardMode==OperateLogMsg.user_type){
								OLED_ShowMixStr(pFormCDC,x,y,"卡片",1,1,FONT_16X16);
						}
				}
				else if(DELETE_NORMAL==OperateLogMsg.operatetype){
						OLED_ShowMixStr(pFormCDC,x,y,"删普通区:",1,1,FONT_16X16);
						x+=80;
						if(FingerPrintMode==OperateLogMsg.user_type){
								OLED_ShowMixStr(pFormCDC,x,y,"指纹",1,1,FONT_16X16);
						}
						else if(PasswdMode==OperateLogMsg.user_type){
								OLED_ShowMixStr(pFormCDC,x,y,"密码",1,1,FONT_16X16);
						}
						else if(ICCardMode==OperateLogMsg.user_type){
								OLED_ShowMixStr(pFormCDC,x,y,"卡片",1,1,FONT_16X16);
						}
				}
				else if(DELETE_GUEST==OperateLogMsg.operatetype){
						
						OLED_ShowMixStr(pFormCDC,x,y,"删宾客区:",1,1,FONT_16X16);
						x+=80;		
						if(FingerPrintMode==OperateLogMsg.user_type){
								OLED_ShowMixStr(pFormCDC,x,y,"指纹",1,1,FONT_16X16);
						}
						else if(PasswdMode==OperateLogMsg.user_type){
								OLED_ShowMixStr(pFormCDC,x,y,"密码",1,1,FONT_16X16);
						}
						else if(ICCardMode==OperateLogMsg.user_type){
								OLED_ShowMixStr(pFormCDC,x,y,"卡片",1,1,FONT_16X16);
						}
				}
				break;
			case OperateLogFail:
				y=1;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"无信息查询",1,y,FONT_16X16);
			default :
				break;
		}		
}



void OperateLogInfo_onChar(BASEFORM* pThis,int keyVal)
{
		unsigned char ret;
		g_Wake_Time_Count=0;
		switch(OperateLogStatus)
		{
			case OperateLogSet:
			PLAY_MUSIC(7);

				break;
			case OperateLogQuery:
				break;
      case OperateLogResult:   
				PLAY_MUSIC(7);
				switch(keyVal){
					case 2:
					g_Wait_Time_Count=50;
						ret=get_Operate_Log_Pri(&OperateLogMsg);
						if(ret) OperateLogStatus=OperateLogResult;
							else OperateLogStatus=OperateLogFail;
						fns_Form_redraw(); 
						break;
					case 8:
						g_Wait_Time_Count=50;
						ret=get_Operate_Log_Next(&OperateLogMsg);
						if(ret) OperateLogStatus=OperateLogResult;
							else OperateLogStatus=OperateLogFail;
						fns_Form_redraw(); 
						break;
					case 10:
						cotMenu_MainExit();	
						fns_Form_redraw();
						break;
					default :
						break;
				}
				break;
			case OperateLogFail:
				PLAY_MUSIC(7);
				switch(keyVal){
					case 2:
					
						ret=get_Operate_Log_Pri(&OperateLogMsg);
						if(ret) OperateLogStatus=OperateLogResult;
							else OperateLogStatus=OperateLogFail;
						fns_Form_redraw(); 
						break;
					case 8:
						ret=get_Operate_Log_Next(&OperateLogMsg);
						if(ret) OperateLogStatus=OperateLogResult;
							else OperateLogStatus=OperateLogFail;
						fns_Form_redraw(); 
						break;
					case 10:
						cotMenu_MainExit();	
						fns_Form_redraw();
						break;
					default :
						break;
				}
				break;
			default :
				break;
		}

}

void OperateLogInfo_onTick(BASEFORM* pThis)
{
	pThis->timeTick=2;
		if(g_Wake_Time_Count>LOGD_DELAY_TIME){
				FNS_ALL_FORM_CLOSE();
				g_Wake_Time_Count=0;
				fns_Form_redraw();
		}
		
		switch(OperateLogStatus)
		{
			case OperateLogSet:
				break;
	    case OperateLogQuery:
				break;
			case OperateLogResult:
				if(	g_Log_Pri_Display!=OperateLogResult){
							g_Log_Pri_Display=OperateLogResult;
							pThis->timeTick=20;//2秒
							g_Wake_Time_Count=0;
							g_Wait_Time_Count=50;
							fns_Form_redraw();
					}
					else{
							if(!g_Wait_Time_Count){
								cotMenu_MainExit();
							}
							fns_Form_redraw();
					}
				break;
			case OperateLogFail:
				if(	g_Log_Pri_Display!=OperateLogFail){
							g_Log_Pri_Display=OperateLogFail;
							pThis->timeTick=20;//2秒
							g_Wake_Time_Count=0;
							fns_Form_redraw();
					}
					else{
							if(!g_Wait_Time_Count){
								cotMenu_MainExit();
							}
							fns_Form_redraw();
					}
				break;
			default:
				break;
		}
			
}

void OperateLogInfo_onCreate(BASEFORM* pThis)
{
		unsigned char ret;

		ret=get_Operate_Log(&OperateLogMsg);
		if(ret){
				OperateLogStatus=OperateLogResult;
		}
		else{
				OperateLogStatus=OperateLogFail;
		}
		pThis->timeTick=2;
		g_Yes_Or_No=0;
		g_Log_Pri_Display=0;
		fns_Form_redraw();
		g_Wake_Time_Count=0;
}

/*******************************************************************/

uint8_t AlarmLogoInfoStr[]={"报警日志"};
AlarmLogStateDef  AlarmLogStatus;
AlarmLogStruts   AlarmLogMsg;
void AlarmLogoInfo_onShow(BASEFORM* pThis,CDC* pFormCDC);
void AlarmLogoInfo_onChar(BASEFORM* pThis,int keyVal);
void AlarmLogoInfo_onTick(BASEFORM* pThis);
void AlarmLogoInfo_onCreate(BASEFORM* pThis);

const FORM_HANDLERS s_formcode_AlarmLogoInfo={
		sizeof(MENUFORM),
		AlarmLogoInfoStr,
		fns_NullForm_onDefault,
		AlarmLogoInfo_onShow,
		AlarmLogoInfo_onChar,
		AlarmLogoInfo_onTick,
		AlarmLogoInfo_onCreate,
		fns_NullForm_onDestroy,
		fns_NullForm_onFocus	
};


void AlarmLogInfo(MENUFORM* pMenuForm)
{
		fns_Form_create(&s_formcode_AlarmLogoInfo);
}


void AlarmLogoInfo_onShow(BASEFORM* pThis,CDC* pFormCDC)
{	
	unsigned char x,y;
		clock_param_t  clock_temp;
		unsigned char str_buff[10];
		switch(AlarmLogStatus)
		{
			case AlarmLogDateQuery:
				y=1;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"正在查询",1,y,FONT_16X16);
				break;
			case AlarmLogDateResult:
				y=1;
				if(PASSWD_ALARM==AlarmLogMsg.alarm_type)
				{
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"密码锁定",1,y,FONT_16X16);
				}
				else if(FINGER_ALARM==AlarmLogMsg.alarm_type){
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"指纹锁定",1,y,FONT_16X16);
				}
				else if(RFID_ALARM==AlarmLogMsg.alarm_type){
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"卡片锁定",1,y,FONT_16X16);
				}
				else if(TEMPER_ALARM==AlarmLogMsg.alarm_type){
					OLED_HZ_ShowMixStr_XCentre(pFormCDC,"强拆报警",1,y,FONT_16X16);
				}
				else if(5==AlarmLogMsg.alarm_type){
	
				}
				y+=16;
				y+=8;
				x=12;
				OLED_ShowMixStr(pFormCDC,x,y,"日期:",1,1,FONT_16X16);
				x+=40;
				TimeSec2Clock(&AlarmLogMsg.clock_sec,&clock_temp);
				sprintf((char *)str_buff,"%02d/%02d/%02d",(clock_temp.year-2000),clock_temp.month,clock_temp.day);
				OLED_ShowMixStr(pFormCDC,x,y,str_buff,1,1,FONT_16X16);
				x=12;
				y+=16;
				OLED_ShowMixStr(pFormCDC,x,y,"时间:",1,1,FONT_16X16);
				x+=40;
				sprintf((char *)str_buff,"%02d:%02d:%02d",(clock_temp.hour),clock_temp.min,clock_temp.sec);
				OLED_ShowMixStr(pFormCDC,x,y,str_buff,1,1,FONT_16X16);
			
			
				break;
			case AlarmLogDateFail:
				y=1;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"系统提示",1,y,FONT_16X16);
				y=32;
				OLED_HZ_ShowMixStr_XCentre(pFormCDC,"无信息查询",1,y,FONT_16X16);
			default :
				break;
		}		
		
}

void AlarmLogoInfo_onChar(BASEFORM* pThis,int keyVal)
{
		unsigned char ret;
		g_Wake_Time_Count=0;
		switch(AlarmLogStatus)
		{
			case AlarmLogDateQuery:
				break;
      case AlarmLogDateResult:   
				PLAY_MUSIC(7);
				switch(keyVal){
					case 2:
						g_Wait_Time_Count=50;
						ret=get_Alarm_Log_Pri(&AlarmLogMsg);
						if(ret) AlarmLogStatus=AlarmLogDateResult;
							else AlarmLogStatus=AlarmLogDateFail;
						fns_Form_redraw(); 
						break;
					case 8:
						g_Wait_Time_Count=50;
						ret=get_Alarm_Log_Next(&AlarmLogMsg);
						if(ret) AlarmLogStatus=AlarmLogDateResult;
							else AlarmLogStatus=AlarmLogDateFail;
						fns_Form_redraw(); 
						break;
					case 10:
						cotMenu_MainExit();	
						fns_Form_redraw();
						break;
					default :
						break;
				}
				break;
			case AlarmLogDateFail:
				PLAY_MUSIC(7);
				switch(keyVal){
					case 2:
					
						ret=get_Alarm_Log_Pri(&AlarmLogMsg);
						if(ret) AlarmLogStatus=AlarmLogDateResult;
							else AlarmLogStatus=AlarmLogDateFail;
						fns_Form_redraw(); 
						break;
					case 8:
						ret=get_Alarm_Log_Next(&AlarmLogMsg);
						if(ret) AlarmLogStatus=AlarmLogDateResult;
							else AlarmLogStatus=AlarmLogDateFail;
						fns_Form_redraw(); 
						break;
					case 10:
						cotMenu_MainExit();	
						fns_Form_redraw();
						break;
					default :
						break;
				}
				break;
			default :
				break;
		}
}

void AlarmLogoInfo_onTick(BASEFORM* pThis)
{
	pThis->timeTick=2;
	if(g_Wake_Time_Count>LOGD_DELAY_TIME){
		FNS_ALL_FORM_CLOSE();
		g_Wake_Time_Count=0;
		fns_Form_redraw();
	}
	switch(AlarmLogStatus)
	{
		case AlarmLogDateSet:
			break;
		case AlarmLogDateQuery:  
			break;			
		case AlarmLogDateResult:
			if(	g_Log_Pri_Display!=AlarmLogDateResult){
							g_Log_Pri_Display=AlarmLogDateResult;
							pThis->timeTick=20;//2秒
							g_Wait_Time_Count=50;
							g_Wake_Time_Count=0;
							fns_Form_redraw();
					}
					else{
							if(!g_Wait_Time_Count){
								cotMenu_MainExit();
							}
							fns_Form_redraw();
					}
			break;
	  case AlarmLogDateFail:
			if(	g_Log_Pri_Display!=AlarmLogDateFail){
							g_Log_Pri_Display=AlarmLogDateFail;
							pThis->timeTick=20;//2秒
							g_Wake_Time_Count=0;
							fns_Form_redraw();
					}
					else{
							if(!g_Wait_Time_Count){
								cotMenu_MainExit();
							}
							fns_Form_redraw();
					}
			break;
		default:
			break;
	}
}

void AlarmLogoInfo_onCreate(BASEFORM* pThis)
{
		unsigned char ret;
	ret=get_Alarm_Log(&AlarmLogMsg);
	if(ret)
	{
		AlarmLogStatus=AlarmLogDateResult;
	}
	else
	{
		AlarmLogStatus=AlarmLogDateFail;
	}
		pThis->timeTick=2;
		g_Yes_Or_No=0;
		g_Log_Pri_Display=0;
		fns_Form_redraw();
	g_Wake_Time_Count=0;
}


/*******************************************************************/

uint8_t CleanLogInfoStr[]={"清空日志"};
DeleteLogoStateDef DeleteLogStatus;
void CleanLogInfo_onShow(BASEFORM* pThis,CDC* pFormCDC);
void CleanLogInfo_onChar(BASEFORM* pThis,int keyVal);
void CleanLogInfo_onTick(BASEFORM* pThis);
void CleanLogInfo_onCreate(BASEFORM* pThis);

const FORM_HANDLERS s_formcode_CleanLogInfo={
		sizeof(MENUFORM),
		CleanLogInfoStr,
		fns_NullForm_onDefault,
		CleanLogInfo_onShow,
		CleanLogInfo_onChar,
		CleanLogInfo_onTick,
		CleanLogInfo_onCreate,
		fns_NullForm_onDestroy,
		fns_NullForm_onFocus	
};


void CleanLogInfo(MENUFORM* pMenuForm)
{
		fns_Form_create(&s_formcode_CleanLogInfo);
}


void CleanLogInfo_onShow(BASEFORM* pThis,CDC* pFormCDC)
{
	unsigned char x,y;

		switch(DeleteLogStatus)
		{
			case DeleteLogo:            // 删除界面
				break;
			case DeleteLogConfirm:
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
			case DeleteLogSuccess:   //删除成功
				System_DelSuccess_Tip(pFormCDC);
					break;
			case DeleteLogFail:            // 
				break;
			case DeleteLogInvalid:        // 
				break;
		}
}


void CleanLogInfo_onChar(BASEFORM* pThis,int keyVal)
{
		switch(DeleteLogStatus)
		{
			case DeleteLogo:            // 删除界面
				break;
			case DeleteLogConfirm:
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
								g_Yes_Or_No=0;
								clean_log();
								DeleteLogStatus=DeleteLogSuccess;
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
			case DeleteLogSuccess:   //删除成功
				break;
			case DeleteLogFail:            // 
				break;
			case DeleteLogInvalid:        // 
				break;
			default:
				break;
		}

}

	
void CleanLogInfo_onTick(BASEFORM* pThis)
{
		pThis->timeTick=2;
		if(g_Wake_Time_Count>LOGD_DELAY_TIME){
				FNS_ALL_FORM_CLOSE();
				g_Wake_Time_Count=0;
				fns_Form_redraw();
		}
		fns_Form_redraw();
		switch(DeleteLogStatus)
		{
				case DeleteLogo:         // 删除界面
					break;
				case DeleteLogConfirm:
					break;
				case DeleteLogSuccess:  //删除成功
					if(	g_Log_Pri_Display!=DeleteLogSuccess){
							g_Log_Pri_Display=DeleteLogSuccess;
						g_Wake_Time_Count=0;
							pThis->timeTick=20;//2秒
							fns_Form_redraw();
					}
					else{
							cotMenu_MainExit();
							fns_Form_redraw();
					}
					break;
				case DeleteLogFail:            // 
					if(	g_Log_Pri_Display!=DeleteLogSuccess){
							g_Log_Pri_Display=DeleteLogSuccess;
							pThis->timeTick=20;//2秒
						g_Wake_Time_Count=0;
							fns_Form_redraw();
					}
					else{
							cotMenu_MainExit();
							fns_Form_redraw();
					}
					break;
				case DeleteLogInvalid:        // 
					if(	g_Log_Pri_Display!=DeleteLogSuccess){
							g_Log_Pri_Display=DeleteLogSuccess;
							pThis->timeTick=20;//2秒
						g_Wake_Time_Count=0;
							fns_Form_redraw();
					}
					else{
							cotMenu_MainExit();
							fns_Form_redraw();
					}
					break;
				default :
				break;
		}
}



void CleanLogInfo_onCreate(BASEFORM* pThis)
{
    pThis->timeTick=2;
		g_Yes_Or_No=0;
		g_Log_Pri_Display=0;
		DeleteLogStatus= DeleteLogConfirm;
		fns_Form_redraw();
		g_Wake_Time_Count=0;		
}






#endif


