#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H



#include <stdint.h>

#define SYSTEM_DELAY_TIME   20

typedef enum
{
	SysTimeSet=1 ,      
	SysTimeSetConfirm,
	SysTimeSetSuccess,   
	SysTimeSetFail ,           
} SysTimeSetStateDef;


typedef enum
{
	VoiceLevelSet=1 ,      
	VoiceLevelSetConfirm,
	VoiceLevelSetSuccess,   
	VoiceLevelSetFail ,           
} VoiceStateDef;

typedef enum
{
	OpenModeSet=1,      
	OpenModeSetConfirm,
	OpenModeSetSuccess,   
	OpenModeSetFail,           
} OpenModeSetDef;

typedef enum
{
	AlwaysOpenSet=1,      
	AlwaysOpenSetConfirm,
	AlwaysOpenSetSuccess,   
	AlwaysOpenSetFail,           
} AlwaysOpenDef;

typedef enum
{
	TamperSet=1,      
	TamperSetConfirm,
	TamperSetSuccess,   
	TamperSetFail,           
} TamperStateDef;

typedef enum
{
	RestoreSetConfirm=2,
	RestoreSetBar,
	RestoreSetSuccess,   
	RestoreSetFail,           
} RestoreStateDef;
typedef enum
{
	SafetyLevelSet=1,      
	SafetyLevelSetConfirm,
	SafetyLevelSetSuccess,   
	SafetyLevelSetFail,           
} SafetyLevelSetDef;


typedef enum
{
	ICCheckSet=1,      
	ICCheckSetConfirm,
	ICCheckSetSuccess,   
	ICCheckSetFail,           
} ICCheckSetDef;


typedef enum
{
	ICEncryptionSet=1,      
	ICEncryptionSetConfirm,
	ICEncryptionSetSuccess,   
	ICEncryptionSetFail,           
} ICEncryptionSetDef;

typedef enum
{
	SysLogoSet=1,      
	SysLogoSetConfirm,
	SysLogoSetSuccess,   
	SysLogoSetFail,           
} SysLogoSetDef;

typedef enum
{
	SelfCheckModeFinger=1,  
	SelfCheckModeFingerOK,	
	SelfCheckModeIC,
	SelfCheckModeICOK,
	selfCheckModeTouch,
	selfCheckModeTouchOK,
	SelfCheckModeInit,
	SelfCheckModeResult,	
} SelfCheckModeDef;

void frmMaintenanceMode_Menu(void);
void Burn_In_Test(void);
void Self_Check_Mode(void);

#endif

