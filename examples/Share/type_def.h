#ifndef TYPE_DEF_H
#define TYPE_DEF_H




#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

 
#define C_IDFull  0


#define USER_OTP_SIZEOF       20
#define PAGE_MAX_TOTP_USERNUM   100
#define MAX_PASS_USERNUM    100

typedef enum{
	PASSWD_ALARM=1,
	FINGER_ALARM,
	RFID_ALARM,
	TEMPER_ALARM,
}AlarmTypeDef;



typedef enum{
	ADD_USER=1,
	DELETE_USER,
	DELETE_ADMIN,
	DELETE_NORMAL,
	DELETE_GUEST,

}OperateTypeDef;



typedef struct _PassWord
{
	unsigned short PassWd_ID_Num;	//NumId+200
	unsigned short PassWd_Save_Num;	//NumId
	unsigned short PassWd_Num;		//密码长度
	unsigned char PassWd_Buff[10];	//密码
	unsigned short User_Type;		//用户类型
}PassWord_TypeDef;

typedef enum
{
	PassWd_OK,      //密码正确
	PassWd_Err_Invalid,  //无效密码
	PassWd_Err_Length,   //长度错误
	PassWd_Err_Dupicate, //密码重复
}PassWd_Request_TypeDef;

typedef enum
{
	Admin_User=1,	//管理员
	Normal_User,  //普通用户
	Guest_User,   //宾客
	Temporary_User,	//临时用户
}User_TypeDef;

typedef enum
{
	NONE_MODE,
	FingerPrintMode=1,	//指纹输入
	PasswdMode,  //密码输入
	ICCardMode,   //IC输入
	RingTrig,	//#门铃提示
}User_Mode_TypeDef;

typedef struct _ICCard
{
	unsigned short ICCard_ID_Num;	//NumId+300
	unsigned short ICCard_Save_Num;	//NumId
	unsigned char ICCard_buf[4];	//IC信息
	unsigned short User_Type;		//用户类型
}ICCard_TypeDef;

typedef struct _AllTypeUserNum
{
	unsigned short normal_user;	//普通用户
	unsigned short admin_user;	//管理员
	unsigned short guest_user;	//宾客
	unsigned short force_user;	//强制用户
}AllTypeUserNum_Def;



typedef struct _Finger_Type
{
	unsigned short Finger_ID_Num;		//NumId+100
	unsigned short Finger_Index_Table;	//
	unsigned short User_Type;			//用户类型
}FingerPrint_TypeDef;

typedef struct _Open_Lock_Info_Def
{
	unsigned short logo_sn;				//
	unsigned short User_ID_Num;			//
	unsigned short User_Type;			//用户
	unsigned short User_Mode;			//输入类型
}Open_Lock_Info_Def;


typedef struct _OpenLogStruts
{
	unsigned short logo_sn;			//flash记录的位置
	unsigned short user_num;		//登入的NumId
	unsigned short  user_type;		//用户类型
	time_t   clock_sec;				//时间戳
}OpenLogStruts;

typedef struct _OperateLogStruts
{
	unsigned short logo_sn;
	unsigned short admin_num;
	time_t   clock_sec;
	unsigned short operatetype;    //添加用户，删除用户
	unsigned short user_num;
	unsigned short user_type;
}OperateLogStruts;


typedef struct _AlarmLogStruts
{
	unsigned short logo_sn;
	unsigned short alarm_type;
	time_t   clock_sec;
}AlarmLogStruts;


typedef struct _LogInfoStruts
{
		unsigned char log_en_flag;      //flash 中是否含有有效数据
		unsigned char log_next_no_data;  //显示之前是否搜索到数据 ,0表示搜索到，1表示没有搜索到
		unsigned char log_pri_no_data;  //显示之前是否搜索到数据 ,0表示搜索到，1表示没有搜索到

		unsigned char log_on_view_sn;   //显示的日志编号
		unsigned char log_on_view_pos;  //显示的日志位置
		unsigned char log_end_flag;     //为1的时候，表示向下搜索结束
		unsigned char log_start_flag;   //为1的时候，表示向上搜索结束
		time_t start_time_sec;          //起始时间
		time_t end_time_sec;            //结束时间
		unsigned char log_min_sn;
		unsigned char log_max_sn;
		unsigned char log_min_pos;
		unsigned char log_max_pos;
}LogInfoStruts;



typedef enum 
{
	Menu_Exit_Type_TimeOut=1,//超时退出
	Menu_Exit_Type_Cancle,//取消按键退出
	Menu_Exit_Type_ProEnd,//运行结束
}Menu_Exit_Type_ENUM;


typedef struct
{
  bool   OTP_Timing_Flag;       // TOTP计时标志
  uint32_t OTP_StarTiming;      // TOTP起始计时  
  uint32_t OTPPassWord[17];     // TOTP密码
}OTPInfo;


typedef struct
{
  uint16_t OpenDoorCount;       // 开门次数
  uint16_t RollingCode;         // 滚动码
  uint8_t  SaveStatus;          // 保存状态
  uint8_t  OTP_Par[5];          // OTP密钥              
}CountInfoOTP;

//typedef struct
//{
//  uint8_t UserType;       
//	uint8_t Key_Manage;
//  uint16_t UserPassID;         
//  uint8_t  PassLength;  
//  uint8_t  PassWord[5];           
//}UserPassInfo;

//typedef enum{
//	Key_Manage=1,
//	Key_Snap,

//}XuserTypeDef;

typedef struct //_LOCAL_TIME 
{    
  uint8_t  Second;      // 秒
  uint8_t  Minute;      // 分
  uint8_t  Hour;      // 时
  uint8_t  DayOfWeek;    // 星期  
  uint8_t  Date;      // 日
  uint8_t  Month;      // 月  
  uint16_t Year;      // 年
}LOCAL_TIME;


			 
#define  TOTP_INTERVAL_TIME        300               // OTP密码间隔时间,最小单位“秒”

//#define		BaoJing  						1   //报警声
//#define		ZhengZaiSuoMen 			2   //正在锁门
//#define		CaoZuoChengGong 		3   //操作成功
//#define		ChangKaiMoShi				4   //常开模式
//#define		ChuShiHua						5   //初始化音
//#define		YongHuBuCunZai			6   //此用户不存在
//#define		DI									7   //滴水按键音
//#define		DianLiangBuZu				8   //电量不足请更换电池
//#define 	GanYingKa						9   //感应卡
//#define 	GuanLiYuan					10  //管理员
//#define 	KeyWake							11  //键盘唤醒音
//#define 	KaiMen							12  //开门音
//#define 	MenLing							13  //门铃音
//#define 	MenYiFanSuo					14   //门已反锁
//#define 	MiMa								15  //密码
//#define 	MiMaBuYiZhi 				16  //密码不一致
//#define 	MiMaWeiShuBuYiZhi  	17  //密码位数不一致
//#define 	WeiShouQuan  				18  //你未授权，请于管理员
//#define 	AnYaoKongQi 				19  //请按遥控器
//#define 	FangGanYingKa  			20  //放感应卡
//#define 	HuanYongHuYanZheng  21  //请换用户验证
//#define 	ShuRu6Dao10Wei  		22  //请输入6到10位
//#define 	ShuRuIDHao  				23  //请输入ID号
//#define 	ShuRuShanChuIDHao  	24  //请输入要删除的ID号
//#define 	FangShouZhi  				25  //请放手指
//#define 	XianTianJia  				26  //请先添加
//#define 	YanZhengGuanLiYuan 	27  //请验证管理员
//#define 	YanZhengShanChuYongHu 28 //请验证要删除的用户
//#define 	ZaiFangShouZhi  			29  //请再放手指
//#define 	ChongXinShuRu  				30  //请重新输入
//#define 	ChongXinYanZheng 			31  //请重新验证
//#define 	QuXiaoAnXingHaoJian 	32  //取消请按*号键
//#define 	QueRenAnJingHaoJian		33  //确认请按井号键
//#define		QueRenXinMiMa  				34  //确认新密码
//#define 	ShanChuChengGong  		35  //删除成功
//#define 	ShanChuShiBai  				36  //删除失败
//#define 	ShiBaiYin  						37  //失败音
//#define 	SuoYiSheZhiChangKai		38  //锁已设置常开
//#define 	SuoYiKaiQiAnBaShou		39  //锁已开启，请按把手
//#define 	TianJiaChengGong			40  //添加成功
//#define		TianJiaShiBai					41  //添加失败
//#define 	TongYiGeYongHu				42  //同一个用户
//#define		TongYiZhongLeiXing		43  //同一种类型
//#define		WeiLeNinDeAnQuan			44  //为了您的安全
//#define		XiTongTuiChu					45  //系统退出
//#define		XinXiJingLing					46  //信息精灵
//#define		YanZhengChengGong			47  //验证成功
//#define		YanZhengShiBai				48  //验证失败
//#define		YaoKongQi							49  //遥控器
//#define		YiCunZai							50  //已存在
//#define		YongHu								51  //用户
//#define		YongHuMiMa						52  //用户密码
//#define		YongHuYiMan						53  //用户已满
//#define		HuiFuChuChangSheZhi		54  //恢复出厂设置，请稍后
//#define   ZhiWen								55  //指纹
//#define		ZhiWenShuRuShiBai			56  //指纹输入失败
//#define		ZhiWenCaiJiYin				57  //指纹采集音
//#define   RenLianTianJiaShiBai	58  //人脸添加失败
//#define		AnJingHaoJianQueRen		59  //按井号键确认
//#define		QingShaoHou						60  //锁已关闭
//#define		SuoYiGuanBi						61  //锁已开启
//#define		SuoYiKaiQi						62  //正在开门
//#define		ZhengZaiKaiMen				63  //滴水按键音B
//#define		DiShuiYin							64  //按键唤醒音B
//#define		JianPanHuanXing				65  //门铃音1
//#define		MenLing1							66
//#define 	Opening_the_door			67
//#define		Locking_the_door			68
//#define  	Lock_is_off						69
//#define		Lock_is_on						70
//#define		Success_operation			71
//#define		User_not_exist				72
//#define		Low_battery						73
//#define		User_exists						74
//#define		Verification_passed		75
//#define		Inconsistent_passwd		76
//#define		Change_another_user		77
//#define		Enter_6_10_admin_passwd			78
//#define		Enter_6_10_passwd			79
//#define		Enter_ID_num					80
//#define  	Enter_ID_num_delete		81
//#define		Enter_user_finger			82
//#define		Verify_admin					83
//#define		Verify_user_delete		84
//#define		Enter_same_finger			85
//#define		Confirm_new_passwd		86
//#define		System_exit						87
//#define		User_is_full					88
//#define		Put_ic_card						89
//#define		JiShiGuanMen					90   //请及时关门
//#define		ShouDongSuoMen				91   //请手动锁门
//#define		XiTongTuiChu2					92   //系统退出
//#define		HuanYingHuiJia				93   //欢迎回家
//#define		TianJiaRenLian				94   //请添加人脸
//#define		ZaiCiTianJiaRenLian		95   //请再次添加人脸
//#define		XiTongTuiChu3					96   //系统退出
//#define		YaoKongQi48to49				97   //遥控器已存在
//#define		YaoKongQi48to39				98   //遥控器添加成功
//#define		ShuRu21_51_58					99   //请输入6到10位用户密码，按井号键确认
//#define		ShuRu21_09_14_58			100  //请输入6到10位管理员密码按井号键确认
//#define		MiMaWeiShu16_29				101  //密码位数输入错误，请重新输入
//#define		MiMaBuYiZhi15_29			102  //密码不一致请重新输入
//#define   MiMaTianJiaChengGong	103  //密码添加成功
//#define		MiMaTianJiaShiBai			104  //密码添加失败
//#define		ZhiWenTianJiaChengGong			105   //指纹添加成功
//#define		GanYingKaYiCunZai			106         //感应卡已存在
//#define		GuanLiYuanTianJiaChengGong	107   //管理员添加成功
//#define		ShuRuID_22_58					108         //请输入ID号，按井号键确认
//#define		WeiLeAnQuan43_25_09		109         //为了您的安全，请先添加管理员
//#define  	TongYiGeYongHu41_30		110         //同一个用户，请重新验证
//#define		KaiMenYin11						111         //开门音
//#define		KaiMenYin11_69				112         //开门音，lock iS on
//#define		WeiLeAnQuan43_89			113         //为了您的安全，请及时关门
//#define		KaiMenYin11_25				114         //开门音，锁已开启
//#define		YiDaKai								121         //已打开
//#define		YiGuanBi							122         //已关闭
//#define		SheZhiChengGong				123         //设置成功
//#define   TaiShouZhi						124        

#endif

