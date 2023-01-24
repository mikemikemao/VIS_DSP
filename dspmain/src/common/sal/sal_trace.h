/*******************************************************************************
 * sal_trace.h
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : heshengyuan <heshengyuan@hikvision.com>
 * Version: V1.0.0  2014��4��2�� Create
 *
 * Description : �滻ϵͳ�Ĵ�ӡ�����������ն˴�ӡ��ɫ������ģ�����ƣ�����ͳһ�ĵ�
                 �Դ�ӡ�ӿڣ��ں�������ϵͳ���򶼿���ʹ�á����ģ�������漰������
                 ���������ÿո���ߴ��Ŵ�ӡ�� �޸�Makefile��SAL_MODULE_NAME = XXX
                 �� XXX Ϊ��Ҫ�����Ƽ��ɡ�
 * Modification: 2014-04-21 ------ ��Ӷ��ն���ɫ��ӡ����ϸ˵����
                 �ն���ʾ��ӡ֧�� 6����ʾ��ʽ��8��ǰ��ɫ��8�ֱ���ɫ����֧�� 6*8*8 = 384 ��ɫ�������ʾ
                 6����ʾ��ʽ:   0(�ر�����) 1(����) 4(�»���) 5(��˸) 7(����) 8(����)
                 8��ǰ��ɫ:     30 (��ɫ) 31(��ɫ) 32(��ɫ) 33(��ɫ) 34(��ɫ) 35(��ɫ) 36(����) 37(��ɫ)
                 8��ǰ��ɫ:     40 (��ɫ) 41(��ɫ) 42(��ɫ) 43(��ɫ) 44(��ɫ) 45(��ɫ) 46(����) 47(��ɫ)
                 ��ʽ����ΪESC[attr;Foreground;Background��������ȡ�˼��ֳ�������ɫ���á�
                 ��������Ľű��ļ�����õ�384��ֱ�۵���ɫ��

                 �޸ĸ�ģ�飬ȥ�������õ���ɫ��ֻ��������������ɫ��������Ϣ(��)��������Ϣ(��)�Լ�������Ϣ(��)
                 �Լ����ӹ����ڴ���־��ӡ���� --by wangweiqin
#!/bin/sh
for attr in 0 1 4 5 7 8; do
        echo "ESC[$attr;Foreground;Background:"
        for fore in 30 31 32 33 34 35 36 37; do
                for back in 40 41 42 43 44 45 46 47; do
                        printf '\033[%s;%s;%sm' $attr $fore $back
                        printf '\\033[%s;%02s;%02sm'  $attr $fore $back         # ���е�ֱ�۴�ӡ����ֱ�����ڳ�����Ϊ��ɫ��
                        printf '\033[m'
                        printf ' '
                done
                echo
        done
        echo
        echo
done

 *******************************************************************************/

#ifndef _SAL_TRACE_H_
#define _SAL_TRACE_H_

#include <sal_log.h>
#include "sal_macro.h"

/* ��־������ */
typedef enum
{
    SAL_LOG_LV_ERR = 0,            /* �����ӡ */
    SAL_LOG_LV_WARN,               /* �����ӡ */
    SAL_LOG_LV_INFO,               /* ͨ���ӡ */
    SAL_LOG_LV_DBG,                /* ���Դ�ӡ */
    SAL_LOG_LV_MAX
}SAL_LogLevel;

/*
 ģ������ֱ�����Makefile������ͷ�ļ��ж��壬���ֲ���Ҫ����[]��
 ��Makefile����ӵķ����ǣ�
 SAL_MODULE_NAME = XXX
 CFLAGS       += -DSAL_MODULE_NAME='"$(SAL_MODULE_NAME)"'
 EXTRA_CFLAGS += -D'SAL_MODULE_NAME=\"$(SAL_MODULE_NAME)\"'
 Ӧ�ó�����ʹ��CFLAGS������������ʹ��EXTRA_CFLAGS��
*/

#define SAL_MODULE_NAME "DSP"

#ifndef SAL_MODULE_NAME
#error ##########################################################
#error Not define SAL_MODULE_NAME at Makefile !!!
#error ##########################################################
#endif

#ifndef SAL_COMPILE_USER
#define SAL_COMPILE_USER "unknown"
#endif

/* Android ��־ͷ�ļ�  */
#include <log/log.h>
#include <sys/syscall.h>
#include "dsp_log4j.h"

#define SAL_log4j_info              SAL_loginfo
#define SAL_log4j_warn              SAL_logwarn
#define SAL_log4j_error             SAL_logerror

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG      "DSP"
#endif

#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)

#define SAL_DEBUG(fmt, args ...) \
    do \
    { \
        if (SAL_SOK == SAL_logAllowed(SAL_LOG_LV_DBG)) \
        { \
            ALOGD("%s|%s|%d: " \
                    fmt, __FILENAME__, __FUNCTION__, __LINE__, ##args); \
        } \
    } \
    while(0)

#define SAL_ERROR(fmt, args ...) \
    do \
    { \
        if (SAL_SOK == SAL_logAllowed(SAL_LOG_LV_ERR)) \
        { \
            ALOGE("%s|%s|%d: " \
                    fmt, __FILENAME__, __FUNCTION__, __LINE__, ##args); \
            if(SAL_SOK == Log4j_recordLevel(SAL_LOG_LV_ERR)){SAL_log4j_error(fmt, ## args);Log4j_updateSyncStatus(SAL_TRUE);}\
        } \
    } \
    while(0)

#define SAL_WARN(fmt, args ...) \
    do \
    { \
        if (SAL_SOK == SAL_logAllowed(SAL_LOG_LV_WARN)) \
        { \
            ALOGW("%s|%s|%d: " \
                    fmt, __FILENAME__, __FUNCTION__, __LINE__, ##args); \
            if(SAL_SOK == Log4j_recordLevel(SAL_LOG_LV_WARN)){SAL_log4j_warn(fmt, ## args);Log4j_updateSyncStatus(SAL_TRUE);}\
        } \
    } \
    while(0)

#define SAL_INFO(fmt, args ...) \
    do \
    { \
        if (SAL_SOK == SAL_logAllowed(SAL_LOG_LV_INFO)) \
        { \
            ALOGI("%s|%s|%d: " \
                    fmt, __FILENAME__, __FUNCTION__, __LINE__, ##args); \
            if(SAL_SOK == Log4j_recordLevel(SAL_LOG_LV_INFO)){SAL_log4j_info(fmt, ## args);Log4j_updateSyncStatus(SAL_TRUE);}\
        } \
    } \
    while(0)

/* ��DFX��̬���Դ�ӡʹ�ã�printf�����������ӡ��� */
#define SAL_logPrintf(fmt, args ...) \
    do \
    { \
        ALOGI(fmt, ##args); \
        printf("[DSP] " fmt, ##args); \
        log4j_dsp_record(fmt, ## args);\
    } \
    while(0)

#endif /* _SAL_TRACE_H_ */

