/*******************************************************************************
 * sal_type.h
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : heshengyuan <heshengyuan@hikvision.com>
 * Version: V1.0.0  2014年4月2日 Create
 *
 * Description : 海思的SDK包定义了很多数据类型，后端DSP代码也定义了很多数据类型，
                 如果类型太多会造成变量间赋值混乱，需要及时强制转换为某一类型。
                 本文件统一定义常见数据类型，在除SDK包以外的文件如果也定义了数据
                 类型，逐步移到这个文件来，方便程序简洁和减少出错。

                 重新整理该文件，整理入SAL模块，SAL模块做为一个通用模块 -- wangweiqin
 * Modification:
 *******************************************************************************/

#ifndef _SAL_TYPES_H_
#define _SAL_TYPES_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif


/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */
/* 布尔量定义 */
#ifndef HIK_TYPE_BOOL
#define HIK_TYPE_BOOL
typedef unsigned char       BOOL;        /* 通用布尔类型 */
#endif

/* 有符号类型定义 */
#ifndef HIK_TYPE_INT8
#define HIK_TYPE_INT8
typedef char                INT8;       /* 有符号8位整形数类型 */
#endif

#ifndef HIK_TYPE_INT16
#define HIK_TYPE_INT16
typedef short               INT16;      /* 有符号16位整形数类型 */
#endif


#ifndef HIK_TYPE_INT32
#define HIK_TYPE_INT32
typedef int                 INT32;      /* 有符号32位整形数类型 */
#endif

#ifndef HIK_TYPE_INT32L
#define HIK_TYPE_INT32L
typedef long                INT32L;     /* 有符号32位长整形数类型 */
#endif

#ifndef HIK_TYPE_INT64L
#define HIK_TYPE_INT64L
typedef long long           INT64L;     /* 有符号64位整形数类型 */
#endif

/* 无符号类型定义 */
#ifndef HIK_TYPE_UINT8
#define HIK_TYPE_UINT8
typedef unsigned char       UINT8;      /* 无符号8位整形数类型 */
#endif

#ifndef HIK_TYPE_UINT16
#define HIK_TYPE_UINT16
typedef unsigned short      UINT16;     /* 无符号16位整形数类型 */
#endif


#ifndef HIK_TYPE_UINT32
#define HIK_TYPE_UINT32
typedef unsigned int        UINT32;     /* 无符号32位整形数类型 */
#endif

#ifndef HIK_TYPE_UINT32L
#define HIK_TYPE_UINT32L
typedef unsigned long       UINT32L;    /* 无符号32位长整形数类型 */
#endif

#ifndef HIK_TYPE_UINT64
#define HIK_TYPE_UINT64
typedef unsigned long long  UINT64;     /* 无符号64位整形数类型 */
#endif

/* 浮点类型定义 */
#ifndef HIK_TYPE_FLOAT32
#define HIK_TYPE_FLOAT32
typedef float               FLOAT32;    /* 32位浮点数类型 */
#endif

#ifndef HIK_TYPE_FLOAT64
#define HIK_TYPE_FLOAT64
typedef double              FLOAT64;    /* 64位浮点数类型 */
#endif

/* 指针类型定义 */
#ifndef HIK_TYPE_PTR
#define HIK_TYPE_PTR
typedef void *              Ptr;         /* 指针类型 */
#endif

/* 句柄类型 */
#ifndef HIK_TYPE_Handle
#define HIK_TYPE_Handle
typedef void *              Handle;      /* 统用句柄类型 */
#endif

#ifndef HIK_TYPE_PUINT8
#define HIK_TYPE_PUINT8
typedef unsigned char       *PUINT8;    /* 无符号8位整形数类型 */
#endif

#ifndef HIK_TYPE_VOID
#define HIK_TYPE_VOID
typedef void                VOID;
#endif


/* BANDWIDTH_64BIT 宏是系统位宽的开关，可通过Makeife传入，传入方法是-D BANDWIDTH_64BIT 。 */
#ifdef BANDWIDTH_64BIT
typedef unsigned long long  PhysAddr;   /* 64 位系统的内存地址描述 */
#else
typedef unsigned int        PhysAddr;   /* 32 位系统的内存地址描述 */
#endif

#ifdef __cplusplus
}
#endif

#endif                                  /*  _SAL_TYPES_H_  */


