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

/**********************************
 * 定义帧的结构，
 * 一帧的结构包括 帧的种类，帧头（用于鉴别是否为重复帧），数据起始stx，数据内容，数据结束，校验码
**********************************/
typedef struct
{
    frame_kind kind;
    bool frame_header;
    int STX = 0x03;
    frame info;
    int EXT = 0x04;
    int LRC;
} frame;


/************************************
网络层传输的基本单位是包，定义包为纯字节的数据
*************************************/
typedef struct
{
    int data[];
} packet;

/************************************
//帧的种类，
（1）ENQ：发送方申请发送的帧，
（2）EOT：数据已经发送结束的帧，
（3）ACK：对方已经接收到帧，并且帧的内容正确
（4）NAK：对方已经接收到帧，但是帧的内容错误，比如，校验码错误，或者重复帧,
（5）DATA：数据帧
************************************/
typedef enum
{
    ACK;
    NAK;
    DATA;
    ENQ;
    EOT
}
frame_kind;

/************************************
 * 中断函数的时间种类，有帧的到来，定时器超时，以及错误次数过多
 ************************************/
typedef enum
{
    frame_arrival,
    time_out,
    error_serious
} event_type;


void disable_networking();               //超过三次错误或者超时，暂定网络服务，并且发出警告
void wait_receipt(event_type *event);    //接受对方的回应。有帧的到来，超时，以及错误次数过多
void start_timer(void);                  //启动网络定时服务
void close_timer(void);                  //关闭网路定时器
void can_send(frame *p);                 //接受帧
void can_rec(packet *p);                 //发送帧
//成帧函数
int frame_forming(packet *sel_direc, packet *angle, packet sel_speed, int *frame_num, int specie)
//数据处理部分的函数
int data_dispossing(frame *p, int *sel); //数据的处理部分，在disposs.c中有详细的解释
int LRC_calculatiing(int *sel, int i);   //求和校验码
 data_loading(frame *s, packet *sel,int num)；//数据的装填
 void frame_forming(frame *s, packet *sel, int *frame_num)
