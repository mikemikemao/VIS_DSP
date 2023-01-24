
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

/* һ�� 14 �ֽ� */
typedef struct
{
    unsigned short bfType;           ///< �ļ���ʶ��һ��Ϊ0x4D42����'BM'
    unsigned short bfSizeLow;        ///< ��λ ���ֽڱ�ʾ�������ļ���С   ����ƽ̨������ int �� bfsize �ֳ����� short �� unsigned int bfSize
    unsigned short bfSizeHigh;       ///<  ��λ �����ļ���С = bfSize1 + bfSize1
    unsigned short bfReserved1;      ///< ������һ������Ϊ0
    unsigned short bfReserved2;     ///< ������һ������Ϊ0
    unsigned short bfOffBitsLow;    ///< λͼ����ƫ��
    unsigned short bfOffBitsHigh;   ///< λͼ����ƫ��
} BMP_FILE_HEADER;

typedef struct
{
    unsigned int   biSize;      ///< λͼ��Ϣͷ�Ĵ�С��ͨ����28H
    unsigned int   biWidth;     ///< λͼ���ؿ��
    unsigned int   biHeight;    ///< λͼ���ظ߶�
    unsigned short biPlanes;    ///< λͼ��λ����������1
    unsigned short biBitCount;  ///< ����λ��
    unsigned int   biCompression;   ///< ѹ��˵��
    unsigned int   biSizeImage;     ///< λͼ���ݵĴ�С��4�ֽڵı���
    unsigned int   biXPelsPerMeter; ///< ������/�ױ�ʾ��ˮƽ�ֱ��ʣ�Ĭ��0x0EC4
    unsigned int   biYPelsPerMeter; ///< ������/�ױ�ʾ�Ĵ�ֱ�ֱ��ʣ�Ĭ��0x0EC4
    unsigned int   biClrUsed;       ///< λͼʹ�õ���ɫ��
    unsigned int   biClrImportant;  ///< ��Ҫ����ɫ��
} BMP_INFO_HEADER;


extern int bmpTonv12(BMP_FRAME *sFrm, unsigned char *outBuf);
extern int bmpToBGRY(BMP_FRAME *sFrm,int img_w,int img_h,unsigned char* yBuf,unsigned char* rBuf,unsigned char* gBuf,unsigned char* bBuf);

/*******************************************************************************
* ������  : SAL_bmpPicDec
* ��  ��  : ���� BMP ͼƬ (24 λ BMP ͼƬ����)
* ��  ��  : - pBinBmpBufmp  : BMP ͼƬ��ַ
*         : - inBmpLen      : BMP ͼƬ����

* ��  ��  : - pYUVData      : ����� YUV ����
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_bmpPicDec(UINT8 *inBmpBuf, UINT32 inBmpLen, UINT8* pYUVData, UINT32* yuvW, UINT32* yuvH);

#endif
