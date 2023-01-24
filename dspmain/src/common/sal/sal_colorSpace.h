/*******************************************************************************
* 版权信息：版权所有 (c) 2010-2040, 杭州海康威视数字技术股份有限公司, 保留所有权利
* 文件名称：color_space.h
* 文件标识：COLOR_SPACE_H
* 摘    要：颜色空间转换
* 版    本：1.0.0
* 作    者：许朝斌,杨彭举
* 日    期：2012年3月21日
* 备    注：创建
********************************************************************************/
#ifndef _COLOR_SPACE_H_
#define _COLOR_SPACE_H_

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <sal_macro.h>
#include "sal_type.h"
#include "sal_colorData.h"

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */
#define CVT_YUV2RGB(y,u,v,r,g,b)  \
    r = SAL_max(0, SAL_min(R_Y[y] + R_V[v], 255)); \
    g = SAL_max(0, SAL_min(R_Y[y] - G_U[u] - G_V[v], 255)); \
    b = SAL_max(0, SAL_min(R_Y[y] + B_U[u], 255));

/*******************************************************************************
* 函数名  : CVT_RGB2YUV
* 描  述  : 研究院的格式转换方法，使用浮点查表

使用了 ITU-R BT.601 公式，有效防止产生负数,工程上使用以此公式为准
Y  = 16   +  0.257 * R  +  0.504 * G  +  0.098 * B
Cb = 128  -  0.148 * R  -  0.291 * G  +  0.439 * B
Cr = 128  +  0.439 * R  -  0.368 * G  -  0.071 * B

* 输  入  : R G B
* 输  出  : Y U V 的值
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
/*                          数据结构定义区                                    */
/* ========================================================================== */
typedef enum _COLOR_CVT_TYPE_
{
    /* 像素级别转换 */
    VCA_PIX_RGB_YUV          = 0x00,
    VCA_PIX_YUV_RGB          = 0x01,
    VCA_PIX_YUV_HSV          = 0x02,
    VCA_PIX_HSV_YUV          = 0x03,

    /* 图像级别转换 */
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
/*                          函数声明区                                        */
/* ========================================================================== */

/*******************************************************************************
* 函数名  : SAL_NV12Align
* 描  述  : NV12格式图片裁剪。
* 输  入  : - pSrcYuv: NV12 原图
*         : - pDstYuv   : 对齐后的NV12图
*         : - srcWidth  : 原图宽
*         : - srcHight  : 原图高
*         : - dstWidth  : 对齐后的图片宽
*         ：- dstHight   : 对齐后的图片高
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
void SAL_NV12Align(unsigned char *pSrcYuv, unsigned char *pDstYuv, unsigned int srcWidth, unsigned int srcHight, unsigned int dstWidth, unsigned int dstHeight);

/*******************************************************************************
* 函数名  : SAL_I420ToNV12
* 描  述  : YUV I420 转换到 NV12格式
* 输  入  : - pSrcYuv: 交积在一起的I420格式，原图
*         : - pDstYuv: Y面和UV面的NV12格式，目标图
*         : - Width  : 图像宽高
*         : - Hight  :

yyyy                yyyy
uu       ==>        uv
vv                  uv

* 输  出  : 无
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
*******************************************************************************/
void SAL_I420ToNV12(unsigned char *pSrcYuv, unsigned char *pDstYuv, unsigned int width, unsigned int hight);

/*******************************************************************************
* 函数名  : SAL_NV12ToI420
* 描  述  : YUV NV12 转换到 I420格式
* 输  入  : - pSrcYuv: Y面和UV面的NV12格式，原图
*         : - pDstYuv: 交积在一起的I420格式，目标图
*         : - Width  : 图像宽高
*         : - Hight  :

yyyy                yyyy
uv       ==>        uu
uv                  vv

* 输  出  : 无
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
*******************************************************************************/
void SAL_NV12ToI420(unsigned char *pSrcYuv, unsigned char *pDstYuv, unsigned int width, unsigned int hight);

/*******************************************************************************
* 函数名  : SAL_NV12ToNV21
* 描  述  : YUV NV12 转换到 NV21格式
* 输  入  : - pSrcYuv: Y面和UV面的NV12格式，原图
*         : - pDstYuv: 交积在一起的I420格式，目标图
*         : - Width  : 图像宽高
*         : - Hight  :

yyyy                yyyy
uv       ==>        vu
uv                  vu

* 输  出  : 无
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
*******************************************************************************/
void SAL_NV12ToNV21(unsigned char *pSrcYuv, unsigned char *pDstYuv, unsigned int width, unsigned int hight);

/*******************************************************************************
* 函数名  : SAL_BGRCross2BGRPlane
* 描  述  : 把 BGR 交积图像转换成RGB三个面
* 输  入  : - bgr_cross: 原图
*         : - bgr_plane: 目标图
*         : - height   : 图像宽高
*         : - width    :
* 输  出  : bgr_plane
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
*******************************************************************************/
void SAL_BGRCross2BGRPlane(unsigned char* bgr_cross, unsigned char* bgr_plane, unsigned int height, unsigned int width);

/*******************************************************************************
* 函数名  : SAL_BGR2YUV420
* 描  述  : 把BGR三个面的图像转换为 YUV420交积图
* 输  入  : - pDstYUV       : 输出的YUV420图像
*         : - frame_w       : 图像宽高
*         : - frame_h       : 
*         : - pSrcBbgrPlane : 输入的BGR分面图片
* 输  出  : 无
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
*******************************************************************************/
void SAL_BGR2YUV420(unsigned char *pDstYUV, unsigned int frame_w, unsigned int frame_h, unsigned char *pSrcBbgrPlane);

/*******************************************************************************
* 函数名  : SAL_NV21ToBGRY
* 描  述  : NV12图像转换为 BGRY 四个独立面的源图像
* 输  入  : - pSrcYuv : NV12 的YUV图像
*         : - w       : 图像宽高
*         : - h       :
*         : - pDstBgry: BRGY 独立面面的目标图像
* 输  出  : 无
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
*******************************************************************************/
void SAL_NV21ToBGRY(unsigned char* pSrcYuv, int w, int h, unsigned char* pDstBgry);

/*******************************************************************************
* 函数名  : SAL_I420To16Align
* 描  述  : YUVI420 16位对齐 存储方式为YYYY U V
* 输  入  : - Width : 原始高
*         : - Height: 原始宽
*         : - pSrc  :
*         : - pDst  :
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
修改历史       :
 1.日      期   : 2018年11月12日
   作      者   : liuyun10
   修改内容   : 新生成函数
*******************************************************************************/
int SAL_I420To16Align(unsigned int *pWidth, unsigned int *pHeight, char *pSrc, char *pDst);


/*******************************************************************************
* 函数名  : SAL_BgryToNv12
* 描  述  : cpu BGRY转NV12
* 输  入  : - pSrc  : 原始数据
*        - pDst  : 目标数据
*        - width ：原始宽:
*        - height: 原始稿
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_BgryToNv12(UINT8 *pSrc, UINT8 *pDst, UINT32 width, UINT32 height);

/*******************************************************************************
* 函数名  : SAL_BgrorgToNv12
* 描  述  : cpu BGR转NV12，BGR为bmp读取的原始数据， bmp的颜色存放顺序是从最后一行到第一行
* 输  入  : - pSrc  : 原始数据
*        - pDst  : 目标数据
*        - width ：原始宽
*        - height: 原始高
*        - bytes : 像素宽
*        - align : 每行尾部补齐的字节数
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_BgrorgToNv12(UINT8 *pSrc, UINT8 *pDst, UINT32 width, UINT32 height, UINT32 bytes, UINT32 align);

/*******************************************************************************
* 函数名  : SAL_I420ToAlignNV12
* 描  述  : YUV I420 转换到 NV12格式， 跨距抽取的方式。
* 输  入  : - pSrcYuv: I420 原图
*         : - pDstYuv: 对齐后的NV12图
*         : - width  :
*         : - hight  :
*         : - stride : 跨距对齐量
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
void SAL_I420ToAlignNV12(unsigned char *pSrcYuv, unsigned char *pDstYuv, unsigned int width, unsigned int hight, unsigned int stride, unsigned int hStride);


#endif

