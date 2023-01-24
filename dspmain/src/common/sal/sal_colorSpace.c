/*******************************************************************************
* ��Ȩ��Ϣ����Ȩ���� (c) 2010-2040, ���ݺ����������ּ����ɷ����޹�˾, ��������Ȩ��
* �ļ����ƣ�color_space.c
* �ļ���ʶ��COLOR_SPACE_C
* ժ    Ҫ����ɫ�ռ�ת��
* ��    ����1.0.0
* ��    �ߣ�����,�����
* ��    �ڣ�2012��3��21��
* ��    ע������
********************************************************************************/

/* ========================================================================== */
/*                             ͷ�ļ���                                       */
/* ========================================================================== */
#include <string.h>
#include <stdio.h>
#include "sal_colorSpace.h"
#include "sal_trace.h"

/* ========================================================================== */
/*                           ������Ͷ�����                                   */
/* ========================================================================== */


/* ========================================================================== */
/*                          ���ݽṹ������                                    */
/* ========================================================================== */


/* ========================================================================== */
/*                          ����������                                        */
/* ========================================================================== */



/* ========================================================================== */
/*                          ����������                                        */
/* ========================================================================== */

/**************************************************************************************************
* ��  �ܣ���ɫ�ռ�ת����yuvתhsv�Ļ�������
* ��  ����IN:
*           y                              -  yͨ��������ֵ
*           u                              -  uͨ��������ֵ
*           v                              -  vͨ��������ֵ
        OUT:
*           h_val                          -  hͨ��������ֵ
*           s_val                          -  s��������ֵ
*           v_val                          -  vͨ��������ֵ
* ����ֵ����
* ��  ע:
**************************************************************************************************/
static int VCA_cvt_yuv2hsv(unsigned char  y,
                        unsigned char  u,
                        unsigned char  v,
                        unsigned char *h_val,
                        unsigned char *s_val,
                        unsigned char *v_val)
{
    unsigned char r, g, b;
    /* int rdif,bdif, invgdif; */
    int dif_val;
    int h_tmp, s_tmp, v_tmp;
    int v_min;
    int vr, vg;

    if ((!h_val) || (!s_val) || (!v_val))
    {
        return -1;
    }
        
    /* �Ƚ�YUVת��RGB */
    CVT_YUV2RGB(y, u, v, r, g, b);

    v_tmp   = b;
    v_min   = b;

    /* ��ȡRGB�����Сֵ */
    v_tmp = SAL_max(v_tmp, g);
    v_tmp = SAL_max(v_tmp, r);
    v_min = SAL_min(v_min, g);
    v_min = SAL_min(v_min, r);

    dif_val = v_tmp - v_min;
    vr = v_tmp == r ? -1 : 0;
    vg = v_tmp == g ? -1 : 0;

    /* ����ʹ�ò�� */
    s_tmp = dif_val * (div_table[v_tmp] >> 12);

    /* ����h����Ҫ����rgbֵ����ѡ�� */
    h_tmp = ((vr & (g - b))) +          /* ��ɫֵ��� */
            /* ��ɫֵ��� */
            ((((~vr) & ((vg & (b - r + 2 * dif_val)))) +
              /* ��ɫֵ��� */
              (((~vg) & (r - g + 4 * dif_val)))));
    h_tmp = (h_tmp * hdiv_table180[dif_val] + (1 << 11)) >> 12;
    h_tmp += h_tmp < 0 ? 180 : 0;       /* hue�ķ�ΧΪ[0,180] */

    /* ����HSV */
    *h_val = (unsigned char)h_tmp;
    *s_val = (unsigned char)s_tmp;
    *v_val = (unsigned char)v_tmp;

    return 0;
}


/**************************************************************************************************
* ��  �ܣ���ɫ�ռ�ת�������ؼ���ת��
* ��  ����IN:
*           src1                           -  ��һͨ��������ֵ
*           src2                           -  �ڶ�ͨ��������ֵ
*           src3                           -  ����ͨ��������ֵ
*           code                           -  ת����ʾ
*         OUT:
*           dst1                           -  ��һͨ��������ֵ
*           dst2                           -  �ڶ�ͨ��������ֵ
*           dst3                           -  ����ͨ��������ֵ
* ����ֵ����
* ��  ע:
**************************************************************************************************/
void VCA_cvt_color_pix(unsigned char  src1,
                       unsigned char  src2,
                       unsigned char  src3,
                       unsigned char *dst1,
                       unsigned char *dst2,
                       unsigned char *dst3,
                       unsigned int  code)
{
    if ((!dst1) || (!dst2) || (!dst3))
    {
        return;
    }
    
    switch(code)
    {
        case VCA_PIX_YUV_RGB:
            CVT_YUV2RGB(src1, src2, src3, *dst1, *dst2, *dst3);
            break;

        case VCA_PIX_RGB_YUV:
            CVT_RGB2YUV(src1, src2, src3, *dst1, *dst2, *dst3);
            break;

        case VCA_PIX_YUV_HSV:
            VCA_cvt_yuv2hsv(src1, src2, src3, dst1, dst2, dst3);
            break;

        default:
            break;
    }
}


/**************************************************************************************************
* ��  �ܣ���ɫ�ռ�ת����YUVתRGB��ͼ�񼶱�
* ��  ����IN:
*           src1                           -  ��һͨ��������ֵ
*           src2                           -  �ڶ�ͨ��������ֵ
*           src3                           -  ����ͨ��������ֵ
*           img_w                          -  ͼ����
*           img_h                          -  ͼ��߶�
*         OUT:
*           dst1                           -  ��һͨ��������ֵ
*           dst2                           -  �ڶ�ͨ��������ֵ
*           dst3                           -  ����ͨ��������ֵ
* ����ֵ����
* ��  ע:
**************************************************************************************************/
void VCA_cvt_YUV2RGB_img(unsigned char *src1,
                         unsigned char *src2,
                         unsigned char *src3,
                         unsigned char *dst1,
                         unsigned char *dst2,
                         unsigned char *dst3,
                         unsigned int img_w, unsigned int img_h)
{
    unsigned int i, j, img_size;

    img_size = img_w * img_h;

    for(i = 0; i < img_size; i += 2)
    {
        CVT_YUV2RGB(src1[i], src2[i], src3[i], dst1[i], dst2[i], dst3[i]);
        j = i + 1;
        CVT_YUV2RGB(src1[j], src2[j], src3[j], dst1[j], dst2[j], dst3[j]);
    }
}


/**************************************************************************************************
* ��  �ܣ���ɫ�ռ�ת����YUVתHSV��ͼ�񼶱�
* ��  ����IN:
*           src1                           -  ��һͨ��������ֵ
*           src2                           -  �ڶ�ͨ��������ֵ
*           src3                           -  ����ͨ��������ֵ
*           img_w                          -  ͼ����
*           img_h                          -  ͼ��߶�
*         OUT:
*           dst1                           -  ��һͨ��������ֵ
*           dst2                           -  �ڶ�ͨ��������ֵ
*           dst3                           -  ����ͨ��������ֵ
* ����ֵ����
* ��  ע:
**************************************************************************************************/
void VCA_cvt_YUV2HSV_img(unsigned char *src1,
                         unsigned char *src2,
                         unsigned char *src3,
                         unsigned char *dst1,
                         unsigned char *dst2,
                         unsigned char *dst3,
                         unsigned int img_w,
                         unsigned int img_h)
{
    unsigned int i, j, img_size;

    img_size = img_w * img_h;

    for(i = 0; i < img_size; i += 2)
    {
        VCA_cvt_yuv2hsv(src1[i], src2[i], src3[i], &dst1[i], &dst2[i], &dst3[i]);
        j = i + 1;
        VCA_cvt_yuv2hsv(src1[j], src2[j], src3[j], &dst1[j], &dst2[j], &dst3[j]);
    }
}


/**************************************************************************************************
* ��  �ܣ���ɫ�ռ�ת����RGBתYUV��ͼ�񼶱�
* ��  ����IN:
*           src1                           -  ��һͨ��������ֵ
*           src2                           -  �ڶ�ͨ��������ֵ
*           src3                           -  ����ͨ��������ֵ
*           img_w                          -  ͼ����
*           img_h                          -  ͼ��߶�
*         OUT:
*           dst1                           -  ��һͨ��������ֵ
*           dst2                           -  �ڶ�ͨ��������ֵ
*           dst3                           -  ����ͨ��������ֵ
* ����ֵ����
* ��  ע:
**************************************************************************************************/
void VCA_cvt_RGB2YUV_img(unsigned char *src1,
                         unsigned char *src2,
                         unsigned char *src3,
                         unsigned char *dst1,
                         unsigned char *dst2,
                         unsigned char *dst3,
                         unsigned int img_w,
                         unsigned int img_h)
{
    unsigned int i, j, img_size;

    img_size = img_w * img_h;

    for(i = 0; i < img_size; i += 2)
    {
        CVT_RGB2YUV(src1[i], src2[i], src3[i], dst1[i], dst2[i], dst3[i]);
        j = i + 1;
        CVT_RGB2YUV(src1[j], src2[j], src3[j], dst1[j], dst2[j], dst3[j]);
    }
}


/**************************************************************************************************
* ��  �ܣ���ɫ�ռ�ת����YUV��420����ʽתΪRGB��ͼ�񼶱�
* ��  ����IN:
*           src1                           -  ��һͨ��������ֵ
*           src2                           -  �ڶ�ͨ��������ֵ
*           src3                           -  ����ͨ��������ֵ
*           img_w                          -  ͼ����
*           img_h                          -  ͼ��߶�
*         OUT:
*           dst1                           -  ��һͨ��������ֵ
*           dst2                           -  �ڶ�ͨ��������ֵ
*           dst3                           -  ����ͨ��������ֵ
* ����ֵ����
* ��  ע:
**************************************************************************************************/
static void VCA_cvt_YUV4202RGB_img(unsigned char *src1,
                                   unsigned char *src2,
                                   unsigned char *src3,
                                   unsigned char *dst1,
                                   unsigned char *dst2,
                                   unsigned char *dst3,
                                   unsigned int img_w,
                                   unsigned int img_h)
{
    unsigned int i, j, oft1, oft2;
    unsigned int  qimg_w;

    qimg_w = img_w >> 1;
    oft1   = 0;
    oft2   = 0;

    for(i = 0; i < img_h; i ++)
    {
        oft1 = i * img_w;
        oft2 = (i >> 1) * qimg_w;

        for(j = 0; j < img_w; j += 2)
        {
            CVT_YUV2RGB(src1[oft1], src2[oft2], src3[oft2], \
                        dst1[oft1], dst2[oft1], dst3[oft1]);
            CVT_YUV2RGB(src1[oft1 + 1], src2[oft2], src3[oft2], \
                        dst1[oft1 + 1], dst2[oft1 + 1], dst3[oft1 + 1]);
            oft1 += 2;
            oft2 ++;
        }
    }
}


/**************************************************************************************************
* ��  �ܣ���ɫ�ռ�ת����YUV��420����ʽתΪHSV��ͼ�񼶱�
* ��  ����IN:
*           src1                           -  ��һͨ��������ֵ
*           src2                           -  �ڶ�ͨ��������ֵ
*           src3                           -  ����ͨ��������ֵ
*           img_w                          -  ͼ����
*           img_h                          -  ͼ��߶�
*         OUT:
*           dst1                           -  ��һͨ��������ֵ
*           dst2                           -  �ڶ�ͨ��������ֵ
*           dst3                           -  ����ͨ��������ֵ
* ����ֵ����
* ��  ע:
**************************************************************************************************/
void VCA_cvt_YUV4202HSV_img(unsigned char *src1,
                            unsigned char *src2,
                            unsigned char *src3,
                            unsigned char *dst1,
                            unsigned char *dst2,
                            unsigned char *dst3,
                            unsigned int img_w,
                            unsigned int img_h)
{
    unsigned int i, j, oft1, oft2;
    unsigned int  qimg_w;

    qimg_w = img_w >> 1;
    oft1   = 0;
    oft2   = 0;

    for(i = 0; i < img_h; i ++)
    {
        oft1 = i * img_w;
        oft2 = (i >> 1) * qimg_w;

        for(j = 0; j < img_w; j += 2)
        {
            VCA_cvt_yuv2hsv(src1[oft1], src2[oft2], src3[oft2], \
                            &dst1[oft1], &dst2[oft1], &dst3[oft1]);
            VCA_cvt_yuv2hsv(src1[oft1 + 1], src2[oft2], src3[oft2], \
                            &dst1[oft1 + 1], &dst2[oft1 + 1], &dst3[oft1 + 1]);
            oft1 += 2;
            oft2 ++;
        }
    }
}



/**************************************************************************************************
* ��  �ܣ���ɫ�ռ�ת����YUV��420��uvͨ����֯����ʽתΪRGB��ͼ�񼶱�
* ��  ����IN:
*           src1                           -  ��һͨ��������ֵ
*           src2                           -  �ڶ�,��ͨ��������ֵ
*           img_w                          -  ͼ����
*           img_h                          -  ͼ��߶�
*         OUT:
*           dst1                           -  ��һͨ��������ֵ
*           dst2                           -  �ڶ�ͨ��������ֵ
*           dst3                           -  ����ͨ��������ֵ
* ����ֵ����
* ��  ע:
**************************************************************************************************/
void VCA_cvt_YUV420MV2RGB_img(unsigned char *src1,
                              unsigned char *src2,
                              unsigned char *dst1,
                              unsigned char *dst2,
                              unsigned char *dst3,
                              unsigned int img_w,
                              unsigned int img_h)
{
    unsigned int i, j, oft1, oft2;

    oft1   = 0;
    oft2   = 0;

    for(i = 0; i < img_h; i ++)
    {
        oft1 = i * img_w;
        oft2 = (i >> 1) * img_w;

        for(j = 0; j < img_w; j += 2)
        {
            CVT_YUV2RGB(src1[oft1], src2[oft2], src2[oft2 + 1], \
                        dst1[oft1], dst2[oft1], dst3[oft1]);
            CVT_YUV2RGB(src1[oft1 + 1], src2[oft2 + 2], src2[oft2 + 3], \
                        dst1[oft1 + 1], dst2[oft1 + 1], dst3[oft1 + 1]);
            oft1 += 2;
            oft2 += 2;
        }
    }
}


/**************************************************************************************************
* ��  �ܣ���ɫ�ռ�ת����YUV��420��uvͨ����֯����ʽתΪHSV��ͼ�񼶱�
* ��  ����IN:
*           src1                           -  ��һͨ��������ֵ
*           src2                           -  �ڶ�,��ͨ��������ֵ
*           img_w                          -  ͼ����
*           img_h                          -  ͼ��߶�
*         OUT:
*           dst1                           -  ��һͨ��������ֵ
*           dst2                           -  �ڶ�ͨ��������ֵ
*           dst3                           -  ����ͨ��������ֵ
* ����ֵ����
* ��  ע:
**************************************************************************************************/
void VCA_cvt_YUV420MV2HSV_img(unsigned char *src1,
                              unsigned char *src2,
                              unsigned char *dst1,
                              unsigned char *dst2,
                              unsigned char *dst3,
                              unsigned int img_w,
                              unsigned int img_h)
{
    unsigned int i, j, oft1, oft2;

    oft1  = 0;
    oft2  = 0;

    for(i = 0; i < img_h; ++i)
    {
        oft1 = i * img_w;
        oft2 = (i >> 1) * img_w;

        for(j = 0; j < img_w; j += 2)
        {
            VCA_cvt_yuv2hsv(src1[oft1], src2[oft2], src2[oft2 + 1], \
                            &dst1[oft1], &dst2[oft1], &dst3[oft1]);
            VCA_cvt_yuv2hsv(src1[oft1 + 1], src2[oft2 + 2], src2[oft2 + 3], \
                            &dst1[oft1 + 1], &dst2[oft1 + 1], &dst3[oft1 + 1]);
            oft1 += 2;
            oft2 += 2;
        }
    }
}

/**************************************************************************************************
* ��  �ܣ���ɫ�ռ�ת����ͼ�񼶱�
* ��  ����IN:
*           src1                           -  ��һͨ��������ֵ
*           src2                           -  �ڶ�ͨ��������ֵ
*           src2                           -  ����ͨ��������ֵ
*           img_w                          -  ͼ����
*           img_h                          -  ͼ��߶�
*           code                           -  ͼ��ת����ʾ
*         OUT:
*           dst1                           -  ��һͨ��������ֵ
*           dst2                           -  �ڶ�ͨ��������ֵ
*           dst3                           -  ����ͨ��������ֵ
* ����ֵ����
* ��  ע:
**************************************************************************************************/
void VCA_cvt_color_img(unsigned char *src1,
                       unsigned char *src2,
                       unsigned char *src3,
                       unsigned char *dst1,
                       unsigned char *dst2,
                       unsigned char *dst3,
                       unsigned int img_w,
                       unsigned int img_h,
                       unsigned int code)
{
    switch(code)
    {

        case VCA_IMG_YUV420_RGB:
            VCA_cvt_YUV4202RGB_img(src1, src2, src3, dst1, dst2, dst3, img_w, img_h);
            break;

        case VCA_IMG_YUV420MV_RGB:
            VCA_cvt_YUV420MV2RGB_img(src1, src2, dst1, dst2, dst3, img_w, img_h);
            break;

        case VCA_IMG_YUV_RGB:
            VCA_cvt_YUV2RGB_img(src1, src2, src3, dst1, dst2, dst3, img_w, img_h);
            break;

        case VCA_IMG_RGB_YUV:
            VCA_cvt_RGB2YUV_img(src1, src2, src3, dst1, dst2, dst3, img_w, img_h);
            break;

        case VCA_IMG_YUV_HSV:
            VCA_cvt_YUV2HSV_img(src1, src2, src3, dst1, dst2, dst3, img_w, img_h);
            break;

        case VCA_IMG_YUV420_HSV:
            VCA_cvt_YUV4202HSV_img(src1, src2, src3, dst1, dst2, dst3, img_w, img_h);
            break;

        case VCA_IMG_YUV420MV_HSV:
            VCA_cvt_YUV420MV2HSV_img(src1, src2, dst1, dst2, dst3, img_w, img_h);
            break;

        default:
            break;
    }
}

/*******************************************************************************
* ������  : SAL_NV12Align
* ��  ��  : NV12��ʽͼƬ�ü���
* ��  ��  : - pSrcYuv: NV12 ԭͼ
*         : - pDstYuv   : ������NV12ͼ
*         : - srcWidth  : ԭͼ��
*         : - srcHight  : ԭͼ��
*         : - dstWidth  : ������ͼƬ��
*         ��- dstHight   : ������ͼƬ��
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
void SAL_NV12Align(unsigned char *pSrcYuv, unsigned char *pDstYuv, unsigned int srcWidth, unsigned int srcHight, unsigned int dstWidth, unsigned int dstHeight)
{
    unsigned int i          = 0;
    unsigned int j          = 0;
    unsigned int DstYsize   = dstWidth * dstHeight;
    unsigned int Ysize      = srcWidth * srcHight;
    unsigned int relen      = dstWidth & 0x3;
    unsigned int strideAlign = dstWidth;

    if(relen > 0)
    {
        strideAlign = dstWidth + (4 - relen);
        DstYsize = strideAlign * dstHeight;
    }

    for (i = 0; i < dstHeight; i++)
    {
        SAL_memCpySafe(pDstYuv+i*dstWidth, pSrcYuv+i*srcWidth, dstWidth);
    }

    for (i = 0; i < dstHeight/2; i++)
    {
        for (j = 0; j < dstWidth /2; j++)
        {
            pDstYuv[DstYsize + i*dstWidth + 2 * j]     = pSrcYuv[Ysize + i*srcWidth + 2 * j];
            pDstYuv[DstYsize + i*dstWidth + 2 * j + 1] = pSrcYuv[Ysize + i*srcWidth + 2 * j + 1];
        }
    }
}

/*******************************************************************************
* ������  : SAL_I420ToNV12
* ��  ��  : YUV I420 ת���� NV12��ʽ
* ��  ��  : - pSrcYuv: ������һ���I420��ʽ��ԭͼ
*         : - pDstYuv: Y���UV���NV12��ʽ��Ŀ��ͼ
*         : - Width  : ͼ����
*         : - Hight  :

yyyy                yyyy
uu       ==>        uv
vv                  uv

* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
void SAL_I420ToNV12(unsigned char *pSrcYuv, unsigned char *pDstYuv, unsigned int width, unsigned int hight)
{
    unsigned int i          = 0;
    unsigned int Ysize      = width * hight;
    unsigned int UVsize     = width * hight / 4;

    if((NULL == pSrcYuv) || (NULL == pDstYuv))
    {
        SAL_INFO("Input NULL!\n");
        return;
    }

    SAL_memCpySafe(pDstYuv, pSrcYuv, width * hight);

    for (i = 0; i < UVsize; i++)
    {
        pDstYuv[Ysize + 2 * i]  = pSrcYuv[Ysize + i];
        pDstYuv[Ysize + 2 * i + 1] = pSrcYuv[Ysize + UVsize + i];
    }
}


/*******************************************************************************
* ������  : SAL_NV12ToI420
* ��  ��  : YUV NV12 ת���� I420��ʽ
* ��  ��  : - pSrcYuv: Y���UV���NV12��ʽ��ԭͼ
*         : - pDstYuv: ������һ���I420��ʽ��Ŀ��ͼ
*         : - Width  : ͼ����
*         : - Hight  :

yyyy                yyyy
uv       ==>        uu
uv                  vv

* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
void SAL_NV12ToI420(unsigned char *pSrcYuv, unsigned char *pDstYuv, unsigned int width, unsigned int hight)
{
    unsigned int i          = 0;
    unsigned int Ysize      = width * hight;
    unsigned int UVsize     = width * hight / 4;

    SAL_memCpySafe(pDstYuv, pSrcYuv, width * hight);

    for (i = 0; i < UVsize; i++)
    {
        pDstYuv[Ysize + i]          = pSrcYuv[Ysize + 2 * i];
        pDstYuv[Ysize + UVsize + i] = pSrcYuv[Ysize + 2 * i + 1];
    }
}

/*******************************************************************************
* ������  : SAL_NV12ToNV21
* ��  ��  : YUV NV12 ת���� NV21��ʽ
* ��  ��  : - pSrcYuv: Y���UV���NV12��ʽ��ԭͼ
*         : - pDstYuv: ������һ���I420��ʽ��Ŀ��ͼ
*         : - Width  : ͼ����
*         : - Hight  :

yyyy                yyyy
uv       ==>        vu
uv                  vu

* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
void SAL_NV12ToNV21(unsigned char *pSrcYuv, unsigned char *pDstYuv, unsigned int width, unsigned int hight)
{
    unsigned int i          = 0;
    unsigned int Ysize      = width * hight;
    unsigned int UVsize     = width * hight / 4;

    SAL_memCpySafe(pDstYuv, pSrcYuv, width * hight);

    for (i = 0; i < UVsize; i++)
    {
        pDstYuv[Ysize + 2 * i]          = pSrcYuv[Ysize + 2 * i + 1];
        pDstYuv[Ysize + 2 * i + 1]      = pSrcYuv[Ysize + 2 * i];
    }
}

/*******************************************************************************
* ������  : SAL_BGRCross2BGRPlane
* ��  ��  : �� BGR ����ͼ��ת����RGB������
* ��  ��  : - bgr_cross: ԭͼ
*         : - bgr_plane: Ŀ��ͼ
*         : - height   : ͼ����
*         : - width    :
* ��  ��  : bgr_plane
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
void SAL_BGRCross2BGRPlane(unsigned char* bgr_cross, unsigned char* bgr_plane, unsigned int height, unsigned int width)
{
    int h = 0;
    int w = 0;
    for (h = 0; h < height; h++)
    {
        for (w = 0; w < width; w++)
        {
            bgr_plane[0 * height * width + h * width + w] = bgr_cross[h * width * 3 + w * 3 + 0]; // b
            bgr_plane[1 * height * width + h * width + w] = bgr_cross[h * width * 3 + w * 3 + 1]; // g
            bgr_plane[2 * height * width + h * width + w] = bgr_cross[h * width * 3 + w * 3 + 2]; // r
        }
    }
}

/*******************************************************************************
* ������  : SAL_BGR2YUV420
* ��  ��  : ��BGR�������ͼ��ת��Ϊ YUV420����ͼ
* ��  ��  : - pDstYUV       : �����YUV420ͼ��
*         : - frame_w       : ͼ����
*         : - frame_h       : 
*         : - pSrcBbgrPlane : �����BGR����ͼƬ
* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
void SAL_BGR2YUV420(unsigned char *pDstYUV, unsigned int frame_w, unsigned int frame_h, unsigned char *pSrcBbgrPlane)
{
    int           i, j;
    unsigned char *y, *u, *v, *r, *g, *b;

    int height2 = frame_h / 2;
    int width2 = frame_w / 2;

    int yuv_height = height2 * 2;
    int yuv_width = width2 * 2;

    for (i = 0; i < height2; ++i)
    {
        for (j = 0; j < width2; ++j)
        {
            // (0, 0)
            y = pDstYUV + (i * 2 + 0) * yuv_width + j * 2 + 0;
            u = pDstYUV + yuv_height * yuv_width + i * width2 + j;
            v = pDstYUV + yuv_height * yuv_width + height2 * width2 + i * width2 + j;
            b = pSrcBbgrPlane + (i * 2 + 0) * frame_w + j * 2 + 0;
            g = b + frame_h * frame_w;
            r = g + frame_h * frame_w;

            VCA_cvt_color_pix(*r, *g, *b, y, u, v, VCA_PIX_RGB_YUV);

            // (0, 1)
            y = pDstYUV + (i * 2 + 0) * yuv_width + j * 2 + 1;
            b = pSrcBbgrPlane + (i * 2 + 0) * frame_w + j * 2 + 1;
            g = b + frame_h * frame_w;
            r = g + frame_h * frame_w;

            CVT_RGB2Y(*r, *g, *b, *y);

            // (1, 0)
            y = pDstYUV + (i * 2 + 1) * yuv_width + j * 2 + 0;
            b = pSrcBbgrPlane + (i * 2 + 1) * frame_w + j * 2 + 0;
            g = b + frame_h * frame_w;
            r = g + frame_h * frame_w;

            CVT_RGB2Y(*r, *g, *b, *y);

            // (1, 1)
            y = pDstYUV + (i * 2 + 1) * yuv_width + j * 2 + 1;
            b = pSrcBbgrPlane + (i * 2 + 1) * frame_w + j * 2 + 1;
            g = b + frame_h * frame_w;
            r = g + frame_h * frame_w;

            CVT_RGB2Y(*r, *g, *b, *y);
        }
    }
}

/*******************************************************************************
* ������  : SAL_NV21ToBGRY
* ��  ��  : NV12ͼ��ת��Ϊ BGRY �ĸ��������Դͼ��
* ��  ��  : - pSrcYuv : NV12 ��YUVͼ��
*         : - w       : ͼ����
*         : - h       :
*         : - pDstBgry: BRGY ���������Ŀ��ͼ��
* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
void SAL_NV21ToBGRY(unsigned char* pSrcYuv, int w, int h, unsigned char* pDstBgry)
{
    int count = w;
    int i = 0;
    int r, g, b;
    unsigned char* y = SAL_NULL;
    unsigned char* uv = SAL_NULL;
    unsigned char _y;
    unsigned char _u;
    unsigned char _v;
    unsigned char* dstB = SAL_NULL;
    unsigned char* dstG = SAL_NULL;
    unsigned char* dstR = SAL_NULL;
    unsigned char* dstY = SAL_NULL;

    if ((SAL_NULL == pSrcYuv) || (SAL_NULL == pDstBgry))
    {
        return;
    }

    dstB = (unsigned char*)pDstBgry;
    dstG = (unsigned char*)(pDstBgry + w*h);
    dstR = (unsigned char*)(pDstBgry + w*h*2);
    dstY = (unsigned char*)(pDstBgry + w*h*3);

    for (i=0; i<h; ++i)
    {
        y       = pSrcYuv + w*i;
        uv      = pSrcYuv + w*h + w*(i/2);
        count   = w;

        while (count > 1)
        {
            _y = y[0];
            _u = uv[0];
            _v = uv[1];

            CVT_YUV2RGB(_y,_u,_v,r,g,b);
            *dstB = b;
            *dstG = g;
            *dstR = r;
            *dstY = _y;

            dstB++;
            dstG++;
            dstR++;
            dstY++;

            y++;
            _y = y[0];

            CVT_YUV2RGB(_y,_u,_v,r,g,b);
            *dstB = b;
            *dstG = g;
            *dstR = r;
            *dstY = _y;

            dstB++;
            dstG++;
            dstR++;
            dstY++;

            y++;
            uv+=2;

            count-=2;
        }
        
        if (count > 0)
        {
            _y = y[0];
            _u = uv[0];
            _v = uv[1];

            CVT_YUV2RGB(_y,_u,_v,r,g,b);
            *dstB = b;
            *dstG = g;
            *dstR = r;
            *dstY = _y;

            dstB++;
            dstG++;
            dstR++;
            dstY++;
        }
    }

}

/*******************************************************************************
* ������  : SAL_I420To16Align
* ��  ��  : YUVI420 16λ���� �洢��ʽΪYYYY U V
* ��  ��  : - Width : ԭʼ��
*         : - Height: ԭʼ��
*         : - pSrc  :
*         : - pDst  :
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
�޸���ʷ       :
 1.��      ��   : 2018��11��12��
   ��      ��   : liuyun10
   �޸�����   : �����ɺ���
*******************************************************************************/
int SAL_I420To16Align(unsigned int *pWidth, unsigned int *pHeight, char *pSrc, char *pDst)
{
    unsigned int width = 0, height = 0;
    char *pYdata = NULL;
    char *pUdata = NULL;
    char *pVdata = NULL;

    char *pSrcTmp = NULL;
    char *pDstTmp = NULL;
    
    unsigned int Flag = 0;
    int  i    = 0;

    if ((NULL == pWidth) || (NULL == pHeight) || (NULL == pSrc) || (NULL == pDst))
    {
        //SAL_ERROR("input param empty\n");
        return SAL_FAIL;
    }
    
    width  = *pWidth;
    height = *pHeight;
    pYdata = pSrc;
    pUdata = pSrc + (width * height);
    pVdata = pUdata + (width * height) / 4;

    /* �ж϶��� */
    if (width % 16 != 0)
    {
        Flag = Flag | 0x1;
        width = width & (~0xF);
    }

    if (height % 16 != 0)
    {
        Flag = Flag | 0x2;
        height = height & (~0xF);
    }

    /* ���ݶ��룬���߲����룬ȥ�������治����Ĳ��֣���������Ҫ����ȥ�� */
    if (0x2 == Flag)
    {
        /* ���߲����� */
        pSrcTmp = pYdata;
        pDstTmp = pDst;
        SAL_memCpySafe(pDstTmp, pSrcTmp, (width * height)); /* Y */

        pSrcTmp = pUdata;
        pDstTmp += width * height;
        SAL_memCpySafe(pDstTmp, pSrcTmp, (width * height)/4); /* U */

        pSrcTmp = pVdata;
        pDstTmp += (width * height)/4;
        SAL_memCpySafe(pDstTmp, pSrcTmp, (width * height)/4); /* V */
    }
    else
    {   
        /* Y */
        pSrcTmp = pYdata;
        pDstTmp = pDst;
        for ( i = 0 ; i < height ; i++ )
        {
            SAL_memCpySafe(pDstTmp, pSrcTmp, width); /* Y */
            pDstTmp += width;
            pSrcTmp += (*pWidth);
        }

        /* UV */
        pSrcTmp = pUdata;
        for ( i = 0 ; i < height ; i++ )
        {

            SAL_memCpySafe(pDstTmp, pSrcTmp, width/2); 
            pDstTmp += width/2;
            pSrcTmp += (*pWidth)/2;
        }
   
    }

    *pWidth  = width;
    *pHeight = height;
    return SAL_SOK;
    
}

/*******************************************************************************
* ������  : SAL_NV21To16Align
* ��  ��  : 
* ��  ��  : - Width : ԭʼ��
*         : - Height: ԭʼ��
*         : - pSrc  :
*         : - pDst  :
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
�޸���ʷ       :
 1.��      ��   : 2018��11��12��
   ��      ��   : liuyun10
   �޸�����   : �����ɺ���
*******************************************************************************/
int SAL_NV21To16Align(unsigned int *pWidth, unsigned int *pHeight, char *pSrc, char *pDst)
{
    unsigned int width = 0, height = 0;
    char *pYdata = NULL;
    char *pUVdata = NULL;

    char *pSrcTmp = NULL;
    char *pDstTmp = NULL;
    
    unsigned int Flag = 0;
    int  i    = 0;

    if ((NULL == pWidth) || (NULL == pHeight) || (NULL == pSrc) || (NULL == pDst))
    {
        //SAL_ERROR("input param empty\n");
        return SAL_FAIL;
    }
    
    width  = *pWidth;
    height = *pHeight;
    pYdata = pSrc;
    pUVdata = pSrc + (width * height);
    

    /* �ж϶��� */
    if (width % 16 != 0)
    {
        Flag = Flag | 0x1;
        width = width & (~0xF);
    }

    if (height % 16 != 0)
    {
        Flag = Flag | 0x2;
        height = height & (~0xF);
    }

    /* ���ݶ��룬���߲����룬ȥ�������治����Ĳ��֣���������Ҫ����ȥ�� */
    if (0x2 == Flag)
    {
        /* ���߲����� */
        pSrcTmp = pYdata;
        pDstTmp = pDst;
        SAL_memCpySafe(pDstTmp, pSrcTmp, (width * height)); /* Y */

        pSrcTmp = pUVdata;
        pDstTmp += width * height;
        SAL_memCpySafe(pDstTmp, pSrcTmp, (width * height)/2); /* UV */

    }
    else
    {   
        /* Y */
        pSrcTmp = pYdata;
        pDstTmp = pDst;
        for ( i = 0 ; i < height ; i++ )
        {
            SAL_memCpySafe(pDstTmp, pSrcTmp, width); /* Y */
            pDstTmp += width;
            pSrcTmp += (*pWidth);
        }

        /* UV */
        pSrcTmp = pUVdata;
        for ( i = 0 ; i < height/2 ; i++ )
        {

            SAL_memCpySafe(pDstTmp, pSrcTmp, width); 
            pDstTmp += width;
            pSrcTmp += (*pWidth);
        }
   
    }

    *pWidth  = width;
    *pHeight = height;
    return SAL_SOK;
}

/*******************************************************************************
* ������  : SAL_BgryToNv12
* ��  ��  : cpu BGRYתNV12
* ��  ��  : - pSrc  : ԭʼ����
*        - pDst  : Ŀ������
*        - width ��ԭʼ��:
*        - height: ԭʼ��
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_BgryToNv12(UINT8 *pSrc, UINT8 *pDst, UINT32 width, UINT32 height)
{
    UINT32 i, j, k = 0;
    UINT32 plane = 0;
    UINT8  B, G, R;
    UINT8 *pBvir = NULL;
    UINT8 *pGvir = NULL;
    UINT8 *pRvir = NULL;
    UINT8 *pUv   = NULL;

    if (NULL == pSrc || NULL == pDst ||
            0 == width || 0 == height ||
            0 != width % 2 || 0 != height % 2)
    {
        SAL_ERROR("Input pSrc: %p, pDst: %p, width: %u, height: %u \n", pSrc, pDst, width, height);
        return SAL_FAIL;
    }

    plane = width * height;

    pBvir = pSrc;
    pGvir = pSrc + plane;
    pRvir = pSrc + plane*2;
    pUv   = pDst  + plane;

    SAL_memCpySafe(pDst, pRvir + plane, plane);

    for (i = 0; i < (height>>1); i++)
    {
        for (j = 0; j < (width>>1); j++)
        {
            B = (pBvir[(i<<1)*width+(j<<1)] + pBvir[((i<<1)+1)*width+(j<<1)] +
                    pBvir[(i<<1)*width+((j<<1)+1)] + pBvir[((i<<1)+1)*width+((j<<1)+1)]) >> 2;
            G = (pGvir[(i<<1)*width+(j<<1)] + pGvir[((i<<1)+1)*width+(j<<1)] +
                    pGvir[(i<<1)*width+((j<<1)+1)] + pGvir[((i<<1)+1)*width+((j<<1)+1)]) >> 2;
            R = (pRvir[(i<<1)*width+(j<<1)] + pRvir[((i<<1)+1)*width+(j<<1)] +
                    pRvir[(i<<1)*width+((j<<1)+1)] + pRvir[((i<<1)+1)*width+((j<<1)+1)]) >> 2;

            pUv[k++] = (UINT8)SAL_max(0, SAL_min((( -38 * R -  74 * G + 112 * B + 128) >> 8) + 128, 255));
            pUv[k++] = (UINT8)SAL_max(0, SAL_min((( 112 * R -  94 * G -  18 * B + 128) >> 8) + 128, 255));

        }
    }

    return SAL_SOK;
}

/*******************************************************************************
* ������  : SAL_BgrorgToNv12
* ��  ��  : cpu BGRתNV12��BGRΪbmp��ȡ��ԭʼ���ݣ� bmp����ɫ���˳���Ǵ����һ�е���һ��
* ��  ��  : - pSrc  : ԭʼ����
*        - pDst  : Ŀ������
*        - width ��ԭʼ��
*        - height: ԭʼ��
*        - bytes : ���ؿ�
*        - align : ÿ��β��������ֽ���
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_BgrorgToNv12(UINT8 *pSrc, UINT8 *pDst, UINT32 width, UINT32 height, UINT32 bytes, UINT32 align)
{
    UINT32 i, j, k = 0;
    UINT32 plane = 0;
    UINT8  b, g, r;
    UINT8 *pUpVir = NULL;
    UINT8 *pDnVir = NULL;
    UINT8 *pUv    = NULL;
    UINT32 w, h;
    UINT32 stride = 0;

    if (NULL == pSrc || NULL == pDst ||
            0 == width || 0 == height ||
            0 != width % 2 || 0 != height % 2 ||
            (3 != bytes && 4 != bytes))
    {
        SAL_ERROR("Input pSrc: %p, pDst: %p, width: %u, height: %u, bytes: %u\n", pSrc, pDst, width, height, bytes);
        return SAL_FAIL;
    }

    w      = width  >> 1;
    h      = height >> 1;
    plane  = width * height;
    pUv    = pDst  + plane;
    stride = width * bytes + align;

    for (i = 0; i < h; i++)
    {
        pUpVir  = pSrc + (height - (i << 1) - 1) * stride;
        pDnVir  = pSrc + (height - (i << 1) - 2) * stride;

        for (j = 0; j < w; j++)
        {
            /* (0, 0) */
            b = *(pUpVir);
            g = *(pUpVir + 1);
            r = *(pUpVir + 2);
            pUpVir = pUpVir + bytes;

            pDst[(i<<1)*width+(j<<1)] = (UINT8)SAL_max(0, SAL_min((( 76 * r + 150 * g + 29 * b + 128) >> 8), 255));
            pUv[k++] = (UINT8)SAL_max(0, SAL_min((( -38 * r -  74 * g + 112 * b + 128) >> 8) + 128, 255));
            pUv[k++] = (UINT8)SAL_max(0, SAL_min((( 112 * r -  94 * g -  18 * b + 128) >> 8) + 128, 255));

            /* (0, 1) */
            b = *(pUpVir);
            g = *(pUpVir + 1);
            r = *(pUpVir + 2);
            pUpVir = pUpVir + bytes;

            pDst[(i<<1)*width+(j<<1) + 1] = (UINT8)SAL_max(0, SAL_min((( 76 * r + 150 * g + 29 * b + 128) >> 8), 255));

            /* (1, 0) */
            b = *(pDnVir);
            g = *(pDnVir + 1);
            r = *(pDnVir + 2);
            pDnVir = pDnVir + bytes;

            pDst[((i<<1)+1)*width+(j<<1)] = (UINT8)SAL_max(0, SAL_min((( 76 * r + 150 * g + 29 * b + 128) >> 8), 255));

            /* (1, 1) */
            b = *(pDnVir);
            g = *(pDnVir + 1);
            r = *(pDnVir + 2);
            pDnVir = pDnVir + bytes;

            pDst[((i<<1)+1)*width+(j<<1) + 1] = (UINT8)SAL_max(0, SAL_min((( 76 * r + 150 * g + 29 * b + 128) >> 8), 255));
        }
    }

    return SAL_SOK;
}

/*******************************************************************************
* ������  : SAL_I420ToAlignNV12
* ��  ��  : YUV I420 ת���� NV12��ʽ�� ����ȡ�ķ�ʽ��
* ��  ��  : - pSrcYuv: I420 ԭͼ
*         : - pDstYuv: ������NV12ͼ
*         : - width  :
*         : - hight  :
*         : - stride : ��������
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
void SAL_I420ToAlignNV12(unsigned char *pSrcYuv, unsigned char *pDstYuv, unsigned int width, unsigned int hight, unsigned int stride, unsigned int hStride)
{
    //USED_TIME_DEFINE
    //USED_TIME_START

    unsigned int i          = 0;
    unsigned int j          = 0;
    unsigned int DstYsize   = stride * hStride;
    unsigned int Ysize      = width * hight;
    unsigned int UVsize     = width * hight / 4;
    unsigned int relen      = stride & 0x3;
    unsigned int strideAlign = stride;

    if (width != stride)
    {
        SAL_INFO("w = %d, h = %d, stride = %d\n", width, hight,stride);
    }

    if(relen > 0)
    {
        strideAlign = stride + (4 - relen);
        DstYsize = strideAlign * hStride;
    }
        
    for (i = 0; i < hight; i++)
    {   
        SAL_memCpySafe(pDstYuv+i*strideAlign, pSrcYuv+i*width, stride);
    }
    
    for (i = 0; i < hight/2; i++)
    {  
        for (j = 0; j < stride/2; j++)
        {   
            pDstYuv[DstYsize + i*strideAlign + 2 * j]     = pSrcYuv[Ysize + i*width/2 + j];
            pDstYuv[DstYsize + i*strideAlign + 2 * j + 1] = pSrcYuv[Ysize + UVsize + i*width/2 + j];
        }       
    }

    //USED_TIME_END
}

