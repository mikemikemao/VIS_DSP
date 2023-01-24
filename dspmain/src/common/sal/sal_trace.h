/*******************************************************************************
 * sal_trace.h
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : heshengyuan <heshengyuan@hikvision.com>
 * Version: V1.0.0  2014年4月2日 Create
 *
 * Description : 替换系统的打印函数，增加终端打印颜色，加上模块名称，进行统一的调
                 试打印接口，内核驱动和系统程序都可以使用。如果模块名称涉及到保密
                 命名可以置空格或者代号打印。 修改Makefile宏SAL_MODULE_NAME = XXX
                 的 XXX 为需要的名称即可。
 * Modification: 2014-04-21 ------ 添加对终端颜色打印的详细说明。
                 终端显示打印支持 6种显示方式，8种前景色，8种背景色；共支持 6*8*8 = 384 种色彩组合显示
                 6种显示方式:   0(关闭属性) 1(高亮) 4(下划线) 5(闪烁) 7(反显) 8(消隐)
                 8种前景色:     30 (黑色) 31(红色) 32(绿色) 33(黄色) 34(蓝色) 35(紫色) 36(深绿) 37(白色)
                 8种前景色:     40 (黑色) 41(红色) 42(绿色) 43(黄色) 44(蓝色) 45(紫色) 46(深绿) 47(白色)
                 格式设置为ESC[attr;Foreground;Background，这里提取了几种常见的颜色配置。
                 运行下面的脚本文件，会得到384种直观的颜色。

                 修改该模块，去除不常用的颜色，只保留常用三种颜色，正常信息(绿)、警告信息(黄)以及错误信息(红)
                 以及增加共享内存日志打印功能 --by wangweiqin
#!/bin/sh
for attr in 0 1 4 5 7 8; do
        echo "ESC[$attr;Foreground;Background:"
        for fore in 30 31 32 33 34 35 36 37; do
                for back in 40 41 42 43 44 45 46 47; do
                        printf '\033[%s;%s;%sm' $attr $fore $back
                        printf '\\033[%s;%02s;%02sm'  $attr $fore $back         # 这行的直观打印可以直接用在程序作为颜色码
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

/* 日志级别定义 */
typedef enum
{
    SAL_LOG_LV_ERR = 0,            /* 错误打印 */
    SAL_LOG_LV_WARN,               /* 警告打印 */
    SAL_LOG_LV_INFO,               /* 通告打印 */
    SAL_LOG_LV_DBG,                /* 调试打印 */
    SAL_LOG_LV_MAX
}SAL_LogLevel;

/*
 模块的名字必须在Makefile或其他头文件中定义，名字不需要加上[]。
 在Makefile中添加的方法是：
 SAL_MODULE_NAME = XXX
 CFLAGS       += -DSAL_MODULE_NAME='"$(SAL_MODULE_NAME)"'
 EXTRA_CFLAGS += -D'SAL_MODULE_NAME=\"$(SAL_MODULE_NAME)\"'
 应用程序中使用CFLAGS，驱动程序中使用EXTRA_CFLAGS。
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

/* Android 日志头文件  */
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

/* 供DFX动态调试打印使用，printf会往串口里打印输出 */
#define SAL_logPrintf(fmt, args ...) \
    do \
    { \
        ALOGI(fmt, ##args); \
        printf("[DSP] " fmt, ##args); \
        log4j_dsp_record(fmt, ## args);\
    } \
    while(0)

#endif /* _SAL_TRACE_H_ */

