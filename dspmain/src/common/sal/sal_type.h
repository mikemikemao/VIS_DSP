/*******************************************************************************
 * sal_type.h
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : heshengyuan <heshengyuan@hikvision.com>
 * Version: V1.0.0  2014��4��2�� Create
 *
 * Description : ��˼��SDK�������˺ܶ��������ͣ����DSP����Ҳ�����˺ܶ��������ͣ�
                 �������̫�����ɱ����丳ֵ���ң���Ҫ��ʱǿ��ת��Ϊĳһ���͡�
                 ���ļ�ͳһ���峣���������ͣ��ڳ�SDK��������ļ����Ҳ����������
                 ���ͣ����Ƶ�����ļ��������������ͼ��ٳ���

                 ����������ļ���������SALģ�飬SALģ����Ϊһ��ͨ��ģ�� -- wangweiqin
 * Modification:
 *******************************************************************************/

#ifndef _SAL_TYPES_H_
#define _SAL_TYPES_H_


/* ========================================================================== */
/*                             ͷ�ļ���                                       */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif


/* ========================================================================== */
/*                           ������Ͷ�����                                   */
/* ========================================================================== */
/* ���������� */
#ifndef HIK_TYPE_BOOL
#define HIK_TYPE_BOOL
typedef unsigned char       BOOL;        /* ͨ�ò������� */
#endif

/* �з������Ͷ��� */
#ifndef HIK_TYPE_INT8
#define HIK_TYPE_INT8
typedef char                INT8;       /* �з���8λ���������� */
#endif

#ifndef HIK_TYPE_INT16
#define HIK_TYPE_INT16
typedef short               INT16;      /* �з���16λ���������� */
#endif


#ifndef HIK_TYPE_INT32
#define HIK_TYPE_INT32
typedef int                 INT32;      /* �з���32λ���������� */
#endif

#ifndef HIK_TYPE_INT32L
#define HIK_TYPE_INT32L
typedef long                INT32L;     /* �з���32λ������������ */
#endif

#ifndef HIK_TYPE_INT64L
#define HIK_TYPE_INT64L
typedef long long           INT64L;     /* �з���64λ���������� */
#endif

/* �޷������Ͷ��� */
#ifndef HIK_TYPE_UINT8
#define HIK_TYPE_UINT8
typedef unsigned char       UINT8;      /* �޷���8λ���������� */
#endif

#ifndef HIK_TYPE_UINT16
#define HIK_TYPE_UINT16
typedef unsigned short      UINT16;     /* �޷���16λ���������� */
#endif


#ifndef HIK_TYPE_UINT32
#define HIK_TYPE_UINT32
typedef unsigned int        UINT32;     /* �޷���32λ���������� */
#endif

#ifndef HIK_TYPE_UINT32L
#define HIK_TYPE_UINT32L
typedef unsigned long       UINT32L;    /* �޷���32λ������������ */
#endif

#ifndef HIK_TYPE_UINT64
#define HIK_TYPE_UINT64
typedef unsigned long long  UINT64;     /* �޷���64λ���������� */
#endif

/* �������Ͷ��� */
#ifndef HIK_TYPE_FLOAT32
#define HIK_TYPE_FLOAT32
typedef float               FLOAT32;    /* 32λ���������� */
#endif

#ifndef HIK_TYPE_FLOAT64
#define HIK_TYPE_FLOAT64
typedef double              FLOAT64;    /* 64λ���������� */
#endif

/* ָ�����Ͷ��� */
#ifndef HIK_TYPE_PTR
#define HIK_TYPE_PTR
typedef void *              Ptr;         /* ָ������ */
#endif

/* ������� */
#ifndef HIK_TYPE_Handle
#define HIK_TYPE_Handle
typedef void *              Handle;      /* ͳ�þ������ */
#endif

#ifndef HIK_TYPE_PUINT8
#define HIK_TYPE_PUINT8
typedef unsigned char       *PUINT8;    /* �޷���8λ���������� */
#endif

#ifndef HIK_TYPE_VOID
#define HIK_TYPE_VOID
typedef void                VOID;
#endif


/* BANDWIDTH_64BIT ����ϵͳλ��Ŀ��أ���ͨ��Makeife���룬���뷽����-D BANDWIDTH_64BIT �� */
#ifdef BANDWIDTH_64BIT
typedef unsigned long long  PhysAddr;   /* 64 λϵͳ���ڴ��ַ���� */
#else
typedef unsigned int        PhysAddr;   /* 32 λϵͳ���ڴ��ַ���� */
#endif

#ifdef __cplusplus
}
#endif

#endif                                  /*  _SAL_TYPES_H_  */


