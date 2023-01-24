/*******************************************************************************
* ��Ȩ��Ϣ����Ȩ���� (c) 2010-2040, ���ݺ����������ּ����ɷ����޹�˾, ��������Ȩ��
* �ļ����ƣ�color_space.h
* �ļ���ʶ��COLOR_SPACE_H
* ժ    Ҫ����ɫ�ռ�ת��
* ��    ����1.0.0
* ��    �ߣ�����,�����
* ��    �ڣ�2012��3��21��
* ��    ע������
********************************************************************************/
#ifndef _COLOR_SPACE_H_
#define _COLOR_SPACE_H_

/* ========================================================================== */
/*                             ͷ�ļ���                                       */
/* ========================================================================== */
#include <sal_macro.h>
#include "sal_type.h"
#include "sal_colorData.h"

/* ========================================================================== */
/*                           ������Ͷ�����                                   */
/* ========================================================================== */
#define CVT_YUV2RGB(y,u,v,r,g,b)  \
    r = SAL_max(0, SAL_min(R_Y[y] + R_V[v], 255)); \
    g = SAL_max(0, SAL_min(R_Y[y] - G_U[u] - G_V[v], 255)); \
    b = SAL_max(0, SAL_min(R_Y[y] + B_U[u], 255));

/*******************************************************************************
* ������  : CVT_RGB2YUV
* ��  ��  : �о�Ժ�ĸ�ʽת��������ʹ�ø�����

ʹ���� ITU-R BT.601 ��ʽ����Ч��ֹ��������,������ʹ���Դ˹�ʽΪ׼
Y  = 16   +  0.257 * R  +  0.504 * G  +  0.098 * B
Cb = 128  -  0.148 * R  -  0.291 * G  +  0.439 * B
Cr = 128  +  0.439 * R  -  0.368 * G  -  0.071 * B

* ��  ��  : R G B
* ��  ��  : Y U V ��ֵ
*******************************************************************************/
#define CVT_RGB2YUV(r,g,b,y,u,v)  \
    y = SAL_max(0, SAL_min(16  + Y_R[r] + Y_G[g] + Y_B[b], 255)); \
    u = SAL_max(0, SAL_min(128 - U_R[r] - U_G[g] + U_B[b], 255)); \
    v = SAL_max(0, SAL_min(128 + V_R[r] - V_G[g] - V_B[b], 255));

#define CVT_RGB2Y(r,g,b,y)  \
    y = SAL_max(0, SAL_min(16  + Y_R[(r)] + Y_G[(g)] + Y_B[(b)], 255));

#define CVT_RGB2UV(r,g,b,y,u,v)  \
    u = SAL_max(0, SAL_min(128 - U_R[r] - U_G[g] + U_B[b], 255)); \
    v = SAL_max(0, SAL_min(128 + V_R[r] - V_G[g] - V_B[b], 255));


/* ========================================================================== */
/*                          ���ݽṹ������                                    */
/* ========================================================================== */
typedef enum _COLOR_CVT_TYPE_
{
    /* ���ؼ���ת�� */
    VCA_PIX_RGB_YUV          = 0x00,
    VCA_PIX_YUV_RGB          = 0x01,
    VCA_PIX_YUV_HSV          = 0x02,
    VCA_PIX_HSV_YUV          = 0x03,

    /* ͼ�񼶱�ת�� */
    VCA_IMG_YUV420_RGB       = 0x10,
    VCA_IMG_YUV420MV_RGB     = 0x11,
    VCA_IMG_YUV_RGB          = 0x12,
    VCA_IMG_RGB_YUV          = 0x13,

    VCA_IMG_YUV420_HSV       = 0x20,
    VCA_IMG_YUV420MV_HSV     = 0x21,
    VCA_IMG_YUV_HSV          = 0x22,
    VCA_IMG_HSV_YUV          = 0x23
}COLOR_CVT_TYPE;

/* ========================================================================== */
/*                          ����������                                        */
/* ========================================================================== */

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
void SAL_NV12Align(unsigned char *pSrcYuv, unsigned char *pDstYuv, unsigned int srcWidth, unsigned int srcHight, unsigned int dstWidth, unsigned int dstHeight);

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
void SAL_I420ToNV12(unsigned char *pSrcYuv, unsigned char *pDstYuv, unsigned int width, unsigned int hight);

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
void SAL_NV12ToI420(unsigned char *pSrcYuv, unsigned char *pDstYuv, unsigned int width, unsigned int hight);

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
void SAL_NV12ToNV21(unsigned char *pSrcYuv, unsigned char *pDstYuv, unsigned int width, unsigned int hight);

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
void SAL_BGRCross2BGRPlane(unsigned char* bgr_cross, unsigned char* bgr_plane, unsigned int height, unsigned int width);

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
void SAL_BGR2YUV420(unsigned char *pDstYUV, unsigned int frame_w, unsigned int frame_h, unsigned char *pSrcBbgrPlane);

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
void SAL_NV21ToBGRY(unsigned char* pSrcYuv, int w, int h, unsigned char* pDstBgry);

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
int SAL_I420To16Align(unsigned int *pWidth, unsigned int *pHeight, char *pSrc, char *pDst);


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
INT32 SAL_BgryToNv12(UINT8 *pSrc, UINT8 *pDst, UINT32 width, UINT32 height);

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
INT32 SAL_BgrorgToNv12(UINT8 *pSrc, UINT8 *pDst, UINT32 width, UINT32 height, UINT32 bytes, UINT32 align);

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
void SAL_I420ToAlignNV12(unsigned char *pSrcYuv, unsigned char *pDstYuv, unsigned int width, unsigned int hight, unsigned int stride, unsigned int hStride);


#endif

