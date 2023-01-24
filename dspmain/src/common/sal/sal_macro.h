/*******************************************************************************
 * sal_macro.h
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : heshengyuan <heshengyuan@hikvision.com>
 * Version: V1.0.0  2018年4月16日 Create
 *
 * Description : 统一的通用宏。
 * Modification: 
 *******************************************************************************/
#ifndef _SAL_MACRO_H_
#define _SAL_MACRO_H_


/* ========================================================================== */
/*                             头文件区                                       */
#include <log/log.h>
#include "securec.h"
//#include "sal_trace.h"

/* ========================================================================== */


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

/* 常用布尔型判断 */
#define SAL_TRUE                        (1)
#define SAL_FALSE                       (0)
#define HIK_TRUE                        SAL_TRUE
#define HIK_FALSE                       SAL_FALSE

/* 常用程序返回值 */
#define SAL_SOK                         (0)
#define SAL_FAIL                        (-1)
#define SAL_ETIMEOUT                    (-2)        /* 等待超时*/
#define SAL_EINTR                       (-3)        /* 被信号中断*/

#define HIK_SOK                         SAL_SOK
#define HIK_FAIL                        SAL_FAIL

/* NULL值定义 */
#define SAL_NULL                        (NULL)

/* 超时参数 */
#define SAL_TIMEOUT_NONE                (0)    /* 不等待，立即返回。*/ 
#define SAL_TIMEOUT_FOREVER             (~0U)  /* 一直等待直到返回 */


/* 数据存储度量单位 */
#define SAL_KB                          (1024)
#define SAL_MB                          (SAL_KB * SAL_KB)
#define SAL_GB                          (SAL_KB * SAL_MB)

/* 数值比较*/
#define SAL_max(a, b)                   ( (a) > (b) ? (a) : (b) )
#define SAL_min(a, b)                   ( (a) < (b) ? (a) : (b) )

/* 安全函数替换 */
#define SAL_memSetSafe(dst, src, size) \
                do \
                { \
                    if (EOK != memset_s((dst),(size), (src), (size))) \
                    { \
                        ALOGE("error code = %d !\n",memset_s((dst),(size), (src), (size)));\
                    } \
                } \
                while(0)

#define SAL_memCpySafe(dst, src, size) \
                do \
                { \
                    if (EOK != memcpy_s((dst),(size), (src), (size))) \
                    { \
                        ALOGE("error code = %d !\n",memcpy_s((dst),(size), (src), (size)));\
                    } \
                } \
                while(0)

#define SAL_memMoveSafe(dst, src, count) \
        do \
        { \
            if (EOK != memmove_s((dst),(count), (src), (count))) \
            { \
                ALOGE("error code = %d !\n",memmove_s((dst),(count), (src), (count)));\
            } \
        } \
        while(0)

#define SAL_strCatSafe(dst, dstsize,src) \
    do \
    { \
        if (EOK != strcat_s((dst),(dstsize), (src))) \
        { \
            ALOGE("error code = %d !\n",strcat_s((dst),(dstsize), (src)));\
        } \
    } \
    while(0)

#define SAL_strnCpySafe(dst,dstsize,src,count) \
    do \
    { \
        if (EOK != strncpy_s((dst),(dstsize), (src),(count))) \
        { \
            ALOGE("error code = %d !\n",strncpy_s((dst),(dstsize), (src),(count)));\
        } \
    } \
    while(0)
        
#define SAL_strnCatSafe(dst,dstsize,src,count) \
    do \
    { \
        if (EOK != strncat_s((dst),(dstsize), (src),(count))) \
        { \
            ALOGE("error code = %d !\n",strncat_s((dst),(dstsize), (src),(count)));\
        } \
    } \
    while(0)

#define SAL_strCpySafe( dst,dstsize,src) \
    do \
    { \
        if (EOK != strcpy_s((dst),(dstsize), (src))) \
        { \
            ALOGE("error code = %d !\n",strcpy_s((dst),(dstsize), (src)));\
        } \
    } \
    while(0)

#define SAL_strTokSafe(token,limit,context) \
    do \
    { \
        if (EOK != strtok_s((token),(limit),(context))) \
        { \
            ALOGE("error code = %d !\n",strtok_s((token),(limit),(context)));\
        } \
    } \
    while(0)

//#define SAL_strCatSafe(dst, dstsize,src)            strcat_s((dst),(dstsize), (src))
//#define SAL_strnCatSafe(dst,dstsize,src,count)      strncat_s((dst),(dstsize), (src),(count))
//#define SAL_strCpySafe( dst,dstsize,src)            strcpy_s((dst),(dstsize), (src))
//#define SAL_strnCpySafe(dst,dstsize,src,count)      strncpy_s((dst),(dstsize), (src),(count))
//#define SAL_strTokSafe(token,limit,context)         strtok_s((token),(limit),(context))	

/* 内存操作*/
#define SAL_clear(ptr)                  SAL_memSetSafe((ptr), 0, sizeof(*(ptr)))
#define SAL_clearSize(ptr, size)        SAL_memSetSafe((ptr), 0, (size))
#define SAL_memCpySize(dst, src, size)  SAL_memCpySafe((dst), (src), (size))
#define SAL_memCmp(dst, src)            memcmp((dst), (src), sizeof(*(src)))
#define SAL_memCmpSize(dst, src, size)  memcmp((dst), (src), (size))

/* 判断返回值 */
#define SAL_isSuccess(status)           (SAL_SOK == (status))
#define SAL_isFail(status)              (SAL_SOK != (status))

#define SAL_isNull(ptr)                 (NULL == (ptr))
#define SAL_isNotNull(ptr)              (NULL != (ptr))

/* 对齐操作*/
#define SAL_align(value, align)         ((( (value) + ( (align) - 1 ) ) / (align) ) * (align) )
#define SAL_alignDown(value, align)     ((value) & ~(align - 1))
#define SAL_isAligned(value, align)     ((value % align) == 0)
#define SAL_ceil(value, align)          (SAL_align(value, align) )
#define SAL_floor(value, align)         (( (value) / (align) ) * (align) )

/* 获取数组成员数量 */
#define SAL_arraySize(array)            (sizeof(array)/sizeof((array)[0]))

/* 获取结构体成员的地址偏移量 */
#ifdef __compiler_offsetof
#define SAL_offsetof(TYPE,MEMBER)       (__compiler_offsetof(TYPE,MEMBER))
#else
#define SAL_offsetof(TYPE, MEMBER)      ((size_t) &((TYPE *)0)->MEMBER)
#endif


/* 通过结构体成员获取结构体首地址 */
#define SAL_containerOf(ptr, type, member) ({                       \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);       \
        (type *)( (char *)__mptr - SAL_offsetof(type,member) );})


/* 检测value值是否是2的N次方 */
#define SAL_checkIs2n(value)                ( ((value) == 0) ? SAL_FALSE  \
                                                : ((value) & ((value) - 1))  \
                                                ? SAL_FALSE : SAL_TRUE )

#define SAL_close(fd)   \
    do                  \
    {                   \
        if(fd <= 0) {    \
            break;      \
        }               \
        close(fd);      \
        fd = 0;         \
    }                   \
    while(0)

#define FRAME_NUM_DUMP_FPS           200

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

