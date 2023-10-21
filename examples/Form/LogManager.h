#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H


#include "date_time.h"



#define LOGD_DELAY_TIME  20



typedef enum
{
	OpenLogDateSet=1,
	OpenLogDateQuery,   
	OpenLogDateResult,     
	OpenLogDateFail,
} OpenLogByDateStateDef;


typedef enum
{
	OpenLogNumSet=1,
	OpenLogNumQuery,   
	OpenLogNumResult,     
	OpenLogNumFail,
} OpenLogByNumStateDef;


typedef enum
{
	AlarmLogDateSet=1,
	AlarmLogDateQuery,   
	AlarmLogDateResult,     
	AlarmLogDateFail,
} AlarmLogStateDef;


typedef enum
{
	OperateLogSet=1,
	OperateLogQuery,   
	OperateLogResult,     
	OperateLogFail,
} OperateLogStateDef;



typedef enum
{
	DeleteLogo=1,						// 删除界面
	DeleteLogConfirm,
	DeleteLogSuccess,				//删除成功
	DeleteLogFail ,					// 
	DeleteLogInvalid,				// 

} DeleteLogoStateDef;









#endif


