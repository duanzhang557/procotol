#ifndef __process_
#define __process_

#define BLOCK 2 //每一帧的数目，如果修改，直接在.h文件中修改

#define ACK 0x03;
#define NAK 0X04;
#define STX 0x05; //数组开始；
#define ETX 0X06; //结束字符
#define DEL 0x08; //转义字符，用于透传
#define ESC 0X50; //填充作用

#define ENQ 0x51; //发送方向接收方申请发送
#define EOT 0X52; //全部数据的发送结束
/*
网络层传输的基本单位是包，定义包为纯字节的数据
*/
typedef struct
{
    int data[];
} packet;0
typedef enum
{
    ACK;
    NAK;
    DATA;
    ENQ;
    EOT
}
frame_kind;

typedef enum
{
    frame_arrival,
    time_out,
    error_serious
} event_type;

typedef struct
{
    frame_kind kind;
    bool frame_header;
    int STX = 0x03;
    frame info;
    int EXT = 0x04;
    int LRC;
} frame;


void disable_networking(); //超过三次错误或者超时，暂定网络服务，并且发出警告
void wait_receipt(event_type *p); //接受对方的回应。有 ACK,NAK，以及超时
void can_rec(frame *p);    //接受帧
void can_send(frame*p);   //发送回执
int judging(int *p);
void data_loading(frame *p, packet *r, int n);
packet *data_parsing(packet *r);
