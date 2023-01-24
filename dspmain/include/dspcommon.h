/*******************************************************************************
 * dspcommon.h
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : huangshuxin <huangshuxin@hikvision.com>
 * Version: V2.6.0  2020年05月06日 Create
 *
 * Description : DSP模块对外所提供的头文件
 * Modification:
 *******************************************************************************/

#ifndef _DSPCOMMON_H_
#define _DSPCOMMON_H_

#ifdef __cplusplus
extern "C"
{
#endif


/*************************************************************************************
                            宏定义
*************************************************************************************/

/* 本接口头文件的版本号，每次更新结构的时候需要更新该宏的值 */
#define DSP_COMMON_MAIN_VERSION                 (2)             /* 主版本号 */
#define DSP_COMMON_SUB_VERSION                  (7)             /* 子版本号 */
#define DSP_COMMON_REV_VERSION                  (204)           /* 小版本号 */
#define DSP_COMMON_DATE_VERSION                 "2023-01-14"    /* 版本生成日期 */
#define DSP_COMMON_VERSION_INFO                 "debug"         /* 版本类型 */

#define MAX_NO_SIGNAL_PIC_CNT                   (2)             /* 目前最多支持使用两个无视频图像 */

/* 系统中支持的最大的通道数 */
#define MAX_CAPT_CHAN                           (4)             /* 采集通道数 */
#define MAX_VENC_CHAN                           (4)             /* 编码通道数 */
#define MAX_VDEC_CHAN                           (4)             /* 解码通道数 */
#define MAX_VIPC_CHAN                           (4)             /* 转包通道数 */
#define MAX_BROADCAST_CHAN                      (4)             /* 广播通道数 */
#define MAX_DISP_CHAN                           (16)            /* 显示通道数 */
#define MAX_AES_CHAN                            (2)             /* 加密通道数 */

#define TALKBACK_FRM_LEN                        (160)           /* 对讲音频每帧长度 */
#define TALKBACK_FRM_CNT                        (128)           /* 对讲音频缓冲帧个数 */

/* 对讲音频缓冲长度 */
#define TALKBACK_BUF_LEN                        (TALKBACK_FRM_LEN * TALKBACK_FRM_CNT)

/* 定义分通道上传，录像存储缓冲区I帧信息存储最大数目 */
#define REC_IFRM_INFO_NUM                       (200)

#define ERR_NO_VIDEO_INPUT                      (0x40000001)   /* 长时间采集不到视频，重启DSP */

/* 多 APP 和  DSP 之间多进程访问 */
#define DSP_INIT_SHARED     "/home/app/.INIT_SHM"
#define DSP_NET_SHARED      "/home/app/.NET_SHM"

/* 编解码码流缓冲区大小 */
#define DSP_POOL_SIZE      (2 * 1024 * 1024)
/* 本地录像缓冲区大小，支持预录 */
#define DSP_REC_POOL_SIZE   (14 * 1024 * 1024)

/* DSP 最大顶点个数 OCR 绘制区域使用*/
#define DSP_MAX_POLYGON_NUM    (10)

/* DSP 最大识别出的OCR 护照个数*/
#define DSP_MAX_OCR_NUM        (3)

/* DSP 识别出单个VDP 的最大字符行数*/
#define DSP_MAX_OCRLINE_NUM    (30)

/* DSP 识别出的OCR 单行最大字符个数*/
#define DSP_MAX_OCRCHAR_NUM    (128)

#define MAX_DISP_FACE_PIC_CNT                    (10)           /* 最大人脸框图片个数 */
#define MAX_DISP_FACE_LAYER_CNT                  (2)            /* 最大人脸框图层个数 */

/*************************************************************************************
                                     结构体定义
*************************************************************************************/

/****************************************************************************/
/*                              DSP回调处理                                 */
/****************************************************************************/

/*
    码流信息魔术数定义
*/
#define STREAM_ELEMENT_MAGIC                    (0xdb1bd987)

/*
    回调命令字定义 HOST_CMD_NON 从0x80000000 起步，回调命令避开这段值即可，定从0x80000000
*/
typedef enum _STREAM_TYPE_
{
    STREAM_ELEMENT_CAPT_ERR             = 0x90010001,    /*采集数据超时*/  
    STREAM_ELEMENT_CAPT_CB              = 0x90010002,    /* 视频采集NV12数据回调 */

    STREAM_ELEMENT_JPEG_IMG             = 0x90020033,    /* 抓图数据JPEG */
    STREAM_ELEMENT_QRCODE               = 0x90020034,    /* 二维码处理结果*/
    STREAM_ELEMENT_MD_RESULT            = 0x90020035,    /* 移动侦测结果 */
    STREAM_ELEMENT_CD_RESULT            = 0x90020036,    /* 遮挡报警结果 */
    STREAM_ELEMENT_VIDEO_I_PS           = 0x90020037,    /* PS流I帧回调通知 */
    STREAM_ELEMENT_VIDEO_I_RTP          = 0x90020038,    /* RTP流I帧回调通知 */
    STREAM_ELEMENT_VIDEO_I_TRANSPACK    = 0x90020039,    /* PS流远端录像I帧回调通知 */
    STREAM_ELEMENT_OCR_RESULT           = 0x9002003a,    /* OCR 识别结果 */

    STREAM_ELEMENT_AUDIO_REC            = 0x90030001,    /* 留言功能回调 */
    STREAM_ELEMENT_AUDIO_RING_PLAY      = 0x90030002,    /* 音频铃声播报功能回调,此命令作废 */
    STREAM_ELEMENT_AUDIO_PLAY_STATUS    = 0x90030003,    /* 音频播报功能状态回调,包括铃声播报、TTS播报、定时广播播报*/

    STREAM_ELEMENT_DEC_RES_ERROR        = 0x90040001,    /* 视频解码分辨率错误*/    
    STREAM_ELEMENT_DEC_ENC_TYPE_ERR     = 0x90040002,    /* 视频格式不支持，非H264，非MJPEG */
    STREAM_ELEMENT_DEC_CAP_INFO         = 0x90040003,    /* 解码成功后提醒应用可下发抓图命令 */
    STREAM_ELEMENT_DEC_HAL_ERR          = 0x90040004,     /* 底层解码错误，导致底层解码器卡死 */
    STREAM_ELEMENT_DEC_FRAME_RATE_ERR   = 0x90040006,     /* 解码帧率不对，卡顿、丢帧*/
    STREAM_ELEMENT_DEC_UNPACK_ERR       = 0x90040007,     /* 解包缓冲区溢出*/
    STREAM_ELEMENT_DEC_SEQUENCE_ERR     = 0x90040008,     /* rtp包序号不连续*/
    STREAM_ELEMENT_DEC_WRITE_TIMEOUT    = 0x90040009,     /* 数据超时未送到（1s） */

    STREAM_ELEMENT_FR_FFD_IQA           = 0x9007004e,    /* 人脸检测及人脸质量评分 */
    STREAM_ELEMENT_FR_BM                = 0x9007004f,    /* 人脸建模 */

    STREAM_ELEMENT_FR_CP                = 0x90070050,    /* 人脸对比 */
    STREAM_ELEMENT_FR_DEC               = 0x90070051,    /* 活体检测 */
    STREAM_ELEMENT_FR_BM_CAP            = 0x90070052,    /* 视频抓拍人脸建模 */
    STREAM_ELEMENT_FR_FD_JPG            = 0x90070053,    /* JPG人脸检测坐标*/
    STREAM_ELEMENT_FR_PRE_CAP           = 0x90070054,    /* JPG预抓功能当前帧前后各2帧*/
    STREAM_ELEMENT_FR_BMP2JPG           = 0x90070055,    /* BMP数据转换成JPG数据*/
    STREAM_ELEMENT_FR_JPGSCALE          = 0x90070056,    /* 分辨率过大的JPG转化成靠近640*480分辨率的JPG*/
    STREAM_ELEMENT_FR_ENG_ERR           = 0x90070057,    /*引擎初始化失败*/
    STREAM_ELEMENT_FR_JPGOVEROSD        = 0x90070058,    /*JPG叠加OSD*/
    STREAM_ELEMENT_FR_1VN_SIM           = 0x90070059,    /* 比对流程1VN 返回相似度*/
    STREAM_ELEMENT_FR_PIC_FD            = 0x9007005a,    /* 人脸检测获取测温区域人脸框 */

    STREAM_ELEMENT_TTS_CONVERSION       = 0x90080001,   /* TTS转换结果 */
    STREAM_ELEMENT_SPEECH_RECOGNITION   = 0x90080002,   /* 语音识别结果 */
    STREAM_ELEMENT_DVPR_RECOGNITION     = 0x90080003,   /*声纹识别结果*/

    STREAM_ELEMENT_IRIS_IQA             = 0x90090001,   /* 人眼检测及质量（通常虹膜采集时选配使用）*/
    STREAM_ELEMENT_IRIS_BM              = 0x90090002,   /* 下发已有虹膜图片后的返回模型特征*/
    STREAM_ELEMENT_IRIS_1VN_SIM         = 0x90090003,   /* 虹膜本地注册和1VN比对相似度结果*/
    STREAM_ELEMENT_IRIS_CALIBRATE       = 0x90090004,   /* 虹膜标定结果*/
    
    STREAM_ELEMENT_REBOOT               = 0x900a0001,   /* APP收到后需要对设备进行重启 */
}STREAM_TYPE_E;

/*
    日期、时间
*/
typedef struct
{
    short year;                                     /* 年 */
    short month;                                    /* 月 */
    short dayOfWeek;                                /* 0:星期日 -6:星期六 */
    short day;                                      /* 日 */
    short hour;                                     /* 时 */
    short minute;                                   /* 分 */
    short second;                                   /* 秒 */
    short milliSecond;                              /* 毫秒 */
} DATE_TIME;

/*
    回调处理的附加信息
*/
typedef struct _STREAM_ELEMENT_
{
    unsigned int    magic;                          /* 常数，供定位 */
    unsigned int    id;                             /* 数据信息类型 */
    unsigned int    chan;                           /* 通道号 */
    unsigned int    type;                           /* 码流类型,STREAM_TYPE_E */

    unsigned int    standardTime;                   /* TS/PS标准时间,单位为90kHz时钟,取其高32位,即45kHz时钟 */
    DATE_TIME       absTime;                        /* 绝对时间 */
    unsigned int    dataLen;                        /* 除该信息外的帧的长度 */
    unsigned char   reserve[32];                    /* 保留 */
} STREAM_ELEMENT;

/*
    回调函数类型定义
*/
typedef void (*DATACALLBACK)
(
    STREAM_ELEMENT  *pEle,                          /* 回调的附加信息 */
    unsigned char   *buf,                           /* 附加信息中的 type 所对应的结构体的地址 */
    unsigned int     bufLen                         /* 附加信息中的 type 所对应的结构体的大小 大小不对，结构体错误 */
);

/*
    下发数据给DSP，立即处理返回，不通过回调的共享内存空间
*/
typedef struct _PROC_SHARE_DATA_
{
   unsigned char*   appAddr;         /*app 访问地址*/
   unsigned char*   dspAddr;         /*dsp 访问地址*/
   unsigned char    reserve[32];     /* 预留 */
}PROC_SHARE_DATA;

typedef struct _DSP_SHM_COMMON_
{
    unsigned int                bufSize;            /* 共享内存创建长度， 包含头部 */
    unsigned int                bufUsed;            /* 共享内存数据使用长度， 不包含头部 */
    unsigned int                state;              /* 共享内存操作状态 */
    unsigned int                offset;             /* 数据区的偏移 */
    unsigned int                cmd;                /* 存储HOST CMD */
    unsigned int                chn;                /* 存储HOST CMD CHN */
    unsigned int                prmLen;             /* 传递参数的长度(不含数据) */
    unsigned char               reserve[32];        /* 预留 */
} DSP_SHM_COMMON;

/************************************************************************************************/
/*                                          与应用交互定义                                      */
/************************************************************************************************/
/*
    码流封包类型定义
*/
typedef enum
{
    PS_STREAM       = 0x1,                          /* PS格式封包  */
    TS_STREAM       = 0x2,                          /* TS格式封包  */
    RTP_STREAM      = 0x4,                          /* RTP格式封包 */
    HIK_STREAM      = 0x8,                          /* HIK格式封包 */
    ORG_STREAM      = 0x10,                         /* ORG格式封包 */
    MEGA_RTP_STREAM = 0x20,                         /* MEGA RTP格式封包 */
    MEGA_PS_STREAM  = 0x40,                         /* MEGA PS格式封包  */
    SP7_PS_STREAM   = 0x80,                         /* 萤石ps格式封包 */
} STREAM_PACKET_TYPE;

/*
    码流类型
*/
typedef enum
{
    STREAM_VIDEO = 0x1001,                          /* 视频流 */
    STREAM_AUDIO = 0x1002,                          /* 音频流 */
    STREAM_MULTI = 0x1003,                          /* 复合流 */
    STREAM_PRIVT = 0x1004,                          /* 私有流 */
} STREAM_TYPE;

/*
    编码通道的类型，主码流、子码流、第三码流
*/
typedef enum encChnTypeE
{
    ENC_MAIN   = 0,                              
    ENC_SUB,                                   
    ENC_THRD,
    ENC_MAX
}ENC_CHNTYPE;

/*
    视频码流类型
*/
typedef enum videoStreamTypeE
{
    ENCODER_S264      = 0x0,                        /* Standard H264    */
    ENCODER_H264      = 0x1,                        /* HIK 264          */
    ENCODER_MPEG4     = 0x2,                        /* MPEG4            */
    ENCODER_MJPEG     = 0x3,                        /* MJPEG            */
    ENCODER_MPEG2     = 0x4,                        /* MPEG2            */
    ENCODER_S265      = 0x5,                        /* Standard H265    */
    ENCODER_H265      = 0x6,                        /* HIK 265          */
    ENCODER_QUICK     = 0x7,                        /* 浅编码           */
    ENCODER_MF        = 0x8,                        /* 图像倍帧         */
    ENCODER_MFD4      = 0x9,                        /* 下采样图像MV计算 */
    ENCODER_RAW       = 0xA,                        /* 裸流     */
    ENCODER_BUTT                                    /* 未知视频格式 */
}VIDEO_STREAM_TYPE_E;

/*
    音频编码类型 音频压缩算法
*/
typedef enum
{
    WAVE         = 0,
    G711_MU      = 1,                               /* G711的U律编码 */
    G711_A       = 2,                               /* G711的A律编码 */
    G722_1       = 3,                               /* G722     编码 */
    G723         = 4,                               /* G723     编码 */
    MP1L2        = 5,                               /* MP1      编码 */
    MP2L2        = 6,                               /* MP2      编码 */
    G726         = 7,                               /* G726     编码 */
    AAC          = 8,                               /* AAC      编码 */
    AAC_LD       = 9,                               /* AAC_LD   编码 */
    OPUS         = 10,                              /* OPUS     编码 */
    MP3          = 11,                              /* MP3      编码 */
    RAW          = 99,                              /* 裸流    无编码 */
    AUD_ENC_MAX  = 0xffffffff
} AUDIO_ENC_TYPE;

/* 音频采样率 */
typedef enum
{
    DSP_AUDIO_SAMPLE_RATE_8000   = 8000,    /* 8K samplerate*/
    DSP_AUDIO_SAMPLE_RATE_12000  = 12000,   /* 12K samplerate*/    
    DSP_AUDIO_SAMPLE_RATE_11025  = 11025,   /* 11.025K samplerate*/
    DSP_AUDIO_SAMPLE_RATE_16000  = 16000,   /* 16K samplerate*/
    DSP_AUDIO_SAMPLE_RATE_22050  = 22050,   /* 22.050K samplerate*/
    DSP_AUDIO_SAMPLE_RATE_24000  = 24000,   /* 24K samplerate*/
    DSP_AUDIO_SAMPLE_RATE_32000  = 32000,   /* 32K samplerate*/
    DSP_AUDIO_SAMPLE_RATE_44100  = 44100,   /* 44.1K samplerate*/
    DSP_AUDIO_SAMPLE_RATE_48000  = 48000,   /* 48K samplerate*/
    DSP_AUDIO_SAMPLE_RATE_BUTT,
}AUDIO_SAMPLE_RATE;

typedef enum
{
    DVPR_NEED_CONFIRM            = 0x80000,    /* 开启后没有检测到有效的语音输入，超时断开前再说一遍 */
    DVPR_TIME_OUT                = 0x80001,    /* 超时断开*/
    DVPR_REGIST_SINGEL           = 0x80002,    /* 声纹注册单句成功*/
    DVPR_REGIST_SOK              = 0x80003,    /* 声纹注册建模成功*/
    DVPR_REGIST_FAIL             = 0x80004,    /* 声纹注册建模失败*/
    DVPR_RECOGNIZE_SOK           = 0x80005,    /* 声纹识别建模成功*/
    DVPR_RECOGNIZE_FAIL          = 0x80006,    /* 声纹识别建模失败*/
    DVPR_MODEL_COMPARE           = 0x80007,    /* 声纹1v1比对结果*/
    DVPR_IMPORT_DATA_REGIST_SOK  = 0x80008,    /* 导入声纹有效数据进行注册建模成功*/
    DVPR_IMPORT_DATA_REGIST_FAIL = 0x80009,     /* 导入声纹有效数据进行注册建模失败*/
    DVPR_AUDIO_ENERGY            = 0x8000a,     /* 音频能量大小*/
    DVPR_IMPORT_DATA_LEN_ILLEGAL = 0x8000b,     /* 导入声纹有效数据的长度不符合要求*/
    DVPR_IMPORT_DATA_NUM_ILLEGAL = 0x8000c,     /* 导入声纹有效数据的句子数不符合要求*/
    DVPR_RECORD_AUDIO_DATA       = 0x8000d,     /* 声纹录音数据，用于后续进行过声纹测试*/

}AUDIO_DVPR_CMDTYPE;


typedef enum
{
    LASR_NEED_CONFIRM            = 0x70000,   /* 唤醒后还未检测到关键词，超时断开前再说一遍 */
    LASR_TIME_OUT                = 0x70001,   /* 超时断开*/
    LASR_WAKE_UP                 = 0x70002,   /* 语音唤醒 */
    LASR_RECOGNIZE_SOK           = 0x70003,   /* 语音识别成功*/
    LASR_RECOGNIZE_FAIL          = 0x70004    /* 语音识别失败*/

}AUDIO_LASR_CMDTYPE;

/*
    连续语音识别返回结构体
*/
typedef struct _AUDIO_LASR_RESULT_
{
    AUDIO_LASR_CMDTYPE  cmdType;   /*状态命令*/
    int                 intent;    /* 语句意图*/
    int                 slots;     /*主语句槽值*/
    char                subSlots[128]; /*子语句槽值*/
    
    unsigned char       reserve[32];
}AUDIO_LASR_RESULT;

/*声纹识别，设置录制声纹音频数据配置 HOST_CMD_SET_DVPR_RECORD_AUDIO*/
typedef struct tagDvprRecordAudioParam
{
    unsigned int        recordTime;     /*单次录制的音频时长，单位ms，范围【0,16000】40ms对齐。录制到足够时长后就会回调APP数据，不再继续录制*/
    unsigned char       reserve[32];
}DVPR_RECORD_AUDIO_PARAM;

/*声纹识别，数字密码配置 HOST_CMD_SET_DVPR_KEYWORD*/
typedef struct tagDvprKeyWordParam
{
    unsigned char       keyword[8];     /*下发数字密码内容(字符串数字)，ex：5 9 3 6 0 7 4 8 (不要使用1和2，算法建议)*/
    unsigned char       reserve[32];
}DVPR_KEYWORD_PARAM;

/*声纹识别，参数配置 HOST_CMD_SET_DVPR_PRM*/
typedef struct tagDvprRegisterParam
{   
    unsigned int        timeout;      /* 超时断开的时间,单位为秒限制范围[1, 15] */
    unsigned int        registerNum;  /*注册句子数目, 限制范围[1, 5]*/
    unsigned char       reserve[32];
}DVPR_PARAM;

/*
    DVPR_VERSION  算法版本 返回结构体 HOST_CMD_DVPR_GET_VERSION
*/
typedef struct tagDvprVersion
{
    unsigned char   versionInfo[64];                    /// 主版本号 版本日期 ex:V1.2.204 22-08-05
    
    unsigned char   reserve[256];
}DVPR_VERSION;

/*
    声纹模型校验 HOST_CMD_DVPR_MODEL_CHECK
*/
typedef struct tagDvprModelCheckSt
{
    unsigned char  *modelAddr;              /* 待校验的模型buf地址,应用分配内存*/
    unsigned int    modelLen;               /* 待校验的模型buf长度*/
    unsigned int    modelFit;               /* 校验结果 0表示符合 1表示不符合 2表示未处理(入参不正确)*/
	unsigned char   reserve[24];
} DVPR_MODEL_CHECK_ST;
/*
    APP下发音频文件进行过声纹性能测试 HOST_CMD_PASS_DVPR_AUDIO
*/
typedef struct tagDvprPassAudioSt
{    
    int                 idx;            			/*序号*/
    unsigned int        dataType;                   /* 数据类型，0为过声纹注册建模性能测试，1为过声纹认证建模性能测试*/
    unsigned char       keyword[8];                 /* 本次过声纹性能测试功能下发数字密码内容(字符串数字)，仅在过声纹性能测（注册或认证）试时被使用到，ex：5 9 3 6 0 7 4 8 (不要使用1和2，算法建议，数字密码来源是过声纹的音频文件名)*/
    unsigned char      *audioPackAddr;              /* 过声纹音频数据打包文件buf地址,应用分配内存*/
    unsigned int        audioPackLen;               /* 过声纹音频数据打包文件buf长度*/
    unsigned char       reserve[32];
} DVPR_PASS_AUDIO_ST;

/*
    导入声纹识别有效数据进行注册 HOST_CMD_DVPR_DATA_IMPORT_REGISTER
*/
typedef struct tagDvprImportDataRegSt
{    
    int                 idx;            			/*序号*/
    unsigned char      *audioPackAddr;              /* 有效数据打包文件buf地址,应用分配内存*/
    unsigned int        audioPackLen;               /* 有效数据打包文件buf长度*/
	unsigned char       reserve[24];
} DVPR_IMPORT_DATA_REG_ST;

/*
    声纹识别返回结构体
*/
typedef struct _AUDIO_DVPR_RESULT_
{
    int                 idx;            /*序号*/
    AUDIO_DVPR_CMDTYPE  cmdType;        /*状态命令*/
    unsigned char      *modelAddr;      /*声纹模型文件地址*/
    unsigned int        modelLen;       /*声纹模型文件长度*/
    float               similarity;     /*1VN对比时的相似度,限制范围[0, 100]*/ 
    unsigned char      *audioPackAddr;  /*音频打包文件地址*/
    unsigned int        audioPackLen;   /*音频打包文件长度*/
    unsigned int        audioEnergy;    /*音频能量大小，范围[1-100]*/
	unsigned char       reserve[128];
}AUDIO_DVPR_RESULT;

/*
    获取声纹相似度比对阈值
    HOST_CMD_DVPR_GET_SIM_THRESHOLD
*/
typedef struct _DVPR_SIM_THRESHOLD_PRM_
{
    float           simThreshold1v1;                                                  /* 1v1 推荐比对阈值 */
    float           simThreshold1vn;                                                  /* 1vn 推荐比对阈值  ，当前算法暂不支持，默认传0 */
    unsigned char   reserve[128];                                                     /* 备用 */
}DVPR_SIM_THRESHOLD_PRM;

/*
    声纹模型1V1对比 _DVPR_1V1_COMPARE_        HOST_CMD_DVPR_MODEL_COMPARE
*/
typedef struct _DVPR_1V1_COMPARE_
{
    unsigned char  *baseModelAddr;  /* 底库模型文件buf地址,应用分配内存*/
    unsigned int    baseModelLen;   /* 底库模型文件buf长度*/
    unsigned char  *camModelAddr;   /* 待对比模型文件buf地址,应用分配内存*/
    unsigned int    camModelLen;    /* 底库模型文件buf长度*/
    float           similarity;     /* 1V1相似度，限制范围[0, 100]*/
	unsigned char   reserve[32];
} DVPR_1V1_COMPARE;


typedef enum
{
    SPEECH_NEED_CONFIRM            = 0x40000,       /* 唤醒后还未检测到关键词，超时断开前再说一遍 */
    SPEECH_TIME_OUT                = 0x40001,       /* 超时断开*/
    SPEECH_WAKE_UP                 = 0x40002,       /* 语音唤醒 */
    SPEECH_ENTER_MANAGE_INTERFACE  = 0x40003,       /* 打开/开启/进入 管理界面 */   
    SPEECH_EXIT_MANAGE_INTERFACE   = 0x40004,       /* 关闭/关掉/退出 管理界面 */ 
    SPEECH_ENTER_BACKGROUND        = 0x40005,       /* 打开/开启/进入 后台 */
    SPEECH_EXIT_BACKGROUND         = 0x40006,       /* 关闭/关掉/退出 后台 */ 
    SPEECH_ENTER_CALLING_INTERFACE = 0x40007,       /* 打开/开启/进入 呼叫 */  
    SPEECH_EXIT_CALLING_INTERFACE  = 0x40008,       /* 关闭/关掉/退出 呼叫 */ 
    SPEECH_OPEN_QR_CODE            = 0x40009,       /* 打开/开启/进入 二维码 */   
    SPEECH_CLOSE_QR_CODE           = 0x4000A,       /* 关闭/关掉/退出 二维码 */ 
    SPEECH_CALL_ROOM_NUMBER        = 0X4000B,       /* 呼叫/打给/拨打/打电话给 房间 */
    SPEECH_CALL_PROPERTY           = 0x4000C,       /* 呼叫/打给/拨打/打电话给 物业, 物业中心*/
    SPEECH_CALL_MANAGE_CENTER      = 0x4000D,       /* 呼叫/打给/拨打/打电话给 管理中心 */
    SPEECH_INCREASE_NEW_USER       = 0x4000E,       /* 增加/添加 用户 */
    SPEECH_INCREASE_NEW_PERSONNEL  = 0x4000F,       /* 增加/添加 人员 */
    SPEECH_INCREASE_VOLUME         = 0x40010,       /* 音量 增大/加大/升高 */
    SPEECH_DECREASE_VOLUME         = 0x40011,       /* 音量 减少/减小/降低 */
    SPEECH_MONITOR_UNIT_DOOR       = 0x40012,       /* 监视/查看 单元门口，打开 监视/监控 */
    SPEECH_MONITOR_NURSE_STATION   = 0x40013,       /* 呼叫护士 / 呼叫护士站 / 呼叫主机/ 呼叫护士站主机 / 呼叫医生/打给护士/ 打给护士站/打给医生 */
    SPEECH_MONITOR_ANSWER          = 0x40014,       /* 接听/接通/通话 */
    SPEECH_MONITOR_HANG_UP         = 0x40015,       /* 挂断/结束通话 */
    SPEECH_CALL_ELEVATOR           = 0x40016,       /* 呼梯/呼叫电梯 */
    SPEECH_ANNOUNCEMENT_INFO       = 0x40017,       /* 公告信息/物业公告 */
    SPEECH_OPEN_LOCK               = 0x40018,       /* 开锁 */
    SPEECH_CLOSE_UP                = 0x40019,       /* 关闭/退出/再见 */
    SPEECH_OPEN_SOUND              = 0x4001A,       /* 打开声音 */
    SPEECH_CLOSE_SOUND             = 0x4001B,       /* 关闭声音 */
}SPEECH_RECOGNITION_RESULT;

/* 语音处理的模式 */
typedef enum
{
    SPEECH_RECOGNITION_MODE = 0x0,                   /*老 关键词识别模式WUW + KWS，会不断上报识别结果 */
    SPEECH_CAPTURE_MODE     = 0x1,                   /* 采集模式，不会上报识别结果，该模式只用于采集音频优化算法库 */
    SPEECH_ONLY_KWS_MODE    = 0x2,                   /* 只进行关键词识别模式，不需要语音唤醒*/
    SPEECH_ONLY_LASR_MODE   = 0x3,                   /* 只进行连续语音识别模式，不需要语音唤醒*/
    SPEECH_WUW_LASR_MODE    = 0x4                    /*连续语音识别模式WUW+LASR，需要进行语音唤醒*/
    
}SPEECN_RECOG_TYPE;


/* 语音处理的参数 */
typedef struct tagSpeechRecogParam
{
    SPEECN_RECOG_TYPE   type;                       /* 语音识别处理模式，使用 SPEECN_RECOG_TYPE */
    unsigned int        timeout;                    /* 超时断开的时间,单位为秒 */
    unsigned char       serialNum[32];              /* 设备的序列号,采集模式下发参数 */
	unsigned char       recordTime;                 /* 采集模式时用来告知DSP当前的wordId是第几遍录制 */
    unsigned int        wordId;                     /* 关键词ID  ,采集模式下发参数 */
    unsigned char       name[128];                  /* 人员姓名,采集模式下发参数 */
	unsigned int        recogMode;                  /* 识别模式设置: 0唤醒词识别模式，1关键词识别模式，其他:保持现有识别模式*/
    unsigned char       reserve[28];
}SPEECH_RECOGNITION_PARAM;

/* 
    视频相关信息
*/
typedef struct tagVideoParamsSt
{
    VIDEO_STREAM_TYPE_E  videoEncType;              /* 主码流编码格式 */
    VIDEO_STREAM_TYPE_E  videoSubEncType;           /* 子码流编码格式 */
    STREAM_PACKET_TYPE  packetType;                 /* 主码流封装格式 */
    STREAM_PACKET_TYPE  subPacketType;              /* 子码流封装格式 */
    unsigned int        bWaterMark;                 /* 水印 */
}VIDEO_PARAM;

/*
    音频编码相关信息
*/
typedef struct tagAudioParamsSt
{
    AUDIO_ENC_TYPE encType;                         /* 音频压缩算法 */
    unsigned int   samplesPerSecond;                /* 采样率 */
    unsigned int   samplesPerFrame;                 /* 每帧样点数 */
    unsigned int   boardSamplesPerSecond;           /* 板子原始音频采样率 */
    unsigned int   encChans;                        /* 算法通道数，通常为单声道 */
    unsigned int   bitRate;                         /* 输出码率 */
} AUDIO_PARAM;

/*
    语音对讲相关信息
*/
typedef struct tagTalkBackParamsSt
{
    AUDIO_ENC_TYPE talkBackType;                    /* 语音对讲压缩算法 */
    unsigned int   talkBackSamplesPerSecond;        /* 语音对讲采样率 */
    unsigned int   talkBackSamplesPerFrame;         /* 语音对讲每帧样点数 */
    unsigned int   boardOutSamplesPerSecond;        /* 板子原始输出音频采样率 */
    unsigned int   talkBackBitRate;                 /* 语音对讲输出码率 */
    unsigned int   talkBackChans;                   /* 语音对讲算法通道数，通常为单声道 */
    unsigned int   boardOutChans;                   /* 板子输出通道数，davinci为立体声 */
    unsigned int   audOutVolume;
}TALK_BACK_PARAM;


/*
    音频与视频码流编码与打包相关信息
*/
typedef struct tagStreamInfoSt
{
    VIDEO_PARAM      stVideoParam;                              /* 视频编码属性 */
    AUDIO_PARAM      stAudioParam;                              /* 音频编码属性 */
    TALK_BACK_PARAM  stTalkBackParam;                           /* 语音对讲属性 */
} STREAM_INFO_ST;

/*
    语音对讲功能 数据缓冲区信息
*/
typedef struct tagAudioTBBufInfoSt
{
    volatile unsigned int totalFrame;                           /* 缓冲区支持的最大音频帧数 */
    volatile unsigned int frameLen;                             /* 音频的每一帧数据的长度   */
    volatile unsigned int hostWriteIdx;                         /* 应用写数据的帧数的计数   */
    volatile unsigned int dspWriteIdx;                          /* DSP写数据的帧数的计数    */
    volatile unsigned int hostReadIdx;                          /* 应用读数据的帧数的计数   */
    volatile unsigned int dspReadIdx;                           /* DSP读数据的帧数的计数    */
    unsigned char         audTalkbackHost[TALKBACK_BUF_LEN];    /* 应用写入音频数据的缓冲区 */
    unsigned char         audTalkbackDsp[TALKBACK_BUF_LEN];     /* DSP 写入音频数据的缓冲区 */
} AUDIO_TB_BUF_INFO_ST;

/*
    无视频图像的基本信息，图像格式由应用与底层约定，不定义
*/
typedef struct tagCaptNoSignalInfoSt
{
    unsigned int uiNoSignalImgW[MAX_NO_SIGNAL_PIC_CNT];         /* 无视频图像的宽 */
    unsigned int uiNoSignalImgH[MAX_NO_SIGNAL_PIC_CNT];         /* 无视频图像的高 */
    unsigned int uiNoSignalImgSize[MAX_NO_SIGNAL_PIC_CNT];      /* 无视频图像的数据大小 */
    void        *uiNoSignalImgAddr[MAX_NO_SIGNAL_PIC_CNT];      /* 无视频图像的数据起始地址，需要应用分配空间 */
} CAPT_NOSIGNAL_INFO_ST;

/*
    编码网络流的RTP头相关信息
*/
typedef struct
{
    unsigned int payload_type;                                  /* rtp头payload_type    */
    unsigned int seq_num;                                       /* rtp头seq_num    */
    unsigned int time_stamp;                                    /* rtp头time_stamp    */
} RTP_INFO;

/*
    录像码流共享缓冲结构中I帧的具体信息
*/
typedef struct
{
    volatile unsigned int uiAddr;                               /* 该帧码流的起始地址 */
    volatile unsigned int stdTime;                              /* 标准时间 */
    DATE_TIME             dspAbsTime;                           /* 绝对时间 */
    volatile unsigned int len;                                  /* 码流长度 */
} STREAM_IFRAME_INFO;

/*
    录像码流共享缓冲结构中I帧信息
*/
typedef struct
{
    STREAM_IFRAME_INFO    ifInfo[REC_IFRM_INFO_NUM];            /* 记录下的每一个I帧信息 */
    volatile unsigned int rIdx;                                 /* I帧信息读索引 */
    volatile unsigned int wIdx;                                 /* I帧信息写索引 */
} IFRAME_INFO_ARRAY;

/*
    编码网传码流共享缓冲结构
*/
typedef struct
{
    unsigned char       * vAddr;                    /* 内存映射出的虚拟地址 */
    unsigned char       * pAddr;                    /* 物理地址 */
    volatile unsigned int wIdx;                     /* 实时码流的DSP写索引 */
    volatile unsigned int delayIdx;                 /* 延时预览的DSP写指针*/
    volatile unsigned int totalWLen;                /* 历史数据总长，超过 0xff000000 溢出的临界值进行重新统计 */
    volatile unsigned int totalLen;                 /* 内存总长 */
    volatile unsigned int vFrmCounter;              /* 帧计数 */
    volatile unsigned int streamType;               /* 码流类型，视频流、复合流等*/

    /* 下面的成员从NVR-DSP代码中添加移植过来 */
    volatile unsigned int controlLevel;             /* 码流上传控制级别 使用UPLOAD_CONTROL_LEVEL的定义 */
    volatile unsigned int muxType;                  /* 封装类型*/
    volatile unsigned int extParam;                 /* 扩展参数，用来扩展一些定制功能*/
    volatile RTP_INFO     videoInfo;                /* 存储视频每帧第一个rtp包中的关键信息*/
    volatile RTP_INFO     audioInfo;                /* 存储音频每帧第一个rtp包中的关键信息*/
    volatile RTP_INFO     privtInfo;                /* 存储私有帧每帧第一个rtp包中的关键信息*/
} NET_POOL_INFO;

/*
    编码录像码流共享缓冲结构
*/
typedef struct
{
    unsigned char         *vAddr;                   /* 内存映射出的虚拟地址 */
    unsigned char         *pAddr;                   /* 物理地址 */
    volatile unsigned int  wOffset;                 /* 实时码流的DSP写偏移值  */
    volatile unsigned int  rOffset;                 /* 实时码流的应用读偏移值 */
    volatile unsigned int  totalLen;                /* 缓冲区总大小 */
    IFRAME_INFO_ARRAY      ifInfo;                  /* I帧信息      */
    volatile unsigned int  wErrTime;                /* 写内存错误计数 */
    volatile unsigned int  lastFrameStdTime;        /* 最后一帧数据的标准时间 */
    DATE_TIME              lastFrameAbsTime;        /* 最后一帧数据的绝对时间 */
    volatile unsigned int  vFrmCounter;             /* 帧计数 */
    volatile unsigned int  streamType;              /* 码流类型，视频流、复合流等 */
    volatile unsigned int  controlLevel;            /* 码流上传控制级别 使用UPLOAD_CONTROL_LEVEL的定义 */

    volatile unsigned int muxType;                  /* 封装类型 */
    volatile unsigned int bSubRec;                  /* 是否是码流录像bSubRec =1:子码流录像；bSubRec =0:主码流录像 */
    volatile unsigned int extParam;                 /* 扩展参数，用来扩展一些定制功能 */
    volatile unsigned int streamBps;                /* 记录该通道的码率统计信息 */
    volatile unsigned int curAddrId;                /* 记录当前录像使用的是哪个地址，仅IPC通道有效 */
} REC_POOL_INFO;

/*
    应用下发待解码码流的共享缓冲区
*/
typedef struct DEC_SHARE_BUF_tag
{
    unsigned char         * pPhysAddr;              /* DSP访问地址 */
    unsigned char         * pVirtAddr;              /* ARM访问地址 */
    volatile unsigned int   bufLen;                 /* 解码缓冲区长度 */
    volatile unsigned int   writeIdx;               /* 解码缓冲写索引 */
    volatile unsigned int   readIdx;                /* 解码缓冲读索引 */
    volatile unsigned int   decodeStandardTime;     /* 解码器相对时间 */
    volatile unsigned int   decodeAbsTime;          /* 解码器绝对时间 */
    volatile unsigned int   decodePlayBackStdTime;  /* 解码器倒放相对时间 */
    int                     bIFrameOnly;            /* 是否只送I帧 */
}DEC_SHARE_BUF;

/*
    应用下发待解码JPG
*/
typedef struct DEC_JPG_TRANS_PRM_tag
{
    unsigned int          decodeType;            /* 硬件解码还是软件解码0-硬件解码1-软件解码*/
    unsigned int          transType;             /* 转化成的类型0-YUV-NV12 1-BMP24bit*/
    unsigned char*        virtAddr;              /* JPG图像地址，应用输入*/
    unsigned int          jpegLen;               /* JPG图像长度，应用输入*/
    unsigned char*        addr[4];               /* 输出的图像地址，应用输入*/
    unsigned int          decBufLen;             /* 输出的图像地址长度，应用输入*/
    unsigned int          width;                 /* 解码出来的图像宽度DSP输出*/
    unsigned int          height;                /* 解码出来的图像高度DSP输出*/
    unsigned int          dataLen;               /* 解码转换出来的数据长度，DSP输出*/

    unsigned int          res[16];
}DEC_JPG_TRANS_PRM;

/* 芯片类型枚举 */
typedef enum 
{
    DSP_CHIP_NONE = 0,                              /*无*/
    DSP_CHIP_MA,                                    /*MA系列芯片*/
    DSP_CHIP_MX,                                    /*MX系列芯片*/
    DSP_CHIP_NT,                                    /*NT系列芯片*/    
    DSP_CHIP_NT_F1x,                                /*NT的F1x系列芯片*/
    DSP_CHIP_R2,                                    /*R2芯片*/
    DSP_CHIP_A2S,                                   /*A2S芯片*/
    DSP_CHIP_MAX = 0xffffffff,
}DSP_CHIP_TYPE;

/*
    FR_VERSION  对比 返回结构体
*/
typedef struct tagFrVersion
{
    /* 引擎版本信息 */
    int             mVersion;                       /* 智能库的主版本 */
    int             subVersion;                     /* 智能库的子版本 */
    int             version;                        /* 智能库的修正版本 */
    int             buildData;                      /* 智能库编译时间 */
    int             chipType;                       /* 返回芯片类型 使用DSP_CHIP_TYPE */
    int             isSupportModelN;                /* 底层是否支持建立N库，如果支持APP可以下发人脸N库，底层自动完成输出1VN的相似度， 0 不支持 1 支持 */

    /* 算法版本信息 */
    char            algoName[64];                   /* 算法名称*/
    unsigned int    mainVersion;                    /* 主板本号*/
    unsigned int    minorVersion;                   /* 次板本号*/
    unsigned int    reVersion;                      /* 修正板本号*/
    char            platName[64];                   /* 主芯片名称*/
    char            sysInfo[32];                    /* 匹配的系统名称*/
    char            accurary[32];                   /* 算法精度*/
    char            encryption[32];                 /* 加密方式*/
    char            buildTime[32];                  /* 版本编译日期*/
    char            versionProperties[32];          /* 版本属性*/

    /* 标定库版本信息 */
    int             matchVersion;                   /* 匹配库版本信息*/
    int             matchBuildDate;                 /* 匹配库编译日期*/
    int             matchSvn;                       /* 匹配库SVN版本号*/

    /* 自研活体版本信息 */
    unsigned char   faceLdVersion[32];              /* 活体库版本信息 */
    unsigned char   faceLdBuildData[32];            /* 活体库编译时间 */

    unsigned char   reserve[192];
}FR_VERSION;

/*
    这个结构体是室内机应用用于申请内存进行远程升级用的。室内机由于内存很少，应用无法自己申请内存，需要dsp释放相关内存之后配合申请返回给他们
    HOST_CMD_DSP_HDAL_MEM_ALLOC  接口复用该结构体时pAddr4free变量无用。
*/
typedef struct _MEM_INFO_
{
    unsigned int    allocSize;                      /*需要申请的内存大小, 以M为单位*/    
    unsigned int    pPhyAddr;                       /*对应的物理地址*/
    void*           pVirAddr;                       /*对应的虚拟地址*/
    void*           pAddr4free;                     /*用于释放内存给dsp的地址*/
}MEM_INFO;


/*
    DSP系统模块运行状态
*/
typedef struct tagSysStatusSt
{
    unsigned int dspMainPid;                        /* DSP main 函数的PID号*/
    unsigned int haveCamera;                        /* 是否包含camera*/
    unsigned int useDfr;                            /* 是否使用dfr算法*/
    unsigned int dfrRun;                            /* DFR库是否已经运行起来*/
    FR_VERSION   frVersion;                         /* DFR 版本号*/

    unsigned int dfrLicence;                        /* 授权状况 */
    int          camIdx;                            /* 所用的camera类型 */

    unsigned int cameraError;                       /* 长时间采集不到视频，重启DSP  */
    int          errType;                           /* DSP 上的错误类型通知应用处理 */
    unsigned int useOsd;                            /* OSD 模块的开关 */
}SYS_STATUS;

/*
    DSP采集模块运行状态
*/
typedef struct tagCaptStatusSt
{
    volatile unsigned int bHaveSignal;              /* 是否有视频信号输入,1 表示有，0 表示没有 */
} CAPT_STATUS;

/*
    DSP编码通道运行状态
*/
typedef struct
{
    volatile unsigned int encodeW;                  /* 当前视频帧宽 */
    volatile unsigned int encodeH;                  /* 当前视频帧高 */
    volatile unsigned int encodeSubW;               /* 当前子码流视频帧宽 */
    volatile unsigned int encodeSubH;               /* 当前子码流视频帧高 */
    volatile unsigned int encodedV;                 /* 已编码的视频帧 底层统计 */
    volatile unsigned int encodedSubV;              /* 已编码的子通道视频帧 底层统计 */
    volatile unsigned int encodedA;                 /* 已编码的音频帧 底层统计 */
    volatile unsigned int bpsV;                     /* 视频码流，底层统计 */
    volatile unsigned int bpsA;                     /* 音频码流 底层统计 */
    volatile unsigned int bps;                      /* 码流  底层统计 */
    volatile unsigned int fpsEncV;                  /* 视频编码帧率 底层统计 */
    volatile unsigned int fpsEncA;                  /* 音频编码帧率 底层统计 */
} ENC_STATUS;

/*
    DSP解码通道运行状态
*/
typedef struct
{
    volatile unsigned int status;                   /* 当前状态 */
    volatile unsigned int receivedData;             /* 接收到的数据 */
    volatile unsigned int invlidData;               /* 无效的数据 */
    volatile unsigned int decodedV;                 /* 解码的视频帧 */
    volatile unsigned int decodedA;                 /* 解码的音频帧 */

    volatile unsigned int decodedW;                 /* 当前解码视频帧宽 */
    volatile unsigned int decodedH;                 /* 当前解码视频帧高 */
}DEC_STATUS;

/*
    IPC 通道运行状态
*/
typedef struct
{
    volatile unsigned int status;                   /* 当前状态         */
    volatile unsigned int receivedData;             /* 接收到的数据     */
    volatile unsigned int invlidData;               /* 无效的数据       */
    volatile unsigned int ipcDecV;                  /* IPC 的视频帧     */
    volatile unsigned int ipcDecA;                  /* IPC 的音频帧     */
    volatile unsigned int ipcDecW;                  /* 当前解码视频帧宽 */
    volatile unsigned int ipcDecH;                  /* 当前解码视频帧高 */
} IPC_STATUS;

/*
    DSP显示通道运行状态
*/
typedef struct
{
    volatile unsigned int fps;                      /* 实时显示帧率 */
    volatile unsigned int uiDispW;                  /* 实时显示宽 */
    volatile unsigned int uiDispH;                  /* 实时显示高 */
} DISP_STATUS;


/****************************************************************************/
/*                                 通用                                     */
/****************************************************************************/
/*
    与DSP交互的数据类型
*/
typedef enum dspPicFormatE
{
    DSP_PIC_FORMAT_YUV  = 0x0,                      /*  YUV  数据  */
    DSP_PIC_FORMAT_JPEG,                            /*  JPEG 图片  */
    DSP_PIC_FORMAT_BMP,                             /*  BMP 图片   */
    DSP_PIC_FORMAT_PNG,                             /*  PNG 图片   */
    DSP_PIC_FORMAT_MAX,
}DSP_PIC_FORMAT_E;

/*
    与DSP交互的数据类型
*/
typedef struct dspPicInfoSt
{
    DSP_PIC_FORMAT_E  picFormat;                    /* 数据格式   */
    unsigned int      picWidth;                     /* 图片的宽度 */
    unsigned int      picHeight;                    /* 图片的高度 */
    
    unsigned char   * picAddr;                      /* 图片的地址 */
    unsigned int      picSize;                      /* 图片的大小 */
}DSP_PIC_INFO;


/*  REGION 区域坐标信息 */
typedef struct stRegionInfoSt
{
    /* 参数设置为图像归一化后的百分比 */
    float  x;                                       /* 坐标 x */
    float  y;                                       /* 坐标 y */
    float  w;                                       /* 宽度   */
    float  h;                                       /* 高度   */
}REGION_INFO;

/****************************************************************************/
/*                                 采集                                     */
/****************************************************************************/
/* 视频标准 */
typedef enum VsStandardE
{
    VS_STD_PAL      = 0,                            /* PAL制  */
    VS_STD_NTSC     = 1,                            /* NTSC制 */
    VS_STD_BUTT,
}VS_STANDARD_E;

/* 应用场景和安全级 */
typedef enum 
{
    ld_indoor_low      = 0,                         /* 室内 安全级低 */
    ld_indoor_middle   = 1,                         /* 室内 安全级中 */
    ld_indoor_high     = 2,                         /* 室内 安全级高 */
    ld_outdoor_low     = 3,                         /* 室外 安全级低 */
    ld_outdoor_middle  = 4,                         /* 室外 安全级中 */
    ld_outdoor_high    = 5,                         /* 室外 安全级高 */
    ld_SCENCE_NONE,
}LD_SCENE;

/*
    配置采集通道属性 HOST_CMD_SET_VIDEO_PARM
*/
typedef struct tagVideoChnPrmSt
{
    unsigned int  x;                                /* 输出视频的起点, x     */
    unsigned int  y;                                /* 输出视频的起点, y     */
    VS_STANDARD_E eStandard;                        /* 视频标准,低分辨率区分 */
    unsigned int  eResolution;                      /* 分辨率枚举值          */
    unsigned int  fps;                              /* 输出视频的帧率        */
}VIDEO_CHN_PRM_ST;

/*
    视频旋转
*/
typedef enum RotateE
{
    ROTATE_MODE_NONE  = 0,                          /* 不旋转        */
    ROTATE_MODE_90    = 1,                          /* 顺时针旋转90  */
    ROTATE_MODE_180   = 2,                          /* 顺时针旋转180 */
    ROTATE_MODE_270   = 3,                          /* 顺时针旋转270 */
    ROTATE_MODE_COUNT,                              /* 旋转180度请使用 镜像来实现 */
} ROTATE_MODE;


/*
    配置采集通道旋转属性 HOST_CMD_SET_VIDEO_ROTATE
*/
typedef struct tagVideoRotateAttrSt
{
    unsigned int uiChan;                            /* 指定的通道 ,用于和 DISP_REGION 的各个通道对应 , 用于配置各个预览通道独立的镜像属性*/ 
    ROTATE_MODE eRotate;                            /* 旋转属性 */
}VIDEO_ROTATE_ATTR_ST;


/*
    视频镜像
*/
typedef enum MirrorE
{
    MIRROR_MODE_NONE        = 0,                    /* 不做镜像 */
    MIRROR_MODE_HORIZONTAL  = 1,                    /* 水平镜像/左右镜像 */
    MIRROR_MODE_VERTICAL    = 2,                    /* 垂直镜像/上下镜像 */
    MIRROR_MODE_CENTER      = 3,                    /* 中心镜像/镜像和颠倒/旋转180度  */
    MIRROR_MODE_COUNT,
} MIRROR_MODE;


/*
    配置采集通道镜像属性  HOST_CMD_SET_VIDEO_MIRROR
*/
typedef struct tagVideoMirrorAttrSt
{
    unsigned int uiChan;                            /* 指定的通道 ,用于和 DISP_REGION 的各个通道对应 , 用于配置各个预览通道独立的镜像属性*/ 
    MIRROR_MODE eMirror;                            /* 镜像属性 */
}VIDEO_MIRROR_ATTR_ST;

/*
    摄像头标定参数
*/
typedef struct
{
    double left_cam_K[9];                           /*左相机内参*/
    double left_cam_D[8];                           /*左相机畸变参数*/
    double right_cam_K[9];                          /*右相机内参*/
    double right_cam_D[8];                          /*右相机畸变参数*/
    double extrinsic_R[9];                          /*left_right 相机外参R*/
    double extrinsic_t[3];                          /*left_right 相机外参t*/
    double left_P[9];                               /*左投影矩阵*/
    double right_P[9];                              /*右投影矩阵*/
}CAM_CALIB_PARAM;

/*
   摄像头在通道上的安装参数
*/
typedef struct
{
    double          yaw;                            /* 相机光轴相对于闸机口正前方的水平逆时针偏航角（角度数） */
    double          pitch;                          /* 相机光轴相对于闸机口正前方的垂直逆时针俯仰角（角度数） */
    double          cam2edge;                       /* 左相机光心到闸机口边缘的距离 */
    double          widthChannel;                   /* 闸机通道的宽度 */
    double          criticalWidth;                  /* 临界宽度（处于判断的临界区域） */
    double          reserve[8];                         /* 浮点型预留字段 */
}CAM_INSTALL_PARAM;

// TOF单目内参
typedef struct tagTofCamSvParamSt
{
    float M[3][3];      // 相机内参矩阵
    float D[8];         // 相机畸变参数
} TOf_CAM_SV_PARAMS_ST;

// TOF双目内参
typedef struct tagTofCamBvParamSt
{
    TOf_CAM_SV_PARAMS_ST left;    // 左相机单目内参
    TOf_CAM_SV_PARAMS_ST right;   // 右相机单目内参
    float R[3][3];                // 相机间旋转矩阵
    float T[3];                   // 相机间平移矩阵
    float R_left[3][3];           // 左相机旋转矩阵
    float R_right[3][3];          // 右相机旋转矩阵
    float P_left[3][4];           // 左相机投影矩阵
    float P_right[3][4];          // 右相机投影矩阵
    float Q[4][4];                // 重投影矩阵
} TOf_CAM_BV_PARAMS_ST;

typedef struct tagTofCalibSt
{
    unsigned int flag;              /* 0：仅有RGB；1：仅有IR；2：RGB与IR均存在 */
    TOf_CAM_BV_PARAMS_ST tofvsRGB;  /* tof与RGB标定参数 */
    TOf_CAM_BV_PARAMS_ST tofvsIR;   /* tof与IR 标定参数 */
}TOF_CALIB_ST;

/*
    裁剪属性
*/
typedef struct tagCropInfoSt
{
    unsigned int  uiEnable;                         /* 是否使能裁剪 */
    unsigned int  channel;                          /* 该ROI参数给channel通道使用 */
    unsigned int  uiX;                              /* 起始坐标 x */
    unsigned int  uiY;                              /* 起始坐标 y */
    unsigned int  uiW;                              /* 裁剪宽     */
    unsigned int  uiH;                              /* 裁剪高     */
    unsigned char reserve[32];
}CROP_INFO_ST;

/*
    音频 设备工作 模式
*/
typedef enum eAudioWorkMode
{
    AUDIO_WORK_MODE_FULL_DIGITAL = 0,               /* 全数字设备 */
    AUDIO_WORK_MODE_SEMI_DIGITAL = 1,               /* 半数字设备 */
    AUDIO_WORK_MODE_INVALID                         /* 无效  设备 */
}AUDIO_WORK_MODE;

/*
    音频 CODEC 类型
*/
typedef enum eACodecType
{
    AUDIO_CODEC_TYPE_NONE  = 0,                     /* CODEC 无   */
    AUDIO_CODEC_TYPE_INNER = 1,                     /* CODEC 内部 */
    AUDIO_CODEC_TYPE_EXTER = 2,                     /* CODEC 外部 */
    AUDIO_CODEC_TYPE_INVALID                        /* CODEC 无效 */
}AUDIO_CODEC_TYPE;

/*
消回声处理类型
*/
typedef enum
{
    AEC_TYPE_HARD = 0x0,                            /*硬件消回声  */
    AEC_TYPE_SOFT = 0x1,                            /*软件消回声  */
    AEC_MODE_INVALID                                /* 无效  类型 */
}AEC_TYPE; 

/*
    音频模块初始化信息
*/
typedef struct tagAudioInitInfoSt
{
    unsigned int     aiChn;                         /* ai 通道数        */
    unsigned int     aoChn;                         /* ao 通道数        */
    unsigned int     eWorkMode;                     /* 设备音频功能模式，使用 AUDIO_WORK_MODE 宏*/
    unsigned int     eCodeType;                     /* 音频 CODEC 类型，使用 AUDIO_CODEC_TYPE */
    unsigned int     eAecType;                      /* 消回声类型，使用 AEC_TYPE */
    unsigned char    reserve[64];
}AUDIO_INIT_INFO_ST;

/*
    采集通道属性
*/
typedef struct tagVideoInputInitPrmSt
{
    unsigned int   eViResolution;                   /* 视频输入分辨率            */
    VS_STANDARD_E  eViStandard;                     /* 输入视频制式，区分30/25帧 */
    CROP_INFO_ST   stCropInfo;                      /* 视频采集裁剪属性          */
}VI_INIT_PRM;

/*
    采集模块的信息
*/
typedef struct tagVideoInputInitInfoSt
{
    unsigned int  uiViChn;                          /* vi 通道数        */
    VI_INIT_PRM  stViInitPrm[MAX_CAPT_CHAN];        /* 每个vi通道的信息 */
}VI_INIT_INFO_ST;

/*
    采集模块宽动态的信息 HOST_CMD_SET_VIDEO_WDR
*/
typedef struct tagVideoWDRPrmSt
{
    unsigned int  uiEnable;                         /* 是否使能 */
}VI_WDR_PRM_ST;

/*
    开启采集数据回调的参数描述 HOST_CMD_START_VIDEO_CB
*/
typedef struct tagVideoInputCbInfo
{
    int width;          /* 采集数据回调的宽 */
    int height;         /* 采集数据回调的高 */
    unsigned char  reserve[64]; /* 保留 */
}VI_CB_INFO_ST;

/*
    采集数据回调结构体 STREAM_ELEMENT_CAPT_CB
*/
typedef struct tagVideoInputCbResult
{
    char       * frameAddr;    /* 回调的采集数据的地址 */
    unsigned int frameSize;    /* 回调的采集数据的大小 */
    unsigned char reserve[64]; /* 保留 */
}VI_CB_RESULT_ST;

/*
    鱼眼中心点偏移坐标 CENTER_OFFSET_ST
*/
typedef struct tagCenterOffsetInfoSt
{
    unsigned int  uiEnable;                         /* 是否使能偏移矫正 */
    int  uiCenterX;                                 /* 中心偏移横坐标     */
    int  uiCenterY;                                 /* 中心偏移纵坐标     */
	unsigned char reserve[64]; 						/* 保留 */
}CENTER_OFFSET_ST;

/****************************************************************************/
/*                                 ISP                                      */
/****************************************************************************/
/*
    ISP 透传
    HOST_CMD_SET_ISP_PARAM
    HOST_CMD_GET_ISP_PARAM
*/
typedef struct _ISP_PARAM
{
    unsigned int cmd;
    unsigned int value;
}ISP_PARAM;


/* 补光灯控制 */
typedef struct _LIGHTCTRL_PARAM_
{
    unsigned int ctrlType;                          /* 0 为自动控制亮度,1 为手动控制亮度交替闪烁,2 为手动控制亮度全亮*/
    unsigned int luminance;                         /* 范围 0-100。自动补光调节时: 补光的最大值；手动调节时: 指定的值 */
}LIGHTCTRL_PARAM;

/****************************************************************************/
/*                                 OSD                                     */
/****************************************************************************/
#define  OSD_MAX_LINE                (16)                       /* OSD的最大行数     */
#define  MAX_CHAR_PER_LINE           (960/8)                    /* OSD每行最大字符数 */

#define OSD_MAX_IMG_W                704                        /* 编解码最大图像宽度*/
#define OSD_MAX_IMG_H                576                        /* 编解码最大图像高度*/
#define OSD_TAG_CNT                  13                         /* 特殊字符个数*/
#define OSD_MAX_CHAR_PER_LINE        (OSD_MAX_IMG_W / 8)        /* 4CIF时 全英文字符*/
#define OSD_CHAR_MAX_W               64                         /* 字符最大宽度 */
#define OSD_CHAR_MAX_H               64                         /* 字符最大宽度 */
#define OSD_TAG_CODE_BASE            0x9000                     /* 特殊字符基址*/
#define OSD_YEAR4                    (OSD_TAG_CODE_BASE + 0)    /* 4位年：2002*/
#define OSD_YEAR2                    (OSD_TAG_CODE_BASE + 1)    /* 2位年：02	*/
#define OSD_MONTH3                   (OSD_TAG_CODE_BASE + 2)    /* 3位月份：JAN～DEC*/
#define OSD_MONTH2                   (OSD_TAG_CODE_BASE + 3)    /* 2位月份：1～12*/
#define OSD_DAY                      (OSD_TAG_CODE_BASE + 4)    /* 2位日期：1～31*/
#define OSD_WEEK2                    (OSD_TAG_CODE_BASE + 5)    /* 2位星期(字符宽度为8)*/
#define OSD_WEEK3                    (OSD_TAG_CODE_BASE + 6)    /* 3位星期(字符宽度为8)*/
#define OSD_CWEEK1                   (OSD_TAG_CODE_BASE + 7)    /* 1位星期(字符宽度为16)*/
#define OSD_HOUR24                   (OSD_TAG_CODE_BASE + 8)    /* 2位24时制小时：00～23	*/
#define OSD_HOUR12                   (OSD_TAG_CODE_BASE + 9)    /* 2位12时制小时：00～12*/
#define OSD_MINUTE                   (OSD_TAG_CODE_BASE + 10)   /* 2位分钟00～59*/
#define OSD_SECOND                   (OSD_TAG_CODE_BASE + 11)   /* 2位秒00～59*/
#define OSD_AMPM                     (OSD_TAG_CODE_BASE + 12)   /* AMPM*/

#define FONT8_CODE_BASE              (OSD_TAG_CODE_BASE + OSD_TAG_CNT + 10)                  /*8×16点阵基址*/
#define FONT16_CODE_BASE             (FONT8_CODE_BASE + 88 * OSD_MAX_LINE * 4)               /*16×16点阵基址*/
#define FONT_LIB_SIZE                (OSD_MAX_CHAR_PER_LINE * OSD_MAX_LINE * OSD_CHAR_MAX_H) /* 传输字库的大小，按16行来算为22k大小 */

/*
    OSD功能所使用的字库信息，由应用下发
*/
typedef struct tagFontLibInfoSt
{
    unsigned int fontAscLib[4096 / 4];              /* 目前的大小为 4096 / 4   */
    unsigned int fontHzLib[267616 / 4];             /* 目前的大小为 267616 / 4 */
} FONT_LIB_INFO_ST;

/*
    OSD功能所使用的字库信息V2版本，使用GBK取代HZK，由于更新的GBK内存较大，定义为指针类型，后续由应用在使用时决定是否需要申请内存
*/
typedef struct tagFontLibInfoStV2
{
    unsigned char  *pfontAscLib;            /* ASC字库   */
    unsigned char  *pfontGbkLib;            /* GBK字库 */
	unsigned int    AscLen;                 /* ASC字库长度，由APP读取字库大小后传入 */
    unsigned int    GbkLen;                 /* GBK字库长度，由APP读取字库大小后传入 */
	unsigned char  reserve[56];				/* 备用 */
} FONT_LIB_INFO_ST_V2;

/*
    每一行OSD的信息
*/
typedef struct tagOsdLineParamSt
{
    unsigned int    charCnt;                        /* 当前行字符个数 */
    unsigned int    top;                            /* 高度           */
    unsigned int    hScale;                         /* 水平缩放比例，0 放大一倍，1 放大两倍，2 放大四倍 */
    unsigned int    vScale;                         /* 垂直缩放比例，0 放大一倍，1 放大两倍，2 放大四倍 */

    /* 字符的码值。0～255：ASCII码；
        OSD_TAG_CODE_BASE～OSD_TAG_CODE_BASE+OSD_SPECIAL_CHAR_COUNT－1：特殊字符
        0xa1a1~ ：汉字内码；
        0xa7a1-0xa7c1,0xa7d1-0xa7f1:俄文内码
     */
    unsigned int    code[MAX_CHAR_PER_LINE];
} OSD_LINE_PARAM;

/*
    OSD设置属性
    HOST_CMD_SET_ENC_OSD
    HOST_CMD_SET_DISP_OSD
*/
typedef struct tagOsdConfigSt
{
    unsigned int   bStart;                          /* 是否开启显示   */
    unsigned int   flash;                           /* 闪烁高8位为显示时间，低8位为停止时间，交替显示即可实现闪烁。例：flash＝(2<<8)|1 表示Logo显示2秒，停止1秒*/
    unsigned int   side;                            /* 是否要勾边  */ 
	int            bAutoAdjustLum;                  /* 是否自动调整亮度，自动调整时，指定的亮度值无效 */
    unsigned int   lum;                             /* OSD的亮度值。   */
    int            bTranslucent;                    /* 是否半透明。    */
    OSD_LINE_PARAM line[OSD_MAX_LINE];              /* 每一行OSD的信息 */
} OSD_CONFIG;


#define MAX_LOGO_NUM_PER_CHAN       (4)             /* 每个通道最多含四个LOGO */

/*
    LOGO信息属性
*/
typedef struct tagLogoPrmSt
{
    unsigned int   flash;                           /* 闪烁控制。例：flash＝(2<<8)|1 表示Logo显示2秒，停止1秒 */
    unsigned int   maskY;                           /* 屏蔽色，当LOGO图像中的值和屏蔽色相等时，LOGO透明，显示原图像 */
    unsigned int   maskU;
    unsigned int   maskV;
    unsigned int   bTranslucent;                    /* 是否半透明 */
}LOGO_PARAM;

#define LOGO_MAX_W          256
#define LOGO_MAX_H          128

/*
    LOGO信息属性
    HOST_CMD_SET_ENC_LOGO
*/
typedef struct
{
    unsigned int   idx;                             /* 当前LOGO的序号 最大数为 MAX_LOGO_NUM_PER_CHAN */
    unsigned int   x;
    unsigned int   y;
    unsigned int   w;                               /* 不大于LOGO_MAX_W，且为LOGO_H_ALIGN的整数倍 */
    unsigned int   h;                               /* 不大于LOGO_MAX_H，且为LOGO_V_ALIGN的整数倍 */

    unsigned int   uiIsPic;                         /* 是否使用图片，0:使用色块， 1:使用图片 */

    LOGO_PARAM   param;                             /* 纯色块属性 */
    unsigned int uiPicAddr;                         /* 图片数据内存地址 */
    unsigned int uiPicSize;                         /* 图片数据大小     */
} LOGO_CONFIG;

/*
DSP 坐标点的信息
*/
typedef struct
{
    float   x;
    float   y;
}DSP_COORDINATES;

/*
OCR 每行的信息
*/
typedef struct
{
    char             strings[DSP_MAX_OCRCHAR_NUM*4];                /*最大128字符，留下 汉字等 非8bit的空间*/    
    unsigned int     charNum;                                       /*有效字符个数*/
    DSP_COORDINATES  points[DSP_MAX_POLYGON_NUM];                   /*该行区域顶点坐标*/
    unsigned int     pointsNum;                                     /*有效顶点个数*/
    float            confidence;                                    /*该行的置信度*/
    unsigned int     blob_type;                                     /*业务输出时的类别*/
    char             reserved[28];
}OCR_LINE;

/*
身份证，护照，驾照 信息
*/
typedef struct
{
    OCR_LINE         stOcrLine[DSP_MAX_OCRLINE_NUM];                /*ocr 信息*/
    unsigned int     lineNum;                                       /*ocr 有效信息行*/
    char            *cOcrAddr;                                      /* 回调的OCR数据JPEG图片的地址 */
    unsigned int    uiOcrSize;                                      /* 回调的OCR数据JPEG图片的大小 */ 
    char             reserved[24];
}OCR_CONTENT;

/*
一张图里面识别出的信息个数
*/
typedef struct
{
    OCR_CONTENT      stOcrContent[DSP_MAX_OCR_NUM];
    char             reserved[32];
}OCR_RESULT;

/****************************************************************************/
/*                                 编码                                      */
/****************************************************************************/
/*
    可见光子码流通道数据源配置参数;
    对应主机命令HOST_CMD_SET_SUB_ENCODE_PRIV的ENCODER_PARAM_PRIV
    结构体的switchViSrc变量赋值
*/
typedef enum eSubEncodePrivSrc
{
	ENCODER_SRC_WL			   	= 0, 				/* 可见光图像,						编码后数据存放0_1共享缓存 */
	ENCODER_SRC_IR			   	= 1, 				/* 红外光图像,						编码后数据存放0_1共享缓存 */
	ENCODER_SRC_THER		   	= 2, 				/* 热度图图像,						编码后数据存放1_0共享缓存 */
	ENCODER_SRC_FUSION_WL_THER 	= 3, 				/* 可见光与热度图像融合,				编码后数据存放1_0共享缓存 */
	ENCODER_SRC_TOF_IR		   	= 4, 				/* TOF图像,y scale,					编码后数据存放0_1共享缓存 */
	ENCODER_SRC_TOF_BKIR	   	= 5, 				/* TOF背景图,back ground ir ,scale,	编码后数据存放0_1共享缓存 */
	ENCODER_SRC_TOF_BKIR_INSIDE	= 6, 				/* TOF背景图,back ground ir	,copy,	编码后数据存放0_1共享缓存 */
	ENCODER_SRC_TOF_IR_IN_RECT 	= 7, 				/* TOF图像,y scale  + 画框,			编码后数据存放0_1共享缓存 */
	ENCODER_SRC_TOF_IR_INSIDE  	= 8, 				/* TOF图像,y scale ,copy,			编码后数据存放0_1共享缓存 */
	ENCODER_SRC_FUSION_WL_TOF  	= 9, 				/* 可见光与TOF图像融合,				编码后数据存放0_1共享缓存 */
	ENCODER_SRC_FUSION_IR_TOF  	= 10,				/* 红外光与TOF图像融合,				编码后数据存放0_1共享缓存 */
    ENCODER_SRC_MAX				= 255
}SUB_ENCODE_PRIV_SRC;

/*
	虹膜编码共享buff描述
	DSP在开启虹膜编码时分配内存，关闭时不释放；重复开启时返回对应通道的地址
*/
typedef struct tagIrisShareBuffInfo
{
	unsigned int  netEnable;					/* 网络传输数据流开关 */

	unsigned int  recEnable;					/* 本地存储数据流开关 */

    unsigned int resolution;            		/* 输出分辨率，跟编码使用的宏一致，支持1.HD1080p_FORMAT 2.HD720p_FORMAT 3.SVGA_FORMAT 4.RES_2560_1920_FORMAT */
	
	NET_POOL_INFO irisNetPool;  				/* 网络传输数据流 */

    REC_POOL_INFO irirRecPool;       			/* 本地存储数据流 */

	unsigned char reserve[64];					/* 备用 */
} IRIS_SHARE_BUFF_INFO;

/*
	USB摄像机(非sensor接入)编码共享buff描述
	DSP在开启USB编码时分配内存，关闭时不释放；重复开启时返回对应通道的地址
*/
typedef struct tagUsbShareBuffInfo
{
	unsigned int  netEnable;					/* 网络传输数据流开关 */

	unsigned int  recEnable;					/* 本地存储数据流开关 */

    unsigned int resolution;            		/* 输出分辨率，跟编码使用的宏一致；<是否需要？会上讨论> */

	NET_POOL_INFO usbNetPool;  					/* 网络传输数据流 */

    REC_POOL_INFO usbRecPool;       			/* 本地存储数据流 */

	unsigned char reserve[64];					/* 备用 */
} USB_SHARE_BUFF_INFO;

/*
    编码属性  HOST_CMD_SET_ENCODER_PARAM
*/
typedef struct
{
    unsigned int muxType;                           /* 封装格式，可选TS/PS/RTP/HIK*/
    unsigned int streamType;                        /* 码流类型，目前可选视频流和复合流 */
    unsigned int videoType;                         /* 视频格式，使用 VIDEO_STREAM_TYPE_E */
    unsigned int audioType;                         /* 音频格式，可选G711/G722/MPEG1L2*/

    unsigned int resolution;                        /* 分辨率 */
    unsigned int quant;                             /* 量化系数(1-31),变码率时有效 */
    unsigned int minQuant;                          /* 最小量化系数(1-31) */
    unsigned int maxQuant;                          /* 最大量化系数(1-31) */
    unsigned int bpsType;                           /* 码率控制类型(0:变码率;1:定码率) */
    unsigned int bps;                               /* 码率(定码率时为码率，变码率时为码率上限) */
    unsigned int fps;                               /* 帧率 */
    unsigned int IFrameInterval;                    /* I帧间隔 */
    unsigned int BFrameNum;                         /* B帧个数 */
    unsigned int EFrameNum;                         /* E帧个数 */
    unsigned int b2FieldEncode;                     /* 两场编码 */
    unsigned int sysParam;                          /* 码流参数，是否包含CRC，水印，RSA等，和文件头中的system_param定义相同 */
    unsigned int max_delay;                         /* 码率控制延时时间(1-60),时间越短,瞬时码率越稳定,但图像
                                                                    质量变化快,容易产生马赛克.场景变化大的网传模式下将max_delay
                                                                    设短,保证码率稳定.推荐值为8. */
    unsigned int fast_motion_opt;                   /* 快速运动图像质量优化(场编码有效),0-关闭,1-开启 */
    unsigned int profile;                           /* 编码器profile值(avc/hevc) */
} ENCODER_PARAM;

/* 图片描述信息缓存区的大小 */
#define JPEG_DESCRIPTION_INFO_SIZE (256)

/*
    编码抓图模式类型
*/
typedef enum jpegCaptureModeE
{
    CAP_ONESHOT   = 0,                              /* 单张抓拍，一次命令抓一张 */
    CAP_CONTINUE,                                   /* 连续抓拍 */
    CAP_INVALID
}JPEG_MODE;


/*
    编码抓图属性 HOST_CMD_SET_ENC_JPEG_PARAM
*/
typedef struct tagJpegParamSt
{
    unsigned int idx;                               /* 图片下标方便应用层做同步,室内机抓图应用会传入idx参数做同步用 */
    unsigned int    width;                          /* 大图片宽 */
    unsigned int    height;                         /* 大图片高 */
    unsigned int    quality;                        /* 图片质量 */
    JPEG_MODE       eJpegMode;                      /* 抓图模式 */
    unsigned int    uiCaptCnt;                      /* 抓拍张数，在连续抓拍模式下有效 */
    unsigned int    uiCaptSec;                      /* 在 uiCaptSec 秒内均匀抓拍 uiCaptCnt 张数 */
    OSD_CONFIG      osdParam;                       /* 配置抓图的OSD信息: 给该图片叠加卡号、名字、时间等信息 */
    unsigned int    smallWidth;                     /* 小图片宽 */
    unsigned int    smallHeight;                    /* 小图片高 */
    unsigned char   reserve[248];
}JPEG_PARAM;

/*
    开启编码抓图附加图片描述属性 HOST_CMD_START_ENC_JPEG
*/
typedef struct tagJpegAddInfo
{
    JPEG_PARAM   stJpegPrm;
    unsigned int uiSize;                            /* 图片描述实际大小 */
    char         cInfo[JPEG_DESCRIPTION_INFO_SIZE]; /* 图片描述缓存     */
}JPEG_ADD_INFO_ST;

/* 抓图错误类型回调信息 */
typedef enum
{
    JPEG_SNAP_RESULT_SUCCESS,
    JPEG_SNAP_ERR_PARA_NULL,                /* 抓图结构体参数为空 */
    JPEG_SNAP_ERR_DEC_BIG_IMG_COPY,         /* 解码抓图大图拷贝失败 */
    JPEG_SNAP_ERR_DEC_BIG_IMG_ENC,          /* 解码抓图大图编码失败 */
    JPEG_SNAP_ERR_DEC_SMA_IMG_COPY,         /* 解码抓图小图拷贝失败 */
    JPEG_SNAP_ERR_DEC_SMA_IMG_ENC,          /* 解码抓图小图编码失败 */
    JPEG_SNAP_ERR_CAP_COPY,                 /* 采集抓图拷贝失败 */
    JPEG_SNAP_ERR_CAP_ENC,                  /* 采集抓图编码失败 */

}JPEG_CB_ERROR_TYPE;

/*
    编码抓图返回结构体 STREAM_ELEMENT_JPEG_IMG
*/
typedef struct tagJpegSnapCbResultSt
{
    char           *cJpegAddr;                      /* 回调的抓图数据的地址 */
    unsigned int    uiJpegSize;                     /* 回调的抓图数据的大小 */    
    unsigned int    jpegIdx;                        /* 图片下标方便应用层做同步*/    
    char           *smallJpegAddr;                  /* 小图回调的抓图数据的地址 */
    unsigned int    smallJpegSize;                  /* 小图回调的抓图数据的大小 */
    unsigned int    error_code;                     /* 抓图错误回调信息,详见JPEG_CB_ERROR_TYPE */
    unsigned char   reserve[60];
}JPEG_SNAP_CB_RESULT_ST;

/*
    插入单张图片编码属性 
    HOST_CMD_INSERT_USER_PIC_PARAM 
    HOST_CMD_SUB_INSERT_USER_PIC_PARAM
*/
typedef struct tagVencUserPicPrmSt
{
    unsigned int enable;                            /* 功能使能   */
    DSP_PIC_INFO  picInfo;                          /* 数据格式   */
}VENC_USER_PIC_PRM;

/* 码流信息私有类型: 画框、打包 */
typedef enum
{
    ENCODER_PARAM_PRIV_TYPE_NONE,
    ENCODER_PARAM_PRIV_TYPE_PACK,
    ENCODER_PARAM_PRIV_TYPE_DRAW, 
    ENCODER_PARAM_PRIV_TYPE_POS, 
    ENCODER_PARAM_PRIV_TYPE_SWITCH_VISRC, 
}ENCODER_PARAM_PRIV_TYPE;

/* 编码码流的私有信息配置: 画框的方式 画字符 私有包 编码视频源 等 */
typedef struct
{
    unsigned int            encPrivType;            /* 1封包画框，2按帧画框 详见 ENCODER_PARAM_PRIV_TYPE  */
    unsigned int            drawEnable;             /* 0 不画框  1 画框   */
    unsigned int            packEnable;             /* 0 不打包  1 开   */
    unsigned int            posEnable;              /* 0 不开调试信息  1开   */
    unsigned int            switchViSrc;            /* 切换当前H26X通道编码输入YUV的视频源 */
}ENCODER_PARAM_PRIV;

/*
    区域编码，从采集的图像中裁剪出区域来编码  
    HOST_CMD_SET_ENC_REGION 
*/
typedef struct stEncoderRegionPrmSt
{
    unsigned int enable;                            /* Region 使能开关 */
    REGION_INFO  stRegionInfo;                      /* Region 区域信息 */
}ENCODER_REGION_PRM;

/* 
融合pic图片去编码 
*/
typedef struct
{
    float kx;                                       /*可见光与热成像x方向缩放系数*/
    float ky;                                       /*可见光与热成像y方向缩放系数*/
    float z0;                                       /*初始标定的固定距离  单位 mm */
    float delta_x;                                  /*x方向像素偏差		单位 pixel */
    float delta_y;                                  /*y方向像素偏差		单位 pixel */
    float f1;                                       /*热成像镜头焦距   单位 mm*/
    float dx;                                       /*可见光镜头与热成像镜头在x（水平）方向的距离	 单位mm*/
    float dy;                                       /*可见光镜头与热成像镜头在y（竖直）方向的距离	 单位mm*/
    float dz;                                       /*可见光镜头与热成像镜头在z方向的距离,  单位 mm. 若两个镜头在一个平面，则dz = 0*/
    float pixel;                                    /*热成像镜头像元大小  单位 mm*/

    unsigned int   dwPicSize;                       /* 图片大小 */
    unsigned char *byPicBuf;                        /* 图片数据 */
    unsigned int   calibrationTime;                 /* 标定次数 0:融合标定 1:二次标定 */
    unsigned int   dwRes[7];                        /* 预留 */
}THERMAL_PIC_CORRECT_PARAM; 

/* 在热度图片上叠温度和人脸框*/
typedef struct
{
    unsigned int   x;               /* 人脸框x  */
    unsigned int   y;               /* 人脸框y  */
    unsigned int   w;               /* 人脸框w  */
    unsigned int   h;               /* 人脸框h  */

    unsigned int   tempX;           /* 温度点x  */
    unsigned int   tempY;           /* 温度点y */
    float          temp;            /* 温度  */
    float          distance;        /* 距离*/
    
    unsigned int   uiSize;          /*原始图片长度 */
    unsigned char *uiAddr;          /*原始图片数据 */
    unsigned int   uiOutSize;       /*输出图片长度 */
    unsigned char *uiOutAddr;       /*输出图片数据 */
}ENCODER_PIC_SHOW_TEMP_PARAM;


/****************************************************************************/
/*                                  解码                                    */
/****************************************************************************/

/*
    解码模式
*/
typedef enum decModeE
{
    DECODE_MODE_INVALID        = 0x0,               /* 无效   */
    DECODE_MODE_REALTIME       = 0x1,               /* 实时模式(RTP)     */
    DECODE_MODE_FILE           = 0x2,               /* 文件回放模式(PS)       */
    DECODE_MODE_FILE_REVERSE   = 0x4,               /* 文件倒放模式，解码所有帧    */
    DECODE_MODE_I_FRAME        = 0x8,               /* I帧解码模式，用于高倍速快放 */
    DECODE_MODE_JPEG           = 0x10,              /* 解码JPEG抓图模式            */
    DECODE_MODE_FILE_MULTI     = 0x11,              /* 文件多路回放同步模式        */
    DECODE_BUTT                                     /* 最大的无效值 */
}DEC_MODE_E;

/*
    音频码流类型
*/
typedef enum audioStreamTypeE
{
    STREAM_TYPE_AUDIO_MPEG1     = 0x03,             /* MPEG1格式编码 */
    STREAM_TYPE_AUDIO_MPEG2     = 0x04,             /* MPEG2格式编码 */
    STREAM_TYPE_AUDIO_AAC       = 0x0f,             /* AAC格式编码   */
    STREAM_TYPE_AUDIO_OPUS      = 0x11,             /* OPUS格式编码   */
    STREAM_TYPE_AUDIO_G711A     = 0x90,             /* G711格式A律编码 */
    STREAM_TYPE_AUDIO_G711U     = 0x91,             /* G711格式U律编码 */
    STREAM_TYPE_AUDIO_G722_1    = 0x92,             /* G722格式编码    */
    STREAM_TYPE_AUDIO_G723_1    = 0x93,             /* G723格式编码    */
    STREAM_TYPE_AUDIO_AAC_LD    = 0x94,             /* AAC_LD格式编码  */
    STREAM_TYPE_AUDIO_G726      = 0x96,             /* G726格式编码    */
    STREAM_TYPE_AUDIO_G728      = 0x98,             /* G728格式编码    */
    STREAM_TYPE_AUDIO_G729      = 0x99,             /* G729格式编码    */
    STREAM_TYPE_AUDIO_AMR_NB    = 0x9a,             /* AMR_NB格式编码  */
    STREAM_TYPE_AUDIO_AMR_WB    = 0x9b,             /* AMR_WB格式编码  */
    STREAM_TYPE_AUDIO_L16       = 0x9c,             /* L16格式编码     */
    STREAM_TYPE_AUDIO_L8        = 0x9d,             /* L8格式编码      */
    STREAM_TYPE_AUDIO_DVI4      = 0x9e,             /* DV14格式编码    */
    STREAM_TYPE_AUDIO_GSM       = 0x9f,             /* GSM格式编码     */
    STREAM_TYPE_AUDIO_GSM_EFR   = 0xa0,             /* GSM_EFR格式编码 */
    STREAM_TYPE_AUDIO_LPC       = 0xa1,             /* LPC格式编码     */
    STREAM_TYPE_AUDIO_QCELP     = 0xa2,             /* QCELP格式编码   */
    STREAM_TYPE_AUDIO_VDVI      = 0xa3,             /* VDVI格式编码    */
    STREAM_TYPE_AUDIO_BUTT                          /* 最大无效值     */
}AUDIO_STREAM_TYPE_E;


/*
    解码播放数据类型，控制播放数据流类型
*/
typedef enum 
{
    DECODE_VIDEO   = 0x0,       /* 视频流，静音模式时可使用 */
    DECODE_AUDIO   = 0x1,       /* 音频流 */
    DECODE_MULTI   = 0x2,       /* 复合流 */
} DEC_STREAM_TYPE;

/*
    控制解码播放数据流的类型，对应主机命令  HOST_CMD_DEC_STREAMTYPE
*/
typedef struct
{
	DEC_STREAM_TYPE     streamType;					/* 用户选择播放数据类型，可以选择播放时只播视频或者音频、或者复合流 */
	unsigned char       reserve[64];
} DECODER_STREAMTYPE;

/*
    开启解码属性  HOST_CMD_DEC_START
*/
typedef struct
{
    DEC_MODE_E          mode;                       /* 解码模式 */
    STREAM_PACKET_TYPE  muxType;                    /* 打包模式 */
    VIDEO_STREAM_TYPE_E videoType;                  /* 视频码流类型 */
    unsigned int        width;                      /* 码流的宽 */
    unsigned int        height;                     /* 码流的高 */
    AUDIO_STREAM_TYPE_E audioType;                  /* 音频码流类型 */
} DECODER_PARAM;

/*解码抓图前通知应用的待抓图图像信息*/
typedef struct tagDecImgInfo
{
    unsigned int    width;
    unsigned int    height;
}DEC_IMG_INFO;

typedef enum eDecTPPacketType
{
    PACKET_TYPE_RTP   = 0x0,       /* RTP 封包 */
    PACKET_TYPE_PS    = 0x1,       /* PS  封包 */
    PACKET_TYPE_MEDIA = 0x2,       /* 多媒体 封包 mp4 avi等 */
} DEC_TP_PACKET_TYPE;

/*
    转包 源码流属性
*/
typedef struct tagDecTPSrcPrmSt
{
    DEC_TP_PACKET_TYPE ePacketType;    /* 源码流的封包类型 */
    unsigned char      reserve[64];
} DECODER_TP_SRC_PARAM;

/*
    转包 目标码流属性
*/
typedef struct tagDecTPDstPrmSt
{
    unsigned int        useSetPrm;                  /* 0: 码流属性与源一致  1 : 用下面的配置 */
    STREAM_PACKET_TYPE  muxType;                    /* 打包模式     */
    VIDEO_STREAM_TYPE_E videoType;                  /* 视频码流类型 */
    unsigned int        width;                      /* 码流的宽     */
    unsigned int        height;                     /* 码流的高     */
    AUDIO_ENC_TYPE      audioType;                  /* 音频码流类型 */
    unsigned int        devChan;                    /* 解包转封装到编码的通道号 */
    unsigned int        chanType;                   /* 0:主码流，1:子码流 */
    unsigned char       reserve[24];
} DECODER_TP_DST_PARAM;

/*
   转包属性  HOST_CMD_DEC_TRANS_PACK和HOST_CMD_DEC_TRANS_TO_ENC
   1. 解包并转封包为PS数据，远端录像；
   2. 将RTP流解包并转封包后存放到编码缓冲区中；
      应用场景是探视分机将IPC camera作为本地camera使用，
      需要在本地解码IPC显示，并将流重新打包发送给对方分机
*/
typedef struct tagDecTransPackSt
{
    unsigned int         start;      /* 1:开启, 0:关闭      */
    DECODER_TP_SRC_PARAM stSrcPrm;   /* 源码流属性   */
    DECODER_TP_DST_PARAM stDstPrm;   /* 目标码流属性 */
    unsigned char        reserve[64]; /* 保留 */
} DECODER_TRANS_PACK_PARAM;

/*
   解码暂停功能相关的结构体，对应HOST_CMD_DEC_PAUSE
*/
typedef struct tagDecPausePara
{
    unsigned int  enable;      /* 1:暂停, 0:继续播放       */
    unsigned char reserve[64]; /* 保留 */
} DECODER_PAUSE_PARAM;

/*
    解码通道录像同步本地录像的时间戳，对应HOST_CMD_DEC_SYNC_REC
*/
typedef struct tagDecSyncPara
{
    unsigned int        decNum;                     /* 解码同步通道个数 */
    unsigned int        decChn[MAX_VDEC_CHAN];      /* 多个解码通道号 */
    unsigned int        devChan;                    /* 本地编码的通道号 */
    unsigned int        chanType;                   /* 0:主码流，1:子码流 */
    unsigned char       reserve[64];                /* 保留 */
}DECODER_SYNC_PARAM;

/****************************************************************************/
/*                                 显示                                     */
/****************************************************************************/

#define DISP_FRM_CNT                 (16)          /*每路显示图像缓冲个数*/

/*
    每一个窗口显示的属性
*/
typedef struct
{
    unsigned int    uiChan;                         /* 通道 */
    unsigned int    x;                              /* 窗口的起始坐标的横坐标 */
    unsigned int    y;                              /* 窗口的起始坐标的纵坐标 */
    unsigned int    w;                              /* 窗口的宽度 */
    unsigned int    h;                              /* 窗口的高度 */
    unsigned int    fps;                            /* 显示帧率   */
    unsigned int    color;                          /* 无视频显示时窗口的颜色值 */
    unsigned int    layer;                          /* 窗口的层级，最大值为窗口通道数-1，取值[0,1]，值越大优先级越高。视频显示区域产生叠加时，优先级高的覆盖优先级低的。遇到优先值一致时，以默认值采集视频为优先级高处理 */
    void            *bpBinder;                      /* Android上上层应用下发一个窗口对应的实例 */
    unsigned char   reserve[32];
} DISP_REGION;

/*
    HOST_CMD_DISP_CLEAR 配置不显示的预览窗口
*/
typedef struct tagDispClearCtrlSt
{
    unsigned int  uiCnt;                            /* 配置的窗口的个数   */
    unsigned int  uiChan[MAX_DISP_CHAN];            /* 窗口clear的通道号  */
}DISP_CLEAR_CTRL;

/*
    HOST_CMD_ALLOC_DISP_REGION 预览以及预览窗口控制参数
*/
typedef struct tagDispCtrlSt
{
    unsigned int  uiCnt;                            /* 配置的窗口的个数，有视频广告播放功能最大支持2个，其他型号后续兼容 */
    DISP_REGION   stDispRegion[MAX_DISP_CHAN];      /* 窗口s显示的属性   */
}DISP_CTRL;

typedef struct 
{
    unsigned int  uiCnt;                            /* 配置的窗口的个数 */
    CROP_INFO_ST  stCropRegion[MAX_DISP_CHAN];      /* 窗口裁剪的属性   */
}CROP_PARAM;

#define SCREEN_LAYER_MAX   (3)

/*
    图层的信息，应用在此图层做界面
*/
typedef struct tagScreenAttrSt
{
    unsigned int    isUse;                          /* 是否使用该图层 */
    unsigned int    x;                              /* 起始坐标 */
    unsigned int    y;                              /* 起始坐标 */
    unsigned int    width;                          /* 宽度     */
    unsigned int    height;                         /* 高度     */
    unsigned int    pitch;                          /* 跨度     */
    unsigned int    bitWidth;                       /* 位宽     */
    unsigned char  *srcAddr;                        /* 显存起始地址 */
    unsigned int    srcSize;                        /* 显存大小 */
    unsigned char  *srcPhyAddr;                     /* 显存起始地址(FB DSP物理地址,UI跨进程使用) */
    unsigned char  *srcDmaPhyAddr;                  /* 硬件加速 所需物理内存地址(FB APP物理地址,旋转/拷贝,UI 跨进程使用)*/ 
}SCREEN_ATTR;

/*
    HOST_CMD_DISP_FB_MMAP 获取界面图层地址
*/
typedef struct tagScreenCtrlSt
{
    unsigned int uiCnt;
    SCREEN_ATTR  stScreenAttr[SCREEN_LAYER_MAX];
}SCREEN_CTRL;


/*
    HOST_CMD_DISP_FB_SHOW 刷新屏幕指定区域
*/
typedef struct
{
    unsigned int    x;                              /* 起始坐标 */
    unsigned int    y;                              /* 起始坐标 */
    unsigned int    width;                          /* 宽度     */
    unsigned int    height;                         /* 高度     */
    unsigned int    pitch;                          /* 跨度     */
}LAYER_ATTR;

/*
    HOST_CMD_DISP_FB_SHOW 属性屏幕指定图层窗口的指定区域
*/
typedef struct
{
    unsigned int    layer;
    LAYER_ATTR      layerAttr;
}SCREEN_SHOW;


/*
    视频采集到显示的映射信息
*/
typedef struct
{
    unsigned int uiCnt;
    unsigned int addr[DISP_FRM_CNT];                /* 采集编号(DUP编号)或者解码通道 */
    unsigned int out[DISP_FRM_CNT];                 /* 显示器上显示通道编号(0-N)*/
    unsigned int draw[DISP_FRM_CNT];                /* 是否叠加智能信息，无智能信息项目可以忽略此参数*/
} DSP_DISP_RCV_PARAM;

/*
    显示风格枚举
*/
typedef enum StyleE
{
    STYLE_STANDARD_MODE,                            /* 标准模式 */
    STYLE_BRIGHT_MODE,                              /* 明亮     */
    STYLE_SOFT_MODE,                                /* 柔和     */
    STYLE_GLARING_MODE                              /* 鲜艳     */
}STYLE_E;

/*
    设置显示风格  HOST_CMD_SET_VO_STYLE
*/
typedef struct tagDispStyleSt
{
    STYLE_E eStyle;                                 /* 显示风格属性 */
}DISP_STYLE;

/*
     显示效果  HOST_CMD_SET_VO_CSC
*/
typedef struct tagDispCscSt
{
    unsigned int uiLuma;                            /* 亮度值   */
    unsigned int uiHue;                             /* 色度值   */
    unsigned int uiContrast;                        /* 对比度值 */
    unsigned int uiSatuature;                       /* 饱和度值 */
}DISP_CSC;

/*
    视频输出标准
*/
typedef enum
{
    VS_NON           = 0xa000,
    VS_NTSC          = 0xa001,
    VS_PAL           = 0xa002,
    VS_WD1_NTSC      = 0xa003,
    VS_WD1_PAL       = 0xa004,
    VS_720P_24HZ     = 0xa005,
    VS_720P_25HZ     = 0xa006,
    VS_720P_30HZ     = 0xa007,
    VS_720P_50HZ     = 0xa008,
    VS_720P_60HZ     = 0xa009,
    VS_1080I_50HZ    = 0xa00a,
    VS_1080I_60HZ    = 0xa00b,
    VS_1080P_24HZ    = 0xa00c,
    VS_1080P_25HZ    = 0xa00d,
    VS_1080P_30HZ    = 0xa00e,
    VS_1080P_50HZ    = 0xa00f,
    VS_1080P_60HZ    = 0xa010,
    VS_HF1080P_25HZ  = 0xa011,
    VS_HF1080P_30HZ  = 0xa012,
    VS_HF720P_50HZ   = 0xa013,
    VS_HF720P_60HZ   = 0xa014,
    VS_I576_50HZ     = 0xa015,
    VS_I486_60HZ     = 0xa016,
    VS_S1080_24HZ    = 0xa017,
    VS_1024x600_50HZ = 0xa018,
    VS_1024x600_60HZ = 0xa019,
    VS_UNKNOWN_VIDEO = 0xa020,
    VS_NO_VIDEO      = 0xa021,
    VS_BULL,
}VIDEO_STANDARD;

/*
    显示输出接口类型
*/
typedef enum tagVideoOutputDevEn
{
    VO_DEV_LCD       = 0,                           /* LCD显示  */
    VO_DEV_VGA       = 1,                           /* VGA 显示 */
    VO_DEV_HDMI      = 2,                           /* HDMI显示 */
    VO_DEV_CVBS      = 3,                           /* CVBS显示 */
    VO_DEV_MIPI      = 4,                           /* 最大无效值 */  
    VO_DEV_MAX_CNT   = 5,                           /* 最大无效值 */
} VIDEO_OUTPUT_DEV_E;

/*
    设置显示输出标准  HOST_CMD_SET_VO_STANDARD
*/
typedef struct tagDispStandardSt
{
    VIDEO_STANDARD eStandard;                       /* 显示输出标准 */
}DISP_STANDARD;

/*
    设置显示设备属性
*/
typedef struct tagDispDevAttrSt
{
    VIDEO_OUTPUT_DEV_E  eVoDev;                     /* 显示输出设备类型 */

    int videoOutputWidth;                           /* 视频输出设备宽度 */
    int videoOutputHeight;                          /* 视频输出设备高度 */
    int videoOutputFps;                             /* 视频输出设备刷新帧率 */
}DISP_DEV_ATTR;

/*
    显示通道属性
*/
typedef struct tagVideoOutputInitPrmSt
{
    unsigned int        bHaveVo;                    /* 是否有视频输出   */
    unsigned int        voChnCnt;                   /* 应用设置的最大通道数，只接收一次 */    
    DISP_DEV_ATTR       stDispDevAttr;              /* 显示设备属性     */
}VO_INIT_PRM;

/*
    显示输出信息
*/
typedef struct tagVideoOutputInitInfoSt
{
    unsigned int uiVoCnt;                           /* vo 输出接口数     */
    VO_INIT_PRM  stVoInfoPrm[MAX_DISP_CHAN];        /* 每一路vo 输出属性 */
}VO_INIT_INFO_ST;

/*
    显示模块插入单张图片属性 HOST_CMD_DISP_INSERT_PIC_PARAM 
*/
typedef struct tagDispUserPicPrmSt
{
    unsigned int enable;                            /* 功能使能                 */
    int          dispChn;                           /* 该显示设备上的显示通道号 
                                                                    -1:表示所有的通道，大于0，表示具体的通道 */
    DSP_PIC_INFO picInfo;                           /* 数据格式                 */
}DISP_USER_PIC_PRM;

/*
    屏幕standby功能，防止极化 
    HOST_CMD_DISP_STANDBY
*/
typedef struct tagDispStandbyPrmSt
{
    unsigned int  uiEnable;                         /* 是否使能 */
}DISP_STANDBY_PRM; 

/* HOST_CMD_DISP_FBE_PARAM 配置预览人脸美颜 */
typedef struct tagDispFbePrmSt
{
    unsigned int    fbeEnable;                      /* 开启预览美颜 0:关闭  1:开启 */
    unsigned int    white;                          /* 美白强度 取值[0-10] ,app默认设置美颜强度值为5*/ 
    unsigned int    fil;                            /* 磨皮强度 取值[0-10] */
    unsigned char   reserve[64];
}DISP_FBE_PRM;


/* 热成像图片显示的方式*/
typedef enum stDispThermoShowTypeE
{
    DSP_DISP_THERMO_ONLY  = 0,           /* 只显示热成像图像，全屏显示 */
    DSP_DISP_WL_ONLY      = 1,           /* 只显示可见光图像，全屏显示 */
    DSP_DISP_THERMO_BACK  = 2,           /* 热成像图片在下面，可见光在上面 */
    DSP_DISP_WL_BACK      = 3,           /* 可见光图片在下面，热成像在上面  */
    DSP_DISP_THERMO_MAX,
}DISP_THERMO_SHOW_TYPE_E;

/*
    屏幕显示热成像图片参数
    HOST_CMD_DISP_THERMO
*/
typedef struct tagDispThermoPrmSt
{
    unsigned int  uiEnable;                   /* 是否使能 */
    DISP_THERMO_SHOW_TYPE_E  showType;        /* 显示类型*/
    unsigned char *uiAddr;                    /*热成像图片数据 */
    unsigned int   uiSize;                    /*热成像图片长度 */

    /* 可将光图像位置 */
    unsigned int  wlX; 
    unsigned int  wlY; 
    unsigned int  wlW; 
    unsigned int  wlH; 

    /* 热成像图像位置 */
    unsigned int  thermoX; 
    unsigned int  thermoY; 
    unsigned int  thermoW; 
    unsigned int  thermoH; 

    /* 热成像图像人脸框位置  */
    float         rectX; 
    float         rectY; 
    float         rectW; 
    float         rectH; 
	
	unsigned char   reserve[64];
}DISP_THERMO_PRM; 



/* USB图像显示的方式*/
typedef enum stUsbDispShowTypeE
{
    USB_DISP_IRIS_ONLY = 0,           /* 只显示USB图像，全屏显示 */
    USB_DISP_WL_ONLY   = 1,           /* 只显示可见光图像，全屏显示,暂不支持 */
    USB_DISP_IRIS_BACK = 2,           /* USB图片在下面，可见光在上面,暂不支持 */
    USB_DISP_WL_BACK   = 3,           /* 可见光图片在下面，USB在上面,暂不支持  */
    USB_DISP_MAX,
}USB_DISP_SHOW_TYPE_E;

/*
    屏幕显示USB相机图像参数
    HOST_CMD_DISP_USB
*/
typedef struct tagUsbDispSt
{
    unsigned int  enable;        	/* 是否使能 */

    USB_DISP_SHOW_TYPE_E  showType;/* 显示类型*/

    /* 可见光图像位置 */
    unsigned int  wlX; 
    unsigned int  wlY; 
    unsigned int  wlW; 
    unsigned int  wlH; 

    /* USB图像位置 */
    unsigned int  usbX; 
    unsigned int  usbY; 
    unsigned int  usbW; 
    unsigned int  usbH; 
	
	unsigned char   reserve[64];
}USB_DISP_PRM; 

/*
    人脸框渲染模式
*/
typedef enum
{
    DISP_FACE_MODE_CIRCULATE     = 0,                        /* 多图循环显示模式 */
    DISP_FACE_MODE_SINGLE_ROTATE = 1,                        /* 单图旋转显示模式 */
    DISP_FACE_MODE_MAX,
} DISP_FACE_MODE;

/*
    人脸跟踪框类型
*/
typedef enum
{
	DISP_FACE_TYPE_DEFAULT       = 0,                        /* 默认的人脸框类型     */
    DISP_FACE_TYPE_STRANGER      = 1,                        /* 陌生人的人脸框类型   */
	DISP_FACE_TYPE_AUTHENTICATOR = 2,                        /* 通过验证的人脸框类型 */
    DISP_FACE_TYPE_BLACKLIST     = 3,                        /* 黑名单的人脸框类型   */
    DISP_FACE_TYPE_WHITELIST     = 4,                        /* 白名单的人脸框类型   */
    DISP_FACE_TYPE_MAX,
} DISP_FACE_TYPE;

/*
    显示模块人脸层级图片属性
*/
typedef struct tagDispFaceLayerPrmSt
{
    unsigned int    picCnt;                                  /* 人脸图片个数        */
    DSP_PIC_INFO    picInfo[MAX_DISP_FACE_PIC_CNT];          /* 动态人脸图显示信息  */
    DISP_FACE_MODE  dispFaceMode;
    unsigned char   reserve[64];
}DISP_FACE_PIC_LAYER_PRM;

/*
    显示模块设置人脸图片属性 HOST_CMD_SET_DISP_FACE_PIC 
*/
typedef struct tagDispFacePicPrmSt
{
    unsigned int            facePicLayerCnt;                        /* 人脸框图层个数   */
    DISP_FACE_PIC_LAYER_PRM facePicLayer[MAX_DISP_FACE_LAYER_CNT];  /* 人脸框图层信息   */
    DISP_FACE_TYPE          faceType;                               /* 人脸跟踪框的类型 */
    unsigned char           reserve[64];
}DISP_FACE_PIC_PRM;

/****************************************************************************/
/*                                 音频                                     */
/****************************************************************************/

/* 
    混音音频业务类型
*/
typedef enum
{
    AMER_IDX_NULL = 0,
    AMER_IDX_PLAYBACK,
    AMER_IDX_SDKTB,
    AMER_IDX_AUDTB,
    AMER_IDX_BROADCAST,
    AMER_IDX_RTBROADCAST,
    AMER_IDX_LOOPBACK,
    AMER_IDX_MAX
}AUD_AMER_IDX;

/*
    音频混音配置信息
*/
typedef struct
{
   unsigned int         bEnable;                   /* 音频混音功能使能，DSP暂时不处理该字段         */
   unsigned int         priority[AMER_IDX_MAX];    /* 优先级    */
   unsigned int         mainVolume;                /* 主音量，用于高优先级音频 */
   unsigned int         subVolume;                 /* 副音量，用于低优先级音频 */
   
   unsigned char        reserve[32];
}AUDIO_AMER_PARAM;

/*
    音频输出通道信息
*/
typedef struct tagAudioChnInfoSt
{
    unsigned int    aoChan;                         /* 操作所对应的音频通道 */
    unsigned int    addr;                           /* 操作所对应的地址     */
    unsigned int    rate;                           /* 音频采样率           */
    unsigned char   reserve[64];
} AUDIO_CHN_INFO_S;

/*
    操作音频输出结构体
    HOST_CMD_SET_AUDIO_LOOPBACK
    HOST_CMD_DEC_SET_DSP_AUDIO

    HOST_CMD_SET_TALKBACK
*/
typedef struct
{
    unsigned int     bEnable;                       /* 是否使能         */
    AUDIO_CHN_INFO_S stAudioChn;                    /* 音频输出通道信息 */
} DSP_AUDIO_PARAM;

/*
    网络其他设备和模拟室内机对讲 控制信息
    HOST_CMD_SET_MANAGE_TALKTO_RESIDENT
*/
typedef struct tagAudioManageToResidentPrmSt
{
    unsigned int bEnable;                           /* 是否使能 */
    ENC_CHNTYPE  encChnType;                        /* 控制音频使用的编码通道，主码流、
                                                       子码流或是第三码流，默认0是使用主码流 */
    unsigned char   reserve[64];
} AUDIO_MANAGE_TO_RESIDENT_PRM;

/*
    门口机与模拟室内机通信 控制信息
    HOST_CMD_SET_DOOR_TALKTO_RESIDENT
*/
typedef struct tagAudioDoorToResidentPrmSt
{
    unsigned int bEnable;                           /* 是否使能    */

    unsigned int micInChn;                          /* 本地输入通道 */
    unsigned int analogInChn;                       /* 模拟输入通道 */
    unsigned int speakOutChn;                       /* 本地输出通道 */
    unsigned int analogOutChn;                      /* 模拟输出通道 */
} AUDIO_DOOR_TO_RESIDENT_PRM;

/*
    语音播报类型
*/
typedef enum
{
    AUDIO_PLAY_NON    = 0x0,
    AUDIO_PLAY_KEY    = 0xa1,
    AUDIO_PLAY_INFO   = 0xa2,
    AUDIO_PLAY_ALARM  = 0xa3,
    AUDIO_PLAY_BUTT
}AUDIO_PLAY_TYPE;

/*
    语音打断类型
*/
typedef enum
{
    AUDIO_INTERRUPT_COVER      = 0x0,              /* 被打断时，直接覆盖 */
    AUDIO_INTERRUPT_RECOVER    = 0x1,              /* 被打断时，恢复播放 */
    AUDIO_INTERRUPT_MIX        = 0x2,              /* 被打断时，混音播放 */
    AUDIO_INTERRUPT_MAX
}AUDIO_INTERRUPT_TYPE;

/* tts的配置 */
typedef enum  
{
    AUDIO_TTS_CONVERT     = 0x0,
    AUDIO_TTS_DIRECT_PLAY = 0x1, 
    AUDIO_TTS_BUTT
}TTS_USAGE_TYPE;

typedef struct
{
    char *voiceName;                                /* 选择不同的发声人 */
    char *txtEncode;                                /* 文本的编码格式,   支持参数: GB2312，GBK，BIG5，UNICODE，GB18030，UTF8 */
    unsigned int speed  ;                           /* 范围[0,100]，值越大语速越快 */
    unsigned int volume ;                           /* 范围[0,100]，值越大音量越大 */
    unsigned int pitch  ;                           /* 范围[0,100]，值越大语调越高 */
    unsigned int rdn    ;                           /* 范围[0,2]，  数字的发声方式  0 按照语义自动判断,1 完全数值,2 完全字符串 */
}TTS_ATTR_CONFIG;

typedef struct
{
   TTS_USAGE_TYPE       usageType;                  /* TTS接口的调用方式 : 1、只转换；2、转完并直接播放*/
   unsigned char        *tts2AudBuf;                /* 放置文本的缓存,目前 DSP 先开20KB，具体大小以后可以再商议 */
   unsigned int         dataLen;                    /* 文本的长度 */
   AUDIO_PLAY_TYPE      stPlayType;                 /* 语音播报类型 */
   unsigned int         sampleRate;                 /* 以怎样的采样率来合成语音 */
   unsigned int         priority;                   /* 值越大优先级越高,范围[1, 10] */
   TTS_ATTR_CONFIG      stTTSAttr;
   unsigned int         id;                         /* 标识长文本 ，用于区分不同小段是否属于同一文本字段用，默认值为 0 */
   unsigned int         bEnable;                    /* 使能位，0关闭，1开启 */
    AUDIO_ENC_TYPE  audEncType;                     /* 音频压缩算法 */
    AUDIO_INTERRUPT_TYPE audInsertType;             /* 音频打断播放类型 */
   unsigned char        reserve[20];
}AUDIO_TTS_PARAM;

typedef enum
{
    QUE_STATE_MIN   = 0,
    QUE_STATE_EMPTY = 1,                       /* 队列有空间 */
    QUE_STATE_FULL  = 2,                       /* 队列满，无空间*/
    QUE_STATE_MAX
}AUDIO_QUE_STATE;

/*
    语音播报功能 信息
    HOST_CMD_START_AUDIO_PLAY
*/
typedef struct
{
    void*               audioBuf;                   /* 音频数据缓存 */
    unsigned int        audioDataLen;               /* 音频数据长度，除去文件头 */
    unsigned int        channel;                    /* 单通道或者双通道 */
    unsigned int        rate;                       /* 采样率 */
    unsigned int        bits;                       /* 数据位数 */
    unsigned int        uiPlayTime;                 /* 播放时间:单位ms，0-播放一遍
                                                                    非0-播放固定时间，一遍结束后重复播放 */
    AUDIO_PLAY_TYPE     stPlayType;                 /* 语音播报类型 */
    AUDIO_CHN_INFO_S    stAudioChn;                 /* 音频输出通道信息 */
    unsigned int        priority;                   /* 音频播放优先级，值越大优先级越高,范围1~10 */
    AUDIO_ENC_TYPE      audEncodeType;              /* 铃声格式，默认wav   */
    unsigned int        volume;                     /* 放音音量大小*/
    /* 下发的第audioIndex个音频从第audioFrameIndex帧开始播放 */
    unsigned int        audioIndex;                 /* 音频数据编号 */
    unsigned int        audioFrameIndex;            /* 音频数据帧编号，编号从1开始，如该参数未设置，默认从第1帧开始播放 */
    AUDIO_QUE_STATE     que_state;                  /* 送入队列的状态*/
    unsigned int        isCurrentFileFinalData;     /* 是否是当前文件的最后一块数据，True表示是，False表示否 */
    unsigned int        error_code;                 /* 输入铃声解析失败错误类型，1表示数据解码失败，需要继续下发下一段，-1表示参数解析失败  ，0表示正常*/
    unsigned char       reserve[12];                /* 预留 */
}AUDIO_PLAY_PARAM;

/*
    语音播报状态，暂停or恢复
    HOST_CMD_CONTROL_AUDIO_PLAY
*/
typedef enum
{
    AUDIO_PLAY_SUSPEND    = 0,
    AUDIO_PLAY_RECOVERY   = 1,
    AUDIO_PLAY_STATE_MAX
}AUDIO_PLAY_STATE;

/*
    语音播报功能控制功能
    HOST_CMD_CONTROL_AUDIO_PLAY
*/
typedef struct
{
    AUDIO_PLAY_STATE    audioPlayState;             /* 控制音频播放功能，暂停or恢复 */
    unsigned char       reserve[16];                /* 预留 */
}AUDIO_PLAY_CTRL_PARAM;

/* 广播类型 */
typedef enum
{
    BROADCAST_FILE = 0,                        /* 音频文件 广播            */
    BROADCAST_CAP  = 1,                        /* 音频采集设备 广播 */
    BROADCAST_APK  = 2,                        /* 第三方APK音源 广播 */
    BROADCAST_LOCAL_PLAY = 3,                  /*本地定时广播播放   */
    BROADCAST_REALTIME_PLAY = 4,               /* 网络实时广播播放*/
    BROADCAST_MAX
}BROADCAST_TYPE;

/*
    广播功能 信息
    HOST_CMD_START_AUDIO_BROADCAST
*/
typedef struct
{
    unsigned int        fileID;                     /*下发文件ID*/
    BROADCAST_TYPE      broadcastType;              /* 广播类型 */
    unsigned int        bEnable;
    AUDIO_ENC_TYPE      audStreamEncodeType;        /* 返回音频流编码格式 */
    
    /* 音频文件 广播信息 */
    void*               audioBuf;                   /* 音频数据缓存，下发WAVE文件时要去除44字节头，下发MP3文件时不用去除头 */
    unsigned int        audioDataLen;               /* 下发数据长度，长度不超过512K。如果一个大文件需多次下发，除最后一次下发外，其余每次下发长度都必须等于512K */
    unsigned int        channel;                    /* 单声道或者双声道，只有下发WAVE文件时需要配置该参数，1表示单声道，2表示双声道 */
    unsigned int        samplerate;                 /* 采样率，只有下发WAVE文件时需要配置该参数，支持8k/16k/22.05k/32k/44.1k/48k */
    unsigned int        bits;                       /* 数据位数 */
    unsigned int        uiPlayTime;                 /* 播放时间:单位ms，0-播放一遍,非0-播放固定时间，一遍结束后重复播放 */
    unsigned int        priority;                   /* 音频播放优先级，值越大优先级越高,范围1~10 */
    AUDIO_ENC_TYPE      audFileEncodeType;          /* 文件格式，目前支持WAVE和MP3 */
    AUDIO_QUE_STATE     que_state;                  /*送入队列的状态*/

    /* 音频采集设备 广播信息 */

    /* Android APK 广播信息 */
    unsigned int        isLocalplay;                /* 本地是否播放，0:不播放，1:播放 */

    unsigned int        isCurrentFileFinalData;     /* 是否是当前文件的最后一块数据，True表示是，False表示否 */
    unsigned char       reserve[28];                /* 预留 */    
}AUDIO_FILE_BROADCAST_PARAM;

/*
    自动应答功能结构体
    HOST_CMD_START_AUTO_ANSWER
*/
typedef struct
{
   char             *audioBuf;                      /* 音频数据缓存 */
   unsigned int     audioDataLen;                   /* 音频数据长度 */
   unsigned char    reserve[32];                   /* 预留         */
} AUTO_ANSWER_PARAM;

/*
    开启语音录制
    HOST_CMD_START_AUDIO_RECORD
*/
typedef struct tagAudioRecordPrmSt
{
   unsigned int     uiTime;                         /* 录制时长        */
   AUDIO_ENC_TYPE   type;                           /*音频录制格式*/
   unsigned char    reserve[32];
}AUDIO_RECORD_PRM_S;

/*
    设置声音超限报警阈值
    HOST_CMD_SET_AUDIO_ALARM_LEVEL
*/
typedef struct tagAudioAlarmLevelPrmSt
{
    unsigned int bEnable;                           /* 是否使能 */
    unsigned int uiLevel;                           /* 阈值     */
}AUDIO_ALARM_LEVEL_PRM_S;

typedef enum 
{
    AUDIO_ALL_VOLUME  = 0x0,                        /* 铃声与对讲不做区分，均输出采用同一输出增益 */
    AUDIO_RING_VOLUME = 0x1,                        /* 铃声采用的输出增益 */
    AUDIO_TALK_VOLUME = 0x2,                        /* 对讲采用的输出增益 */
    AUDIO_VOLUME_BUTT = 0x3
}AUDIO_VOL_TYPE;

/*
    设置音频输入音量
    HOST_CMD_SET_AI_VOLUME
    设置音频输出音量
    HOST_CMD_SET_AO_VOLUME
*/
typedef struct tagAudioVolPrmSt
{
    unsigned int uiChn;                             /* 本地还是远端的通道的SPK或者MIC.0:本地，1:远端*/
    unsigned int uiVol;                             /* 音频声音值   */
    AUDIO_VOL_TYPE enVolType;                       /* 音量控制类型，铃声音量或者对讲音量 */
}AUDIO_VOL_PRM_S;

/*
    设置音频输入音量扩展配置
    HOST_CMD_SET_AI_VOLUME_EX
    设置音频输出音量扩展配置
    HOST_CMD_SET_AO_VOLUME_EX
*/
typedef struct tagAudioVolExPrmSt
{
    unsigned int baseLevel;                         /* 最低的基准音量            */
    unsigned int stepSize;                          /* 以基准音量 向上增长的步长 */
}AUDIO_VOL_EX_PRM_S;

/*
    设置听筒免提切换
    HOST_CMD_SET_EAR_PIECE
*/
typedef struct tagAudioEarpiecePrmSt
{
    unsigned int uiIsUsed;                          /* 是否使用听筒 1 表示听筒，0 表示免提 */
}AUDIO_EARPIECEL_PRM_S;

/*
    设置音频输出设备
    HOST_CMD_SET_AO_DEV
*/
typedef enum
{
    AO_DEV_LOCAL_SPK  = 0x0, /* 本地扬声器 */
    AO_DEV_EARPIECE   = 0x1, /* 听筒 */
    AO_DEV_EARPHONE   = 0x2, /* 耳机 */
    AO_DEV_AMPLIFIER  = 0x3, /* 功放扬声器 */
    AO_DEV_REMOTE_SPK = 0x4, /* 远端扬声器 */
    AO_DEV_BLUETOOTH  = 0x5, /* 蓝牙输出设备 */
    AO_DEV_BUTT       = 0xFF
}AUDIO_OUTPUT_DEV;

/*
    设置音频输出设备
    HOST_CMD_SET_AO_DEV
*/
typedef struct tagAudioOutputDevPrmSt
{
    AUDIO_OUTPUT_DEV stOutDev; /* 音频输出设备类型 */
	unsigned int     devChn;   /* 通道 			  */
	unsigned char               reserve[28];        /* 备用 */

}AUDIO_OUTPUT_DEV_PRM;

/*
    设置音频输入设备
    HOST_CMD_SET_AI_DEV
*/
typedef enum
{
    AI_DEV_LOCAL_MIC  = 0x0, /* 本地麦克风 */
    AI_DEV_EARPIECE   = 0x1, /* 听筒       */
    AI_DEV_EARPHONE   = 0x2, /* 耳机       */
    AI_DEV_PICKUP     = 0x3, /* 外接拾音器 */
    AI_DEV_REMOTE_MIC = 0x4, /* 远端麦克风 */
    AI_DEV_EXTERNAL_DVD = 0x5, /* 3.5mm接口外接DVD大功率设备 */
    AI_DEV_BLUETOOTH    = 0x6, /* 蓝牙输入设备 */
    AI_DEV_BUTT         = 0xFF
}AUDIO_INPUT_DEV;

/*
    设置音频输入设备
    HOST_CMD_SET_AI_DEV
*/
typedef struct tagAudioInputDevPrmSt
{
    AUDIO_INPUT_DEV stInDev;     /* 音频输入设备类型 */
	unsigned int    devChn;      /* 通道 			 */
	unsigned char   reserve[28]; /* 备用             */

}AUDIO_INPUT_DEV_PRM;

#define AUDIO_MAX_MIX_CHN   (2)

/*
    混音功能的类型
*/
typedef enum eAudioMixType
{
    AUDIO_MIX_TALK_BACK     = 0x0, /* 语音对讲混音 */
    AUDIO_MIX_SDK_TALK_BACK = 0x1, /* SDK对讲混音  */
    AUDIO_MIX_BUTT          = 0xff
}AUDIO_MIX_TYPE;

/*
    混音功能 结果处理方式
*/
typedef enum eAudioMixDstType
{
    AUDIO_MIX_SAVE_PS  = 0x1, /* 保存为 主PS  流 */
    AUDIO_MIX_SAVE_RTP = 0x2, /* 保存为 主RTP 流  */

    AUDIO_MIX_SAVE_IPC_PS  = 0x4, /* 保存为 远端PS  流 */
    AUDIO_MIX_SAVE_IPC_RTP = 0x8, /* 保存为 主RTP 流  */

    AUDIO_MIX_DST_BUTT = 0xff
}AUDIO_MIX_DST_TYPE;

/*
   混音通道属性
*/
typedef struct tagAudioMixChnPrmSt
{
    unsigned int chn;       /* 通道号 */
    unsigned char reserve[64];
}AUDIO_MIX_CHN_PRM_S;

/*
    开启混音功能 混音功能的属性
    HOST_CMD_START_AUDIO_MIX
*/
typedef struct tagAudioMixPrmSt
{
    unsigned int        mixNum;       /* 混音功能的路数，默认为2 DSP暂时不处理该字段 */
    AUDIO_MIX_TYPE      eMixType;     /* 混音功能的类型，默认为0 */

    AUDIO_MIX_CHN_PRM_S stChnPrm[AUDIO_MAX_MIX_CHN]; /* 混音功能各通道属性 */

    unsigned int        eDstType;     /* 混音数据的处理方式 AUDIO_MIX_DST_TYPE */
    unsigned char       reserve[32];
}AUDIO_MIX_PRM_S;


/*
    音频语音播报回调类型
*/
typedef enum 
{
    AUDIO_PLAY_CB_TYPE_MIN       = 0x0,
    AUDIO_PLAY_CB_TYPE_RING      = 0x1,                        /* 铃声播报 */
    AUDIO_PLAY_CB_TYPE_TTS       = 0x2,                        /* TTS播报 */
    AUDIO_PLAY_CB_TYPE_BROADCAST = 0x3,                        /* 定时广播播报 */
    AUDIO_PLAY_CB_TYPE_MAX       = 0x4
}AUDIO_PLAY_CB_TYPE;

/*
    音频语音播报回调结果
*/
typedef struct tagAudioPlayCbResultSt
{
    unsigned int AudioID;            /* 音频编号 */
    unsigned int isFullPlay;         /* 编号为AudioID的音频是否完全播报结束，1表示完全播报，0表示中途打断 */
    unsigned int interruptedPos;     /* 被打断的位置，此变量为非零值时有意义，如值为15，表示编号为AudioID的音频第15帧及之后的帧未播放 */
    unsigned int listIndex;          /* 同一AudioID的音频所在播放队列位置1为起始位置 */
    AUDIO_PLAY_CB_TYPE AudioPlayCbType;     /* 音频语音播报回调类型 */
    unsigned char reserve[24];
}AUDIO_PLAY_CB_RESULT_ST;

/****************************************************************************/
/*                             人脸智能                                     */
/****************************************************************************/
/* 预抓图数量的宏定义 */
#define PRE_SNAP_NUM                                (5)

#define MAX_JPGMOD_NUM                              (4)
/* 智能模型数据最大长度 */
#define FR_MAX_MODEL_DATA_LENTH                     (1024)
/* 最大人脸检出数量 */
#define FR_MAX_FFD_FACE_NUM                         (10)

/* FR_LICENCE_STAT 算法加密库授权状态 */
typedef enum
{
     FR_LICENCE_NO = 0,
     FR_LICENCE_FAIL ,
     FR_LICENCE_SUCCESS
}FR_LICENCE_STAT;

/* 特征点的坐标定义，float 类型的坐标为归一化值 */
typedef struct
{
    float x;                                        /* 特征点坐标 x */
    float y;                                        /* 特征点坐标 y */
}FR_POSITION;

/*
    特征点结构体，float 类型的坐标为归一化值
*/
typedef struct _FR_CHARACT_POINTS
{
    float          faceX;                           /* 人脸坐标 x */
    float          faceY;                           /* 人脸坐标 y */
    float          faceW;                           /* 人脸宽度   */
    float          faceH;                           /* 人脸高度   */

    FR_POSITION    leftEye;                         /* 左眼坐标  */
    FR_POSITION    rightEye;                        /* 右眼坐标  */
    FR_POSITION    leftMouth;                       /* 嘴左边坐标  */
    FR_POSITION    rightMouth;                      /* 嘴右边坐标  */
    FR_POSITION    noseTip;                         /* 鼻子坐标  */
}FR_CHARACT_POINTS;

/* 活体检测控制类型 */
typedef enum
{
    FR_LD_ONCE          = 0,                        /* 开启一次活体检测 */
    FR_LD_START         = 1,                        /* 开启连续活体检测 */
    FR_LD_STOP          = 2,                        /* 停止连续活体检测 */
    FR_LD_PIC           = 3,                        /* 使用下发图片活体检测 */
    FR_LD_BUTT
}FR_LD_CRTL_TYPE;

/* 设置开启的活体算法类型, 按照位来设置，支持多种算法并存 */
typedef enum
{
    FR_SOLID_SELF       = 0,                        /* 部门自研活体 */
    FR_SOLID_YJY        = 1,                        /* 研究院活体 */
    FR_SOLID_INV        = 0xffffffff
}FR_SOLID_TYPE;

/*
    活体检测命令字结构体 HOST_CMD_FR_DET
*/
typedef struct frSolidPrmSt
{
    unsigned int      idx;                          /* 回调索引 */
    FR_LD_CRTL_TYPE   eCtrlType;                    /* 活体检测控制类型 */
    LD_SCENE          scene;                        /* 活体检测应用场景和安全级 */

    /* 以下值 只有控制类型为 图片检测时有效 */
    unsigned int      uiNJpgSize;                   /* 可见光JPG图片长度 */
    unsigned int      uiIrJpgSize;                  /* 红外JPG图片长度 */
    FR_CHARACT_POINTS nCPoints;                     /* 可见光特征点坐标 */
    unsigned char*    jpgAddr;                      /* JPEG 数据的地址 */
    unsigned int      totalLen;                     /* JPEG 数据总长度 */
    unsigned int      retCropFrm;                   /* 是否返回裁剪后的图像 */
    unsigned int      solidType;                    /* 活体算法类型见 FR_SOLID_TYPE */
    unsigned int      sceneFlag;                    /* 场景转换开关，置1时将scene进行内部转换 ，先去掉，功能实现后变更头文件 */
	unsigned char     reserve[28];                  /* 备用 */
}FR_SOLID_PRM_S;

/* 活体等级控制 */
typedef enum 
{
    LD_OFF          = 0,                            /* 活体关闭 */
    LD_NORMAL       = 1,                            /* 活体通用等级 */
    LD_ENHANCE      = 2,                            /* 活体增强等级 */
    LD_PROFESSIONAL = 3,                            /* 活体专业等级 */
    LD_CUSTOM       = 4,                            /* 活体自定义等级 */
    LD_NONE,                                        /* 彻底关闭活体 */
}LD_LEVEL_CTRL;

/*
    新·活体检测命令字结构体 HOST_CMD_FR_DET_V2
*/
typedef struct frSolidPrmStV2
{
    unsigned int      idx;                          /* 回调索引 */
    LD_LEVEL_CTRL     liveLevel;                    /* 活体等级 */
    unsigned char     reserve[128];                 /* 备用 */
}FR_SOLID_PRM_S_V2;

/*
    活体阈值设置和获取结构体 
    HOST_CMD_SET_THRESHOLD
    HOST_CMD_GET_THRESHOLD
*/
typedef struct frSolidThershold
{
    unsigned int      fdType;                       /* 0->自研活体;1->研究院活体 */
    LD_LEVEL_CTRL     liveLevel;                    /* 活体等级 */
    float             threshold;                    /* 活体阈值 */
    float             maskThreshold;                /* 口罩活体阈值 */
    float             distance;                     /* 距离:备用 */
    unsigned char     reserve[32];                 	/* 备用 */
}FR_SOLID_THRESHOLD;


/*
    活体透传
    HOST_CMD_SET_SOLID_PARAM
    HOST_CMD_GET_SOLID_PARAM
*/
typedef struct frSolidTransPrmSt
{
    unsigned int cmd;
    unsigned long long value;
}FR_SOLID_TRANS_PRM_ST;

/*
    JPEG图片数据信息 对应 JPEG_TYPE
*/
typedef struct frBMJpegPrmSt
{
    unsigned int      uiSize;                       /* 图片的数据的大小 */
    unsigned char*    uiAddr;                       /* 图片的数据的地址 */
    unsigned int      picCnt;                       /* 批量传递的JPG数量*/
    unsigned int      picLen[MAX_JPGMOD_NUM];       /* 每一张JPG的长度*/
    unsigned int      idNum[MAX_JPGMOD_NUM];        /* 对应的ID 号*/
    unsigned int      haveChPoints;                 /* 是否包含特征点*/
    FR_CHARACT_POINTS cPoints;                      /* 特征点坐标 匹配第一张*/
}FR_BM_JPEG_PRM_ST;

 /* BMP转JPG结构体 */
typedef struct frJPGBMPPrmSt
{
    unsigned int    idx;                        /*回调索引*/
    unsigned int    uiSize;                     /* JPG + BMP 数据的总大小 */
    unsigned int    uiJpgSize;                  /* JPG数据的大小 */
    unsigned int    uiBmpSize;                  /* BMP数据的大小 */
    unsigned char*  uiAddr;                     /* JPG和BMP数据地址,BMP在前 */
}FR_BM_JPG_BMP_PRM_ST;

 /* JPG转JPG结构体 */
typedef struct frJPGJPGPrmSt
{
    unsigned int    idx;                        /* 回调索引 */
    unsigned int    uiSize;                     /* 数据的总大小 */
    unsigned int    uiJpgSize;                  /* JPG数据的大小 */
    unsigned char*  uiAddr;                     /* JPG数据指针 */
}FR_BM_JPG_JPG_PRM_ST;


/* 
	图片缩小(目前不支持放大)、重编码结构体；支持jpg和png，bmp；
	1.缩小按照图片分辨率等比例暴力处理，比例不对齐时填白，请谨慎使用,
	2.DSP在分辨率有对齐要求的芯片上开发时进行向下对齐；
	3.对应主机命令 HOST_CMD_FR_JPGSCALE  
*/

typedef struct frJPGScalePrmSt
{
    unsigned int    srcJpgSize;                	/* 待处理图片数据的大小，内存复用图片下发，不能超过2MB */
    unsigned char*  srcJpgAddr;                	/* 待处理图片数据指针,待处理图片分辨率不能超过1080P(1920x1080)，超过直接返回失败 */

    unsigned int    dstJpgSize;               	/* 目标JPG数据的大小 */
    unsigned char*  dstJpgAddr;              	/* 目标JPG数据指针,需外部申请，大小为分辨率的1.5倍，内部会限制输出分辨率不能大于1080P，所以此处最大不会超过3MB,建议外部直接申请3MB大小 */

	unsigned int    dstJpgWidth;                /* 目标JPG数据的宽(外部想缩小的大小),范围:[64~1920](dstJpgWidth x dstJpeHeight <= 1920x1080),无缩放进行重编码时dstJpgWidth和dstJpeHeight必须为全0         */
    unsigned int    dstJpeHeight;               /* 目标JPG数据的高(外部想缩小的大小),范围:[64~1920](dstJpgWidth x dstJpeHeight <= 1920x1080) 无缩放进行重编码时dstJpgWidth和dstJpeHeight必须为全0*/

    unsigned int    dstJpgRealWidth;           	/* 外部需求的分辨率没有对齐时(更多的情况是芯片要求对齐)，DSP内部向下对齐(如125或变成124)，并赋值对齐后的分辨率信息 */
    unsigned int    dstJpeRealHeight;          	/* 外部需求的分辨率没有对齐时(更多的情况是芯片要求对齐)，DSP内部向下对齐(如125或变成124)，并赋值对齐后的分辨率信息 */
	
    unsigned char   reserve[56];        		/* 备用 */

}FR_BM_JPG_SCALE_PRM_ST;


/*
    建模具体信息，对应不同的 FR_DATA_TYPE_E
*/
typedef union frBuildModelPrmU
{
    FR_BM_JPEG_PRM_ST       stJpegTypePrm;          /* 对应 JPEG/BMP/PNG 图片的信息 */
}FR_BM_PRM_U;

/*
    人脸对比方式类型
*/
typedef enum frCPTypeE
{
    ONE_VS_ONE,                                     /* 一对一 */
    ONE_VS_N                                        /* 一对多 */
}FR_CP_TYPE_E;

/*活体检测及建模错误码定义*/
typedef enum frBMResultType
{
    RESULT_SUCCESS            = 0x00,               /* 建模成功 */
    RESULT_LIVEDET_NOLIVE_ERR = 0x10,               /* 活体检测为非活体 */
    RESULT_LIVEDET_NOSURE_ERR = 0x11,               /* 活体检测无法确定 */
    RESULT_ALIGN_WL_ERR       = 0x20,               /* 白光数据对齐失败 */
    RESULT_ALIGN_IR_ERR       = 0x21,               /* 红光数据对齐失败 */
    RESULT_BM_WL_ERR          = 0x30,               /* 白光获取模型数据失败 */
    RESULT_BM_IR_ERR          = 0x31,               /* 红光获取模型数据失败 */
    RESULT_SIM_COMPARE_ERR    = 0x40,               /* 相似度比对失败*/
    RESULT_FACEDET_IR_ERR     = 0x50,               /* 红光人脸检测失败 */
    RESULT_FACEDET_LD_ERR     = 0x51,               /* 活体第二路人脸检测失败 */
    RESULT_FACEDET_MATCH_ERR  = 0x52,               /* 双目人脸匹配失败 */

    RESULT_PIC_FACEDET_ERR    = 0x60,               /* 图片下发人脸检测失败 */
    RESULT_PIC_NUM0_ERR       = 0x61,               /* 图片下发人脸检测为0*/
    RESULT_PIC_EYEDIS_ERR     = 0x62,               /* 图片下发眼间距过小*/
    RESULT_PIC_JPG_DEC_ERR    = 0x63,               /* 图片下发解码失败*/
    RESULT_PIC_JPG_DOWN_ERR   = 0x64,               /* 图片下发降采样失败*/
    RESULT_PIC_RESIZE_ERR     = 0x65,               /* 图片下发resize失败*/
    RESULT_PIC_MA_ALIGN_ERR   = 0x66,               /* 图片下发数据对齐失败 */
    RESULT_PIC_BM_ERR         = 0x67,               /* 图片下发建模 失败*/
    RESULT_PIC_FORMAT_ERR     = 0x68,               /* 图片下发格式错误 */
    RESULT_PIC_DEC_DSPMEM_ERR = 0x69,               /* JPG是特殊的格式，无优化内存方法，超出DSP JPG解码分配的常用格式3通道YUV420最大内存*/

}FR_BM_RESULT_TYPE;

typedef struct frBMResultJPEGSt
{
    unsigned int   WLSize;                          /* 白光抓拍照jpg图片大小 */
    unsigned char* pWLAddr;                         /* 白光抓拍照jpg图片地址 */
    unsigned int   IRSize;                          /* 红光抓拍照jpg图片大小 */
    unsigned char* pIRAddr;                         /* 红光抓拍照jpg图片地址 */
    unsigned int   faceSize;                        /* 1VN出来抓拍照片中再抠图的大小--定制需要 */
    unsigned char* pfaceAddr;                       /* 1VN出来抓拍照片中再抠图的地址--定制需要 */
}FR_BM_RESULT_JPEG_ST;

/* 返回的图片的信息 */
typedef struct tagFrPicInfo
{
    unsigned char *paddr;                           /* 智能模块回传的图片        */
    unsigned int   size;                            /* 智能模块回传图片数据的长度 */
}FR_PIC_INFO;

/* 建模存图功能，保存送引擎的原始图片 */
typedef struct tagFrSavePicInfo
{
    FR_PIC_INFO                  visYuv;            /* 可见光YUV大图 */
    FR_PIC_INFO                  visBigJpeg;        /* 可见光jpg大图，1080P */
    FR_PIC_INFO                  visSmallJpeg;      /* 可见光jpg小图 */  
    FR_PIC_INFO                  irYuv;             /* 红外光YUV大图 */
    FR_PIC_INFO                  irBigJpeg;         /* 红外光jpg大图，1080P */
    FR_PIC_INFO                  irSmallJpeg;       /* 红外光jpg小图 */

}FR_SAVE_PIC_INFO;

/* 开启建模存图功能控制结构体 HOST_CMD_FR_SAVE_1VN_PIC */
typedef struct tagFrCtrlSavePic
{
    unsigned int                 enable;            /* 使能开关 */
    unsigned char                flag[8];           /* 0-8对应关系，0- 可见光YUV大图;1-可见光jpg大图，1080P;
                                                                    2- 可见光jpg小图;3-红外光YUV大图;
                                                                    4-红外光jpg大图，1080P;5-红外光jpg小图 */           
    unsigned char               reserve[32];        /* 备用 */

}FR_CTRL_SAVE_PIC;

/* 开启1vN回调编码功能控制结构体 */
typedef struct tagFrCtrlEncPic
{
    unsigned char                flag[2];    /* flag[0]- 可见光jpg编码;flag[1]-红外光jpg编码，（0开启，1关闭） */
}FR_CTRL_ENC_PIC;

/* 安全帽颜色 */
typedef enum frSafetyHelmetColourTypeE
{
    FR_SAFETY_HELMET_RED    = 0,                    /* 红色 */
    FR_SAFETY_HELMET_YELLOW    ,                    /* 黄色 */
    FR_SAFETY_HELMET_BLUE      ,                    /* 蓝色 */
    FR_SAFETY_HELMET_WHITE     ,                    /* 白色 */
    FR_SAFETY_HELMET_OTHER     ,                    /* 其他 */
}FR_SAFETY_HELMET_COLOUR_TYPE_E;

/*
    安全帽功能结构体
*/
typedef struct stFrSafetyHelmetCtrl
{
    int                          enable;            /* 是否开启安全帽功能: 0:关闭  1:开启*/
}FR_SAFETY_HELMET_CTRL;

/*

    安全帽功能返回结果
*/
typedef struct
{
    int                          isWear;            /* 是否佩戴安全帽: 0:否  1:是*/
    unsigned int                 colour;            /* 安全帽颜色 FR_SAFETY_HELMET_COLOUR_TYPE_E*/
    float                        colourTrust;       /* 颜色置信度 */
} FR_SAFETY_HELMET_RESULT;

/*
    过图片功能参数结构体
*/
typedef struct stFrInsertImagePrm
{
    unsigned char           type;                   /* 插入类型，0单图可见，1单图红外，2双图 */
    unsigned char           reserve[3];             /* reserve */
    unsigned char          *pFrmBuf[2];             /* 插入yuv图像的存储buffer */
    unsigned int            frmLen[2];              /* 插入yuv图像的存储长度 */
    unsigned short          width[2];               /* 插入yuv图像的宽 */
    unsigned short          height[2];              /* 插入yuv图像的高 */
}FR_INSERT_IMAGE_PRM;

/*
    过图片功能结构体
*/
typedef struct stFrInsertImageCtrl
{
    int                          enable;            /* 是否开启过图片功能: 0:关闭  1:开启*/
}FR_INSERT_IMAGE_CTRL;

/*
    自研活体补光阈值判断结构体
*/
typedef struct stFrFaceLdLightCtrl
{
    int                          flag;            /* 自研活体灯控标志位：0:未超过设定阈值，1:超过设置阈值 */
    unsigned char                reserve[16];
}FR_FACELD_LIGHT_CTRL;

/*
    自研活体存图路径结构体
*/
typedef struct stFrFaceLdSavPicDirInfo
{
    unsigned char                *picDirName;	/* 自定义路径 */
    unsigned char                reserve[64];
}FR_FACELD_SAV_PIC_DIR_INFO;


/*
    底库删除模型结构体
*/
typedef struct stFrDelModelPrm
{
    unsigned int                 idx;            /* 删除起始序号 */
    unsigned int                 cnt;            /* 连续删除个数 */
    unsigned char               *addr;           /* 底库首地址 */
    unsigned char                reserve[16];
}FR_DEL_MODEL_PRM;

/*
    底库查询和添加模型结构体
*/
typedef struct stFrOptModelPrm
{
    unsigned int                 idx;            /* 起始序号 */
    unsigned int                 cnt;            /* 连续个数 */
    unsigned char               *addr;           /* 底库首地址 */
    unsigned char               *modelAddr;      /* 查得/ ADD 的模型buf ,应用分配内存*/
    unsigned int                 modelBufLen;    /* 装模型的bufLen,应用输入*/
    unsigned  int                cmpEnable;      /* 查重功能使能 0->关闭 1->开启 */
    float                        threshold;      /* 相似度阈值 */
    int                          topId;          /* 最高相似度ID */
    int                          isSim;          /* 是否重复 0  ->不重复 1->重复 */
    unsigned char                reserve[12];
}FR_OPT_MODEL_PRM;

/*
    1VN 比对结构体
*/
typedef struct stFrCmpModelPrm
{
    unsigned char               *addr;           /* 底库首地址 */
    unsigned char               *modelAddr;      /* 查得/ ADD 的模型buf ,应用分配内存*/
    unsigned int                 modelBufLen;    /* 装模型的bufLen,应用输入*/
    unsigned int                 top1Id;         /* 底库命中的ID 号*/
    float                        top1Sim;        /* 底库命中的相似度*/
    unsigned char                reserve[16];
}FR_CMP_MODEL_PRM;


/*
    库比对模式
*/
typedef enum stFrFaceFeatreCpModeE
{
    DSP_FR_FACE_FEATRE_CP_MODE_SINGLE = 0,          /* 单库比对模式 */
    DSP_FR_FACE_FEATRE_CP_MODE_MULTI,               /* 多库比对模式，比对遍历多个N库 */
    DSP_FR_FACE_FEATRE_CP_MODE_MAX,
} FR_FACE_FEATRE_CP_MODE_E;

/* 1VN的几种人脸N库的模型属性 */
typedef enum stFrFaceFeatreCpAttrE
{
    DSP_FR_FACE_FEATRE_CP_ATTR_NORMAL = 0,          /* 普通人脸特征底库，只有一个的混合库也算普通库 */
    DSP_FR_FACE_FEATRE_CP_ATTR_MASK,                /* 有遮挡的人脸、戴口罩的人脸特征底库 */
    DSP_FR_FACE_FEATRE_CP_ATTR_MAX,
}FR_FACE_FEATRE_CP_ATTR_E;

/*
    STREAM_ELEMENT_FR_BM 建模数据返回结构体
*/
typedef struct _FR_BM_RESULT_
{
    int                          uiId;                                  /* ID号，返回应用下发 ID   */
    unsigned int                 bmResult;                              /* 是否建模成功   */
    unsigned int                 model_data_size;                       /* 模型数据大小   */
    unsigned char                model_data[FR_MAX_MODEL_DATA_LENTH];   /* 人脸模型数据   */

    float                        pitch;                                 /* 平面外上下俯仰角，人脸朝上为正  */
    float                        yaw;                                   /* 平面外左右偏转角，人脸朝左为正  */
    float                        roll;                                  /* 平面内旋转角，人脸顺时针旋转为正*/
    float                        eyedis;                                /* 两眼瞳孔距*/
    float                        visible;                               /* 能见无遮挡程度*/
    float                        faceDist;                              /* 人脸距离摄像头的距离信息*/
    float                        faceSocre;                             /* 人脸评分*/

    FR_CHARACT_POINTS            cPoints;                               /* 特征点信息*/

    FR_SAFETY_HELMET_RESULT      stSafetyHelmetResult;                  /* 安全帽功能信息*/

    unsigned int                 mask;                                  /* 戴口罩结果 0不戴口罩,1戴口罩,-1口罩结果不确定 */
    float                        maskConfidence;                        /* 戴口罩的置信度*/

    unsigned long                bmFNum;                                /* 建模这张的frame编号,用来提取预抓图像*/
    FR_BM_RESULT_JPEG_ST         stBMResultJpeg;                        /* 建模返回JPEG图片信息*/
    FR_BM_RESULT_TYPE            stBMResultType;                        /* 建模返回结果类型*/
    unsigned long long           picPts;                                /* 生成该建模图片的时间戳 */

    FR_SAVE_PIC_INFO             stSavePic;                             /* 存图功能图片信息，与正常事件上报区分 */

	float                        leftEyeOpen;                           /* 左眼开闭程度 */
	float                        rightEyeOpen;                          /* 左眼开闭程度 */
	float                        mouthOpen;                             /* 嘴巴开闭程度 */

	char                         glass;                                 /* 眼镜，0表示无眼镜，1表示戴眼镜，2表示带墨镜，-1表示未知 */
    char                         hat;                                   /* 帽子，0表示无帽，,1表示戴帽子，-1表示未知 */
    char                         beard;                                 /* 胡子，0表示，/小胡子，1表示大胡须，-1表示未知 */
    char                         gender;                                /* 性别，0表示女，1表示男，-1表示未知 */
    char                         race;                                  /*  */
    char                         nation;                                /*  */
    char                         express;                               /* 表情，0表示中性，1高兴，2惊讶, 3表示害怕，, 4表示厌恶, 5表示难过, 6表示愤怒，-1表示未知 */
    char                         age;                                   /* 年龄，范，0~99，小，1周岁的都认为0岁，大于等于99岁都认为99， */
    char                         ageStage;                              /* 年龄段，0 表示儿童 0-6岁， 1表示少年 7-17岁，2表示青年 18-40岁，3表示中年 41-65岁，4表示老年 >=66 */
    char                         hairStyle;                             /* 发型：预留 */
    char                         faceShape;                             /* 脸型：预留 */
	unsigned int                 faceTrkId;                             /* 人脸跟踪ID号*/    
    float                        liveTrust;                             /* 活体置信度*/
    char*                        pFileName;                             /* 文件名*/
    int                          liveType;                              /* 活体类型 0-关闭活体 1-白光活体 2-红光活体*/
    float                        clearSocre;                            /* 人脸清晰度评分*/
    float                        bm1vNSim;                               /* 图片下发流程1vN最高相似度*/
    unsigned int                 bm1vNId;                                /* 图片下发流程1vN最高相似度对应人员*/

    unsigned char                reserve[36];                           /* 备用 */
}FR_BM_RESULT;

typedef struct _FR_1VN_RESULT_
{
    FR_BM_RESULT                 stBmResult;                            /* 建模结果信息 */

    float                        fsimVisVsNir;                          /* 可见光和红外比对的相似度*/
    float                        top1Fsim;                              /* top1 命中的相似度*/
    unsigned int                 top1Id;                                /* top1 命中的ID 号*/
    unsigned int                 listVersion;                           /* 名单库版本信息*/
    unsigned int                 liveResult;                            /* 活体结果0-假体，1-真人，0xbad-未知 */
    float                        liveTrust;                             /* 研究院活体置信度 */

    /* 多库下有效: 当前抓拍图片和不同属性N库模型对比的TOP1相似度值，[0]=抓拍VS普通库, [1]=抓拍VS口罩库 */
    int                          featreCompareMode;                     /* 见 FR_FACE_FEATRE_CP_MODE_E 配置单库比对、还是多库比对*/
    float                        multiTop1Fsim[DSP_FR_FACE_FEATRE_CP_ATTR_MAX];
    unsigned int                 multitop1Id[DSP_FR_FACE_FEATRE_CP_ATTR_MAX];    
    unsigned int                 frmIdx;                                /*人脸识别的帧号*/
    char                         faceIdx;                               /*当前人脸是总人脸中的第几个*/
    char                         faceCnt;                               /*这一帧里面检测到人脸的总数*/

    unsigned char                liveMerge;                            /* 0->活体结果合并未启用；1->活体合并结果启用，使用新的变量 liveBody 传递结果 */
    int                          liveBody;                             /* 活体结果： 0->假体; 1->真人; 0xbad->未知 */
    unsigned char                reserve[21];                           /* 预留备用 */
    
}FR_1VN_RESULT;


/* HOST_CMD_FR_SIM_THRESHOLD 库及比对阈值配置 */
typedef struct _FR_1VN_SIM_THRESHOLD_
{
    /* 见 FR_FACE_FEATRE_CP_MODE_E 配置单库比对、还是多库比对*/
    int                         featreCompareMode;
    /* 见 FR_FACE_FEATRE_CP_ATTR_E 配置库的属性，单库模式下通过该变量指定比对的库，多库模式下该属性暂时不起作用 */
    int                         featreCompareAttr; 

    /* 配置不同属性库的阈值，[抓拍图][底库]: [0][0]=普通VS普通库, [1][1]=口罩VS口罩库, [0][1]=普通VS口罩库, [1][0]=口罩VS普通库 */
    float                       simThreshold[DSP_FR_FACE_FEATRE_CP_ATTR_MAX][DSP_FR_FACE_FEATRE_CP_ATTR_MAX];

    unsigned char               reserve[32];
}FR_1VN_SIM_THRESHOLD;

typedef struct tagDfrPicInfoSt
{
    unsigned int uiWitdh;                           /* 人脸图像宽度     */
    unsigned int uiHeight;                          /* 人脸图像高度     */
    unsigned int uiLen;                             /* 人脸图像数据长度 */
}DFR_PIC_INFO_ST;

/*
    STREAM_ELEMENT_FR_DEC 活体检测结果返回结构体
*/
typedef struct tagFrDetResultSt
{
    int                 idx;                        /* 唯一标识符，直接返回应用下发ID */
    unsigned int        detectionResult;            /* 是否是活体 */
    unsigned int        ifIrFace;                   /* 是否红外图像检测到人脸 */

    DFR_PIC_INFO_ST     stIrFacePicInfo;            /* 红外抠出来的人脸图像信息   */
    DFR_PIC_INFO_ST     stWlFacePicInfo;            /* 可见光抠出来的人脸图像信息 */

    DFR_PIC_INFO_ST     stIrPicInfo;                /* 红外原始的图像信息   */
    DFR_PIC_INFO_ST     stWlPicInfo;                /* 可见光原始的图像信息 */

    FR_CHARACT_POINTS   irCPts;                     /* 红外的特征点坐标*/
    unsigned int        tLen;                       /* 红外和可见光数据的总长*/
}FR_DET_RESULT;

/* 人脸姿态信息 */
typedef struct tagFrFacePostureInfo
{
    float             clearity;                     /* 清晰度评分 */
    float             frontal_conf;                 /* 正面评分   */
    float             pitch;                        /* 平面外上下俯仰角，人脸朝上为正  */
    float             yaw;                          /* 平面外左右偏转角，人脸朝左为正  */
    float             roll;                         /* 平面内旋转角，人脸顺时针旋转为正*/
    float             eyedis;                       /* 两眼瞳孔距*/    
    float             reserve[64];                 /* 预留字段 */
}FR_FACE_POSTURE_INFO;

/*
    STREAM_ELEMENT_FR_FFD_IQA 人脸检测 对比 返回结构体
*/
typedef struct tagFrFfdIqaSingle
{
    int                     idx;                    /* 唯一标识符，直接返回应用下发ID */
    unsigned int            ifface;                 /* 是否检测到人脸    */
    FR_CHARACT_POINTS       cPoints;                /* 特征点信息*/
    FR_FACE_POSTURE_INFO    stFPInfo;               /* 检测到的人脸姿态信息 */
    FR_PIC_INFO             stFdPicInfo;            /* 检测到的人脸所对应的图片 */

    unsigned long           fdFNum;                 /* 人脸检测的frame编号   */
    unsigned int            irLength;               /* 灰度图大小            */
    unsigned int            beFirstFFD;             /* ==1时为移动侦测前的第一次人脸检测 */    
    unsigned char           reserve[32];           /* 预留字段 */
}FR_FFD_IQA_SINGLE;


/*
    STREAM_ELEMENT_FR_FFD_IQA 人脸检测 对比 返回结构体
*/
typedef struct tagFrFfdIqaResult
{
    int               idx;                          /* 唯一标识符，直接返回应用下发ID */
    unsigned int      facenum;                      /* 检测到的人脸数量    */

    FR_FFD_IQA_SINGLE stFFdIqaResult[FR_MAX_FFD_FACE_NUM];   /* 检测到的人脸信息 */
    FR_PIC_INFO       stFrWlPic;                    /* 可见光图片       */
    FR_PIC_INFO       stFrIrPic;                    /* 红外光图片       */    
    unsigned char     reserve[32];                 /* 预留字段 */
}FR_FFD_IQA_RESULT;

/*
    DSP 人脸检测的类型
*/
typedef enum
{
    FR_FD_ONESHOT = 0,                              /* DSP 被动式检测 */
    FR_FD_CONTINUE,                                 /* DSP 主动式检测 */
    FR_FD_ONESHOT_JPG,                              /* DSP JPEG格式人脸检测返回坐标框 */

    FR_FD_CONTINUE_NO_BM,                           /* DSP 主动式检测,但不返回建模数据 */
    FR_FR_INVALID                                   /* DSP 无效的检测 停止人脸检测 */
}FR_FD_TYPE;

/* 
	下发图片的格式，JPG/BMP/PNG 可以统一使用 DSP_FR_DOWNLOAD_JPEG  
*/
typedef enum frDataTypeE
{
    DSP_FR_DOWNLOAD_JPEG        = 0x100200,         /* 来源为 下发 JPG/BMP/PNG  图片  */
    DSP_FR_DOWNLOAD_YUV,                            /* 来源为 下发 YUV  数据  */
    DSP_FR_DOWNLOAD_COMP,                           /* 来源为 下发 COMP 图片,生物秘钥使用   */
    DSP_FR_DOWNLOAD_PIONS,                          /* 来源为 下发 图片 + 特征点信息 */
    
    DSP_FR_MAX_TYPE,
}FR_DATA_TYPE_E;

/* 本地检测的白天、黑夜模式 */
typedef enum frDetTypeE
{
    FR_DET_TYPE_WL = 0,
    FR_DET_TYPE_IR,
    FR_DET_TYPE_MAX,
}FR_DET_TYPE_E;

/* 要求回调的图片类型 */
typedef enum
{
    FR_BM_CB_TYPE_JPEG   = 0x00,                    /* 回调证件照jpeg图和模型 */
    FR_BM_CB_TYPE_COMP,                             /* 回调证件照的生物秘钥数据和模型 */
    FR_BM_CB_TYPE_JPEGONLY,                         /* 回调证件照的jpeg图像  */
    FR_BM_CB_TYPE_BMPONLY,                          /* 回调证件照的bmp图像  */
    FR_BM_CB_TYPE_MAX,                              /* 无效值 */
} FR_BM_CB_TYPE_E;

/*
    HOST_CMD_FFD_CTRL 人脸检测信息用 STREAM_ELEMENT_FR_FFD_IQA 回调 人脸检测结果与评分
*/
typedef struct frFfdPrmSt
{
    int                     idx;                    /* 回调索引 */
    FR_FD_TYPE              fdType;                 /* DSP 人脸检测的类型 */
    unsigned int            fdnum;                  /* 多人脸检测的人脸数量 取值0与1时，DSP 只检测一个人脸 */
    unsigned int            solid;                  /* 是否是立体的       */
    FR_DET_TYPE_E           eDataType;              /* 数据来源类型       */
    FR_BM_PRM_U             uDataInfo;              /* 数据内容           */
    FR_FACE_POSTURE_INFO    stFPInfo;               /* 人脸姿态、眼间距的过滤 */

    float                   fdConfidence;           /* 人脸检测的置信度 -1 为无效使用默认值或者上次设置的值 */
    unsigned int            maskEnable;             /* 戴口罩使能，0是关闭，1是开启，默认关闭 */
    unsigned int            depthEnable;            /* 测距功能开关，0关闭，1开启*/
    FR_INSERT_IMAGE_PRM     stInsertImgPrm;         /* 过图片功能数据信息 */
    FR_CTRL_ENC_PIC         stEncPicCtrl;           /* 可见、红外编码开关 */
    unsigned char           faceCrop;               /* 人脸抠图JPG送应用,0是关闭，1是开启，默认关闭*/
    unsigned char           faceDraw;               /* 原始图上绘制人脸框后JPG,0是关闭，1是开启，默认关闭*/
    unsigned int           	faceTrack;             	/* 人脸跟踪功能开关，默认关闭，0->关闭，1->打开 */

    //unsigned char           distFlag;				/* 使用识别距离时的开关，0->使用眼间距stFPInfo.eyedis,1->使用距离recogdist */
    //float           		recogdist;              /* 识别距离，大于0时启用，等于0使用eyedis */
    unsigned char           reserve[60];             /* 预留字段 */
}FR_FFD_PRM_S;

/*
    建模数据来源类型
*/
typedef enum frOpTypeE
{
    FR_REGISTER,                                    /* 建模注册         */
    UN_REGISTER,                                    /* 去注册           */
    UN_REGISTER_LIST,                               /* 整个库删除掉     */
    NO_REGISTER                                     /* 不做注册相关操作 */
}FR_OP_TYPE_E;

/*
    活体连续检测功能控制信息
*/
typedef struct _FR_LIVTEST_CTRL_
{
    unsigned int ctrl;                              /* 是否开启活体检测 */
    unsigned int time;                              /* 检测时间 */
}FR_LIVTEST_CTRL;

/* 建模命令字对应的信息 HOST_CMD_FR_BM */
typedef struct frBuildModelPrmSt
{
    int                     idx;                    /* 唯一标识符，由应用下发ID，回调处理直接返回 */
    FR_OP_TYPE_E            eOpType;                /* 建模的同时也同时完成对算法黑白名单库注册 or 去注册 */
    FR_DATA_TYPE_E          eDataType;              /* 建模数据来源类型  */
    FR_BM_PRM_U             uBMPrm;                 /* 数据来源类型对应的具体信息 */

    FR_LIVTEST_CTRL         stLive;                 /* 活体功能控制      */
    
    int                     idPhoto;                /*0预检，1解码建模*//* 是否返回证件照， 0-直接使用下发图片做人脸检测建模 1-下发图片转换成证件照再建模 */  
    int                     enableQs;               /* 使能人脸评分功能 0-不使能人脸评分 1-使能人脸评分功能*/
    int                     maskEnable;             /* 使能口罩检测功能 0-disable 1-enable */
    
    FR_BM_CB_TYPE_E         eCbType;                /* 回调图像类型 */
    
    FR_CHARACT_POINTS       frPoins;                /* 人脸框关键点信息 */
    void                    *priv;                  /* 私有信息 */
    unsigned int            privLen;                /* 私有信息长度 */
    int                     bm1vNEnable;            /* 图片下发流程中1vN使能 0-不使能 1-使能 */

    unsigned char           reserve[28];            /* 预留字段 */
}FR_BUILD_MODEL_PRM_ST;

/*
    人脸模型对比 HOST_CMD_FR_COMPARE
*/
typedef struct frCompareSt
{
    int             idx;
    unsigned char   baseModel[1536];
    unsigned int    baseModelLen;
    unsigned char   camModel[1536];
    unsigned int    camModelLen;
    float           similarity;
} FR_COMPARE_ST;

/*
    人脸模型校验 HOST_CMD_FR_CHECK_MODEL
*/
typedef struct frModelCheckSt
{
    unsigned char   model[1536];
    unsigned int    modelLen;
    unsigned int    modelFit;                       /*0表示符合1表示不符合 2表示未处理(入参不正确)*/
} FR_MODEL_CHECK_ST;

/*
    活体检测方式类型
*/
typedef enum frLDTypeE
{
    ONE_SHOT,                                       /* 单次检测 */
    CONTINUOUS                                      /* 多次检测 */
}FR_LD_TYPE_E;


/* 人脸 ROI 检测区域，参数为百分比的分子数，需要设置为不镜像的图像坐标 HOST_CMD_FR_ROI */
typedef struct stFrRoiRectSt
{
    unsigned int        roiEnable;                  /* ROI 使能开关 */
    float               x;                          /* 参数设置为图像归一化后的百分比 */
    float               y;                          /* 偏移坐标 y */
    float               width;                      /* 偏移宽度   */
    float               height;                     /* 偏移高度   */
}FR_ROI_RECT;


/* 名单库内存同步 */
typedef struct
{
    unsigned int   eleCnt;                          /* 名单库里元素的个数 */
    unsigned int   elePadCnt;                       /* 名单库里补充元素的个数一般1000 */
    unsigned int   eleAllCnt;                       /* 名单库里所有元素的个数 */
    unsigned int   featLen;                         /* 名单库里每个元素中人脸模型的长度 */
    unsigned int   addInfoLen;                      /* 名单库里每个元素中应用附加的长度 */
    unsigned int   eleSize;                         /* 名单库里每个元素的内存大小 */
    unsigned int   memSize;                         /* 名单库里的总内存大小 */
    unsigned int   headSize;                        /* 名单库头的结构体大小 */

    unsigned int   poolVersion;                     /* 名单库版本号，应用层填写，引擎读取 */
    unsigned char* virtHead;                        /* 名单库里头的虚首地址 */
    unsigned char* physHead;                        /* 名单库里头的物理首地址 */
    unsigned char* virtPool;                        /* 名单库里ele的虚首地址 */
    unsigned char* physPool;                        /* 名单库里ele的物理首地址 */
    unsigned char  reserve[12];                     /* 名单库里的保留字段 */
}FACE_MODEL_POOL_ST;

/* 名单库管理函数 */
typedef struct tagFacePoolCnt
{
    int             poolFeatType;                   /* 名单库的类型: 不同的类型表示不同的名单N库。使用 FR_FACE_FEATRE_CP_ATTR_E 的类型值 */
    void*           virtAddr;
    void*           poolLockFunc;                   /* 名单库锁函数 */
    void*           poolUnlockFunc;                 /* 名单库解锁函数 */
    void*           flushCacheFunc;                 /* 名单库刷cache,写回内存函数 */
}FACE_POOL_PRM;

/* 几种人脸抠图业务类型 */
typedef enum stFrFaceCropTypeE
{
    DSP_FR_FACE_CROP_TYPE_NONE,
    DSP_FR_FACE_CROP_DFR_FD,                        /* 人脸检测成功做人脸抠图 */
    DSP_FR_FACE_CROP_DFR_BM,                        /* 人脸建模成功做人脸抠图 */
    DSP_FR_FACE_CROP_DOWNLOAD_PIC,                  /* 对下发图片做人脸抠图 */
    DSP_FR_FACE_CROP_TYPE_INVALID,
}FR_FACE_CROP_TYPE_E;

/* 使用的抠图方法 */
typedef enum stFrFaceCropMethodE
{
    DSP_FR_FACE_CROP_METHOD_NONE,
    DSP_FR_FACE_CROP_DYNAMIC_RATIO,                 /* 人脸图片按照人脸宽高设置比例 */
    DSP_FR_FACE_CROP_FIXED_RATIO,                   /* 人脸图按照配置比例方式输出 */
    DSP_FR_FACE_CROP_FIXED_HEIGHT,                  /* 人脸图按照固定高度抠图输出 */
    DSP_FR_FACE_CROP_FIXED_WH,                      /* 人脸图按照固定分辨率输出 */
    DSP_FR_FACE_CROP_METHOD_INVALID,
}FR_FACE_CROP_METHOD_E;

/* 设置人脸抠图 HOST_CMD_FR_FACE_CROP */
typedef struct stFrFaceCrop
{
    unsigned int        cropType;                   /* 使用 FR_FACE_CROP_TYPE_E 指定的类型 */
    unsigned int        cropEnable;                 /* 人脸抠图使能开关 使能1  关闭0 */
    unsigned int        cropMethod;                 /* 使用 FR_FACE_CROP_METHOD_E 指定的类型 */

    /* cropMethod = DSP_FR_FACE_CROP_DYNAMIC_RATIO 时配置下面的参数 */
    float               totalfaceWidthRatio;        /* 抠图宽度使用人脸宽度的比例系数 */
    float               upperPartHeightRatio;       /* 抠图高度使用人脸宽度的比例系数 */
    float               underPartHeightRatio;       /* 抠图的身体部分使用人脸高度的比例系数 */

    /* cropMethod = DSP_FR_FACE_CROP_FIXED_RATIO 时配置下面的参数 */
    float               faceWidthRatio;             /* 抠图宽度使用人脸宽度的比例系数 */
    float               faceHeightRatio;            /* 抠图高度使用人脸宽度的比例系数 */

    /* cropMethod = DSP_FR_FACE_CROP_FIXED_HEIGHT 时配置下面的参数 */
    int                 fixedHeight;                /* 抠图固定高度 */

    /* cropMethod = DSP_FR_FACE_CROP_FIXED_WH 时配置下面的参数 和 fixedHeight */
    int                 fixedWeight;                /* 抠图固定高度 */
}FR_FACE_CROP;

/*
    获取相似度比对阈值
    HOST_CMD_FR_GET_SIM_THRESHOLD
*/
typedef struct frSimThresholdPrmSt
{
    float           simThreshold[FR_DET_TYPE_MAX][DSP_FR_FACE_FEATRE_CP_ATTR_MAX];    /* 推荐比对阈值：simThreshold[0]--白天模式阈值；simThreshold[1]黑夜模式阈值
                                                                                         simThreshold[0][n] : 白天模式：0--非口罩阈值，1--口罩阈值
                                                                                         simThreshold[1][n] : 黑夜模式：0--非口罩阈值，1--口罩阈值 */
    float           simThreshold1v1;                                                  /* 1v1 推荐比对阈值 */
    unsigned char   reserve[128];                                                     /* 备用 */
}FR_SIM_THRESHOLD_PRM_ST;

/****************************************************************************/
/*                      虹膜智能模块                                        */
/****************************************************************************/
#define IRIS_MAX_MODEL_DATA_LENTH      (4096)

/* 虹膜模块能力集 */
typedef struct
{
    unsigned int modelSize;                          /* 单个虹膜模型大小 */
    unsigned int supportModelNum;                    /* 算法支持的虹膜模板库最大容量 */
    unsigned int issupportRepeat;                    /*算法是否支持查重 */
    float        threshold1v1;                       /* 1V1参考阈值 */
    float        threshold1vN;                       /* 1VN参考阈值 */
	unsigned int eyeNum1vn;							 /* 识别眼睛数量 */
	unsigned int curEyeNum; 						 /* 当前虹膜容量 */

    unsigned char reserve[24];                       /* 保留字段 */
} IRIS_ABILITY;

/* 返回的图片的信息 */
typedef struct
{
    unsigned char          *pAddr;                  /* 虹膜模块回传的图片 */
    unsigned int            size;                   /* 虹膜模块回传图片数据的长度 */

    unsigned char          reserve[32];             /* 保留字段 */
}IRIS_BUF_INFO;

/* 用于虹膜图像检测和采集过程中的人工/程序自标注 */
typedef struct
{
    unsigned char           isEnable;               /* 标注使能开关 */
    unsigned char           eyeType;                /* 眼睛类型: 0-正常，1-眼镜，2-美瞳，3-透明隐形 */
    unsigned char           cbPicEnable;            /* 是否开启回调图片 */
    unsigned char          *pLabel;                 /* 对虹膜图像进行字符标注，长度限制在 1024字节 */
    unsigned int            labelLen;               /* 标注的字符串长度 */
    unsigned int            totalCnt;               /* 单位周期内采集的数量, 通常不用填写 */

    unsigned char           reserve[32];            /* 保留字段 */
} IRIS_LABEL_INFO;

/* 虹膜图像内存信息 */
typedef struct
{
    IRIS_BUF_INFO           facePic;                /* 人脸图像，用于人脸虹膜联动业务*/
    IRIS_BUF_INFO           srcIrisPic;             /* 虹膜脸部图像 */
    IRIS_BUF_INFO           irisPic[2];             /* 虹膜图像 */

    unsigned char           reserve[32];            /* 保留字段 */
} IRIS_PIC_INFO;

/* 虹膜关键点信息 */
typedef struct
{
    REGION_INFO             facePoint;              /* 人脸框位置区域 */
    REGION_INFO             srcIrisPoint[2];        /* 虹膜脸部图像中人眼位置区域*/
    REGION_INFO             irisPoint[2];           /* 虹膜图像中人眼位置区域 */

    unsigned char           reserve[32];            /* 保留字段 */
} IRIS_POINTS_INFO;

/* 虹膜图像各阶段耗时统计 */
typedef struct
{
    unsigned long           detectTime;             /* 人眼检测+裁剪等环节消耗的时间 */
    unsigned long           enrollTime;             /* 人眼质量判断+建模+选帧等环节消耗的时间 */
    unsigned long           matchTime;              /* 人眼质量判断+建模+比对等环节消耗的时间 */
    unsigned long           bmTime;                 /* 图片下发建模的时间 */

    unsigned char           reserve[32];            /* 保留字段 */
} IRIS_TIME_COUNT ;

/* 虹膜检测类型 */
typedef enum
{
    IRIS_DETECT_TYPE_CAPT_1VN = 1,                  /* 一直进行虹膜检测识别的1VN流程 */
    IRIS_DETECT_TYPE_CAPT_ENROLL,                   /* 一直进行虹膜检测的注册流程 */
    IRIS_DETECT_TYPE_CAPT_ENROLL_REPEAT,            /* 查重，预留 */
    IRIS_DETECT_TYPE_CAPT_STOP,                     /* 停止虹膜注册or识别*/
    IRIS_DETECT_TYPE_CAPT_INVALID                   /* 无效，不使用*/
} IRIS_DETECT_TYPE;

/* 开启建模存图功能控制结构体 HOST_CMD_FR_SAVE_1VN_PIC */
typedef struct tagIrisCtrlSavePic
{
    unsigned int                 detEnable;            /* 人眼检测存图使能开关 */
    unsigned int                 detFlag;              /* 对应关系:0X01- 虹膜原图jpg; 0X02-人眼图bmp，VGA; 0X03-都存*/
    unsigned int                 matchEnable;          /* 1VN存图使能开关 */
    unsigned int                 matchFlag;            /* 对应关系:0X01- 虹膜原图jpg; 0X02-人眼图bmp，VGA； 0X03-虹膜原图jpg与人眼图bmp； 0X04-可见光图，JPG; 0X07-都存*/
                                                                   
    unsigned char                reserve[32];          /* 备用 */
}IRIS_CTRL_SAVE_PIC;

/*  HOST_CMD_IRIS_DETECT 虹膜检测信息参数配置 */
typedef struct
{
    int                     sequence;               /* 唯一标识符，直接返回应用下发主机命令HOST_CMD_IRIS_DETECT的ID */
    unsigned int            eyeNum;                 /* 进行单/双眼注册or识别 */
    IRIS_DETECT_TYPE        detectType;             /* 检测类型，见 IRIS_DETECT_TYPE */
    IRIS_LABEL_INFO         label;                  /* 虹膜检测是否需要开启标注信息 */

    unsigned char           reserve[32];            /* 保留字段 */
} IRIS_DETECT_PRMAM;

/* STREAM_ELEMENT_IRIS_IQA 虹膜检测回调结构体 */
typedef struct
{
    int                     sequence;               /* 唯一标识符，直接返回应用下发主机命令HOST_CMD_IRIS_DETECT的ID */
    unsigned int            frameId;                /* 该图像对应的帧ID*/
    unsigned int            eyeNum;                 /* 检测到的人眼数量 */
    unsigned int            distance;               /* 该图像对应的距离信息  */
    unsigned long long      pts;                    /* 该图像对应的时间戳 */
    int                     gray[2];                /* 两只眼睛的灰度值 */
    IRIS_PIC_INFO           picInfo;                /* 图像的详细信息 */
    IRIS_POINTS_INFO        points;                 /* 虹膜关键点信息 */
    IRIS_TIME_COUNT          timeCount;             /* 该图像对应的耗时统计 */
    IRIS_LABEL_INFO         label;                  /* 该结果仅仅在检测开启了标注信息后有效 */

    unsigned char           reserve[32];            /* 保留字段 */
} IRIS_IQA_RESULT;

/* 虹膜图像错误码 */
typedef enum
{
    IRIS_STATUS_SUCCESS         = 0,            /* 处理成功 */

    IRIS_DETECT_ERR             = 0x10,         /* 虹膜检测失败 */
    IRIS_SIM_COMPARE_ERR        = 0x20,         /* 1V1比对失败*/
    IRIS_BUILD_ERR              = 0x30,         /* 建模失败*/
    IRIS_PIC_DEC_ERR            = 0x40,         /* 图片下发解码失败*/
    IRIS_PIC_FORMAT_ERR         = 0x41,         /* 图片下发格式错误 */
    IRIS_PIC_RES_ERR            = 0x42,         /* 图片下发分辨率错误 */
    IRIS_QUALITY_ERR            = 0x50,         /* 虹膜图像质量不合格 */
    IRIS_QUALITY_BOUNDS_ERR     = 0x51,         /* 虹膜出界 */
    IRIS_QUALITY_VISIBILITY_ERR = 0x52,         /* 虹膜可见度低于阈值（20）*/
    IRIS_QUALITY_PUPILRATIO_ERR = 0x53,         /* 瞳孔比例高于阈值（70）*/
    IRIS_QUALITY_BRIGHT_ERR     = 0x54,         /* 图像过亮*/
    IRIS_QUALITY_CLARITY_ERR    = 0x55,         /* 清晰度低于阈值（40）*/
    IRIS_QUALITY_GAZE_ERR       = 0x56,         /* 斜视度高于阈值（90）*/
    IRIS_QUALITY_RATIO_ERR      = 0x57,         /* 虹膜直径/瞳孔直径过小*/
    IRIS_QUALITY_COMPOSITE_ERR  = 0x58,         /* 图像质量加权得分低于阈值（47）*/
    IRIS_QUALITY_BRIGHTSPOT_ERR = 0x59,			/* 虹膜区域光斑太大 */
    IRIS_1VN_DISTANCE_FAR       = 0x70,         /* 1VN识别距离太远无法检测到眼睛 */
    IRIS_1VN_DISTANCE_CLOSE     = 0x71,         /* 1VN识别距离太近无法检测到眼睛 */
    IRIS_LIVEDET_NOLIVE_ERR     = 0x90,         /* 活体检测为非活体 */
    IRIS_LIVEDET_NOSURE_ERR     = 0x91,         /* 活体检测无法确定 */
} IRIS_STATUS_TYPE;

/* 虹膜图像质量参数 */
typedef struct
{
    unsigned int            textureFocus;           /* 人眼区域清晰度 */
    unsigned int            visualDegree;           /* 眼睛张合度 */
    unsigned int            gaze;                   /* 斜视度 */
    unsigned int            foucs;                  /* 聚焦度：虹膜区域的对比度 */
    unsigned int            move;                   /* 运动模糊 */
    unsigned int            ratio;                  /* 虹膜与瞳孔的比例 */
    unsigned int            composite;              /* 虹膜注册时的综合评分 */
    unsigned int            centreX;                /* 虹膜中心 x坐标 */
    unsigned int            centreY;                /* 虹膜中心 Y坐标 */
    unsigned int            irisDiam;               /* 虹膜直径 */
    unsigned int            pupilDiam;              /* 瞳孔直径 */

    unsigned char           reserve[32];            /* 保留字段 */
} IRIS_SCORE_PARAM;

/* 单张虹膜图像注册&&1V1比对信息 */
typedef struct
{
    IRIS_STATUS_TYPE        statusCode;                       /* 虹膜图像错误码 */
    unsigned int            eyeFlag;                          /* 右眼标识，0：左眼，1：右眼*/
    unsigned int            frameId;                          /* 该图像对应的帧ID*/
    unsigned int            distance;                         /* 该图像对应的距离信息  */
    unsigned long long      pts;                              /* 该图像对应的时间戳 */
    float                   top1Sim;                          /* top1 命中的相似度 */
    unsigned int            top1Id;                           /* top1 命中的ID 号 */
    unsigned char           model[IRIS_MAX_MODEL_DATA_LENTH]; /* 虹膜模型数据 */
    unsigned int            modelSize;                        /* 模型数据大小，注册：256 Byte，其他：1536 Byte */
    IRIS_SCORE_PARAM        picParam;                         /* 虹膜图像质量参数 */
    IRIS_BUF_INFO           irisPic;                          /* 虹膜图像 */
    REGION_INFO             irisPoint;                        /* 虹膜图像中人眼位置区域 */
    IRIS_TIME_COUNT         timeCount;                        /* 该图像对应的耗时统计 */
	
    unsigned char           reserve[32];                      /* 保留字段 */
}IRIS_EYE_INFO;

/* STREAM_ELEMENT_IRIS_1VN_SIM 虹膜 注册 or 1VN比对 回调结构体 */
typedef struct
{
    int                     sequence;               /* 唯一标识符，直接返回应用下发ID */
    IRIS_EYE_INFO           eyeData[2];             /* 虹膜信息 [0]:左眼， [1]：右眼*/
    IRIS_BUF_INFO           facePic;                /* 人脸图像，用于人脸虹膜联动业务*/
    IRIS_BUF_INFO           srcIrisPic;             /* 虹膜脸部图像 */
	REGION_INFO             facePoint;              /* 人脸框位置区域 */

    unsigned char           reserve[32];            /* 保留字段 */
}IRIS_1VN_RESULT;

/* HOST_CMD_IRIS_BM 虹膜图片下发建模参数配置 */
typedef struct
{
    int                     sequence;               /* 唯一标识符，由应用下发ID，回调处理直接返回 */
    unsigned int            modelFlag;              /* 1：获取虹膜特征值（1536 Byte）；0：获取虹膜模板值（256 Byte）；2：两个都要，顺序：256+1536 */
    unsigned char          *imgAddr;                /* 下发图片地址 */
    unsigned int            imgAize;                /* 下发图片大小 */

    unsigned char           reserve[32];            /* 保留字段 */
} IRIS_BUILD_MODEL_PRM;

/* 单张虹膜图像建模信息 */
typedef struct
{
    IRIS_STATUS_TYPE        statusCode;             /* 虹膜图像错误码 */
    unsigned int            modelSize;              /* 模型数据大小   */
    unsigned char           model[IRIS_MAX_MODEL_DATA_LENTH]; /* 虹膜模型数据 */
    IRIS_BUF_INFO           irisPic;                /* 虹膜图像 */
    IRIS_SCORE_PARAM        score;                  /* 虹膜图像参数 */

    unsigned char           reserve[32];            /* 保留字段 */
}IRIS_BM_DATA;

/* STREAM_ELEMENT_IRIS_BM 虹膜图片下发建模回调结构体 */
typedef struct
{
    int                     sequence;               /* 唯一标识符，由应用下发ID，回调处理直接返回 */
    IRIS_TIME_COUNT         timeCount;              /* 该图像对应的耗时统计 */
    IRIS_BM_DATA            irisData;               /* 虹膜图像信息 */

    unsigned char           reserve[32];            /* 保留字段 */
}IRIS_IMG_BM_RESULT;

/* HOST_CMD_IRIS_COMPARE 虹膜 1V1 比对参数配置 */
typedef struct
{
    int                     sequence;                                 /* 唯一标识符，由应用下发ID，回调处理直接返回 */
    unsigned char           firstModel[IRIS_MAX_MODEL_DATA_LENTH];    /* 第一个模型数据 */
    unsigned int            firstModelLen;                            /* 第一个模型长度 */
    unsigned char           secondModel[IRIS_MAX_MODEL_DATA_LENTH];   /* 第二个模型数据 */
    unsigned int            secondModelLen;                           /* 第二个模型长度 */
    float                   similarity;                               /* 返回的相似度 */

    unsigned char           reserve[32];                              /* 保留字段 */
} IRIS_COMPARE_PRM;

/* 名单库内存同步 */
typedef struct
{
    unsigned int   eleCnt;                          /* 名单库里元素的个数 */
    unsigned int   elePadCnt;                       /* 名单库里补充元素的个数 */
    unsigned int   eleAllCnt;                       /* 名单库里所有元素的个数 */
    unsigned int   featLen;                         /* 名单库里每个元素中虹膜模型的长度 */
    unsigned int   addInfoLen;                      /* 名单库里每个元素中应用附加的长度 */
    unsigned int   eleSize;                         /* 名单库里每个元素的内存大小 */
    unsigned int   memSize;                         /* 名单库里的总内存大小 */
    unsigned int   headSize;                        /* 名单库头的结构体大小 */

    unsigned int   poolVersion;                     /* 名单库版本号，应用层填写，引擎读取 */
    unsigned char* virtHead;                        /* 名单库里头的虚首地址 */
    unsigned char* physHead;                        /* 名单库里头的物理首地址 */
    unsigned char* virtPool;                        /* 名单库里ele的虚首地址 */
    unsigned char* physPool;                        /* 名单库里ele的物理首地址 */
    unsigned char  reserve[12];                     /* 名单库里的保留字段 */
}IRIS_MODEL_POOL_ST;


/* 模板库管理函数 */
typedef struct
{
    void                   *virtAddr;               /* 模板库地址  */
    void                   *poolLockFunc;           /* 模板库锁函数 */
    void                   *poolUnlockFunc;         /* 模板库解锁函数 */
    void                   *flushCacheFunc;         /* 模板库刷cache,写回内存函数 */

    unsigned char          reserve[32];             /* 保留字段 */
}IRIS_POOL_PRM;

typedef enum
{
    IRIS_CALIBRATE_SUCESS = 1,    /* vcm 标定成功 */
    IRIS_CALIBRATE_TIMEOUT = 2,   /* vcm 标定超时 */
    IRIS_CALIBRATE_MACRO_FAIL = 3,    /* vcm 近距标定失败 */
    IRIS_CALIBRATE_INF_FAIL = 4,      /* vcm 远距标定失败 */
    IRIS_CALIBRATE_CHECK_ERROR = 5,   /* vcm 检查错误 */
    IRIS_CALIBRATE_AE_FAIL = 6,   /* AE 检查错误 */
    IRIS_CALIBRATE_SAVE_FAIL = 7,  /* 保存文件失败 */
} IRIS_STATUS_CALIBRATE;

/* IRIS_VCM_Calibarte_RESULT 虹膜VCM标定回调结构体 */
typedef struct
{
    IRIS_STATUS_CALIBRATE   calibrateStatus;        /* 标定状态码 */
    unsigned char           reserve[32];            /* 保留字段 */
}IRIS_VCM_CALIBARTE_RESULT;

/*  HOST_CMD_IRIS_ISP_PRM 虹膜自研ISP参数配置 */
typedef struct
{
    unsigned int            slopeAngle;             /* 设备放置状态 0-垂直	1-倾斜15度       */
    unsigned char           reserve[32];            /* 保留字段 */
} IRIS_ISP_PRMAM;


/****************************************************************************/
/*                      TOF模块                                        */
/****************************************************************************/
/* TOF温度 */
typedef struct
{
    unsigned int sensorTemp;                        /* TOF Sensor温度 */
    unsigned int driveTemp;                    	    /* TOF Driver温度 */
	
	unsigned char          reserve[32];             /* 保留字段 */
} TOF_TEMPERATURE;

/****************************************************************************/
/*                      微智能模块: 移动侦测与遮挡、二维码                  */
/****************************************************************************/
/* 移动侦测设置 */
#define MD_BLOCK_W              (32)                /* 块宽度     */
#define MD_BLOCK_H              (32)                /* 块高度     */
#define MD_BLOCK_CNT            (704 / MD_BLOCK_W)  /* 每行块个数 */
#define MD_MAX_LINE_CNT         (576 / MD_BLOCK_H)  /* 最大的行数 */
#define MD_MAX_LEVEL            (6)                 /* 灵敏度级别，最大为6(灵敏度最低)，最小为0(灵敏度最高) */

/* 遮挡报警设置 */
#define CD_MAX_LEVEL            (15)

/* 屏蔽设置 */
#define MASK_MAX_REGION         (4)                 /* 最多支持区域个数 */
#define MASK_ALIGN_H            (8)                 /* W、X对齐 */
#define MASK_ALIGN_V            (2)                 /* H、Y对齐 */

/*
    移动侦测的配置信息  HOST_CMD_SET_MD
*/
typedef struct
{
    unsigned int level;                             /* 级别0(灵敏度最高)-6(灵敏度最低),默认为2 */
    unsigned int fastInterval;                      /* 快速检测侦间隔，默认为3 */
    unsigned int slowInterval;                      /* 慢速检测帧间隔，默认为15 */
    unsigned int delay;                             /* 延时 */
    unsigned int bNeedDetail;                       /* 如果为真，DSP会将移动侦测的详细结构上传给主机 */
    unsigned int mask[MD_MAX_LINE_CNT];             /* 屏蔽值，对应位为1时判断该块，只有在bNeedDetail为FALSE时才有效 */
    unsigned int bAuto;                             /* 是否自适应 */
} MD_CONFIG;

/*
    遮挡报警的配置信息  HOST_CMD_SET_CD
*/
typedef struct
{
    unsigned int bEnable;                           /* 使能 */
    unsigned int level;                             /* 遮挡级别  */
    unsigned int delay;                             /* 延时，暂时未使用 */
    unsigned int x;                                 /* 遮挡区域位置参数 */
    unsigned int y;
    unsigned int w;
    unsigned int h;
} CD_CONFIG;


/*
    屏蔽的配置信息  HOST_CMD_SET_MASK
*/
typedef struct
{
    unsigned int regionCnt;                         /* 区域个数 */
    unsigned int x[MASK_MAX_REGION];                /* 区域位置参数 */
    unsigned int y[MASK_MAX_REGION];
    unsigned int w[MASK_MAX_REGION];
    unsigned int h[MASK_MAX_REGION];
    unsigned int flgMaskEnable;                     /* 使能 */
} MASK_CONFIG;

/*
    水印数据信息
    HOST_CMD_SET_WATERMARK
    HOST_CMD_SET_SUB_WATERMARK
*/
typedef struct
{
    unsigned int  global_time;                      /* 当前的全局时间(绝对时间)，需要和GROUP_HEADER中的时间相同 */
    unsigned int  device_sn;                        /* 设备的序列号(整数表示，非BCD码) */
    unsigned char mac_addr[6];                      /* 码流所属设备的MAC地址 */
    unsigned char device_type;                      /* 设备的类型(型号) */
    unsigned char device_info;                      /* 设备的附加信息(区域、语言、版本等) */
    unsigned char channel_num;                      /* 码流所属的通道号 */
    unsigned char reserve[47];                      /* 填补，保证按64对齐 */
} WATERMARK_VER1;

/*
    移动侦测结果返回 STREAM_ELEMENT_MD_RESULT
*/
typedef struct tagMdCbResultSt
{
    unsigned int uiMdResult[MD_MAX_LINE_CNT];
}MD_CB_RESULT_ST;


/*
    遮挡报警结果返回 STREAM_ELEMENT_CD_RESULT
*/
typedef struct tagCdCbResultSt
{
    unsigned int            chn;
    unsigned int            cdResult;
    unsigned char           reserve[8];
}CD_CB_RESULT_ST;

/* 二维码扫码区域配置，使用图像像素偏移坐标 HOST_CMD_QR_START  */
typedef struct tagQrRect
{
    unsigned int            roiEnable;              /* ROI 使能开关 */
    unsigned int            x;                      /* 扫码区域配置 */
    unsigned int            y;
    unsigned int            width;
    unsigned int            height;
} QR_ROI_RECT;

/* TDCL_POINT_F 点坐标结构体 */
typedef struct _QR_POINT_F
{
    float                   x;                      /* x坐标 */
    float                   y;                      /* y坐标 */
}QR_POINT_F;

/* QR码编码信息 */
typedef struct _QR_CODE_INFO_
{
    unsigned char           *charater;              /* 字符信息 */
    int                     len;                    /* 字符长度 */
}QR_CODE_INFO;

/* QR码中心点坐标、旋转角度结构体 */
typedef struct _QR_LOCATION_INFO
{
    QR_POINT_F              centerPt;               /* QR码中心点坐标 */

    QR_POINT_F              start_pt;               /* 拟合直线起始点 */
    QR_POINT_F              end_pt;                 /* 拟合直线终止点 */
    float                   rotateAngle;            /* QR码图像的旋转角度 */
}QR_LOCATION_INFO;

/* 二维码检测返回结果 STREAM_ELEMENT_QRCODE */
typedef struct tagQrCodeCbResultSt
{
    QR_CODE_INFO            codeInfo;               /* 编码信息 */
    QR_LOCATION_INFO        locInfo;                /* QR码位置角度信息 */
}QR_CODE_RESULT;

/****************************************************************************/
/*                                 萤石云 加密                                    */
/****************************************************************************/
#define CIPHER_KEY_SIZE   (32)

/** Cipher 工作模式 */
typedef enum CipherAlgWorkModeE
{
    DSP_CIPHER_WORK_MODE_UNKNOW = 0x0,
    DSP_CIPHER_WORK_MODE_ECB    = 0x1,        /* ECB 模式*/
    DSP_CIPHER_WORK_MODE_MAX
}CIPHER_ALG_WORK_MODE_E;

/* cipher 加密位宽 */
typedef enum CipherAlgBitWidthE
{
    DSP_CIPHER_BIT_WIDTH_UNKNOW = 0x0,
    DSP_CIPHER_BIT_WIDTH_128BIT = 0x10,  /* 最常用 128 位 */
    DSP_CIPHER_BIT_WIDTH_194BIT = 0x18,  /*        194 位 */
    DSP_CIPHER_BIT_WIDTH_256BIT = 0x20,  /*        256 位 */
}CIPHER_ALG_BIT_WIDTH_E;

/* cipher 设置类型 */
typedef enum CipherAlgConfigType
{
    DSP_CIPHER_UNKNOW      = 0x0,
    DSP_CIPHER_ENCRYPT     = 0x10,  /*  加密设置 */
    DSP_CIPHER_DECRYPT     = 0x18,  /*  解密设置 */
    DSP_CIPHER_TRANS_PACK  = 0x20,  /*  转包成加密设置 */
}CIPHER_ALG_CONFIG_TYPE;


/* 
	配置加密属性 HOST_CMD_AES_CONFIG 

	设置通道时，使用 SendCmdToDsp 中的 unsigned int chan 参数;
	对应 DSP_ENC_EZVIZ_CRYP_INFO 中的 encDevChan，
	对应 DSP_DEC_EZVIZ_CRYP_INFO 中的 chan。
*/
typedef struct tagCipherAlgConfigInfoSt
{
    CIPHER_ALG_WORK_MODE_E  workMode;               /* 加密模式 */
    CIPHER_ALG_BIT_WIDTH_E  bitWidth;               /* 位宽     */
    char                    key[CIPHER_KEY_SIZE];   /* 密钥     */
	unsigned char  reserve[32];		/* 备用 */
}CIPHER_ALG_CONFIG_INFO;

/*  
    PS 流加密 HOST_CMD_AES_PS_PROC
    RTP流加密 HOST_CMD_AES_RTP_PROC 
*/
typedef struct tagCipherAlgEncryptInfoSt
{
    unsigned int enable;         /* 加密控制 */
	unsigned char  reserve[32];		/* 备用 */
}CIPHER_ALG_ENCRYPT_INFO;

/****************************************************************************/
/*                                  双光融合                                */
/****************************************************************************/

/* 双光融合业务 属性 */
typedef enum eEifJobType
{
    EIF_NONE_JOB     = 0,              /* 双光融合 未指定任务   属性 */
    EIF_ENCODER_JOB  = 0xaa000000,     /* 双光融合用于 编码任务 属性 */
    EIF_DISPLAY_JOB  = 0xbb000000,     /* 双光融合用于 显示任务 属性 */        
    EIF_ALL_JOB      = 0xFFFFFFFF      /* 双光融合用于 所有任务 */
}EIF_JOB_TYPE;

/*  HOST_CMD_CHG_ISP_CFG  配置 标定畸变矫正属性 */
typedef struct tagIspCilbratePrmSt
{
    unsigned int    ispcalibrateEnable;      /* 开启690/691标定时ISP 的配置文件 */
    unsigned char   reserve[64];
}ISP_CILBRATE_PRM;

/* HOST_CMD_EIF_SET_PARAM  配置 双光融合 属性 */
typedef struct tagEifPrmSt
{
    unsigned int    eifEnable;      /* 开启双光融合功能 0:关闭  1:开启 */
    EIF_JOB_TYPE    eJobType;       /* 指定开启的任务                  */
    unsigned char   reserve[64];
}EIF_PRM;

/****************************************************************************/
/*                                  系统                                    */
/****************************************************************************/

/* DSP程序的版本信息 */
typedef struct tagDspSysVersion
{
    int             mainVersion;                    /* DSP库接口的主版本 */
    int             subVersion;                     /* DSP库接口的子版本 */
    int             svnVersion;                     /* DSP程序库的代码svn版本号 */
    char            buildData[32];                  /* DSP程序库编译日期 */
    char            buildTime[32];                  /* DSP程序库编译时间 */
    char            platForm[32];                   /* DSP程序库平台 */
    char            comVersion[32];                 /* DSP程序库对应组件版本号 */
    unsigned char   reserve[256];
}DSP_SYS_VERSION;

typedef struct tagDspPrmAddr
{
    unsigned char  *appVAddr;                       /* app访问的虚拟地址 */
    unsigned char  *dspVAddr;                       /* dsp访问的虚拟地址 */
} DSP_PRM_ADDR;

typedef struct tagDspRecPool
{
    REC_POOL_INFO AudRecPool[MAX_VENC_CHAN];        /* 编码录音buf描述 */
    REC_POOL_INFO ipcRecPool[MAX_VDEC_CHAN];        /* 转包码流共享缓冲区 */
} DSP_REC_POOL;

/*
	萤石云加密共享buff描述---采集加密
	DSP在开启萤石云加密时分配内存，关闭时不释放；重复开启时返回对应通道的地址
*/
typedef struct tagDspEncEzvizCrypInfo
{
	unsigned int encDevChan;                      	/* 加密设备号： 0->可见光 1->红外光 */

	unsigned int streamId;                      	/* 加密设备通道： 0->主码流 1->自码流 2->三码流 */
	
	NET_POOL_INFO  cryptNetPool;  					/* 编码加密码流buf描述 */
	NET_POOL_INFO *cryptNetPoolShmAddr;			    /* 编码加密码流buf描述,仅使用于双进程通信 */

	unsigned char reserve[60];						/* 备用 */
} DSP_ENC_EZVIZ_CRYP_INFO;


/*
	萤石云加密共享buff描述---解码加密
	DSP在开启萤石云加密时分配内存，关闭时不释放；重复开启时返回对应通道的地址
*/
typedef struct tagDspDecEzvizCrypInfo
{
    unsigned int chan;                      		/* 初始化加密编码通道编号 */

    unsigned int decDataType;                      	/* 0->rtp数据，1->ps数据 */
	
    DEC_SHARE_BUF decSharePool;       				/* 待加密数据共享缓冲区(PS/RTP) */

	NET_POOL_INFO  cryptNetPool;  					/* 编码加密码流buf描述 */
	
	DEC_SHARE_BUF *decSharePoolShmAddr;                /* 待加密数据共享缓冲区(PS/RTP),仅使用于双进程通信 */
    NET_POOL_INFO *cryptNetPoolShmAddr;                /* 编码加密码流buf描述,仅使用于双进程通信*/

	unsigned char reserve[56];						/* 备用 */
} DSP_DEC_EZVIZ_CRYP_INFO;


/* 应用与DSP模块共享信息结构体 */
typedef struct tagDspInitParaSt
{
    /***********************************************************************/
    /*                    设备类型区分信息                                 */
    /***********************************************************************/
    unsigned int viType;                            /* 视频输入类型 */
    unsigned int aiType;                            /* 音频输入模式 */
    unsigned int machineType;                       /* 机器型号(对讲有用来区分有音频和无音频)
                                                                    暂时保留，尽量不要用该参数，有无音频用
                                                                    aiType来区分
                                                                    */
    unsigned int boardType;                         /* 设备类型   */
    unsigned int sn;                                /* 设备序列号 */

    /***********************************************************************/
    /*                    DSP模块通道数定义信息                            */
    /***********************************************************************/
    unsigned int encChanCnt;                        /* 同时支持编码通道个数 */
    unsigned int decChanCnt;                        /* 同时支持解码通道个数 */
    unsigned int ipcChanCnt;                        /* 同时支持转包通道个数 */
    unsigned int dispChanCnt;                       /* 视频输出设备个数     */

    /* 视频采集参数配置 MAX_CAPT_CHAN */
    VI_INIT_INFO_ST stViInitInfoSt;                 /* 采集模块初始化属性 */

    /* 视频输出参数配置 MAX_DISP_CHAN */
    VO_INIT_INFO_ST    stVoInitInfoSt;              /* 显示输出模块初始化属性 */

    AUDIO_INIT_INFO_ST stAudioInfoSt;               /* 音频模块初始化属性     */

    /***********************************************************************/
    /*                  应用下发信息，用于实现对应功能                     */
    /***********************************************************************/
    /* 采集无视频信号时，所使用的无视频信号图像信息 */
    CAPT_NOSIGNAL_INFO_ST stCaptNoSignalInfo;
    FONT_LIB_INFO_ST      stFontLibInfo;            /* OSD功能所使用字库的信息 */
    STREAM_INFO_ST        stStreamInfo;             /* 码流编码与封包信息      */
    AUDIO_TB_BUF_INFO_ST  stAudioTBBufInfo;         /* 语音对讲功能缓冲区信息 */

    /***********************************************************************/
    /*                      码流共享缓冲区信息                             */
    /***********************************************************************/
    REC_POOL_INFO RecPoolMain[MAX_VENC_CHAN]; /* 编码录像主码流buf描述 */
    REC_POOL_INFO RecPoolSub[MAX_VENC_CHAN];  /* 编码录像子码流buf描述 */
    REC_POOL_INFO RecPoolThr[MAX_VENC_CHAN];  /* 编码录像第三码流buf描述 */
    
    NET_POOL_INFO NetPoolMain[MAX_VENC_CHAN]; /* 编码网络主码流buf描述 */
    NET_POOL_INFO NetPoolSub[MAX_VENC_CHAN];  /* 编码网络子码流buf描述 */
    NET_POOL_INFO NetPoolThr[MAX_VENC_CHAN];  /* 编码网络第三码流buf描述 */

    DEC_SHARE_BUF decShareBuf[MAX_VDEC_CHAN];       /* 解码码流共享缓冲区(PS/RTP) */
    DEC_SHARE_BUF ipcShareBuf[MAX_VDEC_CHAN];       /* 转包码流共享缓冲区 */

    /***********************************************************************/
    /*                      DSP模块状态信息                                */
    /***********************************************************************/
    /* 模块状态信息，应用按需获取，应用分配内存 */
    SYS_STATUS  stSysStatus;                        /* 系统状态 */
    CAPT_STATUS CaptStatus[MAX_CAPT_CHAN];          /* 采集 */
    ENC_STATUS  VencStatus[MAX_VENC_CHAN];          /* 编码 */
    DEC_STATUS  VdecStatus[MAX_VDEC_CHAN];          /* 解码 */
    DISP_STATUS DispStatus[MAX_DISP_CHAN];          /* 显示 */
    IPC_STATUS  VipcStatus[MAX_VIPC_CHAN];          /* 远程解码 */

    /***名单库容量,调放到结构体后部分，reserved前面***/
    unsigned int modleCnt;                   /* 人脸名单库的容量*/
    
    /* 多个N库模型容量: 支持分开配置每个库的最大容量，用于有效初始化常驻内存。(注意多库时让 multiModleCnt[0] = modleCnt)  */
    unsigned int enableMultiModle;          /* 启用多库后modleCnt不再有效，使用multiModleCnt的各个值 */
    unsigned int multiModleCnt[DSP_FR_FACE_FEATRE_CP_ATTR_MAX];

    /***虹膜模板库容量***/
    unsigned int irisDbCnt;                         /*  虹膜模板容量  */

    DSP_PRM_ADDR dspRecPool;                        /* 录像缓冲区 */
	
	unsigned int  broadcastChanCnt;                  		   /* 广播通道个数     */
    DEC_SHARE_BUF broadcastShareBuf[MAX_BROADCAST_CHAN];       /* 实时广播共享缓冲区 */
	
    /***********************************************************************/
    /*                      DSP功能扩展区定义                              */
    /***********************************************************************/
    /*
        此处给出地址数组，用于保存对应功能的结构体的地址，另需要根据功能定义
        相关的结构体，并由应用为其分配内存，传给DSP，以实现共享的作用
     */
    unsigned char reserve[868];

} DSPINITPARA;

/****************************************************************************/
/*                                 命令字                                   */
/****************************************************************************/

/*
    应用与DSP交互命令定义	
*/
typedef enum
{
    HOST_CMD_NON                        = 0x7FFFFFFF,       /* 起始 无效值 */
    /* 初始化DSP模块 */
    HOST_CMD_MODULE_SYS                 = 0x80000000,       /* 系统模块的能力级 */
    HOST_CMD_DSP_INIT                   = 0x80000001,       /* 初始化DSP库 */
    HOST_CMD_DSP_DEINIT                 = 0x80000002,       /* 去初始化DSP库 */
    HOST_CMD_DSP_GET_STATUS             = 0x80000003,
    HOST_CMD_DSP_GET_MEM                = 0x80000004,       /* 获取内存接口，用于产品升级*/
    HOST_CMD_DSP_FREE_MEM               = 0x80000005,       /* 释放对应的内存*/
    HOST_CMD_DSP_GET_SYS_VERISION       = 0x80000006,       /* 获取DSP程序的版本信息 */
    HOST_CMD_DSP_DUMP_STATUS            = 0x80000007,       /* 进程挂死时下发，保存DSP内部各模块状态信息 */
    HOST_CMD_DSP_HDAL_MEM_ALLOC         = 0x80000008,       /* 申请设备的内存(物理内存和虚拟内存) 与HOST_CMD_DSP_HDAL_MEM_FREE成对 仅AI3平台支持;复用结构体 MEM_INFO;必须在HOST_CMD_DSP_INIT调用后才能使用 */
    HOST_CMD_DSP_HDAL_MEM_FREE         	= 0x80000009,       /* 释放设备的内存(物理内存和虚拟内存) 与HOST_CMD_DSP_HDAL_MEM_ALLOC成对 仅AI3平台支持;复用结构体 MEM_INFO              ;必须在HOST_CMD_DSP_INIT调用后才能使用*/

    /* 采集 */
    HOST_CMD_MODULE_VI                  = 0x80010000,       /* 采集模块的能力级 */
    HOST_CMD_START_VIDEO_INPUT          = 0x80010001,       /* 启动视频采集 */
    HOST_CMD_STOP_VIDEO_INPUT           = 0x80010002,       /* 停止视频采集 */
    HOST_CMD_SET_VIDEO_PARM             = 0x80010003,       /* 配置视频采集属性 */
    HOST_CMD_SET_VIDEO_ROTATE           = 0x80010004,       /* 设置视频旋转属性 */
    HOST_CMD_SET_VIDEO_MIRROR           = 0x80010005,       /* 设置视频镜像属性 */

    HOST_CMD_SET_VIDEO_WDR              = 0x80010006,       /* 设置视频宽动态   */
    HOST_CMD_SET_VIDEO_NOSIGNAL         = 0x80010007,       /* 设置视频无信号图片 */
    HOST_CMD_SET_VIDEO_CALIBRATION      = 0x80010008,       /* 设置摄像头标定系数*/
    HOST_CMD_SET_VIDEO_INSTALL          = 0x80010009,       /* 设置摄像头安装的角度距离等信息用于无感通行*/

    HOST_CMD_START_VIDEO_CB             = 0x8001000a,       /* 开启视频采集回调功能 */
    HOST_CMD_STOP_VIDEO_CB              = 0x8001000b,       /* 停止视频采集回调功能 */
    HOST_CMD_SET_VIDEO_CENTER_OFFSET    = 0x8001000c,       /* 设置视频中心点偏移参数 */

    HOST_CMD_START_USB_INPUT            = 0x80011001,       /* 开启USB视频采集功能,暂不需要带参数，后续拓展的话再添加 */
    HOST_CMD_STOP_USB_INPUT             = 0x80011002,       /* 停止USB视频采集功能,暂不需要带参数，后续拓展的话再添加 */


    /* 编码 */
    HOST_CMD_MODULE_VENC                = 0x80020000,       /* 编码模块的能力级 */
    HOST_CMD_START_ENCODE               = 0x80020001,       /* 开启编码功能 */
    HOST_CMD_STOP_ENCODE                = 0x80020002,       /* 停止编码功能 */
    HOST_CMD_INSERT_I_FRAME             = 0x80020003,       /* 强制编码I帧  */
    HOST_CMD_SET_ENCODER_PARAM          = 0x80020004,       /* 设置编码器属性 */
    HOST_CMD_INSERT_USER_PIC_PARAM      = 0x80020005,       /* 主码流插入单张用户图片编码 */
    HOST_CMD_SET_ENCODE_PRIV            = 0x80020006,       /* 设置编码的私有属性 */
    HOST_CMD_START_MERGE_PIC_ENC        = 0x80020007,       /* 开始融合图片编码 */
    HOST_CMD_STOP_MERGE_PIC_ENC         = 0x80020008,       /* 结束融合图片编码 */
	HOST_CMD_ENC_IRIS_START				= 0x80020009,		/* 开启虹膜图像编码       ，使用结构体 IRIS_SHARE_BUFF_INFO */ 
	HOST_CMD_ENC_IRIS_STOP				= 0x8002000a, 	   	/* 停止虹膜图像编码，使用结构体 IRIS_SHARE_BUFF_INFO */ 
	HOST_CMD_ENC_USB_START				= 0x8002000b,		/* 开启USB图像编码       ，使用结构体 USB_SHARE_BUFF_INFO */ 
	HOST_CMD_ENC_USB_STOP				= 0x8002000c, 	   	/* 停止USB图像编码 ，使用结构体 USB_SHARE_BUFF_INFO */ 

    HOST_CMD_START_SUB_ENCODE           = 0x80021001,       /* 开始子码流编码功能 */
    HOST_CMD_STOP_SUB_ENCODE            = 0x80021002,       /* 停止子码流编码功能 */
    HOST_CMD_INSERT_SUB_I_FRAME         = 0x80021003,       /* 强制子码流编码I帧  */
    HOST_CMD_SET_SUB_ENCODER_PARAM      = 0x80021004,       /* 设置子码流编码器属性 */
    HOST_CMD_SUB_INSERT_USER_PIC_PARAM  = 0x80021005,       /* 子码流插入单张用户图片编码 */
    HOST_CMD_SET_SUB_ENCODE_PRIV        = 0x80021006,       /* 设置编码的私有属性 */

    HOST_CMD_START_THIRD_ENCODE         = 0x80022001,       /* 开始第三通道码流编码功能 */
    HOST_CMD_STOP_THIRD_ENCODE          = 0x80022002,       /* 停止第三通道码流编码功能 */
    HOST_CMD_INSERT_THIRD_I_FRAME       = 0x80022003,       /* 强制第三通道码流编码I帧  */
    HOST_CMD_SET_THIRD_ENCODE_PARAM     = 0x80022004,       /* 设置第三通道码流编码器属性 */
    HOST_CMD_SET_THIRD_ENCODE_PRIV      = 0x80022006,       /* 设置编码器属性 */

    HOST_CMD_SET_ENC_REGION             = 0x8002300d,       /* 设置编码ROI */
    HOST_CMD_PIC_SHOW_TEMP              = 0x8002300e,       /* 在热度图片上叠温度和人脸框*/

    /* 抓图 */
    HOST_CMD_START_ENC_JPEG             = 0x80020100,       /* 开启编码抓图功能 */
    HOST_CMD_STOP_ENC_JPEG              = 0x80020101,       /* 停止编码抓图功能 */
    HOST_CMD_SET_ENC_JPEG_PARAM         = 0x80020102,       /* 设置编码抓图属性 */

    /* 解码 */
    HOST_CMD_MODULE_DEC                 = 0x80030000,       /* 解码模块的能力级 */
    HOST_CMD_DEC_START                  = 0x80030001,       /* 开启解码功能 */
    HOST_CMD_DEC_STOP                   = 0x80030002,       /* 停止解码功能 */
    HOST_CMD_DEC_RESET                  = 0x80030003,       /* 重启解码器   */
    HOST_CMD_DEC_NEXT_FRM               = 0x80030004,       /* 解码单帧显示 */
    HOST_CMD_DEC_GET_IMG                = 0x80030005,       /* 设置解码抓图属性 */
    HOST_CMD_DEC_TRANS_PACK             = 0x80030006,       /* 解码转包，远程录像 */
    HOST_CMD_DEC_ATTRI                  = 0x80030007,       /* 图片格式分辨率解析 */
    HOST_CMD_DEC_JPG                    = 0x80030008,       /* 对JPG图片数据进行解码*/
    HOST_CMD_DEC_TRANS_TO_ENC           = 0x80030009,       /* 解码转包至编码缓冲区，本地录像 */
    HOST_CMD_DEC_PAUSE                  = 0x8003000a,       /* 暂停解码功能       */
    HOST_CMD_DEC_SYNC_REC               = 0x8003000b,       /* 解码通道录像与本地录像的时间戳同步 */
	HOST_CMD_DEC_STREAMTYPE             = 0x8003000c,       /* 控制解码数据类型主机命令,使用结构体 DECODER_STREAMTYPE  */

    /* 显示 */
    HOST_CMD_MODULE_DISP                = 0x80040000,        /* 显示模块的能力级 */
    HOST_CMD_ALLOC_DISP_REGION          = 0x80040001,        /* 配置显示窗口属性 */
    HOST_CMD_DISP_CLEAR                 = 0x80040002,        /* 清除显示窗口属性 */
    HOST_CMD_SET_OUTPUT_REGION          = 0x80040003,        /* 配置显示通道的输出区域 */
    HOST_CMD_SET_DISP_ROTATE            = 0x80040004,        /* 设置视频显示旋转属性 */
    HOST_CMD_SET_DISP_MIRROR            = 0x80040005,        /* 设置视频镜像属性     */

    HOST_CMD_SET_VI_DISP                = 0x80040006,        /* 配置采集预览功能 */
    HOST_CMD_SET_DEC_DISP               = 0x80040007,        /* 配置解码预览功能 */

    HOST_CMD_SET_VO_STYLE               = 0x80040008,        /* 设置显示输出风格 */
    HOST_CMD_SET_VO_CSC                 = 0x80040009,        /* 设置显示输出效果 */
    HOST_CMD_DISP_FB_MMAP               = 0x8004000a,        /* UI层内存映射   */
    HOST_CMD_DISP_FB_UMAP               = 0x8004000b,        /* UI层内存去映射 */
    HOST_CMD_DISP_FB_SHOW               = 0x8004000c,        /* 刷新UI显示     */
    HOST_CMD_DISP_FB_SNAP               = 0x8004000d,        /* UI抓图   */
    HOST_CMD_SET_VO_STANDARD            = 0x8004000e,        /* 修改CVBS输出制式 */
    HOST_CMD_DISP_INSERT_PIC_PARAM      = 0x8004000f,        /* 显示模块插入单张用户图片显示 */
    HOST_CMD_DISP_STANDBY               = 0x80040010,        /* 显示模块息屏初始化，防止极化 DISP_STANDBY_PRM*/
    HOST_CMD_SET_DISP_PTZ               = 0x80040011,        /* 设置显示预览云台功能*/
    HOST_CMD_DISP_FBE_PARAM             = 0x80040012,        /*显示强化功能，用于美颜功能*/
    HOST_CMD_DISP_THERMO                = 0x80040013,        /* 显示热成像图片DISP_THERMO_PRM*/
	HOST_CMD_DISP_IRIS					= 0x80040014,        /* 显示虹膜图像 IRIS_DISP_PRM,暂不支持*/
	HOST_CMD_DISP_USB					= 0x80040015,        /* 显示USB图像 USB_DISP_PRM*/
    HOST_CMD_SET_DISP_FACE_PIC          = 0x80040016,        /* 设置人脸显示图片*/
	
    /* 音频 */
    HOST_CMD_MODULE_AUDIO               = 0x80050000,       /* 音频模块的能力级 */
    HOST_CMD_SET_AUDIO_LOOPBACK         = 0x80050001,       /* 设置音频采集回环 */
    HOST_CMD_SET_DEC_PLAY               = 0x80050002,       /* 设置音频解码播放 */
    HOST_CMD_SET_TALKBACK               = 0x80050003,       /* 设置语音对讲功能属性 */
    HOST_CMD_START_AUDIO_PLAY           = 0x80050004,       /* 开启语音播报 */
    HOST_CMD_STOP_AUDIO_PLAY            = 0x80050005,       /* 停止语音播报 */
    HOST_CMD_START_AUTO_ANSWER          = 0x80050006,       /* 开启自动应答功能 */
    HOST_CMD_STOP_AUTO_ANSWER           = 0x80050007,       /* 停止自动应答功能 */
    HOST_CMD_START_AUDIO_RECORD         = 0x80050008,       /* 启动语音录制功能 */
    HOST_CMD_STOP_AUDIO_RECORD          = 0x80050009,       /* 停止语音录制功能 */
    HOST_CMD_START_AUDIO_OUTPUT         = 0x8005000a,       /* 启动语音输出 */
    HOST_CMD_STOP_AUDIO_OUTPUT          = 0x8005000b,       /* 停止语音输出 */
    HOST_CMD_START_AUDIO_INPUT          = 0x8005000c,       /* 启动语音输入 */
    HOST_CMD_STOP_AUDIO_INPUT           = 0x8005000d,       /* 停止语音输入 */
    HOST_CMD_SET_AUDIO_ALARM_LEVEL      = 0x8005000e,       /* 设置声音超限报警阈值 */
    HOST_CMD_SET_AI_VOLUME              = 0x8005000f,       /* 设置音频输入音量 */
    HOST_CMD_SET_AO_VOLUME              = 0x80050010,       /* 设置音频输出音量 */

    HOST_CMD_SET_EAR_PIECE              = 0x80050011,       /* 设置听筒免提切换 */
    HOST_CMD_START_SOUND_RECORD         = 0x80050012,       /* 开启音频输入录音 */
    HOST_CMD_STOP_SOUND_RECORD          = 0x80050013,       /* 停止音频输入录音 */
    HOST_CMD_SET_MANAGE_TALKTO_RESIDENT = 0x80050014,       /* 启动网络设备与模拟室内机对讲功能，或者本地MIC1和SPK2对讲 */
    HOST_CMD_SET_DOOR_TALKTO_RESIDENT   = 0x80050015,       /* 门口机与模拟室内机对讲功能，或者本地MIC1和SPK2对讲 */
    HOST_CMD_SET_AUDIO_ENC_TYPE         = 0x80050016,       /* 设置音频编码类型 */

    HOST_CMD_SET_AI_VOLUME_EX           = 0x80050017,       /* 设置音频输入音量 扩展配置 */
    HOST_CMD_SET_AO_VOLUME_EX           = 0x80050018,       /* 设置音频输出音量 扩展配置*/

    HOST_CMD_SET_TTS_PARAM              = 0x80050019,       /* 设置 TTS 参数  */

    HOST_CMD_START_SPEECH_RECOGNITION     = 0x8005001a,     /* 开启语音识别 */
    HOST_CMD_SET_SPEECH_RECOGNITION_PARAM = 0x8005001b,     /* 设置相关参数 */
    HOST_CMD_STOP_SPEECH_RECOGNITION      = 0x8005001c,     /* 关闭语音识别 */

    
    HOST_CMD_START_DVPR_REGISTER        = 0x80050020,     /* 开启声纹注册 */
    HOST_CMD_STOP_DVPR_REGISTER         = 0x80050021,     /* 关闭声纹注册 */
	HOST_CMD_START_DVPR_RECOGNITION     = 0x80050022,     /* 开启声纹识别 */
    HOST_CMD_SET_DVPR_PRM               = 0x80050023,     /* 设置声纹识别相关参数 DVPR_PARAM*/
    HOST_CMD_STOP_DVPR_RECOGNITION      = 0x80050024,     /* 关闭声纹识别 */
    HOST_CMD_DVPR_MODEL_COMPARE         = 0x80050025,     /* 声纹模型比对1V1 DVPR_1V1_COMPARE*/
    HOST_CMD_SET_DVPR_KEYWORD           = 0x80050026,       /* 设置数字密码 DVPR_KEYWORD_PARAM*/
    HOST_CMD_IMPORT_DVPR_DATA_REGISTER  = 0x80050027,       /* 导入声纹识别有效数据进行注册 DVPR_IMPORT_DATA_REG_ST*/
    HOST_CMD_DVPR_MODEL_CHECK           = 0x80050028,       /* 声纹模型校验 DVPR_MODEL_CHECK_ST*/
    HOST_CMD_DVPR_GET_VERSION           = 0x80050029,       /* 获取声纹算法版本 DVPR_VERSION*/
    HOST_CMD_SET_DVPR_REGIST_REST       = 0x8005002a,       /* 注册过程中复位，重新从第一句开始 */
    HOST_CMD_SET_DVPR_RECORD_AUDIO      = 0x8005002b,     /* 设置录制声纹音频数据配置，单次录制时长由APP下发。用于后续进行过声纹测试 DVPR_RECORD_AUDIO_PARAM */
    HOST_CMD_PASS_DVPR_AUDIO            = 0x8005002c,     /* 进行过声纹测试，单次下发一句音频+对应数字密码 DVPR_PASS_AUDIO_ST */
    HOST_CMD_DVPR_GET_SIM_THRESHOLD     = 0x8005002d,        /* 获取声纹比对相似度推荐阈值 DVPR_SIM_THRESHOLD_PRM*/

    HOST_CMD_SET_AO_DEV                 = 0x8005001d,       /* 设置音频输出设备 */
    HOST_CMD_SET_AI_DEV                 = 0x8005001e,       /* 设置音频输入设备 */

    HOST_CMD_CONTROL_AUDIO_PLAY         = 0x8005001f,       /* 控制语音播报，暂停和恢复 */

    HOST_CMD_START_AUDIO_MIX            = 0x80050101,     /* 开启混音功能 */
    HOST_CMD_STOP_AUDIO_MIX             = 0x80050102,     /* 停止混音功能 */

    HOST_CMD_START_AUDIO_BROADCAST      = 0x80050103,       /* 开启音频广播 */
    HOST_CMD_STOP_AUDIO_BROADCAST       = 0x80050104,       /* 关闭音频广播 */

    HOST_CMD_SET_AMER_PARAM             = 0x80050105,       /* 设置混音参数 */
    /* OSD */
    HOST_CMD_MODULE_OSD                 = 0x80060000,       /* OSD模块的能力级 */
    HOST_CMD_SET_ENC_OSD                = 0x80060001,       /* 设置编码通道OSD属性 */
    HOST_CMD_SET_ENC_DEFAULT_OSD        = 0x80060002,       /* 设置编码通道默认OSD属性 */
    HOST_CMD_START_ENC_OSD              = 0x80060003,       /* 开启编码通道OSD功能 */
    HOST_CMD_STOP_ENC_OSD               = 0x80060004,       /* 停止编码通道OSD功能 */
    
    HOST_CMD_SET_JPEG_OSD               = 0x80060005,       /* 设置抓图通道OSD属性 */
    HOST_CMD_SET_JPEG_DEFAULT_OSD       = 0x80060006,       /* 设置抓图通道默认OSD属性 */
    HOST_CMD_START_JPEG_OSD             = 0x80060007,       /* 开启抓图通道OSD功能 */
    HOST_CMD_STOP_JPEG_OSD              = 0x80060008,       /* 停止抓图通道OSD功能 */

    HOST_CMD_SET_FR_JPEG_OSD            = 0x80060009,       /* 设置人脸抓图通道OSD属性 */
    HOST_CMD_SET_FR_JPEG_DEFAULT_OSD    = 0x80060010,       /* 设置人脸抓图通道默认OSD属性 */
    HOST_CMD_START_FR_JPEG_OSD          = 0x80060011,       /* 开启人脸抓图通道OSD功能 */
    HOST_CMD_STOP_FR_JPEG_OSD           = 0x80060012,       /* 停止人脸抓图通道OSD功能 */

    HOST_CMD_SET_DISP_OSD               = 0x80060100,       /* 设置显示通道OSD功能 */
    HOST_CMD_START_DISP_OSD             = 0x80060101,       /* 开启显示通道OSD功能 */
    HOST_CMD_STOP_DISP_OSD              = 0x80060102,       /* 停止显示通道OSD功能 */

    HOST_CMD_SET_ENC_LOGO               = 0x80060200,       /* 设置编码通道LOGO属性 */
    HOST_CMD_START_ENC_LOGO             = 0x80060201,       /* 开启编码通道LOGO功能 */
    HOST_CMD_STOP_ENC_LOGO              = 0x80060202,       /* 停止编码通道LOGO功能 */

    HOST_CMD_SET_DISP_LOGO              = 0x80060300,       /* 设置显示通道LOGO属性 */
    HOST_CMD_START_DISP_LOGO            = 0x80060301,       /* 开启显示通道LOGO功能 */
    HOST_CMD_STOP_DISP_LOGO             = 0x80060302,       /* 停止显示通道LOGO功能 */

    HOST_CMD_UPDATE_FONT_LIB            = 0x80060400,       /* 更新OSD 字库 */

    /* ISP */
    HOST_CMD_MODULE_ISP                 = 0x80070000,       /* ISP模块的能力级 */
    HOST_CMD_LIGHTCTR                   = 0x80070001,
    HOST_CMD_SET_ISP_PARAM              = 0x80070002,       /* 设置ISP参数 */
    HOST_CMD_GET_ISP_PARAM              = 0x80070003,       /* 获取ISP参数 */
    HOST_CMD_SET_EXPOSURE               = 0x80070004,
    HOST_CMD_CHG_ISP_CFG                = 0x80070005,       /*修改ISP的配置文件，目前690/691项目专用，使用结构体  ISP_CILBRATE_PRM */
    
    HOST_CMD_SENSOR_START               = 0x80070100,       /* 启动 sensor 输出 */
    HOST_CMD_SENSOR_STOP                = 0x80070101,       /* 停止 sensor 输出 */

    /* 智能模块 人脸识别 */
    HOST_CMD_MODULE_FR                  = 0x80080000,       /* 智能模块的能力级 */
    HOST_CMD_INIT_FR                    = 0x80080001,       /* 初始化智能模块 */
    HOST_CMD_FFD_CTRL                   = 0x80080002,       /* 人脸检测功能控制属性 */
    HOST_CMD_FR_BM                      = 0x80080003,       /* 人脸建模功能 */
    HOST_CMD_FR_DET                     = 0x80080007,       /* 活体检测命令字 */
    HOST_CMD_GET_FR_VER                 = 0x80080008,       /* 获取算法版本号 */
    HOST_CMD_FR_COMPARE                 = 0x80080009,       /* 1v1  */
    HOST_CMD_FR_POOL_PRM                = 0x8008000b,       /* 获取底库地址和锁管理内存和刷cache函数*/
    HOST_CMD_FR_JPGSCALE                = 0x8008000d,       /* 图片缩放，支持jpg和png， 使用结构体   :FR_BM_JPG_SCALE_PRM_ST       */
    HOST_CMD_FR_FACE_CROP               = 0x8008000c,       /* 设置人脸抠图参数 */    
    HOST_CMD_FR_BMP2JPG                 = 0x8008000e,       /* BMP转jpg */
    HOST_CMD_FR_ROI                     = 0x8008000f,       /* 人脸检测区域 */
    HOST_CMD_FR_SAFETY_HELMET           = 0x80080011,       /* 安全帽功能*/
    HOST_CMD_FR_PIC_FD                  = 0x80080012,       /* 图片人脸检测功能 */
    HOST_CMD_FR_SIM_THRESHOLD           = 0x80080013,       /* 设置单库或者多库的使用，和比对相似度的不同阈值 */
    HOST_CMD_FR_CHECK_MODEL             = 0x80080014,       /* 模型校验 */
    HOST_CMD_FR_SAVE_1VN_PIC            = 0x80080015,       /* 1VN存图功能调试开关 */
    HOST_CMD_FR_INSERT_IMAGE            = 0x80080016,       /* 过图片使能开关 */
    HOST_CMD_FR_FLD_LIGHT_PRM           = 0x80080017,       /* 获取自研活体控灯状态 */
    HOST_CMD_GET_ENG_MEM                = 0x80080018,       /* 获取引擎内存使用情况 */
    HOST_CMD_FR_DEL_MODEL               = 0x80080019,       /* 删除模型*/
    HOST_CMD_FR_ADD_MODEL               = 0x8008001a,       /* 添加模型*/
    HOST_CMD_FR_QUE_MODEL               = 0x8008001b,       /* 查询模型*/
    HOST_CMD_FR_CMP_1VN                 = 0x8008001c,       /* 模型1VN操作*/
    HOST_CMD_FR_SET_TOF_CALIB           = 0x8008001d,       /* 设置TOF的标定参数 */
    HOST_CMD_FR_SET_FLD_SAV_PIC_DIR     = 0x8008001e,       /* 设置自研活体存图路径 FR_FACELD_SAV_PIC_DIR_INFO */
    HOST_CMD_FR_SET_SCORE               = 0x8008001f,       /* 设置DFR  流程建模的人脸综合评分阈值*/
    HOST_CMD_FR_GET_SCORE               = 0x80080020,       /* 获取DFR  流程建模的人脸综合评分阈值*/

    HOST_CMD_FR_SET_CALIBRATION         = 0x80080021,       /* 标定参数配置 */
    HOST_CMD_FR_DET_V2                  = 0x80080022,       /* 活体控制命令字 */
    HOST_CMD_SET_THRESHOLD              = 0x80080023,       /* 活体阈值设置 */
    HOST_CMD_GET_THRESHOLD              = 0x80080024,       /* 活体阈值获取 */

    HOST_CMD_FR_GET_SIM_THRESHOLD       = 0x80080025,       /* 获取默认相似度比对阈值 */

    /* 微智能模块 */
    HOST_CMD_MODULE_VDA                 = 0x80090000,       /* 视频侦测模块的能力级 */
    HOST_CMD_START_MD                   = 0x80090001,       /* 启动移动侦测 */
    HOST_CMD_STOP_MD                    = 0x80090002,       /* 停止移动侦测 */
    HOST_CMD_SET_MD                     = 0x80090003,       /* 设置移动侦测属性 */
    HOST_CMD_SET_CD                     = 0x80090004,       /* 设置遮挡检测 */
    HOST_CMD_SET_MASK                   = 0x80090005,       /* 设置视频遮挡 */
    HOST_CMD_QR_START                   = 0x80090006,       /* 开启二维码扫码 */
    HOST_CMD_QR_STOP                    = 0x80090007,       /* 关闭二维码扫码 */
    HOST_CMD_OCR_START                  = 0x80090008,       /* 开启文字识别*/
    HOST_CMD_OCR_STOP                   = 0x80090009,       /* 关闭文字识别*/

    HOST_CMD_MODULE_PRIVATE             = 0x80090200,       /* 设置私有信息        */
    HOST_CMD_SET_WATERMARK              = 0x80090201,       /* 设置水印        */
    HOST_CMD_SET_SUB_WATERMARK          = 0x80090202,       /* 设置子通道水印  */

    HOST_CMD_MODULE_FLD                 = 0x800a0000,       /* 活体检测能力级 */
    HOST_CMD_SET_SOLID_PARAM            = 0x800a0001,       /* 设置活体参数 */
    HOST_CMD_GET_SOLID_PARAM            = 0x800a0002,       /* 获取活体参数 */

    /* 虹膜识别模块 */
    HOST_CMD_MODULE_IRIS                = 0x800b0000,       /* 虹膜识别模块的能力级 */
    HOST_CMD_IRIS_INIT                  = 0x800b0001,       /* 初始化IRIS模块 */
    HOST_CMD_IRIS_DEINIT                = 0x800b0002,       /* 反初始化和释放虹膜资源 */
    HOST_CMD_IRIS_GET_VER               = 0x800b0003,       /* 获取虹膜算法版本 */
    HOST_CMD_IRIS_GET_ABILITY           = 0x800b0004,       /* 获取虹膜算法能力集 */
    HOST_CMD_IRIS_DETECT                = 0x800b0005,       /* 开启虹膜检测、识别、本地录入 */
    HOST_CMD_IRIS_BM                    = 0x800b0006,       /* 下发虹膜图片进行建模提取，结果回调返回 */
    HOST_CMD_IRIS_COMPARE               = 0x800b0007,       /* 两个虹膜模板进行1V1 比对，结果直接返回 */
    HOST_CMD_IRIS_POOL_PRM              = 0x800b0008,       /* 获取底库地址和锁管理内存和刷cache函数、实现模板库的管理 */
	HOST_CMD_IRIS_TOF            		= 0x800b0009,       /* 获取TOF测距信息 */
	HOST_CMD_IRIS_SAVE_PIC              = 0x800b000a,       /* 存图功能调试开关 */
	HOST_CMD_IRIS_CALIBRATE             = 0x800b000b,       /* 标定模式开关 */	
	HOST_CMD_IRIS_ISP_PRM				= 0x800b000c,		/* 虹膜ISP参数，使用结构体IRIS_ISP_PRMAM，支持多线程使用，需在DSP初始化后 */


    HOST_CMD_MODULE_EIF                 = 0x800c0000,       /* 双光融合功能 */
    HOST_CMD_EIF_SET_PARAM              = 0x800c0001,       /* 配置 双光融合功能 属性 */

    /* TOF模块 */
	HOST_CMD_MODULE_TOF                 = 0x800d0000,        /*TOF模块的能力级 */
    HOST_CMD_TOF_START                  = 0x800d0001,        /* 开启TOF检测 */
    HOST_CMD_TOF_STOP                   = 0x800d0002,        /* 关闭TOF检测 */
    HOST_CMD_TOF_START_SAVE             = 0x800d0003,        /* 开启TOF采图保存功能 */
    HOST_CMD_TOF_STOP_SAVE              = 0x800d0004,        /* 关闭TOF采图保存功能 */
    HOST_CMD_TOF_TEMPERATURE            = 0x800d0005,        /* 获取TOFSensor温度与Drive温度 */

    /* 萤石云加密 */
    HOST_CMD_EZVIZ_AES_MODULE		    = 0x800e0000,       /* 数据加密模块能力级 */

	/* 编码加密，只支持H264 */
	HOST_CMD_ENC_EZVIZ_AES_CONFIG       = 0x800e0001,       /* AES加密配置，配置密钥时用，设置前必须停止加密，如果未停止，需返回失败 */
    HOST_CMD_ENC_EZVIZ_AES_START        = 0x800e0002,       /* 开始AES 加密，数据从0地址写数据 */
    HOST_CMD_ENC_EZVIZ_AES_STOP         = 0x800e0003,       /* 停止 AES 加密 */

	/* 解码加密，只支持H264 */
	HOST_CMD_DEC_EZVIZ_AES_CONFIG       = 0x800e0004,       /* AES加密配置，配置密钥时用，设置前必须停止加密，如果未停止，需返回失败 */
    HOST_CMD_DEC_EZVIZ_AES_START        = 0x800e0005,       /* 开始AES 加密，数据从0地址写数据 */
    HOST_CMD_DEC_EZVIZ_AES_STOP         = 0x800e0006,       /* 停止 AES 加密 */

    /* 其他 */
    HOST_CMD_INVALID                    = 0xffffffff,       /* 最大无效值 */
}HOST_CMD;

/*************************************************************************************
                            接口定义
*************************************************************************************/
int SendCmdToDsp
(
    HOST_CMD cmd, unsigned int chan,
    unsigned int * pParam, void *pBuf
);


/*******************************************************************************
* 函数名  : InitDspSys
* 描  述  : 预初始化DSP运行起来需要的基础资源，完成全局结构的空间分配；多进程下
            还会进行前期进程通讯资源的建立，调用这个接口成功返回后，APP就可以进
            行 DSPINITPARA 参数的配置了；参数配置完毕通过 HOST_CMD_DSP_INIT 命令
            把 DSP 所有资源及业务启动。
* 输  入  : - ppDspInitPara: APP将要指向全局结构体的指针
*         : - pFunc        : APP传递给DSP的回调函数体
* 输  出  : - ppDspInitPara: APP获取到的全局结构体
* 返回值  : 0  : 成功
*           -1 : 失败
*******************************************************************************/
int InitDspSys(DSPINITPARA **ppDspInitPara, DATACALLBACK pFunc);



/* SDK 和 APP直接的交互分辨率，该分辨率由SDK定义，等APP完全平台化后会取消掉。 后续不放在DSP头文字中。 */
#define CIF_FORMAT              0x00001001
#define QCIF_FORMAT             0x00001002
#define FCIF_FORMAT             0x00001003  /*add 2002-6-25*/
#define HCIF_FORMAT             0x00001004
#define QQCIF_FORMAT            0x00001005  /* PAL: 96*80 NTSC: 96*64*/
#define NCIF_FORMAT             0x00001006  /* 320*240*/
#define QNCIF_FORMAT            0x00001007  /* 160*120*/
#define DCIF_FORMAT             0x00001008  /* PAL:528*384 NTSC:528*320 */
#define D1_FORMAT               0x0000100F  /* 720*576 仅测试用 2003-12-25 */
#define VGA_FORMAT              0x00001010  /* VGA:640*480，PAL和NTSC相同 */
#define Q2CIF_FORMAT            0x00001009
#define QQ720P_FORMAT           0x0000100A  /*320*192 (1/16 720p) added 20090306*/
#define RES_320_180             0x0000100B  /*320*180 added 20091217*/

#define UXGA_FORMAT             0x00001011  /* UXGA:1600*1200 added  2007-4-1*/
#define SVGA_FORMAT             0x00001012  /*800*600 added  2007-12-12*/
#define HD720p_FORMAT           0x00001013  /*1280*720 added  2007-12-12*/

#define SXGA_FORMAT             0x00001014  /* 1280x1024 */
#define XVGA_FORMAT             0x00001014  /*1280x960 added  2008-12-17*/
#define HD900p_FORMAT           0x00001015  /*1600x900 added  2008-12-17*/
#define SXGAp_FORMAT            0x00001016  /*1360*1024 added  2009-05-13*/

#define HD_H_FORMAT             0x00001017  /*前端如果进来的是1920*1080,则该分辨率为
                                          1920*540,如果前端进来时720p，则进行1280*360 added 20090712*/
#define HD_D_FORMAT             0x00001018  /*前端如果进来的是1920*1080,则该分辨率为
                                          1440*720,如果前端进来时720p，则进行960*480 added 20090712*/
#define HD_Q_FORMAT             0x00001019  /*前端如果进来的是1920*1080,则该分辨率为
                                          60*540,如果前端进来时720p，则进行640*360 added 20090712*/
#define HD_QQ_FORMAT            0x0000101a  /*前端如果进来的是1920*1080,则该分辨率为
                                          480*270,如果前端进来时720p，则进行320*180 added 20090712*/
#define HD1080i_FORMAT          0x0000101b  /*1920*1080i added 200904010*/
#define HD1080p_FORMAT          0x0000101b  /*注意和1920*1080i一样!! 以后修正, 1920*1080p*/
#define RES_2560_1920_FORMAT    0x0000101c  /*2560*1920  added 2009-10-19*/
#define RES_1600_304_FORMAT     0x0000101d  /*1600*304  added 2009-10-30*/
#define RES_2048_1536_FORMAT    0x0000101e  /*2048*1536  added 2009-12-23*/
#define RES_2448_2048_FORMAT    0x0000101f  //2448*2048 added 2010-01-06
//#define SXGA_FORMAT                0x00001023        /* 1280x1024 */

#define RES_2208_1242_FORMAT    0x00001024
#define RES_2304_1296_FORMAT    0x00001025
#define RES_2304_1536_FORMAT    0x00001026
#define RES_2592_1944_FORMAT    0x00001027  /* 2592*1944, 500w分辨率 */

#define RES_720_1280_FORMAT     0x000010a7
#define RES_1080_1920_FORMAT    0x000010ac
#define RES_1080_720_FORMAT     0x000010d7
#define RES_360_640_FORMAT      0x000010d8
#define RES_1944_2592_FORMAT    0x000010d9  /* 1944*2592, 500w分辨率，供竖放sensor使用 */
#define RES_480_640_FORMAT      0x000010da  /* 480*640, 结构光IR使用 */

#ifdef __cplusplus
}
#endif


#endif  /* _DSPCOMMON_H_ */
