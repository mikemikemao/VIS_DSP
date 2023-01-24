/*******************************************************************************
 * sal_file.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : chenying21 <chenying21@hikvision.com>
 * Version: V1.0.0  2019年1月24日 Create
 *
 * Description :读写视频文件， 用于分析比对
 * Modification: 
 *******************************************************************************/
 
/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <sal.h>
#include <sal_file.h>
#include <sys/stat.h>

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */


/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */



/* ========================================================================== */
/*                            全局变量定义区                                  */
/* ========================================================================== */



/* ========================================================================== */
/*                          函数定义区                                        */
/* ========================================================================== */
/*******************************************************************************
* 函数名  : SAL_MkDir
* 描  述  : 创建文件夹
* 输  入  : path ：保存路径
* 输  出  : 无
* 返回值  : SAL_SOK  成功
*         SAL_FAIL 失败
*******************************************************************************/
INT32 SAL_MkDir(char *path)
{
    INT32 ret;

    if(NULL == path)
    {
        SAL_ERROR("path: %p !!!\n", path);
        return SAL_FAIL;
    }
    
    /* 判断是否存在名字与下发的卡号一样的文件夹 */   
    mode_t mode = umask(0);
    ret = mkdir(path, 0777);
    if(ret < 0)
    {
        if(17 == errno)
        {
            SAL_WARN("%s exit !!\n", path);
            return SAL_SOK;
        }
        else
        {
            SAL_ERROR("mkdir failed,err:%d !!!!\n", errno);            
            return SAL_FAIL;
        }
    }

    umask(mode);

    return SAL_SOK;
}

/*******************************************************************************
* 函数名  : SAL_WriteFile
* 描  述  : 该函数负责在linux写数据到文件中
*           同名文件允许追加保存
* 输  入  : - name:  写入的文件名
*                  - buf: 被写入的数据地址
*                  - len: 被写入的数据长度
* 输  出  : 无。
* 返回值  : SAL_SOK: 写入成功
*                     SAL_FAIL: 写入失败
*******************************************************************************/
INT32 SAL_WriteFile(const INT8*name, INT8*buf, UINT32 len)
{
    INT32 ret = 0;
    FILE* fd = NULL;

    if ((NULL == name) || (NULL == buf) || (len <= 0))
    {
        SAL_ERROR("input parameter is NULL!name = %p, buf = %p, len = %d\n", name, buf, len);
        return SAL_FAIL;
    }

    fd = fopen(name, "ab+");
    if (NULL == fd)
    {
        SAL_ERROR("open file fail! %s\n",name);
        SAL_ERROR("errno:%d,error:%s\n",errno,strerror(errno));
        return SAL_FAIL;
    }

    ret = fwrite(buf, len, 1, fd);
    if (ret != 1)
    {
        SAL_ERROR("open file fail!\n");
        SAL_ERROR("errno:%d,error:%s\n",errno,strerror(errno));
        fclose(fd);
        return SAL_FAIL;
    }

    ret = fflush(fd);
    if (ret != 0)
    {
        SAL_ERROR("fflush file fail!\n");
        SAL_ERROR("errno:%d,error:%s\n",errno,strerror(errno));
        fclose(fd);
        return SAL_FAIL;
    }

    fclose(fd);

    return SAL_SOK;
}

/*******************************************************************************
* 函数名  : SAL_WriteFileWb
* 描  述  : 该函数负责在linux写数据到文件中
*           文件覆盖
* 输  入  : - name:  写入的文件名
*                  - buf: 被写入的数据地址
*                  - len: 被写入的数据长度
* 输  出  : 无。
* 返回值  : SAL_SOK: 写入成功
*                     SAL_FAIL: 写入失败
*******************************************************************************/
INT32 SAL_WriteFileWb(const INT8*name, INT8*buf, UINT32 len)
{
    INT32 ret = 0;
    FILE* fd = NULL;

    if ((NULL == name) || (NULL == buf) || (len <= 0))
    {
        SAL_ERROR("input parameter is NULL!name = %p, buf = %p, len = %d\n", name, buf, len);
        return SAL_FAIL;
    }

    fd = fopen(name, "wb");
    if (NULL == fd)
    {
        SAL_ERROR("open file fail!\n");
        return SAL_FAIL;
    }

    ret = fwrite(buf, len, 1, fd);
    if (ret != 1)
    {
        SAL_ERROR("open file fail!\n");
        fclose(fd);
        return SAL_FAIL;
    }

    ret = fflush(fd);
    if (ret != 0)
    {
        SAL_ERROR("fflush file fail!\n");
        fclose(fd);
        return SAL_FAIL;
    }

    fclose(fd);

    return SAL_SOK;
}

/*******************************************************************************
* 函数名  : SAL_ReadFile
* 描  述  : 该函数负责在读文件数据到程序内存中
*
* 输  入  : - name:  读入的文件名
*                  - buf: 要写入的数据地址
*                  - len: 要写入的数据长度
* 输  出  : 无。
* 返回值  : SAL_SOK: 读出成功
*                     SAL_FAIL: 读出失败
*******************************************************************************/
INT32 SAL_ReadFile(const INT8*name, INT8*buf, UINT32 len)
{
    FILE   *fd   = NULL;
    INT32  ret   = 0;
    INT32  fsize = 0;
    struct stat fprop;  /* 文件属性 */

    if (NULL == name || NULL == buf || 0 == len)
    {
        SAL_ERROR("!!!Input params error, path(%p), pdata(%p), len(%u) \n", name, buf, len);
        return SAL_FAIL;
    }

    fd = fopen(name, "rb");
    if (NULL == fd)
    {
        SAL_ERROR("!!!open file(%s) fail \n", name);
        return SAL_FAIL;
    }

    ret = stat(name, &fprop);
    if (0 != ret || fprop.st_size <= 0 || len < fprop.st_size)
    {
        SAL_ERROR("stat path(%s) fail, file size(%u), buf size(%u)!!!\n", name, (unsigned int)fprop.st_size, len);
        fclose(fd);
        fd = NULL;
        return SAL_FAIL;
    }

    fsize = fread(buf, 1, fprop.st_size, fd);
    fclose(fd);
    fd = NULL;

    return fsize;
}

/*******************************************************************************
* 函数名  : SAL_fileWrite
* 描  述  : 写文件
*
* 输  入  : - data : 待写入数据地址指针
            - size : 是每次读取的字节数
            - nmemb: 读取的次数
            - file : 目标文件
* 输  出  : 无。
* 返回值  : 
*******************************************************************************/
INT32 SAL_fileWrite(INT8* data, INT32 size, INT32 nmemb, FILE* file)
{
    INT32 fsize;
    if(SAL_isNull(file) || SAL_isNull(data))
    {
        return SAL_FAIL;
    }

    fsize = fwrite((void *)data, size, nmemb, file);
    fflush(file);
    
    return fsize;
}

/*******************************************************************************
* 函数名  : SAL_fileWrite
* 描  述  : 读文件
*
* 输  入  : - data : 读取的数据存放的内存的指针
            - size : 是每次读取的字节数
            - nmemb: 读取的次数
            - file : 待读取文件

* 输  出  : 无。
* 返回值  : 
*******************************************************************************/
INT32 SAL_fileRead(INT32* data, INT32 size, INT32 nmemb, FILE* file)
{    
    INT32 fsize;
    if(SAL_isNull(file) || SAL_isNull(data))
    {
        return SAL_FAIL;
    }
    
    fsize = fread(data, size, nmemb, file);
    fflush(file);
    
    return fsize;
}

/*******************************************************************************
* 函数名  : SAL_dumpDataToFile
* 描  述  :  保存数据到文件
* 输  入  : - data      待保存数据
            - dataLen   数据长度
            - filepath  保存路径
* 输  出  : 
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_dumpDataToFile(UINT8 *data, UINT32 dataLen, UINT8 *filePath)
{
    FILE * fileFd;

    if(SAL_isNull(data) || SAL_isNull(filePath))
    {
        SAL_ERROR("data or filePath is NULL,data:%p,filePath:%p \n",data,filePath);
        return SAL_FAIL;
    }
    
    fileFd = fopen((const char *)filePath, "ab");
    if(SAL_isNull(fileFd))
    {
        SAL_ERROR("open file failed:%s!\n", strerror(errno));
        return SAL_FAIL;
    }
    else
    {
        SAL_fileWrite((INT8 *)data, dataLen, 1, fileFd);
        fclose(fileFd);
    }
    return SAL_SOK;
}

/*******************************************************************************
* 函数名  : SAL_fileSaveSP420ToPlanar
* 描  述  : 保存YUV数据(SP420 -> Planar)
*
* 输  入  : - pVBuf : yuv数据内存地址
            - width : 图片宽
            - height: 图片高
            - stride: 图片跨距
            - pfd   : 数据写入目标文件

* 输  出  : 无。
* 返回值  : 
*******************************************************************************/
INT32 SAL_fileSaveSP420ToPlanar(INT8 *pVBuf, INT32 width, INT32 height, INT32 *stride, FILE *pfd)
{
    UINT32 w, h;
    INT8 *pVBuf_Y;
    INT8 *pVBuf_C;
    INT8 *pMemContent;
    INT8 TmpBuff[2000];                //如果这个值太小，图像很大的话存不了

    if(SAL_isNull(pVBuf) || SAL_isNull(pfd))
    {
        SAL_ERROR("input null!\n");
        return SAL_FAIL;
    }

    pVBuf_Y = pVBuf; 
    pVBuf_C = pVBuf_Y + (stride[0])*(height);
    
    /* save Y ----------------------------------------------------------------*/
    for(h=0; h<height; h++)
    {
        pMemContent = pVBuf_Y + h*stride[0];
        SAL_fileWrite(pMemContent, width, 1, pfd);
    }    

    /* save U ----------------------------------------------------------------*/
    for(h=0; h<height/2; h++)
    {
        pMemContent = pVBuf_C + h*stride[1];

        pMemContent += 1;

        for(w=0; w<width/2; w++)
        {
            TmpBuff[w] = *pMemContent;
            pMemContent += 2;
        }
        SAL_fileWrite(TmpBuff, width/2, 1, pfd);
    }
    
    /* save V ----------------------------------------------------------------*/
    for(h=0; h<height/2; h++)    
    {
        pMemContent = pVBuf_C + h*stride[1];

        for(w=0; w<width/2; w++)
        {
            TmpBuff[w] = *pMemContent;
            pMemContent += 2;
        }
        SAL_fileWrite(TmpBuff, width/2, 1, pfd);
    }

    return SAL_SOK;
}

/*******************************************************************************
* 函数名  : SAL_dumpInfoToFile
* 描  述  :  保存信息到文件
* 输  入  : - filepath  保存路径
        : - pFmt  格式
* 输  出  : 
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_dumpInfoToFile(const INT8 *filePath, const INT8 *pFmt, ...)
{
    FILE * fileFd;
    va_list args;

    if(SAL_isNull(filePath))
    {
        SAL_ERROR("filePath is NULL \n");
        return SAL_FAIL;
    }
    
    if(SAL_isNull(pFmt))
    {
        SAL_ERROR("data is NULL \n");
        return SAL_FAIL;
    }
    
    fileFd = fopen(filePath, "ab");
    if(SAL_isNull(fileFd))
    {
        SAL_ERROR("open file failed:%s!\n", strerror(errno));
        return SAL_FAIL;
    }
    else
    {
        va_start(args, pFmt);
        vfprintf(fileFd, pFmt, args);
        va_end(args);
        fclose(fileFd);
    }
    
    return SAL_SOK;
}


