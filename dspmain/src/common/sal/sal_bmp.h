
#ifndef _SAL_BMP_H_
#define _SAL_BMP_H_

#include <sal.h>
#include <sys/time.h>
#include <string.h>
#include <semaphore.h>

typedef struct
{
    unsigned char *data;
    unsigned int  width;
    unsigned int  height;
    unsigned int  validW;
    unsigned int  validH; 
    unsigned int  widthAlign;
    unsigned int  length;
}BMP_FRAME;

/* 一共 14 字节 */
typedef struct
{
    unsigned short bfType;           ///< 文件标识，一般为0x4D42，即'BM'
    unsigned short bfSizeLow;        ///< 低位 用字节表示的整个文件大小   考虑平台兼容性 int 型 bfsize 分成两个 short 型 unsigned int bfSize
    unsigned short bfSizeHigh;       ///<  高位 整个文件大小 = bfSize1 + bfSize1
    unsigned short bfReserved1;      ///< 保留，一般设置为0
    unsigned short bfReserved2;     ///< 保留，一般设置为0
    unsigned short bfOffBitsLow;    ///< 位图数据偏移
    unsigned short bfOffBitsHigh;   ///< 位图数据偏移
} BMP_FILE_HEADER;

typedef struct
{
    unsigned int   biSize;      ///< 位图信息头的大小，通常是28H
    unsigned int   biWidth;     ///< 位图像素宽度
    unsigned int   biHeight;    ///< 位图像素高度
    unsigned short biPlanes;    ///< 位图的位面数，总是1
    unsigned short biBitCount;  ///< 像素位宽
    unsigned int   biCompression;   ///< 压缩说明
    unsigned int   biSizeImage;     ///< 位图数据的大小，4字节的倍数
    unsigned int   biXPelsPerMeter; ///< 用象素/米表示的水平分辨率，默认0x0EC4
    unsigned int   biYPelsPerMeter; ///< 用象素/米表示的垂直分辨率，默认0x0EC4
    unsigned int   biClrUsed;       ///< 位图使用的颜色数
    unsigned int   biClrImportant;  ///< 重要的颜色数
} BMP_INFO_HEADER;


extern int bmpTonv12(BMP_FRAME *sFrm, unsigned char *outBuf);
extern int bmpToBGRY(BMP_FRAME *sFrm,int img_w,int img_h,unsigned char* yBuf,unsigned char* rBuf,unsigned char* gBuf,unsigned char* bBuf);

/*******************************************************************************
* 函数名  : SAL_bmpPicDec
* 描  述  : 解码 BMP 图片 (24 位 BMP 图片解码)
* 输  入  : - pBinBmpBufmp  : BMP 图片地址
*         : - inBmpLen      : BMP 图片长度

* 输  出  : - pYUVData      : 解码后 YUV 数据
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
*******************************************************************************/
INT32 SAL_bmpPicDec(UINT8 *inBmpBuf, UINT32 inBmpLen, UINT8* pYUVData, UINT32* yuvW, UINT32* yuvH);

#endif
