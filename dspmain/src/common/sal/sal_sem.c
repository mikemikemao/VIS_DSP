/*******************************************************************
*
*文件说明:
*    本文件封装对linux下IPC的信号量的操作
*    王伟琴
*    2015.06.01
*
********************************************************************/
#if 0
#include "sal.h"
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun
{
    INT32                   val;
    struct semid_ds        *buf;
    unsigned short int     *array;
    struct seminfo         *__buf;
};

/* 设置信号量的值 */
int SAL_semSetValue(int iSemid, int iVal)
{
    union semun sem_union;
    sem_union.val = iVal;

    if (-1 == semctl(iSemid, 0, SETVAL, sem_union))
    {
        perror("semctl(SETVAL) error:");
        return SAL_FAIL;
    }

    return SAL_SOK;
}

/* 获取信号量的值 */
INT32 SAL_semGetValue(INT32 semid)
{
    union semun sem_union;

    if (-1 == semctl(semid, 0, GETVAL, sem_union))
    {
        SAL_ERROR("semctl(GETVAL) error\n");
        return SAL_FAIL;
    }

    return sem_union.val;
}

/* 删除信号量  */
INT32 SAL_semDel(INT32 semid)
{
    if(-1 == semctl(semid, 0, IPC_RMID, 0))
    {
        SAL_ERROR("semctl(IPC_RMID) error\n");
        return SAL_FAIL;
    }
    return SAL_SOK;
}

/* 信号量P操作：对信号量进行减一操作 */
int SAL_semP(int iSemid)
{
    struct sembuf stSemBuf;

    stSemBuf.sem_num    = 0;            /*信号量编号 */
    stSemBuf.sem_op     = -1;           /*P操作  */
    stSemBuf.sem_flg    = SEM_UNDO;

    if(-1 == semop(iSemid, &stSemBuf, 1))
    {
        perror("semctl(semop P) error:");
        return SAL_FAIL;
    }

    return SAL_SOK;
}

/* 信号量V操作：对信号量进行加一操作 */
int SAL_semV(int iSemid)
{
    struct sembuf stSemBuf;

    stSemBuf.sem_num    = 0;            /*信号量编号 */
    stSemBuf.sem_op     = 1;            /*V操作   */
    stSemBuf.sem_flg    = SEM_UNDO;

    if(-1 == semop(iSemid, &stSemBuf, 1))
    {
        perror("semctl(semop V) error:");
        return SAL_FAIL;
    }

    return SAL_SOK;
}


/* 创建信号量 */
int SAL_semCreate(key_t key,int nsems,int semflg)
{
    int iSemid;

    iSemid = semget(key, nsems, semflg);
    if (-1 == iSemid)
    {
        if (EEXIST == errno)
        {
            return SAL_FAIL;
        }
        perror("semget error:");
        return SAL_FAIL;
    }

    return iSemid;
}

#endif
