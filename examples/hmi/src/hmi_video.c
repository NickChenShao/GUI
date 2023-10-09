#include "hmi_video.h"
#include "mainhmi.h"
#include <stdio.h>
#include <string.h>


const MenuImage_t sgc_VideoImage = {
"vvvvvvvvvv",
"#"
};




void Hmi_VideoLoad(uint8_t menuState)
{
    
}

void Hmi_VideoExit(void)
{

}

void Hmi_VideoTask(void* pExtendInputData, uint8_t menuState, MenuShow_s* pstShowInfo)
{
    int cmd;

    printf("--------------------------\n");
    printf("     视频功能测试界面\n");
    printf("--------------------------\n");
	sleep(1);
//    printf("选择操作(0-退出): ");
//    scanf(" %d", &cmd); // 空格作用是忽略上次的回车

//    if (cmd == 0)
//    {
//        cotMenu_Exit(false);
//    }
}
