/*******************************************************************************
 * sal_shm.h
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : zhaocan <zhaocan@hikvision.com>
 * Version: V1.0.0  2020��08��18�� Create
 *
 * Description : Linux�¹����ڴ����ͷ�ļ�
 * Modification:
 *******************************************************************************/

#ifndef _SAL_SHM_H_
#define _SAL_SHM_H_

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include <sal.h>

/*************************************************************************************
                            �궨��
*************************************************************************************/
#define MAX_SHM_ID  (256)
#define MIN_SHM_ID  (1)


/*************************************************************************************
                                     �ṹ�嶨��
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
    UINT32 ID;                                /* �����ڴ�ID */
    UINT32 shmSize;                           /* �����ڴ��С */
    UINT32 pid;                               /* �����ڴ��������� */
    INT32  shmID;                             /* �����ڴ��shmID */
    UINT8 *pShmVaddr;                         /* �����ڴ�ӳ�����ʼ��ַ */
} SAL_SHM_INFO;

/*************************************************************************************
                                     ��������
*************************************************************************************/

/*******************************************************************************
* ������   : SAL_shmCreate
* ��  ��    : ���������ڴ�
* ��  ��    : - shmID  : �������Ĺ����ڴ��ʶ����MIN_SHM_ID <= ID <= MAX_SHM_ID
*             - size   : �������Ĺ����ڴ��С��size <= (32MB-40Byte)
*           : - needWritePermission : �����Ĺ����ڴ��Ƿ���ҪдȨ��
* ��  ��    : - pstShmInfo   : ������ɵĹ����ڴ���Ϣ
* ����ֵ    : SAL_SHM_OK  : �����ɹ�
*          ���� : ����ʧ��
*******************************************************************************/
SAL_SHM_RESULT SAL_shmCreate(UINT32 ID, UINT32 size, BOOL needWritePermission, SAL_SHM_INFO *pstShmInfo);

/*******************************************************************************
* ������   : SAL_shmDelete
* ��  ��    : �Ͽ��빲���ڴ�����ӣ����һ�ε��û��Զ�ɾ���ù����ڴ�
* ��  ��    : - pstShmInfo  : ���Ͽ��Ĺ����ڴ���Ϣ��ֻ�踳ֵID��pShmVaddr���ɡ�
* ��  ��    : ��
* ����ֵ    : SAL_SHM_OK  : �Ͽ��ɹ�
*          ���� : �Ͽ�ʧ��
*******************************************************************************/
SAL_SHM_RESULT SAL_shmDetach(SAL_SHM_INFO *pstShmInfo);

/*******************************************************************************
* ������   : SAL_shmDelete
* ��  ��    : ��ȡ���õĹ����ڴ�ID
* ��  ��    : ��
* ��  ��    : - pID  : ����ֵ�Ĺ����ڴ�ID��MIN_SHM_ID <= *pID <= MAX_SHM_ID��
* ����ֵ    : SAL_SHM_OK  : �Ͽ��ɹ�
*          ���� : �Ͽ�ʧ��
*******************************************************************************/
SAL_SHM_RESULT SAL_shmGetID(UINT32 *pID);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

