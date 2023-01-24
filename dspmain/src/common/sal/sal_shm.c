#include "sal_shm.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>


typedef struct
{
    UINT32 idx;
    UINT32 idUsed;
}SHMID_PARAM;

static SHMID_PARAM id_array[MAX_SHM_ID - MIN_SHM_ID];
static Handle shmIDMutex = 0;

// 支持Init重复调用
static SAL_SHM_RESULT ShareShmresourceInit(VOID)
{
    INT32 i = 0;

    if (0 == shmIDMutex)
    {
        if (SAL_SOK != SAL_mutexCreate(SAL_MUTEX_NORMAL, &shmIDMutex))
        {
            SAL_ERROR("SAL_mutexCreate failed!\n");
            return SAL_SHM_ERR;
        }

        SAL_mutexLock(shmIDMutex);
        for (i = 0; i < MAX_SHM_ID - MIN_SHM_ID; i++)
        {
            id_array[i].idx    = MIN_SHM_ID + i;
            id_array[i].idUsed = SAL_FALSE;
        }
        SAL_mutexUnlock(shmIDMutex);
    }

    return SAL_SHM_OK;
}


static SAL_SHM_RESULT ShareShmresourceDeInit(VOID)
{
    INT32 i = 0;
    if (0 != shmIDMutex)
    {
        SAL_mutexLock(shmIDMutex);
        for (i = 0; i < MAX_SHM_ID - MIN_SHM_ID; i++)
        {
            id_array[i].idx    = MIN_SHM_ID + i;
            id_array[i].idUsed = SAL_FALSE;
        }
        SAL_mutexUnlock(shmIDMutex);

        SAL_mutexDelete(shmIDMutex);
        shmIDMutex = 0;
    }

    return SAL_SHM_OK;
}


static SAL_SHM_RESULT ShareShmcreateTempFile(INT8 * pStrName)
{
    INT32 iFileFd = 0;

    if (NULL == pStrName)
    {
        SAL_ERROR("input path is NULL !!!\n");
        return SAL_SHM_ERR_NOT_AVAIL;
    }

    iFileFd = creat(pStrName, O_CREAT | 0666);
    if (-1 == iFileFd)
    {
        SAL_ERROR("%s %d file create failed! ERROR: %s!\n", __FUNCTION__, __LINE__, strerror(errno));
        return SAL_SHM_ERR;
    }

    SAL_close(iFileFd);

    return SAL_SHM_OK;
}

/* 获取共享内存地址 */
static void * ShareShmMAP(INT32 iShmid, void* shmaddr, BOOL needWritePermission)
{
    void * pVaddr = NULL;
    int shmflg = 0;     // 0 表示有读写权限

    if (SAL_FALSE == needWritePermission)
    {
        shmflg = SHM_RDONLY;
    }

    if (NULL == shmaddr)
    {
        pVaddr = shmat(iShmid, 0, shmflg);
    }
    else
    {
        shmflg = shmflg | SHM_REMAP;
        pVaddr = shmat(iShmid, shmaddr, shmflg);
    }

    if (NULL == pVaddr)
    {
        SAL_ERROR("Get Share Memory Error:%s\n",strerror(errno));
        return NULL;
    }

    return pVaddr;
}

/*****************************************************************************
 * 创建共享内存标志

   key      : 共享内存标志
   size     : 共享内存大小
   shmflg   : 共享内存权限 通常用IPC_CREAT | 0666即可

   返回值   : 共享内存的标识符
*****************************************************************************/
SAL_SHM_RESULT ShareShmCreate(key_t iKey, size_t iSize, INT32 iShmflg)
{
    INT32 shmid = 0;
    /* 返回 EEXIST 表示 参数key所指的共享内存已存在，且参数中设置了IPC_EXCL */
    shmid = shmget(iKey, iSize, iShmflg);
    if (-1 == shmid )
    {
        if (EEXIST == errno)
        {
            SAL_ERROR("It is exist !!\n");
            return SAL_SHM_ERR;
        }

        SAL_ERROR("Create Share Memory Error:%s\n",strerror(errno));
        return SAL_SHM_ERR;
    }

    return shmid;
}

SAL_SHM_RESULT SAL_shmCreate(UINT32 ID, UINT32 size, BOOL needWritePermission, SAL_SHM_INFO *pstShmInfo)
{
    INT32   shmid  = 0;
    INT8 shmFilename[256];
    key_t sem_key = 0;
    UINT8* pAddr = NULL;
    static UINT32 use_id = 0;

    if (NULL == pstShmInfo)
    {
        SAL_ERROR("input pstShmInfo is NULL !!!\n");
        return SAL_SHM_ERR_NOT_AVAIL;
    }

    if (MIN_SHM_ID > ID || MAX_SHM_ID < ID)
    {
        SAL_ERROR("SHM ID %d is out of range %d~%d !!!\n", ID, MIN_SHM_ID, MAX_SHM_ID);
        return SAL_SHM_ERR_NOT_AVAIL;
    }

    // 防止未调用过SAL_shmGetID接口时，创建共享内存，在释放时状态判断不正确；
    if (SAL_SHM_OK != ShareShmresourceInit())
    {
        SAL_ERROR("SHM Init resource Failed !!!\n");
        return SAL_SHM_ERR;
    }

    sprintf_s(shmFilename, sizeof(shmFilename)-1, "/tmp/DSP_FILE_FOR_SHM.image");
    /* 创建临时文件 */
    if (SAL_SHM_OK != ShareShmcreateTempFile(shmFilename))
    {
        SAL_ERROR("SHM Create shmFilename File Failed !!!\n");
        return SAL_SHM_ERR;
    }

    /* 获取key 值 */
    sem_key = ftok(shmFilename, ID);

    /* 创建共享内存id */
    shmid = ShareShmCreate(sem_key, size, IPC_CREAT|0666);
    if (SAL_SHM_ERR == shmid)
    {
        SAL_ERROR("SHM Create Failed !!!\n");
        return SAL_SHM_ERR;
    }

    /* 获取共享内存在本进程中的映射地址值 */
    pAddr = (UINT8*)ShareShmMAP(shmid, NULL, needWritePermission);
    if (NULL == pAddr)
    {
        SAL_ERROR("SHM Get Failed !!!\n");
        return SAL_SHM_ERR_ADDR_NULL;
    }

    pstShmInfo->ID        = ID;
    pstShmInfo->pid       = getpid();
    pstShmInfo->shmSize   = size;
    pstShmInfo->shmID     = shmid;
    pstShmInfo->pShmVaddr = pAddr;

    SAL_INFO("PID %d Cmd Shm ID %d shmid %d Create SUC  %p!!!\n", pstShmInfo->pid, ID, shmid, pAddr);

    return SAL_SHM_OK;
}

/* 断开与共享内存的链接 */
SAL_SHM_RESULT SAL_shmDetach(SAL_SHM_INFO *pstShmInfo)
{
    INT32 status = SAL_SHM_OK;
    INT32 shmID = 0;
    UINT32 id = 0;
    INT32 i = 0;
    UINT32 useCount = 0;
    INT8 rmCMD[256];
    UINT8* pVaddr = NULL;

    if (NULL == pstShmInfo)
    {
        SAL_WARN("input pstShmInfo is NULL !!!\n");
        return SAL_SHM_ERR_NOT_AVAIL;
    }

    if (NULL == pstShmInfo->pShmVaddr)
    {
        SAL_WARN("pstShmInfo->pShmVaddr is NULL !!!\n");
        return SAL_SHM_ERR_NOT_AVAIL;
    }

    pVaddr = pstShmInfo->pShmVaddr;

    shmID = pstShmInfo->shmID;
    id = pstShmInfo->ID;

    if (-1 == shmdt(pVaddr))
    {
        SAL_ERROR("Detach Share Memory Error:%s\n",strerror(errno));
        return SAL_SHM_ERR_PERMISSION_DENIED;
    }

    shmctl(shmID, IPC_RMID, NULL);

    SAL_mutexLock(shmIDMutex);
    for (i = 0; i < MAX_SHM_ID - MIN_SHM_ID; i++)
    {
        if(id == id_array[i].idx)
        {
            id_array[i].idUsed = SAL_FALSE;
            break;
        }
    }
    SAL_mutexUnlock(shmIDMutex);

    if (MAX_SHM_ID - MIN_SHM_ID <= i)
    {
        SAL_ERROR("clean shm ID err, i(%d) record Shm ID(%u) !!!\n", i, id);
        return SAL_SHM_ERR;
    }

    return SAL_SHM_OK;
}

/* 获取可用的共享内存ID */
SAL_SHM_RESULT SAL_shmGetID(UINT32 *pID)
{
    INT32 i = 0;
    INT32 status = SAL_SHM_OK;

    if (NULL == pID)
    {
        SAL_ERROR("input pID is NULL !!!\n");
        return SAL_SHM_ERR_NOT_AVAIL;
    }

    status = ShareShmresourceInit();
    if (SAL_SHM_OK != status)
    {
        SAL_ERROR("SHM Init resource Failed !!!\n");
        return SAL_SHM_ERR;
    }

    SAL_mutexLock(shmIDMutex);
    for (i = 0; i < MAX_SHM_ID - MIN_SHM_ID; i++)
    {
        if (SAL_FALSE == id_array[i].idUsed)
        {
            *pID = id_array[i].idx;
            id_array[i].idUsed = SAL_TRUE;
            break;
        }
    }
    SAL_mutexUnlock(shmIDMutex);
    
    if (MAX_SHM_ID - MIN_SHM_ID <= i)
    {
        SAL_ERROR("not get shm id !!!\n");
        return SAL_SHM_ERR_NOID;
    }

    return SAL_SHM_OK;
}
