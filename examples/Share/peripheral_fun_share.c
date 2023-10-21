#include "peripheral_fun_share.h"

#include "ble_simple_peripheral.h"
#include "GUI_Form_Fun.h"
#include "config.h"
#include "fcl_flash.h"
#include "driver_flash.h"
#include "StartForm.h"
#include "driver_efuse.h"


SystmeParaStructure g_System_Para_Info;

//static unsigned int Finger_Time_Count=0;
//static unsigned int Motor_Time_Count=0;
unsigned int g_Wake_Time_Count=0;		//强制睡眠时间记录标志
//
//static unsigned char Finger_Time_Flag=0;
//static unsigned char Motor_Time_Flag=0;
//
//
//unsigned int Passwd_Time_Count=0; 
//unsigned int pmu_io_int_pins=0;
//
//os_timer_t motor_timer_refresh;// 用于刷新传感器数据以及显示等
//os_timer_t finger_timer_refresh;
//os_timer_t led_timer_refresh;
//
//unsigned char g_Uart0_Recv_Msg_Step=0;
//unsigned char g_Finger_Uart_Flag=0;
//unsigned char	g_Finger_Uart_Recv_Num=0;
//
//unsigned char g_fmTxbuf[UART_BUFF_SIZE];
//unsigned char g_fmRxbuf[UART_BUFF_SIZE];
//unsigned char Uart0_Rec_Data_Temp[UART_BUFF_SIZE];
//unsigned char g_Lock_Sleep_Wake_status=0;  //0是休眠，1是唤醒
//unsigned char g_Play_BaoJing_Flag=0;
//unsigned char g_Play_BaoJing_Times=0;
unsigned char g_Force_Disable_Flag=0;			//防拆开关，掉电丢失
//unsigned char g_Restore_Flag=0;
//unsigned int g_Restore_Times=0;
//unsigned int g_Finger_Disable_Int_Flag=0;
unsigned int g_Finger_Int_Flag=0;


//time_t g_Pri_Clock_Sec;   //记录前一次的时间秒数
//
//unsigned char g_Led_Enable_Flag=0;
//unsigned int g_Led_Flashes_Delay=0;
//unsigned char g_Led_Flashes_Times=0;
//
//unsigned char g_Hal_Trig_Flag=0;
//unsigned char g_Hal_Trig_Delay=0;
//
//
unsigned char g_Voltage_Level=0;	//电压检测
//unsigned char g_Touch_Led_Flag;
unsigned char Input_Type;
//
//unsigned char g_Touch_Event=0;
//unsigned char g_Touch_Value=0;
OpenLogStruts  OpenLog_Info;


clock_param_t clock_env =
{
    .year = 2021,
    .month = 6,
    .day = 3,
    .week = 5,
    .hour = 0,
    .min = 0,
    .sec = 0,
};

//OperateLogStruts OperateLog_Info;
//
//AlarmLogStruts AlarmLog_Info;
//
//Open_Lock_Info_Def g_Admin_Login_Info;

void motor_time_start(unsigned int data_temp)
{
		Motor_Time_Count=data_temp;
	
		if(!Motor_Time_Flag){
				Motor_Time_Flag=1;
				os_timer_start(&motor_timer_refresh,100,1);
		}
}

void finger_time_start(unsigned int data_temp)
{
		Finger_Time_Count=data_temp;
	
		if(!Finger_Time_Flag)
		{
				Finger_Time_Flag=1;
				os_timer_start(&finger_timer_refresh,100,1);
		}
}


void motor_timer_refresh_fun(void *arg)
{
		if( Motor_Time_Count)
		{
			Motor_Time_Count--;
		}
		else
		{		
			stop_motor();
			Motor_Time_Flag=0;
			os_timer_stop(&motor_timer_refresh);
		}
}

void finger_timer_refresh_fun(void *arg)
{
		if( Finger_Time_Count){
				Finger_Time_Count--;
		}
		else{		
			os_timer_stop(&finger_timer_refresh);
		}
}

void led_timer_refresh_fun(void *arg)
{
		if(g_Led_Enable_Flag){
			g_Led_Flashes_Delay++;
			if(0==g_Led_Flashes_Delay%2){
				if(0==g_Led_Flashes_Delay%4){
					led_on();
						if(g_Led_Flashes_Times) g_Led_Flashes_Times--;
						if(!g_Led_Flashes_Times){
							g_Led_Enable_Flag=0;
							os_timer_stop(&led_timer_refresh);
						}
				}
				else{
						led_off();	
				}
		}
	}
}

unsigned int get_finger_time(void)
{
		return Finger_Time_Count;
}

void UART0_Receive(unsigned char recv_data)
{
		static unsigned int Uart_Data_Temp;
		static unsigned char Uart0_Rec_Num_Temp;
		Uart_Data_Temp=recv_data;

		if(0==g_Uart0_Recv_Msg_Step)    //确定帧头
		{
				if(0xef==Uart_Data_Temp){
						Uart0_Rec_Data_Temp[0]=Uart_Data_Temp;
				}
				else if(0x01==Uart_Data_Temp){
						g_Uart0_Recv_Msg_Step=1;
						g_Finger_Uart_Recv_Num=2;
						Uart0_Rec_Data_Temp[1]=Uart_Data_Temp;
				}
				else{
						g_Uart0_Recv_Msg_Step=0;
						memset(Uart0_Rec_Data_Temp,0,UART_BUFF_SIZE);
				}
		}
		else if(1==g_Uart0_Recv_Msg_Step)  //确定长度
		{
				if(8==g_Finger_Uart_Recv_Num){
						Uart0_Rec_Data_Temp[g_Finger_Uart_Recv_Num++]=Uart_Data_Temp;
						Uart0_Rec_Num_Temp=Uart_Data_Temp+9;
						if(Uart0_Rec_Num_Temp<60){
							g_Uart0_Recv_Msg_Step=2;
						}
						else{
							g_Uart0_Recv_Msg_Step=0;
							g_Finger_Uart_Flag=0;
							g_Finger_Uart_Recv_Num=0;
							memset(Uart0_Rec_Data_Temp,0,UART_BUFF_SIZE);
						}
				}
				else{
						Uart0_Rec_Data_Temp[g_Finger_Uart_Recv_Num++]=Uart_Data_Temp;
				}
		}
		else if(2==g_Uart0_Recv_Msg_Step){
				Uart0_Rec_Data_Temp[g_Finger_Uart_Recv_Num]=Uart_Data_Temp;
				g_Finger_Uart_Recv_Num++;
				if(Uart0_Rec_Num_Temp==g_Finger_Uart_Recv_Num){
					g_Uart0_Recv_Msg_Step=0;
					g_Finger_Uart_Flag=1;
					memcpy(g_fmRxbuf,Uart0_Rec_Data_Temp,UART_BUFF_SIZE);
					memset(Uart0_Rec_Data_Temp,0,UART_BUFF_SIZE);	
						
				}
		}
		else{
				g_Uart0_Recv_Msg_Step=0;
				g_Finger_Uart_Flag=0;
				g_Finger_Uart_Recv_Num=0;
				memset(Uart0_Rec_Data_Temp,0,UART_BUFF_SIZE);
		}
}	

void peripheral_get_into_sleep(void)
{
		Power_Down(SC92F_CHIP_SLEEP);		//触摸进入休眠
		COMMAND_PWR_DOWN();							//语音进入休眠
		cmd_Sleep();										//指纹进入休眠
}


void system_getinto_sleep(void)
{
		os_timer_stop(&timer_refresh);
		os_timer_stop(&lcd_timer_refresh);
		os_timer_stop(&motor_timer_refresh);
		os_timer_stop(&finger_timer_refresh);
//		os_timer_start(&wake_sleep_test,500,1);
		fcl_adc_disable();
		Touch_Led_Disable();
		Input_Type=NONE_MODE;
		peripheral_get_into_sleep();
//		system_sleep_enable();//开启休眠
		g_Finger_Disable_Int_Flag=1;
}
	
	void timer_refresh_fun(void *arg)
{

//	Xprintf("GUI_Form_Start\r\n");
//	current_time=GetTouchKey();
//	if(Passwd_Time_Count) Passwd_Time_Count--;
//	else
//	{
//			Passwd_Time_Count=20;
//			Power_Down(SC92F_WAKE_CHIP);
//	}
//	adc_get_result(ADC_TRANS_SOURCE_PAD, 0x01, &result);
//		g_Wake_Time_Count++;
	wdt_feed();
//	//如果在主界面
//	if(g_Wake_Time_Count>40)  //大于10s
//	{
//		g_Wake_Time_Count=0;
//		system_getinto_sleep();
//	}
//  如果不在主界面
//		if(g_Play_BaoJing_Flag)
//		{
//				if(g_Play_BaoJing_Times){
//						COMMAND_READ_STATUS();
//						if(!(u8STATUS_SFR&0x80)) {
//							g_Wake_Time_Count=0;
//								g_Play_BaoJing_Times--;
//								play_music(BaoJing);
//						}
//				}
//				else  g_Play_BaoJing_Flag=0;
//		}


}


void lcd_timer_refresh_fun(void *arg)
{
		if(g_Touch_Event){
//			Xprintf("Touch Key:%d 0x%x\r\n",g_Touch_Event,g_Touch_Value);
			g_Touch_Event=0;
			test_fns_Form_doChar(g_Touch_Value);
		}
//		extern void XmMcu_Test_countTimeByPin(void);
//		XmMcu_Test_countTimeByPin();
		GUI_Form_Refresh( );
		
//		if(g_Hal_Trig_Flag){
//				if(g_Hal_Trig_Delay){
//						g_Hal_Trig_Delay--;
//						if(!g_Hal_Trig_Delay){
//								g_Hal_Trig_Flag=0;	
//								FNS_ALL_FORM_CLOSE();		
//								menuState=StartStates;
//								g_Wake_Time_Count=0;				
//								system_getinto_sleep();
//						}
//				}
//		}
}



	
void system_exit_sleep(void)
{
		led_gpio_init();

		Exit_SC92F8XX_LowPower();
		Power_Down(SC92F_WAKE_CHIP);		
		COMMAND_WAKUP();

		LCD_Init();
		RC522_Init();
		fcl_motor_init();
		FP_INIT();
		fcl_adc_init();
		g_Voltage_Level=Get_Voltage_Level();

	menuState=Dect_Mode;
		os_timer_start(&timer_refresh,500,1);
		os_timer_start(&lcd_timer_refresh,100,1);
		g_Finger_Disable_Int_Flag=0;
}




void Play_BaoJing_music(void)
{
		g_Play_BaoJing_Flag=1;
		g_Play_BaoJing_Times=5;
}


void Init_System_Para(void)
{
		uint32_t addr;
		uint8_t ret=0;
//		uint8_t data_str[20];
	  struct chip_unique_id_t id_data;
		addr=SYSTEM_PARA_ADDR;
		flash_read(addr,sizeof(SystmeParaStructure),(unsigned char *)&g_System_Para_Info);
	
		if(g_System_Para_Info.always_open_flag==0xffff){
			g_System_Para_Info.always_open_flag=0;
			ret++;
		}
		 if(g_System_Para_Info.remote_enable==0xffff){
			g_System_Para_Info.remote_enable=0;
			ret++;
		}
		 if(g_System_Para_Info.remote_flag==0xffff){
			g_System_Para_Info.remote_flag=0;    //这个地方要通过检查判定
			ret++;
		}
		 if(g_System_Para_Info.restart_flag==0xffff){
			g_System_Para_Info.restart_flag=0;       
			ret++;
		}
		 if(g_System_Para_Info.system_logo_enable==0xffff){
			g_System_Para_Info.system_logo_enable=0;
			ret++;
		}
		 if(g_System_Para_Info.tamper_enable==0xffff){
			g_System_Para_Info.tamper_enable=1;
			 g_Force_Disable_Flag=0;
			ret++;
		}
		 else{
				if(g_System_Para_Info.tamper_enable)
				g_Force_Disable_Flag=0;
				else g_Force_Disable_Flag=1;
		 }
		
		 if(g_System_Para_Info.voice_level==0xffff){
			g_System_Para_Info.voice_level=3;
			ret++;
		}
		 if(g_System_Para_Info.wifi_enable==0xffff){
			g_System_Para_Info.wifi_enable=0;
			ret++;
		}
		 if(g_System_Para_Info.wifi_flag==0xffff){
			g_System_Para_Info.wifi_flag=0;    //这个地方要通过检查判定
			ret++;
		}
		 
		 if(g_System_Para_Info.restore_factory==0xffff){
				g_System_Para_Info.restore_factory=0;
				ret++;
		 }

		 if(memcmp(g_System_Para_Info.soft_version,"XSUN-FSX1-SG002",sizeof("XSUN-FSX1-SG002"))) {
				efuse_get_chip_unique_id(&id_data);
//				sprintf(data_str,"%s", __DATE__);
				memcpy(g_System_Para_Info.soft_version,"XSUN-FSX1-SG002",sizeof("XSUN-FSX1-SG002") );
				memcpy(g_System_Para_Info.product_id,&id_data,6);
				memcpy(g_System_Para_Info.soft_date,__DATE__,strlen(__DATE__));
				memcpy(g_System_Para_Info.soft_time,__TIME__,strlen(__TIME__));	
				ret++;
		 }
		 
			if(ret){
				System_Para_Save();
			}
			//恢复时间
			TimeClock_Init();
			System_Logo_Icon_Read();
}

void System_Para_Save(void)
{
		uint32_t addr;
		addr=SYSTEM_PARA_ADDR;
	Fcl_Flash_Write(addr, sizeof(SystmeParaStructure), (unsigned char *)&g_System_Para_Info);
	flash_read(addr,sizeof(SystmeParaStructure),(unsigned char *)&g_System_Para_Info);
}

void System2Factory(void)
{
	unsigned char finger_chip_sn[32];
	
		g_System_Para_Info.always_open_flag=0;
		g_System_Para_Info.remote_enable=0;
		g_System_Para_Info.remote_flag=0;    //这个地方要通过检查判定
		g_System_Para_Info.restart_flag=0;       
//		g_System_Para_Info.system_logo_enable=0;
		g_System_Para_Info.tamper_enable=1;   //强拆默认开启
		g_System_Para_Info.voice_level=3;
		g_System_Para_Info.wifi_enable=0;
		g_System_Para_Info.wifi_flag=0;    //这个地方要通过检查判定
		g_System_Para_Info.restore_factory=0;
		g_System_Para_Info.double_check_enable=0;
	
	
		memset(g_System_Para_Info.soft_time,0,16);
		memset(g_System_Para_Info.soft_date,0,16);
		memset(g_System_Para_Info.product_id,0,16);
		memset(g_System_Para_Info.soft_version,0,16);
	
		memcpy(g_System_Para_Info.soft_version,"XSUN-FSX1-SG002",sizeof("XSUN-FSX1-SG002") );
		memcpy(g_System_Para_Info.soft_date,__DATE__,strlen(__DATE__));
		memcpy(g_System_Para_Info.soft_time,__TIME__,strlen(__TIME__));		
				
		memset(finger_chip_sn,0,32);
		cmd_GetChipSN( finger_chip_sn, 32);
		if(memcmp(finger_chip_sn,g_System_Para_Info.finger_chip_sn,32))
		{						
			memcpy(g_System_Para_Info.finger_chip_sn,finger_chip_sn,32);
		}
		System_Para_Save();
}


void TimeClock_Init(void )
{
		uint32_t addr;
		addr=TIME_SAVE_ADDR;
		time_t clock_sec1,clock_sec2;
		clock_param_t clock_env_temp;
		flash_read(addr,sizeof(time_t),(unsigned char *)&clock_sec1);
		TimeSec2Clock( &clock_sec1, &clock_env_temp);
	
		clock_sec2=Clock2Time(&clock_env);
		if(clock_sec1==0xffffffff) 
		{
				g_Pri_Clock_Sec=clock_sec2;
				Fcl_Flash_Write(addr, sizeof(time_t), (unsigned char *)&g_Pri_Clock_Sec);
		}
		else if(clock_env_temp.year>2099)
		{
				g_Pri_Clock_Sec=clock_sec2;
				Fcl_Flash_Write(addr, sizeof(time_t), (unsigned char *)&g_Pri_Clock_Sec);
		}
		else{
				g_Pri_Clock_Sec=clock_sec1;
				memcpy(&clock_env,&clock_env_temp,sizeof(clock_param_t));
		}
}

void Clock_Save(void )
{
		time_t clock_sec;
		uint32_t addr;
		addr=TIME_SAVE_ADDR;
	
		clock_sec=Clock2Time(&clock_env);
		Fcl_Flash_Write(addr, sizeof(time_t), (unsigned char *)&clock_sec);
}


void Clock_Save_Update(void)
{
		uint32_t addr;
		time_t diff_sec;
		addr=TIME_SAVE_ADDR;
		time_t clock_sec2;
	
		clock_sec2=Clock2Time(&clock_env);
	
		if(clock_sec2>g_Pri_Clock_Sec){
				diff_sec=clock_sec2-g_Pri_Clock_Sec;
				if(diff_sec>1200){
						g_Pri_Clock_Sec=clock_sec2;
						Fcl_Flash_Write(addr, sizeof(time_t), (unsigned char *)&g_Pri_Clock_Sec);
				}
		}
}

void Led_Flashes(unsigned char led_times)
{
		g_Led_Enable_Flag=1;
		g_Led_Flashes_Delay=0;
		g_Led_Flashes_Times=led_times;
		os_timer_start(&led_timer_refresh,80,1);
}

void Touch_Led_Enable(void)
{
		g_Touch_Led_Flag=1;
		led_on();
}

void Touch_Led_Disable(void)
{
		g_Touch_Led_Flag=0;
		led_off();
}
void wake_sleep_test_fun(void *arg)
{
 Xprintf("wake test\r\n");
	if(g_Lock_Sleep_Wake_status)
	{
		 Xprintf("wake test2\r\n");
//		system_sleep_enable();//开启休眠
//		system_getinto_sleep();
	}

}

void Xprintf(const char *fmt, ...)
{
#ifdef  FCL_DEBUG
	unsigned int i=0xffff;
	unsigned int gpio_int_value;
  static char sz[30] = { 0, };
	
	pmu_set_pin_to_CPU(GPIO_PORT_A, 1<<GPIO_BIT_2);
	pmu_set_pin_to_CPU(GPIO_PORT_A, 1<<GPIO_BIT_3);
	
	gpio_set_dir(GPIO_PORT_A, GPIO_BIT_2, GPIO_DIR_IN);
	gpio_set_dir(GPIO_PORT_A, GPIO_BIT_3, GPIO_DIR_OUT);
	system_set_port_pull(GPIO_PA2,true);
	system_set_port_pull(GPIO_PA3,true);

	system_set_port_mux(GPIO_PORT_A, GPIO_BIT_2, PORTA2_FUNC_UART1_RXD);
	system_set_port_mux(GPIO_PORT_A, GPIO_BIT_3, PORTA3_FUNC_UART1_TXD);
	uart_init(UART1, BAUD_RATE_115200);  

  va_list ap;
  va_start(ap, fmt);
  vsprintf(sz, fmt, ap); 
  co_printf(sz);
  va_end(ap);
	
//		while(i) i--;
//		pmu_set_pin_to_PMU(GPIO_PORT_A,1<<GPIO_BIT_3);
//		pmu_set_port_mux(GPIO_PORT_A,GPIO_BIT_3,PMU_PORT_MUX_GPIO); 	//设为IO
//		pmu_set_pin_dir(GPIO_PORT_A,1<<GPIO_BIT_3,GPIO_DIR_IN);
//		pmu_set_pin_pull(GPIO_PORT_A, (1<<GPIO_BIT_3), true);   //防拆中断PA5


//		pmu_io_int_pins=GPIO_PB4|GPIO_PB7|GPIO_PA5|GPIO_PC6|GPIO_PA3;
//		gpio_int_value=ool_read32(PMU_REG_GPIOA_V);
//		gpio_int_value&=pmu_io_int_pins;
//    ool_write32(PMU_REG_PORTA_LAST,gpio_int_value);
//		pmu_port_wakeup_func_set(pmu_io_int_pins);
		

		#endif
}

	void System_OpenLogInfo_Save(clock_param_t *clock_env_temp,unsigned short id_num,unsigned short user_type)
{
	OpenLogStruts  OpenLog_Info_Temp;
	OpenLog_Info_Temp.clock_sec=Clock2Time(clock_env_temp);
	OpenLog_Info_Temp.user_num=id_num;
	OpenLog_Info_Temp.user_type=user_type;
	XmMcu_Log_saveOpenLog(&OpenLog_Info_Temp);

}	

time_t Clock2Time(clock_param_t *clock_temp)
{
		struct tm  time_info;
		time_t  clock_sec;

		time_info.tm_year=clock_temp->year-1900;  //从1970开始计算
		time_info.tm_mon=clock_temp->month-1;     //0 to 11
		time_info.tm_mday=clock_temp->day;        //1 to 31
		time_info.tm_hour=clock_temp->hour;
		time_info.tm_min=clock_temp->min;
		time_info.tm_sec=clock_temp->sec;
		clock_sec=mktime(&time_info);
		return clock_sec;
}

