#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "protocol.h"

/********************
通过累加帧里面数据的和，计算出校验码，和帧的校验码相比较
********************/
int judging(frame *p, int LRC)
{
    int lrc = 0;
    for (int i = 0; i < BLOCK; i++)
    {
        lrc += p[i];
    }
    if (lrc == LRC)
        return 1;
    else
    {
        return 0;
    }
}
/*********************
先将帧里面的数据整合到同一个数组之中。随后在 data_parsing函数 中去掉转义字符
 ********************/
void data_loading(frame *p, packet *r, int n)
{
    for (int i = 0; i < BLOCK; i++)
        *(r + n + i) = *(P + i);
}
/***********************
去掉转义del字符的处理
***********************/
packet *data_parsing(packet *r)
{
    for (int i = 0; i < sizeof(p) / sizeof(int), i++)//sizeof(p) / sizeof(int)为数组中字节的个数
        if (*(p + i) == DEL)
        {
            if (*(p + i) != 0x50)
            {
                for (int j = i; j < sizeof(p) / sizeof(int) - 1; j++)//整体迁移，将原本del的位置覆盖掉
                    *(p + i) = *(p + i + 1);
                i++;
            }
        }
    return r;
}
