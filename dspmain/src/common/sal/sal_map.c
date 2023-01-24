/*******************************************************************************
 * sal_map.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : hsy <heshengyuan@hikvision.com>
 * Version: V1.0.0  2022��6��20�� Create
 *
 * Description : 
 * Modification: 
 *******************************************************************************/

/* ========================================================================== */
/*                             ͷ�ļ���                                       */
/* ========================================================================== */
#include <sys/mman.h>
#include "sal.h"
#include "sal_map.h"

/* ========================================================================== */
/*                           ������Ͷ�����                                   */
/* ========================================================================== */


/* ========================================================================== */
/*                          ���ݽṹ������                                    */
/* ========================================================================== */
static UINT32 mapShareTotalLen  = 0;

/* ========================================================================== */
/*                          ����������                                        */
/* ========================================================================== */


/*******************************************************************************
* ������  : SAL_mmapFileInit
* ��  ��  : ��װͨ���ļ���ʽ�������ڴ�ӳ���ͨ�ú���
* ��  ��  : - pFileName: �ļ�·��, ���ļ�ֱ�������ڴ�ռ��ϣ�ͨѶЧ�ʺ�ͬ�����
*         : - role     : ����˻��ǿͻ��ˣ�����ֻ�ǳ�ʼ��ʱ�Ƿ�����ļ����ݣ�����
                         �޲��
*         : - size     : ӳ���С��Ҳ�ǹ����ڴ�Ĵ�С
* ��  ��  : ��
* ����ֵ  : NULL       : ʧ�ܣ�����ӳ��ɹ�����Ƭ�ڴ���׵�ַ
*******************************************************************************/
void *SAL_mmapFileInit(const INT8 *pFileName, UINT32 role, UINT32 size)
{
    INT32           mapfd       = -1;
    INT32           openflag    = 0;
    void           *pVaddr      = NULL;

    if(pFileName == NULL)
    {
        SAL_ERROR("pFile is NULL\n");
        return NULL;
    }

    /* �����ӳ���ļ�����գ��ͻ���ֱ��ӳ�伴�ɲ���Ҫ������� */
    if (SAL_SHM_ROLE_SERVER == role)
    {
        openflag = O_RDWR|O_CREAT|O_TRUNC;
    }
    else if (SAL_SHM_ROLE_CLIENT == role)
    {
        openflag = O_RDWR|O_CREAT;
    }
    else
    {
        SAL_ERROR("!!!Shm role error \n");
        return NULL;
    }

    mapfd = open(pFileName, openflag, 0666);
    if (mapfd < 0)
    {
        SAL_ERROR("!!!Open %s error, %s\n", pFileName, strerror(errno));
        return NULL;
    }

    ftruncate(mapfd, size);  /* set bufsize */

    pVaddr = mmap(NULL, size, (PROT_READ|PROT_WRITE), MAP_SHARED, mapfd, 0);  //last offset
    if (pVaddr == MAP_FAILED)
    {
        SAL_ERROR("!!!mmap fail \n");
        SAL_close(mapfd);

        /* remove this file */
        if (SAL_SOK != remove(pFileName))
        {
            SAL_WARN("Shm remove %s error \n", pFileName);
        }
        return NULL;
    }

    SAL_close(mapfd);

    mapShareTotalLen += size;

    return pVaddr;
}

/*******************************************************************************
* ������  : SAL_mmapFileDeinit
* ��  ��  : �Ƿ��ļ�ӳ�乲���ڴ�
* ��  ��  : - pVaddr: �ͷŵ�ַ����Ҫʹ�������ַ
*         : - size  : �ڴ泤��
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_mmapFileDeinit(void *pVaddr, UINT32 size)
{

    if(SAL_SOK != munmap(pVaddr, (size_t)size))
    {
        SAL_ERROR("!!!mmumap fail, error(%s)\n", strerror(errno));
        return SAL_FAIL;
    }
    
    mapShareTotalLen -= size;

    return SAL_SOK;
}

