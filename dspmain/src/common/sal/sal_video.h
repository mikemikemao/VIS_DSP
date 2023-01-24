/*******************************************************************************
 * sal_video.h
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : heshengyuan <heshengyuan@hikvision.com>
 * Version: V1.0.0  2019��1��2�� Create
 *
 * Description : ��Ƶ��Ϣ��֡��Ϣ����Ƶ���ݡ�֡���ݼ�Buf�Ķ��������������ƽ̨��ģ���޹��Ե����ݽṹ��
 * Modification: 
 *******************************************************************************/
#ifndef _SAL_VIDEO_H_
#define _SAL_VIDEO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             ͷ�ļ���                                       */
/* ========================================================================== */
#include <sal_type.h>
#include <sal_trace.h>
#include <sal_macro.h>
#include <sal_memList.h>
#include "dspcommon.h"

/* ========================================================================== */
/*                           ������Ͷ�����                                   */
/* ========================================================================== */
/* ��Ƶ֡Buf��ħ���ַ� SAL VIDEO ����дASIIC��: SVID */
#define SAL_MAGIC_VIDEO_BUF                  (0x53564944)

/* ƽ̨Ӳ��ģ��ID,����Ӳ��ƽ̨������, �ýṹ���Ǵ�system_common.h �ƹ����� */
typedef enum tagSystemModIDEn
{
    SYSTEM_MOD_ID_SYS   = 0,   /* ϵͳģ�� */
    SYSTEM_MOD_ID_CAPT  = 1,   /* �ɼ�ģ�� */
    SYSTEM_MOD_ID_VENC  = 2,   /* ����ģ�� */
    SYSTEM_MOD_ID_JPEG  = 3,   /* ����ģ�� */
    SYSTEM_MOD_ID_VDEC  = 4,   /* ����ģ�� */
    SYSTEM_MOD_ID_DISP  = 5,   /* ��ʾģ�� */
    SYSTEM_MOD_ID_AUDIO = 6,   /* ��Ƶģ�� */
    SYSTEM_MOD_ID_DUP   = 7,   /* DUPģ��  */
    SYSTEM_MOD_ID_NULL  = 8,   /* NULLģ�� */
    SYSTEM_MOD_ID_MD    = 9,   /* MD  ģ�� */
    SYSTEM_MOD_ID_QR    = 10,  /* ��ά��  ģ�� */
    SYSTEM_MOD_ID_DFR   = 11,  /* ����  ģ�� */
    SYSTEM_MOD_ID_FRAME_OUT = 12,  /* ֡�������   */
    SYSTEM_MOD_ID_BITS_OUT  = 13,  /* ����������� */
    SYSTEM_MOD_ID_FRAME_IN  = 14,  /* ֡��������   */
    SYSTEM_MOD_ID_BITS_IN   = 15,  /* ������������ */
    SYSTEM_MOD_ID_EIF       = 16,  /* ˫���ں�ģ�� */
    SYSTEM_MOD_ID_MAX
}SYSTEM_MOD_ID_E;

/* ���ڻ�������ͼ�ĽǶ�: ��Ƶ��ת��������ת�Ƕȣ�֧����չ */
typedef enum
{
    SAL_VIDEO_ANGLE_NONE        = 0u,
    /* ˮƽ����/���Ҿ��� */
    SAL_VIDEO_ANGLE_HFLIP       = 1u,
    /* ��ֱ����/���¾��� */
    SAL_VIDEO_ANGLE_VFLIP       = 2u,    
    /* ���ķ�ת */
    SAL_VIDEO_ANGLE_CENTER      = 3u,
    /* ���ֳ����ĽǶ� */
    SAL_VIDEO_ANGLE_90          = 90u,
    SAL_VIDEO_ANGLE_180         = 180u,
    SAL_VIDEO_ANGLE_270         = 270u
} SAL_VideoAngleType;

/* ÿ������ռ�õ�λ����֧����չ */ 
typedef enum
{
    /* 8Bitsÿ������ */
    SAL_VIDEO_BPP_BITS8,
    /* 12Bitsÿ������ */
    SAL_VIDEO_BPP_BITS12,
    /* 16Bitsÿ������ */
    SAL_VIDEO_BPP_BITS16,
    /* 24Bitsÿ������ */
    SAL_VIDEO_BPP_BITS24,
    /* 32Bitsÿ������ */
    SAL_VIDEO_BPP_BITS32,
    /* ������������ڼ�����У�� */
    SAL_VIDEO_BPP_MAX
} SAL_VideoBitsPerPixel;

typedef enum
{
    /* YUV 422�������ʽ - UYVY��*/
    SAL_VIDEO_DATFMT_UYVY 	 = 0x00,
    /* YUV 422�������ʽ - YUYV��*/
    SAL_VIDEO_DATFMT_YUYV,
    /* YUV 422�������ʽ - YVYU��*/
    SAL_VIDEO_DATFMT_YVYU,
    /* YUV 422�������ʽ - VYUY��*/
    SAL_VIDEO_DATFMT_VYUY,
    /* YUV 422��ƽ�汣���ʽ - Y��һ��ƽ�棬UV����һƽ�棨�����棩��*/
    SAL_VIDEO_DATFMT_YUV422SP_UV,
    /* YUV 422��ƽ�汣���ʽ - Y��һ��ƽ�棬VU����һƽ�棨�����棩��*/
    SAL_VIDEO_DATFMT_YUV422SP_VU,
    /* YUV 422ƽ�汣���ʽ - Y��һ��ƽ��, V��һ��ƽ�棬UҲ��һ��ƽ�档*/
    SAL_VIDEO_DATFMT_YUV422P,
    
    /* YUV 420��ƽ�汣���ʽ��Ҳ��   NV12 ��ʽ  - Y��һ��ƽ�棬UV����һƽ�棨�����棩��*/
    SAL_VIDEO_DATFMT_YUV420SP_UV,
    /* YUV 420��ƽ�汣���ʽ��Ҳ��   NV21 ��ʽ  - Y��һ��ƽ�棬VU����һƽ�棨�����棩��*/
    SAL_VIDEO_DATFMT_YUV420SP_VU,
    /* YUV 420ƽ�汣���ʽ��  Ҳ�� YUV 888��ƽ���ʽ  - Y��һ��ƽ��, U��һ��ƽ�棬VҲ��һ��ƽ�档*/
    SAL_VIDEO_DATFMT_YUV420P,
    /* YUV420P ƽ��洢��ʽ��U��ǰ��V�ں� */
    SAL_VIDEO_DATFMT_YUV420P_I420,
    /* YUV420P ƽ��洢��ʽ��V��ǰ��U�ں� */
    SAL_VIDEO_DATFMT_YUV420P_YV12,
    
    /* BGRY 4��ƽ�汣���ʽ��B��G��R��Y ���ֱ���һ�������� */
    SAL_VIDEO_DATFMT_PLANAR_Y,
    SAL_VIDEO_DATFMT_BGRY    = 0x100 ,

    /* RGB565 16-bit�����ʽ����ƽ�棬����5bits R, 6bits G, 5bits B��*/
    SAL_VIDEO_DATFMT_RGB16_565,
    /* ARGB1555 16-bit�����ʽ����ƽ�棬����1bits Ϊ͸����5bits R, 5bits G, 5bits B��*/
    SAL_VIDEO_DATFMT_ARGB16_1555,
    /* RGBA5551 16-bit�����ʽ����ƽ�棬����5bits R, 5bits G, 5bits B, 1bits Ϊ͸����*/
    SAL_VIDEO_DATFMT_RGBA16_5551,
    /* RGB888 24-bit�����ʽ����ƽ�棬R��G��B��8bits */
    SAL_VIDEO_DATFMT_RGB24_888,
    /* RGB888 32-bit�����ʽ����ƽ�棬R��G��B��8bits��͸����A 8bits */
    SAL_VIDEO_DATFMT_RGBA_8888,
    SAL_VIDEO_DATFMT_ARGB_8888,
    SAL_VIDEO_DATFMT_BGRA_8888,

    /* RAW Bayer�����ʽ����ƽ�� */
    SAL_VIDEO_DATFMT_BAYER_RAW  = 0x200,

    /* BITMAP �ǳ���λͼ��Ϣ */ 
    SAL_VIDEO_DATFMT_BITMAP8,
    SAL_VIDEO_DATFMT_BITMAP16,
    SAL_VIDEO_DATFMT_BITMAP24,
    SAL_VIDEO_DATFMT_BITMAP32,
    
    /* Bit Strema�����ʽ�������Ǳ��������ݡ�*/
    SAL_VIDEO_DATFMT_BIT_STREAM_PNG   = 0x300,
    SAL_VIDEO_DATFMT_BIT_STREAM_MJPEG,
    SAL_VIDEO_DATFMT_BIT_STREAM_MPEG4,
    SAL_VIDEO_DATFMT_BIT_STREAM_H264,
    SAL_VIDEO_DATFMT_BIT_STREAM_H265,

    /* ��Ч�����ݸ�ʽ�������ڳ�ʼ�����ݸ�ʽ����. */
    SAL_VIDEO_DATFMT_INVALID    = 0xF00,
    /* ������������ڼ�����У��*/
    SAL_VIDEO_DATFMT_MAX
} SAL_VideoDataFormat;

typedef enum
{
    BUFFER_TYPE_CAM = 0xf0, /* ����ͷ���� */
    BUFFER_TYPE_NO_SIGNAL,  /* ���ź�ͼƬ */
    BUFFER_TYPE_DISPLAY     /* ��ʾ����       */
} BUFFER_TYPE;

/* Buffer��ģ��ռ��״̬��ÿ��ģ��ֻʹ��һλ */
typedef enum  {
    MODULE_NONE_IN_USE = 0x00,   /* û��ģ����ʹ��          */
    MODULE_DFR_IN_USE  = 0x01,   /* ��������ģ��ռ�� */
    MODULE_VENC_IN_USE = (MODULE_DFR_IN_USE << 1),      /* ��Ƶ����ģ��ռ�� */
    MODULE_JPEG_IN_USE = (MODULE_DFR_IN_USE << 2),      /* �ɼ�ץͼģ��ռ�� */
    MODULE_OSD_IN_USE  = (MODULE_DFR_IN_USE << 3),      /* ˮӡ��ͼģ��ռ�� */
    MODULE_DISP_IN_USE = (MODULE_DFR_IN_USE << 4),      /* ��ʾģ��ռ��     */
    MODULE_MD_IN_USE   = (MODULE_DFR_IN_USE << 5),      /* �ƶ����ģ��ռ�� */
    MODULE_QR_IN_USE   = (MODULE_DFR_IN_USE << 6),      /* ��ά��ģ��ռ��   */ 
    MODULE_EIF_IN_USE  = (MODULE_DFR_IN_USE << 7),      /* ˫���ں�ģ��ռ�� */
} SAL_BufferStatus;

/* ========================================================================== */
/*                          ���ݽṹ������                                    */
/* ========================================================================== */

/* ͼ���ϵ������ */
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

/* ��������ͼ������Ϣ */
typedef struct
{
    UINT32                      width;
    UINT32                      height;
} SAL_VideoSize;

/* ��Ƶ֡������ͼ�񡢿�����С����ͼ�ľ��������ʾ */
typedef struct
{
    /* ��Ƶ�������ƫ�� */
    UINT32                      left;

    /* ��Ƶ�������ƫ�� */
    UINT32                      top;

    /* ��Ƶ����Ŀ�� */
    UINT32                      width;

    /* ��Ƶ����ĸ߶� */
    UINT32                      height;
} SAL_VideoCrop;

/* ��Ƶ֡������ͼ�񡢵Ĺ�һ��λ�ñ�ʶ */
typedef struct
{
    FLOAT32                      x;
    FLOAT32                      y;
    FLOAT32                      w;
    FLOAT32                      h;
} SAL_VideoPos;


typedef struct
{
    /* ��Ƶ֡ʵ�� */
    UINT32                      width;
    /* ��Ƶ֡ʵ�� */
    UINT32                      height;
    /* ˮƽ�����࣬���RGA�ü�ʱʹ��                */
    UINT32                      widthStride;
    /* ��ֱ�����࣬��ߣ�RGA�ü�ʱʹ�� */
    UINT32                      heightStride;
    /* ������Ϣ������Ƶ֡�����ݸ�ʽ���� SAL_VideoDataFormat ��������� */
    UINT32                      dataFormat;
    /* ������Ϣ������Ƶ֡������λ����ɫ��ȣ��� SAL_VideoBitsPerPixel ��������� */
    UINT32                      bitsPerPixel;
    /* ������Ϣ������Ƶ֡���ڻ�������ͼ�ĽǶ���Ϣ���� SAL_VideoAngleType ��������� */
    UINT32                      angle;
    /* ������Ϣ������Ƶ֡���ڻ�������ͼ�ľ���λ�ã��� SAL_VideoAngleType ��������� */
    UINT32                      mirror;

    /* �߼���Ϣ����ͼ��Ϣ */
    SAL_VideoCrop               crop;
    /* �߼���Ϣ����Ƶ����(JPEG�������ͼ������(1-99)����Ƶ�������������µ�����ϵ��) */
    UINT32                      quality;
    /* �߼���Ϣ����Ƶ֡�� */
    UINT32                      fps;
    /* �߼���Ϣ��I֡��� */
    UINT32                      gop;
    /* �߼���Ϣ������ĵȼ� */
    UINT32                      profile;
    /* �߼���Ϣ����Ƶ��ʽ */
    UINT32                      standard;
    /* �߼���Ϣ�����ʿ��Ʒ�ʽ(������/������) */
    UINT32                      bpsType;
    /* �߼���Ϣ����Ƶ���� */
    UINT32                      bps;
    /* �߼���Ϣ����Ƶ��������(h264/h265) �ο� SAL_VideoDataFormat */
    UINT32                      videoType;

    /* �߼���Ϣ��˫���ں�ģ��ü��ڱ���͸�����ϲ����Ϣ���������������¼���ʹ�� */
    SAL_VideoCrop               extCrop;
    /*
     * reserved[0]������ʱʹ�õĵ�ַ���ͣ�SAL_TRUE:�����ַ��SAL_FALSE�������ַ����FD
     * reserved[1]�����������Ƿ�ü���SAL_TRUE:�ü���SAL_FALSE�����ü�
     * reserved[2], �������ݲü�����������Ƿ���sensor����һ��
     */
    UINT32                      reserved[8];
}SAL_VideoFrameParam;

typedef struct
{
    /* У��ħ����������Buffer�Ƿ��ƻ�(��DMA��Խ�������), ֵ�̶�Ϊ: SAL_MAGIC_VIDEO_BUF */
    UINT32                      magicNum;
    /* ������Ϣ����Ƶ�������� */
    SAL_VideoFrameParam         frameParam;

    /* ������Ϣ���ڴ�fd */
    INT32                       poolId;
    /* ������Ϣ���ڴ������ַ */
    PhysAddr                    virAddr[4];
    /* ������Ϣ���ڴ������ַ */
    PhysAddr                    phyAddr[4];
    UINT32                      stride[4];

    /* ������Ϣ����Ƶ֡���ݵ��ܳ� */
    UINT32                      bufLen;
    /* ������Ϣ���ڴ���ܴ�С */
    UINT32                      memTotalLen;

    /* �߼���Ϣ����Ƶ֡��ʱ��� */
    UINT64                      pts;

    /* �߼���Ϣ������ͨ���ţ�����ͬ��ƵԴ */
    UINT32                      chanId;

    /* �߼���Ϣ��ͬһ����ƵԴ�µĸ��ó���������ID */
    UINT32                      streamId;

    /* �߼���Ϣ��ģ������ڲ�ʹ�� */
    UINT32                      interId; 

    /* �߼���Ϣ��֡���,ÿ·���������Լ���֡��š�*/
    UINT32                      frameNum;

    /* �߼���Ϣ����֡��ʹ�õ����ü����������ڲ�ʹ�� */
    UINT32                      refCnt;
    
    /* �߼���Ϣ����Ƶ��������(h264/h265)  */
    UINT32                      videoType;

    /* ��buffer��ŵ�����������, ֵ��Χ��ö�� BUFFER_TYPE                �� */
    UINT32                      bufferType;

    /* ��buffer���ڱ���Щģ��ʹ�ã�����ͬʱ�����ģ��ռ�� */
    SAL_BufferStatus            bufferStatus;

    /* ˽�����ݣ��ɸ�����Ҫ���� */
    Handle                      privateDate;
    /* ���������ݵ�ģ�� */
    SYSTEM_MOD_ID_E             moduleId;

    UINT8                       buffName[LIST_CONT_LEN];
    
    UINT8                       reserved[20];
} SAL_VideoFrameBuf;


/* ʹ�ÿ�����ָ�룬�������ģ��֮�䡢�˼䡢�������佻����Ƶ��Ϣ����Ҫ����ת������ʹ�á�
*  ��Ƶ֡BUFͳһʹ����������г��󴫵ݣ�������⵱ǰʹ�õ�ͼ����Ϣ�����ڴ������
*  ��: SAL_VideoFrameBuf *pFrameBuf = (SAL_VideoFrameBuf *)pVideoFrame��
*/
typedef void * SAL_VideoFrame;

/* ========================================================================== */
/*                          ����������                                        */
/* ========================================================================== */

#ifdef __cplusplus
}
#endif

#endif

