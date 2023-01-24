/*******************************************************************************
 * dspcommon.h
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : huangshuxin <huangshuxin@hikvision.com>
 * Version: V2.6.0  2020��05��06�� Create
 *
 * Description : DSPģ��������ṩ��ͷ�ļ�
 * Modification:
 *******************************************************************************/

#ifndef _DSPCOMMON_H_
#define _DSPCOMMON_H_

#ifdef __cplusplus
extern "C"
{
#endif


/*************************************************************************************
                            �궨��
*************************************************************************************/

/* ���ӿ�ͷ�ļ��İ汾�ţ�ÿ�θ��½ṹ��ʱ����Ҫ���¸ú��ֵ */
#define DSP_COMMON_MAIN_VERSION                 (2)             /* ���汾�� */
#define DSP_COMMON_SUB_VERSION                  (7)             /* �Ӱ汾�� */
#define DSP_COMMON_REV_VERSION                  (204)           /* С�汾�� */
#define DSP_COMMON_DATE_VERSION                 "2023-01-14"    /* �汾�������� */
#define DSP_COMMON_VERSION_INFO                 "debug"         /* �汾���� */

#define MAX_NO_SIGNAL_PIC_CNT                   (2)             /* Ŀǰ���֧��ʹ����������Ƶͼ�� */

/* ϵͳ��֧�ֵ�����ͨ���� */
#define MAX_CAPT_CHAN                           (4)             /* �ɼ�ͨ���� */
#define MAX_VENC_CHAN                           (4)             /* ����ͨ���� */
#define MAX_VDEC_CHAN                           (4)             /* ����ͨ���� */
#define MAX_VIPC_CHAN                           (4)             /* ת��ͨ���� */
#define MAX_BROADCAST_CHAN                      (4)             /* �㲥ͨ���� */
#define MAX_DISP_CHAN                           (16)            /* ��ʾͨ���� */
#define MAX_AES_CHAN                            (2)             /* ����ͨ���� */

#define TALKBACK_FRM_LEN                        (160)           /* �Խ���Ƶÿ֡���� */
#define TALKBACK_FRM_CNT                        (128)           /* �Խ���Ƶ����֡���� */

/* �Խ���Ƶ���峤�� */
#define TALKBACK_BUF_LEN                        (TALKBACK_FRM_LEN * TALKBACK_FRM_CNT)

/* �����ͨ���ϴ���¼��洢������I֡��Ϣ�洢�����Ŀ */
#define REC_IFRM_INFO_NUM                       (200)

#define ERR_NO_VIDEO_INPUT                      (0x40000001)   /* ��ʱ��ɼ�������Ƶ������DSP */

/* �� APP ��  DSP ֮�����̷��� */
#define DSP_INIT_SHARED     "/home/app/.INIT_SHM"
#define DSP_NET_SHARED      "/home/app/.NET_SHM"

/* �����������������С */
#define DSP_POOL_SIZE      (2 * 1024 * 1024)
/* ����¼�񻺳�����С��֧��Ԥ¼ */
#define DSP_REC_POOL_SIZE   (14 * 1024 * 1024)

/* DSP ��󶥵���� OCR ��������ʹ��*/
#define DSP_MAX_POLYGON_NUM    (10)

/* DSP ���ʶ�����OCR ���ո���*/
#define DSP_MAX_OCR_NUM        (3)

/* DSP ʶ�������VDP ������ַ�����*/
#define DSP_MAX_OCRLINE_NUM    (30)

/* DSP ʶ�����OCR ��������ַ�����*/
#define DSP_MAX_OCRCHAR_NUM    (128)

#define MAX_DISP_FACE_PIC_CNT                    (10)           /* ���������ͼƬ���� */
#define MAX_DISP_FACE_LAYER_CNT                  (2)            /* ���������ͼ����� */

/*************************************************************************************
                                     �ṹ�嶨��
*************************************************************************************/

/****************************************************************************/
/*                              DSP�ص�����                                 */
/****************************************************************************/

/*
    ������Ϣħ��������
*/
#define STREAM_ELEMENT_MAGIC                    (0xdb1bd987)

/*
    �ص������ֶ��� HOST_CMD_NON ��0x80000000 �𲽣��ص�����ܿ����ֵ���ɣ�����0x80000000
*/
typedef enum _STREAM_TYPE_
{
    STREAM_ELEMENT_CAPT_ERR             = 0x90010001,    /*�ɼ����ݳ�ʱ*/  
    STREAM_ELEMENT_CAPT_CB              = 0x90010002,    /* ��Ƶ�ɼ�NV12���ݻص� */

    STREAM_ELEMENT_JPEG_IMG             = 0x90020033,    /* ץͼ����JPEG */
    STREAM_ELEMENT_QRCODE               = 0x90020034,    /* ��ά�봦����*/
    STREAM_ELEMENT_MD_RESULT            = 0x90020035,    /* �ƶ������ */
    STREAM_ELEMENT_CD_RESULT            = 0x90020036,    /* �ڵ�������� */
    STREAM_ELEMENT_VIDEO_I_PS           = 0x90020037,    /* PS��I֡�ص�֪ͨ */
    STREAM_ELEMENT_VIDEO_I_RTP          = 0x90020038,    /* RTP��I֡�ص�֪ͨ */
    STREAM_ELEMENT_VIDEO_I_TRANSPACK    = 0x90020039,    /* PS��Զ��¼��I֡�ص�֪ͨ */
    STREAM_ELEMENT_OCR_RESULT           = 0x9002003a,    /* OCR ʶ���� */

    STREAM_ELEMENT_AUDIO_REC            = 0x90030001,    /* ���Թ��ܻص� */
    STREAM_ELEMENT_AUDIO_RING_PLAY      = 0x90030002,    /* ��Ƶ�����������ܻص�,���������� */
    STREAM_ELEMENT_AUDIO_PLAY_STATUS    = 0x90030003,    /* ��Ƶ��������״̬�ص�,��������������TTS��������ʱ�㲥����*/

    STREAM_ELEMENT_DEC_RES_ERROR        = 0x90040001,    /* ��Ƶ����ֱ��ʴ���*/    
    STREAM_ELEMENT_DEC_ENC_TYPE_ERR     = 0x90040002,    /* ��Ƶ��ʽ��֧�֣���H264����MJPEG */
    STREAM_ELEMENT_DEC_CAP_INFO         = 0x90040003,    /* ����ɹ�������Ӧ�ÿ��·�ץͼ���� */
    STREAM_ELEMENT_DEC_HAL_ERR          = 0x90040004,     /* �ײ������󣬵��µײ���������� */
    STREAM_ELEMENT_DEC_FRAME_RATE_ERR   = 0x90040006,     /* ����֡�ʲ��ԣ����١���֡*/
    STREAM_ELEMENT_DEC_UNPACK_ERR       = 0x90040007,     /* ������������*/
    STREAM_ELEMENT_DEC_SEQUENCE_ERR     = 0x90040008,     /* rtp����Ų�����*/
    STREAM_ELEMENT_DEC_WRITE_TIMEOUT    = 0x90040009,     /* ���ݳ�ʱδ�͵���1s�� */

    STREAM_ELEMENT_FR_FFD_IQA           = 0x9007004e,    /* ������⼰������������ */
    STREAM_ELEMENT_FR_BM                = 0x9007004f,    /* ������ģ */

    STREAM_ELEMENT_FR_CP                = 0x90070050,    /* �����Ա� */
    STREAM_ELEMENT_FR_DEC               = 0x90070051,    /* ������ */
    STREAM_ELEMENT_FR_BM_CAP            = 0x90070052,    /* ��Ƶץ��������ģ */
    STREAM_ELEMENT_FR_FD_JPG            = 0x90070053,    /* JPG�����������*/
    STREAM_ELEMENT_FR_PRE_CAP           = 0x90070054,    /* JPGԤץ���ܵ�ǰ֡ǰ���2֡*/
    STREAM_ELEMENT_FR_BMP2JPG           = 0x90070055,    /* BMP����ת����JPG����*/
    STREAM_ELEMENT_FR_JPGSCALE          = 0x90070056,    /* �ֱ��ʹ����JPGת���ɿ���640*480�ֱ��ʵ�JPG*/
    STREAM_ELEMENT_FR_ENG_ERR           = 0x90070057,    /*�����ʼ��ʧ��*/
    STREAM_ELEMENT_FR_JPGOVEROSD        = 0x90070058,    /*JPG����OSD*/
    STREAM_ELEMENT_FR_1VN_SIM           = 0x90070059,    /* �ȶ�����1VN �������ƶ�*/
    STREAM_ELEMENT_FR_PIC_FD            = 0x9007005a,    /* ��������ȡ�������������� */

    STREAM_ELEMENT_TTS_CONVERSION       = 0x90080001,   /* TTSת����� */
    STREAM_ELEMENT_SPEECH_RECOGNITION   = 0x90080002,   /* ����ʶ���� */
    STREAM_ELEMENT_DVPR_RECOGNITION     = 0x90080003,   /*����ʶ����*/

    STREAM_ELEMENT_IRIS_IQA             = 0x90090001,   /* ���ۼ�⼰������ͨ����Ĥ�ɼ�ʱѡ��ʹ�ã�*/
    STREAM_ELEMENT_IRIS_BM              = 0x90090002,   /* �·����к�ĤͼƬ��ķ���ģ������*/
    STREAM_ELEMENT_IRIS_1VN_SIM         = 0x90090003,   /* ��Ĥ����ע���1VN�ȶ����ƶȽ��*/
    STREAM_ELEMENT_IRIS_CALIBRATE       = 0x90090004,   /* ��Ĥ�궨���*/
    
    STREAM_ELEMENT_REBOOT               = 0x900a0001,   /* APP�յ�����Ҫ���豸�������� */
}STREAM_TYPE_E;

/*
    ���ڡ�ʱ��
*/
typedef struct
{
    short year;                                     /* �� */
    short month;                                    /* �� */
    short dayOfWeek;                                /* 0:������ -6:������ */
    short day;                                      /* �� */
    short hour;                                     /* ʱ */
    short minute;                                   /* �� */
    short second;                                   /* �� */
    short milliSecond;                              /* ���� */
} DATE_TIME;

/*
    �ص�����ĸ�����Ϣ
*/
typedef struct _STREAM_ELEMENT_
{
    unsigned int    magic;                          /* ����������λ */
    unsigned int    id;                             /* ������Ϣ���� */
    unsigned int    chan;                           /* ͨ���� */
    unsigned int    type;                           /* ��������,STREAM_TYPE_E */

    unsigned int    standardTime;                   /* TS/PS��׼ʱ��,��λΪ90kHzʱ��,ȡ���32λ,��45kHzʱ�� */
    DATE_TIME       absTime;                        /* ����ʱ�� */
    unsigned int    dataLen;                        /* ������Ϣ���֡�ĳ��� */
    unsigned char   reserve[32];                    /* ���� */
} STREAM_ELEMENT;

/*
    �ص��������Ͷ���
*/
typedef void (*DATACALLBACK)
(
    STREAM_ELEMENT  *pEle,                          /* �ص��ĸ�����Ϣ */
    unsigned char   *buf,                           /* ������Ϣ�е� type ����Ӧ�Ľṹ��ĵ�ַ */
    unsigned int     bufLen                         /* ������Ϣ�е� type ����Ӧ�Ľṹ��Ĵ�С ��С���ԣ��ṹ����� */
);

/*
    �·����ݸ�DSP�����������أ���ͨ���ص��Ĺ����ڴ�ռ�
*/
typedef struct _PROC_SHARE_DATA_
{
   unsigned char*   appAddr;         /*app ���ʵ�ַ*/
   unsigned char*   dspAddr;         /*dsp ���ʵ�ַ*/
   unsigned char    reserve[32];     /* Ԥ�� */
}PROC_SHARE_DATA;

typedef struct _DSP_SHM_COMMON_
{
    unsigned int                bufSize;            /* �����ڴ洴�����ȣ� ����ͷ�� */
    unsigned int                bufUsed;            /* �����ڴ�����ʹ�ó��ȣ� ������ͷ�� */
    unsigned int                state;              /* �����ڴ����״̬ */
    unsigned int                offset;             /* ��������ƫ�� */
    unsigned int                cmd;                /* �洢HOST CMD */
    unsigned int                chn;                /* �洢HOST CMD CHN */
    unsigned int                prmLen;             /* ���ݲ����ĳ���(��������) */
    unsigned char               reserve[32];        /* Ԥ�� */
} DSP_SHM_COMMON;

/************************************************************************************************/
/*                                          ��Ӧ�ý�������                                      */
/************************************************************************************************/
/*
    ����������Ͷ���
*/
typedef enum
{
    PS_STREAM       = 0x1,                          /* PS��ʽ���  */
    TS_STREAM       = 0x2,                          /* TS��ʽ���  */
    RTP_STREAM      = 0x4,                          /* RTP��ʽ��� */
    HIK_STREAM      = 0x8,                          /* HIK��ʽ��� */
    ORG_STREAM      = 0x10,                         /* ORG��ʽ��� */
    MEGA_RTP_STREAM = 0x20,                         /* MEGA RTP��ʽ��� */
    MEGA_PS_STREAM  = 0x40,                         /* MEGA PS��ʽ���  */
    SP7_PS_STREAM   = 0x80,                         /* өʯps��ʽ��� */
} STREAM_PACKET_TYPE;

/*
    ��������
*/
typedef enum
{
    STREAM_VIDEO = 0x1001,                          /* ��Ƶ�� */
    STREAM_AUDIO = 0x1002,                          /* ��Ƶ�� */
    STREAM_MULTI = 0x1003,                          /* ������ */
    STREAM_PRIVT = 0x1004,                          /* ˽���� */
} STREAM_TYPE;

/*
    ����ͨ�������ͣ�������������������������
*/
typedef enum encChnTypeE
{
    ENC_MAIN   = 0,                              
    ENC_SUB,                                   
    ENC_THRD,
    ENC_MAX
}ENC_CHNTYPE;

/*
    ��Ƶ��������
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
    ENCODER_QUICK     = 0x7,                        /* ǳ����           */
    ENCODER_MF        = 0x8,                        /* ͼ��֡         */
    ENCODER_MFD4      = 0x9,                        /* �²���ͼ��MV���� */
    ENCODER_RAW       = 0xA,                        /* ����     */
    ENCODER_BUTT                                    /* δ֪��Ƶ��ʽ */
}VIDEO_STREAM_TYPE_E;

/*
    ��Ƶ�������� ��Ƶѹ���㷨
*/
typedef enum
{
    WAVE         = 0,
    G711_MU      = 1,                               /* G711��U�ɱ��� */
    G711_A       = 2,                               /* G711��A�ɱ��� */
    G722_1       = 3,                               /* G722     ���� */
    G723         = 4,                               /* G723     ���� */
    MP1L2        = 5,                               /* MP1      ���� */
    MP2L2        = 6,                               /* MP2      ���� */
    G726         = 7,                               /* G726     ���� */
    AAC          = 8,                               /* AAC      ���� */
    AAC_LD       = 9,                               /* AAC_LD   ���� */
    OPUS         = 10,                              /* OPUS     ���� */
    MP3          = 11,                              /* MP3      ���� */
    RAW          = 99,                              /* ����    �ޱ��� */
    AUD_ENC_MAX  = 0xffffffff
} AUDIO_ENC_TYPE;

/* ��Ƶ������ */
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
    DVPR_NEED_CONFIRM            = 0x80000,    /* ������û�м�⵽��Ч���������룬��ʱ�Ͽ�ǰ��˵һ�� */
    DVPR_TIME_OUT                = 0x80001,    /* ��ʱ�Ͽ�*/
    DVPR_REGIST_SINGEL           = 0x80002,    /* ����ע�ᵥ��ɹ�*/
    DVPR_REGIST_SOK              = 0x80003,    /* ����ע�Ὠģ�ɹ�*/
    DVPR_REGIST_FAIL             = 0x80004,    /* ����ע�Ὠģʧ��*/
    DVPR_RECOGNIZE_SOK           = 0x80005,    /* ����ʶ��ģ�ɹ�*/
    DVPR_RECOGNIZE_FAIL          = 0x80006,    /* ����ʶ��ģʧ��*/
    DVPR_MODEL_COMPARE           = 0x80007,    /* ����1v1�ȶԽ��*/
    DVPR_IMPORT_DATA_REGIST_SOK  = 0x80008,    /* ����������Ч���ݽ���ע�Ὠģ�ɹ�*/
    DVPR_IMPORT_DATA_REGIST_FAIL = 0x80009,     /* ����������Ч���ݽ���ע�Ὠģʧ��*/
    DVPR_AUDIO_ENERGY            = 0x8000a,     /* ��Ƶ������С*/
    DVPR_IMPORT_DATA_LEN_ILLEGAL = 0x8000b,     /* ����������Ч���ݵĳ��Ȳ�����Ҫ��*/
    DVPR_IMPORT_DATA_NUM_ILLEGAL = 0x8000c,     /* ����������Ч���ݵľ�����������Ҫ��*/
    DVPR_RECORD_AUDIO_DATA       = 0x8000d,     /* ����¼�����ݣ����ں������й����Ʋ���*/

}AUDIO_DVPR_CMDTYPE;


typedef enum
{
    LASR_NEED_CONFIRM            = 0x70000,   /* ���Ѻ�δ��⵽�ؼ��ʣ���ʱ�Ͽ�ǰ��˵һ�� */
    LASR_TIME_OUT                = 0x70001,   /* ��ʱ�Ͽ�*/
    LASR_WAKE_UP                 = 0x70002,   /* �������� */
    LASR_RECOGNIZE_SOK           = 0x70003,   /* ����ʶ��ɹ�*/
    LASR_RECOGNIZE_FAIL          = 0x70004    /* ����ʶ��ʧ��*/

}AUDIO_LASR_CMDTYPE;

/*
    ��������ʶ�𷵻ؽṹ��
*/
typedef struct _AUDIO_LASR_RESULT_
{
    AUDIO_LASR_CMDTYPE  cmdType;   /*״̬����*/
    int                 intent;    /* �����ͼ*/
    int                 slots;     /*������ֵ*/
    char                subSlots[128]; /*������ֵ*/
    
    unsigned char       reserve[32];
}AUDIO_LASR_RESULT;

/*����ʶ������¼��������Ƶ�������� HOST_CMD_SET_DVPR_RECORD_AUDIO*/
typedef struct tagDvprRecordAudioParam
{
    unsigned int        recordTime;     /*����¼�Ƶ���Ƶʱ������λms����Χ��0,16000��40ms���롣¼�Ƶ��㹻ʱ����ͻ�ص�APP���ݣ����ټ���¼��*/
    unsigned char       reserve[32];
}DVPR_RECORD_AUDIO_PARAM;

/*����ʶ�������������� HOST_CMD_SET_DVPR_KEYWORD*/
typedef struct tagDvprKeyWordParam
{
    unsigned char       keyword[8];     /*�·�������������(�ַ�������)��ex��5 9 3 6 0 7 4 8 (��Ҫʹ��1��2���㷨����)*/
    unsigned char       reserve[32];
}DVPR_KEYWORD_PARAM;

/*����ʶ�𣬲������� HOST_CMD_SET_DVPR_PRM*/
typedef struct tagDvprRegisterParam
{   
    unsigned int        timeout;      /* ��ʱ�Ͽ���ʱ��,��λΪ�����Ʒ�Χ[1, 15] */
    unsigned int        registerNum;  /*ע�������Ŀ, ���Ʒ�Χ[1, 5]*/
    unsigned char       reserve[32];
}DVPR_PARAM;

/*
    DVPR_VERSION  �㷨�汾 ���ؽṹ�� HOST_CMD_DVPR_GET_VERSION
*/
typedef struct tagDvprVersion
{
    unsigned char   versionInfo[64];                    /// ���汾�� �汾���� ex:V1.2.204 22-08-05
    
    unsigned char   reserve[256];
}DVPR_VERSION;

/*
    ����ģ��У�� HOST_CMD_DVPR_MODEL_CHECK
*/
typedef struct tagDvprModelCheckSt
{
    unsigned char  *modelAddr;              /* ��У���ģ��buf��ַ,Ӧ�÷����ڴ�*/
    unsigned int    modelLen;               /* ��У���ģ��buf����*/
    unsigned int    modelFit;               /* У���� 0��ʾ���� 1��ʾ������ 2��ʾδ����(��β���ȷ)*/
	unsigned char   reserve[24];
} DVPR_MODEL_CHECK_ST;
/*
    APP�·���Ƶ�ļ����й��������ܲ��� HOST_CMD_PASS_DVPR_AUDIO
*/
typedef struct tagDvprPassAudioSt
{    
    int                 idx;            			/*���*/
    unsigned int        dataType;                   /* �������ͣ�0Ϊ������ע�Ὠģ���ܲ��ԣ�1Ϊ��������֤��ģ���ܲ���*/
    unsigned char       keyword[8];                 /* ���ι��������ܲ��Թ����·�������������(�ַ�������)�����ڹ��������ܲ⣨ע�����֤����ʱ��ʹ�õ���ex��5 9 3 6 0 7 4 8 (��Ҫʹ��1��2���㷨���飬����������Դ�ǹ����Ƶ���Ƶ�ļ���)*/
    unsigned char      *audioPackAddr;              /* ��������Ƶ���ݴ���ļ�buf��ַ,Ӧ�÷����ڴ�*/
    unsigned int        audioPackLen;               /* ��������Ƶ���ݴ���ļ�buf����*/
    unsigned char       reserve[32];
} DVPR_PASS_AUDIO_ST;

/*
    ��������ʶ����Ч���ݽ���ע�� HOST_CMD_DVPR_DATA_IMPORT_REGISTER
*/
typedef struct tagDvprImportDataRegSt
{    
    int                 idx;            			/*���*/
    unsigned char      *audioPackAddr;              /* ��Ч���ݴ���ļ�buf��ַ,Ӧ�÷����ڴ�*/
    unsigned int        audioPackLen;               /* ��Ч���ݴ���ļ�buf����*/
	unsigned char       reserve[24];
} DVPR_IMPORT_DATA_REG_ST;

/*
    ����ʶ�𷵻ؽṹ��
*/
typedef struct _AUDIO_DVPR_RESULT_
{
    int                 idx;            /*���*/
    AUDIO_DVPR_CMDTYPE  cmdType;        /*״̬����*/
    unsigned char      *modelAddr;      /*����ģ���ļ���ַ*/
    unsigned int        modelLen;       /*����ģ���ļ�����*/
    float               similarity;     /*1VN�Ա�ʱ�����ƶ�,���Ʒ�Χ[0, 100]*/ 
    unsigned char      *audioPackAddr;  /*��Ƶ����ļ���ַ*/
    unsigned int        audioPackLen;   /*��Ƶ����ļ�����*/
    unsigned int        audioEnergy;    /*��Ƶ������С����Χ[1-100]*/
	unsigned char       reserve[128];
}AUDIO_DVPR_RESULT;

/*
    ��ȡ�������ƶȱȶ���ֵ
    HOST_CMD_DVPR_GET_SIM_THRESHOLD
*/
typedef struct _DVPR_SIM_THRESHOLD_PRM_
{
    float           simThreshold1v1;                                                  /* 1v1 �Ƽ��ȶ���ֵ */
    float           simThreshold1vn;                                                  /* 1vn �Ƽ��ȶ���ֵ  ����ǰ�㷨�ݲ�֧�֣�Ĭ�ϴ�0 */
    unsigned char   reserve[128];                                                     /* ���� */
}DVPR_SIM_THRESHOLD_PRM;

/*
    ����ģ��1V1�Ա� _DVPR_1V1_COMPARE_        HOST_CMD_DVPR_MODEL_COMPARE
*/
typedef struct _DVPR_1V1_COMPARE_
{
    unsigned char  *baseModelAddr;  /* �׿�ģ���ļ�buf��ַ,Ӧ�÷����ڴ�*/
    unsigned int    baseModelLen;   /* �׿�ģ���ļ�buf����*/
    unsigned char  *camModelAddr;   /* ���Ա�ģ���ļ�buf��ַ,Ӧ�÷����ڴ�*/
    unsigned int    camModelLen;    /* �׿�ģ���ļ�buf����*/
    float           similarity;     /* 1V1���ƶȣ����Ʒ�Χ[0, 100]*/
	unsigned char   reserve[32];
} DVPR_1V1_COMPARE;


typedef enum
{
    SPEECH_NEED_CONFIRM            = 0x40000,       /* ���Ѻ�δ��⵽�ؼ��ʣ���ʱ�Ͽ�ǰ��˵һ�� */
    SPEECH_TIME_OUT                = 0x40001,       /* ��ʱ�Ͽ�*/
    SPEECH_WAKE_UP                 = 0x40002,       /* �������� */
    SPEECH_ENTER_MANAGE_INTERFACE  = 0x40003,       /* ��/����/���� ������� */   
    SPEECH_EXIT_MANAGE_INTERFACE   = 0x40004,       /* �ر�/�ص�/�˳� ������� */ 
    SPEECH_ENTER_BACKGROUND        = 0x40005,       /* ��/����/���� ��̨ */
    SPEECH_EXIT_BACKGROUND         = 0x40006,       /* �ر�/�ص�/�˳� ��̨ */ 
    SPEECH_ENTER_CALLING_INTERFACE = 0x40007,       /* ��/����/���� ���� */  
    SPEECH_EXIT_CALLING_INTERFACE  = 0x40008,       /* �ر�/�ص�/�˳� ���� */ 
    SPEECH_OPEN_QR_CODE            = 0x40009,       /* ��/����/���� ��ά�� */   
    SPEECH_CLOSE_QR_CODE           = 0x4000A,       /* �ر�/�ص�/�˳� ��ά�� */ 
    SPEECH_CALL_ROOM_NUMBER        = 0X4000B,       /* ����/���/����/��绰�� ���� */
    SPEECH_CALL_PROPERTY           = 0x4000C,       /* ����/���/����/��绰�� ��ҵ, ��ҵ����*/
    SPEECH_CALL_MANAGE_CENTER      = 0x4000D,       /* ����/���/����/��绰�� �������� */
    SPEECH_INCREASE_NEW_USER       = 0x4000E,       /* ����/��� �û� */
    SPEECH_INCREASE_NEW_PERSONNEL  = 0x4000F,       /* ����/��� ��Ա */
    SPEECH_INCREASE_VOLUME         = 0x40010,       /* ���� ����/�Ӵ�/���� */
    SPEECH_DECREASE_VOLUME         = 0x40011,       /* ���� ����/��С/���� */
    SPEECH_MONITOR_UNIT_DOOR       = 0x40012,       /* ����/�鿴 ��Ԫ�ſڣ��� ����/��� */
    SPEECH_MONITOR_NURSE_STATION   = 0x40013,       /* ���л�ʿ / ���л�ʿվ / ��������/ ���л�ʿվ���� / ����ҽ��/�����ʿ/ �����ʿվ/���ҽ�� */
    SPEECH_MONITOR_ANSWER          = 0x40014,       /* ����/��ͨ/ͨ�� */
    SPEECH_MONITOR_HANG_UP         = 0x40015,       /* �Ҷ�/����ͨ�� */
    SPEECH_CALL_ELEVATOR           = 0x40016,       /* ����/���е��� */
    SPEECH_ANNOUNCEMENT_INFO       = 0x40017,       /* ������Ϣ/��ҵ���� */
    SPEECH_OPEN_LOCK               = 0x40018,       /* ���� */
    SPEECH_CLOSE_UP                = 0x40019,       /* �ر�/�˳�/�ټ� */
    SPEECH_OPEN_SOUND              = 0x4001A,       /* ������ */
    SPEECH_CLOSE_SOUND             = 0x4001B,       /* �ر����� */
}SPEECH_RECOGNITION_RESULT;

/* ���������ģʽ */
typedef enum
{
    SPEECH_RECOGNITION_MODE = 0x0,                   /*�� �ؼ���ʶ��ģʽWUW + KWS���᲻���ϱ�ʶ���� */
    SPEECH_CAPTURE_MODE     = 0x1,                   /* �ɼ�ģʽ�������ϱ�ʶ��������ģʽֻ���ڲɼ���Ƶ�Ż��㷨�� */
    SPEECH_ONLY_KWS_MODE    = 0x2,                   /* ֻ���йؼ���ʶ��ģʽ������Ҫ��������*/
    SPEECH_ONLY_LASR_MODE   = 0x3,                   /* ֻ������������ʶ��ģʽ������Ҫ��������*/
    SPEECH_WUW_LASR_MODE    = 0x4                    /*��������ʶ��ģʽWUW+LASR����Ҫ������������*/
    
}SPEECN_RECOG_TYPE;


/* ��������Ĳ��� */
typedef struct tagSpeechRecogParam
{
    SPEECN_RECOG_TYPE   type;                       /* ����ʶ����ģʽ��ʹ�� SPEECN_RECOG_TYPE */
    unsigned int        timeout;                    /* ��ʱ�Ͽ���ʱ��,��λΪ�� */
    unsigned char       serialNum[32];              /* �豸�����к�,�ɼ�ģʽ�·����� */
	unsigned char       recordTime;                 /* �ɼ�ģʽʱ������֪DSP��ǰ��wordId�ǵڼ���¼�� */
    unsigned int        wordId;                     /* �ؼ���ID  ,�ɼ�ģʽ�·����� */
    unsigned char       name[128];                  /* ��Ա����,�ɼ�ģʽ�·����� */
	unsigned int        recogMode;                  /* ʶ��ģʽ����: 0���Ѵ�ʶ��ģʽ��1�ؼ���ʶ��ģʽ������:��������ʶ��ģʽ*/
    unsigned char       reserve[28];
}SPEECH_RECOGNITION_PARAM;

/* 
    ��Ƶ�����Ϣ
*/
typedef struct tagVideoParamsSt
{
    VIDEO_STREAM_TYPE_E  videoEncType;              /* �����������ʽ */
    VIDEO_STREAM_TYPE_E  videoSubEncType;           /* �����������ʽ */
    STREAM_PACKET_TYPE  packetType;                 /* ��������װ��ʽ */
    STREAM_PACKET_TYPE  subPacketType;              /* ��������װ��ʽ */
    unsigned int        bWaterMark;                 /* ˮӡ */
}VIDEO_PARAM;

/*
    ��Ƶ���������Ϣ
*/
typedef struct tagAudioParamsSt
{
    AUDIO_ENC_TYPE encType;                         /* ��Ƶѹ���㷨 */
    unsigned int   samplesPerSecond;                /* ������ */
    unsigned int   samplesPerFrame;                 /* ÿ֡������ */
    unsigned int   boardSamplesPerSecond;           /* ����ԭʼ��Ƶ������ */
    unsigned int   encChans;                        /* �㷨ͨ������ͨ��Ϊ������ */
    unsigned int   bitRate;                         /* ������� */
} AUDIO_PARAM;

/*
    �����Խ������Ϣ
*/
typedef struct tagTalkBackParamsSt
{
    AUDIO_ENC_TYPE talkBackType;                    /* �����Խ�ѹ���㷨 */
    unsigned int   talkBackSamplesPerSecond;        /* �����Խ������� */
    unsigned int   talkBackSamplesPerFrame;         /* �����Խ�ÿ֡������ */
    unsigned int   boardOutSamplesPerSecond;        /* ����ԭʼ�����Ƶ������ */
    unsigned int   talkBackBitRate;                 /* �����Խ�������� */
    unsigned int   talkBackChans;                   /* �����Խ��㷨ͨ������ͨ��Ϊ������ */
    unsigned int   boardOutChans;                   /* �������ͨ������davinciΪ������ */
    unsigned int   audOutVolume;
}TALK_BACK_PARAM;


/*
    ��Ƶ����Ƶ�����������������Ϣ
*/
typedef struct tagStreamInfoSt
{
    VIDEO_PARAM      stVideoParam;                              /* ��Ƶ�������� */
    AUDIO_PARAM      stAudioParam;                              /* ��Ƶ�������� */
    TALK_BACK_PARAM  stTalkBackParam;                           /* �����Խ����� */
} STREAM_INFO_ST;

/*
    �����Խ����� ���ݻ�������Ϣ
*/
typedef struct tagAudioTBBufInfoSt
{
    volatile unsigned int totalFrame;                           /* ������֧�ֵ������Ƶ֡�� */
    volatile unsigned int frameLen;                             /* ��Ƶ��ÿһ֡���ݵĳ���   */
    volatile unsigned int hostWriteIdx;                         /* Ӧ��д���ݵ�֡���ļ���   */
    volatile unsigned int dspWriteIdx;                          /* DSPд���ݵ�֡���ļ���    */
    volatile unsigned int hostReadIdx;                          /* Ӧ�ö����ݵ�֡���ļ���   */
    volatile unsigned int dspReadIdx;                           /* DSP�����ݵ�֡���ļ���    */
    unsigned char         audTalkbackHost[TALKBACK_BUF_LEN];    /* Ӧ��д����Ƶ���ݵĻ����� */
    unsigned char         audTalkbackDsp[TALKBACK_BUF_LEN];     /* DSP д����Ƶ���ݵĻ����� */
} AUDIO_TB_BUF_INFO_ST;

/*
    ����Ƶͼ��Ļ�����Ϣ��ͼ���ʽ��Ӧ����ײ�Լ����������
*/
typedef struct tagCaptNoSignalInfoSt
{
    unsigned int uiNoSignalImgW[MAX_NO_SIGNAL_PIC_CNT];         /* ����Ƶͼ��Ŀ� */
    unsigned int uiNoSignalImgH[MAX_NO_SIGNAL_PIC_CNT];         /* ����Ƶͼ��ĸ� */
    unsigned int uiNoSignalImgSize[MAX_NO_SIGNAL_PIC_CNT];      /* ����Ƶͼ������ݴ�С */
    void        *uiNoSignalImgAddr[MAX_NO_SIGNAL_PIC_CNT];      /* ����Ƶͼ���������ʼ��ַ����ҪӦ�÷���ռ� */
} CAPT_NOSIGNAL_INFO_ST;

/*
    ������������RTPͷ�����Ϣ
*/
typedef struct
{
    unsigned int payload_type;                                  /* rtpͷpayload_type    */
    unsigned int seq_num;                                       /* rtpͷseq_num    */
    unsigned int time_stamp;                                    /* rtpͷtime_stamp    */
} RTP_INFO;

/*
    ¼������������ṹ��I֡�ľ�����Ϣ
*/
typedef struct
{
    volatile unsigned int uiAddr;                               /* ��֡��������ʼ��ַ */
    volatile unsigned int stdTime;                              /* ��׼ʱ�� */
    DATE_TIME             dspAbsTime;                           /* ����ʱ�� */
    volatile unsigned int len;                                  /* �������� */
} STREAM_IFRAME_INFO;

/*
    ¼������������ṹ��I֡��Ϣ
*/
typedef struct
{
    STREAM_IFRAME_INFO    ifInfo[REC_IFRM_INFO_NUM];            /* ��¼�µ�ÿһ��I֡��Ϣ */
    volatile unsigned int rIdx;                                 /* I֡��Ϣ������ */
    volatile unsigned int wIdx;                                 /* I֡��Ϣд���� */
} IFRAME_INFO_ARRAY;

/*
    ������������������ṹ
*/
typedef struct
{
    unsigned char       * vAddr;                    /* �ڴ�ӳ����������ַ */
    unsigned char       * pAddr;                    /* �����ַ */
    volatile unsigned int wIdx;                     /* ʵʱ������DSPд���� */
    volatile unsigned int delayIdx;                 /* ��ʱԤ����DSPдָ��*/
    volatile unsigned int totalWLen;                /* ��ʷ�����ܳ������� 0xff000000 ������ٽ�ֵ��������ͳ�� */
    volatile unsigned int totalLen;                 /* �ڴ��ܳ� */
    volatile unsigned int vFrmCounter;              /* ֡���� */
    volatile unsigned int streamType;               /* �������ͣ���Ƶ������������*/

    /* ����ĳ�Ա��NVR-DSP�����������ֲ���� */
    volatile unsigned int controlLevel;             /* �����ϴ����Ƽ��� ʹ��UPLOAD_CONTROL_LEVEL�Ķ��� */
    volatile unsigned int muxType;                  /* ��װ����*/
    volatile unsigned int extParam;                 /* ��չ������������չһЩ���ƹ���*/
    volatile RTP_INFO     videoInfo;                /* �洢��Ƶÿ֡��һ��rtp���еĹؼ���Ϣ*/
    volatile RTP_INFO     audioInfo;                /* �洢��Ƶÿ֡��һ��rtp���еĹؼ���Ϣ*/
    volatile RTP_INFO     privtInfo;                /* �洢˽��֡ÿ֡��һ��rtp���еĹؼ���Ϣ*/
} NET_POOL_INFO;

/*
    ����¼������������ṹ
*/
typedef struct
{
    unsigned char         *vAddr;                   /* �ڴ�ӳ����������ַ */
    unsigned char         *pAddr;                   /* �����ַ */
    volatile unsigned int  wOffset;                 /* ʵʱ������DSPдƫ��ֵ  */
    volatile unsigned int  rOffset;                 /* ʵʱ������Ӧ�ö�ƫ��ֵ */
    volatile unsigned int  totalLen;                /* �������ܴ�С */
    IFRAME_INFO_ARRAY      ifInfo;                  /* I֡��Ϣ      */
    volatile unsigned int  wErrTime;                /* д�ڴ������� */
    volatile unsigned int  lastFrameStdTime;        /* ���һ֡���ݵı�׼ʱ�� */
    DATE_TIME              lastFrameAbsTime;        /* ���һ֡���ݵľ���ʱ�� */
    volatile unsigned int  vFrmCounter;             /* ֡���� */
    volatile unsigned int  streamType;              /* �������ͣ���Ƶ������������ */
    volatile unsigned int  controlLevel;            /* �����ϴ����Ƽ��� ʹ��UPLOAD_CONTROL_LEVEL�Ķ��� */

    volatile unsigned int muxType;                  /* ��װ���� */
    volatile unsigned int bSubRec;                  /* �Ƿ�������¼��bSubRec =1:������¼��bSubRec =0:������¼�� */
    volatile unsigned int extParam;                 /* ��չ������������չһЩ���ƹ��� */
    volatile unsigned int streamBps;                /* ��¼��ͨ��������ͳ����Ϣ */
    volatile unsigned int curAddrId;                /* ��¼��ǰ¼��ʹ�õ����ĸ���ַ����IPCͨ����Ч */
} REC_POOL_INFO;

/*
    Ӧ���·������������Ĺ�������
*/
typedef struct DEC_SHARE_BUF_tag
{
    unsigned char         * pPhysAddr;              /* DSP���ʵ�ַ */
    unsigned char         * pVirtAddr;              /* ARM���ʵ�ַ */
    volatile unsigned int   bufLen;                 /* ���뻺�������� */
    volatile unsigned int   writeIdx;               /* ���뻺��д���� */
    volatile unsigned int   readIdx;                /* ���뻺������� */
    volatile unsigned int   decodeStandardTime;     /* ���������ʱ�� */
    volatile unsigned int   decodeAbsTime;          /* ����������ʱ�� */
    volatile unsigned int   decodePlayBackStdTime;  /* �������������ʱ�� */
    int                     bIFrameOnly;            /* �Ƿ�ֻ��I֡ */
}DEC_SHARE_BUF;

/*
    Ӧ���·�������JPG
*/
typedef struct DEC_JPG_TRANS_PRM_tag
{
    unsigned int          decodeType;            /* Ӳ�����뻹���������0-Ӳ������1-�������*/
    unsigned int          transType;             /* ת���ɵ�����0-YUV-NV12 1-BMP24bit*/
    unsigned char*        virtAddr;              /* JPGͼ���ַ��Ӧ������*/
    unsigned int          jpegLen;               /* JPGͼ�񳤶ȣ�Ӧ������*/
    unsigned char*        addr[4];               /* �����ͼ���ַ��Ӧ������*/
    unsigned int          decBufLen;             /* �����ͼ���ַ���ȣ�Ӧ������*/
    unsigned int          width;                 /* ���������ͼ����DSP���*/
    unsigned int          height;                /* ���������ͼ��߶�DSP���*/
    unsigned int          dataLen;               /* ����ת�����������ݳ��ȣ�DSP���*/

    unsigned int          res[16];
}DEC_JPG_TRANS_PRM;

/* оƬ����ö�� */
typedef enum 
{
    DSP_CHIP_NONE = 0,                              /*��*/
    DSP_CHIP_MA,                                    /*MAϵ��оƬ*/
    DSP_CHIP_MX,                                    /*MXϵ��оƬ*/
    DSP_CHIP_NT,                                    /*NTϵ��оƬ*/    
    DSP_CHIP_NT_F1x,                                /*NT��F1xϵ��оƬ*/
    DSP_CHIP_R2,                                    /*R2оƬ*/
    DSP_CHIP_A2S,                                   /*A2SоƬ*/
    DSP_CHIP_MAX = 0xffffffff,
}DSP_CHIP_TYPE;

/*
    FR_VERSION  �Ա� ���ؽṹ��
*/
typedef struct tagFrVersion
{
    /* ����汾��Ϣ */
    int             mVersion;                       /* ���ܿ�����汾 */
    int             subVersion;                     /* ���ܿ���Ӱ汾 */
    int             version;                        /* ���ܿ�������汾 */
    int             buildData;                      /* ���ܿ����ʱ�� */
    int             chipType;                       /* ����оƬ���� ʹ��DSP_CHIP_TYPE */
    int             isSupportModelN;                /* �ײ��Ƿ�֧�ֽ���N�⣬���֧��APP�����·�����N�⣬�ײ��Զ�������1VN�����ƶȣ� 0 ��֧�� 1 ֧�� */

    /* �㷨�汾��Ϣ */
    char            algoName[64];                   /* �㷨����*/
    unsigned int    mainVersion;                    /* ���屾��*/
    unsigned int    minorVersion;                   /* �ΰ屾��*/
    unsigned int    reVersion;                      /* �����屾��*/
    char            platName[64];                   /* ��оƬ����*/
    char            sysInfo[32];                    /* ƥ���ϵͳ����*/
    char            accurary[32];                   /* �㷨����*/
    char            encryption[32];                 /* ���ܷ�ʽ*/
    char            buildTime[32];                  /* �汾��������*/
    char            versionProperties[32];          /* �汾����*/

    /* �궨��汾��Ϣ */
    int             matchVersion;                   /* ƥ���汾��Ϣ*/
    int             matchBuildDate;                 /* ƥ����������*/
    int             matchSvn;                       /* ƥ���SVN�汾��*/

    /* ���л���汾��Ϣ */
    unsigned char   faceLdVersion[32];              /* �����汾��Ϣ */
    unsigned char   faceLdBuildData[32];            /* ��������ʱ�� */

    unsigned char   reserve[192];
}FR_VERSION;

/*
    ����ṹ�������ڻ�Ӧ�����������ڴ����Զ�������õġ����ڻ������ڴ���٣�Ӧ���޷��Լ������ڴ棬��Ҫdsp�ͷ�����ڴ�֮��������뷵�ظ�����
    HOST_CMD_DSP_HDAL_MEM_ALLOC  �ӿڸ��øýṹ��ʱpAddr4free�������á�
*/
typedef struct _MEM_INFO_
{
    unsigned int    allocSize;                      /*��Ҫ������ڴ��С, ��MΪ��λ*/    
    unsigned int    pPhyAddr;                       /*��Ӧ�������ַ*/
    void*           pVirAddr;                       /*��Ӧ�������ַ*/
    void*           pAddr4free;                     /*�����ͷ��ڴ��dsp�ĵ�ַ*/
}MEM_INFO;


/*
    DSPϵͳģ������״̬
*/
typedef struct tagSysStatusSt
{
    unsigned int dspMainPid;                        /* DSP main ������PID��*/
    unsigned int haveCamera;                        /* �Ƿ����camera*/
    unsigned int useDfr;                            /* �Ƿ�ʹ��dfr�㷨*/
    unsigned int dfrRun;                            /* DFR���Ƿ��Ѿ���������*/
    FR_VERSION   frVersion;                         /* DFR �汾��*/

    unsigned int dfrLicence;                        /* ��Ȩ״�� */
    int          camIdx;                            /* ���õ�camera���� */

    unsigned int cameraError;                       /* ��ʱ��ɼ�������Ƶ������DSP  */
    int          errType;                           /* DSP �ϵĴ�������֪ͨӦ�ô��� */
    unsigned int useOsd;                            /* OSD ģ��Ŀ��� */
}SYS_STATUS;

/*
    DSP�ɼ�ģ������״̬
*/
typedef struct tagCaptStatusSt
{
    volatile unsigned int bHaveSignal;              /* �Ƿ�����Ƶ�ź�����,1 ��ʾ�У�0 ��ʾû�� */
} CAPT_STATUS;

/*
    DSP����ͨ������״̬
*/
typedef struct
{
    volatile unsigned int encodeW;                  /* ��ǰ��Ƶ֡�� */
    volatile unsigned int encodeH;                  /* ��ǰ��Ƶ֡�� */
    volatile unsigned int encodeSubW;               /* ��ǰ��������Ƶ֡�� */
    volatile unsigned int encodeSubH;               /* ��ǰ��������Ƶ֡�� */
    volatile unsigned int encodedV;                 /* �ѱ������Ƶ֡ �ײ�ͳ�� */
    volatile unsigned int encodedSubV;              /* �ѱ������ͨ����Ƶ֡ �ײ�ͳ�� */
    volatile unsigned int encodedA;                 /* �ѱ������Ƶ֡ �ײ�ͳ�� */
    volatile unsigned int bpsV;                     /* ��Ƶ�������ײ�ͳ�� */
    volatile unsigned int bpsA;                     /* ��Ƶ���� �ײ�ͳ�� */
    volatile unsigned int bps;                      /* ����  �ײ�ͳ�� */
    volatile unsigned int fpsEncV;                  /* ��Ƶ����֡�� �ײ�ͳ�� */
    volatile unsigned int fpsEncA;                  /* ��Ƶ����֡�� �ײ�ͳ�� */
} ENC_STATUS;

/*
    DSP����ͨ������״̬
*/
typedef struct
{
    volatile unsigned int status;                   /* ��ǰ״̬ */
    volatile unsigned int receivedData;             /* ���յ������� */
    volatile unsigned int invlidData;               /* ��Ч������ */
    volatile unsigned int decodedV;                 /* �������Ƶ֡ */
    volatile unsigned int decodedA;                 /* �������Ƶ֡ */

    volatile unsigned int decodedW;                 /* ��ǰ������Ƶ֡�� */
    volatile unsigned int decodedH;                 /* ��ǰ������Ƶ֡�� */
}DEC_STATUS;

/*
    IPC ͨ������״̬
*/
typedef struct
{
    volatile unsigned int status;                   /* ��ǰ״̬         */
    volatile unsigned int receivedData;             /* ���յ�������     */
    volatile unsigned int invlidData;               /* ��Ч������       */
    volatile unsigned int ipcDecV;                  /* IPC ����Ƶ֡     */
    volatile unsigned int ipcDecA;                  /* IPC ����Ƶ֡     */
    volatile unsigned int ipcDecW;                  /* ��ǰ������Ƶ֡�� */
    volatile unsigned int ipcDecH;                  /* ��ǰ������Ƶ֡�� */
} IPC_STATUS;

/*
    DSP��ʾͨ������״̬
*/
typedef struct
{
    volatile unsigned int fps;                      /* ʵʱ��ʾ֡�� */
    volatile unsigned int uiDispW;                  /* ʵʱ��ʾ�� */
    volatile unsigned int uiDispH;                  /* ʵʱ��ʾ�� */
} DISP_STATUS;


/****************************************************************************/
/*                                 ͨ��                                     */
/****************************************************************************/
/*
    ��DSP��������������
*/
typedef enum dspPicFormatE
{
    DSP_PIC_FORMAT_YUV  = 0x0,                      /*  YUV  ����  */
    DSP_PIC_FORMAT_JPEG,                            /*  JPEG ͼƬ  */
    DSP_PIC_FORMAT_BMP,                             /*  BMP ͼƬ   */
    DSP_PIC_FORMAT_PNG,                             /*  PNG ͼƬ   */
    DSP_PIC_FORMAT_MAX,
}DSP_PIC_FORMAT_E;

/*
    ��DSP��������������
*/
typedef struct dspPicInfoSt
{
    DSP_PIC_FORMAT_E  picFormat;                    /* ���ݸ�ʽ   */
    unsigned int      picWidth;                     /* ͼƬ�Ŀ�� */
    unsigned int      picHeight;                    /* ͼƬ�ĸ߶� */
    
    unsigned char   * picAddr;                      /* ͼƬ�ĵ�ַ */
    unsigned int      picSize;                      /* ͼƬ�Ĵ�С */
}DSP_PIC_INFO;


/*  REGION ����������Ϣ */
typedef struct stRegionInfoSt
{
    /* ��������Ϊͼ���һ����İٷֱ� */
    float  x;                                       /* ���� x */
    float  y;                                       /* ���� y */
    float  w;                                       /* ���   */
    float  h;                                       /* �߶�   */
}REGION_INFO;

/****************************************************************************/
/*                                 �ɼ�                                     */
/****************************************************************************/
/* ��Ƶ��׼ */
typedef enum VsStandardE
{
    VS_STD_PAL      = 0,                            /* PAL��  */
    VS_STD_NTSC     = 1,                            /* NTSC�� */
    VS_STD_BUTT,
}VS_STANDARD_E;

/* Ӧ�ó����Ͱ�ȫ�� */
typedef enum 
{
    ld_indoor_low      = 0,                         /* ���� ��ȫ���� */
    ld_indoor_middle   = 1,                         /* ���� ��ȫ���� */
    ld_indoor_high     = 2,                         /* ���� ��ȫ���� */
    ld_outdoor_low     = 3,                         /* ���� ��ȫ���� */
    ld_outdoor_middle  = 4,                         /* ���� ��ȫ���� */
    ld_outdoor_high    = 5,                         /* ���� ��ȫ���� */
    ld_SCENCE_NONE,
}LD_SCENE;

/*
    ���òɼ�ͨ������ HOST_CMD_SET_VIDEO_PARM
*/
typedef struct tagVideoChnPrmSt
{
    unsigned int  x;                                /* �����Ƶ�����, x     */
    unsigned int  y;                                /* �����Ƶ�����, y     */
    VS_STANDARD_E eStandard;                        /* ��Ƶ��׼,�ͷֱ������� */
    unsigned int  eResolution;                      /* �ֱ���ö��ֵ          */
    unsigned int  fps;                              /* �����Ƶ��֡��        */
}VIDEO_CHN_PRM_ST;

/*
    ��Ƶ��ת
*/
typedef enum RotateE
{
    ROTATE_MODE_NONE  = 0,                          /* ����ת        */
    ROTATE_MODE_90    = 1,                          /* ˳ʱ����ת90  */
    ROTATE_MODE_180   = 2,                          /* ˳ʱ����ת180 */
    ROTATE_MODE_270   = 3,                          /* ˳ʱ����ת270 */
    ROTATE_MODE_COUNT,                              /* ��ת180����ʹ�� ������ʵ�� */
} ROTATE_MODE;


/*
    ���òɼ�ͨ����ת���� HOST_CMD_SET_VIDEO_ROTATE
*/
typedef struct tagVideoRotateAttrSt
{
    unsigned int uiChan;                            /* ָ����ͨ�� ,���ں� DISP_REGION �ĸ���ͨ����Ӧ , �������ø���Ԥ��ͨ�������ľ�������*/ 
    ROTATE_MODE eRotate;                            /* ��ת���� */
}VIDEO_ROTATE_ATTR_ST;


/*
    ��Ƶ����
*/
typedef enum MirrorE
{
    MIRROR_MODE_NONE        = 0,                    /* �������� */
    MIRROR_MODE_HORIZONTAL  = 1,                    /* ˮƽ����/���Ҿ��� */
    MIRROR_MODE_VERTICAL    = 2,                    /* ��ֱ����/���¾��� */
    MIRROR_MODE_CENTER      = 3,                    /* ���ľ���/����͵ߵ�/��ת180��  */
    MIRROR_MODE_COUNT,
} MIRROR_MODE;


/*
    ���òɼ�ͨ����������  HOST_CMD_SET_VIDEO_MIRROR
*/
typedef struct tagVideoMirrorAttrSt
{
    unsigned int uiChan;                            /* ָ����ͨ�� ,���ں� DISP_REGION �ĸ���ͨ����Ӧ , �������ø���Ԥ��ͨ�������ľ�������*/ 
    MIRROR_MODE eMirror;                            /* �������� */
}VIDEO_MIRROR_ATTR_ST;

/*
    ����ͷ�궨����
*/
typedef struct
{
    double left_cam_K[9];                           /*������ڲ�*/
    double left_cam_D[8];                           /*������������*/
    double right_cam_K[9];                          /*������ڲ�*/
    double right_cam_D[8];                          /*������������*/
    double extrinsic_R[9];                          /*left_right ������R*/
    double extrinsic_t[3];                          /*left_right ������t*/
    double left_P[9];                               /*��ͶӰ����*/
    double right_P[9];                              /*��ͶӰ����*/
}CAM_CALIB_PARAM;

/*
   ����ͷ��ͨ���ϵİ�װ����
*/
typedef struct
{
    double          yaw;                            /* ������������բ������ǰ����ˮƽ��ʱ��ƫ���ǣ��Ƕ����� */
    double          pitch;                          /* ������������բ������ǰ���Ĵ�ֱ��ʱ�븩���ǣ��Ƕ����� */
    double          cam2edge;                       /* ��������ĵ�բ���ڱ�Ե�ľ��� */
    double          widthChannel;                   /* բ��ͨ���Ŀ�� */
    double          criticalWidth;                  /* �ٽ��ȣ������жϵ��ٽ����� */
    double          reserve[8];                         /* ������Ԥ���ֶ� */
}CAM_INSTALL_PARAM;

// TOF��Ŀ�ڲ�
typedef struct tagTofCamSvParamSt
{
    float M[3][3];      // ����ڲξ���
    float D[8];         // ����������
} TOf_CAM_SV_PARAMS_ST;

// TOF˫Ŀ�ڲ�
typedef struct tagTofCamBvParamSt
{
    TOf_CAM_SV_PARAMS_ST left;    // �������Ŀ�ڲ�
    TOf_CAM_SV_PARAMS_ST right;   // �������Ŀ�ڲ�
    float R[3][3];                // �������ת����
    float T[3];                   // �����ƽ�ƾ���
    float R_left[3][3];           // �������ת����
    float R_right[3][3];          // �������ת����
    float P_left[3][4];           // �����ͶӰ����
    float P_right[3][4];          // �����ͶӰ����
    float Q[4][4];                // ��ͶӰ����
} TOf_CAM_BV_PARAMS_ST;

typedef struct tagTofCalibSt
{
    unsigned int flag;              /* 0������RGB��1������IR��2��RGB��IR������ */
    TOf_CAM_BV_PARAMS_ST tofvsRGB;  /* tof��RGB�궨���� */
    TOf_CAM_BV_PARAMS_ST tofvsIR;   /* tof��IR �궨���� */
}TOF_CALIB_ST;

/*
    �ü�����
*/
typedef struct tagCropInfoSt
{
    unsigned int  uiEnable;                         /* �Ƿ�ʹ�ܲü� */
    unsigned int  channel;                          /* ��ROI������channelͨ��ʹ�� */
    unsigned int  uiX;                              /* ��ʼ���� x */
    unsigned int  uiY;                              /* ��ʼ���� y */
    unsigned int  uiW;                              /* �ü���     */
    unsigned int  uiH;                              /* �ü���     */
    unsigned char reserve[32];
}CROP_INFO_ST;

/*
    ��Ƶ �豸���� ģʽ
*/
typedef enum eAudioWorkMode
{
    AUDIO_WORK_MODE_FULL_DIGITAL = 0,               /* ȫ�����豸 */
    AUDIO_WORK_MODE_SEMI_DIGITAL = 1,               /* �������豸 */
    AUDIO_WORK_MODE_INVALID                         /* ��Ч  �豸 */
}AUDIO_WORK_MODE;

/*
    ��Ƶ CODEC ����
*/
typedef enum eACodecType
{
    AUDIO_CODEC_TYPE_NONE  = 0,                     /* CODEC ��   */
    AUDIO_CODEC_TYPE_INNER = 1,                     /* CODEC �ڲ� */
    AUDIO_CODEC_TYPE_EXTER = 2,                     /* CODEC �ⲿ */
    AUDIO_CODEC_TYPE_INVALID                        /* CODEC ��Ч */
}AUDIO_CODEC_TYPE;

/*
��������������
*/
typedef enum
{
    AEC_TYPE_HARD = 0x0,                            /*Ӳ��������  */
    AEC_TYPE_SOFT = 0x1,                            /*���������  */
    AEC_MODE_INVALID                                /* ��Ч  ���� */
}AEC_TYPE; 

/*
    ��Ƶģ���ʼ����Ϣ
*/
typedef struct tagAudioInitInfoSt
{
    unsigned int     aiChn;                         /* ai ͨ����        */
    unsigned int     aoChn;                         /* ao ͨ����        */
    unsigned int     eWorkMode;                     /* �豸��Ƶ����ģʽ��ʹ�� AUDIO_WORK_MODE ��*/
    unsigned int     eCodeType;                     /* ��Ƶ CODEC ���ͣ�ʹ�� AUDIO_CODEC_TYPE */
    unsigned int     eAecType;                      /* ���������ͣ�ʹ�� AEC_TYPE */
    unsigned char    reserve[64];
}AUDIO_INIT_INFO_ST;

/*
    �ɼ�ͨ������
*/
typedef struct tagVideoInputInitPrmSt
{
    unsigned int   eViResolution;                   /* ��Ƶ����ֱ���            */
    VS_STANDARD_E  eViStandard;                     /* ������Ƶ��ʽ������30/25֡ */
    CROP_INFO_ST   stCropInfo;                      /* ��Ƶ�ɼ��ü�����          */
}VI_INIT_PRM;

/*
    �ɼ�ģ�����Ϣ
*/
typedef struct tagVideoInputInitInfoSt
{
    unsigned int  uiViChn;                          /* vi ͨ����        */
    VI_INIT_PRM  stViInitPrm[MAX_CAPT_CHAN];        /* ÿ��viͨ������Ϣ */
}VI_INIT_INFO_ST;

/*
    �ɼ�ģ���̬����Ϣ HOST_CMD_SET_VIDEO_WDR
*/
typedef struct tagVideoWDRPrmSt
{
    unsigned int  uiEnable;                         /* �Ƿ�ʹ�� */
}VI_WDR_PRM_ST;

/*
    �����ɼ����ݻص��Ĳ������� HOST_CMD_START_VIDEO_CB
*/
typedef struct tagVideoInputCbInfo
{
    int width;          /* �ɼ����ݻص��Ŀ� */
    int height;         /* �ɼ����ݻص��ĸ� */
    unsigned char  reserve[64]; /* ���� */
}VI_CB_INFO_ST;

/*
    �ɼ����ݻص��ṹ�� STREAM_ELEMENT_CAPT_CB
*/
typedef struct tagVideoInputCbResult
{
    char       * frameAddr;    /* �ص��Ĳɼ����ݵĵ�ַ */
    unsigned int frameSize;    /* �ص��Ĳɼ����ݵĴ�С */
    unsigned char reserve[64]; /* ���� */
}VI_CB_RESULT_ST;

/*
    �������ĵ�ƫ������ CENTER_OFFSET_ST
*/
typedef struct tagCenterOffsetInfoSt
{
    unsigned int  uiEnable;                         /* �Ƿ�ʹ��ƫ�ƽ��� */
    int  uiCenterX;                                 /* ����ƫ�ƺ�����     */
    int  uiCenterY;                                 /* ����ƫ��������     */
	unsigned char reserve[64]; 						/* ���� */
}CENTER_OFFSET_ST;

/****************************************************************************/
/*                                 ISP                                      */
/****************************************************************************/
/*
    ISP ͸��
    HOST_CMD_SET_ISP_PARAM
    HOST_CMD_GET_ISP_PARAM
*/
typedef struct _ISP_PARAM
{
    unsigned int cmd;
    unsigned int value;
}ISP_PARAM;


/* ����ƿ��� */
typedef struct _LIGHTCTRL_PARAM_
{
    unsigned int ctrlType;                          /* 0 Ϊ�Զ���������,1 Ϊ�ֶ��������Ƚ�����˸,2 Ϊ�ֶ���������ȫ��*/
    unsigned int luminance;                         /* ��Χ 0-100���Զ��������ʱ: ��������ֵ���ֶ�����ʱ: ָ����ֵ */
}LIGHTCTRL_PARAM;

/****************************************************************************/
/*                                 OSD                                     */
/****************************************************************************/
#define  OSD_MAX_LINE                (16)                       /* OSD���������     */
#define  MAX_CHAR_PER_LINE           (960/8)                    /* OSDÿ������ַ��� */

#define OSD_MAX_IMG_W                704                        /* ��������ͼ����*/
#define OSD_MAX_IMG_H                576                        /* ��������ͼ��߶�*/
#define OSD_TAG_CNT                  13                         /* �����ַ�����*/
#define OSD_MAX_CHAR_PER_LINE        (OSD_MAX_IMG_W / 8)        /* 4CIFʱ ȫӢ���ַ�*/
#define OSD_CHAR_MAX_W               64                         /* �ַ������ */
#define OSD_CHAR_MAX_H               64                         /* �ַ������ */
#define OSD_TAG_CODE_BASE            0x9000                     /* �����ַ���ַ*/
#define OSD_YEAR4                    (OSD_TAG_CODE_BASE + 0)    /* 4λ�꣺2002*/
#define OSD_YEAR2                    (OSD_TAG_CODE_BASE + 1)    /* 2λ�꣺02	*/
#define OSD_MONTH3                   (OSD_TAG_CODE_BASE + 2)    /* 3λ�·ݣ�JAN��DEC*/
#define OSD_MONTH2                   (OSD_TAG_CODE_BASE + 3)    /* 2λ�·ݣ�1��12*/
#define OSD_DAY                      (OSD_TAG_CODE_BASE + 4)    /* 2λ���ڣ�1��31*/
#define OSD_WEEK2                    (OSD_TAG_CODE_BASE + 5)    /* 2λ����(�ַ����Ϊ8)*/
#define OSD_WEEK3                    (OSD_TAG_CODE_BASE + 6)    /* 3λ����(�ַ����Ϊ8)*/
#define OSD_CWEEK1                   (OSD_TAG_CODE_BASE + 7)    /* 1λ����(�ַ����Ϊ16)*/
#define OSD_HOUR24                   (OSD_TAG_CODE_BASE + 8)    /* 2λ24ʱ��Сʱ��00��23	*/
#define OSD_HOUR12                   (OSD_TAG_CODE_BASE + 9)    /* 2λ12ʱ��Сʱ��00��12*/
#define OSD_MINUTE                   (OSD_TAG_CODE_BASE + 10)   /* 2λ����00��59*/
#define OSD_SECOND                   (OSD_TAG_CODE_BASE + 11)   /* 2λ��00��59*/
#define OSD_AMPM                     (OSD_TAG_CODE_BASE + 12)   /* AMPM*/

#define FONT8_CODE_BASE              (OSD_TAG_CODE_BASE + OSD_TAG_CNT + 10)                  /*8��16�����ַ*/
#define FONT16_CODE_BASE             (FONT8_CODE_BASE + 88 * OSD_MAX_LINE * 4)               /*16��16�����ַ*/
#define FONT_LIB_SIZE                (OSD_MAX_CHAR_PER_LINE * OSD_MAX_LINE * OSD_CHAR_MAX_H) /* �����ֿ�Ĵ�С����16������Ϊ22k��С */

/*
    OSD������ʹ�õ��ֿ���Ϣ����Ӧ���·�
*/
typedef struct tagFontLibInfoSt
{
    unsigned int fontAscLib[4096 / 4];              /* Ŀǰ�Ĵ�СΪ 4096 / 4   */
    unsigned int fontHzLib[267616 / 4];             /* Ŀǰ�Ĵ�СΪ 267616 / 4 */
} FONT_LIB_INFO_ST;

/*
    OSD������ʹ�õ��ֿ���ϢV2�汾��ʹ��GBKȡ��HZK�����ڸ��µ�GBK�ڴ�ϴ󣬶���Ϊָ�����ͣ�������Ӧ����ʹ��ʱ�����Ƿ���Ҫ�����ڴ�
*/
typedef struct tagFontLibInfoStV2
{
    unsigned char  *pfontAscLib;            /* ASC�ֿ�   */
    unsigned char  *pfontGbkLib;            /* GBK�ֿ� */
	unsigned int    AscLen;                 /* ASC�ֿⳤ�ȣ���APP��ȡ�ֿ��С���� */
    unsigned int    GbkLen;                 /* GBK�ֿⳤ�ȣ���APP��ȡ�ֿ��С���� */
	unsigned char  reserve[56];				/* ���� */
} FONT_LIB_INFO_ST_V2;

/*
    ÿһ��OSD����Ϣ
*/
typedef struct tagOsdLineParamSt
{
    unsigned int    charCnt;                        /* ��ǰ���ַ����� */
    unsigned int    top;                            /* �߶�           */
    unsigned int    hScale;                         /* ˮƽ���ű�����0 �Ŵ�һ����1 �Ŵ�������2 �Ŵ��ı� */
    unsigned int    vScale;                         /* ��ֱ���ű�����0 �Ŵ�һ����1 �Ŵ�������2 �Ŵ��ı� */

    /* �ַ�����ֵ��0��255��ASCII�룻
        OSD_TAG_CODE_BASE��OSD_TAG_CODE_BASE+OSD_SPECIAL_CHAR_COUNT��1�������ַ�
        0xa1a1~ ���������룻
        0xa7a1-0xa7c1,0xa7d1-0xa7f1:��������
     */
    unsigned int    code[MAX_CHAR_PER_LINE];
} OSD_LINE_PARAM;

/*
    OSD��������
    HOST_CMD_SET_ENC_OSD
    HOST_CMD_SET_DISP_OSD
*/
typedef struct tagOsdConfigSt
{
    unsigned int   bStart;                          /* �Ƿ�����ʾ   */
    unsigned int   flash;                           /* ��˸��8λΪ��ʾʱ�䣬��8λΪֹͣʱ�䣬������ʾ����ʵ����˸������flash��(2<<8)|1 ��ʾLogo��ʾ2�룬ֹͣ1��*/
    unsigned int   side;                            /* �Ƿ�Ҫ����  */ 
	int            bAutoAdjustLum;                  /* �Ƿ��Զ��������ȣ��Զ�����ʱ��ָ��������ֵ��Ч */
    unsigned int   lum;                             /* OSD������ֵ��   */
    int            bTranslucent;                    /* �Ƿ��͸����    */
    OSD_LINE_PARAM line[OSD_MAX_LINE];              /* ÿһ��OSD����Ϣ */
} OSD_CONFIG;


#define MAX_LOGO_NUM_PER_CHAN       (4)             /* ÿ��ͨ����ຬ�ĸ�LOGO */

/*
    LOGO��Ϣ����
*/
typedef struct tagLogoPrmSt
{
    unsigned int   flash;                           /* ��˸���ơ�����flash��(2<<8)|1 ��ʾLogo��ʾ2�룬ֹͣ1�� */
    unsigned int   maskY;                           /* ����ɫ����LOGOͼ���е�ֵ������ɫ���ʱ��LOGO͸������ʾԭͼ�� */
    unsigned int   maskU;
    unsigned int   maskV;
    unsigned int   bTranslucent;                    /* �Ƿ��͸�� */
}LOGO_PARAM;

#define LOGO_MAX_W          256
#define LOGO_MAX_H          128

/*
    LOGO��Ϣ����
    HOST_CMD_SET_ENC_LOGO
*/
typedef struct
{
    unsigned int   idx;                             /* ��ǰLOGO����� �����Ϊ MAX_LOGO_NUM_PER_CHAN */
    unsigned int   x;
    unsigned int   y;
    unsigned int   w;                               /* ������LOGO_MAX_W����ΪLOGO_H_ALIGN�������� */
    unsigned int   h;                               /* ������LOGO_MAX_H����ΪLOGO_V_ALIGN�������� */

    unsigned int   uiIsPic;                         /* �Ƿ�ʹ��ͼƬ��0:ʹ��ɫ�飬 1:ʹ��ͼƬ */

    LOGO_PARAM   param;                             /* ��ɫ������ */
    unsigned int uiPicAddr;                         /* ͼƬ�����ڴ��ַ */
    unsigned int uiPicSize;                         /* ͼƬ���ݴ�С     */
} LOGO_CONFIG;

/*
DSP ��������Ϣ
*/
typedef struct
{
    float   x;
    float   y;
}DSP_COORDINATES;

/*
OCR ÿ�е���Ϣ
*/
typedef struct
{
    char             strings[DSP_MAX_OCRCHAR_NUM*4];                /*���128�ַ������� ���ֵ� ��8bit�Ŀռ�*/    
    unsigned int     charNum;                                       /*��Ч�ַ�����*/
    DSP_COORDINATES  points[DSP_MAX_POLYGON_NUM];                   /*�������򶥵�����*/
    unsigned int     pointsNum;                                     /*��Ч�������*/
    float            confidence;                                    /*���е����Ŷ�*/
    unsigned int     blob_type;                                     /*ҵ�����ʱ�����*/
    char             reserved[28];
}OCR_LINE;

/*
���֤�����գ����� ��Ϣ
*/
typedef struct
{
    OCR_LINE         stOcrLine[DSP_MAX_OCRLINE_NUM];                /*ocr ��Ϣ*/
    unsigned int     lineNum;                                       /*ocr ��Ч��Ϣ��*/
    char            *cOcrAddr;                                      /* �ص���OCR����JPEGͼƬ�ĵ�ַ */
    unsigned int    uiOcrSize;                                      /* �ص���OCR����JPEGͼƬ�Ĵ�С */ 
    char             reserved[24];
}OCR_CONTENT;

/*
һ��ͼ����ʶ�������Ϣ����
*/
typedef struct
{
    OCR_CONTENT      stOcrContent[DSP_MAX_OCR_NUM];
    char             reserved[32];
}OCR_RESULT;

/****************************************************************************/
/*                                 ����                                      */
/****************************************************************************/
/*
    �ɼ���������ͨ������Դ���ò���;
    ��Ӧ��������HOST_CMD_SET_SUB_ENCODE_PRIV��ENCODER_PARAM_PRIV
    �ṹ���switchViSrc������ֵ
*/
typedef enum eSubEncodePrivSrc
{
	ENCODER_SRC_WL			   	= 0, 				/* �ɼ���ͼ��,						��������ݴ��0_1������ */
	ENCODER_SRC_IR			   	= 1, 				/* �����ͼ��,						��������ݴ��0_1������ */
	ENCODER_SRC_THER		   	= 2, 				/* �ȶ�ͼͼ��,						��������ݴ��1_0������ */
	ENCODER_SRC_FUSION_WL_THER 	= 3, 				/* �ɼ������ȶ�ͼ���ں�,				��������ݴ��1_0������ */
	ENCODER_SRC_TOF_IR		   	= 4, 				/* TOFͼ��,y scale,					��������ݴ��0_1������ */
	ENCODER_SRC_TOF_BKIR	   	= 5, 				/* TOF����ͼ,back ground ir ,scale,	��������ݴ��0_1������ */
	ENCODER_SRC_TOF_BKIR_INSIDE	= 6, 				/* TOF����ͼ,back ground ir	,copy,	��������ݴ��0_1������ */
	ENCODER_SRC_TOF_IR_IN_RECT 	= 7, 				/* TOFͼ��,y scale  + ����,			��������ݴ��0_1������ */
	ENCODER_SRC_TOF_IR_INSIDE  	= 8, 				/* TOFͼ��,y scale ,copy,			��������ݴ��0_1������ */
	ENCODER_SRC_FUSION_WL_TOF  	= 9, 				/* �ɼ�����TOFͼ���ں�,				��������ݴ��0_1������ */
	ENCODER_SRC_FUSION_IR_TOF  	= 10,				/* �������TOFͼ���ں�,				��������ݴ��0_1������ */
    ENCODER_SRC_MAX				= 255
}SUB_ENCODE_PRIV_SRC;

/*
	��Ĥ���빲��buff����
	DSP�ڿ�����Ĥ����ʱ�����ڴ棬�ر�ʱ���ͷţ��ظ�����ʱ���ض�Ӧͨ���ĵ�ַ
*/
typedef struct tagIrisShareBuffInfo
{
	unsigned int  netEnable;					/* ���紫������������ */

	unsigned int  recEnable;					/* ���ش洢���������� */

    unsigned int resolution;            		/* ����ֱ��ʣ�������ʹ�õĺ�һ�£�֧��1.HD1080p_FORMAT 2.HD720p_FORMAT 3.SVGA_FORMAT 4.RES_2560_1920_FORMAT */
	
	NET_POOL_INFO irisNetPool;  				/* ���紫�������� */

    REC_POOL_INFO irirRecPool;       			/* ���ش洢������ */

	unsigned char reserve[64];					/* ���� */
} IRIS_SHARE_BUFF_INFO;

/*
	USB�����(��sensor����)���빲��buff����
	DSP�ڿ���USB����ʱ�����ڴ棬�ر�ʱ���ͷţ��ظ�����ʱ���ض�Ӧͨ���ĵ�ַ
*/
typedef struct tagUsbShareBuffInfo
{
	unsigned int  netEnable;					/* ���紫������������ */

	unsigned int  recEnable;					/* ���ش洢���������� */

    unsigned int resolution;            		/* ����ֱ��ʣ�������ʹ�õĺ�һ�£�<�Ƿ���Ҫ����������> */

	NET_POOL_INFO usbNetPool;  					/* ���紫�������� */

    REC_POOL_INFO usbRecPool;       			/* ���ش洢������ */

	unsigned char reserve[64];					/* ���� */
} USB_SHARE_BUFF_INFO;

/*
    ��������  HOST_CMD_SET_ENCODER_PARAM
*/
typedef struct
{
    unsigned int muxType;                           /* ��װ��ʽ����ѡTS/PS/RTP/HIK*/
    unsigned int streamType;                        /* �������ͣ�Ŀǰ��ѡ��Ƶ���͸����� */
    unsigned int videoType;                         /* ��Ƶ��ʽ��ʹ�� VIDEO_STREAM_TYPE_E */
    unsigned int audioType;                         /* ��Ƶ��ʽ����ѡG711/G722/MPEG1L2*/

    unsigned int resolution;                        /* �ֱ��� */
    unsigned int quant;                             /* ����ϵ��(1-31),������ʱ��Ч */
    unsigned int minQuant;                          /* ��С����ϵ��(1-31) */
    unsigned int maxQuant;                          /* �������ϵ��(1-31) */
    unsigned int bpsType;                           /* ���ʿ�������(0:������;1:������) */
    unsigned int bps;                               /* ����(������ʱΪ���ʣ�������ʱΪ��������) */
    unsigned int fps;                               /* ֡�� */
    unsigned int IFrameInterval;                    /* I֡��� */
    unsigned int BFrameNum;                         /* B֡���� */
    unsigned int EFrameNum;                         /* E֡���� */
    unsigned int b2FieldEncode;                     /* �������� */
    unsigned int sysParam;                          /* �����������Ƿ����CRC��ˮӡ��RSA�ȣ����ļ�ͷ�е�system_param������ͬ */
    unsigned int max_delay;                         /* ���ʿ�����ʱʱ��(1-60),ʱ��Խ��,˲ʱ����Խ�ȶ�,��ͼ��
                                                                    �����仯��,���ײ���������.�����仯�������ģʽ�½�max_delay
                                                                    ���,��֤�����ȶ�.�Ƽ�ֵΪ8. */
    unsigned int fast_motion_opt;                   /* �����˶�ͼ�������Ż�(��������Ч),0-�ر�,1-���� */
    unsigned int profile;                           /* ������profileֵ(avc/hevc) */
} ENCODER_PARAM;

/* ͼƬ������Ϣ�������Ĵ�С */
#define JPEG_DESCRIPTION_INFO_SIZE (256)

/*
    ����ץͼģʽ����
*/
typedef enum jpegCaptureModeE
{
    CAP_ONESHOT   = 0,                              /* ����ץ�ģ�һ������ץһ�� */
    CAP_CONTINUE,                                   /* ����ץ�� */
    CAP_INVALID
}JPEG_MODE;


/*
    ����ץͼ���� HOST_CMD_SET_ENC_JPEG_PARAM
*/
typedef struct tagJpegParamSt
{
    unsigned int idx;                               /* ͼƬ�±귽��Ӧ�ò���ͬ��,���ڻ�ץͼӦ�ûᴫ��idx������ͬ���� */
    unsigned int    width;                          /* ��ͼƬ�� */
    unsigned int    height;                         /* ��ͼƬ�� */
    unsigned int    quality;                        /* ͼƬ���� */
    JPEG_MODE       eJpegMode;                      /* ץͼģʽ */
    unsigned int    uiCaptCnt;                      /* ץ��������������ץ��ģʽ����Ч */
    unsigned int    uiCaptSec;                      /* �� uiCaptSec ���ھ���ץ�� uiCaptCnt ���� */
    OSD_CONFIG      osdParam;                       /* ����ץͼ��OSD��Ϣ: ����ͼƬ���ӿ��š����֡�ʱ�����Ϣ */
    unsigned int    smallWidth;                     /* СͼƬ�� */
    unsigned int    smallHeight;                    /* СͼƬ�� */
    unsigned char   reserve[248];
}JPEG_PARAM;

/*
    ��������ץͼ����ͼƬ�������� HOST_CMD_START_ENC_JPEG
*/
typedef struct tagJpegAddInfo
{
    JPEG_PARAM   stJpegPrm;
    unsigned int uiSize;                            /* ͼƬ����ʵ�ʴ�С */
    char         cInfo[JPEG_DESCRIPTION_INFO_SIZE]; /* ͼƬ��������     */
}JPEG_ADD_INFO_ST;

/* ץͼ�������ͻص���Ϣ */
typedef enum
{
    JPEG_SNAP_RESULT_SUCCESS,
    JPEG_SNAP_ERR_PARA_NULL,                /* ץͼ�ṹ�����Ϊ�� */
    JPEG_SNAP_ERR_DEC_BIG_IMG_COPY,         /* ����ץͼ��ͼ����ʧ�� */
    JPEG_SNAP_ERR_DEC_BIG_IMG_ENC,          /* ����ץͼ��ͼ����ʧ�� */
    JPEG_SNAP_ERR_DEC_SMA_IMG_COPY,         /* ����ץͼСͼ����ʧ�� */
    JPEG_SNAP_ERR_DEC_SMA_IMG_ENC,          /* ����ץͼСͼ����ʧ�� */
    JPEG_SNAP_ERR_CAP_COPY,                 /* �ɼ�ץͼ����ʧ�� */
    JPEG_SNAP_ERR_CAP_ENC,                  /* �ɼ�ץͼ����ʧ�� */

}JPEG_CB_ERROR_TYPE;

/*
    ����ץͼ���ؽṹ�� STREAM_ELEMENT_JPEG_IMG
*/
typedef struct tagJpegSnapCbResultSt
{
    char           *cJpegAddr;                      /* �ص���ץͼ���ݵĵ�ַ */
    unsigned int    uiJpegSize;                     /* �ص���ץͼ���ݵĴ�С */    
    unsigned int    jpegIdx;                        /* ͼƬ�±귽��Ӧ�ò���ͬ��*/    
    char           *smallJpegAddr;                  /* Сͼ�ص���ץͼ���ݵĵ�ַ */
    unsigned int    smallJpegSize;                  /* Сͼ�ص���ץͼ���ݵĴ�С */
    unsigned int    error_code;                     /* ץͼ����ص���Ϣ,���JPEG_CB_ERROR_TYPE */
    unsigned char   reserve[60];
}JPEG_SNAP_CB_RESULT_ST;

/*
    ���뵥��ͼƬ�������� 
    HOST_CMD_INSERT_USER_PIC_PARAM 
    HOST_CMD_SUB_INSERT_USER_PIC_PARAM
*/
typedef struct tagVencUserPicPrmSt
{
    unsigned int enable;                            /* ����ʹ��   */
    DSP_PIC_INFO  picInfo;                          /* ���ݸ�ʽ   */
}VENC_USER_PIC_PRM;

/* ������Ϣ˽������: ���򡢴�� */
typedef enum
{
    ENCODER_PARAM_PRIV_TYPE_NONE,
    ENCODER_PARAM_PRIV_TYPE_PACK,
    ENCODER_PARAM_PRIV_TYPE_DRAW, 
    ENCODER_PARAM_PRIV_TYPE_POS, 
    ENCODER_PARAM_PRIV_TYPE_SWITCH_VISRC, 
}ENCODER_PARAM_PRIV_TYPE;

/* ����������˽����Ϣ����: ����ķ�ʽ ���ַ� ˽�а� ������ƵԴ �� */
typedef struct
{
    unsigned int            encPrivType;            /* 1�������2��֡���� ��� ENCODER_PARAM_PRIV_TYPE  */
    unsigned int            drawEnable;             /* 0 ������  1 ����   */
    unsigned int            packEnable;             /* 0 �����  1 ��   */
    unsigned int            posEnable;              /* 0 ����������Ϣ  1��   */
    unsigned int            switchViSrc;            /* �л���ǰH26Xͨ����������YUV����ƵԴ */
}ENCODER_PARAM_PRIV;

/*
    ������룬�Ӳɼ���ͼ���вü�������������  
    HOST_CMD_SET_ENC_REGION 
*/
typedef struct stEncoderRegionPrmSt
{
    unsigned int enable;                            /* Region ʹ�ܿ��� */
    REGION_INFO  stRegionInfo;                      /* Region ������Ϣ */
}ENCODER_REGION_PRM;

/* 
�ں�picͼƬȥ���� 
*/
typedef struct
{
    float kx;                                       /*�ɼ������ȳ���x��������ϵ��*/
    float ky;                                       /*�ɼ������ȳ���y��������ϵ��*/
    float z0;                                       /*��ʼ�궨�Ĺ̶�����  ��λ mm */
    float delta_x;                                  /*x��������ƫ��		��λ pixel */
    float delta_y;                                  /*y��������ƫ��		��λ pixel */
    float f1;                                       /*�ȳ���ͷ����   ��λ mm*/
    float dx;                                       /*�ɼ��⾵ͷ���ȳ���ͷ��x��ˮƽ������ľ���	 ��λmm*/
    float dy;                                       /*�ɼ��⾵ͷ���ȳ���ͷ��y����ֱ������ľ���	 ��λmm*/
    float dz;                                       /*�ɼ��⾵ͷ���ȳ���ͷ��z����ľ���,  ��λ mm. ��������ͷ��һ��ƽ�棬��dz = 0*/
    float pixel;                                    /*�ȳ���ͷ��Ԫ��С  ��λ mm*/

    unsigned int   dwPicSize;                       /* ͼƬ��С */
    unsigned char *byPicBuf;                        /* ͼƬ���� */
    unsigned int   calibrationTime;                 /* �궨���� 0:�ںϱ궨 1:���α궨 */
    unsigned int   dwRes[7];                        /* Ԥ�� */
}THERMAL_PIC_CORRECT_PARAM; 

/* ���ȶ�ͼƬ�ϵ��¶Ⱥ�������*/
typedef struct
{
    unsigned int   x;               /* ������x  */
    unsigned int   y;               /* ������y  */
    unsigned int   w;               /* ������w  */
    unsigned int   h;               /* ������h  */

    unsigned int   tempX;           /* �¶ȵ�x  */
    unsigned int   tempY;           /* �¶ȵ�y */
    float          temp;            /* �¶�  */
    float          distance;        /* ����*/
    
    unsigned int   uiSize;          /*ԭʼͼƬ���� */
    unsigned char *uiAddr;          /*ԭʼͼƬ���� */
    unsigned int   uiOutSize;       /*���ͼƬ���� */
    unsigned char *uiOutAddr;       /*���ͼƬ���� */
}ENCODER_PIC_SHOW_TEMP_PARAM;


/****************************************************************************/
/*                                  ����                                    */
/****************************************************************************/

/*
    ����ģʽ
*/
typedef enum decModeE
{
    DECODE_MODE_INVALID        = 0x0,               /* ��Ч   */
    DECODE_MODE_REALTIME       = 0x1,               /* ʵʱģʽ(RTP)     */
    DECODE_MODE_FILE           = 0x2,               /* �ļ��ط�ģʽ(PS)       */
    DECODE_MODE_FILE_REVERSE   = 0x4,               /* �ļ�����ģʽ����������֡    */
    DECODE_MODE_I_FRAME        = 0x8,               /* I֡����ģʽ�����ڸ߱��ٿ�� */
    DECODE_MODE_JPEG           = 0x10,              /* ����JPEGץͼģʽ            */
    DECODE_MODE_FILE_MULTI     = 0x11,              /* �ļ���·�ط�ͬ��ģʽ        */
    DECODE_BUTT                                     /* ������Чֵ */
}DEC_MODE_E;

/*
    ��Ƶ��������
*/
typedef enum audioStreamTypeE
{
    STREAM_TYPE_AUDIO_MPEG1     = 0x03,             /* MPEG1��ʽ���� */
    STREAM_TYPE_AUDIO_MPEG2     = 0x04,             /* MPEG2��ʽ���� */
    STREAM_TYPE_AUDIO_AAC       = 0x0f,             /* AAC��ʽ����   */
    STREAM_TYPE_AUDIO_OPUS      = 0x11,             /* OPUS��ʽ����   */
    STREAM_TYPE_AUDIO_G711A     = 0x90,             /* G711��ʽA�ɱ��� */
    STREAM_TYPE_AUDIO_G711U     = 0x91,             /* G711��ʽU�ɱ��� */
    STREAM_TYPE_AUDIO_G722_1    = 0x92,             /* G722��ʽ����    */
    STREAM_TYPE_AUDIO_G723_1    = 0x93,             /* G723��ʽ����    */
    STREAM_TYPE_AUDIO_AAC_LD    = 0x94,             /* AAC_LD��ʽ����  */
    STREAM_TYPE_AUDIO_G726      = 0x96,             /* G726��ʽ����    */
    STREAM_TYPE_AUDIO_G728      = 0x98,             /* G728��ʽ����    */
    STREAM_TYPE_AUDIO_G729      = 0x99,             /* G729��ʽ����    */
    STREAM_TYPE_AUDIO_AMR_NB    = 0x9a,             /* AMR_NB��ʽ����  */
    STREAM_TYPE_AUDIO_AMR_WB    = 0x9b,             /* AMR_WB��ʽ����  */
    STREAM_TYPE_AUDIO_L16       = 0x9c,             /* L16��ʽ����     */
    STREAM_TYPE_AUDIO_L8        = 0x9d,             /* L8��ʽ����      */
    STREAM_TYPE_AUDIO_DVI4      = 0x9e,             /* DV14��ʽ����    */
    STREAM_TYPE_AUDIO_GSM       = 0x9f,             /* GSM��ʽ����     */
    STREAM_TYPE_AUDIO_GSM_EFR   = 0xa0,             /* GSM_EFR��ʽ���� */
    STREAM_TYPE_AUDIO_LPC       = 0xa1,             /* LPC��ʽ����     */
    STREAM_TYPE_AUDIO_QCELP     = 0xa2,             /* QCELP��ʽ����   */
    STREAM_TYPE_AUDIO_VDVI      = 0xa3,             /* VDVI��ʽ����    */
    STREAM_TYPE_AUDIO_BUTT                          /* �����Чֵ     */
}AUDIO_STREAM_TYPE_E;


/*
    ���벥���������ͣ����Ʋ�������������
*/
typedef enum 
{
    DECODE_VIDEO   = 0x0,       /* ��Ƶ��������ģʽʱ��ʹ�� */
    DECODE_AUDIO   = 0x1,       /* ��Ƶ�� */
    DECODE_MULTI   = 0x2,       /* ������ */
} DEC_STREAM_TYPE;

/*
    ���ƽ��벥�������������ͣ���Ӧ��������  HOST_CMD_DEC_STREAMTYPE
*/
typedef struct
{
	DEC_STREAM_TYPE     streamType;					/* �û�ѡ�񲥷��������ͣ�����ѡ�񲥷�ʱֻ����Ƶ������Ƶ�����߸����� */
	unsigned char       reserve[64];
} DECODER_STREAMTYPE;

/*
    ������������  HOST_CMD_DEC_START
*/
typedef struct
{
    DEC_MODE_E          mode;                       /* ����ģʽ */
    STREAM_PACKET_TYPE  muxType;                    /* ���ģʽ */
    VIDEO_STREAM_TYPE_E videoType;                  /* ��Ƶ�������� */
    unsigned int        width;                      /* �����Ŀ� */
    unsigned int        height;                     /* �����ĸ� */
    AUDIO_STREAM_TYPE_E audioType;                  /* ��Ƶ�������� */
} DECODER_PARAM;

/*����ץͼǰ֪ͨӦ�õĴ�ץͼͼ����Ϣ*/
typedef struct tagDecImgInfo
{
    unsigned int    width;
    unsigned int    height;
}DEC_IMG_INFO;

typedef enum eDecTPPacketType
{
    PACKET_TYPE_RTP   = 0x0,       /* RTP ��� */
    PACKET_TYPE_PS    = 0x1,       /* PS  ��� */
    PACKET_TYPE_MEDIA = 0x2,       /* ��ý�� ��� mp4 avi�� */
} DEC_TP_PACKET_TYPE;

/*
    ת�� Դ��������
*/
typedef struct tagDecTPSrcPrmSt
{
    DEC_TP_PACKET_TYPE ePacketType;    /* Դ�����ķ������ */
    unsigned char      reserve[64];
} DECODER_TP_SRC_PARAM;

/*
    ת�� Ŀ����������
*/
typedef struct tagDecTPDstPrmSt
{
    unsigned int        useSetPrm;                  /* 0: ����������Դһ��  1 : ����������� */
    STREAM_PACKET_TYPE  muxType;                    /* ���ģʽ     */
    VIDEO_STREAM_TYPE_E videoType;                  /* ��Ƶ�������� */
    unsigned int        width;                      /* �����Ŀ�     */
    unsigned int        height;                     /* �����ĸ�     */
    AUDIO_ENC_TYPE      audioType;                  /* ��Ƶ�������� */
    unsigned int        devChan;                    /* ���ת��װ�������ͨ���� */
    unsigned int        chanType;                   /* 0:��������1:������ */
    unsigned char       reserve[24];
} DECODER_TP_DST_PARAM;

/*
   ת������  HOST_CMD_DEC_TRANS_PACK��HOST_CMD_DEC_TRANS_TO_ENC
   1. �����ת���ΪPS���ݣ�Զ��¼��
   2. ��RTP�������ת������ŵ����뻺�����У�
      Ӧ�ó�����̽�ӷֻ���IPC camera��Ϊ����cameraʹ�ã�
      ��Ҫ�ڱ��ؽ���IPC��ʾ�����������´�����͸��Է��ֻ�
*/
typedef struct tagDecTransPackSt
{
    unsigned int         start;      /* 1:����, 0:�ر�      */
    DECODER_TP_SRC_PARAM stSrcPrm;   /* Դ��������   */
    DECODER_TP_DST_PARAM stDstPrm;   /* Ŀ���������� */
    unsigned char        reserve[64]; /* ���� */
} DECODER_TRANS_PACK_PARAM;

/*
   ������ͣ������صĽṹ�壬��ӦHOST_CMD_DEC_PAUSE
*/
typedef struct tagDecPausePara
{
    unsigned int  enable;      /* 1:��ͣ, 0:��������       */
    unsigned char reserve[64]; /* ���� */
} DECODER_PAUSE_PARAM;

/*
    ����ͨ��¼��ͬ������¼���ʱ�������ӦHOST_CMD_DEC_SYNC_REC
*/
typedef struct tagDecSyncPara
{
    unsigned int        decNum;                     /* ����ͬ��ͨ������ */
    unsigned int        decChn[MAX_VDEC_CHAN];      /* �������ͨ���� */
    unsigned int        devChan;                    /* ���ر����ͨ���� */
    unsigned int        chanType;                   /* 0:��������1:������ */
    unsigned char       reserve[64];                /* ���� */
}DECODER_SYNC_PARAM;

/****************************************************************************/
/*                                 ��ʾ                                     */
/****************************************************************************/

#define DISP_FRM_CNT                 (16)          /*ÿ·��ʾͼ�񻺳����*/

/*
    ÿһ��������ʾ������
*/
typedef struct
{
    unsigned int    uiChan;                         /* ͨ�� */
    unsigned int    x;                              /* ���ڵ���ʼ����ĺ����� */
    unsigned int    y;                              /* ���ڵ���ʼ����������� */
    unsigned int    w;                              /* ���ڵĿ�� */
    unsigned int    h;                              /* ���ڵĸ߶� */
    unsigned int    fps;                            /* ��ʾ֡��   */
    unsigned int    color;                          /* ����Ƶ��ʾʱ���ڵ���ɫֵ */
    unsigned int    layer;                          /* ���ڵĲ㼶�����ֵΪ����ͨ����-1��ȡֵ[0,1]��ֵԽ�����ȼ�Խ�ߡ���Ƶ��ʾ�����������ʱ�����ȼ��ߵĸ������ȼ��͵ġ���������ֵһ��ʱ����Ĭ��ֵ�ɼ���ƵΪ���ȼ��ߴ��� */
    void            *bpBinder;                      /* Android���ϲ�Ӧ���·�һ�����ڶ�Ӧ��ʵ�� */
    unsigned char   reserve[32];
} DISP_REGION;

/*
    HOST_CMD_DISP_CLEAR ���ò���ʾ��Ԥ������
*/
typedef struct tagDispClearCtrlSt
{
    unsigned int  uiCnt;                            /* ���õĴ��ڵĸ���   */
    unsigned int  uiChan[MAX_DISP_CHAN];            /* ����clear��ͨ����  */
}DISP_CLEAR_CTRL;

/*
    HOST_CMD_ALLOC_DISP_REGION Ԥ���Լ�Ԥ�����ڿ��Ʋ���
*/
typedef struct tagDispCtrlSt
{
    unsigned int  uiCnt;                            /* ���õĴ��ڵĸ���������Ƶ��沥�Ź������֧��2���������ͺź������� */
    DISP_REGION   stDispRegion[MAX_DISP_CHAN];      /* ����s��ʾ������   */
}DISP_CTRL;

typedef struct 
{
    unsigned int  uiCnt;                            /* ���õĴ��ڵĸ��� */
    CROP_INFO_ST  stCropRegion[MAX_DISP_CHAN];      /* ���ڲü�������   */
}CROP_PARAM;

#define SCREEN_LAYER_MAX   (3)

/*
    ͼ�����Ϣ��Ӧ���ڴ�ͼ��������
*/
typedef struct tagScreenAttrSt
{
    unsigned int    isUse;                          /* �Ƿ�ʹ�ø�ͼ�� */
    unsigned int    x;                              /* ��ʼ���� */
    unsigned int    y;                              /* ��ʼ���� */
    unsigned int    width;                          /* ���     */
    unsigned int    height;                         /* �߶�     */
    unsigned int    pitch;                          /* ���     */
    unsigned int    bitWidth;                       /* λ��     */
    unsigned char  *srcAddr;                        /* �Դ���ʼ��ַ */
    unsigned int    srcSize;                        /* �Դ��С */
    unsigned char  *srcPhyAddr;                     /* �Դ���ʼ��ַ(FB DSP�����ַ,UI�����ʹ��) */
    unsigned char  *srcDmaPhyAddr;                  /* Ӳ������ ���������ڴ��ַ(FB APP�����ַ,��ת/����,UI �����ʹ��)*/ 
}SCREEN_ATTR;

/*
    HOST_CMD_DISP_FB_MMAP ��ȡ����ͼ���ַ
*/
typedef struct tagScreenCtrlSt
{
    unsigned int uiCnt;
    SCREEN_ATTR  stScreenAttr[SCREEN_LAYER_MAX];
}SCREEN_CTRL;


/*
    HOST_CMD_DISP_FB_SHOW ˢ����Ļָ������
*/
typedef struct
{
    unsigned int    x;                              /* ��ʼ���� */
    unsigned int    y;                              /* ��ʼ���� */
    unsigned int    width;                          /* ���     */
    unsigned int    height;                         /* �߶�     */
    unsigned int    pitch;                          /* ���     */
}LAYER_ATTR;

/*
    HOST_CMD_DISP_FB_SHOW ������Ļָ��ͼ�㴰�ڵ�ָ������
*/
typedef struct
{
    unsigned int    layer;
    LAYER_ATTR      layerAttr;
}SCREEN_SHOW;


/*
    ��Ƶ�ɼ�����ʾ��ӳ����Ϣ
*/
typedef struct
{
    unsigned int uiCnt;
    unsigned int addr[DISP_FRM_CNT];                /* �ɼ����(DUP���)���߽���ͨ�� */
    unsigned int out[DISP_FRM_CNT];                 /* ��ʾ������ʾͨ�����(0-N)*/
    unsigned int draw[DISP_FRM_CNT];                /* �Ƿ����������Ϣ����������Ϣ��Ŀ���Ժ��Դ˲���*/
} DSP_DISP_RCV_PARAM;

/*
    ��ʾ���ö��
*/
typedef enum StyleE
{
    STYLE_STANDARD_MODE,                            /* ��׼ģʽ */
    STYLE_BRIGHT_MODE,                              /* ����     */
    STYLE_SOFT_MODE,                                /* ���     */
    STYLE_GLARING_MODE                              /* ����     */
}STYLE_E;

/*
    ������ʾ���  HOST_CMD_SET_VO_STYLE
*/
typedef struct tagDispStyleSt
{
    STYLE_E eStyle;                                 /* ��ʾ������� */
}DISP_STYLE;

/*
     ��ʾЧ��  HOST_CMD_SET_VO_CSC
*/
typedef struct tagDispCscSt
{
    unsigned int uiLuma;                            /* ����ֵ   */
    unsigned int uiHue;                             /* ɫ��ֵ   */
    unsigned int uiContrast;                        /* �Աȶ�ֵ */
    unsigned int uiSatuature;                       /* ���Ͷ�ֵ */
}DISP_CSC;

/*
    ��Ƶ�����׼
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
    ��ʾ����ӿ�����
*/
typedef enum tagVideoOutputDevEn
{
    VO_DEV_LCD       = 0,                           /* LCD��ʾ  */
    VO_DEV_VGA       = 1,                           /* VGA ��ʾ */
    VO_DEV_HDMI      = 2,                           /* HDMI��ʾ */
    VO_DEV_CVBS      = 3,                           /* CVBS��ʾ */
    VO_DEV_MIPI      = 4,                           /* �����Чֵ */  
    VO_DEV_MAX_CNT   = 5,                           /* �����Чֵ */
} VIDEO_OUTPUT_DEV_E;

/*
    ������ʾ�����׼  HOST_CMD_SET_VO_STANDARD
*/
typedef struct tagDispStandardSt
{
    VIDEO_STANDARD eStandard;                       /* ��ʾ�����׼ */
}DISP_STANDARD;

/*
    ������ʾ�豸����
*/
typedef struct tagDispDevAttrSt
{
    VIDEO_OUTPUT_DEV_E  eVoDev;                     /* ��ʾ����豸���� */

    int videoOutputWidth;                           /* ��Ƶ����豸��� */
    int videoOutputHeight;                          /* ��Ƶ����豸�߶� */
    int videoOutputFps;                             /* ��Ƶ����豸ˢ��֡�� */
}DISP_DEV_ATTR;

/*
    ��ʾͨ������
*/
typedef struct tagVideoOutputInitPrmSt
{
    unsigned int        bHaveVo;                    /* �Ƿ�����Ƶ���   */
    unsigned int        voChnCnt;                   /* Ӧ�����õ����ͨ������ֻ����һ�� */    
    DISP_DEV_ATTR       stDispDevAttr;              /* ��ʾ�豸����     */
}VO_INIT_PRM;

/*
    ��ʾ�����Ϣ
*/
typedef struct tagVideoOutputInitInfoSt
{
    unsigned int uiVoCnt;                           /* vo ����ӿ���     */
    VO_INIT_PRM  stVoInfoPrm[MAX_DISP_CHAN];        /* ÿһ·vo ������� */
}VO_INIT_INFO_ST;

/*
    ��ʾģ����뵥��ͼƬ���� HOST_CMD_DISP_INSERT_PIC_PARAM 
*/
typedef struct tagDispUserPicPrmSt
{
    unsigned int enable;                            /* ����ʹ��                 */
    int          dispChn;                           /* ����ʾ�豸�ϵ���ʾͨ���� 
                                                                    -1:��ʾ���е�ͨ��������0����ʾ�����ͨ�� */
    DSP_PIC_INFO picInfo;                           /* ���ݸ�ʽ                 */
}DISP_USER_PIC_PRM;

/*
    ��Ļstandby���ܣ���ֹ���� 
    HOST_CMD_DISP_STANDBY
*/
typedef struct tagDispStandbyPrmSt
{
    unsigned int  uiEnable;                         /* �Ƿ�ʹ�� */
}DISP_STANDBY_PRM; 

/* HOST_CMD_DISP_FBE_PARAM ����Ԥ���������� */
typedef struct tagDispFbePrmSt
{
    unsigned int    fbeEnable;                      /* ����Ԥ������ 0:�ر�  1:���� */
    unsigned int    white;                          /* ����ǿ�� ȡֵ[0-10] ,appĬ����������ǿ��ֵΪ5*/ 
    unsigned int    fil;                            /* ĥƤǿ�� ȡֵ[0-10] */
    unsigned char   reserve[64];
}DISP_FBE_PRM;


/* �ȳ���ͼƬ��ʾ�ķ�ʽ*/
typedef enum stDispThermoShowTypeE
{
    DSP_DISP_THERMO_ONLY  = 0,           /* ֻ��ʾ�ȳ���ͼ��ȫ����ʾ */
    DSP_DISP_WL_ONLY      = 1,           /* ֻ��ʾ�ɼ���ͼ��ȫ����ʾ */
    DSP_DISP_THERMO_BACK  = 2,           /* �ȳ���ͼƬ�����棬�ɼ��������� */
    DSP_DISP_WL_BACK      = 3,           /* �ɼ���ͼƬ�����棬�ȳ���������  */
    DSP_DISP_THERMO_MAX,
}DISP_THERMO_SHOW_TYPE_E;

/*
    ��Ļ��ʾ�ȳ���ͼƬ����
    HOST_CMD_DISP_THERMO
*/
typedef struct tagDispThermoPrmSt
{
    unsigned int  uiEnable;                   /* �Ƿ�ʹ�� */
    DISP_THERMO_SHOW_TYPE_E  showType;        /* ��ʾ����*/
    unsigned char *uiAddr;                    /*�ȳ���ͼƬ���� */
    unsigned int   uiSize;                    /*�ȳ���ͼƬ���� */

    /* �ɽ���ͼ��λ�� */
    unsigned int  wlX; 
    unsigned int  wlY; 
    unsigned int  wlW; 
    unsigned int  wlH; 

    /* �ȳ���ͼ��λ�� */
    unsigned int  thermoX; 
    unsigned int  thermoY; 
    unsigned int  thermoW; 
    unsigned int  thermoH; 

    /* �ȳ���ͼ��������λ��  */
    float         rectX; 
    float         rectY; 
    float         rectW; 
    float         rectH; 
	
	unsigned char   reserve[64];
}DISP_THERMO_PRM; 



/* USBͼ����ʾ�ķ�ʽ*/
typedef enum stUsbDispShowTypeE
{
    USB_DISP_IRIS_ONLY = 0,           /* ֻ��ʾUSBͼ��ȫ����ʾ */
    USB_DISP_WL_ONLY   = 1,           /* ֻ��ʾ�ɼ���ͼ��ȫ����ʾ,�ݲ�֧�� */
    USB_DISP_IRIS_BACK = 2,           /* USBͼƬ�����棬�ɼ���������,�ݲ�֧�� */
    USB_DISP_WL_BACK   = 3,           /* �ɼ���ͼƬ�����棬USB������,�ݲ�֧��  */
    USB_DISP_MAX,
}USB_DISP_SHOW_TYPE_E;

/*
    ��Ļ��ʾUSB���ͼ�����
    HOST_CMD_DISP_USB
*/
typedef struct tagUsbDispSt
{
    unsigned int  enable;        	/* �Ƿ�ʹ�� */

    USB_DISP_SHOW_TYPE_E  showType;/* ��ʾ����*/

    /* �ɼ���ͼ��λ�� */
    unsigned int  wlX; 
    unsigned int  wlY; 
    unsigned int  wlW; 
    unsigned int  wlH; 

    /* USBͼ��λ�� */
    unsigned int  usbX; 
    unsigned int  usbY; 
    unsigned int  usbW; 
    unsigned int  usbH; 
	
	unsigned char   reserve[64];
}USB_DISP_PRM; 

/*
    ��������Ⱦģʽ
*/
typedef enum
{
    DISP_FACE_MODE_CIRCULATE     = 0,                        /* ��ͼѭ����ʾģʽ */
    DISP_FACE_MODE_SINGLE_ROTATE = 1,                        /* ��ͼ��ת��ʾģʽ */
    DISP_FACE_MODE_MAX,
} DISP_FACE_MODE;

/*
    �������ٿ�����
*/
typedef enum
{
	DISP_FACE_TYPE_DEFAULT       = 0,                        /* Ĭ�ϵ�����������     */
    DISP_FACE_TYPE_STRANGER      = 1,                        /* İ���˵�����������   */
	DISP_FACE_TYPE_AUTHENTICATOR = 2,                        /* ͨ����֤������������ */
    DISP_FACE_TYPE_BLACKLIST     = 3,                        /* ������������������   */
    DISP_FACE_TYPE_WHITELIST     = 4,                        /* ������������������   */
    DISP_FACE_TYPE_MAX,
} DISP_FACE_TYPE;

/*
    ��ʾģ�������㼶ͼƬ����
*/
typedef struct tagDispFaceLayerPrmSt
{
    unsigned int    picCnt;                                  /* ����ͼƬ����        */
    DSP_PIC_INFO    picInfo[MAX_DISP_FACE_PIC_CNT];          /* ��̬����ͼ��ʾ��Ϣ  */
    DISP_FACE_MODE  dispFaceMode;
    unsigned char   reserve[64];
}DISP_FACE_PIC_LAYER_PRM;

/*
    ��ʾģ����������ͼƬ���� HOST_CMD_SET_DISP_FACE_PIC 
*/
typedef struct tagDispFacePicPrmSt
{
    unsigned int            facePicLayerCnt;                        /* ������ͼ�����   */
    DISP_FACE_PIC_LAYER_PRM facePicLayer[MAX_DISP_FACE_LAYER_CNT];  /* ������ͼ����Ϣ   */
    DISP_FACE_TYPE          faceType;                               /* �������ٿ������ */
    unsigned char           reserve[64];
}DISP_FACE_PIC_PRM;

/****************************************************************************/
/*                                 ��Ƶ                                     */
/****************************************************************************/

/* 
    ������Ƶҵ������
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
    ��Ƶ����������Ϣ
*/
typedef struct
{
   unsigned int         bEnable;                   /* ��Ƶ��������ʹ�ܣ�DSP��ʱ��������ֶ�         */
   unsigned int         priority[AMER_IDX_MAX];    /* ���ȼ�    */
   unsigned int         mainVolume;                /* �����������ڸ����ȼ���Ƶ */
   unsigned int         subVolume;                 /* �����������ڵ����ȼ���Ƶ */
   
   unsigned char        reserve[32];
}AUDIO_AMER_PARAM;

/*
    ��Ƶ���ͨ����Ϣ
*/
typedef struct tagAudioChnInfoSt
{
    unsigned int    aoChan;                         /* ��������Ӧ����Ƶͨ�� */
    unsigned int    addr;                           /* ��������Ӧ�ĵ�ַ     */
    unsigned int    rate;                           /* ��Ƶ������           */
    unsigned char   reserve[64];
} AUDIO_CHN_INFO_S;

/*
    ������Ƶ����ṹ��
    HOST_CMD_SET_AUDIO_LOOPBACK
    HOST_CMD_DEC_SET_DSP_AUDIO

    HOST_CMD_SET_TALKBACK
*/
typedef struct
{
    unsigned int     bEnable;                       /* �Ƿ�ʹ��         */
    AUDIO_CHN_INFO_S stAudioChn;                    /* ��Ƶ���ͨ����Ϣ */
} DSP_AUDIO_PARAM;

/*
    ���������豸��ģ�����ڻ��Խ� ������Ϣ
    HOST_CMD_SET_MANAGE_TALKTO_RESIDENT
*/
typedef struct tagAudioManageToResidentPrmSt
{
    unsigned int bEnable;                           /* �Ƿ�ʹ�� */
    ENC_CHNTYPE  encChnType;                        /* ������Ƶʹ�õı���ͨ������������
                                                       ���������ǵ���������Ĭ��0��ʹ�������� */
    unsigned char   reserve[64];
} AUDIO_MANAGE_TO_RESIDENT_PRM;

/*
    �ſڻ���ģ�����ڻ�ͨ�� ������Ϣ
    HOST_CMD_SET_DOOR_TALKTO_RESIDENT
*/
typedef struct tagAudioDoorToResidentPrmSt
{
    unsigned int bEnable;                           /* �Ƿ�ʹ��    */

    unsigned int micInChn;                          /* ��������ͨ�� */
    unsigned int analogInChn;                       /* ģ������ͨ�� */
    unsigned int speakOutChn;                       /* �������ͨ�� */
    unsigned int analogOutChn;                      /* ģ�����ͨ�� */
} AUDIO_DOOR_TO_RESIDENT_PRM;

/*
    ������������
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
    �����������
*/
typedef enum
{
    AUDIO_INTERRUPT_COVER      = 0x0,              /* �����ʱ��ֱ�Ӹ��� */
    AUDIO_INTERRUPT_RECOVER    = 0x1,              /* �����ʱ���ָ����� */
    AUDIO_INTERRUPT_MIX        = 0x2,              /* �����ʱ���������� */
    AUDIO_INTERRUPT_MAX
}AUDIO_INTERRUPT_TYPE;

/* tts������ */
typedef enum  
{
    AUDIO_TTS_CONVERT     = 0x0,
    AUDIO_TTS_DIRECT_PLAY = 0x1, 
    AUDIO_TTS_BUTT
}TTS_USAGE_TYPE;

typedef struct
{
    char *voiceName;                                /* ѡ��ͬ�ķ����� */
    char *txtEncode;                                /* �ı��ı����ʽ,   ֧�ֲ���: GB2312��GBK��BIG5��UNICODE��GB18030��UTF8 */
    unsigned int speed  ;                           /* ��Χ[0,100]��ֵԽ������Խ�� */
    unsigned int volume ;                           /* ��Χ[0,100]��ֵԽ������Խ�� */
    unsigned int pitch  ;                           /* ��Χ[0,100]��ֵԽ�����Խ�� */
    unsigned int rdn    ;                           /* ��Χ[0,2]��  ���ֵķ�����ʽ  0 ���������Զ��ж�,1 ��ȫ��ֵ,2 ��ȫ�ַ��� */
}TTS_ATTR_CONFIG;

typedef struct
{
   TTS_USAGE_TYPE       usageType;                  /* TTS�ӿڵĵ��÷�ʽ : 1��ֻת����2��ת�겢ֱ�Ӳ���*/
   unsigned char        *tts2AudBuf;                /* �����ı��Ļ���,Ŀǰ DSP �ȿ�20KB�������С�Ժ���������� */
   unsigned int         dataLen;                    /* �ı��ĳ��� */
   AUDIO_PLAY_TYPE      stPlayType;                 /* ������������ */
   unsigned int         sampleRate;                 /* �������Ĳ��������ϳ����� */
   unsigned int         priority;                   /* ֵԽ�����ȼ�Խ��,��Χ[1, 10] */
   TTS_ATTR_CONFIG      stTTSAttr;
   unsigned int         id;                         /* ��ʶ���ı� ���������ֲ�ͬС���Ƿ�����ͬһ�ı��ֶ��ã�Ĭ��ֵΪ 0 */
   unsigned int         bEnable;                    /* ʹ��λ��0�رգ�1���� */
    AUDIO_ENC_TYPE  audEncType;                     /* ��Ƶѹ���㷨 */
    AUDIO_INTERRUPT_TYPE audInsertType;             /* ��Ƶ��ϲ������� */
   unsigned char        reserve[20];
}AUDIO_TTS_PARAM;

typedef enum
{
    QUE_STATE_MIN   = 0,
    QUE_STATE_EMPTY = 1,                       /* �����пռ� */
    QUE_STATE_FULL  = 2,                       /* ���������޿ռ�*/
    QUE_STATE_MAX
}AUDIO_QUE_STATE;

/*
    ������������ ��Ϣ
    HOST_CMD_START_AUDIO_PLAY
*/
typedef struct
{
    void*               audioBuf;                   /* ��Ƶ���ݻ��� */
    unsigned int        audioDataLen;               /* ��Ƶ���ݳ��ȣ���ȥ�ļ�ͷ */
    unsigned int        channel;                    /* ��ͨ������˫ͨ�� */
    unsigned int        rate;                       /* ������ */
    unsigned int        bits;                       /* ����λ�� */
    unsigned int        uiPlayTime;                 /* ����ʱ��:��λms��0-����һ��
                                                                    ��0-���Ź̶�ʱ�䣬һ��������ظ����� */
    AUDIO_PLAY_TYPE     stPlayType;                 /* ������������ */
    AUDIO_CHN_INFO_S    stAudioChn;                 /* ��Ƶ���ͨ����Ϣ */
    unsigned int        priority;                   /* ��Ƶ�������ȼ���ֵԽ�����ȼ�Խ��,��Χ1~10 */
    AUDIO_ENC_TYPE      audEncodeType;              /* ������ʽ��Ĭ��wav   */
    unsigned int        volume;                     /* ����������С*/
    /* �·��ĵ�audioIndex����Ƶ�ӵ�audioFrameIndex֡��ʼ���� */
    unsigned int        audioIndex;                 /* ��Ƶ���ݱ�� */
    unsigned int        audioFrameIndex;            /* ��Ƶ����֡��ţ���Ŵ�1��ʼ����ò���δ���ã�Ĭ�ϴӵ�1֡��ʼ���� */
    AUDIO_QUE_STATE     que_state;                  /* ������е�״̬*/
    unsigned int        isCurrentFileFinalData;     /* �Ƿ��ǵ�ǰ�ļ������һ�����ݣ�True��ʾ�ǣ�False��ʾ�� */
    unsigned int        error_code;                 /* ������������ʧ�ܴ������ͣ�1��ʾ���ݽ���ʧ�ܣ���Ҫ�����·���һ�Σ�-1��ʾ��������ʧ��  ��0��ʾ����*/
    unsigned char       reserve[12];                /* Ԥ�� */
}AUDIO_PLAY_PARAM;

/*
    ��������״̬����ͣor�ָ�
    HOST_CMD_CONTROL_AUDIO_PLAY
*/
typedef enum
{
    AUDIO_PLAY_SUSPEND    = 0,
    AUDIO_PLAY_RECOVERY   = 1,
    AUDIO_PLAY_STATE_MAX
}AUDIO_PLAY_STATE;

/*
    �����������ܿ��ƹ���
    HOST_CMD_CONTROL_AUDIO_PLAY
*/
typedef struct
{
    AUDIO_PLAY_STATE    audioPlayState;             /* ������Ƶ���Ź��ܣ���ͣor�ָ� */
    unsigned char       reserve[16];                /* Ԥ�� */
}AUDIO_PLAY_CTRL_PARAM;

/* �㲥���� */
typedef enum
{
    BROADCAST_FILE = 0,                        /* ��Ƶ�ļ� �㲥            */
    BROADCAST_CAP  = 1,                        /* ��Ƶ�ɼ��豸 �㲥 */
    BROADCAST_APK  = 2,                        /* ������APK��Դ �㲥 */
    BROADCAST_LOCAL_PLAY = 3,                  /*���ض�ʱ�㲥����   */
    BROADCAST_REALTIME_PLAY = 4,               /* ����ʵʱ�㲥����*/
    BROADCAST_MAX
}BROADCAST_TYPE;

/*
    �㲥���� ��Ϣ
    HOST_CMD_START_AUDIO_BROADCAST
*/
typedef struct
{
    unsigned int        fileID;                     /*�·��ļ�ID*/
    BROADCAST_TYPE      broadcastType;              /* �㲥���� */
    unsigned int        bEnable;
    AUDIO_ENC_TYPE      audStreamEncodeType;        /* ������Ƶ�������ʽ */
    
    /* ��Ƶ�ļ� �㲥��Ϣ */
    void*               audioBuf;                   /* ��Ƶ���ݻ��棬�·�WAVE�ļ�ʱҪȥ��44�ֽ�ͷ���·�MP3�ļ�ʱ����ȥ��ͷ */
    unsigned int        audioDataLen;               /* �·����ݳ��ȣ����Ȳ�����512K�����һ�����ļ������·��������һ���·��⣬����ÿ���·����ȶ��������512K */
    unsigned int        channel;                    /* ����������˫������ֻ���·�WAVE�ļ�ʱ��Ҫ���øò�����1��ʾ��������2��ʾ˫���� */
    unsigned int        samplerate;                 /* �����ʣ�ֻ���·�WAVE�ļ�ʱ��Ҫ���øò�����֧��8k/16k/22.05k/32k/44.1k/48k */
    unsigned int        bits;                       /* ����λ�� */
    unsigned int        uiPlayTime;                 /* ����ʱ��:��λms��0-����һ��,��0-���Ź̶�ʱ�䣬һ��������ظ����� */
    unsigned int        priority;                   /* ��Ƶ�������ȼ���ֵԽ�����ȼ�Խ��,��Χ1~10 */
    AUDIO_ENC_TYPE      audFileEncodeType;          /* �ļ���ʽ��Ŀǰ֧��WAVE��MP3 */
    AUDIO_QUE_STATE     que_state;                  /*������е�״̬*/

    /* ��Ƶ�ɼ��豸 �㲥��Ϣ */

    /* Android APK �㲥��Ϣ */
    unsigned int        isLocalplay;                /* �����Ƿ񲥷ţ�0:�����ţ�1:���� */

    unsigned int        isCurrentFileFinalData;     /* �Ƿ��ǵ�ǰ�ļ������һ�����ݣ�True��ʾ�ǣ�False��ʾ�� */
    unsigned char       reserve[28];                /* Ԥ�� */    
}AUDIO_FILE_BROADCAST_PARAM;

/*
    �Զ�Ӧ���ܽṹ��
    HOST_CMD_START_AUTO_ANSWER
*/
typedef struct
{
   char             *audioBuf;                      /* ��Ƶ���ݻ��� */
   unsigned int     audioDataLen;                   /* ��Ƶ���ݳ��� */
   unsigned char    reserve[32];                   /* Ԥ��         */
} AUTO_ANSWER_PARAM;

/*
    ��������¼��
    HOST_CMD_START_AUDIO_RECORD
*/
typedef struct tagAudioRecordPrmSt
{
   unsigned int     uiTime;                         /* ¼��ʱ��        */
   AUDIO_ENC_TYPE   type;                           /*��Ƶ¼�Ƹ�ʽ*/
   unsigned char    reserve[32];
}AUDIO_RECORD_PRM_S;

/*
    �����������ޱ�����ֵ
    HOST_CMD_SET_AUDIO_ALARM_LEVEL
*/
typedef struct tagAudioAlarmLevelPrmSt
{
    unsigned int bEnable;                           /* �Ƿ�ʹ�� */
    unsigned int uiLevel;                           /* ��ֵ     */
}AUDIO_ALARM_LEVEL_PRM_S;

typedef enum 
{
    AUDIO_ALL_VOLUME  = 0x0,                        /* ������Խ��������֣����������ͬһ������� */
    AUDIO_RING_VOLUME = 0x1,                        /* �������õ�������� */
    AUDIO_TALK_VOLUME = 0x2,                        /* �Խ����õ�������� */
    AUDIO_VOLUME_BUTT = 0x3
}AUDIO_VOL_TYPE;

/*
    ������Ƶ��������
    HOST_CMD_SET_AI_VOLUME
    ������Ƶ�������
    HOST_CMD_SET_AO_VOLUME
*/
typedef struct tagAudioVolPrmSt
{
    unsigned int uiChn;                             /* ���ػ���Զ�˵�ͨ����SPK����MIC.0:���أ�1:Զ��*/
    unsigned int uiVol;                             /* ��Ƶ����ֵ   */
    AUDIO_VOL_TYPE enVolType;                       /* �����������ͣ������������߶Խ����� */
}AUDIO_VOL_PRM_S;

/*
    ������Ƶ����������չ����
    HOST_CMD_SET_AI_VOLUME_EX
    ������Ƶ���������չ����
    HOST_CMD_SET_AO_VOLUME_EX
*/
typedef struct tagAudioVolExPrmSt
{
    unsigned int baseLevel;                         /* ��͵Ļ�׼����            */
    unsigned int stepSize;                          /* �Ի�׼���� ���������Ĳ��� */
}AUDIO_VOL_EX_PRM_S;

/*
    ������Ͳ�����л�
    HOST_CMD_SET_EAR_PIECE
*/
typedef struct tagAudioEarpiecePrmSt
{
    unsigned int uiIsUsed;                          /* �Ƿ�ʹ����Ͳ 1 ��ʾ��Ͳ��0 ��ʾ���� */
}AUDIO_EARPIECEL_PRM_S;

/*
    ������Ƶ����豸
    HOST_CMD_SET_AO_DEV
*/
typedef enum
{
    AO_DEV_LOCAL_SPK  = 0x0, /* ���������� */
    AO_DEV_EARPIECE   = 0x1, /* ��Ͳ */
    AO_DEV_EARPHONE   = 0x2, /* ���� */
    AO_DEV_AMPLIFIER  = 0x3, /* ���������� */
    AO_DEV_REMOTE_SPK = 0x4, /* Զ�������� */
    AO_DEV_BLUETOOTH  = 0x5, /* ��������豸 */
    AO_DEV_BUTT       = 0xFF
}AUDIO_OUTPUT_DEV;

/*
    ������Ƶ����豸
    HOST_CMD_SET_AO_DEV
*/
typedef struct tagAudioOutputDevPrmSt
{
    AUDIO_OUTPUT_DEV stOutDev; /* ��Ƶ����豸���� */
	unsigned int     devChn;   /* ͨ�� 			  */
	unsigned char               reserve[28];        /* ���� */

}AUDIO_OUTPUT_DEV_PRM;

/*
    ������Ƶ�����豸
    HOST_CMD_SET_AI_DEV
*/
typedef enum
{
    AI_DEV_LOCAL_MIC  = 0x0, /* ������˷� */
    AI_DEV_EARPIECE   = 0x1, /* ��Ͳ       */
    AI_DEV_EARPHONE   = 0x2, /* ����       */
    AI_DEV_PICKUP     = 0x3, /* ���ʰ���� */
    AI_DEV_REMOTE_MIC = 0x4, /* Զ����˷� */
    AI_DEV_EXTERNAL_DVD = 0x5, /* 3.5mm�ӿ����DVD�����豸 */
    AI_DEV_BLUETOOTH    = 0x6, /* ���������豸 */
    AI_DEV_BUTT         = 0xFF
}AUDIO_INPUT_DEV;

/*
    ������Ƶ�����豸
    HOST_CMD_SET_AI_DEV
*/
typedef struct tagAudioInputDevPrmSt
{
    AUDIO_INPUT_DEV stInDev;     /* ��Ƶ�����豸���� */
	unsigned int    devChn;      /* ͨ�� 			 */
	unsigned char   reserve[28]; /* ����             */

}AUDIO_INPUT_DEV_PRM;

#define AUDIO_MAX_MIX_CHN   (2)

/*
    �������ܵ�����
*/
typedef enum eAudioMixType
{
    AUDIO_MIX_TALK_BACK     = 0x0, /* �����Խ����� */
    AUDIO_MIX_SDK_TALK_BACK = 0x1, /* SDK�Խ�����  */
    AUDIO_MIX_BUTT          = 0xff
}AUDIO_MIX_TYPE;

/*
    �������� �������ʽ
*/
typedef enum eAudioMixDstType
{
    AUDIO_MIX_SAVE_PS  = 0x1, /* ����Ϊ ��PS  �� */
    AUDIO_MIX_SAVE_RTP = 0x2, /* ����Ϊ ��RTP ��  */

    AUDIO_MIX_SAVE_IPC_PS  = 0x4, /* ����Ϊ Զ��PS  �� */
    AUDIO_MIX_SAVE_IPC_RTP = 0x8, /* ����Ϊ ��RTP ��  */

    AUDIO_MIX_DST_BUTT = 0xff
}AUDIO_MIX_DST_TYPE;

/*
   ����ͨ������
*/
typedef struct tagAudioMixChnPrmSt
{
    unsigned int chn;       /* ͨ���� */
    unsigned char reserve[64];
}AUDIO_MIX_CHN_PRM_S;

/*
    ������������ �������ܵ�����
    HOST_CMD_START_AUDIO_MIX
*/
typedef struct tagAudioMixPrmSt
{
    unsigned int        mixNum;       /* �������ܵ�·����Ĭ��Ϊ2 DSP��ʱ��������ֶ� */
    AUDIO_MIX_TYPE      eMixType;     /* �������ܵ����ͣ�Ĭ��Ϊ0 */

    AUDIO_MIX_CHN_PRM_S stChnPrm[AUDIO_MAX_MIX_CHN]; /* �������ܸ�ͨ������ */

    unsigned int        eDstType;     /* �������ݵĴ���ʽ AUDIO_MIX_DST_TYPE */
    unsigned char       reserve[32];
}AUDIO_MIX_PRM_S;


/*
    ��Ƶ���������ص�����
*/
typedef enum 
{
    AUDIO_PLAY_CB_TYPE_MIN       = 0x0,
    AUDIO_PLAY_CB_TYPE_RING      = 0x1,                        /* �������� */
    AUDIO_PLAY_CB_TYPE_TTS       = 0x2,                        /* TTS���� */
    AUDIO_PLAY_CB_TYPE_BROADCAST = 0x3,                        /* ��ʱ�㲥���� */
    AUDIO_PLAY_CB_TYPE_MAX       = 0x4
}AUDIO_PLAY_CB_TYPE;

/*
    ��Ƶ���������ص����
*/
typedef struct tagAudioPlayCbResultSt
{
    unsigned int AudioID;            /* ��Ƶ��� */
    unsigned int isFullPlay;         /* ���ΪAudioID����Ƶ�Ƿ���ȫ����������1��ʾ��ȫ������0��ʾ��;��� */
    unsigned int interruptedPos;     /* ����ϵ�λ�ã��˱���Ϊ����ֵʱ�����壬��ֵΪ15����ʾ���ΪAudioID����Ƶ��15֡��֮���֡δ���� */
    unsigned int listIndex;          /* ͬһAudioID����Ƶ���ڲ��Ŷ���λ��1Ϊ��ʼλ�� */
    AUDIO_PLAY_CB_TYPE AudioPlayCbType;     /* ��Ƶ���������ص����� */
    unsigned char reserve[24];
}AUDIO_PLAY_CB_RESULT_ST;

/****************************************************************************/
/*                             ��������                                     */
/****************************************************************************/
/* Ԥץͼ�����ĺ궨�� */
#define PRE_SNAP_NUM                                (5)

#define MAX_JPGMOD_NUM                              (4)
/* ����ģ��������󳤶� */
#define FR_MAX_MODEL_DATA_LENTH                     (1024)
/* �������������� */
#define FR_MAX_FFD_FACE_NUM                         (10)

/* FR_LICENCE_STAT �㷨���ܿ���Ȩ״̬ */
typedef enum
{
     FR_LICENCE_NO = 0,
     FR_LICENCE_FAIL ,
     FR_LICENCE_SUCCESS
}FR_LICENCE_STAT;

/* ����������궨�壬float ���͵�����Ϊ��һ��ֵ */
typedef struct
{
    float x;                                        /* ���������� x */
    float y;                                        /* ���������� y */
}FR_POSITION;

/*
    ������ṹ�壬float ���͵�����Ϊ��һ��ֵ
*/
typedef struct _FR_CHARACT_POINTS
{
    float          faceX;                           /* �������� x */
    float          faceY;                           /* �������� y */
    float          faceW;                           /* �������   */
    float          faceH;                           /* �����߶�   */

    FR_POSITION    leftEye;                         /* ��������  */
    FR_POSITION    rightEye;                        /* ��������  */
    FR_POSITION    leftMouth;                       /* ���������  */
    FR_POSITION    rightMouth;                      /* ���ұ�����  */
    FR_POSITION    noseTip;                         /* ��������  */
}FR_CHARACT_POINTS;

/* ������������� */
typedef enum
{
    FR_LD_ONCE          = 0,                        /* ����һ�λ����� */
    FR_LD_START         = 1,                        /* �������������� */
    FR_LD_STOP          = 2,                        /* ֹͣ���������� */
    FR_LD_PIC           = 3,                        /* ʹ���·�ͼƬ������ */
    FR_LD_BUTT
}FR_LD_CRTL_TYPE;

/* ���ÿ����Ļ����㷨����, ����λ�����ã�֧�ֶ����㷨���� */
typedef enum
{
    FR_SOLID_SELF       = 0,                        /* �������л��� */
    FR_SOLID_YJY        = 1,                        /* �о�Ժ���� */
    FR_SOLID_INV        = 0xffffffff
}FR_SOLID_TYPE;

/*
    �����������ֽṹ�� HOST_CMD_FR_DET
*/
typedef struct frSolidPrmSt
{
    unsigned int      idx;                          /* �ص����� */
    FR_LD_CRTL_TYPE   eCtrlType;                    /* ������������� */
    LD_SCENE          scene;                        /* ������Ӧ�ó����Ͱ�ȫ�� */

    /* ����ֵ ֻ�п�������Ϊ ͼƬ���ʱ��Ч */
    unsigned int      uiNJpgSize;                   /* �ɼ���JPGͼƬ���� */
    unsigned int      uiIrJpgSize;                  /* ����JPGͼƬ���� */
    FR_CHARACT_POINTS nCPoints;                     /* �ɼ������������� */
    unsigned char*    jpgAddr;                      /* JPEG ���ݵĵ�ַ */
    unsigned int      totalLen;                     /* JPEG �����ܳ��� */
    unsigned int      retCropFrm;                   /* �Ƿ񷵻زü����ͼ�� */
    unsigned int      solidType;                    /* �����㷨���ͼ� FR_SOLID_TYPE */
    unsigned int      sceneFlag;                    /* ����ת�����أ���1ʱ��scene�����ڲ�ת�� ����ȥ��������ʵ�ֺ���ͷ�ļ� */
	unsigned char     reserve[28];                  /* ���� */
}FR_SOLID_PRM_S;

/* ����ȼ����� */
typedef enum 
{
    LD_OFF          = 0,                            /* ����ر� */
    LD_NORMAL       = 1,                            /* ����ͨ�õȼ� */
    LD_ENHANCE      = 2,                            /* ������ǿ�ȼ� */
    LD_PROFESSIONAL = 3,                            /* ����רҵ�ȼ� */
    LD_CUSTOM       = 4,                            /* �����Զ���ȼ� */
    LD_NONE,                                        /* ���׹رջ��� */
}LD_LEVEL_CTRL;

/*
    �¡������������ֽṹ�� HOST_CMD_FR_DET_V2
*/
typedef struct frSolidPrmStV2
{
    unsigned int      idx;                          /* �ص����� */
    LD_LEVEL_CTRL     liveLevel;                    /* ����ȼ� */
    unsigned char     reserve[128];                 /* ���� */
}FR_SOLID_PRM_S_V2;

/*
    ������ֵ���úͻ�ȡ�ṹ�� 
    HOST_CMD_SET_THRESHOLD
    HOST_CMD_GET_THRESHOLD
*/
typedef struct frSolidThershold
{
    unsigned int      fdType;                       /* 0->���л���;1->�о�Ժ���� */
    LD_LEVEL_CTRL     liveLevel;                    /* ����ȼ� */
    float             threshold;                    /* ������ֵ */
    float             maskThreshold;                /* ���ֻ�����ֵ */
    float             distance;                     /* ����:���� */
    unsigned char     reserve[32];                 	/* ���� */
}FR_SOLID_THRESHOLD;


/*
    ����͸��
    HOST_CMD_SET_SOLID_PARAM
    HOST_CMD_GET_SOLID_PARAM
*/
typedef struct frSolidTransPrmSt
{
    unsigned int cmd;
    unsigned long long value;
}FR_SOLID_TRANS_PRM_ST;

/*
    JPEGͼƬ������Ϣ ��Ӧ JPEG_TYPE
*/
typedef struct frBMJpegPrmSt
{
    unsigned int      uiSize;                       /* ͼƬ�����ݵĴ�С */
    unsigned char*    uiAddr;                       /* ͼƬ�����ݵĵ�ַ */
    unsigned int      picCnt;                       /* �������ݵ�JPG����*/
    unsigned int      picLen[MAX_JPGMOD_NUM];       /* ÿһ��JPG�ĳ���*/
    unsigned int      idNum[MAX_JPGMOD_NUM];        /* ��Ӧ��ID ��*/
    unsigned int      haveChPoints;                 /* �Ƿ����������*/
    FR_CHARACT_POINTS cPoints;                      /* ���������� ƥ���һ��*/
}FR_BM_JPEG_PRM_ST;

 /* BMPתJPG�ṹ�� */
typedef struct frJPGBMPPrmSt
{
    unsigned int    idx;                        /*�ص�����*/
    unsigned int    uiSize;                     /* JPG + BMP ���ݵ��ܴ�С */
    unsigned int    uiJpgSize;                  /* JPG���ݵĴ�С */
    unsigned int    uiBmpSize;                  /* BMP���ݵĴ�С */
    unsigned char*  uiAddr;                     /* JPG��BMP���ݵ�ַ,BMP��ǰ */
}FR_BM_JPG_BMP_PRM_ST;

 /* JPGתJPG�ṹ�� */
typedef struct frJPGJPGPrmSt
{
    unsigned int    idx;                        /* �ص����� */
    unsigned int    uiSize;                     /* ���ݵ��ܴ�С */
    unsigned int    uiJpgSize;                  /* JPG���ݵĴ�С */
    unsigned char*  uiAddr;                     /* JPG����ָ�� */
}FR_BM_JPG_JPG_PRM_ST;


/* 
	ͼƬ��С(Ŀǰ��֧�ַŴ�)���ر���ṹ�壻֧��jpg��png��bmp��
	1.��С����ͼƬ�ֱ��ʵȱ���������������������ʱ��ף������ʹ��,
	2.DSP�ڷֱ����ж���Ҫ���оƬ�Ͽ���ʱ�������¶��룻
	3.��Ӧ�������� HOST_CMD_FR_JPGSCALE  
*/

typedef struct frJPGScalePrmSt
{
    unsigned int    srcJpgSize;                	/* ������ͼƬ���ݵĴ�С���ڴ渴��ͼƬ�·������ܳ���2MB */
    unsigned char*  srcJpgAddr;                	/* ������ͼƬ����ָ��,������ͼƬ�ֱ��ʲ��ܳ���1080P(1920x1080)������ֱ�ӷ���ʧ�� */

    unsigned int    dstJpgSize;               	/* Ŀ��JPG���ݵĴ�С */
    unsigned char*  dstJpgAddr;              	/* Ŀ��JPG����ָ��,���ⲿ���룬��СΪ�ֱ��ʵ�1.5�����ڲ�����������ֱ��ʲ��ܴ���1080P�����Դ˴���󲻻ᳬ��3MB,�����ⲿֱ������3MB��С */

	unsigned int    dstJpgWidth;                /* Ŀ��JPG���ݵĿ�(�ⲿ����С�Ĵ�С),��Χ:[64~1920](dstJpgWidth x dstJpeHeight <= 1920x1080),�����Ž����ر���ʱdstJpgWidth��dstJpeHeight����Ϊȫ0         */
    unsigned int    dstJpeHeight;               /* Ŀ��JPG���ݵĸ�(�ⲿ����С�Ĵ�С),��Χ:[64~1920](dstJpgWidth x dstJpeHeight <= 1920x1080) �����Ž����ر���ʱdstJpgWidth��dstJpeHeight����Ϊȫ0*/

    unsigned int    dstJpgRealWidth;           	/* �ⲿ����ķֱ���û�ж���ʱ(����������оƬҪ�����)��DSP�ڲ����¶���(��125����124)������ֵ�����ķֱ�����Ϣ */
    unsigned int    dstJpeRealHeight;          	/* �ⲿ����ķֱ���û�ж���ʱ(����������оƬҪ�����)��DSP�ڲ����¶���(��125����124)������ֵ�����ķֱ�����Ϣ */
	
    unsigned char   reserve[56];        		/* ���� */

}FR_BM_JPG_SCALE_PRM_ST;


/*
    ��ģ������Ϣ����Ӧ��ͬ�� FR_DATA_TYPE_E
*/
typedef union frBuildModelPrmU
{
    FR_BM_JPEG_PRM_ST       stJpegTypePrm;          /* ��Ӧ JPEG/BMP/PNG ͼƬ����Ϣ */
}FR_BM_PRM_U;

/*
    �����Աȷ�ʽ����
*/
typedef enum frCPTypeE
{
    ONE_VS_ONE,                                     /* һ��һ */
    ONE_VS_N                                        /* һ�Զ� */
}FR_CP_TYPE_E;

/*�����⼰��ģ�����붨��*/
typedef enum frBMResultType
{
    RESULT_SUCCESS            = 0x00,               /* ��ģ�ɹ� */
    RESULT_LIVEDET_NOLIVE_ERR = 0x10,               /* ������Ϊ�ǻ��� */
    RESULT_LIVEDET_NOSURE_ERR = 0x11,               /* �������޷�ȷ�� */
    RESULT_ALIGN_WL_ERR       = 0x20,               /* �׹����ݶ���ʧ�� */
    RESULT_ALIGN_IR_ERR       = 0x21,               /* ������ݶ���ʧ�� */
    RESULT_BM_WL_ERR          = 0x30,               /* �׹��ȡģ������ʧ�� */
    RESULT_BM_IR_ERR          = 0x31,               /* ����ȡģ������ʧ�� */
    RESULT_SIM_COMPARE_ERR    = 0x40,               /* ���ƶȱȶ�ʧ��*/
    RESULT_FACEDET_IR_ERR     = 0x50,               /* ����������ʧ�� */
    RESULT_FACEDET_LD_ERR     = 0x51,               /* ����ڶ�·�������ʧ�� */
    RESULT_FACEDET_MATCH_ERR  = 0x52,               /* ˫Ŀ����ƥ��ʧ�� */

    RESULT_PIC_FACEDET_ERR    = 0x60,               /* ͼƬ�·��������ʧ�� */
    RESULT_PIC_NUM0_ERR       = 0x61,               /* ͼƬ�·��������Ϊ0*/
    RESULT_PIC_EYEDIS_ERR     = 0x62,               /* ͼƬ�·��ۼ���С*/
    RESULT_PIC_JPG_DEC_ERR    = 0x63,               /* ͼƬ�·�����ʧ��*/
    RESULT_PIC_JPG_DOWN_ERR   = 0x64,               /* ͼƬ�·�������ʧ��*/
    RESULT_PIC_RESIZE_ERR     = 0x65,               /* ͼƬ�·�resizeʧ��*/
    RESULT_PIC_MA_ALIGN_ERR   = 0x66,               /* ͼƬ�·����ݶ���ʧ�� */
    RESULT_PIC_BM_ERR         = 0x67,               /* ͼƬ�·���ģ ʧ��*/
    RESULT_PIC_FORMAT_ERR     = 0x68,               /* ͼƬ�·���ʽ���� */
    RESULT_PIC_DEC_DSPMEM_ERR = 0x69,               /* JPG������ĸ�ʽ�����Ż��ڴ淽��������DSP JPG�������ĳ��ø�ʽ3ͨ��YUV420����ڴ�*/

}FR_BM_RESULT_TYPE;

typedef struct frBMResultJPEGSt
{
    unsigned int   WLSize;                          /* �׹�ץ����jpgͼƬ��С */
    unsigned char* pWLAddr;                         /* �׹�ץ����jpgͼƬ��ַ */
    unsigned int   IRSize;                          /* ���ץ����jpgͼƬ��С */
    unsigned char* pIRAddr;                         /* ���ץ����jpgͼƬ��ַ */
    unsigned int   faceSize;                        /* 1VN����ץ����Ƭ���ٿ�ͼ�Ĵ�С--������Ҫ */
    unsigned char* pfaceAddr;                       /* 1VN����ץ����Ƭ���ٿ�ͼ�ĵ�ַ--������Ҫ */
}FR_BM_RESULT_JPEG_ST;

/* ���ص�ͼƬ����Ϣ */
typedef struct tagFrPicInfo
{
    unsigned char *paddr;                           /* ����ģ��ش���ͼƬ        */
    unsigned int   size;                            /* ����ģ��ش�ͼƬ���ݵĳ��� */
}FR_PIC_INFO;

/* ��ģ��ͼ���ܣ������������ԭʼͼƬ */
typedef struct tagFrSavePicInfo
{
    FR_PIC_INFO                  visYuv;            /* �ɼ���YUV��ͼ */
    FR_PIC_INFO                  visBigJpeg;        /* �ɼ���jpg��ͼ��1080P */
    FR_PIC_INFO                  visSmallJpeg;      /* �ɼ���jpgСͼ */  
    FR_PIC_INFO                  irYuv;             /* �����YUV��ͼ */
    FR_PIC_INFO                  irBigJpeg;         /* �����jpg��ͼ��1080P */
    FR_PIC_INFO                  irSmallJpeg;       /* �����jpgСͼ */

}FR_SAVE_PIC_INFO;

/* ������ģ��ͼ���ܿ��ƽṹ�� HOST_CMD_FR_SAVE_1VN_PIC */
typedef struct tagFrCtrlSavePic
{
    unsigned int                 enable;            /* ʹ�ܿ��� */
    unsigned char                flag[8];           /* 0-8��Ӧ��ϵ��0- �ɼ���YUV��ͼ;1-�ɼ���jpg��ͼ��1080P;
                                                                    2- �ɼ���jpgСͼ;3-�����YUV��ͼ;
                                                                    4-�����jpg��ͼ��1080P;5-�����jpgСͼ */           
    unsigned char               reserve[32];        /* ���� */

}FR_CTRL_SAVE_PIC;

/* ����1vN�ص����빦�ܿ��ƽṹ�� */
typedef struct tagFrCtrlEncPic
{
    unsigned char                flag[2];    /* flag[0]- �ɼ���jpg����;flag[1]-�����jpg���룬��0������1�رգ� */
}FR_CTRL_ENC_PIC;

/* ��ȫñ��ɫ */
typedef enum frSafetyHelmetColourTypeE
{
    FR_SAFETY_HELMET_RED    = 0,                    /* ��ɫ */
    FR_SAFETY_HELMET_YELLOW    ,                    /* ��ɫ */
    FR_SAFETY_HELMET_BLUE      ,                    /* ��ɫ */
    FR_SAFETY_HELMET_WHITE     ,                    /* ��ɫ */
    FR_SAFETY_HELMET_OTHER     ,                    /* ���� */
}FR_SAFETY_HELMET_COLOUR_TYPE_E;

/*
    ��ȫñ���ܽṹ��
*/
typedef struct stFrSafetyHelmetCtrl
{
    int                          enable;            /* �Ƿ�����ȫñ����: 0:�ر�  1:����*/
}FR_SAFETY_HELMET_CTRL;

/*

    ��ȫñ���ܷ��ؽ��
*/
typedef struct
{
    int                          isWear;            /* �Ƿ������ȫñ: 0:��  1:��*/
    unsigned int                 colour;            /* ��ȫñ��ɫ FR_SAFETY_HELMET_COLOUR_TYPE_E*/
    float                        colourTrust;       /* ��ɫ���Ŷ� */
} FR_SAFETY_HELMET_RESULT;

/*
    ��ͼƬ���ܲ����ṹ��
*/
typedef struct stFrInsertImagePrm
{
    unsigned char           type;                   /* �������ͣ�0��ͼ�ɼ���1��ͼ���⣬2˫ͼ */
    unsigned char           reserve[3];             /* reserve */
    unsigned char          *pFrmBuf[2];             /* ����yuvͼ��Ĵ洢buffer */
    unsigned int            frmLen[2];              /* ����yuvͼ��Ĵ洢���� */
    unsigned short          width[2];               /* ����yuvͼ��Ŀ� */
    unsigned short          height[2];              /* ����yuvͼ��ĸ� */
}FR_INSERT_IMAGE_PRM;

/*
    ��ͼƬ���ܽṹ��
*/
typedef struct stFrInsertImageCtrl
{
    int                          enable;            /* �Ƿ�����ͼƬ����: 0:�ر�  1:����*/
}FR_INSERT_IMAGE_CTRL;

/*
    ���л��岹����ֵ�жϽṹ��
*/
typedef struct stFrFaceLdLightCtrl
{
    int                          flag;            /* ���л���ƿر�־λ��0:δ�����趨��ֵ��1:����������ֵ */
    unsigned char                reserve[16];
}FR_FACELD_LIGHT_CTRL;

/*
    ���л����ͼ·���ṹ��
*/
typedef struct stFrFaceLdSavPicDirInfo
{
    unsigned char                *picDirName;	/* �Զ���·�� */
    unsigned char                reserve[64];
}FR_FACELD_SAV_PIC_DIR_INFO;


/*
    �׿�ɾ��ģ�ͽṹ��
*/
typedef struct stFrDelModelPrm
{
    unsigned int                 idx;            /* ɾ����ʼ��� */
    unsigned int                 cnt;            /* ����ɾ������ */
    unsigned char               *addr;           /* �׿��׵�ַ */
    unsigned char                reserve[16];
}FR_DEL_MODEL_PRM;

/*
    �׿��ѯ�����ģ�ͽṹ��
*/
typedef struct stFrOptModelPrm
{
    unsigned int                 idx;            /* ��ʼ��� */
    unsigned int                 cnt;            /* �������� */
    unsigned char               *addr;           /* �׿��׵�ַ */
    unsigned char               *modelAddr;      /* ���/ ADD ��ģ��buf ,Ӧ�÷����ڴ�*/
    unsigned int                 modelBufLen;    /* װģ�͵�bufLen,Ӧ������*/
    unsigned  int                cmpEnable;      /* ���ع���ʹ�� 0->�ر� 1->���� */
    float                        threshold;      /* ���ƶ���ֵ */
    int                          topId;          /* ������ƶ�ID */
    int                          isSim;          /* �Ƿ��ظ� 0  ->���ظ� 1->�ظ� */
    unsigned char                reserve[12];
}FR_OPT_MODEL_PRM;

/*
    1VN �ȶԽṹ��
*/
typedef struct stFrCmpModelPrm
{
    unsigned char               *addr;           /* �׿��׵�ַ */
    unsigned char               *modelAddr;      /* ���/ ADD ��ģ��buf ,Ӧ�÷����ڴ�*/
    unsigned int                 modelBufLen;    /* װģ�͵�bufLen,Ӧ������*/
    unsigned int                 top1Id;         /* �׿����е�ID ��*/
    float                        top1Sim;        /* �׿����е����ƶ�*/
    unsigned char                reserve[16];
}FR_CMP_MODEL_PRM;


/*
    ��ȶ�ģʽ
*/
typedef enum stFrFaceFeatreCpModeE
{
    DSP_FR_FACE_FEATRE_CP_MODE_SINGLE = 0,          /* ����ȶ�ģʽ */
    DSP_FR_FACE_FEATRE_CP_MODE_MULTI,               /* ���ȶ�ģʽ���ȶԱ������N�� */
    DSP_FR_FACE_FEATRE_CP_MODE_MAX,
} FR_FACE_FEATRE_CP_MODE_E;

/* 1VN�ļ�������N���ģ������ */
typedef enum stFrFaceFeatreCpAttrE
{
    DSP_FR_FACE_FEATRE_CP_ATTR_NORMAL = 0,          /* ��ͨ���������׿⣬ֻ��һ���Ļ�Ͽ�Ҳ����ͨ�� */
    DSP_FR_FACE_FEATRE_CP_ATTR_MASK,                /* ���ڵ��������������ֵ����������׿� */
    DSP_FR_FACE_FEATRE_CP_ATTR_MAX,
}FR_FACE_FEATRE_CP_ATTR_E;

/*
    STREAM_ELEMENT_FR_BM ��ģ���ݷ��ؽṹ��
*/
typedef struct _FR_BM_RESULT_
{
    int                          uiId;                                  /* ID�ţ�����Ӧ���·� ID   */
    unsigned int                 bmResult;                              /* �Ƿ�ģ�ɹ�   */
    unsigned int                 model_data_size;                       /* ģ�����ݴ�С   */
    unsigned char                model_data[FR_MAX_MODEL_DATA_LENTH];   /* ����ģ������   */

    float                        pitch;                                 /* ƽ�������¸����ǣ���������Ϊ��  */
    float                        yaw;                                   /* ƽ��������ƫת�ǣ���������Ϊ��  */
    float                        roll;                                  /* ƽ������ת�ǣ�����˳ʱ����תΪ��*/
    float                        eyedis;                                /* ����ͫ�׾�*/
    float                        visible;                               /* �ܼ����ڵ��̶�*/
    float                        faceDist;                              /* ������������ͷ�ľ�����Ϣ*/
    float                        faceSocre;                             /* ��������*/

    FR_CHARACT_POINTS            cPoints;                               /* ��������Ϣ*/

    FR_SAFETY_HELMET_RESULT      stSafetyHelmetResult;                  /* ��ȫñ������Ϣ*/

    unsigned int                 mask;                                  /* �����ֽ�� 0��������,1������,-1���ֽ����ȷ�� */
    float                        maskConfidence;                        /* �����ֵ����Ŷ�*/

    unsigned long                bmFNum;                                /* ��ģ���ŵ�frame���,������ȡԤץͼ��*/
    FR_BM_RESULT_JPEG_ST         stBMResultJpeg;                        /* ��ģ����JPEGͼƬ��Ϣ*/
    FR_BM_RESULT_TYPE            stBMResultType;                        /* ��ģ���ؽ������*/
    unsigned long long           picPts;                                /* ���ɸý�ģͼƬ��ʱ��� */

    FR_SAVE_PIC_INFO             stSavePic;                             /* ��ͼ����ͼƬ��Ϣ���������¼��ϱ����� */

	float                        leftEyeOpen;                           /* ���ۿ��ճ̶� */
	float                        rightEyeOpen;                          /* ���ۿ��ճ̶� */
	float                        mouthOpen;                             /* ��Ϳ��ճ̶� */

	char                         glass;                                 /* �۾���0��ʾ���۾���1��ʾ���۾���2��ʾ��ī����-1��ʾδ֪ */
    char                         hat;                                   /* ñ�ӣ�0��ʾ��ñ��,1��ʾ��ñ�ӣ�-1��ʾδ֪ */
    char                         beard;                                 /* ���ӣ�0��ʾ��/С���ӣ�1��ʾ����룬-1��ʾδ֪ */
    char                         gender;                                /* �Ա�0��ʾŮ��1��ʾ�У�-1��ʾδ֪ */
    char                         race;                                  /*  */
    char                         nation;                                /*  */
    char                         express;                               /* ���飬0��ʾ���ԣ�1���ˣ�2����, 3��ʾ���£�, 4��ʾ���, 5��ʾ�ѹ�, 6��ʾ��ŭ��-1��ʾδ֪ */
    char                         age;                                   /* ���䣬����0~99��С��1����Ķ���Ϊ0�꣬���ڵ���99�궼��Ϊ99�� */
    char                         ageStage;                              /* ����Σ�0 ��ʾ��ͯ 0-6�꣬ 1��ʾ���� 7-17�꣬2��ʾ���� 18-40�꣬3��ʾ���� 41-65�꣬4��ʾ���� >=66 */
    char                         hairStyle;                             /* ���ͣ�Ԥ�� */
    char                         faceShape;                             /* ���ͣ�Ԥ�� */
	unsigned int                 faceTrkId;                             /* ��������ID��*/    
    float                        liveTrust;                             /* �������Ŷ�*/
    char*                        pFileName;                             /* �ļ���*/
    int                          liveType;                              /* �������� 0-�رջ��� 1-�׹���� 2-������*/
    float                        clearSocre;                            /* ��������������*/
    float                        bm1vNSim;                               /* ͼƬ�·�����1vN������ƶ�*/
    unsigned int                 bm1vNId;                                /* ͼƬ�·�����1vN������ƶȶ�Ӧ��Ա*/

    unsigned char                reserve[36];                           /* ���� */
}FR_BM_RESULT;

typedef struct _FR_1VN_RESULT_
{
    FR_BM_RESULT                 stBmResult;                            /* ��ģ�����Ϣ */

    float                        fsimVisVsNir;                          /* �ɼ���ͺ���ȶԵ����ƶ�*/
    float                        top1Fsim;                              /* top1 ���е����ƶ�*/
    unsigned int                 top1Id;                                /* top1 ���е�ID ��*/
    unsigned int                 listVersion;                           /* ������汾��Ϣ*/
    unsigned int                 liveResult;                            /* ������0-���壬1-���ˣ�0xbad-δ֪ */
    float                        liveTrust;                             /* �о�Ժ�������Ŷ� */

    /* �������Ч: ��ǰץ��ͼƬ�Ͳ�ͬ����N��ģ�ͶԱȵ�TOP1���ƶ�ֵ��[0]=ץ��VS��ͨ��, [1]=ץ��VS���ֿ� */
    int                          featreCompareMode;                     /* �� FR_FACE_FEATRE_CP_MODE_E ���õ���ȶԡ����Ƕ��ȶ�*/
    float                        multiTop1Fsim[DSP_FR_FACE_FEATRE_CP_ATTR_MAX];
    unsigned int                 multitop1Id[DSP_FR_FACE_FEATRE_CP_ATTR_MAX];    
    unsigned int                 frmIdx;                                /*����ʶ���֡��*/
    char                         faceIdx;                               /*��ǰ�������������еĵڼ���*/
    char                         faceCnt;                               /*��һ֡�����⵽����������*/

    unsigned char                liveMerge;                            /* 0->�������ϲ�δ���ã�1->����ϲ�������ã�ʹ���µı��� liveBody ���ݽ�� */
    int                          liveBody;                             /* �������� 0->����; 1->����; 0xbad->δ֪ */
    unsigned char                reserve[21];                           /* Ԥ������ */
    
}FR_1VN_RESULT;


/* HOST_CMD_FR_SIM_THRESHOLD �⼰�ȶ���ֵ���� */
typedef struct _FR_1VN_SIM_THRESHOLD_
{
    /* �� FR_FACE_FEATRE_CP_MODE_E ���õ���ȶԡ����Ƕ��ȶ�*/
    int                         featreCompareMode;
    /* �� FR_FACE_FEATRE_CP_ATTR_E ���ÿ�����ԣ�����ģʽ��ͨ���ñ���ָ���ȶԵĿ⣬���ģʽ�¸�������ʱ�������� */
    int                         featreCompareAttr; 

    /* ���ò�ͬ���Կ����ֵ��[ץ��ͼ][�׿�]: [0][0]=��ͨVS��ͨ��, [1][1]=����VS���ֿ�, [0][1]=��ͨVS���ֿ�, [1][0]=����VS��ͨ�� */
    float                       simThreshold[DSP_FR_FACE_FEATRE_CP_ATTR_MAX][DSP_FR_FACE_FEATRE_CP_ATTR_MAX];

    unsigned char               reserve[32];
}FR_1VN_SIM_THRESHOLD;

typedef struct tagDfrPicInfoSt
{
    unsigned int uiWitdh;                           /* ����ͼ����     */
    unsigned int uiHeight;                          /* ����ͼ��߶�     */
    unsigned int uiLen;                             /* ����ͼ�����ݳ��� */
}DFR_PIC_INFO_ST;

/*
    STREAM_ELEMENT_FR_DEC �����������ؽṹ��
*/
typedef struct tagFrDetResultSt
{
    int                 idx;                        /* Ψһ��ʶ����ֱ�ӷ���Ӧ���·�ID */
    unsigned int        detectionResult;            /* �Ƿ��ǻ��� */
    unsigned int        ifIrFace;                   /* �Ƿ����ͼ���⵽���� */

    DFR_PIC_INFO_ST     stIrFacePicInfo;            /* ����ٳ���������ͼ����Ϣ   */
    DFR_PIC_INFO_ST     stWlFacePicInfo;            /* �ɼ���ٳ���������ͼ����Ϣ */

    DFR_PIC_INFO_ST     stIrPicInfo;                /* ����ԭʼ��ͼ����Ϣ   */
    DFR_PIC_INFO_ST     stWlPicInfo;                /* �ɼ���ԭʼ��ͼ����Ϣ */

    FR_CHARACT_POINTS   irCPts;                     /* ���������������*/
    unsigned int        tLen;                       /* ����Ϳɼ������ݵ��ܳ�*/
}FR_DET_RESULT;

/* ������̬��Ϣ */
typedef struct tagFrFacePostureInfo
{
    float             clearity;                     /* ���������� */
    float             frontal_conf;                 /* ��������   */
    float             pitch;                        /* ƽ�������¸����ǣ���������Ϊ��  */
    float             yaw;                          /* ƽ��������ƫת�ǣ���������Ϊ��  */
    float             roll;                         /* ƽ������ת�ǣ�����˳ʱ����תΪ��*/
    float             eyedis;                       /* ����ͫ�׾�*/    
    float             reserve[64];                 /* Ԥ���ֶ� */
}FR_FACE_POSTURE_INFO;

/*
    STREAM_ELEMENT_FR_FFD_IQA ������� �Ա� ���ؽṹ��
*/
typedef struct tagFrFfdIqaSingle
{
    int                     idx;                    /* Ψһ��ʶ����ֱ�ӷ���Ӧ���·�ID */
    unsigned int            ifface;                 /* �Ƿ��⵽����    */
    FR_CHARACT_POINTS       cPoints;                /* ��������Ϣ*/
    FR_FACE_POSTURE_INFO    stFPInfo;               /* ��⵽��������̬��Ϣ */
    FR_PIC_INFO             stFdPicInfo;            /* ��⵽����������Ӧ��ͼƬ */

    unsigned long           fdFNum;                 /* ��������frame���   */
    unsigned int            irLength;               /* �Ҷ�ͼ��С            */
    unsigned int            beFirstFFD;             /* ==1ʱΪ�ƶ����ǰ�ĵ�һ��������� */    
    unsigned char           reserve[32];           /* Ԥ���ֶ� */
}FR_FFD_IQA_SINGLE;


/*
    STREAM_ELEMENT_FR_FFD_IQA ������� �Ա� ���ؽṹ��
*/
typedef struct tagFrFfdIqaResult
{
    int               idx;                          /* Ψһ��ʶ����ֱ�ӷ���Ӧ���·�ID */
    unsigned int      facenum;                      /* ��⵽����������    */

    FR_FFD_IQA_SINGLE stFFdIqaResult[FR_MAX_FFD_FACE_NUM];   /* ��⵽��������Ϣ */
    FR_PIC_INFO       stFrWlPic;                    /* �ɼ���ͼƬ       */
    FR_PIC_INFO       stFrIrPic;                    /* �����ͼƬ       */    
    unsigned char     reserve[32];                 /* Ԥ���ֶ� */
}FR_FFD_IQA_RESULT;

/*
    DSP ������������
*/
typedef enum
{
    FR_FD_ONESHOT = 0,                              /* DSP ����ʽ��� */
    FR_FD_CONTINUE,                                 /* DSP ����ʽ��� */
    FR_FD_ONESHOT_JPG,                              /* DSP JPEG��ʽ������ⷵ������� */

    FR_FD_CONTINUE_NO_BM,                           /* DSP ����ʽ���,�������ؽ�ģ���� */
    FR_FR_INVALID                                   /* DSP ��Ч�ļ�� ֹͣ������� */
}FR_FD_TYPE;

/* 
	�·�ͼƬ�ĸ�ʽ��JPG/BMP/PNG ����ͳһʹ�� DSP_FR_DOWNLOAD_JPEG  
*/
typedef enum frDataTypeE
{
    DSP_FR_DOWNLOAD_JPEG        = 0x100200,         /* ��ԴΪ �·� JPG/BMP/PNG  ͼƬ  */
    DSP_FR_DOWNLOAD_YUV,                            /* ��ԴΪ �·� YUV  ����  */
    DSP_FR_DOWNLOAD_COMP,                           /* ��ԴΪ �·� COMP ͼƬ,������Կʹ��   */
    DSP_FR_DOWNLOAD_PIONS,                          /* ��ԴΪ �·� ͼƬ + ��������Ϣ */
    
    DSP_FR_MAX_TYPE,
}FR_DATA_TYPE_E;

/* ���ؼ��İ��졢��ҹģʽ */
typedef enum frDetTypeE
{
    FR_DET_TYPE_WL = 0,
    FR_DET_TYPE_IR,
    FR_DET_TYPE_MAX,
}FR_DET_TYPE_E;

/* Ҫ��ص���ͼƬ���� */
typedef enum
{
    FR_BM_CB_TYPE_JPEG   = 0x00,                    /* �ص�֤����jpegͼ��ģ�� */
    FR_BM_CB_TYPE_COMP,                             /* �ص�֤���յ�������Կ���ݺ�ģ�� */
    FR_BM_CB_TYPE_JPEGONLY,                         /* �ص�֤���յ�jpegͼ��  */
    FR_BM_CB_TYPE_BMPONLY,                          /* �ص�֤���յ�bmpͼ��  */
    FR_BM_CB_TYPE_MAX,                              /* ��Чֵ */
} FR_BM_CB_TYPE_E;

/*
    HOST_CMD_FFD_CTRL ���������Ϣ�� STREAM_ELEMENT_FR_FFD_IQA �ص� ���������������
*/
typedef struct frFfdPrmSt
{
    int                     idx;                    /* �ص����� */
    FR_FD_TYPE              fdType;                 /* DSP ������������ */
    unsigned int            fdnum;                  /* ������������������ ȡֵ0��1ʱ��DSP ֻ���һ������ */
    unsigned int            solid;                  /* �Ƿ��������       */
    FR_DET_TYPE_E           eDataType;              /* ������Դ����       */
    FR_BM_PRM_U             uDataInfo;              /* ��������           */
    FR_FACE_POSTURE_INFO    stFPInfo;               /* ������̬���ۼ��Ĺ��� */

    float                   fdConfidence;           /* �����������Ŷ� -1 Ϊ��Чʹ��Ĭ��ֵ�����ϴ����õ�ֵ */
    unsigned int            maskEnable;             /* ������ʹ�ܣ�0�ǹرգ�1�ǿ�����Ĭ�Ϲر� */
    unsigned int            depthEnable;            /* ��๦�ܿ��أ�0�رգ�1����*/
    FR_INSERT_IMAGE_PRM     stInsertImgPrm;         /* ��ͼƬ����������Ϣ */
    FR_CTRL_ENC_PIC         stEncPicCtrl;           /* �ɼ���������뿪�� */
    unsigned char           faceCrop;               /* ������ͼJPG��Ӧ��,0�ǹرգ�1�ǿ�����Ĭ�Ϲر�*/
    unsigned char           faceDraw;               /* ԭʼͼ�ϻ����������JPG,0�ǹرգ�1�ǿ�����Ĭ�Ϲر�*/
    unsigned int           	faceTrack;             	/* �������ٹ��ܿ��أ�Ĭ�Ϲرգ�0->�رգ�1->�� */

    //unsigned char           distFlag;				/* ʹ��ʶ�����ʱ�Ŀ��أ�0->ʹ���ۼ��stFPInfo.eyedis,1->ʹ�þ���recogdist */
    //float           		recogdist;              /* ʶ����룬����0ʱ���ã�����0ʹ��eyedis */
    unsigned char           reserve[60];             /* Ԥ���ֶ� */
}FR_FFD_PRM_S;

/*
    ��ģ������Դ����
*/
typedef enum frOpTypeE
{
    FR_REGISTER,                                    /* ��ģע��         */
    UN_REGISTER,                                    /* ȥע��           */
    UN_REGISTER_LIST,                               /* ������ɾ����     */
    NO_REGISTER                                     /* ����ע����ز��� */
}FR_OP_TYPE_E;

/*
    ����������⹦�ܿ�����Ϣ
*/
typedef struct _FR_LIVTEST_CTRL_
{
    unsigned int ctrl;                              /* �Ƿ��������� */
    unsigned int time;                              /* ���ʱ�� */
}FR_LIVTEST_CTRL;

/* ��ģ�����ֶ�Ӧ����Ϣ HOST_CMD_FR_BM */
typedef struct frBuildModelPrmSt
{
    int                     idx;                    /* Ψһ��ʶ������Ӧ���·�ID���ص�����ֱ�ӷ��� */
    FR_OP_TYPE_E            eOpType;                /* ��ģ��ͬʱҲͬʱ��ɶ��㷨�ڰ�������ע�� or ȥע�� */
    FR_DATA_TYPE_E          eDataType;              /* ��ģ������Դ����  */
    FR_BM_PRM_U             uBMPrm;                 /* ������Դ���Ͷ�Ӧ�ľ�����Ϣ */

    FR_LIVTEST_CTRL         stLive;                 /* ���幦�ܿ���      */
    
    int                     idPhoto;                /*0Ԥ�죬1���뽨ģ*//* �Ƿ񷵻�֤���գ� 0-ֱ��ʹ���·�ͼƬ��������⽨ģ 1-�·�ͼƬת����֤�����ٽ�ģ */  
    int                     enableQs;               /* ʹ���������ֹ��� 0-��ʹ���������� 1-ʹ���������ֹ���*/
    int                     maskEnable;             /* ʹ�ܿ��ּ�⹦�� 0-disable 1-enable */
    
    FR_BM_CB_TYPE_E         eCbType;                /* �ص�ͼ������ */
    
    FR_CHARACT_POINTS       frPoins;                /* ������ؼ�����Ϣ */
    void                    *priv;                  /* ˽����Ϣ */
    unsigned int            privLen;                /* ˽����Ϣ���� */
    int                     bm1vNEnable;            /* ͼƬ�·�������1vNʹ�� 0-��ʹ�� 1-ʹ�� */

    unsigned char           reserve[28];            /* Ԥ���ֶ� */
}FR_BUILD_MODEL_PRM_ST;

/*
    ����ģ�ͶԱ� HOST_CMD_FR_COMPARE
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
    ����ģ��У�� HOST_CMD_FR_CHECK_MODEL
*/
typedef struct frModelCheckSt
{
    unsigned char   model[1536];
    unsigned int    modelLen;
    unsigned int    modelFit;                       /*0��ʾ����1��ʾ������ 2��ʾδ����(��β���ȷ)*/
} FR_MODEL_CHECK_ST;

/*
    �����ⷽʽ����
*/
typedef enum frLDTypeE
{
    ONE_SHOT,                                       /* ���μ�� */
    CONTINUOUS                                      /* ��μ�� */
}FR_LD_TYPE_E;


/* ���� ROI ������򣬲���Ϊ�ٷֱȵķ���������Ҫ����Ϊ�������ͼ������ HOST_CMD_FR_ROI */
typedef struct stFrRoiRectSt
{
    unsigned int        roiEnable;                  /* ROI ʹ�ܿ��� */
    float               x;                          /* ��������Ϊͼ���һ����İٷֱ� */
    float               y;                          /* ƫ������ y */
    float               width;                      /* ƫ�ƿ��   */
    float               height;                     /* ƫ�Ƹ߶�   */
}FR_ROI_RECT;


/* �������ڴ�ͬ�� */
typedef struct
{
    unsigned int   eleCnt;                          /* ��������Ԫ�صĸ��� */
    unsigned int   elePadCnt;                       /* �������ﲹ��Ԫ�صĸ���һ��1000 */
    unsigned int   eleAllCnt;                       /* ������������Ԫ�صĸ��� */
    unsigned int   featLen;                         /* ��������ÿ��Ԫ��������ģ�͵ĳ��� */
    unsigned int   addInfoLen;                      /* ��������ÿ��Ԫ����Ӧ�ø��ӵĳ��� */
    unsigned int   eleSize;                         /* ��������ÿ��Ԫ�ص��ڴ��С */
    unsigned int   memSize;                         /* ������������ڴ��С */
    unsigned int   headSize;                        /* ������ͷ�Ľṹ���С */

    unsigned int   poolVersion;                     /* ������汾�ţ�Ӧ�ò���д�������ȡ */
    unsigned char* virtHead;                        /* ��������ͷ�����׵�ַ */
    unsigned char* physHead;                        /* ��������ͷ�������׵�ַ */
    unsigned char* virtPool;                        /* ��������ele�����׵�ַ */
    unsigned char* physPool;                        /* ��������ele�������׵�ַ */
    unsigned char  reserve[12];                     /* ��������ı����ֶ� */
}FACE_MODEL_POOL_ST;

/* ����������� */
typedef struct tagFacePoolCnt
{
    int             poolFeatType;                   /* �����������: ��ͬ�����ͱ�ʾ��ͬ������N�⡣ʹ�� FR_FACE_FEATRE_CP_ATTR_E ������ֵ */
    void*           virtAddr;
    void*           poolLockFunc;                   /* ������������ */
    void*           poolUnlockFunc;                 /* ������������� */
    void*           flushCacheFunc;                 /* ������ˢcache,д���ڴ溯�� */
}FACE_POOL_PRM;

/* ����������ͼҵ������ */
typedef enum stFrFaceCropTypeE
{
    DSP_FR_FACE_CROP_TYPE_NONE,
    DSP_FR_FACE_CROP_DFR_FD,                        /* �������ɹ���������ͼ */
    DSP_FR_FACE_CROP_DFR_BM,                        /* ������ģ�ɹ���������ͼ */
    DSP_FR_FACE_CROP_DOWNLOAD_PIC,                  /* ���·�ͼƬ��������ͼ */
    DSP_FR_FACE_CROP_TYPE_INVALID,
}FR_FACE_CROP_TYPE_E;

/* ʹ�õĿ�ͼ���� */
typedef enum stFrFaceCropMethodE
{
    DSP_FR_FACE_CROP_METHOD_NONE,
    DSP_FR_FACE_CROP_DYNAMIC_RATIO,                 /* ����ͼƬ��������������ñ��� */
    DSP_FR_FACE_CROP_FIXED_RATIO,                   /* ����ͼ�������ñ�����ʽ��� */
    DSP_FR_FACE_CROP_FIXED_HEIGHT,                  /* ����ͼ���չ̶��߶ȿ�ͼ��� */
    DSP_FR_FACE_CROP_FIXED_WH,                      /* ����ͼ���չ̶��ֱ������ */
    DSP_FR_FACE_CROP_METHOD_INVALID,
}FR_FACE_CROP_METHOD_E;

/* ����������ͼ HOST_CMD_FR_FACE_CROP */
typedef struct stFrFaceCrop
{
    unsigned int        cropType;                   /* ʹ�� FR_FACE_CROP_TYPE_E ָ�������� */
    unsigned int        cropEnable;                 /* ������ͼʹ�ܿ��� ʹ��1  �ر�0 */
    unsigned int        cropMethod;                 /* ʹ�� FR_FACE_CROP_METHOD_E ָ�������� */

    /* cropMethod = DSP_FR_FACE_CROP_DYNAMIC_RATIO ʱ��������Ĳ��� */
    float               totalfaceWidthRatio;        /* ��ͼ���ʹ��������ȵı���ϵ�� */
    float               upperPartHeightRatio;       /* ��ͼ�߶�ʹ��������ȵı���ϵ�� */
    float               underPartHeightRatio;       /* ��ͼ�����岿��ʹ�������߶ȵı���ϵ�� */

    /* cropMethod = DSP_FR_FACE_CROP_FIXED_RATIO ʱ��������Ĳ��� */
    float               faceWidthRatio;             /* ��ͼ���ʹ��������ȵı���ϵ�� */
    float               faceHeightRatio;            /* ��ͼ�߶�ʹ��������ȵı���ϵ�� */

    /* cropMethod = DSP_FR_FACE_CROP_FIXED_HEIGHT ʱ��������Ĳ��� */
    int                 fixedHeight;                /* ��ͼ�̶��߶� */

    /* cropMethod = DSP_FR_FACE_CROP_FIXED_WH ʱ��������Ĳ��� �� fixedHeight */
    int                 fixedWeight;                /* ��ͼ�̶��߶� */
}FR_FACE_CROP;

/*
    ��ȡ���ƶȱȶ���ֵ
    HOST_CMD_FR_GET_SIM_THRESHOLD
*/
typedef struct frSimThresholdPrmSt
{
    float           simThreshold[FR_DET_TYPE_MAX][DSP_FR_FACE_FEATRE_CP_ATTR_MAX];    /* �Ƽ��ȶ���ֵ��simThreshold[0]--����ģʽ��ֵ��simThreshold[1]��ҹģʽ��ֵ
                                                                                         simThreshold[0][n] : ����ģʽ��0--�ǿ�����ֵ��1--������ֵ
                                                                                         simThreshold[1][n] : ��ҹģʽ��0--�ǿ�����ֵ��1--������ֵ */
    float           simThreshold1v1;                                                  /* 1v1 �Ƽ��ȶ���ֵ */
    unsigned char   reserve[128];                                                     /* ���� */
}FR_SIM_THRESHOLD_PRM_ST;

/****************************************************************************/
/*                      ��Ĥ����ģ��                                        */
/****************************************************************************/
#define IRIS_MAX_MODEL_DATA_LENTH      (4096)

/* ��Ĥģ�������� */
typedef struct
{
    unsigned int modelSize;                          /* ������Ĥģ�ʹ�С */
    unsigned int supportModelNum;                    /* �㷨֧�ֵĺ�Ĥģ���������� */
    unsigned int issupportRepeat;                    /*�㷨�Ƿ�֧�ֲ��� */
    float        threshold1v1;                       /* 1V1�ο���ֵ */
    float        threshold1vN;                       /* 1VN�ο���ֵ */
	unsigned int eyeNum1vn;							 /* ʶ���۾����� */
	unsigned int curEyeNum; 						 /* ��ǰ��Ĥ���� */

    unsigned char reserve[24];                       /* �����ֶ� */
} IRIS_ABILITY;

/* ���ص�ͼƬ����Ϣ */
typedef struct
{
    unsigned char          *pAddr;                  /* ��Ĥģ��ش���ͼƬ */
    unsigned int            size;                   /* ��Ĥģ��ش�ͼƬ���ݵĳ��� */

    unsigned char          reserve[32];             /* �����ֶ� */
}IRIS_BUF_INFO;

/* ���ں�Ĥͼ����Ͳɼ������е��˹�/�����Ա�ע */
typedef struct
{
    unsigned char           isEnable;               /* ��עʹ�ܿ��� */
    unsigned char           eyeType;                /* �۾�����: 0-������1-�۾���2-��ͫ��3-͸������ */
    unsigned char           cbPicEnable;            /* �Ƿ����ص�ͼƬ */
    unsigned char          *pLabel;                 /* �Ժ�Ĥͼ������ַ���ע������������ 1024�ֽ� */
    unsigned int            labelLen;               /* ��ע���ַ������� */
    unsigned int            totalCnt;               /* ��λ�����ڲɼ�������, ͨ��������д */

    unsigned char           reserve[32];            /* �����ֶ� */
} IRIS_LABEL_INFO;

/* ��Ĥͼ���ڴ���Ϣ */
typedef struct
{
    IRIS_BUF_INFO           facePic;                /* ����ͼ������������Ĥ����ҵ��*/
    IRIS_BUF_INFO           srcIrisPic;             /* ��Ĥ����ͼ�� */
    IRIS_BUF_INFO           irisPic[2];             /* ��Ĥͼ�� */

    unsigned char           reserve[32];            /* �����ֶ� */
} IRIS_PIC_INFO;

/* ��Ĥ�ؼ�����Ϣ */
typedef struct
{
    REGION_INFO             facePoint;              /* ������λ������ */
    REGION_INFO             srcIrisPoint[2];        /* ��Ĥ����ͼ��������λ������*/
    REGION_INFO             irisPoint[2];           /* ��Ĥͼ��������λ������ */

    unsigned char           reserve[32];            /* �����ֶ� */
} IRIS_POINTS_INFO;

/* ��Ĥͼ����׶κ�ʱͳ�� */
typedef struct
{
    unsigned long           detectTime;             /* ���ۼ��+�ü��Ȼ������ĵ�ʱ�� */
    unsigned long           enrollTime;             /* ���������ж�+��ģ+ѡ֡�Ȼ������ĵ�ʱ�� */
    unsigned long           matchTime;              /* ���������ж�+��ģ+�ȶԵȻ������ĵ�ʱ�� */
    unsigned long           bmTime;                 /* ͼƬ�·���ģ��ʱ�� */

    unsigned char           reserve[32];            /* �����ֶ� */
} IRIS_TIME_COUNT ;

/* ��Ĥ������� */
typedef enum
{
    IRIS_DETECT_TYPE_CAPT_1VN = 1,                  /* һֱ���к�Ĥ���ʶ���1VN���� */
    IRIS_DETECT_TYPE_CAPT_ENROLL,                   /* һֱ���к�Ĥ����ע������ */
    IRIS_DETECT_TYPE_CAPT_ENROLL_REPEAT,            /* ���أ�Ԥ�� */
    IRIS_DETECT_TYPE_CAPT_STOP,                     /* ֹͣ��Ĥע��orʶ��*/
    IRIS_DETECT_TYPE_CAPT_INVALID                   /* ��Ч����ʹ��*/
} IRIS_DETECT_TYPE;

/* ������ģ��ͼ���ܿ��ƽṹ�� HOST_CMD_FR_SAVE_1VN_PIC */
typedef struct tagIrisCtrlSavePic
{
    unsigned int                 detEnable;            /* ���ۼ���ͼʹ�ܿ��� */
    unsigned int                 detFlag;              /* ��Ӧ��ϵ:0X01- ��Ĥԭͼjpg; 0X02-����ͼbmp��VGA; 0X03-����*/
    unsigned int                 matchEnable;          /* 1VN��ͼʹ�ܿ��� */
    unsigned int                 matchFlag;            /* ��Ӧ��ϵ:0X01- ��Ĥԭͼjpg; 0X02-����ͼbmp��VGA�� 0X03-��Ĥԭͼjpg������ͼbmp�� 0X04-�ɼ���ͼ��JPG; 0X07-����*/
                                                                   
    unsigned char                reserve[32];          /* ���� */
}IRIS_CTRL_SAVE_PIC;

/*  HOST_CMD_IRIS_DETECT ��Ĥ�����Ϣ�������� */
typedef struct
{
    int                     sequence;               /* Ψһ��ʶ����ֱ�ӷ���Ӧ���·���������HOST_CMD_IRIS_DETECT��ID */
    unsigned int            eyeNum;                 /* ���е�/˫��ע��orʶ�� */
    IRIS_DETECT_TYPE        detectType;             /* ������ͣ��� IRIS_DETECT_TYPE */
    IRIS_LABEL_INFO         label;                  /* ��Ĥ����Ƿ���Ҫ������ע��Ϣ */

    unsigned char           reserve[32];            /* �����ֶ� */
} IRIS_DETECT_PRMAM;

/* STREAM_ELEMENT_IRIS_IQA ��Ĥ���ص��ṹ�� */
typedef struct
{
    int                     sequence;               /* Ψһ��ʶ����ֱ�ӷ���Ӧ���·���������HOST_CMD_IRIS_DETECT��ID */
    unsigned int            frameId;                /* ��ͼ���Ӧ��֡ID*/
    unsigned int            eyeNum;                 /* ��⵽���������� */
    unsigned int            distance;               /* ��ͼ���Ӧ�ľ�����Ϣ  */
    unsigned long long      pts;                    /* ��ͼ���Ӧ��ʱ��� */
    int                     gray[2];                /* ��ֻ�۾��ĻҶ�ֵ */
    IRIS_PIC_INFO           picInfo;                /* ͼ�����ϸ��Ϣ */
    IRIS_POINTS_INFO        points;                 /* ��Ĥ�ؼ�����Ϣ */
    IRIS_TIME_COUNT          timeCount;             /* ��ͼ���Ӧ�ĺ�ʱͳ�� */
    IRIS_LABEL_INFO         label;                  /* �ý�������ڼ�⿪���˱�ע��Ϣ����Ч */

    unsigned char           reserve[32];            /* �����ֶ� */
} IRIS_IQA_RESULT;

/* ��Ĥͼ������� */
typedef enum
{
    IRIS_STATUS_SUCCESS         = 0,            /* ����ɹ� */

    IRIS_DETECT_ERR             = 0x10,         /* ��Ĥ���ʧ�� */
    IRIS_SIM_COMPARE_ERR        = 0x20,         /* 1V1�ȶ�ʧ��*/
    IRIS_BUILD_ERR              = 0x30,         /* ��ģʧ��*/
    IRIS_PIC_DEC_ERR            = 0x40,         /* ͼƬ�·�����ʧ��*/
    IRIS_PIC_FORMAT_ERR         = 0x41,         /* ͼƬ�·���ʽ���� */
    IRIS_PIC_RES_ERR            = 0x42,         /* ͼƬ�·��ֱ��ʴ��� */
    IRIS_QUALITY_ERR            = 0x50,         /* ��Ĥͼ���������ϸ� */
    IRIS_QUALITY_BOUNDS_ERR     = 0x51,         /* ��Ĥ���� */
    IRIS_QUALITY_VISIBILITY_ERR = 0x52,         /* ��Ĥ�ɼ��ȵ�����ֵ��20��*/
    IRIS_QUALITY_PUPILRATIO_ERR = 0x53,         /* ͫ�ױ���������ֵ��70��*/
    IRIS_QUALITY_BRIGHT_ERR     = 0x54,         /* ͼ�����*/
    IRIS_QUALITY_CLARITY_ERR    = 0x55,         /* �����ȵ�����ֵ��40��*/
    IRIS_QUALITY_GAZE_ERR       = 0x56,         /* б�Ӷȸ�����ֵ��90��*/
    IRIS_QUALITY_RATIO_ERR      = 0x57,         /* ��Ĥֱ��/ͫ��ֱ����С*/
    IRIS_QUALITY_COMPOSITE_ERR  = 0x58,         /* ͼ��������Ȩ�÷ֵ�����ֵ��47��*/
    IRIS_QUALITY_BRIGHTSPOT_ERR = 0x59,			/* ��Ĥ������̫�� */
    IRIS_1VN_DISTANCE_FAR       = 0x70,         /* 1VNʶ�����̫Զ�޷���⵽�۾� */
    IRIS_1VN_DISTANCE_CLOSE     = 0x71,         /* 1VNʶ�����̫���޷���⵽�۾� */
    IRIS_LIVEDET_NOLIVE_ERR     = 0x90,         /* ������Ϊ�ǻ��� */
    IRIS_LIVEDET_NOSURE_ERR     = 0x91,         /* �������޷�ȷ�� */
} IRIS_STATUS_TYPE;

/* ��Ĥͼ���������� */
typedef struct
{
    unsigned int            textureFocus;           /* �������������� */
    unsigned int            visualDegree;           /* �۾��ź϶� */
    unsigned int            gaze;                   /* б�Ӷ� */
    unsigned int            foucs;                  /* �۽��ȣ���Ĥ����ĶԱȶ� */
    unsigned int            move;                   /* �˶�ģ�� */
    unsigned int            ratio;                  /* ��Ĥ��ͫ�׵ı��� */
    unsigned int            composite;              /* ��Ĥע��ʱ���ۺ����� */
    unsigned int            centreX;                /* ��Ĥ���� x���� */
    unsigned int            centreY;                /* ��Ĥ���� Y���� */
    unsigned int            irisDiam;               /* ��Ĥֱ�� */
    unsigned int            pupilDiam;              /* ͫ��ֱ�� */

    unsigned char           reserve[32];            /* �����ֶ� */
} IRIS_SCORE_PARAM;

/* ���ź�Ĥͼ��ע��&&1V1�ȶ���Ϣ */
typedef struct
{
    IRIS_STATUS_TYPE        statusCode;                       /* ��Ĥͼ������� */
    unsigned int            eyeFlag;                          /* ���۱�ʶ��0�����ۣ�1������*/
    unsigned int            frameId;                          /* ��ͼ���Ӧ��֡ID*/
    unsigned int            distance;                         /* ��ͼ���Ӧ�ľ�����Ϣ  */
    unsigned long long      pts;                              /* ��ͼ���Ӧ��ʱ��� */
    float                   top1Sim;                          /* top1 ���е����ƶ� */
    unsigned int            top1Id;                           /* top1 ���е�ID �� */
    unsigned char           model[IRIS_MAX_MODEL_DATA_LENTH]; /* ��Ĥģ������ */
    unsigned int            modelSize;                        /* ģ�����ݴ�С��ע�᣺256 Byte��������1536 Byte */
    IRIS_SCORE_PARAM        picParam;                         /* ��Ĥͼ���������� */
    IRIS_BUF_INFO           irisPic;                          /* ��Ĥͼ�� */
    REGION_INFO             irisPoint;                        /* ��Ĥͼ��������λ������ */
    IRIS_TIME_COUNT         timeCount;                        /* ��ͼ���Ӧ�ĺ�ʱͳ�� */
	
    unsigned char           reserve[32];                      /* �����ֶ� */
}IRIS_EYE_INFO;

/* STREAM_ELEMENT_IRIS_1VN_SIM ��Ĥ ע�� or 1VN�ȶ� �ص��ṹ�� */
typedef struct
{
    int                     sequence;               /* Ψһ��ʶ����ֱ�ӷ���Ӧ���·�ID */
    IRIS_EYE_INFO           eyeData[2];             /* ��Ĥ��Ϣ [0]:���ۣ� [1]������*/
    IRIS_BUF_INFO           facePic;                /* ����ͼ������������Ĥ����ҵ��*/
    IRIS_BUF_INFO           srcIrisPic;             /* ��Ĥ����ͼ�� */
	REGION_INFO             facePoint;              /* ������λ������ */

    unsigned char           reserve[32];            /* �����ֶ� */
}IRIS_1VN_RESULT;

/* HOST_CMD_IRIS_BM ��ĤͼƬ�·���ģ�������� */
typedef struct
{
    int                     sequence;               /* Ψһ��ʶ������Ӧ���·�ID���ص�����ֱ�ӷ��� */
    unsigned int            modelFlag;              /* 1����ȡ��Ĥ����ֵ��1536 Byte����0����ȡ��Ĥģ��ֵ��256 Byte����2��������Ҫ��˳��256+1536 */
    unsigned char          *imgAddr;                /* �·�ͼƬ��ַ */
    unsigned int            imgAize;                /* �·�ͼƬ��С */

    unsigned char           reserve[32];            /* �����ֶ� */
} IRIS_BUILD_MODEL_PRM;

/* ���ź�Ĥͼ��ģ��Ϣ */
typedef struct
{
    IRIS_STATUS_TYPE        statusCode;             /* ��Ĥͼ������� */
    unsigned int            modelSize;              /* ģ�����ݴ�С   */
    unsigned char           model[IRIS_MAX_MODEL_DATA_LENTH]; /* ��Ĥģ������ */
    IRIS_BUF_INFO           irisPic;                /* ��Ĥͼ�� */
    IRIS_SCORE_PARAM        score;                  /* ��Ĥͼ����� */

    unsigned char           reserve[32];            /* �����ֶ� */
}IRIS_BM_DATA;

/* STREAM_ELEMENT_IRIS_BM ��ĤͼƬ�·���ģ�ص��ṹ�� */
typedef struct
{
    int                     sequence;               /* Ψһ��ʶ������Ӧ���·�ID���ص�����ֱ�ӷ��� */
    IRIS_TIME_COUNT         timeCount;              /* ��ͼ���Ӧ�ĺ�ʱͳ�� */
    IRIS_BM_DATA            irisData;               /* ��Ĥͼ����Ϣ */

    unsigned char           reserve[32];            /* �����ֶ� */
}IRIS_IMG_BM_RESULT;

/* HOST_CMD_IRIS_COMPARE ��Ĥ 1V1 �ȶԲ������� */
typedef struct
{
    int                     sequence;                                 /* Ψһ��ʶ������Ӧ���·�ID���ص�����ֱ�ӷ��� */
    unsigned char           firstModel[IRIS_MAX_MODEL_DATA_LENTH];    /* ��һ��ģ������ */
    unsigned int            firstModelLen;                            /* ��һ��ģ�ͳ��� */
    unsigned char           secondModel[IRIS_MAX_MODEL_DATA_LENTH];   /* �ڶ���ģ������ */
    unsigned int            secondModelLen;                           /* �ڶ���ģ�ͳ��� */
    float                   similarity;                               /* ���ص����ƶ� */

    unsigned char           reserve[32];                              /* �����ֶ� */
} IRIS_COMPARE_PRM;

/* �������ڴ�ͬ�� */
typedef struct
{
    unsigned int   eleCnt;                          /* ��������Ԫ�صĸ��� */
    unsigned int   elePadCnt;                       /* �������ﲹ��Ԫ�صĸ��� */
    unsigned int   eleAllCnt;                       /* ������������Ԫ�صĸ��� */
    unsigned int   featLen;                         /* ��������ÿ��Ԫ���к�Ĥģ�͵ĳ��� */
    unsigned int   addInfoLen;                      /* ��������ÿ��Ԫ����Ӧ�ø��ӵĳ��� */
    unsigned int   eleSize;                         /* ��������ÿ��Ԫ�ص��ڴ��С */
    unsigned int   memSize;                         /* ������������ڴ��С */
    unsigned int   headSize;                        /* ������ͷ�Ľṹ���С */

    unsigned int   poolVersion;                     /* ������汾�ţ�Ӧ�ò���д�������ȡ */
    unsigned char* virtHead;                        /* ��������ͷ�����׵�ַ */
    unsigned char* physHead;                        /* ��������ͷ�������׵�ַ */
    unsigned char* virtPool;                        /* ��������ele�����׵�ַ */
    unsigned char* physPool;                        /* ��������ele�������׵�ַ */
    unsigned char  reserve[12];                     /* ��������ı����ֶ� */
}IRIS_MODEL_POOL_ST;


/* ģ�������� */
typedef struct
{
    void                   *virtAddr;               /* ģ����ַ  */
    void                   *poolLockFunc;           /* ģ��������� */
    void                   *poolUnlockFunc;         /* ģ���������� */
    void                   *flushCacheFunc;         /* ģ���ˢcache,д���ڴ溯�� */

    unsigned char          reserve[32];             /* �����ֶ� */
}IRIS_POOL_PRM;

typedef enum
{
    IRIS_CALIBRATE_SUCESS = 1,    /* vcm �궨�ɹ� */
    IRIS_CALIBRATE_TIMEOUT = 2,   /* vcm �궨��ʱ */
    IRIS_CALIBRATE_MACRO_FAIL = 3,    /* vcm ����궨ʧ�� */
    IRIS_CALIBRATE_INF_FAIL = 4,      /* vcm Զ��궨ʧ�� */
    IRIS_CALIBRATE_CHECK_ERROR = 5,   /* vcm ������ */
    IRIS_CALIBRATE_AE_FAIL = 6,   /* AE ������ */
    IRIS_CALIBRATE_SAVE_FAIL = 7,  /* �����ļ�ʧ�� */
} IRIS_STATUS_CALIBRATE;

/* IRIS_VCM_Calibarte_RESULT ��ĤVCM�궨�ص��ṹ�� */
typedef struct
{
    IRIS_STATUS_CALIBRATE   calibrateStatus;        /* �궨״̬�� */
    unsigned char           reserve[32];            /* �����ֶ� */
}IRIS_VCM_CALIBARTE_RESULT;

/*  HOST_CMD_IRIS_ISP_PRM ��Ĥ����ISP�������� */
typedef struct
{
    unsigned int            slopeAngle;             /* �豸����״̬ 0-��ֱ	1-��б15��       */
    unsigned char           reserve[32];            /* �����ֶ� */
} IRIS_ISP_PRMAM;


/****************************************************************************/
/*                      TOFģ��                                        */
/****************************************************************************/
/* TOF�¶� */
typedef struct
{
    unsigned int sensorTemp;                        /* TOF Sensor�¶� */
    unsigned int driveTemp;                    	    /* TOF Driver�¶� */
	
	unsigned char          reserve[32];             /* �����ֶ� */
} TOF_TEMPERATURE;

/****************************************************************************/
/*                      ΢����ģ��: �ƶ�������ڵ�����ά��                  */
/****************************************************************************/
/* �ƶ�������� */
#define MD_BLOCK_W              (32)                /* ����     */
#define MD_BLOCK_H              (32)                /* ��߶�     */
#define MD_BLOCK_CNT            (704 / MD_BLOCK_W)  /* ÿ�п���� */
#define MD_MAX_LINE_CNT         (576 / MD_BLOCK_H)  /* �������� */
#define MD_MAX_LEVEL            (6)                 /* �����ȼ������Ϊ6(���������)����СΪ0(���������) */

/* �ڵ��������� */
#define CD_MAX_LEVEL            (15)

/* �������� */
#define MASK_MAX_REGION         (4)                 /* ���֧��������� */
#define MASK_ALIGN_H            (8)                 /* W��X���� */
#define MASK_ALIGN_V            (2)                 /* H��Y���� */

/*
    �ƶ�����������Ϣ  HOST_CMD_SET_MD
*/
typedef struct
{
    unsigned int level;                             /* ����0(���������)-6(���������),Ĭ��Ϊ2 */
    unsigned int fastInterval;                      /* ���ټ��������Ĭ��Ϊ3 */
    unsigned int slowInterval;                      /* ���ټ��֡�����Ĭ��Ϊ15 */
    unsigned int delay;                             /* ��ʱ */
    unsigned int bNeedDetail;                       /* ���Ϊ�棬DSP�Ὣ�ƶ�������ϸ�ṹ�ϴ������� */
    unsigned int mask[MD_MAX_LINE_CNT];             /* ����ֵ����ӦλΪ1ʱ�жϸÿ飬ֻ����bNeedDetailΪFALSEʱ����Ч */
    unsigned int bAuto;                             /* �Ƿ�����Ӧ */
} MD_CONFIG;

/*
    �ڵ�������������Ϣ  HOST_CMD_SET_CD
*/
typedef struct
{
    unsigned int bEnable;                           /* ʹ�� */
    unsigned int level;                             /* �ڵ�����  */
    unsigned int delay;                             /* ��ʱ����ʱδʹ�� */
    unsigned int x;                                 /* �ڵ�����λ�ò��� */
    unsigned int y;
    unsigned int w;
    unsigned int h;
} CD_CONFIG;


/*
    ���ε�������Ϣ  HOST_CMD_SET_MASK
*/
typedef struct
{
    unsigned int regionCnt;                         /* ������� */
    unsigned int x[MASK_MAX_REGION];                /* ����λ�ò��� */
    unsigned int y[MASK_MAX_REGION];
    unsigned int w[MASK_MAX_REGION];
    unsigned int h[MASK_MAX_REGION];
    unsigned int flgMaskEnable;                     /* ʹ�� */
} MASK_CONFIG;

/*
    ˮӡ������Ϣ
    HOST_CMD_SET_WATERMARK
    HOST_CMD_SET_SUB_WATERMARK
*/
typedef struct
{
    unsigned int  global_time;                      /* ��ǰ��ȫ��ʱ��(����ʱ��)����Ҫ��GROUP_HEADER�е�ʱ����ͬ */
    unsigned int  device_sn;                        /* �豸�����к�(������ʾ����BCD��) */
    unsigned char mac_addr[6];                      /* ���������豸��MAC��ַ */
    unsigned char device_type;                      /* �豸������(�ͺ�) */
    unsigned char device_info;                      /* �豸�ĸ�����Ϣ(�������ԡ��汾��) */
    unsigned char channel_num;                      /* ����������ͨ���� */
    unsigned char reserve[47];                      /* �����֤��64���� */
} WATERMARK_VER1;

/*
    �ƶ���������� STREAM_ELEMENT_MD_RESULT
*/
typedef struct tagMdCbResultSt
{
    unsigned int uiMdResult[MD_MAX_LINE_CNT];
}MD_CB_RESULT_ST;


/*
    �ڵ������������ STREAM_ELEMENT_CD_RESULT
*/
typedef struct tagCdCbResultSt
{
    unsigned int            chn;
    unsigned int            cdResult;
    unsigned char           reserve[8];
}CD_CB_RESULT_ST;

/* ��ά��ɨ���������ã�ʹ��ͼ������ƫ������ HOST_CMD_QR_START  */
typedef struct tagQrRect
{
    unsigned int            roiEnable;              /* ROI ʹ�ܿ��� */
    unsigned int            x;                      /* ɨ���������� */
    unsigned int            y;
    unsigned int            width;
    unsigned int            height;
} QR_ROI_RECT;

/* TDCL_POINT_F ������ṹ�� */
typedef struct _QR_POINT_F
{
    float                   x;                      /* x���� */
    float                   y;                      /* y���� */
}QR_POINT_F;

/* QR�������Ϣ */
typedef struct _QR_CODE_INFO_
{
    unsigned char           *charater;              /* �ַ���Ϣ */
    int                     len;                    /* �ַ����� */
}QR_CODE_INFO;

/* QR�����ĵ����ꡢ��ת�ǶȽṹ�� */
typedef struct _QR_LOCATION_INFO
{
    QR_POINT_F              centerPt;               /* QR�����ĵ����� */

    QR_POINT_F              start_pt;               /* ���ֱ����ʼ�� */
    QR_POINT_F              end_pt;                 /* ���ֱ����ֹ�� */
    float                   rotateAngle;            /* QR��ͼ�����ת�Ƕ� */
}QR_LOCATION_INFO;

/* ��ά���ⷵ�ؽ�� STREAM_ELEMENT_QRCODE */
typedef struct tagQrCodeCbResultSt
{
    QR_CODE_INFO            codeInfo;               /* ������Ϣ */
    QR_LOCATION_INFO        locInfo;                /* QR��λ�ýǶ���Ϣ */
}QR_CODE_RESULT;

/****************************************************************************/
/*                                 өʯ�� ����                                    */
/****************************************************************************/
#define CIPHER_KEY_SIZE   (32)

/** Cipher ����ģʽ */
typedef enum CipherAlgWorkModeE
{
    DSP_CIPHER_WORK_MODE_UNKNOW = 0x0,
    DSP_CIPHER_WORK_MODE_ECB    = 0x1,        /* ECB ģʽ*/
    DSP_CIPHER_WORK_MODE_MAX
}CIPHER_ALG_WORK_MODE_E;

/* cipher ����λ�� */
typedef enum CipherAlgBitWidthE
{
    DSP_CIPHER_BIT_WIDTH_UNKNOW = 0x0,
    DSP_CIPHER_BIT_WIDTH_128BIT = 0x10,  /* ��� 128 λ */
    DSP_CIPHER_BIT_WIDTH_194BIT = 0x18,  /*        194 λ */
    DSP_CIPHER_BIT_WIDTH_256BIT = 0x20,  /*        256 λ */
}CIPHER_ALG_BIT_WIDTH_E;

/* cipher �������� */
typedef enum CipherAlgConfigType
{
    DSP_CIPHER_UNKNOW      = 0x0,
    DSP_CIPHER_ENCRYPT     = 0x10,  /*  �������� */
    DSP_CIPHER_DECRYPT     = 0x18,  /*  �������� */
    DSP_CIPHER_TRANS_PACK  = 0x20,  /*  ת���ɼ������� */
}CIPHER_ALG_CONFIG_TYPE;


/* 
	���ü������� HOST_CMD_AES_CONFIG 

	����ͨ��ʱ��ʹ�� SendCmdToDsp �е� unsigned int chan ����;
	��Ӧ DSP_ENC_EZVIZ_CRYP_INFO �е� encDevChan��
	��Ӧ DSP_DEC_EZVIZ_CRYP_INFO �е� chan��
*/
typedef struct tagCipherAlgConfigInfoSt
{
    CIPHER_ALG_WORK_MODE_E  workMode;               /* ����ģʽ */
    CIPHER_ALG_BIT_WIDTH_E  bitWidth;               /* λ��     */
    char                    key[CIPHER_KEY_SIZE];   /* ��Կ     */
	unsigned char  reserve[32];		/* ���� */
}CIPHER_ALG_CONFIG_INFO;

/*  
    PS ������ HOST_CMD_AES_PS_PROC
    RTP������ HOST_CMD_AES_RTP_PROC 
*/
typedef struct tagCipherAlgEncryptInfoSt
{
    unsigned int enable;         /* ���ܿ��� */
	unsigned char  reserve[32];		/* ���� */
}CIPHER_ALG_ENCRYPT_INFO;

/****************************************************************************/
/*                                  ˫���ں�                                */
/****************************************************************************/

/* ˫���ں�ҵ�� ���� */
typedef enum eEifJobType
{
    EIF_NONE_JOB     = 0,              /* ˫���ں� δָ������   ���� */
    EIF_ENCODER_JOB  = 0xaa000000,     /* ˫���ں����� �������� ���� */
    EIF_DISPLAY_JOB  = 0xbb000000,     /* ˫���ں����� ��ʾ���� ���� */        
    EIF_ALL_JOB      = 0xFFFFFFFF      /* ˫���ں����� �������� */
}EIF_JOB_TYPE;

/*  HOST_CMD_CHG_ISP_CFG  ���� �궨����������� */
typedef struct tagIspCilbratePrmSt
{
    unsigned int    ispcalibrateEnable;      /* ����690/691�궨ʱISP �������ļ� */
    unsigned char   reserve[64];
}ISP_CILBRATE_PRM;

/* HOST_CMD_EIF_SET_PARAM  ���� ˫���ں� ���� */
typedef struct tagEifPrmSt
{
    unsigned int    eifEnable;      /* ����˫���ںϹ��� 0:�ر�  1:���� */
    EIF_JOB_TYPE    eJobType;       /* ָ������������                  */
    unsigned char   reserve[64];
}EIF_PRM;

/****************************************************************************/
/*                                  ϵͳ                                    */
/****************************************************************************/

/* DSP����İ汾��Ϣ */
typedef struct tagDspSysVersion
{
    int             mainVersion;                    /* DSP��ӿڵ����汾 */
    int             subVersion;                     /* DSP��ӿڵ��Ӱ汾 */
    int             svnVersion;                     /* DSP�����Ĵ���svn�汾�� */
    char            buildData[32];                  /* DSP������������ */
    char            buildTime[32];                  /* DSP��������ʱ�� */
    char            platForm[32];                   /* DSP�����ƽ̨ */
    char            comVersion[32];                 /* DSP������Ӧ����汾�� */
    unsigned char   reserve[256];
}DSP_SYS_VERSION;

typedef struct tagDspPrmAddr
{
    unsigned char  *appVAddr;                       /* app���ʵ������ַ */
    unsigned char  *dspVAddr;                       /* dsp���ʵ������ַ */
} DSP_PRM_ADDR;

typedef struct tagDspRecPool
{
    REC_POOL_INFO AudRecPool[MAX_VENC_CHAN];        /* ����¼��buf���� */
    REC_POOL_INFO ipcRecPool[MAX_VDEC_CHAN];        /* ת�������������� */
} DSP_REC_POOL;

/*
	өʯ�Ƽ��ܹ���buff����---�ɼ�����
	DSP�ڿ���өʯ�Ƽ���ʱ�����ڴ棬�ر�ʱ���ͷţ��ظ�����ʱ���ض�Ӧͨ���ĵ�ַ
*/
typedef struct tagDspEncEzvizCrypInfo
{
	unsigned int encDevChan;                      	/* �����豸�ţ� 0->�ɼ��� 1->����� */

	unsigned int streamId;                      	/* �����豸ͨ���� 0->������ 1->������ 2->������ */
	
	NET_POOL_INFO  cryptNetPool;  					/* �����������buf���� */
	NET_POOL_INFO *cryptNetPoolShmAddr;			    /* �����������buf����,��ʹ����˫����ͨ�� */

	unsigned char reserve[60];						/* ���� */
} DSP_ENC_EZVIZ_CRYP_INFO;


/*
	өʯ�Ƽ��ܹ���buff����---�������
	DSP�ڿ���өʯ�Ƽ���ʱ�����ڴ棬�ر�ʱ���ͷţ��ظ�����ʱ���ض�Ӧͨ���ĵ�ַ
*/
typedef struct tagDspDecEzvizCrypInfo
{
    unsigned int chan;                      		/* ��ʼ�����ܱ���ͨ����� */

    unsigned int decDataType;                      	/* 0->rtp���ݣ�1->ps���� */
	
    DEC_SHARE_BUF decSharePool;       				/* ���������ݹ�������(PS/RTP) */

	NET_POOL_INFO  cryptNetPool;  					/* �����������buf���� */
	
	DEC_SHARE_BUF *decSharePoolShmAddr;                /* ���������ݹ�������(PS/RTP),��ʹ����˫����ͨ�� */
    NET_POOL_INFO *cryptNetPoolShmAddr;                /* �����������buf����,��ʹ����˫����ͨ��*/

	unsigned char reserve[56];						/* ���� */
} DSP_DEC_EZVIZ_CRYP_INFO;


/* Ӧ����DSPģ�鹲����Ϣ�ṹ�� */
typedef struct tagDspInitParaSt
{
    /***********************************************************************/
    /*                    �豸����������Ϣ                                 */
    /***********************************************************************/
    unsigned int viType;                            /* ��Ƶ�������� */
    unsigned int aiType;                            /* ��Ƶ����ģʽ */
    unsigned int machineType;                       /* �����ͺ�(�Խ���������������Ƶ������Ƶ)
                                                                    ��ʱ������������Ҫ�øò�����������Ƶ��
                                                                    aiType������
                                                                    */
    unsigned int boardType;                         /* �豸����   */
    unsigned int sn;                                /* �豸���к� */

    /***********************************************************************/
    /*                    DSPģ��ͨ����������Ϣ                            */
    /***********************************************************************/
    unsigned int encChanCnt;                        /* ͬʱ֧�ֱ���ͨ������ */
    unsigned int decChanCnt;                        /* ͬʱ֧�ֽ���ͨ������ */
    unsigned int ipcChanCnt;                        /* ͬʱ֧��ת��ͨ������ */
    unsigned int dispChanCnt;                       /* ��Ƶ����豸����     */

    /* ��Ƶ�ɼ��������� MAX_CAPT_CHAN */
    VI_INIT_INFO_ST stViInitInfoSt;                 /* �ɼ�ģ���ʼ������ */

    /* ��Ƶ����������� MAX_DISP_CHAN */
    VO_INIT_INFO_ST    stVoInitInfoSt;              /* ��ʾ���ģ���ʼ������ */

    AUDIO_INIT_INFO_ST stAudioInfoSt;               /* ��Ƶģ���ʼ������     */

    /***********************************************************************/
    /*                  Ӧ���·���Ϣ������ʵ�ֶ�Ӧ����                     */
    /***********************************************************************/
    /* �ɼ�����Ƶ�ź�ʱ����ʹ�õ�����Ƶ�ź�ͼ����Ϣ */
    CAPT_NOSIGNAL_INFO_ST stCaptNoSignalInfo;
    FONT_LIB_INFO_ST      stFontLibInfo;            /* OSD������ʹ���ֿ����Ϣ */
    STREAM_INFO_ST        stStreamInfo;             /* ��������������Ϣ      */
    AUDIO_TB_BUF_INFO_ST  stAudioTBBufInfo;         /* �����Խ����ܻ�������Ϣ */

    /***********************************************************************/
    /*                      ��������������Ϣ                             */
    /***********************************************************************/
    REC_POOL_INFO RecPoolMain[MAX_VENC_CHAN]; /* ����¼��������buf���� */
    REC_POOL_INFO RecPoolSub[MAX_VENC_CHAN];  /* ����¼��������buf���� */
    REC_POOL_INFO RecPoolThr[MAX_VENC_CHAN];  /* ����¼���������buf���� */
    
    NET_POOL_INFO NetPoolMain[MAX_VENC_CHAN]; /* ��������������buf���� */
    NET_POOL_INFO NetPoolSub[MAX_VENC_CHAN];  /* ��������������buf���� */
    NET_POOL_INFO NetPoolThr[MAX_VENC_CHAN];  /* ���������������buf���� */

    DEC_SHARE_BUF decShareBuf[MAX_VDEC_CHAN];       /* ����������������(PS/RTP) */
    DEC_SHARE_BUF ipcShareBuf[MAX_VDEC_CHAN];       /* ת�������������� */

    /***********************************************************************/
    /*                      DSPģ��״̬��Ϣ                                */
    /***********************************************************************/
    /* ģ��״̬��Ϣ��Ӧ�ð����ȡ��Ӧ�÷����ڴ� */
    SYS_STATUS  stSysStatus;                        /* ϵͳ״̬ */
    CAPT_STATUS CaptStatus[MAX_CAPT_CHAN];          /* �ɼ� */
    ENC_STATUS  VencStatus[MAX_VENC_CHAN];          /* ���� */
    DEC_STATUS  VdecStatus[MAX_VDEC_CHAN];          /* ���� */
    DISP_STATUS DispStatus[MAX_DISP_CHAN];          /* ��ʾ */
    IPC_STATUS  VipcStatus[MAX_VIPC_CHAN];          /* Զ�̽��� */

    /***����������,���ŵ��ṹ��󲿷֣�reservedǰ��***/
    unsigned int modleCnt;                   /* ���������������*/
    
    /* ���N��ģ������: ֧�ַֿ�����ÿ��������������������Ч��ʼ����פ�ڴ档(ע����ʱ�� multiModleCnt[0] = modleCnt)  */
    unsigned int enableMultiModle;          /* ���ö���modleCnt������Ч��ʹ��multiModleCnt�ĸ���ֵ */
    unsigned int multiModleCnt[DSP_FR_FACE_FEATRE_CP_ATTR_MAX];

    /***��Ĥģ�������***/
    unsigned int irisDbCnt;                         /*  ��Ĥģ������  */

    DSP_PRM_ADDR dspRecPool;                        /* ¼�񻺳��� */
	
	unsigned int  broadcastChanCnt;                  		   /* �㲥ͨ������     */
    DEC_SHARE_BUF broadcastShareBuf[MAX_BROADCAST_CHAN];       /* ʵʱ�㲥�������� */
	
    /***********************************************************************/
    /*                      DSP������չ������                              */
    /***********************************************************************/
    /*
        �˴�������ַ���飬���ڱ����Ӧ���ܵĽṹ��ĵ�ַ������Ҫ���ݹ��ܶ���
        ��صĽṹ�壬����Ӧ��Ϊ������ڴ棬����DSP����ʵ�ֹ��������
     */
    unsigned char reserve[868];

} DSPINITPARA;

/****************************************************************************/
/*                                 ������                                   */
/****************************************************************************/

/*
    Ӧ����DSP���������	
*/
typedef enum
{
    HOST_CMD_NON                        = 0x7FFFFFFF,       /* ��ʼ ��Чֵ */
    /* ��ʼ��DSPģ�� */
    HOST_CMD_MODULE_SYS                 = 0x80000000,       /* ϵͳģ��������� */
    HOST_CMD_DSP_INIT                   = 0x80000001,       /* ��ʼ��DSP�� */
    HOST_CMD_DSP_DEINIT                 = 0x80000002,       /* ȥ��ʼ��DSP�� */
    HOST_CMD_DSP_GET_STATUS             = 0x80000003,
    HOST_CMD_DSP_GET_MEM                = 0x80000004,       /* ��ȡ�ڴ�ӿڣ����ڲ�Ʒ����*/
    HOST_CMD_DSP_FREE_MEM               = 0x80000005,       /* �ͷŶ�Ӧ���ڴ�*/
    HOST_CMD_DSP_GET_SYS_VERISION       = 0x80000006,       /* ��ȡDSP����İ汾��Ϣ */
    HOST_CMD_DSP_DUMP_STATUS            = 0x80000007,       /* ���̹���ʱ�·�������DSP�ڲ���ģ��״̬��Ϣ */
    HOST_CMD_DSP_HDAL_MEM_ALLOC         = 0x80000008,       /* �����豸���ڴ�(�����ڴ�������ڴ�) ��HOST_CMD_DSP_HDAL_MEM_FREE�ɶ� ��AI3ƽ̨֧��;���ýṹ�� MEM_INFO;������HOST_CMD_DSP_INIT���ú����ʹ�� */
    HOST_CMD_DSP_HDAL_MEM_FREE         	= 0x80000009,       /* �ͷ��豸���ڴ�(�����ڴ�������ڴ�) ��HOST_CMD_DSP_HDAL_MEM_ALLOC�ɶ� ��AI3ƽ̨֧��;���ýṹ�� MEM_INFO              ;������HOST_CMD_DSP_INIT���ú����ʹ��*/

    /* �ɼ� */
    HOST_CMD_MODULE_VI                  = 0x80010000,       /* �ɼ�ģ��������� */
    HOST_CMD_START_VIDEO_INPUT          = 0x80010001,       /* ������Ƶ�ɼ� */
    HOST_CMD_STOP_VIDEO_INPUT           = 0x80010002,       /* ֹͣ��Ƶ�ɼ� */
    HOST_CMD_SET_VIDEO_PARM             = 0x80010003,       /* ������Ƶ�ɼ����� */
    HOST_CMD_SET_VIDEO_ROTATE           = 0x80010004,       /* ������Ƶ��ת���� */
    HOST_CMD_SET_VIDEO_MIRROR           = 0x80010005,       /* ������Ƶ�������� */

    HOST_CMD_SET_VIDEO_WDR              = 0x80010006,       /* ������Ƶ��̬   */
    HOST_CMD_SET_VIDEO_NOSIGNAL         = 0x80010007,       /* ������Ƶ���ź�ͼƬ */
    HOST_CMD_SET_VIDEO_CALIBRATION      = 0x80010008,       /* ��������ͷ�궨ϵ��*/
    HOST_CMD_SET_VIDEO_INSTALL          = 0x80010009,       /* ��������ͷ��װ�ĽǶȾ������Ϣ�����޸�ͨ��*/

    HOST_CMD_START_VIDEO_CB             = 0x8001000a,       /* ������Ƶ�ɼ��ص����� */
    HOST_CMD_STOP_VIDEO_CB              = 0x8001000b,       /* ֹͣ��Ƶ�ɼ��ص����� */
    HOST_CMD_SET_VIDEO_CENTER_OFFSET    = 0x8001000c,       /* ������Ƶ���ĵ�ƫ�Ʋ��� */

    HOST_CMD_START_USB_INPUT            = 0x80011001,       /* ����USB��Ƶ�ɼ�����,�ݲ���Ҫ��������������չ�Ļ������ */
    HOST_CMD_STOP_USB_INPUT             = 0x80011002,       /* ֹͣUSB��Ƶ�ɼ�����,�ݲ���Ҫ��������������չ�Ļ������ */


    /* ���� */
    HOST_CMD_MODULE_VENC                = 0x80020000,       /* ����ģ��������� */
    HOST_CMD_START_ENCODE               = 0x80020001,       /* �������빦�� */
    HOST_CMD_STOP_ENCODE                = 0x80020002,       /* ֹͣ���빦�� */
    HOST_CMD_INSERT_I_FRAME             = 0x80020003,       /* ǿ�Ʊ���I֡  */
    HOST_CMD_SET_ENCODER_PARAM          = 0x80020004,       /* ���ñ��������� */
    HOST_CMD_INSERT_USER_PIC_PARAM      = 0x80020005,       /* ���������뵥���û�ͼƬ���� */
    HOST_CMD_SET_ENCODE_PRIV            = 0x80020006,       /* ���ñ����˽������ */
    HOST_CMD_START_MERGE_PIC_ENC        = 0x80020007,       /* ��ʼ�ں�ͼƬ���� */
    HOST_CMD_STOP_MERGE_PIC_ENC         = 0x80020008,       /* �����ں�ͼƬ���� */
	HOST_CMD_ENC_IRIS_START				= 0x80020009,		/* ������Ĥͼ�����       ��ʹ�ýṹ�� IRIS_SHARE_BUFF_INFO */ 
	HOST_CMD_ENC_IRIS_STOP				= 0x8002000a, 	   	/* ֹͣ��Ĥͼ����룬ʹ�ýṹ�� IRIS_SHARE_BUFF_INFO */ 
	HOST_CMD_ENC_USB_START				= 0x8002000b,		/* ����USBͼ�����       ��ʹ�ýṹ�� USB_SHARE_BUFF_INFO */ 
	HOST_CMD_ENC_USB_STOP				= 0x8002000c, 	   	/* ֹͣUSBͼ����� ��ʹ�ýṹ�� USB_SHARE_BUFF_INFO */ 

    HOST_CMD_START_SUB_ENCODE           = 0x80021001,       /* ��ʼ���������빦�� */
    HOST_CMD_STOP_SUB_ENCODE            = 0x80021002,       /* ֹͣ���������빦�� */
    HOST_CMD_INSERT_SUB_I_FRAME         = 0x80021003,       /* ǿ������������I֡  */
    HOST_CMD_SET_SUB_ENCODER_PARAM      = 0x80021004,       /* �������������������� */
    HOST_CMD_SUB_INSERT_USER_PIC_PARAM  = 0x80021005,       /* ���������뵥���û�ͼƬ���� */
    HOST_CMD_SET_SUB_ENCODE_PRIV        = 0x80021006,       /* ���ñ����˽������ */

    HOST_CMD_START_THIRD_ENCODE         = 0x80022001,       /* ��ʼ����ͨ���������빦�� */
    HOST_CMD_STOP_THIRD_ENCODE          = 0x80022002,       /* ֹͣ����ͨ���������빦�� */
    HOST_CMD_INSERT_THIRD_I_FRAME       = 0x80022003,       /* ǿ�Ƶ���ͨ����������I֡  */
    HOST_CMD_SET_THIRD_ENCODE_PARAM     = 0x80022004,       /* ���õ���ͨ���������������� */
    HOST_CMD_SET_THIRD_ENCODE_PRIV      = 0x80022006,       /* ���ñ��������� */

    HOST_CMD_SET_ENC_REGION             = 0x8002300d,       /* ���ñ���ROI */
    HOST_CMD_PIC_SHOW_TEMP              = 0x8002300e,       /* ���ȶ�ͼƬ�ϵ��¶Ⱥ�������*/

    /* ץͼ */
    HOST_CMD_START_ENC_JPEG             = 0x80020100,       /* ��������ץͼ���� */
    HOST_CMD_STOP_ENC_JPEG              = 0x80020101,       /* ֹͣ����ץͼ���� */
    HOST_CMD_SET_ENC_JPEG_PARAM         = 0x80020102,       /* ���ñ���ץͼ���� */

    /* ���� */
    HOST_CMD_MODULE_DEC                 = 0x80030000,       /* ����ģ��������� */
    HOST_CMD_DEC_START                  = 0x80030001,       /* �������빦�� */
    HOST_CMD_DEC_STOP                   = 0x80030002,       /* ֹͣ���빦�� */
    HOST_CMD_DEC_RESET                  = 0x80030003,       /* ����������   */
    HOST_CMD_DEC_NEXT_FRM               = 0x80030004,       /* ���뵥֡��ʾ */
    HOST_CMD_DEC_GET_IMG                = 0x80030005,       /* ���ý���ץͼ���� */
    HOST_CMD_DEC_TRANS_PACK             = 0x80030006,       /* ����ת����Զ��¼�� */
    HOST_CMD_DEC_ATTRI                  = 0x80030007,       /* ͼƬ��ʽ�ֱ��ʽ��� */
    HOST_CMD_DEC_JPG                    = 0x80030008,       /* ��JPGͼƬ���ݽ��н���*/
    HOST_CMD_DEC_TRANS_TO_ENC           = 0x80030009,       /* ����ת�������뻺����������¼�� */
    HOST_CMD_DEC_PAUSE                  = 0x8003000a,       /* ��ͣ���빦��       */
    HOST_CMD_DEC_SYNC_REC               = 0x8003000b,       /* ����ͨ��¼���뱾��¼���ʱ���ͬ�� */
	HOST_CMD_DEC_STREAMTYPE             = 0x8003000c,       /* ���ƽ�������������������,ʹ�ýṹ�� DECODER_STREAMTYPE  */

    /* ��ʾ */
    HOST_CMD_MODULE_DISP                = 0x80040000,        /* ��ʾģ��������� */
    HOST_CMD_ALLOC_DISP_REGION          = 0x80040001,        /* ������ʾ�������� */
    HOST_CMD_DISP_CLEAR                 = 0x80040002,        /* �����ʾ�������� */
    HOST_CMD_SET_OUTPUT_REGION          = 0x80040003,        /* ������ʾͨ����������� */
    HOST_CMD_SET_DISP_ROTATE            = 0x80040004,        /* ������Ƶ��ʾ��ת���� */
    HOST_CMD_SET_DISP_MIRROR            = 0x80040005,        /* ������Ƶ��������     */

    HOST_CMD_SET_VI_DISP                = 0x80040006,        /* ���òɼ�Ԥ������ */
    HOST_CMD_SET_DEC_DISP               = 0x80040007,        /* ���ý���Ԥ������ */

    HOST_CMD_SET_VO_STYLE               = 0x80040008,        /* ������ʾ������ */
    HOST_CMD_SET_VO_CSC                 = 0x80040009,        /* ������ʾ���Ч�� */
    HOST_CMD_DISP_FB_MMAP               = 0x8004000a,        /* UI���ڴ�ӳ��   */
    HOST_CMD_DISP_FB_UMAP               = 0x8004000b,        /* UI���ڴ�ȥӳ�� */
    HOST_CMD_DISP_FB_SHOW               = 0x8004000c,        /* ˢ��UI��ʾ     */
    HOST_CMD_DISP_FB_SNAP               = 0x8004000d,        /* UIץͼ   */
    HOST_CMD_SET_VO_STANDARD            = 0x8004000e,        /* �޸�CVBS�����ʽ */
    HOST_CMD_DISP_INSERT_PIC_PARAM      = 0x8004000f,        /* ��ʾģ����뵥���û�ͼƬ��ʾ */
    HOST_CMD_DISP_STANDBY               = 0x80040010,        /* ��ʾģ��Ϣ����ʼ������ֹ���� DISP_STANDBY_PRM*/
    HOST_CMD_SET_DISP_PTZ               = 0x80040011,        /* ������ʾԤ����̨����*/
    HOST_CMD_DISP_FBE_PARAM             = 0x80040012,        /*��ʾǿ�����ܣ��������չ���*/
    HOST_CMD_DISP_THERMO                = 0x80040013,        /* ��ʾ�ȳ���ͼƬDISP_THERMO_PRM*/
	HOST_CMD_DISP_IRIS					= 0x80040014,        /* ��ʾ��Ĥͼ�� IRIS_DISP_PRM,�ݲ�֧��*/
	HOST_CMD_DISP_USB					= 0x80040015,        /* ��ʾUSBͼ�� USB_DISP_PRM*/
    HOST_CMD_SET_DISP_FACE_PIC          = 0x80040016,        /* ����������ʾͼƬ*/
	
    /* ��Ƶ */
    HOST_CMD_MODULE_AUDIO               = 0x80050000,       /* ��Ƶģ��������� */
    HOST_CMD_SET_AUDIO_LOOPBACK         = 0x80050001,       /* ������Ƶ�ɼ��ػ� */
    HOST_CMD_SET_DEC_PLAY               = 0x80050002,       /* ������Ƶ���벥�� */
    HOST_CMD_SET_TALKBACK               = 0x80050003,       /* ���������Խ��������� */
    HOST_CMD_START_AUDIO_PLAY           = 0x80050004,       /* ������������ */
    HOST_CMD_STOP_AUDIO_PLAY            = 0x80050005,       /* ֹͣ�������� */
    HOST_CMD_START_AUTO_ANSWER          = 0x80050006,       /* �����Զ�Ӧ���� */
    HOST_CMD_STOP_AUTO_ANSWER           = 0x80050007,       /* ֹͣ�Զ�Ӧ���� */
    HOST_CMD_START_AUDIO_RECORD         = 0x80050008,       /* ��������¼�ƹ��� */
    HOST_CMD_STOP_AUDIO_RECORD          = 0x80050009,       /* ֹͣ����¼�ƹ��� */
    HOST_CMD_START_AUDIO_OUTPUT         = 0x8005000a,       /* ����������� */
    HOST_CMD_STOP_AUDIO_OUTPUT          = 0x8005000b,       /* ֹͣ������� */
    HOST_CMD_START_AUDIO_INPUT          = 0x8005000c,       /* ������������ */
    HOST_CMD_STOP_AUDIO_INPUT           = 0x8005000d,       /* ֹͣ�������� */
    HOST_CMD_SET_AUDIO_ALARM_LEVEL      = 0x8005000e,       /* �����������ޱ�����ֵ */
    HOST_CMD_SET_AI_VOLUME              = 0x8005000f,       /* ������Ƶ�������� */
    HOST_CMD_SET_AO_VOLUME              = 0x80050010,       /* ������Ƶ������� */

    HOST_CMD_SET_EAR_PIECE              = 0x80050011,       /* ������Ͳ�����л� */
    HOST_CMD_START_SOUND_RECORD         = 0x80050012,       /* ������Ƶ����¼�� */
    HOST_CMD_STOP_SOUND_RECORD          = 0x80050013,       /* ֹͣ��Ƶ����¼�� */
    HOST_CMD_SET_MANAGE_TALKTO_RESIDENT = 0x80050014,       /* ���������豸��ģ�����ڻ��Խ����ܣ����߱���MIC1��SPK2�Խ� */
    HOST_CMD_SET_DOOR_TALKTO_RESIDENT   = 0x80050015,       /* �ſڻ���ģ�����ڻ��Խ����ܣ����߱���MIC1��SPK2�Խ� */
    HOST_CMD_SET_AUDIO_ENC_TYPE         = 0x80050016,       /* ������Ƶ�������� */

    HOST_CMD_SET_AI_VOLUME_EX           = 0x80050017,       /* ������Ƶ�������� ��չ���� */
    HOST_CMD_SET_AO_VOLUME_EX           = 0x80050018,       /* ������Ƶ������� ��չ����*/

    HOST_CMD_SET_TTS_PARAM              = 0x80050019,       /* ���� TTS ����  */

    HOST_CMD_START_SPEECH_RECOGNITION     = 0x8005001a,     /* ��������ʶ�� */
    HOST_CMD_SET_SPEECH_RECOGNITION_PARAM = 0x8005001b,     /* ������ز��� */
    HOST_CMD_STOP_SPEECH_RECOGNITION      = 0x8005001c,     /* �ر�����ʶ�� */

    
    HOST_CMD_START_DVPR_REGISTER        = 0x80050020,     /* ��������ע�� */
    HOST_CMD_STOP_DVPR_REGISTER         = 0x80050021,     /* �ر�����ע�� */
	HOST_CMD_START_DVPR_RECOGNITION     = 0x80050022,     /* ��������ʶ�� */
    HOST_CMD_SET_DVPR_PRM               = 0x80050023,     /* ��������ʶ����ز��� DVPR_PARAM*/
    HOST_CMD_STOP_DVPR_RECOGNITION      = 0x80050024,     /* �ر�����ʶ�� */
    HOST_CMD_DVPR_MODEL_COMPARE         = 0x80050025,     /* ����ģ�ͱȶ�1V1 DVPR_1V1_COMPARE*/
    HOST_CMD_SET_DVPR_KEYWORD           = 0x80050026,       /* ������������ DVPR_KEYWORD_PARAM*/
    HOST_CMD_IMPORT_DVPR_DATA_REGISTER  = 0x80050027,       /* ��������ʶ����Ч���ݽ���ע�� DVPR_IMPORT_DATA_REG_ST*/
    HOST_CMD_DVPR_MODEL_CHECK           = 0x80050028,       /* ����ģ��У�� DVPR_MODEL_CHECK_ST*/
    HOST_CMD_DVPR_GET_VERSION           = 0x80050029,       /* ��ȡ�����㷨�汾 DVPR_VERSION*/
    HOST_CMD_SET_DVPR_REGIST_REST       = 0x8005002a,       /* ע������и�λ�����´ӵ�һ�俪ʼ */
    HOST_CMD_SET_DVPR_RECORD_AUDIO      = 0x8005002b,     /* ����¼��������Ƶ�������ã�����¼��ʱ����APP�·������ں������й����Ʋ��� DVPR_RECORD_AUDIO_PARAM */
    HOST_CMD_PASS_DVPR_AUDIO            = 0x8005002c,     /* ���й����Ʋ��ԣ������·�һ����Ƶ+��Ӧ�������� DVPR_PASS_AUDIO_ST */
    HOST_CMD_DVPR_GET_SIM_THRESHOLD     = 0x8005002d,        /* ��ȡ���Ʊȶ����ƶ��Ƽ���ֵ DVPR_SIM_THRESHOLD_PRM*/

    HOST_CMD_SET_AO_DEV                 = 0x8005001d,       /* ������Ƶ����豸 */
    HOST_CMD_SET_AI_DEV                 = 0x8005001e,       /* ������Ƶ�����豸 */

    HOST_CMD_CONTROL_AUDIO_PLAY         = 0x8005001f,       /* ����������������ͣ�ͻָ� */

    HOST_CMD_START_AUDIO_MIX            = 0x80050101,     /* ������������ */
    HOST_CMD_STOP_AUDIO_MIX             = 0x80050102,     /* ֹͣ�������� */

    HOST_CMD_START_AUDIO_BROADCAST      = 0x80050103,       /* ������Ƶ�㲥 */
    HOST_CMD_STOP_AUDIO_BROADCAST       = 0x80050104,       /* �ر���Ƶ�㲥 */

    HOST_CMD_SET_AMER_PARAM             = 0x80050105,       /* ���û������� */
    /* OSD */
    HOST_CMD_MODULE_OSD                 = 0x80060000,       /* OSDģ��������� */
    HOST_CMD_SET_ENC_OSD                = 0x80060001,       /* ���ñ���ͨ��OSD���� */
    HOST_CMD_SET_ENC_DEFAULT_OSD        = 0x80060002,       /* ���ñ���ͨ��Ĭ��OSD���� */
    HOST_CMD_START_ENC_OSD              = 0x80060003,       /* ��������ͨ��OSD���� */
    HOST_CMD_STOP_ENC_OSD               = 0x80060004,       /* ֹͣ����ͨ��OSD���� */
    
    HOST_CMD_SET_JPEG_OSD               = 0x80060005,       /* ����ץͼͨ��OSD���� */
    HOST_CMD_SET_JPEG_DEFAULT_OSD       = 0x80060006,       /* ����ץͼͨ��Ĭ��OSD���� */
    HOST_CMD_START_JPEG_OSD             = 0x80060007,       /* ����ץͼͨ��OSD���� */
    HOST_CMD_STOP_JPEG_OSD              = 0x80060008,       /* ֹͣץͼͨ��OSD���� */

    HOST_CMD_SET_FR_JPEG_OSD            = 0x80060009,       /* ��������ץͼͨ��OSD���� */
    HOST_CMD_SET_FR_JPEG_DEFAULT_OSD    = 0x80060010,       /* ��������ץͼͨ��Ĭ��OSD���� */
    HOST_CMD_START_FR_JPEG_OSD          = 0x80060011,       /* ��������ץͼͨ��OSD���� */
    HOST_CMD_STOP_FR_JPEG_OSD           = 0x80060012,       /* ֹͣ����ץͼͨ��OSD���� */

    HOST_CMD_SET_DISP_OSD               = 0x80060100,       /* ������ʾͨ��OSD���� */
    HOST_CMD_START_DISP_OSD             = 0x80060101,       /* ������ʾͨ��OSD���� */
    HOST_CMD_STOP_DISP_OSD              = 0x80060102,       /* ֹͣ��ʾͨ��OSD���� */

    HOST_CMD_SET_ENC_LOGO               = 0x80060200,       /* ���ñ���ͨ��LOGO���� */
    HOST_CMD_START_ENC_LOGO             = 0x80060201,       /* ��������ͨ��LOGO���� */
    HOST_CMD_STOP_ENC_LOGO              = 0x80060202,       /* ֹͣ����ͨ��LOGO���� */

    HOST_CMD_SET_DISP_LOGO              = 0x80060300,       /* ������ʾͨ��LOGO���� */
    HOST_CMD_START_DISP_LOGO            = 0x80060301,       /* ������ʾͨ��LOGO���� */
    HOST_CMD_STOP_DISP_LOGO             = 0x80060302,       /* ֹͣ��ʾͨ��LOGO���� */

    HOST_CMD_UPDATE_FONT_LIB            = 0x80060400,       /* ����OSD �ֿ� */

    /* ISP */
    HOST_CMD_MODULE_ISP                 = 0x80070000,       /* ISPģ��������� */
    HOST_CMD_LIGHTCTR                   = 0x80070001,
    HOST_CMD_SET_ISP_PARAM              = 0x80070002,       /* ����ISP���� */
    HOST_CMD_GET_ISP_PARAM              = 0x80070003,       /* ��ȡISP���� */
    HOST_CMD_SET_EXPOSURE               = 0x80070004,
    HOST_CMD_CHG_ISP_CFG                = 0x80070005,       /*�޸�ISP�������ļ���Ŀǰ690/691��Ŀר�ã�ʹ�ýṹ��  ISP_CILBRATE_PRM */
    
    HOST_CMD_SENSOR_START               = 0x80070100,       /* ���� sensor ��� */
    HOST_CMD_SENSOR_STOP                = 0x80070101,       /* ֹͣ sensor ��� */

    /* ����ģ�� ����ʶ�� */
    HOST_CMD_MODULE_FR                  = 0x80080000,       /* ����ģ��������� */
    HOST_CMD_INIT_FR                    = 0x80080001,       /* ��ʼ������ģ�� */
    HOST_CMD_FFD_CTRL                   = 0x80080002,       /* ������⹦�ܿ������� */
    HOST_CMD_FR_BM                      = 0x80080003,       /* ������ģ���� */
    HOST_CMD_FR_DET                     = 0x80080007,       /* ������������ */
    HOST_CMD_GET_FR_VER                 = 0x80080008,       /* ��ȡ�㷨�汾�� */
    HOST_CMD_FR_COMPARE                 = 0x80080009,       /* 1v1  */
    HOST_CMD_FR_POOL_PRM                = 0x8008000b,       /* ��ȡ�׿��ַ���������ڴ��ˢcache����*/
    HOST_CMD_FR_JPGSCALE                = 0x8008000d,       /* ͼƬ���ţ�֧��jpg��png�� ʹ�ýṹ��   :FR_BM_JPG_SCALE_PRM_ST       */
    HOST_CMD_FR_FACE_CROP               = 0x8008000c,       /* ����������ͼ���� */    
    HOST_CMD_FR_BMP2JPG                 = 0x8008000e,       /* BMPתjpg */
    HOST_CMD_FR_ROI                     = 0x8008000f,       /* ����������� */
    HOST_CMD_FR_SAFETY_HELMET           = 0x80080011,       /* ��ȫñ����*/
    HOST_CMD_FR_PIC_FD                  = 0x80080012,       /* ͼƬ������⹦�� */
    HOST_CMD_FR_SIM_THRESHOLD           = 0x80080013,       /* ���õ�����߶���ʹ�ã��ͱȶ����ƶȵĲ�ͬ��ֵ */
    HOST_CMD_FR_CHECK_MODEL             = 0x80080014,       /* ģ��У�� */
    HOST_CMD_FR_SAVE_1VN_PIC            = 0x80080015,       /* 1VN��ͼ���ܵ��Կ��� */
    HOST_CMD_FR_INSERT_IMAGE            = 0x80080016,       /* ��ͼƬʹ�ܿ��� */
    HOST_CMD_FR_FLD_LIGHT_PRM           = 0x80080017,       /* ��ȡ���л���ص�״̬ */
    HOST_CMD_GET_ENG_MEM                = 0x80080018,       /* ��ȡ�����ڴ�ʹ����� */
    HOST_CMD_FR_DEL_MODEL               = 0x80080019,       /* ɾ��ģ��*/
    HOST_CMD_FR_ADD_MODEL               = 0x8008001a,       /* ���ģ��*/
    HOST_CMD_FR_QUE_MODEL               = 0x8008001b,       /* ��ѯģ��*/
    HOST_CMD_FR_CMP_1VN                 = 0x8008001c,       /* ģ��1VN����*/
    HOST_CMD_FR_SET_TOF_CALIB           = 0x8008001d,       /* ����TOF�ı궨���� */
    HOST_CMD_FR_SET_FLD_SAV_PIC_DIR     = 0x8008001e,       /* �������л����ͼ·�� FR_FACELD_SAV_PIC_DIR_INFO */
    HOST_CMD_FR_SET_SCORE               = 0x8008001f,       /* ����DFR  ���̽�ģ�������ۺ�������ֵ*/
    HOST_CMD_FR_GET_SCORE               = 0x80080020,       /* ��ȡDFR  ���̽�ģ�������ۺ�������ֵ*/

    HOST_CMD_FR_SET_CALIBRATION         = 0x80080021,       /* �궨�������� */
    HOST_CMD_FR_DET_V2                  = 0x80080022,       /* ������������� */
    HOST_CMD_SET_THRESHOLD              = 0x80080023,       /* ������ֵ���� */
    HOST_CMD_GET_THRESHOLD              = 0x80080024,       /* ������ֵ��ȡ */

    HOST_CMD_FR_GET_SIM_THRESHOLD       = 0x80080025,       /* ��ȡĬ�����ƶȱȶ���ֵ */

    /* ΢����ģ�� */
    HOST_CMD_MODULE_VDA                 = 0x80090000,       /* ��Ƶ���ģ��������� */
    HOST_CMD_START_MD                   = 0x80090001,       /* �����ƶ���� */
    HOST_CMD_STOP_MD                    = 0x80090002,       /* ֹͣ�ƶ���� */
    HOST_CMD_SET_MD                     = 0x80090003,       /* �����ƶ�������� */
    HOST_CMD_SET_CD                     = 0x80090004,       /* �����ڵ���� */
    HOST_CMD_SET_MASK                   = 0x80090005,       /* ������Ƶ�ڵ� */
    HOST_CMD_QR_START                   = 0x80090006,       /* ������ά��ɨ�� */
    HOST_CMD_QR_STOP                    = 0x80090007,       /* �رն�ά��ɨ�� */
    HOST_CMD_OCR_START                  = 0x80090008,       /* ��������ʶ��*/
    HOST_CMD_OCR_STOP                   = 0x80090009,       /* �ر�����ʶ��*/

    HOST_CMD_MODULE_PRIVATE             = 0x80090200,       /* ����˽����Ϣ        */
    HOST_CMD_SET_WATERMARK              = 0x80090201,       /* ����ˮӡ        */
    HOST_CMD_SET_SUB_WATERMARK          = 0x80090202,       /* ������ͨ��ˮӡ  */

    HOST_CMD_MODULE_FLD                 = 0x800a0000,       /* ������������ */
    HOST_CMD_SET_SOLID_PARAM            = 0x800a0001,       /* ���û������ */
    HOST_CMD_GET_SOLID_PARAM            = 0x800a0002,       /* ��ȡ������� */

    /* ��Ĥʶ��ģ�� */
    HOST_CMD_MODULE_IRIS                = 0x800b0000,       /* ��Ĥʶ��ģ��������� */
    HOST_CMD_IRIS_INIT                  = 0x800b0001,       /* ��ʼ��IRISģ�� */
    HOST_CMD_IRIS_DEINIT                = 0x800b0002,       /* ����ʼ�����ͷź�Ĥ��Դ */
    HOST_CMD_IRIS_GET_VER               = 0x800b0003,       /* ��ȡ��Ĥ�㷨�汾 */
    HOST_CMD_IRIS_GET_ABILITY           = 0x800b0004,       /* ��ȡ��Ĥ�㷨������ */
    HOST_CMD_IRIS_DETECT                = 0x800b0005,       /* ������Ĥ��⡢ʶ�𡢱���¼�� */
    HOST_CMD_IRIS_BM                    = 0x800b0006,       /* �·���ĤͼƬ���н�ģ��ȡ������ص����� */
    HOST_CMD_IRIS_COMPARE               = 0x800b0007,       /* ������Ĥģ�����1V1 �ȶԣ����ֱ�ӷ��� */
    HOST_CMD_IRIS_POOL_PRM              = 0x800b0008,       /* ��ȡ�׿��ַ���������ڴ��ˢcache������ʵ��ģ���Ĺ��� */
	HOST_CMD_IRIS_TOF            		= 0x800b0009,       /* ��ȡTOF�����Ϣ */
	HOST_CMD_IRIS_SAVE_PIC              = 0x800b000a,       /* ��ͼ���ܵ��Կ��� */
	HOST_CMD_IRIS_CALIBRATE             = 0x800b000b,       /* �궨ģʽ���� */	
	HOST_CMD_IRIS_ISP_PRM				= 0x800b000c,		/* ��ĤISP������ʹ�ýṹ��IRIS_ISP_PRMAM��֧�ֶ��߳�ʹ�ã�����DSP��ʼ���� */


    HOST_CMD_MODULE_EIF                 = 0x800c0000,       /* ˫���ںϹ��� */
    HOST_CMD_EIF_SET_PARAM              = 0x800c0001,       /* ���� ˫���ںϹ��� ���� */

    /* TOFģ�� */
	HOST_CMD_MODULE_TOF                 = 0x800d0000,        /*TOFģ��������� */
    HOST_CMD_TOF_START                  = 0x800d0001,        /* ����TOF��� */
    HOST_CMD_TOF_STOP                   = 0x800d0002,        /* �ر�TOF��� */
    HOST_CMD_TOF_START_SAVE             = 0x800d0003,        /* ����TOF��ͼ���湦�� */
    HOST_CMD_TOF_STOP_SAVE              = 0x800d0004,        /* �ر�TOF��ͼ���湦�� */
    HOST_CMD_TOF_TEMPERATURE            = 0x800d0005,        /* ��ȡTOFSensor�¶���Drive�¶� */

    /* өʯ�Ƽ��� */
    HOST_CMD_EZVIZ_AES_MODULE		    = 0x800e0000,       /* ���ݼ���ģ�������� */

	/* ������ܣ�ֻ֧��H264 */
	HOST_CMD_ENC_EZVIZ_AES_CONFIG       = 0x800e0001,       /* AES�������ã�������Կʱ�ã�����ǰ����ֹͣ���ܣ����δֹͣ���践��ʧ�� */
    HOST_CMD_ENC_EZVIZ_AES_START        = 0x800e0002,       /* ��ʼAES ���ܣ����ݴ�0��ַд���� */
    HOST_CMD_ENC_EZVIZ_AES_STOP         = 0x800e0003,       /* ֹͣ AES ���� */

	/* ������ܣ�ֻ֧��H264 */
	HOST_CMD_DEC_EZVIZ_AES_CONFIG       = 0x800e0004,       /* AES�������ã�������Կʱ�ã�����ǰ����ֹͣ���ܣ����δֹͣ���践��ʧ�� */
    HOST_CMD_DEC_EZVIZ_AES_START        = 0x800e0005,       /* ��ʼAES ���ܣ����ݴ�0��ַд���� */
    HOST_CMD_DEC_EZVIZ_AES_STOP         = 0x800e0006,       /* ֹͣ AES ���� */

    /* ���� */
    HOST_CMD_INVALID                    = 0xffffffff,       /* �����Чֵ */
}HOST_CMD;

/*************************************************************************************
                            �ӿڶ���
*************************************************************************************/
int SendCmdToDsp
(
    HOST_CMD cmd, unsigned int chan,
    unsigned int * pParam, void *pBuf
);


/*******************************************************************************
* ������  : InitDspSys
* ��  ��  : Ԥ��ʼ��DSP����������Ҫ�Ļ�����Դ�����ȫ�ֽṹ�Ŀռ���䣻�������
            �������ǰ�ڽ���ͨѶ��Դ�Ľ�������������ӿڳɹ����غ�APP�Ϳ��Խ�
            �� DSPINITPARA �����������ˣ������������ͨ�� HOST_CMD_DSP_INIT ����
            �� DSP ������Դ��ҵ��������
* ��  ��  : - ppDspInitPara: APP��Ҫָ��ȫ�ֽṹ���ָ��
*         : - pFunc        : APP���ݸ�DSP�Ļص�������
* ��  ��  : - ppDspInitPara: APP��ȡ����ȫ�ֽṹ��
* ����ֵ  : 0  : �ɹ�
*           -1 : ʧ��
*******************************************************************************/
int InitDspSys(DSPINITPARA **ppDspInitPara, DATACALLBACK pFunc);



/* SDK �� APPֱ�ӵĽ����ֱ��ʣ��÷ֱ�����SDK���壬��APP��ȫƽ̨�����ȡ������ ����������DSPͷ�����С� */
#define CIF_FORMAT              0x00001001
#define QCIF_FORMAT             0x00001002
#define FCIF_FORMAT             0x00001003  /*add 2002-6-25*/
#define HCIF_FORMAT             0x00001004
#define QQCIF_FORMAT            0x00001005  /* PAL: 96*80 NTSC: 96*64*/
#define NCIF_FORMAT             0x00001006  /* 320*240*/
#define QNCIF_FORMAT            0x00001007  /* 160*120*/
#define DCIF_FORMAT             0x00001008  /* PAL:528*384 NTSC:528*320 */
#define D1_FORMAT               0x0000100F  /* 720*576 �������� 2003-12-25 */
#define VGA_FORMAT              0x00001010  /* VGA:640*480��PAL��NTSC��ͬ */
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

#define HD_H_FORMAT             0x00001017  /*ǰ�������������1920*1080,��÷ֱ���Ϊ
                                          1920*540,���ǰ�˽���ʱ720p�������1280*360 added 20090712*/
#define HD_D_FORMAT             0x00001018  /*ǰ�������������1920*1080,��÷ֱ���Ϊ
                                          1440*720,���ǰ�˽���ʱ720p�������960*480 added 20090712*/
#define HD_Q_FORMAT             0x00001019  /*ǰ�������������1920*1080,��÷ֱ���Ϊ
                                          60*540,���ǰ�˽���ʱ720p�������640*360 added 20090712*/
#define HD_QQ_FORMAT            0x0000101a  /*ǰ�������������1920*1080,��÷ֱ���Ϊ
                                          480*270,���ǰ�˽���ʱ720p�������320*180 added 20090712*/
#define HD1080i_FORMAT          0x0000101b  /*1920*1080i added 200904010*/
#define HD1080p_FORMAT          0x0000101b  /*ע���1920*1080iһ��!! �Ժ�����, 1920*1080p*/
#define RES_2560_1920_FORMAT    0x0000101c  /*2560*1920  added 2009-10-19*/
#define RES_1600_304_FORMAT     0x0000101d  /*1600*304  added 2009-10-30*/
#define RES_2048_1536_FORMAT    0x0000101e  /*2048*1536  added 2009-12-23*/
#define RES_2448_2048_FORMAT    0x0000101f  //2448*2048 added 2010-01-06
//#define SXGA_FORMAT                0x00001023        /* 1280x1024 */

#define RES_2208_1242_FORMAT    0x00001024
#define RES_2304_1296_FORMAT    0x00001025
#define RES_2304_1536_FORMAT    0x00001026
#define RES_2592_1944_FORMAT    0x00001027  /* 2592*1944, 500w�ֱ��� */

#define RES_720_1280_FORMAT     0x000010a7
#define RES_1080_1920_FORMAT    0x000010ac
#define RES_1080_720_FORMAT     0x000010d7
#define RES_360_640_FORMAT      0x000010d8
#define RES_1944_2592_FORMAT    0x000010d9  /* 1944*2592, 500w�ֱ��ʣ�������sensorʹ�� */
#define RES_480_640_FORMAT      0x000010da  /* 480*640, �ṹ��IRʹ�� */

#ifdef __cplusplus
}
#endif


#endif  /* _DSPCOMMON_H_ */
