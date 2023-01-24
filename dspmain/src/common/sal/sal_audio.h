/*******************************************************************************
* sal_audio.h
*
* HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
*
* Author : yanghzifu <yangzhifu@hikvision.com.cn>
* Version: V1.0.0  2019年1月16日 Create
*
* Description : 
* Modification: 
*******************************************************************************/

#ifndef _SAL_AUDIO_H_
#define _SAL_AUDIO_H_
 
 /* ========================================================================== */
 /*                             头文件区                                       */
 /* ========================================================================== */
#include <sal_type.h>
#include <sal_trace.h>
#include <sal_macro.h>
 
/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

#define SAL_AUDIO_FRAME_LEN_MAX (SAL_AUDIO_SAMPLE_RATE_48000 * SAL_AUDIO_SOUND_MODE_STEREO * SAL_AUDIO_BIT_WIDTH_16 / 8 / SAL_AUDIO_FRAME_RATE_25)
#define AUDIO_PCM_CONFIG_RATE SAL_AUDIO_SAMPLE_RATE_16000
/* 采样位宽 */
typedef enum
{
    SAL_AUDIO_BIT_WIDTH_8   = 8,    /* 8bit width */
    SAL_AUDIO_BIT_WIDTH_16  = 16,   /* 16bit width*/
    SAL_AUDIO_BIT_WIDTH_24  = 24,   /* 24bit width*/
    SAL_AUDIO_BIT_WIDTH_BUTT,
}SAL_AduioBitWidth;

/* 音频采样率 */
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

/* 音频帧率 */
typedef enum
{
    SAL_AUDIO_FRAME_RATE_25 = 25,
    SAL_AUDIO_FRAME_RATE_50 = 50,
    SAL_AUDIO_FRAME_RATE_BUTT,
}SAL_AudioFrameRate;

/* 音频单双声道 */
typedef enum
{
    SAL_AUDIO_SOUND_MODE_MONO   = 1,/*mono*/
    SAL_AUDIO_SOUND_MODE_STEREO = 2,/*stereo*/
    SAL_AUDIO_SOUND_MODE_FOUR = 4,  /*four*/
    SAL_AUDIO_SOUND_MODE_EIGHT  = 8, /*eight*/
    SAL_AUDIO_SOUND_MODE_BUTT
}SAL_AudioSoundMode;

/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */

/* 音频帧的基础参数 */
typedef struct
{
    /* 音频的采样位宽， 具体见枚举 SAL_AduioBitWidth*/
    UINT32                  bitWidth;
    /* 音频的采样率，   具体见枚举 SAL_AudioSampleRate*/
    UINT32                  sampleRate;
    /* 音频的帧率，     具体见枚举 SAL_AudioFrameRate */
    UINT32                  frameRate;
    /* 音频的声道模式， 具体见枚举 SAL_AudioSoundMode */
    UINT32                  soundMode;
    /* 音量 */
    UINT32                  volume;
}SAL_AudioFrameParam;


typedef struct
{
    /* 音频帧的基础参数 */
    SAL_AudioFrameParam     frameParam;

    /* 音频数据内存少数据量小，都是使用通用的申请虚拟内存 */
    Ptr                     pBuf;
    /* 音频帧数据的总长 */
    UINT32                  bufLen;
    /* 有效数据长度，长度不超过 bufLen */
    UINT32                  byteUse;

    /* 音频帧的时间戳 */
    UINT64                  pts;

    UINT32                  reserved[8];
}SAL_AudioFrameBuf;

typedef struct
{
    SAL_AudioFrameBuf stAudEchoFrmBuf;    /* 回声信号 */
    SAL_AudioFrameBuf stAudRefFrmBuf;     /* 参考信号 */
    SAL_AudioFrameBuf stAudEchoStereoFrmBuf;     /* 采集双通道信号 */
    SAL_AudioFrameBuf stAudAnaEchoFrmBuf;     /* 采集模拟室内机发来的信号 */
}SAL_AudioCapFrameBuf;

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */
 
 
#endif



