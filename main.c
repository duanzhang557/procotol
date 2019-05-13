#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "protocol.h"

void main(int argc, char *argv[])
{
    frame s;                       //定义帧
    packet r;                      //定义从网络层接受的包
    event_type event;              //定义时间类型，
    s.frame_header frame_send = 0; //定义帧头，并且初始化为0；
    int frame_num = 0;             //记录已经发送的帧的数目，
    int *num_address = &frame_num;
    int specie = 0;     //用于记录发送的种类，为0 方向，为1角度，为2速度
    int error_num = 0； //累计超时，或者错误的次数，如果超过三次，发出警告

        //*can_rec(&r)用于接受来自物理层或者其他的数据，
        //其实已经给定，三个数组direc angle speed
        packet direc[] = {0x10, 0x12, 0x03, 0x04, 0x13, 0x05, 0x08, 0x07, 0x21, 0x45, 0x13, 0x07, 0x21, 0x70, 0x12, 0x04, 0x02, 0x97, 0x22, 0x04};
    packet angle[] = {0x01, 0x03, 0x05, 0x07, 0x09, 0x20, 0x21, 0x24, 0x05, 0x07, 0x18, 0x23, 0x35, 0x34, 0x21, 0x32, 0x05, 0x07, 0x07, 0x05};
    packet speed[] = {0x20, 0x22, 0x03, 0x07, 0x23, 0x10, 0x04, 0x05, 0x07, 0x23, 0x21, 0x13, 0x01, 0x23, 0x05, 0x01, 0x07, 0x22, 0x05, 0x07};

    //根据课题要求，每一帧包含block个字节（课题中给的是8个字节，也就是2）；
    //因此，需要对经过透传后的数据进行分组，每8个一组，
    //比如对direction经过透传后的数组，进行分组，2个数据一组，一共有sizeof（p）/sizeof（int）+1组，不足的以0x50补齐；
    //因此定义一系列的二维指针
    packet(*sel_direc)[2];
    packet(*sel_angle)[2];
    packet(*sel_speed)[2];

    //数据的处理，包括透传，分组
    //sel_direc为双重指针，指向经过分组后的二维数组的每个子数组的地址；
    data_dispossing(direc, sel_direc);
    data_dispossing(angle, sel_angle);
    data_dispossing(speed, sel_speed);

    //发送方首先发送发送请求，等待对方的肯定回复。即ACK，第一次握手
    s.kind = ENQ;
    can_send(&s);
    start_timer();
    wait_receipt(&s);
    if (event == frame_arrival)
    { //确认收到接受方的回复 ，第二次握手
        if (s.kind = ACK)
        {
            //第三次握手
            s.frame_kind = ACK;
            can_send(&s);
        }
    }
    else if (event == time_out || event == error_serious)
    {
        printf("通信连接建立失败！！") ； return 0;
    }

    s.frame_header = frame_send; //给帧头赋值
    while (true)                 //死循环传输
    {
        start_timer();    //启动定时器，
        wait_receipt(*p); //对接收方反馈的信息 p 进行鉴别
        if (event_type == frame_arrival)
        {
            can_rec(&s);
            switch (s.kind)
            {
            case ACK:
                //data_into_frame是选择帧的种类和序号的函数，
                int data_into_frame(packet * sel_direc, packet * angle, packet sel_speed, int *frame_num, int specie);
                frame_num++;
                break;
            case NAK:
                can_send(&s);
                start_timer();
                error_num++;
                break;
            default;
                break;
            }
        }
        else if (event == time_out)
        {
            can_send(&s);
            error_num++;
        }
        else if (event_type == error_serious)
        {
            disable_networking();
        }
    }
}
/*********************************
 * 成帧函数
 *specie是用于记录帧的种类，每个frame_num是用于记录已经发送的帧的数目
 每次调用函数只会执行函数的三个if中的一个，specie鉴别传输的种类，frame_num鉴别需要发送的帧的序号
*********************************/
int frame_forming(packet *sel_direc, packet *angle, packet sel_speed, int *frame_num, int specie)
{
    int num = 0; //记录每种数据形成的帧的个数
    if (specie == 0)
    {
        num = sizeof(sel_direc) / sizeof(int) / BLOCK + 1;
        if (*frame_num < num)
        {   //info_into_frame为成帧函数，
            specie = info_into_frame(&s, direc, num_address, sel_angle);
            return specie; //发送完就推出if循环，不进行下面的if环节
        }
        //表明direc的帧已经发送完成，此时speice自增，下次发送下一种类的帧
        else
        {
            specie++;
            *frame_num = 0;
        }
    }
    else if (specie == 1)
    {
        num = sizeof(sel_angle) / sizeof(int) / BLOCK + 1;
        if (*frame_num < num)
        {
            specie = info_into_frame(&s, packet * angle, num_address, packet * sel_angle);
            return specie;
        }
        else
        {
            specie++;
            *frame_num = 0;
        }
    }
    else if (specie == 2)
    {
        num = sizeof(sel_speed) / sizeof(int) / BLOCK + 1;
        if (*frame_num < num)
        {
        }
        else
        {
            specie++;
            *frame_num = 0;
        }
    }
    else
    {
        s.frame_kind = EOT;
        can_send(&s);
        break;
    }
}
//成帧函数
int info_into_frame(&s, packet *p, int *frame_num, packet *sel)
{
    data_loading(*(sel + *frame_num), direc); //帧中info数据的赋值
    s.lrc = lrc_calculatiing(sel_angle, n);   //lrc_calculatiing是计算校验码的函数，采用求和校验码
    s.frame_header = DATA;
    frame_send = ^frame_send; //帧头的翻转
    can_send(&);
    start_timer();
    return specie;
}
