#ifndef _SAL_UTILS_H_
#define _SAL_UTILS_H_

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include "sal_type.h"
#include <utils/Timers.h>

typedef struct video_fps_info_s
{
    UINT32              mCurFrmCnt;
    UINT32              mLastFrmCnt;
    FLOAT32             mCurFps;
    nsecs_t             mLastFpsTime;
} video_fps_info_t;

VOID dumpVideoFPS(video_fps_info_t *fpsInfo);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SAL_UTILS_H_*/

