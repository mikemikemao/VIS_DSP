#include "sal_thr.h"

#include <errno.h>
#include <string.h>
#include <pthread.h>

#define FUNC_NAME(x)  #x


#define OSA_assert(x)  \
  { \
    if( (x) == 0) { \
      fprintf(stderr, " ASSERT (%s|%s|%d)\r\n", __FILE__, __func__, __LINE__); \
      while (getchar()!='q')  \
        ; \
    } \
  }

int SAL_setThrName(const char * thrName)
{
    /* 只能在当前线程中配置 */
    prctl(PR_SET_NAME, thrName);
    return SAL_SOK;
}


int SAL_thrCreate(SAL_ThrHndl *hndl, SAL_ThrEntryFunc entryFunc, UINT32 pri, UINT32 stackSize, void *prm)
{
    int status = SAL_SOK;
    pthread_attr_t thread_attr;
    struct sched_param schedprm;

    // initialize thread attributes structure
    status = pthread_attr_init(&thread_attr);

    if(status != SAL_SOK)
    {
        SAL_ERROR("SAL_thrCreate() - Could not initialize thread attributes\n");
        return status;
    }

    if(stackSize != SAL_THR_STACK_SIZE_DEFAULT)
    {
        pthread_attr_setstacksize(&thread_attr, stackSize);
    }
    /*
        SCHED_OTHER（正常、非实时）、SCHED_RR（实时、轮转法）和 SCHED_FIFO（实时、先入先出）三种，
        缺省为SCHED_OTHER，后两种调度策略仅对超级用户有效
     */
    /*
        SCHED_FIFO 会导致上下文切换变多，使得系统的 Load average 变高
    */

    //status |= pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED);

    if(pri > SAL_THR_PRI_MAX)
    {
        pri = SAL_THR_PRI_MAX;
    }
    else if(pri < SAL_THR_PRI_MIN)
    {
        pri = SAL_THR_PRI_MIN;
    }

    /* 需要高优先级的线程，配置为 SCHED_FIFO */
    if (SAL_THR_PRI_MAX == pri)
    {
        status |= pthread_attr_setschedpolicy(&thread_attr, SCHED_FIFO);
        if(status != SAL_SOK)
        {
            SAL_ERROR("SAL_thrCreate() - Could not setschedpolicy \n");
            OSA_assert(status == SAL_SOK);
        }

        schedprm.sched_priority = SAL_THR_PRI_MAX;
        status |= pthread_attr_setschedparam(&thread_attr, &schedprm);
        if(status!=SAL_SOK)
        {
            SAL_ERROR("SAL_thrCreate() - Could not initialize thread attributes\n");
            goto error_exit;
        }
    }
    else
    {
        status |= pthread_attr_setschedpolicy(&thread_attr, SCHED_OTHER);
        if(status != SAL_SOK)
        {
            SAL_ERROR("SAL_thrCreate() - Could not setschedpolicy \n");
            OSA_assert(status == SAL_SOK);
        }
    }

    status = pthread_create(&hndl->hndl, &thread_attr, entryFunc, prm);
    if(status != SAL_SOK)
    {
        SAL_ERROR("SAL_thrCreate() - Could not create thread \n");
        OSA_assert(status == SAL_SOK);
    }

error_exit:
    pthread_attr_destroy(&thread_attr);

//    if (SAL_FAIL == SAL_memListAdd(hndl->hndl, 1, "Sal_Thr"))
//    {
//        SAL_ERROR("SAL_memListAdd error\n");
//    }

    return status;
}

int SAL_thrJoin(SAL_ThrHndl *hndl)
{
    int status=SAL_SOK;
    void *returnVal;

    status = pthread_join(hndl->hndl, &returnVal);

//    if(SAL_FAIL == SAL_memListDel(hndl->hndl))
//    {
//        SAL_ERROR("SAL_memListDel fail!");
//    }

    return status;
}

int SAL_thrDelete(SAL_ThrHndl *hndl)
{
    int status = SAL_SOK;

    if(hndl != NULL)
    {
        /* android bionic doesn't have pthread_cancel */
        //status = pthread_cancel(hndl->hndl);
        status = SAL_thrJoin(hndl);
    }

    return status;
}

int SAL_thrChangePri(SAL_ThrHndl *hndl, UINT32 pri)
{
    int status = SAL_SOK;
    struct sched_param schedprm;

    if(pri>SAL_THR_PRI_MAX)
    {
        pri=SAL_THR_PRI_MAX;
    }
    else if(pri<SAL_THR_PRI_MIN)
    {
        pri=SAL_THR_PRI_MIN;
    }

    schedprm.sched_priority = pri;
    status |= pthread_setschedparam(hndl->hndl, SCHED_FIFO, &schedprm);

    return status;
}

int SAL_thrExit(void *returnVal)
{
    pthread_exit(returnVal);
    return SAL_SOK;
}

int SAL_thrBindCore(UINT32 core)
{
    INT32L     coreNum = sysconf(_SC_NPROCESSORS_ONLN);
    cpu_set_t cpuMask;

    if (core >= (UINT32)coreNum)
    {
        SAL_ERROR("!!!Bind core(%u) not support, total coreNum: %ld\n", core, coreNum);
        return SAL_FAIL;
    }

    CPU_ZERO(&cpuMask);
    CPU_SET(core, &cpuMask);

    if (SAL_SOK != sched_setaffinity(gettid(), sizeof(cpu_set_t), &cpuMask))
    {
        SAL_ERROR("!!!Bind core(%u) error \n", core);
        return SAL_FAIL;
    }

    return SAL_SOK;
}

int SAL_thrDetach(SAL_ThrHndl *hndl)
{
    pthread_detach(hndl->hndl);
    return  SAL_thrJoin(hndl);
}
