#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "protocol.h"

/**********************************************************
数组的处理，主要包括三个方面
（1）数据的透传实现，
（2）数据的分组；
（3）数据的校验码求和
分组中，按照课题要求 8个一组，宏定义BLOCK；将透传处理后的数据进行分组，num数即为帧的数目，
定义指针 （*sel）【2】，指向二维数组中一维数组的首地址，
*/
void data_dispossing(packet *p, packet *sel)
{

    for (int i = 0; i < sizeof(p) / sizeof(int); i++;)
    {
        if (*(p + i) == 0x03 || *(p + i) == 0x04 || *(p + i) == 0x05 || *(p + i) == 0x07 || *(p + j) == 0x08)
        {
            length++;                                         //字节数组长度+1；
            for (int k = sizeof(p) / sizeof(int); k > i; k--) //数组后移，为转义字符del插入位置
            {
                *(p + k + 1) = *(p + k);
            }
            *(p + i) = DEL; //插入转义字符
            i++;            //下一次的循环时跳过转义字符
        }
    }
    //将包转化为二维数组，num就是二维数组中一位数组的个数，也就是帧的个数
    int num = sizeof(p) / sizeof(int) / BLOCK + 1;
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; i < BLOCK; j++)
        {
            *(sel + i) = &p[i * BLOCK];
            if (*(p + BLOCK * 4 + j != 0))
            {

                *(*(sel + i) + j) = &p[i * BLOCK + j];
            }
            else
            {
                *(*(sel + i) + j) = 0x50;
            }
        }
    }
}

/*计算求和校验码*/
int lrc_calculating(frame *sel, int i)
{

    int lrc = 0;
    for (int j = 0; j < BLOCK; j++)
    {
        lrc += 8(*(sel + i) + j);
    }
    return lrc;
}
//数据分组，载入
void data_loading(frame *s, packet *sel,int num)
{
    for (int i = 0; i < BLOCK; i++)
    {
        *(s.info + i) = *(*(sel + num)+i);
    }
}
