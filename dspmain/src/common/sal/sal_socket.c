/*******************************************************************************
 * sal_socket.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : hsy <heshengyuan@hikvision.com>
 * Version: V1.0.0  2020年9月27日 Create
 *
 * Description : 
 * Modification: 
 *******************************************************************************/

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <sal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/select.h>
#include "sal_socket.h"
#include "sal_macro.h"

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */


/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */


/* ========================================================================== */
/*                          函数定义区                                        */
/* ========================================================================== */
/*******************************************************************************
* 函数名  : SAL_socketOpen
* 描  述  : 通过指定的 URL 字符串地址，建立SOCKET通讯
* 输  入  : - socketFd: 申请到的套接字句柄
*         : - url     : 该字符串可以是路径或者网络地址
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_socketOpen(INT32 *pSocketFd, const char *pUrl)
{
    INT32                   fd          =0;
    struct sockaddr_un      un;

    if (pUrl == NULL)
    {
        SAL_ERROR("Input parms NULL !\n");
        return SAL_FAIL;
    }

    fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (0 > fd)
    {
        SAL_ERROR("socket error! %s\n", strerror(errno));
        return SAL_FAIL;
    }
    
    unlink(pUrl);

    SAL_memSetSafe(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    if(strlen(pUrl) > sizeof(un.sun_path))
    {
        SAL_socketClose(fd);
        return SAL_FAIL;
    }
    SAL_strnCpySafe(un.sun_path, sizeof(un.sun_path),pUrl, strlen(pUrl));
    if (-1 == bind(fd, (const struct sockaddr*)&un, sizeof(un)))
    {
        SAL_ERROR("bind error! %s\n", strerror(errno));
        SAL_socketClose(fd);
        return SAL_FAIL;
    }

    *pSocketFd = fd;
    SAL_DEBUG("phSocket= %d\n", *pSocketFd);
    
    return SAL_SOK;
}

/*******************************************************************************
* 函数名  : SAL_socketClose
* 描  述  : 销毁套接字
* 输  入  : - hSocket:
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_socketClose(INT32 socketFd)
{
    SAL_DEBUG("socketFd= %d\n", socketFd);

    if (socketFd <= 0)
    {
        SAL_ERROR("socket fd(%d) error!\n", socketFd);
        return SAL_FAIL;
    }

    if (-1 == shutdown(socketFd, SHUT_RDWR))
    {
        SAL_ERROR("shutdown fd(%d) error! %s\n", socketFd, strerror(errno));
        return SAL_FAIL;
    }
    
    if (-1 == close(socketFd))
    {
        SAL_ERROR("close fd(%d) error! %s\n", socketFd, strerror(errno));
        return SAL_FAIL;
    }

    return SAL_SOK;
}

/*******************************************************************************
* 函数名  : SAL_socketSend
* 描  述  : 主动发送消息, 发送数据所需的 URL 可以指定是否使用新的预套接字
* 输  入  : - hSocket: 套接字句柄
*         : - pNewUrl: 如果需要，请在这里指定新的预套接字
*         : - pData  : 数据内容
*         : - dataLen: 长度
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_socketSend(INT32 socketFd, const char *pUrl, const void *pData, INT32 dataLen)
{
    struct sockaddr_un      remoteUn;
    INT32                   addrLen     = sizeof(remoteUn);

    if ((socketFd < 0) || (NULL == pUrl))
    {
        SAL_ERROR("socket fd(%d) error!\n", socketFd);
        return SAL_FAIL;
    }
    
    SAL_DEBUG("socketFd= %d url=%s\n", socketFd, pUrl);
    
    SAL_memSetSafe(&remoteUn, 0, sizeof(remoteUn));
    remoteUn.sun_family = AF_UNIX;
    if(strlen(pUrl) > sizeof(remoteUn.sun_path))
        return -1;
    SAL_strnCpySafe(remoteUn.sun_path, sizeof(remoteUn.sun_path),pUrl, strlen(pUrl));
    
    if (-1 == sendto(socketFd, pData, dataLen, 0, (const struct sockaddr*)&remoteUn, addrLen))
    {
        SAL_ERROR("socketFd= %d send to %s error! %s\n", socketFd, pUrl, strerror(errno));
        return SAL_FAIL;
    }

    return SAL_SOK;
}


INT32 SAL_socketRecv(INT32 socketFd, void *pData, INT32 *pDataLen, char *pRemoteUrl)
{
    INT32                   ret         = SAL_SOK;
    struct sockaddr_un      remoteUn;
    INT32                   addrLen     = sizeof(remoteUn);
    INT32                   dataLen     = *pDataLen;

    if (socketFd < 0)
    {
        SAL_ERROR("socket fd(%d) error!\n", socketFd);
        return SAL_FAIL;
    }

    SAL_DEBUG("socketFd= %d dataLen=%d\n", socketFd, dataLen);
    
    SAL_memSetSafe(&remoteUn, 0, sizeof(remoteUn));
    
    ret= recvfrom(socketFd, pData, dataLen, 0, (struct sockaddr*)&remoteUn, (socklen_t*)&addrLen);
    if( (-1 == ret) || (0 == ret) )
    {
        SAL_ERROR("recv error! %s\n", strerror(errno));
        return SAL_FAIL;
    }
    else
    {
        *pDataLen = ret;
    }

    if(NULL != pRemoteUrl)
    {
        SAL_strCpySafe(pRemoteUrl, 2014, remoteUn.sun_path);
    }
    return SAL_SOK;
}



