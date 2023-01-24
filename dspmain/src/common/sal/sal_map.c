/*******************************************************************************
 * sal_map.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : hsy <heshengyuan@hikvision.com>
 * Version: V1.0.0  2022年6月20日 Create
 *
 * Description : 
 * Modification: 
 *******************************************************************************/

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <sys/mman.h>
#include "sal.h"
#include "sal_map.h"

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */


/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */
static UINT32 mapShareTotalLen  = 0;

/* ========================================================================== */
/*                          函数定义区                                        */
/* ========================================================================== */


/*******************************************************************************
* 函数名  : SAL_mmapFileInit
* 描  述  : 封装通过文件方式做共享内存映射的通用函数
* 输  入  : - pFileName: 文件路径, 该文件直接属于内存空间上，通讯效率和同步会快
*         : - role     : 服务端还是客户端，区别只是初始化时是否清空文件内容，其他
                         无差别
*         : - size     : 映射大小，也是共享内存的大小
* 输  出  : 无
* 返回值  : NULL       : 失败，返回映射成功的整片内存的首地址
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

    /* 服务端映射文件会清空，客户端直接映射即可不需要额外操作 */
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
* 函数名  : SAL_mmapFileDeinit
* 描  述  : 是否文件映射共享内存
* 输  入  : - pVaddr: 释放地址，需要使用虚拟地址
*         : - size  : 内存长度
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
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

