#include <QString>
#ifndef FRAME_H
#define FRAME_H


#define INT8U unsigned char
#define INT32U unsigned int
#define INT16U unsigned short
#define MAX_MSG_LEN 128

typedef struct _Msg_
{
    INT8U       length;
    INT8U		crc;
    INT8U		data[MAX_MSG_LEN];
}Msg,*pMsg;
#pragma pack()


class Frame
{
//    Q_OBJECT

public:
    Frame();
    int CheckFrame(QString str);
    QString LinkFram(QString str);
    QString CheckCode(QString frame);
    QString split(QString Frame);
    bool PackFrame(Msg src, INT8U * dst, INT8U *len); //组包
    INT8U UnpackFrame(INT8U ch, Msg *pmsg);           //解包
    QString test1;


private:
    enum FRAME_STATE
    {
        F_ERROR = -1,
        F_HEADER_H,
        F_HEADER_L,
        F_LENGTH,
        F_DATA,
        F_CRC,
        F_END_H,
        F_END_L,
        F_OVER,
    };

   // Encrypt *_encrypt;    //加密对象

    int converter = 0;
    int data_point = 0;
    FRAME_STATE frame_state;

    INT8U protocol_convert(INT8U ch);  //转义
    INT8U protocol_deconvert(INT8U ch);  //反转义
    INT8U CRC8( INT8U*buffer, INT8U len);

};

#endif // FRAME_H
