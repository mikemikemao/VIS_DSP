/*******************************************************************************
 * sal.h
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : heshengyuan <heshengyuan@hikvision.com>
 * Version: V1.0.0  2014年4月2日 Create
 *
 * Description : System Abstraction Layer.
 *               包含对系统抽象层的函数、系统API 封装的头文件，程序、驱动只要包含
 *               这个头文件即可直接使用大部分系统函数等。
 * Modification:
 *******************************************************************************/
#ifndef _SAL_H_
#define _SAL_H_

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <assert.h>
#include <sys/syscall.h>
#include <sys/prctl.h>

/* 基本头文件 */
#include "sal_type.h"
#include "sal_log.h"
#include "sal_trace.h"
#include "sal_macro.h"
#include "sal_str.h"


/* 通用功能接口头文件 */
#include "sal_list.h"
#include "sal_que.h"
#include "sal_sem.h"
#include "sal_shm.h"
#include "sal_thr.h"
#include "sal_time.h"
#include "sal_mem.h"
#include "sal_mutex.h"
#include "sal_video.h"
#include "sal_audio.h"
#include "sal_utils.h"
#include "sal_json.h"
#include "sal_contentmessage.h"
#include "sal_file.h"
#include "sal_bmp.h"

#endif

