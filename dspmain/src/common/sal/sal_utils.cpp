#include <sal_utils.h>

VOID dumpVideoFPS(video_fps_info_t *fpsInfo)
{
    nsecs_t now, diff;

    if (fpsInfo->mCurFrmCnt == 0)
    {
        fpsInfo->mLastFpsTime = systemTime(SYSTEM_TIME_MONOTONIC);
    }

    fpsInfo->mCurFrmCnt++;

    if (!(fpsInfo->mCurFrmCnt % 100))
    {
        now = systemTime(SYSTEM_TIME_MONOTONIC);
        diff = now - fpsInfo->mLastFpsTime;
        if (diff != 0)
        {
            fpsInfo->mCurFps = ((fpsInfo->mCurFrmCnt - fpsInfo->mLastFrmCnt) * s2ns(1)) * 1.0 / diff;
        }
        fpsInfo->mLastFpsTime = now;
        fpsInfo->mLastFrmCnt  = fpsInfo->mCurFrmCnt;
    }
}

