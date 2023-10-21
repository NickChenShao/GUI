#ifndef USER_MANAGERSET_H
#define USER_MANAGERSET_H

#include <stdint.h>
#include "Form.h"
#include "menuForm.h"


#define  USER_MANAGER_DELAY_TIME   20

typedef enum
{
	InputPassWord=1,         // 输入密码
	ReInputPassWord ,       // 重新输入密码
	PassWordFull ,          // 密码已满
	RegistPasswdSuccess,          // 密码注册成功
	PasswdErrLess,        	//密码位数不够
	PasswdErrDiff,          //密码不止
	PasswdErrDupicate,      //密码输入重复
	PasswdNoAdmin,

} RegistKeyStateDef;

typedef enum
{
	putICCard=1 ,             // 放置IC卡
  CardExists,             // IC卡已登记
	ICCardFull ,            // IC卡已满
	ICCardFail,
	RegistICSuccess,          // IC卡注册成功
	RegistICNoAdmin,
} RegistICCardStateDef;


typedef enum
{
	DelFingerSuccess,             // 删除成功
	DelFingerFail,                // 删除失败
	InputFingerError ,            // 输入出错
	DelFailAdmin,           			// 管理员指纹最后一枚指纹不能删除
	RegistFinger ,          			// 输入指纹
	ReRegistFinger ,        			// 重新输入指纹

	Raise_Finger,                 // 抬起手指
	RegistFingerFailAdmin,        // 请先登记管理员指纹
	FingerFull ,            			// 指纹已满
	FingerExists,           			// 该指纹已登记
	RegistFingerFail,             // 指纹登记失败
	RegistFingerSuccess,           // 指纹登记成功
	RegistFingerNoAdmin,
} RegFingerStateDef;


typedef enum
{
	DeleteMenuByGroup=1 ,             // 删除界面
	DeleteMenuByGroupNormal,
	DeleteMenuByGroupAdmin,
	DeleteMenuByGroupGuest,
	DeleteMenuByGoupForce,
	PendingDeletionByGroup,  			 //正在删除
	DeleteMenuByGoupSuccess,   //删除成功
	DeleteFailByGroup ,            // 
	DeleteInvalidByGroup,        // 

} DeleteByGroupStateDef;


typedef enum
{
	DeleteMenuBySn=1 ,             // 删除界面
	DeleteMenuBySnConfirm,
	DeleteMenuBySnSuccess,   //删除成功
	DeleteMenuBySnFail ,            // 
	DeleteMenuBySnNoUser,
	DeleteMenuBySnInvalid,        // 

} DeleteBySnStateDef;


typedef enum
{
	DeleteMenuByVerify=1 ,             // 删除界面
//	DeleteMenuByVerifyInputErr,
	DeleteMenuByVerifyConfirm,
	DeleteMenuByVerifySuccess,   //删除成功
	DeleteMenuByVerifyFail ,            // 
	DeleteMenuByVerifyNoUser,
	DeleteMenuByVerifyInvalid,        // 

} DeleteByVerifyStateDef;


//typedef enum   //分组删除枚举
//{
//	DeleteICCardByType=1,      
//	DeletePasswdByType,  //
//	DeleteFingerByType,   //长度错误
//}Delete_By_Type_TypeDef;

//typedef enum   //编号删除枚举
//{
//	DeleteICCardBySN=1,      
//	DeletePasswdBySN,  //
//	DeleteFingerBySN,   //
//}Delete_By_SN_TypeDef;


//typedef enum   //验证删除枚举枚举
//{
//	DeleteICCardByVerify=1,      
//	DeletePasswdByVerify,  //
//	DeleteFingerByVerify,   //
//}Delete_By_Verify_TypeDef;


typedef enum   //验证删除枚举枚举
{
	DeleteFingerUser=1,      
	DeletePasswdUser,  //
	DeleteICCardUser,   //
}DeleteModeType_TypeDef;




#endif
