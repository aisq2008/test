#include <QCoreApplication>


uint  getData8421BCD(uchar  buff[8],uchar  offset,uchar  len,
                        uint & rawValue,bool isInter)
{
    uint   errNum = 1;
    uchar  ucBitOffset = offset;
    uchar  ucBitLength = len;
    if( 0 == ucBitLength || 32 < ucBitLength )
    {
        return errNum;
    }

    int byteOffset      = 0;
    int bitInByteOffset = 0;

    uchar  byteMsk       = 0x01;
    byteOffset          = ucBitOffset/8;
    bitInByteOffset     = ucBitOffset - byteOffset*8;
    byteMsk             = byteMsk<<bitInByteOffset;

    uint rsValue        = 0;

    uchar  tempByte = 0;
    uchar  tempMask = 0x01;
    int index = 0;

    int factor = 1;

    if(isInter)
    {
        for(int i=0;i<ucBitLength;i++)
        {
            if(buff[byteOffset] & byteMsk)
            {
                tempByte = tempByte | tempMask;
            }
            else
            {
                tempByte = tempByte & (~tempMask);
            }


            byteMsk = byteMsk<<1;
            tempMask = tempMask<<1;
            index++;

            if(tempByte > 9)//BCD码取值不能大于9
            {
                //QMessageBox
                return errNum;
            }


            if(i == (ucBitLength-1))
            {
                rsValue = rsValue + tempByte * factor;
            }
            else if(4 == index)//一个BCD码读完 进位
            {
                rsValue = rsValue + tempByte * factor;
                factor = factor * 10;

                index    = 0;
                tempByte = 0;
                tempMask = 0x01;

            }

            if(!byteMsk)//一字节完了，取下一个字节
            {
                byteOffset++;
                byteMsk  = 0x01;
            }

        }

    }
    else
    {
        uchar  tempByte = 0;
        uchar  tempMask = 0x01;
        int index = 0;

        factor = 10;
        for(int i=0;i<ucBitLength;i++)
        {
            if(buff[byteOffset] & byteMsk)
            {
                tempByte = tempByte | tempMask;
            }
            else
            {
                tempByte = tempByte & (~tempMask);
            }


            byteMsk = byteMsk<<1;
            tempMask = tempMask<<1;
            index++;


            if(i == (ucBitLength-1))
            {
                if(index>4)
                {
                    rsValue = rsValue * factor + (tempByte>>4);
                }

                tempByte = tempByte<<4;
                rsValue = rsValue * factor + (tempByte>>4);
            }
            else if(!byteMsk)//一字节完了，取下一个字节
            {
                if(！tempMask) //获得一个字节数据
                {
                    rsValue = rsValue * factor + (tempByte>>4);

                    tempByte = tempByte<<4;
                    rsValue = rsValue * factor + (tempByte>>4);

                    tempMask = 0x01;
                    index    = 0;
                    tempByte = 0;
                }


                byteOffset++;
                byteMsk  = 0x01;

            }

        }


//        for(int i=0;i<ucBitLength;i++)
//        {
//            if(buff[byteOffset] & byteMsk)
//            {
//                tempByte = tempByte | tempMask;
//            }
//            else
//            {
//                tempByte = tempByte & (~tempMask);
//            }


//            byteMsk = byteMsk<<1;
//            tempMask = tempMask<<1;
//            index++;

//            if(tempByte > 9)//BCD码取值不能大于9
//            {
//                //QMessageBox
//                return errNum;
//            }


//            if(i == (ucBitLength-1))
//            {
//                rsValue = rsValue * factor + tempByte;
//            }
//            else if(4 == index)//一个BCD码读完 进位
//            {
//                rsValue = rsValue * factor + tempByte;
//                factor = 10;

//                index    = 0;
//                tempByte = 0;
//                tempMask = 0x01;

//            }

//            if(!byteMsk)//一字节完了，取下一个字节
//            {
//                byteOffset++;
//                byteMsk  = 0x01;
//            }

//        }

    }

    rawValue = rsValue;

    return 0;
}

uint  getDataUnsigned(uchar  buff[8],uchar  offset,uchar  len,
                        uint & rawValue,bool isInter)
{
    //isInter 小端法

    uint   errNum = 1;
    uchar  ucBitOffset = offset;
    uchar  ucBitLength = len;
    if( 0 == ucBitLength || 32 < ucBitLength )
    {
        return errNum;
    }

    int byteOffset      = 0;
    int bitInByteOffset = 0;

    uchar  byteMsk       = 0x01;
    byteOffset          = ucBitOffset/8;
    bitInByteOffset     = ucBitOffset - byteOffset*8;
    byteMsk             = byteMsk<<bitInByteOffset;

    uint rsValue            = 0;
    uint  uintMsk       = 0;

    if(isInter)
    {
        uintMsk       = 0x00000001;
        for(int i=0;i<ucBitLength;i++)
        {
            if(buff[byteOffset] & byteMsk)
            {
                rsValue = rsValue | uintMsk;
            }
            else
            {
                rsValue = rsValue & (~uintMsk);
            }

            byteMsk = byteMsk<<1;
            uintMsk = uintMsk<<1;
            if(!byteMsk)//一字节完了，取下一个字节
            {
                //Intel format 【小端法 litter endian】
                byteOffset++;
                byteMsk = 0x01;
            }
        }
    }
    else
    {
        uchar  tempByte = 0;
        uchar  tempMask = 0x01;
        int index = 0;

        for(int i=0;i<ucBitLength;i++)
        {
            if(buff[byteOffset] & byteMsk)
            {
                tempByte = tempByte | tempMask;
            }
            else
            {
                tempByte = tempByte & (~tempMask);
            }


            byteMsk = byteMsk<<1;
            tempMask = tempMask<<1;
            index++;


            if(i == (ucBitLength-1))
            {
                rsValue = rsValue<<index;
                rsValue = rsValue | tempByte;
            }
            else if(!byteMsk)//一字节完了，取下一个字节
            {
                rsValue = rsValue<<index;
                rsValue = rsValue | tempByte;

                byteOffset++;
                byteMsk  = 0x01;
                tempMask = 0x01;
                index    = 0;
                tempByte = 0;

            }

        }
    }


    rawValue = rsValue;

    return 0;

}





int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    uchar buffer[4] = {0x12,0x34,0x56,0x78};

    int tm = 3;

    int num = 1;
    num = num<<tm;

    uchar offset = 2;
    uchar len = 8;

    uint rs = 0;
    getData8421BCD(buffer,offset,len,rs,true);

    uint rs2 = 0;
    getData8421BCD(buffer,offset,len,rs2,false);

    uint rs3 = 0;
    getDataUnsigned(buffer,offset,len,rs3,true);

    uint rs4 = 0;
    getDataUnsigned(buffer,offset,len,rs4,false);


    rs = 1;
    
    return a.exec();
}
