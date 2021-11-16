#include "frame.h"
#include<QDebug>
#include<QString>

#define CHECK_CODE "aa55"   //设置帧头用于定位和组帧

QString FIRST_FRAME = "aa551216310100XXXX";    //用于握手的第一帧

QString ERR_00 = "aa551216310100XX";      //无效ID
QString ERR_01 = "aa551216310101XX";      //数据长度n超范围；
QString ERR_02 = "aa551216310102XX";      //ID+n超范围；

QString linked_frame="";    //保存连接后的帧

//确保输出的是两个连接正确的帧
bool first=false;
bool second=false;
Frame::Frame(){

}
//用于选择的函数 因为本页不能使用connec传递参数，所以只让函数直接返回分类值。
/*1 代表握手成功
 *0 代表需要进一步处理的帧
 *-1代表无效ID
 *-2代表输出长度n超范围
 *-3代表ID+n超范围
 */
int Frame::CheckFrame(QString str){
    if(str.mid(0,4)==CHECK_CODE){
        if(str==FIRST_FRAME){
            qDebug()<<"握手成功";
            return 1;
            //emit connect_sucess(REPLY_FIRST);
        }else if (str==ERR_00) {
            qDebug()<<"无效ID";
            return -1;
        }else if (str==ERR_01) {
            qDebug()<<"数据长度n超范围";
            return -2;
        }else if (str==ERR_02) {
            return -3;
            qDebug()<<"ID+n超范围";
        }else{
            return 0;

        }
    }else{
        return 0;
    }
}

//用于连接的函数
QString Frame::LinkFram(QString str){

        if(str.mid(0,4)==CHECK_CODE&&first==false&&linked_frame.length()<124){
            first=true;
            linked_frame="";
            linked_frame=str;
        }else{
            if(first==true&&linked_frame.length()<124){         //帧前半部分已确定
                linked_frame=linked_frame+str;
                first=false;
                //qDebug()<<"连接后的:"<<linked_frame;           //输出连接后的帧
                if(linked_frame.length()<130){
                    return linked_frame;
                }else{
                    return 0;
                }
                return linked_frame;                            //舍弃
            }else{
                first=false;
                linked_frame="";
                return 0;                   //舍弃
            }                               //帧前半部分没确定

        }
        return 0;                           //舍弃
}
//用于校验的函数
QString Frame::CheckCode(QString frame){
    int len=frame.length();
    QString sum_code=frame.mid(len-2,2);    //截取最后两位
   // qDebug()<<"校验输入";
    bool ok;                        //用在下面QString 16进制转10进制
    QString sum="";;
    int test=0;
    for(int i=0;i<round((len-2));){
        sum=frame.mid(i,2);
        test+=sum.toInt(&ok,16);    //16进制转10进制然后进行求和运算
        i=i+2;
    }
    //上面通过求和校验
    if(QString::number(test, 16).right(2)==sum_code){
    //  qDebug()<<"校验成功输出";
        return frame;
    }else{
        return 0;
    }
}
/*
//拆分帧
QString Frame::split(QString Frame){
    QString test1=Frame.mid(4,5);
    return test1;
}

//校验
INT8U Frame::CRC8( INT8U* buffer, INT8U len)
 {
     //return _encrypt->CRC8(buffer, len);
 }
//转义
INT8U Frame::protocol_convert(INT8U ch)
{
    if ((converter == 1) && (ch == 0xA5))
    {
        converter = 0;
        ch = 0x5A;
    }
    else if ((converter == 1) && (ch == 0x66))
    {
        converter = 0;
        ch = 0x99;
    }
    else if ((converter == 1) && (ch == 0x95))
    {
        converter = 0;
        ch = 0x6A;
    }
    else if (converter == 1)
    {
        frame_state = F_ERROR;
    }
    return ch;
}
//反转义
INT8U Frame::protocol_deconvert(INT8U ch)
 {
     INT8U rtn = 0;
     switch(ch)
     {
         case 0x5A:
                 rtn = 0xA5;
                 break;
         case 0x99:
                 rtn = 0x66;
                 break;
         case 0x6A:
                 rtn = 0x95;
                 break;
         default:
                 rtn = ch;
                 break;
     }
     return rtn;
 }
//组帧，解析帧
bool Frame::PackFrame(Msg src, INT8U * dst, INT8U *len)
{

     // 增加CRC校验
     src.crc = CRC8(src.data, src.length);

     dst[0] = 0x5A;
     dst[1] = 0x55;
     int8_t j = 2;
     // lenth
     if (src.length == protocol_deconvert(src.length))
     {
         dst[j++] = src.length;
     }
     else
     {
         dst[j++] = 0x99;
         dst[j++] = protocol_deconvert(src.length);
     }
     //data
     for (int i = 0; i < src.length; i++)
     {
         if (src.data[i] == protocol_deconvert(src.data[i]))
         {
             dst[j++] = src.data[i];
         }
         else
         {
             dst[j++] = 0x99;
             dst[j++] = protocol_deconvert(src.data[i]);
         }
     }
     //crc
     if (src.crc == protocol_deconvert(src.crc))
     {
         dst[j++] = src.crc;
     }
     else
     {
         dst[j++] = 0x99;
         dst[j++] = protocol_deconvert(src.crc);
     }

     dst[j++] = 0x6A; //packet tail1
     dst[j++] = 0x69; //packet tail2
     (*len) = j;

     return true;

 }


INT8U Frame::UnpackFrame(INT8U ch, Msg *pmsg)
{
  if ((ch == 0x5a) && (frame_state != F_HEADER_H) && (frame_state != F_CRC))
  {
      frame_state = F_HEADER_H;
  }
  if ((ch == 0x6a) && (frame_state != F_END_H) && (frame_state != F_CRC))
  {
      frame_state = F_ERROR;
  }

  if (frame_state == F_HEADER_H)
  {
      if (ch == 0x5A)
      {
          data_point = 0;
          frame_state = F_HEADER_L;
      }
      else
      {
          frame_state = F_ERROR;
      }
  }
  else if (frame_state == F_HEADER_L)
  {
      if (ch == 0x55)
      {
          frame_state = F_LENGTH;
      }
      else
      {
          frame_state = F_ERROR;
      }
  }
  else if (frame_state == F_LENGTH)
  {
      if (ch == 0x99)
      {
          converter = 1;
          return 0;
      }
      pmsg->length = protocol_convert(ch);
      if (pmsg->length > MAX_MSG_LEN)
      {
          frame_state = F_ERROR;
      }
      else
      {
          frame_state = F_DATA;
      }
  }
  else if (frame_state == F_DATA)
  {
      if (pmsg->length == 0)//没有数据区
      {
          frame_state = F_CRC;
          return 0;
      }

      if (ch == 0x99)    //转义
      {
          converter = 1;
          return 0;
      }

      pmsg->data[data_point] = protocol_convert(ch);
      data_point++;
      if (data_point == pmsg->length)
      {
          data_point = 0;
          frame_state = F_CRC;
      }
  }
  else if (frame_state == F_CRC)
  {
      if (ch == 0x99)    //转义
      {
          converter = 1;
          return 0;
      }
      pmsg->crc = protocol_convert(ch);
      frame_state = F_END_H;
  }
  else if (frame_state == F_END_H)
  {
      if (ch != 0x6A)
      {
          frame_state = F_ERROR;
      }
      else
      {
          frame_state = F_END_L;
      }

  }
  else if (frame_state == F_END_L)
  {
      if (ch != 0x69)
      {
          frame_state = F_ERROR;
      }
      else
      {
          // frame_state = FRAME_STATE.F_HEADER_H;
          //CRC success
          if (pmsg->crc == CRC8(pmsg->data, pmsg->length))
          {
              frame_state = F_HEADER_H;
              return 1;
          }
          else
          {
              frame_state = F_ERROR;
          }
      }
  }

  if (frame_state == F_ERROR)
  {
      frame_state = F_HEADER_H;
      return 2;
  }

  return 0;
}
*/

