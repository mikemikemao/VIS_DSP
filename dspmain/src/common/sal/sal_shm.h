/*******************************************************************************
 * sal_shm.h
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : zhaocan <zhaocan@hikvision.com>
 * Version: V1.0.0  2020年08月18日 Create
 *
 * Description : Linux下共享内存管理头文件
 * Modification:
 *******************************************************************************/

#ifndef _SAL_SHM_H_
#define _SAL_SHM_H_

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include <sal.h>

/*************************************************************************************
                            宏定义
*************************************************************************************/
#define MAX_SHM_ID  (256)
#define MIN_SHM_ID  (1)


/*************************************************************************************
                                     结构体定义
*************************************************************************************/
//#pragma pack(1)

typedef enum _SAL_SHM_RESULT
{
    SAL_SHM_OK                      = 0,

    SAL_SHM_ERR                     = -1, /* err */
    SAL_SHM_ERR_NOT_AVAIL           = -2, /* input invalid */
    SAL_SHM_ERR_NOMEM               = -3, /* No Enough Memory */
    SAL_SHM_ERR_PID_ERROR           = -4, /* No this Process */
    SAL_SHM_ERR_ADDR_NULL           = -5, /* Process virtual address is NULL */
    SAL_SHM_ERR_TIMEDOUT            = -6, /* timed out */
    SAL_SHM_ERR_BAD_DATA            = -7, /* bad data --data is already covered */
    SAL_SHM_ERR_PERMISSION_DENIED   = -8, /* have no permission */
    SAL_SHM_ERR_NOID                = -9, /* No Enough ID */
} SAL_SHM_RESULT;

typedef struct _SAL_SHM_INFO
{
    UINT32 ID;                                /* 共享内存ID */
    UINT32 shmSize;                           /* 共享内存大小 */
    UINT32 pid;                               /* 共享内存所属进程 */
    INT32  shmID;                             /* 共享内存的shmID */
    UINT8 *pShmVaddr;                         /* 共享内存映射的起始地址 */
} SAL_SHM_INFO;

/*************************************************************************************
                                     函数定义
*************************************************************************************/

/*******************************************************************************
* 函数名   : SAL_shmCreate
* 描  述    : 创建共享内存
* 输  入    : - shmID  : 待创建的共享内存标识符，MIN_SHM_ID <= ID <= MAX_SHM_ID
*             - size   : 待创建的共享内存大小，size <= (32MB-40Byte)
*           : - needWritePermission : 创建的共享内存是否需要写权限
* 输  出    : - pstShmInfo   : 创建完成的共享内存信息
* 返回值    : SAL_SHM_OK  : 创建成功
*          其他 : 创建失败
*******************************************************************************/
SAL_SHM_RESULT SAL_shmCreate(UINT32 ID, UINT32 size, BOOL needWritePermission, SAL_SHM_INFO *pstShmInfo);

/*******************************************************************************
* 函数名   : SAL_shmDelete
* 描  述    : 断开与共享内存的链接，最后一次调用会自动删除该共享内存
* 输  入    : - pstShmInfo  : 待断开的共享内存信息，只需赋值ID和pShmVaddr即可。
* 输  出    : 无
* 返回值    : SAL_SHM_OK  : 断开成功
*          其他 : 断开失败
*******************************************************************************/
SAL_SHM_RESULT SAL_shmDetach(SAL_SHM_INFO *pstShmInfo);

/*******************************************************************************
* 函数名   : SAL_shmDelete
* 描  述    : 获取可用的共享内存ID
* 输  入    : 无
* 输  出    : - pID  : 待赋值的共享内存ID，MIN_SHM_ID <= *pID <= MAX_SHM_ID。
* 返回值    : SAL_SHM_OK  : 断开成功
*          其他 : 断开失败
*******************************************************************************/
SAL_SHM_RESULT SAL_shmGetID(UINT32 *pID);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

