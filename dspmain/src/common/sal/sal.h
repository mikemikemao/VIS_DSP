/*******************************************************************************
 * sal.h
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : heshengyuan <heshengyuan@hikvision.com>
 * Version: V1.0.0  2014��4��2�� Create
 *
 * Description : System Abstraction Layer.
 *               ������ϵͳ�����ĺ�����ϵͳAPI ��װ��ͷ�ļ�����������ֻҪ����
 *               ���ͷ�ļ�����ֱ��ʹ�ô󲿷�ϵͳ�����ȡ�
 * Modification:
 *******************************************************************************/
#ifndef _SAL_H_
#define _SAL_H_

/* ========================================================================== */
/*                             ͷ�ļ���                                       */
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

/* ����ͷ�ļ� */
#include "sal_type.h"
#include "sal_log.h"
#include "sal_trace.h"
#include "sal_macro.h"
#include "sal_str.h"


/* ͨ�ù��ܽӿ�ͷ�ļ� */
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

