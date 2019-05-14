#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "protocol.h"

void main(int argc, char *argv[])
{
    frame s;        //定义帧，接受来自发送方的数据
    packet direc[]; //方向数据包，用于存放发送发帧中的数据
    packet angle[]; //
    packet speed[];
    s.frame_header = frame_rec = 0; //定义帧头，并且初始化为0；
    bool correction;                //发送的帧是否正确，正确为1
    int n;
    int *num = &n;  //用于记录已经发送的帧的数目
    int specie = 0; //为0 时，表示发送的是方向的帧，为1是角度，为2是速度
    event_type event;
    wait_receipt(*p); //等待对方发送enq发送请求
    while (true)
    {
        if (event == frame_arrival)
        {
            can_rec(&s);
            switch (s.kind)
            {
            case DATA:
                correction = judging(&s, s.LRC);                     //判断校验码
                if (correction == 0 || s.frame_header != frame_send) //帧头不相同，或者校验码不正确
                {
                    s.frame_kind = NAK;
                    can_send(&s);
                }
                else
                {
                    specie = data_selecting(packet * direc, packet * angle, packet * speed, frame * s, int *num, int specie);
                    s.frame_kind = ACK;
                    can_send(&s);
                }
                break;
            case ENQ:
                s.kind = ACK;
                can_send(&s);
                break;
            case EOT:
                s.kind = ACK;
                can_send(&s);
                disable_networking();
                break;
            default;
            break;
            }
        }
    }
}

int data_selecting(packet *direc, packet *angle, packet *speed, frame *s, int *num, int specie)
{
    static specie = 0;
    if (specie == 0)
    {
        if (s.info[3] != 0x50)
        {
            data_loading(&s, direc, *num); //将帧中的数据整合，放在一起
            *num += BLOCK;
            data_parsing(direc); //将direc中的数据进行去转义字符处理，因为函数是进行地址运算，因此可以不用进行内存的请求和释放
        }
        else
        {
            *num = 0;
            specie++;
        }
    }
    else if (specie == 1)
    {
        if (s.info[3] != 0x50)
        {
            data_loading(&s, angle, *num); //将帧中的数据整合，放在一起
            *num += BLOCK;
            data_parsing(angle); //将angle中的数据进行去转义字符处理，因为函数是进行地址运算，因此可以不用进行内存的请求和释放
        }
        else
        {
            *num = 0;
            specie++;
        }
    }
    else if (specie == 3)
    {
        if (s.info[3] != 0x50)
        {

            data_loading(&s, speed, *num); //将帧中的数据整合，放在一起
            *num += BLOCK;
            data_parsing(speed); //将speed中的数据进行去转义字符处理，因为函数是进行地址运算，因此可以不用进行内存的请求和释放
        }
        else
        {
            *num = 0;
            specie++;
        }
    }
    return specie;
}
