/*******************************************************************************
 * sal_memList.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : huangshuxin <huangshuxin@hikvision.com>
 * Version: V1.0.0  2019��12��16�� Create
 *
 * Description : 
 * Modification: 
 *******************************************************************************/



/* ========================================================================== */
/*                             ͷ�ļ���                                       */
/* ========================================================================== */
#include <sal.h>
#include <sal_memList.h>

/* ========================================================================== */
/*                           ������Ͷ�����                                   */
/* ========================================================================== */


/* ========================================================================== */
/*                          ���ݽṹ������                                    */
/* ========================================================================== */

List_head *g_pstMemList = NULL;
static INT32 g_listMemSize = 0;
static pthread_mutex_t g_stMemListlock;

/* ========================================================================== */
/*                          ����������                                        */
/* ========================================================================== */

/*******************************************************************************
* ������  : SAL_memList
* ��  ��  : ��ȡ�ڴ��������
* ��  ��  : - VOID:
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
List_head *SAL_memList(VOID)
{
    return g_pstMemList;
} 
/*******************************************************************************
* ������  : SAL_memSetListHead
* ��  ��  : ��������ͷ���
* ��  ��  : - VOID:
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
void SAL_memSetListHead(List_head * pListHead)
{
    g_pstMemList = pListHead;
} 

/*******************************************************************************
* ������  : SAL_memListSortSize
* ��  ��  :���ڴ������Ա��������
* ��  ��  : 
* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*             HIK_FAIL : ʧ��
*******************************************************************************/
UINT32 SAL_memListSortSize (List_head* p1,List_head* p2)
{
    TSAL_ListMemHead *p1List = NULL;
    TSAL_ListMemHead *p2List = NULL;
    p1List = list_entry(p1,TSAL_ListMemHead, salList);
    p2List = list_entry(p2,TSAL_ListMemHead, salList);
    if(p1List->salListHead.size < p2List->salListHead.size)
    {
        return SAL_SOK;
    }
    return SAL_FAIL;
}
/*******************************************************************************
* ������    : SAL_memListSortName
* ��    ��    :���ڴ�����name��������
* ��    ��    : 
* ��    ��    : ��
* ����ֵ    : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
 UINT32 SAL_memListSortName(List_head* p1,List_head* p2)
 {
     TSAL_ListMemHead *p1List = NULL;
     TSAL_ListMemHead *p2List = NULL;
     p1List = list_entry(p1,TSAL_ListMemHead, salList);
     p2List = list_entry(p2,TSAL_ListMemHead, salList);
     if(0 <strcmp(p1List->modelName,p2List->modelName))
     {
         return SAL_SOK;
     }
     return SAL_FAIL;
 }
 
/*******************************************************************************
* ������  : SAL_memListGetNode
* ��  ��  : �ҵ�ɾ���ڴ������Ա
* ��  ��  : - pBufBase: Ҫɾ���Ľڵ�
* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
TSAL_ListMemHead *SAL_memListGetNode(SAL_MemBlockObject *pBufHead)
{
    TSAL_ListMemHead *ptCur = NULL;
    List_head *ptList = NULL;

    if (NULL == pBufHead)
    {
        SAL_ERROR("pBufBase is NULL\n");
        return NULL;
    }
    
    ptList = SAL_memList();

    if (ptList)
    {
        do {
            ptCur = list_entry(ptList,TSAL_ListMemHead, salList);
            if (ptCur->salListHead.pBufBase == pBufHead->pBufBase)
            {
                return ptCur;
            }
            else
            {
                ptList = ptList->next;
            }
        }while (ptList);
    }
    else
    {
        SAL_WARN("Mem List is NULL\n");
    }

    return NULL;
}

/*******************************************************************************
* ������  : SAL_memListAdd
* ��  ��  : �������ӽڵ�
* ��  ��  : - pBufHead: 
*           - pBufEnd: 
*           - size��
* ��  ��  : �ޡ�
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_memListAdd(SAL_MemBlockObject *pBufHead, SAL_MemBlockObject *pBufEnd,  UINT32 size, const char *modId)
{
    TSAL_ListMemHead *ptNew = NULL;

    if ((NULL == pBufHead) || (NULL == pBufEnd) || (NULL == modId))
    {
        SAL_ERROR("SAL_memListAddNode ERROR\n");
        return SAL_FAIL;
    }

    ptNew = malloc(sizeof(TSAL_ListMemHead));
    if(NULL == ptNew)
    {
        SAL_ERROR("alloc failed\n");
        return SAL_FAIL;
    }

    SAL_memSetSafe(ptNew,0x00,sizeof(TSAL_ListMemHead));

    ptNew->salListHead.nMgicNum = pBufHead->nMgicNum;
    ptNew->salListHead.size     = pBufHead->size;   
    ptNew->salListHead.pBufBase = pBufHead->pBufBase;

    ptNew->salListEnd.nMgicNum  = pBufEnd->nMgicNum;
    ptNew->salListEnd.size      = pBufEnd->size;
    ptNew->salListEnd.pBufBase  = pBufEnd->pBufBase;

    SAL_strCpySafe(ptNew->modelName, sizeof(ptNew->modelName)-1, modId);

    ptNew->salList.pre  = NULL;
    ptNew->salList.next = NULL;

    if (NULL == SAL_memList())
    {   
        pthread_mutex_init(&g_stMemListlock, NULL);
        pthread_mutex_lock(&g_stMemListlock);
        SAL_memSetListHead(&ptNew->salList);
        g_listMemSize += sizeof(TSAL_ListMemHead);
        pthread_mutex_unlock(&g_stMemListlock);

        return SAL_SOK;
    }

    pthread_mutex_lock(&g_stMemListlock);
    if (SAL_FAIL == SAL_memListAddNode(&ptNew->salList,SAL_memList()))
    {
        pthread_mutex_unlock(&g_stMemListlock);
        SAL_ERROR("SAL_memListAddNode ERROR\n");
        return SAL_FAIL;
    }

    g_listMemSize += sizeof(TSAL_ListMemHead);

    pthread_mutex_unlock(&g_stMemListlock);

    return SAL_SOK;
}

/*******************************************************************************
* ������  : SAL_memListDel
* ��  ��  : ɾ���ڵ�
* ��  ��  : - pBufHead: 
* ��  ��  : �ޡ�
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_memListDel(SAL_MemBlockObject *pBufHead)
{
    INT32 ret = SAL_SOK;
    TSAL_ListMemHead *ptFind = NULL;

    if (NULL == pBufHead)
    {
        SAL_ERROR("pBufHead is NULL\n");
        return SAL_FAIL;
    }

    pthread_mutex_lock(&g_stMemListlock);

    ptFind = SAL_memListGetNode(pBufHead);
    if (NULL == ptFind)
    {
        pthread_mutex_unlock(&g_stMemListlock);
        SAL_ERROR("SAL_memListGetNode not find error \n");
        return SAL_FAIL;
    }

    ret = SAL_memListDelNode(&ptFind->salList,SAL_memList());
    if (SAL_FAIL == ret)
    {
        SAL_ERROR("SAL_memListDel not find error \n");
    }
    
    free(ptFind);
    ptFind = NULL;    

    g_listMemSize -= sizeof(TSAL_ListMemHead);

    if (NULL == SAL_memList())
    {
        pthread_mutex_unlock(&g_stMemListlock);
        pthread_mutex_destroy(&g_stMemListlock);
    }
    else
    {
        pthread_mutex_unlock(&g_stMemListlock);
    }

    return ret;
}

/*******************************************************************************
* ������  : SAL_memListCheck
* ��  ��  : ��������
* ��  ��  : ��
* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
void SAL_memListCheck(INT32 checkPattern)
{
    #ifdef MEMLISTON

    UINT32 cnt           = 0;
    UINT32 memSize       = 0;
    UINT32 modleMemSize  = 0;
    char                 name[100] = "Begin";
    SAL_MemBlockObject  *ptHead    = NULL;
    SAL_MemBlockObject  *ptEnd     = NULL;
    TSAL_ListMemHead    *ptCur     = NULL;
    List_head           *ptList    = NULL;

    pthread_mutex_lock(&g_stMemListlock);

    switch (checkPattern)
    {
        case CHECK_PATTERN_ORDER :
        {
            ptCur = list_entry(SAL_memList(),TSAL_ListMemHead, salList);
            break;
        }
        case CHECK_PATTERN_SIZE :
        {            
            ptCur = list_entry(SAL_memListSortNode(SAL_memListSortSize),TSAL_ListMemHead, salList);
            break;
        }
        case CHECK_PATTERN_MODULE :
        {
            ptCur = list_entry(SAL_memListSortNode(SAL_memListSortName),TSAL_ListMemHead, salList);
            break;
        }
        default:
        {
            pthread_mutex_unlock(&g_stMemListlock);
            SAL_ERROR("SAL memCheck Not Support checkPattern (0~2)\n");
            return;
        }
    }
    
    ptList = &ptCur->salList;
    while (ptList)
    {
        ptCur = list_entry(ptList,TSAL_ListMemHead, salList);
        ptHead = (SAL_MemBlockObject *)(ptCur->salListHead.pBufBase);
        ptEnd  = (SAL_MemBlockObject *)((PhysAddr)ptCur->salListHead.pBufBase + (ptCur->salListHead.size - sizeof(SAL_MemBlockObject)));
        
        if(ptCur->salListHead.nMgicNum != ptHead->nMgicNum)
        {
            SAL_logPrintf("%s Memory Head is out of bounds ........\n",ptCur->modelName);
        }

        if(ptCur->salListEnd.nMgicNum != ptEnd->nMgicNum)
        {
            SAL_logPrintf("%s Memory End is out of bounds ........\n",ptCur->modelName);
        }
        else
        {
            if(CHECK_PATTERN_MODULE == checkPattern)
            {
                if(0==strcmp((char *)ptCur->modelName,name))
                {
                    modleMemSize += ptEnd->size;
                }
                else
                {
                    SAL_logPrintf("[%8s] Total Used Memory : %d(%d KB,%.2f MB)\n", name, modleMemSize,
                            modleMemSize/1024, modleMemSize*1.0/1024/1024);
                    SAL_strCpySafe(name, sizeof(name), ptCur->modelName);
                    modleMemSize = ptEnd->size;
                }

            }
            SAL_logPrintf("[%10s] Used Memory : %d (%.2f KB)\n" ,ptCur->modelName, ptEnd->size, ptEnd->size*1.0/1024);
            memSize += ptEnd->size;
        }
        
        cnt++;

        ptList = ptList->next;
        
    }
    
    if(CHECK_PATTERN_MODULE == checkPattern)
    {
        SAL_logPrintf("[%8s] Total Used Memory : %d\n",name,modleMemSize);
    }

    SAL_logPrintf("%d sal buffer ,alloc mem is %d;dsp total malloc mem is %d KB(%d MB)\n\n",
            cnt, g_listMemSize, (memSize/1024), ((memSize/1024)/1024));

    pthread_mutex_unlock(&g_stMemListlock);
    
    #endif
}

/*******************************************************************************
* ������  : SAL_memListCheckToFile
* ��  ��  : ��������
* ��  ��  : ��
* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
void SAL_memListCheckToFile(INT32 checkPattern)
{
    FILE *fileFd;
    #ifdef MEMLISTON

    UINT32 cnt           = 0;
    UINT32 memSize       = 0;
    UINT32 modleMemSize  = 0;
    char                 fileName[100];
    char                 name[100] = "Begin";
    SAL_MemBlockObject  *ptHead    = NULL;
    SAL_MemBlockObject  *ptEnd     = NULL;
    TSAL_ListMemHead    *ptCur     = NULL;
    List_head           *ptList    = NULL; 

    sprintf_s(fileName, sizeof(fileName), "%smemList.txt",SAVEDATAPATH);
    fileFd = fopen(fileName, "wb+");
    if (NULL == fileFd)
    {
        SAL_ERROR("open file %s failed !!!\n",fileName);
        return;
    }

    pthread_mutex_lock(&g_stMemListlock);

    switch (checkPattern)
    {
        case CHECK_PATTERN_ORDER :
        {
            ptList = SAL_memList();
            break;
        }
        case CHECK_PATTERN_SIZE :
        {            
            ptList = SAL_memListSortNode(SAL_memListSortSize);
            break;
        }
        case CHECK_PATTERN_MODULE :
        {
            ptList = SAL_memListSortNode(SAL_memListSortName);
            break;
        }
        default:
        {
            SAL_ERROR("SAL memCheck Not Support checkPattern (0~2)\n");
            fclose(fileFd);
            fileFd = NULL;
            pthread_mutex_unlock(&g_stMemListlock);
            return;
        }
    }
    
    while (ptList)
    {
        ptCur = list_entry(ptList,TSAL_ListMemHead, salList);
        ptHead = (SAL_MemBlockObject *)(ptCur->salListHead.pBufBase);
        ptEnd  = (SAL_MemBlockObject *)((PhysAddr)ptCur->salListHead.pBufBase + (ptCur->salListHead.size - sizeof(SAL_MemBlockObject)));
        
        if(ptCur->salListHead.nMgicNum != ptHead->nMgicNum)
        {
            fprintf(fileFd,"%s Memory Head is out of bounds ........\n",ptCur->modelName);
        }

        if(ptCur->salListEnd.nMgicNum != ptEnd->nMgicNum)
        {
            fprintf(fileFd,"%s Memory End is out of bounds ........\n",ptCur->modelName);
        }
        else
        {
            if(CHECK_PATTERN_MODULE == checkPattern)
            {
                if(0==strcmp(ptCur->modelName,name))
                {
                    modleMemSize += ptEnd->size;
                }
                else
                {
                    fprintf(fileFd,"[%8s] Total Used Memory : %d \n",name,modleMemSize);
                    SAL_strCpySafe(name, sizeof(name), ptCur->modelName);
                    modleMemSize = ptEnd->size;
                }

            }
            fprintf(fileFd,"[ptList:%p][%8s] Used Memory : %d \n",ptList, ptCur->modelName, ptEnd->size);
            memSize += ptEnd->size;
        }
        
        cnt++;

        ptList = ptList->next;
        
    }
    
    if(CHECK_PATTERN_MODULE == checkPattern)
    {
        fprintf(fileFd, "[%8s] Total Used Memory : %d \n",name,modleMemSize);
    }

    fprintf(fileFd, "%d sal buffer, alloc mem is %d, dsp total malloc mem is %d KB(%d MB) \n",
            cnt, g_listMemSize, (memSize/1024), ((memSize/1024)/1024));

    fclose(fileFd);
    fileFd = NULL;
    pthread_mutex_unlock(&g_stMemListlock);
    #endif
    return;
}

