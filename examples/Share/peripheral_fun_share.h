#ifndef PERIPHERAL_FUN_SHARE_H
#define PERIPHERAL_FUN_SHARE_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "peripheral_fun_share.h"
#include "type_def.h"

typedef struct clock_param
{
    uint16_t year;
    uint16_t month;
    uint16_t day;
    uint16_t week;
    uint16_t hour;
    uint16_t min;
    uint16_t sec;
} clock_param_t;


#define UART_BUFF_SIZE  200

#define MAX_PASSWD_NUM  100
#define MAX_FINGER_NUM  100
#define MAX_RFID_NUM		100

typedef enum{
	SYSTEM_NONE=0,
	SYSTEM_WAKE,
	SYSTEM_SLEEP
}SystemStatus;



				
typedef struct _SystmeParaStructure
{
	unsigned short remote_flag;
	unsigned short remote_enable;
	unsigned short wifi_flag;
	unsigned short wifi_enable;
	unsigned short voice_level;
	unsigned short double_check_enable;
	unsigned short always_open_flag;
	unsigned short system_logo_enable;
	unsigned short tamper_enable;
	unsigned short restart_flag;	
	unsigned short restore_factory;     //出厂设置状态
	unsigned char  soft_version[16];
	unsigned char  product_id[16];
	unsigned char  soft_date[16];
	unsigned char  soft_time[16];
	unsigned short safetylevel;
	unsigned short ic_check_enable;
	unsigned short ic_encryption_enable;
  unsigned short sys_logo_switch;
	unsigned char  finger_chip_sn[32];
	unsigned char  finger_type_status;   //0 表示需要发休眠指令，1表示不需要发休眠指令，2以上表示无指纹或指纹异常
}SystmeParaStructure;



//extern unsigned int pmu_io_int_pins;
//extern unsigned int Passwd_Time_Count; 
extern unsigned int g_Wake_Time_Count;
//extern unsigned char g_Lock_Sleep_Wake_status;
//extern unsigned char g_Play_BaoJing_Flag;
//extern unsigned char g_Play_BaoJing_Times;
extern unsigned char g_Force_Disable_Flag;
//extern unsigned char g_Restore_Flag;
//extern unsigned int g_Restore_Times;
//extern unsigned int g_Finger_Disable_Int_Flag;
//
//extern os_timer_t motor_timer_refresh;
//extern os_timer_t finger_timer_refresh;
//extern os_timer_t led_timer_refresh;
//
//
//extern unsigned char g_Hal_Trig_Flag;
//extern unsigned char g_Hal_Trig_Delay;
//
//extern unsigned char g_Uart0_Recv_Msg_Step;
//extern unsigned char g_Finger_Uart_Flag;
//extern unsigned char g_Finger_Uart_Recv_Num;
//
//extern unsigned char g_fmTxbuf[UART_BUFF_SIZE];
//extern unsigned char g_fmRxbuf[UART_BUFF_SIZE];
//extern unsigned char Uart0_Rec_Data_Temp[UART_BUFF_SIZE];
extern SystmeParaStructure g_System_Para_Info;
//extern unsigned char g_Touch_Led_Flag;
extern unsigned char g_Voltage_Level;
extern unsigned char Input_Type;

extern clock_param_t clock_env ;

//extern unsigned char g_Touch_Event;
//extern unsigned char g_Touch_Value;
//
extern OpenLogStruts  OpenLog_Info;
//extern OperateLogStruts OperateLog_Info;
//extern AlarmLogStruts AlarmLog_Info;
extern unsigned int g_Finger_Int_Flag;
//extern Open_Lock_Info_Def g_Admin_Login_Info;


void motor_time_start(unsigned int data_temp);
void finger_time_start(unsigned int data_temp);
void motor_timer_refresh_fun(void *arg);
void finger_timer_refresh_fun(void *arg);
void led_timer_refresh_fun(void *arg);

unsigned int get_finger_time(void);
void UART0_Receive(unsigned char recv_data);
void peripheral_get_into_sleep(void);
void system_getinto_sleep(void);
void timer_refresh_fun(void *arg);
void lcd_timer_refresh_fun(void *arg);
void system_exit_sleep(void);
void Play_BaoJing_music(void);
void Init_System_Para(void);
void System_Para_Save(void);
void System2Factory(void);
void TimeClock_Init(void );
void Clock_Save(void );
void Clock_Save_Update(void);
void Led_Flashes(unsigned char led_times);
void Touch_Led_Enable(void);
void Touch_Led_Disable(void);
void wake_sleep_test_fun(void *arg);
void Xprintf(const char *fmt, ...);
void System_OpenLogInfo_Save(clock_param_t *clock_env_temp,unsigned short id_num,unsigned short user_type);
time_t Clock2Time(clock_param_t *clock_temp);

#endif
