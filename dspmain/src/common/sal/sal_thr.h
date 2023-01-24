
#ifndef _SAL_THR_H_
#define _SAL_THR_H_

#ifndef __USE_GNU
#define __USE_GNU		//启用CPU_ZERO等相关的宏
#endif

#include <sched.h>
#include <pthread.h>
#include "sal.h"
#include <sys/time.h>
#include <sys/resource.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SAL_THR_PRI_MAX                 sched_get_priority_max(SCHED_FIFO)
#define SAL_THR_PRI_MIN                 sched_get_priority_min(SCHED_FIFO)

#define SAL_THR_PRI_DEFAULT             ( SAL_THR_PRI_MIN + (SAL_THR_PRI_MAX-SAL_THR_PRI_MIN)/2 )

#define SAL_THR_STACK_SIZE_DEFAULT      0


#define SAL_THR_NORMAL_EXIT             NULL
typedef void*                           SAL_THR_RET_TYPE;

typedef SAL_THR_RET_TYPE (*SAL_ThrEntryFunc)(void *);

typedef struct {
    pthread_t      hndl;
} SAL_ThrHndl;



int SAL_setThrName(const char * thrName);
int SAL_thrCreate(SAL_ThrHndl *hndl, SAL_ThrEntryFunc entryFunc, UINT32 pri, UINT32 stackSize, void *prm);
int SAL_thrDelete(SAL_ThrHndl *hndl);
int SAL_thrJoin(SAL_ThrHndl *hndl);
int SAL_thrChangePri(SAL_ThrHndl *hndl, UINT32 pri);
int SAL_thrExit(void *returnVal);
int SAL_thrBindCore(UINT32 core);
int SAL_thrDetach(SAL_ThrHndl *hndl);

/* 所有线程优先级为120-5=115，SCHED_OTHER模式 */
#define SAL_SET_THR_NAME() \
    {\
    SAL_INFO("%s tsk is running, pid is %ld \n", __FUNCTION__, syscall(SYS_gettid));\
    SAL_setThrName(__FUNCTION__);\
    setpriority(PRIO_PROCESS, 0, -5); \
    }

#ifdef __cplusplus
}
#endif

#endif /* _SAL_THR_H_ */

