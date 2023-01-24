/*******************************************************************************
* sal_audio.h
*
* HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
*
* Author : yanghzifu <yangzhifu@hikvision.com.cn>
* Version: V1.0.0  2019��1��16�� Create
*
* Description : 
* Modification: 
*******************************************************************************/

#ifndef _SAL_AUDIO_H_
#define _SAL_AUDIO_H_
 
 /* ========================================================================== */
 /*                             ͷ�ļ���                                       */
 /* ========================================================================== */
#include <sal_type.h>
#include <sal_trace.h>
#include <sal_macro.h>
 
/* ========================================================================== */
/*                           ������Ͷ�����                                   */
/* ========================================================================== */

#define SAL_AUDIO_FRAME_LEN_MAX (SAL_AUDIO_SAMPLE_RATE_48000 * SAL_AUDIO_SOUND_MODE_STEREO * SAL_AUDIO_BIT_WIDTH_16 / 8 / SAL_AUDIO_FRAME_RATE_25)
#define AUDIO_PCM_CONFIG_RATE SAL_AUDIO_SAMPLE_RATE_16000
/* ����λ�� */
typedef enum
{
    SAL_AUDIO_BIT_WIDTH_8   = 8,    /* 8bit width */
    SAL_AUDIO_BIT_WIDTH_16  = 16,   /* 16bit width*/
    SAL_AUDIO_BIT_WIDTH_24  = 24,   /* 24bit width*/
    SAL_AUDIO_BIT_WIDTH_BUTT,
}SAL_AduioBitWidth;

/* ��Ƶ������ */
typedef enum
{
    SAL_AUDIO_SAMPLE_RATE_8000   = 8000,    /* 8K samplerate*/
    SAL_AUDIO_SAMPLE_RATE_12000  = 12000,   /* 12K samplerate*/    
    SAL_AUDIO_SAMPLE_RATE_11025  = 11025,   /* 11.025K samplerate*/
    SAL_AUDIO_SAMPLE_RATE_16000  = 16000,   /* 16K samplerate*/
    SAL_AUDIO_SAMPLE_RATE_22050  = 22050,   /* 22.050K samplerate*/
    SAL_AUDIO_SAMPLE_RATE_24000  = 24000,   /* 24K samplerate*/
    SAL_AUDIO_SAMPLE_RATE_32000  = 32000,   /* 32K samplerate*/
    SAL_AUDIO_SAMPLE_RATE_44100  = 44100,   /* 44.1K samplerate*/
    SAL_AUDIO_SAMPLE_RATE_48000  = 48000,   /* 48K samplerate*/
    SAL_AUDIO_SAMPLE_RATE_64000  = 64000,   /* 64K samplerate*/
    SAL_AUDIO_SAMPLE_RATE_96000  = 96000,   /* 96K samplerate*/
    SAL_AUDIO_SAMPLE_RATE_BUTT,
}SAL_AudioSampleRate;

/* ��Ƶ֡�� */
typedef enum
{
    SAL_AUDIO_FRAME_RATE_25 = 25,
    SAL_AUDIO_FRAME_RATE_50 = 50,
    SAL_AUDIO_FRAME_RATE_BUTT,
}SAL_AudioFrameRate;

/* ��Ƶ��˫���� */
typedef enum
{
    SAL_AUDIO_SOUND_MODE_MONO   = 1,/*mono*/
    SAL_AUDIO_SOUND_MODE_STEREO = 2,/*stereo*/
    SAL_AUDIO_SOUND_MODE_FOUR = 4,  /*four*/
    SAL_AUDIO_SOUND_MODE_EIGHT  = 8, /*eight*/
    SAL_AUDIO_SOUND_MODE_BUTT
}SAL_AudioSoundMode;

/* ========================================================================== */
/*                          ���ݽṹ������                                    */
/* ========================================================================== */

/* ��Ƶ֡�Ļ������� */
typedef struct
{
    /* ��Ƶ�Ĳ���λ�� �����ö�� SAL_AduioBitWidth*/
    UINT32                  bitWidth;
    /* ��Ƶ�Ĳ����ʣ�   �����ö�� SAL_AudioSampleRate*/
    UINT32                  sampleRate;
    /* ��Ƶ��֡�ʣ�     �����ö�� SAL_AudioFrameRate */
    UINT32                  frameRate;
    /* ��Ƶ������ģʽ�� �����ö�� SAL_AudioSoundMode */
    UINT32                  soundMode;
    /* ���� */
    UINT32                  volume;
}SAL_AudioFrameParam;


typedef struct
{
    /* ��Ƶ֡�Ļ������� */
    SAL_AudioFrameParam     frameParam;

    /* ��Ƶ�����ڴ���������С������ʹ��ͨ�õ����������ڴ� */
    Ptr                     pBuf;
    /* ��Ƶ֡���ݵ��ܳ� */
    UINT32                  bufLen;
    /* ��Ч���ݳ��ȣ����Ȳ����� bufLen */
    UINT32                  byteUse;

    /* ��Ƶ֡��ʱ��� */
    UINT64                  pts;

    UINT32                  reserved[8];
}SAL_AudioFrameBuf;

typedef struct
{
    SAL_AudioFrameBuf stAudEchoFrmBuf;    /* �����ź� */
    SAL_AudioFrameBuf stAudRefFrmBuf;     /* �ο��ź� */
    SAL_AudioFrameBuf stAudEchoStereoFrmBuf;     /* �ɼ�˫ͨ���ź� */
    SAL_AudioFrameBuf stAudAnaEchoFrmBuf;     /* �ɼ�ģ�����ڻ��������ź� */
}SAL_AudioCapFrameBuf;

/* ========================================================================== */
/*                          ����������                                        */
/* ========================================================================== */
 
 
#endif



