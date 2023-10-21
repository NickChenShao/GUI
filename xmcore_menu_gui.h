#ifndef __FORMCDC_001
#define __FORMCDC_001

#include "CDC.h"
#include <stdint.h>
#include "xmcore_menu_gui.h"
#include "Form.h"
#include <string.h>

typedef struct _CDC{
    uint8_t color;
    uint8_t *lcdBuff;
    /////////////////FormCDC////////////////////
    //...
    /////////////////FormCDC////////////////////
}CDC;


#define SCREEN_CLIENT_LEFT 		0
#define SCREEN_CLIENT_TOP 		0
#define SCREEN_CLIENT_RIGHT		LCD_X_MAX
#define SCREEN_CLIENT_BOTTOM 	LCD_Y_MAX
#define SCREEN_MAXLINE          3
#define SCREEN_MAXCOLUMN	    32

#define SCREEN_PAGE0 0x00
#define SCREEN_PAGE1 0x100
extern unsigned char Normal_Icon[518];


typedef enum //跟第一个定义不同的是，此处的标号DAY省略，这是允许的。
{
	FONT_16X16=0,
	FONT_24X22,
} Font_Type;//变量workday的类型为枚举型enum DAY


void fns_FormCDC_reset(CDC* pCDC);
void OLED_Clear(void);
void GUI_Draw_Point(CDC* pCDC,uint8_t x,uint8_t y,uint8_t t);
void GUI_Draw_Point2(uint8_t x,uint8_t y,uint8_t t);
void OLED_ShowChar(CDC* pCDC,uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode,unsigned char font_type);
void OLED_Custom_Icon(void);
void OLED_ShowHZChar(CDC* pCDC,uint8_t x, uint8_t y, uint8_t* chr,uint8_t mode,unsigned char font_type);
void OLED_ShowMixStr(CDC* pCDC,uint8_t x, uint8_t y, uint8_t* ucDataStr,uint8_t mode,uint8_t Refresh_en,unsigned char font_type);
void OLED_ShowNum(CDC* pCDC,uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode,unsigned char font_type);
void OLED_HZ_ShowMixStr_Centre(CDC* pCDC,uint8_t * ucDataStr,uint8_t mode,unsigned char font_type);
void OLED_HZ_ShowMixStr_XCentre(CDC* pCDC,uint8_t * ucDataStr,uint8_t mode,unsigned char y_line,unsigned char font_type);
void OLED_YesOrNO(CDC* pCDC,uint8_t x, uint8_t y,uint8_t yes_no );
void LCD_Error_Icon(CDC* pCDC,uint8_t x, uint8_t y) ; //分辨率32*32
void LCD_Alarm_Icon(CDC* pCDC,uint8_t x, uint8_t y);
void LCD_Finger_Input_Icon(CDC* pCDC,uint8_t x, uint8_t y);//48*48  Finger_Input_Icon
void LCD_Open_Icon(CDC* pCDC,uint8_t x, uint8_t y);
void LCD_Lock_Icon(CDC* pCDC,uint8_t x, uint8_t y);
void LCD_Battery_Level(CDC* pCDC,uint8_t power_level,uint8_t x, uint8_t y);
void LCD_Wifi_Icon(CDC* pCDC,uint8_t x, uint8_t y);
void LCD_Admin_Icon(CDC* pCDC,uint8_t x, uint8_t y);
void LCD_SystemLogo_Icon(void);
void LCD_Ring_Icon(CDC* pCDC,unsigned char ring_times,uint8_t x, uint8_t y);  //32X32
void Err_Alarm_Icon(CDC* pCDC,uint8_t x, uint8_t y);
void progress_bar(CDC* pFormCDC,unsigned char value_temp,unsigned char x_label,unsigned char y_label);
void Lower_Power_Icon(CDC* pCDC,uint8_t x, uint8_t y);
void Dir2_Logo_Icon(CDC* pCDC,uint8_t x, uint8_t y);
void Dir8_Logo_Icon(CDC* pCDC,uint8_t x, uint8_t y);
void Finger_Collect_Icon(CDC* pCDC,uint8_t icon_num,uint8_t x, uint8_t y);
void Dir4_Logo_Icon(CDC* pCDC,uint8_t x, uint8_t y);
void Dir6_Logo_Icon(CDC* pCDC,uint8_t x, uint8_t y);
void Finger_Collect_Icon(CDC* pCDC,uint8_t icon_num,uint8_t x, uint8_t y);
void System_Logo_Icon(void);
void System_SetSuccess_Tip(CDC* pFormCDC);
void System_SetFail_Tip(CDC* pFormCDC);
void System_UpdateConfirm_Tip(CDC* pFormCDC,unsigned char  Yes_Or_No );
void System_DelSuccess_Tip(CDC* pFormCDC);
void System_DelFail_Tip(CDC* pFormCDC);
void System_UDOperation_Tip(CDC* pFormCDC, uint8_t* pTitleStr, uint8_t y);



#endif

