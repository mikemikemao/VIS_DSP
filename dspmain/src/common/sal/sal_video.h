/*******************************************************************************
 * sal_video.h
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : heshengyuan <heshengyuan@hikvision.com>
 * Version: V1.0.0  2019年1月2日 Create
 *
 * Description : 视频信息、帧信息、视频数据、帧数据及Buf的定义和描述，方便平台、模块无关性的数据结构。
 * Modification: 
 *******************************************************************************/
#ifndef _SAL_VIDEO_H_
#define _SAL_VIDEO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <sal_type.h>
#include <sal_trace.h>
#include <sal_macro.h>
#include <sal_memList.h>
#include "dspcommon.h"

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */
/* 视频帧Buf的魔数字符 SAL VIDEO 的缩写ASIIC码: SVID */
#define SAL_MAGIC_VIDEO_BUF                  (0x53564944)

/* 平台硬件模块ID,根据硬件平台可增加, 该结构体是从system_common.h 移过来的 */
typedef enum tagSystemModIDEn
{
    SYSTEM_MOD_ID_SYS   = 0,   /* 系统模块 */
    SYSTEM_MOD_ID_CAPT  = 1,   /* 采集模块 */
    SYSTEM_MOD_ID_VENC  = 2,   /* 编码模块 */
    SYSTEM_MOD_ID_JPEG  = 3,   /* 编码模块 */
    SYSTEM_MOD_ID_VDEC  = 4,   /* 解码模块 */
    SYSTEM_MOD_ID_DISP  = 5,   /* 显示模块 */
    SYSTEM_MOD_ID_AUDIO = 6,   /* 音频模块 */
    SYSTEM_MOD_ID_DUP   = 7,   /* DUP模块  */
    SYSTEM_MOD_ID_NULL  = 8,   /* NULL模块 */
    SYSTEM_MOD_ID_MD    = 9,   /* MD  模块 */
    SYSTEM_MOD_ID_QR    = 10,  /* 二维码  模块 */
    SYSTEM_MOD_ID_DFR   = 11,  /* 智能  模块 */
    SYSTEM_MOD_ID_FRAME_OUT = 12,  /* 帧数据输出   */
    SYSTEM_MOD_ID_BITS_OUT  = 13,  /* 码流数据输出 */
    SYSTEM_MOD_ID_FRAME_IN  = 14,  /* 帧数据输入   */
    SYSTEM_MOD_ID_BITS_IN   = 15,  /* 码流数据输入 */
    SYSTEM_MOD_ID_EIF       = 16,  /* 双光融合模块 */
    SYSTEM_MOD_ID_MAX
}SYSTEM_MOD_ID_E;

/* 基于画面正视图的角度: 视频翻转、镜像、旋转角度，支持扩展 */
typedef enum
{
    SAL_VIDEO_ANGLE_NONE        = 0u,
    /* 水平镜像/左右镜像 */
    SAL_VIDEO_ANGLE_HFLIP       = 1u,
    /* 垂直镜像/上下镜像 */
    SAL_VIDEO_ANGLE_VFLIP       = 2u,    
    /* 中心翻转 */
    SAL_VIDEO_ANGLE_CENTER      = 3u,
    /* 几种常见的角度 */
    SAL_VIDEO_ANGLE_90          = 90u,
    SAL_VIDEO_ANGLE_180         = 180u,
    SAL_VIDEO_ANGLE_270         = 270u
} SAL_VideoAngleType;

/* 每个像素占用的位数，支持扩展 */ 
typedef enum
{
    /* 8Bits每个像素 */
    SAL_VIDEO_BPP_BITS8,
    /* 12Bits每个像素 */
    SAL_VIDEO_BPP_BITS12,
    /* 16Bits每个像素 */
    SAL_VIDEO_BPP_BITS16,
    /* 24Bits每个像素 */
    SAL_VIDEO_BPP_BITS24,
    /* 32Bits每个像素 */
    SAL_VIDEO_BPP_BITS32,
    /* 最大数，可用于计数和校验 */
    SAL_VIDEO_BPP_MAX
} SAL_VideoBitsPerPixel;

typedef enum
{
    /* YUV 422交错保存格式 - UYVY。*/
    SAL_VIDEO_DATFMT_UYVY 	 = 0x00,
    /* YUV 422交错保存格式 - YUYV。*/
    SAL_VIDEO_DATFMT_YUYV,
    /* YUV 422交错保存格式 - YVYU。*/
    SAL_VIDEO_DATFMT_YVYU,
    /* YUV 422交错保存格式 - VYUY。*/
    SAL_VIDEO_DATFMT_VYUY,
    /* YUV 422半平面保存格式 - Y在一个平面，UV在另一平面（交错保存）。*/
    SAL_VIDEO_DATFMT_YUV422SP_UV,
    /* YUV 422半平面保存格式 - Y是一个平面，VU是另一平面（交错保存）。*/
    SAL_VIDEO_DATFMT_YUV422SP_VU,
    /* YUV 422平面保存格式 - Y是一个平面, V是一个平面，U也是一个平面。*/
    SAL_VIDEO_DATFMT_YUV422P,
    
    /* YUV 420半平面保存格式，也叫   NV12 格式  - Y在一个平面，UV在另一平面（交错保存）。*/
    SAL_VIDEO_DATFMT_YUV420SP_UV,
    /* YUV 420半平面保存格式，也叫   NV21 格式  - Y是一个平面，VU是另一平面（交错保存）。*/
    SAL_VIDEO_DATFMT_YUV420SP_VU,
    /* YUV 420平面保存格式，  也叫 YUV 888三平面格式  - Y是一个平面, U是一个平面，V也是一个平面。*/
    SAL_VIDEO_DATFMT_YUV420P,
    /* YUV420P 平面存储格式，U在前，V在后 */
    SAL_VIDEO_DATFMT_YUV420P_I420,
    /* YUV420P 平面存储格式，V在前，U在后 */
    SAL_VIDEO_DATFMT_YUV420P_YV12,
    
    /* BGRY 4个平面保存格式，B、G、R、Y 都分别是一个独立面 */
    SAL_VIDEO_DATFMT_PLANAR_Y,
    SAL_VIDEO_DATFMT_BGRY    = 0x100 ,

    /* RGB565 16-bit保存格式，单平面，其中5bits R, 6bits G, 5bits B。*/
    SAL_VIDEO_DATFMT_RGB16_565,
    /* ARGB1555 16-bit保存格式，单平面，其中1bits 为透明，5bits R, 5bits G, 5bits B。*/
    SAL_VIDEO_DATFMT_ARGB16_1555,
    /* RGBA5551 16-bit保存格式，单平面，其中5bits R, 5bits G, 5bits B, 1bits 为透明。*/
    SAL_VIDEO_DATFMT_RGBA16_5551,
    /* RGB888 24-bit保存格式，单平面，R、G、B各8bits */
    SAL_VIDEO_DATFMT_RGB24_888,
    /* RGB888 32-bit保存格式，单平面，R、G、B各8bits，透明度A 8bits */
    SAL_VIDEO_DATFMT_RGBA_8888,
    SAL_VIDEO_DATFMT_ARGB_8888,
    SAL_VIDEO_DATFMT_BGRA_8888,

    /* RAW Bayer保存格式，单平面 */
    SAL_VIDEO_DATFMT_BAYER_RAW  = 0x200,

    /* BITMAP 是常见位图信息 */ 
    SAL_VIDEO_DATFMT_BITMAP8,
    SAL_VIDEO_DATFMT_BITMAP16,
    SAL_VIDEO_DATFMT_BITMAP24,
    SAL_VIDEO_DATFMT_BITMAP32,
    
    /* Bit Strema保存格式，表明是编码后的数据。*/
    SAL_VIDEO_DATFMT_BIT_STREAM_PNG   = 0x300,
    SAL_VIDEO_DATFMT_BIT_STREAM_MJPEG,
    SAL_VIDEO_DATFMT_BIT_STREAM_MPEG4,
    SAL_VIDEO_DATFMT_BIT_STREAM_H264,
    SAL_VIDEO_DATFMT_BIT_STREAM_H265,

    /* 无效的数据格式，可用于初始化数据格式变量. */
    SAL_VIDEO_DATFMT_INVALID    = 0xF00,
    /* 最大数，可用于计数和校验*/
    SAL_VIDEO_DATFMT_MAX
} SAL_VideoDataFormat;

typedef enum
{
    BUFFER_TYPE_CAM = 0xf0, /* 摄像头数据 */
    BUFFER_TYPE_NO_SIGNAL,  /* 无信号图片 */
    BUFFER_TYPE_DISPLAY     /* 显示数据       */
} BUFFER_TYPE;

/* Buffer被模块占用状态，每个模块只使能一位 */
typedef enum  {
    MODULE_NONE_IN_USE = 0x00,   /* 没有模块在使用          */
    MODULE_DFR_IN_USE  = 0x01,   /* 人脸智能模块占用 */
    MODULE_VENC_IN_USE = (MODULE_DFR_IN_USE << 1),      /* 视频编码模块占用 */
    MODULE_JPEG_IN_USE = (MODULE_DFR_IN_USE << 2),      /* 采集抓图模块占用 */
    MODULE_OSD_IN_USE  = (MODULE_DFR_IN_USE << 3),      /* 水印叠图模块占用 */
    MODULE_DISP_IN_USE = (MODULE_DFR_IN_USE << 4),      /* 显示模块占用     */
    MODULE_MD_IN_USE   = (MODULE_DFR_IN_USE << 5),      /* 移动侦测模块占用 */
    MODULE_QR_IN_USE   = (MODULE_DFR_IN_USE << 6),      /* 二维码模块占用   */ 
    MODULE_EIF_IN_USE  = (MODULE_DFR_IN_USE << 7),      /* 双光融合模块占用 */
} SAL_BufferStatus;

/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */

/* 图像上的坐标点 */
typedef struct
{
    UINT32                      x;
    UINT32                      y;
} SAL_VideoPoint;
typedef struct
{
    FLOAT32                     x;
    FLOAT32                     y;
} SAL_VideoPointF;

/* 仅仅描述图像宽高信息 */
typedef struct
{
    UINT32                      width;
    UINT32                      height;
} SAL_VideoSize;

/* 视频帧、单幅图像、开窗大小、抠图的矩形区域表示 */
typedef struct
{
    /* 视频区域的左偏移 */
    UINT32                      left;

    /* 视频区域的上偏移 */
    UINT32                      top;

    /* 视频区域的宽度 */
    UINT32                      width;

    /* 视频区域的高度 */
    UINT32                      height;
} SAL_VideoCrop;

/* 视频帧、单幅图像、的归一化位置标识 */
typedef struct
{
    FLOAT32                      x;
    FLOAT32                      y;
    FLOAT32                      w;
    FLOAT32                      h;
} SAL_VideoPos;


typedef struct
{
    /* 视频帧实宽 */
    UINT32                      width;
    /* 视频帧实高 */
    UINT32                      height;
    /* 水平方向跨距，虚宽，RGA裁剪时使用                */
    UINT32                      widthStride;
    /* 竖直方向跨距，虚高，RGA裁剪时使用 */
    UINT32                      heightStride;
    /* 基础信息，本视频帧的数据格式，见 SAL_VideoDataFormat 定义的类型 */
    UINT32                      dataFormat;
    /* 基础信息，本视频帧的数据位宽、颜色深度，见 SAL_VideoBitsPerPixel 定义的类型 */
    UINT32                      bitsPerPixel;
    /* 基础信息，本视频帧基于画面正视图的角度信息，见 SAL_VideoAngleType 定义的类型 */
    UINT32                      angle;
    /* 基础信息，本视频帧基于画面正视图的镜像位置，见 SAL_VideoAngleType 定义的类型 */
    UINT32                      mirror;

    /* 高级信息，抠图信息 */
    SAL_VideoCrop               crop;
    /* 高级信息，视频质量(JPEG编码代表图像质量(1-99)，视频编码代表变码率下的量化系数) */
    UINT32                      quality;
    /* 高级信息，视频帧率 */
    UINT32                      fps;
    /* 高级信息，I帧间隔 */
    UINT32                      gop;
    /* 高级信息，编码的等级 */
    UINT32                      profile;
    /* 高级信息，视频制式 */
    UINT32                      standard;
    /* 高级信息，码率控制方式(编码率/定码率) */
    UINT32                      bpsType;
    /* 高级信息，视频码率 */
    UINT32                      bps;
    /* 高级信息，视频编码类型(h264/h265) 参考 SAL_VideoDataFormat */
    UINT32                      videoType;

    /* 高级信息，双光融合模块裁剪黑边需透传至上层的信息，供人脸坐标重新计算使用 */
    SAL_VideoCrop               extCrop;
    /*
     * reserved[0]，拷贝时使用的地址类型，SAL_TRUE:虚拟地址，SAL_FALSE：物理地址或者FD
     * reserved[1]，拷贝数据是否裁剪，SAL_TRUE:裁剪，SAL_FALSE：不裁剪
     * reserved[2], 拷贝数据裁剪后横竖方向是否与sensor方向一致
     */
    UINT32                      reserved[8];
}SAL_VideoFrameParam;

typedef struct
{
    /* 校验魔数，检查这块Buffer是否被破坏(如DMA、越界操作等), 值固定为: SAL_MAGIC_VIDEO_BUF */
    UINT32                      magicNum;
    /* 基础信息，视频基础参数 */
    SAL_VideoFrameParam         frameParam;

    /* 基础信息，内存fd */
    INT32                       poolId;
    /* 基础信息，内存虚拟地址 */
    PhysAddr                    virAddr[4];
    /* 基础信息，内存物理地址 */
    PhysAddr                    phyAddr[4];
    UINT32                      stride[4];

    /* 基础信息，视频帧数据的总长 */
    UINT32                      bufLen;
    /* 基础信息，内存的总大小 */
    UINT32                      memTotalLen;

    /* 高级信息，视频帧的时间戳 */
    UINT64                      pts;

    /* 高级信息，物理通道号，区别不同视频源 */
    UINT32                      chanId;

    /* 高级信息，同一个视频源下的复用出来的码流ID */
    UINT32                      streamId;

    /* 高级信息，模块程序内部使用 */
    UINT32                      interId; 

    /* 高级信息，帧序号,每路码流都有自己的帧序号。*/
    UINT32                      frameNum;

    /* 高级信息，该帧被使用的引用计数，建议内部使用 */
    UINT32                      refCnt;
    
    /* 高级信息，视频编码类型(h264/h265)  */
    UINT32                      videoType;

    /* 该buffer存放的数据类类型, 值范围在枚举 BUFFER_TYPE                里 */
    UINT32                      bufferType;

    /* 该buffer正在被哪些模块使用，可以同时被多个模块占用 */
    SAL_BufferStatus            bufferStatus;

    /* 私有数据，可根据需要设置 */
    Handle                      privateDate;
    /* 来拷贝数据的模块 */
    SYSTEM_MOD_ID_E             moduleId;

    UINT8                       buffName[LIST_CONT_LEN];
    
    UINT8                       reserved[20];
} SAL_VideoFrameBuf;


/* 使用空类型指针，方便各个模块之间、核间、处理器间交互视频信息，需要进行转换才能使用。
*  视频帧BUF统一使用这个来进行抽象传递，容易理解当前使用的图像信息参数内存情况。
*  如: SAL_VideoFrameBuf *pFrameBuf = (SAL_VideoFrameBuf *)pVideoFrame。
*/
typedef void * SAL_VideoFrame;

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */

#ifdef __cplusplus
}
#endif

#endif

