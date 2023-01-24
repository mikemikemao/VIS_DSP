/*******************************************************************************
 * sal_file.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : chenying21 <chenying21@hikvision.com>
 * Version: V1.0.0  2019��1��24�� Create
 *
 * Description :��д��Ƶ�ļ��� ���ڷ����ȶ�
 * Modification: 
 *******************************************************************************/
 
/* ========================================================================== */
/*                             ͷ�ļ���                                       */
/* ========================================================================== */
#include <sal.h>
#include <sal_file.h>
#include <sys/stat.h>

/* ========================================================================== */
/*                           ������Ͷ�����                                   */
/* ========================================================================== */


/* ========================================================================== */
/*                          ���ݽṹ������                                    */
/* ========================================================================== */



/* ========================================================================== */
/*                            ȫ�ֱ���������                                  */
/* ========================================================================== */



/* ========================================================================== */
/*                          ����������                                        */
/* ========================================================================== */
/*******************************************************************************
* ������  : SAL_MkDir
* ��  ��  : �����ļ���
* ��  ��  : path ������·��
* ��  ��  : ��
* ����ֵ  : SAL_SOK  �ɹ�
*         SAL_FAIL ʧ��
*******************************************************************************/
INT32 SAL_MkDir(char *path)
{
    INT32 ret;

    if(NULL == path)
    {
        SAL_ERROR("path: %p !!!\n", path);
        return SAL_FAIL;
    }
    
    /* �ж��Ƿ�����������·��Ŀ���һ�����ļ��� */   
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
* ������  : SAL_WriteFile
* ��  ��  : �ú���������linuxд���ݵ��ļ���
*           ͬ���ļ�����׷�ӱ���
* ��  ��  : - name:  д����ļ���
*                  - buf: ��д������ݵ�ַ
*                  - len: ��д������ݳ���
* ��  ��  : �ޡ�
* ����ֵ  : SAL_SOK: д��ɹ�
*                     SAL_FAIL: д��ʧ��
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
* ������  : SAL_WriteFileWb
* ��  ��  : �ú���������linuxд���ݵ��ļ���
*           �ļ�����
* ��  ��  : - name:  д����ļ���
*                  - buf: ��д������ݵ�ַ
*                  - len: ��д������ݳ���
* ��  ��  : �ޡ�
* ����ֵ  : SAL_SOK: д��ɹ�
*                     SAL_FAIL: д��ʧ��
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
* ������  : SAL_ReadFile
* ��  ��  : �ú��������ڶ��ļ����ݵ������ڴ���
*
* ��  ��  : - name:  ������ļ���
*                  - buf: Ҫд������ݵ�ַ
*                  - len: Ҫд������ݳ���
* ��  ��  : �ޡ�
* ����ֵ  : SAL_SOK: �����ɹ�
*                     SAL_FAIL: ����ʧ��
*******************************************************************************/
INT32 SAL_ReadFile(const INT8*name, INT8*buf, UINT32 len)
{
    FILE   *fd   = NULL;
    INT32  ret   = 0;
    INT32  fsize = 0;
    struct stat fprop;  /* �ļ����� */

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
* ������  : SAL_fileWrite
* ��  ��  : д�ļ�
*
* ��  ��  : - data : ��д�����ݵ�ַָ��
            - size : ��ÿ�ζ�ȡ���ֽ���
            - nmemb: ��ȡ�Ĵ���
            - file : Ŀ���ļ�
* ��  ��  : �ޡ�
* ����ֵ  : 
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
* ������  : SAL_fileWrite
* ��  ��  : ���ļ�
*
* ��  ��  : - data : ��ȡ�����ݴ�ŵ��ڴ��ָ��
            - size : ��ÿ�ζ�ȡ���ֽ���
            - nmemb: ��ȡ�Ĵ���
            - file : ����ȡ�ļ�

* ��  ��  : �ޡ�
* ����ֵ  : 
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
* ������  : SAL_dumpDataToFile
* ��  ��  :  �������ݵ��ļ�
* ��  ��  : - data      ����������
            - dataLen   ���ݳ���
            - filepath  ����·��
* ��  ��  : 
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
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
* ������  : SAL_fileSaveSP420ToPlanar
* ��  ��  : ����YUV����(SP420 -> Planar)
*
* ��  ��  : - pVBuf : yuv�����ڴ��ַ
            - width : ͼƬ��
            - height: ͼƬ��
            - stride: ͼƬ���
            - pfd   : ����д��Ŀ���ļ�

* ��  ��  : �ޡ�
* ����ֵ  : 
*******************************************************************************/
INT32 SAL_fileSaveSP420ToPlanar(INT8 *pVBuf, INT32 width, INT32 height, INT32 *stride, FILE *pfd)
{
    UINT32 w, h;
    INT8 *pVBuf_Y;
    INT8 *pVBuf_C;
    INT8 *pMemContent;
    INT8 TmpBuff[2000];                //������ֵ̫С��ͼ��ܴ�Ļ��治��

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
* ������  : SAL_dumpInfoToFile
* ��  ��  :  ������Ϣ���ļ�
* ��  ��  : - filepath  ����·��
        : - pFmt  ��ʽ
* ��  ��  : 
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
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


