#include "hmi_video.h"
#include "cot_menu.h"
#include <stdio.h>
#include <string.h>


const MenuImage_t sgc_VideoImage = {
"vvvvvvvvvv",
"#"
};




void Hmi_VideoLoad(void)
{
    
}

void Hmi_VideoExit(void)
{

}

void Hmi_VideoTask(void)
{
    int cmd;

    printf("--------------------------\n");
    printf("     ��Ƶ���ܲ��Խ���\n");
    printf("--------------------------\n");

    printf("ѡ�����(0-�˳�): ");
    scanf(" %d", &cmd); // �ո������Ǻ����ϴεĻس�

    if (cmd == 0)
    {
        cotMenu_Exit(false);
    }
}
