#include <string.h>
#include <sal.h>
#include "sal_bmp.h"
#include "sal_colorSpace.h"
#include "sal_macro.h"

#define YUV_TO_RGB(Y, U, V, R, G, B) \
        { \
            R = clip((298 * (Y-16) + 409 * (V-128) + 128) >> 8); \
            G = clip((298 * (Y-16) - 100 * (U-128) - 208 * (V-128) + 128) >> 8); \
            B = clip((298 * (Y-16) + 516 * (U-128) + 128) >> 8); \
        }

//Y =  0.257*R + 0.504*G + 0.098*B + 16;
//U = -0.148*R - 0.291*G + 0.439*B + 128;
//V =  0.439*R - 0.368*G - 0.071*B + 128;
#define RGB_TO_YUV(R, G, B, Y, U, V) \
        { \
            Y = ((66  * R + 129 * G + 25  * B + 128) >> 8) + 16;  \
            U = ((-38 * R - 74  * G + 112 * B + 128) >> 8) + 128; \
            V = ((112 * R - 94  * G - 18  * B + 128) >> 8) + 128; \
        }



int SAL_bmpTonv12(BMP_FRAME *sFrm, unsigned char *outBuf)
{
    int            w,h,i,j;
    int            s32Ret  = SAL_SOK;
    unsigned char* pInBuf  = NULL;
    unsigned char* pY      = NULL;
    unsigned char* pUV     = NULL;
    unsigned char* dstY    = NULL;
    unsigned char* dstUV   = NULL;

    unsigned char  R,G,B,Y,U,V;

    w = sFrm->width;
    h = sFrm->height;

    dstY  = outBuf;
    dstUV = dstY + w*h;

    for (i = 0; i < h; i++)
    {
        // 从bmp中读取一行数据
        pInBuf = sFrm->data + (h-1-i)*((w*3 + 3)/4*4);
        pY     = dstY  + i * w;
        pUV    = dstUV + i * w / 2;

        // 转换
        if ((i & 1) == 0)
        {
            for (j = 0; j < w; j++)
            {
                B = pInBuf[j*3];
                G = pInBuf[j*3+1];
                R = pInBuf[j*3+2];
                RGB_TO_YUV(R,G,B,Y,U,V);
                pY[j]=Y;
                if ((j & 1) == 0)
                {
                    pUV[j]=U;
                    pUV[j+1]=V;
                }
            }
        }
        else
        {
            for (j = 0; j < w; j++)
            {
                B = pInBuf[j*3];
                G = pInBuf[j*3+1];
                R = pInBuf[j*3+2];
                RGB_TO_YUV(R,G,B,Y,U,V);
                pY[j]=Y;
            }
        }
    }

    return s32Ret;
}

int SAL_bmpToBGRY(BMP_FRAME *sFrm,int img_w,int img_h,unsigned char* yBuf,unsigned char* rBuf,unsigned char* gBuf,unsigned char* bBuf)
{
    int            s32Ret  = SAL_SOK;
    int            i = 0,j = 0;
    int            w = sFrm->width;
    int            h  = sFrm->height;
    unsigned char* pInBuf;
    unsigned char* pR;
    unsigned char* pG;
    unsigned char* pB;
    unsigned char* pY;
    unsigned R,G,B,Y,U,V;
    for (i = 0; i < img_h; i++)
    {
        pInBuf = sFrm->data + (h-1-i)*((w*3 + 3)/4*4);
        pY     = yBuf  + i * img_w;
        pR     = rBuf  + i * img_w;
        pG     = gBuf  + i * img_w;
        pB     = bBuf  + i * img_w;
        for (j = 0; j < img_w; j++)
        {
            B = pInBuf[j*3];
            G = pInBuf[j*3+1];
            R = pInBuf[j*3+2];
            RGB_TO_YUV(R,G,B,Y,U,V);
            pY[j] =Y;
            pR[j] = B;
            pG[j] = G;
            pB[j] = R;
        }
    }
    return s32Ret;
}


/*******************************************************************************
* 函数名  : SAL_bmpGetPicInfo
* 描  述  : 解析 BMP 信息
* 输  入  : - pBmp    : BMP 图片地址
*         : - bmpLen  : BMP 图片长度
*         : - bmpFrm  : BMP 图片信息

* 输  出  : 无
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
*******************************************************************************/
static INT32 SAL_bmpGetPicInfo(unsigned char* pBmp,int bmpLen,BMP_FRAME *bmpFrm)
{
    INT32 w , h;
    INT32 FlieSize = 0;
    BMP_FILE_HEADER  *pFHeader = NULL;
    BMP_INFO_HEADER  *pIHeader = NULL;

    if((NULL == pBmp) || (NULL == bmpFrm))
    {
        SAL_ERROR("!!!\n");
        return SAL_FAIL;
    }

    pFHeader = (BMP_FILE_HEADER *)pBmp;
    pIHeader = (BMP_INFO_HEADER *)(pBmp + sizeof(BMP_FILE_HEADER));
    w = (unsigned int)pIHeader->biWidth;
    h = (unsigned int)pIHeader->biHeight;
    FlieSize = (pFHeader->bfSizeHigh << 16) + pFHeader->bfSizeLow;

    if((pFHeader->bfType != 0x4D42)
        || (FlieSize < 55)
        || (pIHeader->biSize < 40)
        || (w == 0) || h == 0)
    {
        SAL_ERROR("bfType : %x FlieSize = %d biSize = %d, w = %d, h = %d\n", 
                  pFHeader->bfType, FlieSize, pIHeader->biSize, w, h);
        return SAL_FAIL;
    }

    if (FlieSize != bmpLen)
    {
        SAL_ERROR("wrong bmp data, FlieSize : %d , bmpLen : %d\n", FlieSize, bmpLen);
        return SAL_FAIL;
    }

    if((bmpLen - pFHeader->bfOffBitsLow) < w * h * 3)
    {
        SAL_ERROR("1 %x, %d %d %d %d %d  %d %d\n",
            pFHeader->bfType,pFHeader->bfSizeLow,pIHeader->biSize,pIHeader->biSizeImage,w,h,bmpLen,pFHeader->bfOffBitsLow);
        return SAL_FAIL;
    }

    bmpFrm->width  = w;
    bmpFrm->height = h;
    bmpFrm->data   = pBmp + pFHeader->bfOffBitsLow; /* 跳过文件头偏移，获取bmp的bgr数据地址 */
    bmpFrm->widthAlign = (FlieSize - bmpFrm->width * bmpFrm->height * 3 - pFHeader->bfOffBitsLow) / bmpFrm->height;
    bmpFrm->length = FlieSize - pFHeader->bfOffBitsLow;

    SAL_INFO("PIC INFO : W %d, H %d, biBitCount %d, length %d,  validW %d, validH %d , widthAlign %d!!!\n",
             bmpFrm->width, bmpFrm->height, pIHeader->biBitCount, bmpFrm->length, bmpFrm->validW, bmpFrm->validH, bmpFrm->widthAlign);

    return SAL_SOK;
}

/*******************************************************************************
* 函数名  : SAL_bmpPicDec
* 描  述  : 解码 BMP 图片 (24 位 BMP 图片解码)
* 输  入  : - pBinBmpBufmp  : BMP 图片地址
*         : - inBmpLen      : BMP 图片长度

* 输  出  : - pYUVData      : 解码后 YUV 数据
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
*******************************************************************************/
INT32 SAL_bmpPicDec(UINT8 *inBmpBuf, UINT32 inBmpLen, UINT8* pYUVData, UINT32* yuvW, UINT32* yuvH)
{
    INT32 iRet = 0;
    BMP_FRAME stBmpFrm;
    UINT8* pixels = NULL;
    UINT8* img_data_bgr_plane = NULL;
    UINT32 cols = 0;
    UINT32 rows = 0;
    UINT32 stride = 0;
    UINT8* temp = NULL;
    UINT8* yuvTemp = NULL;
    UINT32 h = 0;

    SAL_memSetSafe(&stBmpFrm, 0, sizeof(BMP_FRAME));

    iRet = SAL_bmpGetPicInfo(inBmpBuf, inBmpLen, &stBmpFrm);
    if (SAL_SOK != iRet)
    {
        return SAL_FAIL;
    }
    /* 24位的bmp的rgb数据按照b g r顺序排列，每个颜色一个字节 */
    cols = stBmpFrm.width;
    rows = stBmpFrm.height;
    stride = cols * 3 + stBmpFrm.widthAlign; /* 理论的bmp宽都是4对齐的 */

    img_data_bgr_plane  = (UINT8*)SAL_memAlloc(stBmpFrm.length, "bmp");
    pixels              = (UINT8*)SAL_memAlloc(stBmpFrm.length, "bmp");
    temp                = (UINT8*)SAL_memAlloc(stride, "bmp");
    yuvTemp             = (UINT8*)SAL_memAlloc(cols * rows * 3 / 2, "bmp");
    /* bmp的颜色存放顺序是从最后一行到第一行，转换成yuv需要先调整顺序 */
    for (h = 0; h < rows; h++)
    {
        SAL_memCpySafe(temp, (stBmpFrm.data + stride * h), stride);
        SAL_memCpySafe((pixels + (rows - 1 - h) * cols * 3), temp, cols * 3);
    }

    SAL_BGRCross2BGRPlane(pixels, img_data_bgr_plane, stBmpFrm.height, stBmpFrm.width);
    SAL_BGR2YUV420(yuvTemp, stBmpFrm.width, stBmpFrm.height, img_data_bgr_plane);
    SAL_I420ToNV12(yuvTemp, pYUVData, cols, rows);



    #if 0

    SAL_WriteFile("/mnt/sd/aaa.yuv", pYUVData, cols * rows * 3/2);

    #endif
    if (NULL != pixels)
    {
        SAL_memFree(pixels);
        pixels = NULL;
    }

    if (NULL != temp)
    {
        SAL_memFree(temp);
        temp = NULL;
    }
    
    if (NULL != yuvTemp)
    {
        SAL_memFree(yuvTemp);
        yuvTemp = NULL;
    }
    if (NULL != img_data_bgr_plane)
    {
        SAL_memFree(img_data_bgr_plane);
        img_data_bgr_plane = NULL;
    }
    *yuvW = stBmpFrm.width;
    *yuvH = stBmpFrm.height;

    return SAL_SOK;
}

/*******************************************************************************
* 函数名  : SAL_nv12Tobmp
* 描  述     : 编码 BMP 图片 (24 位 BMP 图片编码)
* 输  入     : - 
*                   : - 
* 输  出  : - 
* 返回值  : HIK_SOK  : 成功
*                     HIK_FAIL  : 失败
*******************************************************************************/
int SAL_nv12Tobmp(unsigned char *virAddr,UINT32 width,UINT32 height,UINT32 dstLen,UINT32 *dataLen)
{
    UINT32           i           =   0;
    UINT32           j           =   0;
    UINT32           s32Ret      =   SAL_SOK;
    char*            pTemp       =   NULL;
    UINT32           tBufLen     =   0;
    UINT32           dataSize    =   0;
    UINT32           pixSize     =   0;
    BMP_FILE_HEADER *pFileHead   =   NULL;
    BMP_INFO_HEADER *pInfoHead   =   NULL;
    unsigned char   *pR          =   NULL;
    unsigned char   *pG          =   NULL;
    unsigned char   *pB          =   NULL;
    unsigned char   *pY          =   NULL;
    unsigned char   *pU          =   NULL;
    unsigned char   *pV          =   NULL;
    unsigned char   *pRgbLine    =   NULL;
    char            *pRgbBase    =   NULL;
    unsigned char   *pYBase      =   NULL;
    unsigned char   *pUvBase     =   NULL;

    pixSize    = 3;
    dataSize   = width * height * pixSize;
    tBufLen    = dataSize + sizeof(BMP_FILE_HEADER) + sizeof(BMP_INFO_HEADER);

    if(dstLen < tBufLen)
    {
        SAL_ERROR("bmp too large to save bm-%d buf-%d\n",tBufLen,dstLen);
        return SAL_FAIL;
    }

    pTemp    = SAL_memAlloc(tBufLen,"bmptempbuf");
    if(NULL == pTemp)
    {
        SAL_ERROR("pTemp %p\n",pTemp);
        return SAL_FAIL;
    }

    pFileHead  = (BMP_FILE_HEADER *)pTemp;
    SAL_memSetSafe(pFileHead,0,sizeof(BMP_FILE_HEADER));
    pFileHead->bfType          = 0x4d42;
    pFileHead->bfSizeLow       = (54 + dataSize) & 0xffff;
    pFileHead->bfSizeHigh      = ((54 + dataSize) >> 16) & 0xffff;
    pFileHead->bfReserved1     = 0;
    pFileHead->bfReserved2     = 0;
    pFileHead->bfOffBitsLow    = 54;

    pInfoHead  = (BMP_INFO_HEADER *)(pTemp + sizeof(BMP_FILE_HEADER));
    SAL_memSetSafe(pInfoHead, 0x0, sizeof(BMP_INFO_HEADER));

    pInfoHead->biSize          = 40;
    pInfoHead->biWidth         = width;
    pInfoHead->biHeight        = height;
    pInfoHead->biPlanes        = 1;
    pInfoHead->biBitCount      = pixSize*8;
    pInfoHead->biCompression   = 0;
    pInfoHead->biSizeImage     = dataSize;
    pInfoHead->biXPelsPerMeter = 0xec4;
    pInfoHead->biYPelsPerMeter = 0xec4;
    pInfoHead->biClrUsed       = 0;
    pInfoHead->biClrImportant  = 0;
    
    pRgbBase   =  pTemp + sizeof(BMP_FILE_HEADER) + sizeof(BMP_INFO_HEADER);
    pYBase     =  virAddr;
    pUvBase    =  virAddr + width * height;
    pRgbLine   =  SAL_memAlloc(width * pixSize,"bmpline");

    if(NULL == pRgbLine)
    {
        SAL_ERROR("pRgbLine %p\n",pRgbLine);
        SAL_memFree(pTemp);
        return SAL_FAIL;
    }

    for (i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            pY   =   pYBase  + width * i + j;
            pU   =   pUvBase + width * (i / 2) + (j / 2) * 2;
            pV   =   pUvBase + width * (i / 2) + (j / 2) * 2 + 1;
            
            pB   =   pRgbLine + j * pixSize;
            pG   =   pRgbLine + j * pixSize + 1;
            pR   =   pRgbLine + j * pixSize + 2;
            CVT_YUV2RGB(*pY,*pU,*pV,*pR,*pG,*pB);
        }
        
        SAL_memCpySafe(pRgbBase + (height - 1 - i) * width * pixSize, pRgbLine ,width * pixSize);
    }

    SAL_memCpySafe(virAddr,pTemp,tBufLen);
    *dataLen  = tBufLen;

    SAL_memFree(pTemp);
    SAL_memFree(pRgbLine);

    return s32Ret;
}

