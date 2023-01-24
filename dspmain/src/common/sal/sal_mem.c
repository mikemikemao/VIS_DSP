/*******************************************************************************
 * sal_mem.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : heshengyuan <heshengyuan@hikvision.com>
 * Version: V1.0.0  2015��2��9�� Create
 *
 * Description : ϵͳ�������ڴ�����ļ�, �����ڴ���䡢�ڴ濽�����ڴ�����ķ�װ
 * Modification: DSP����ʹ��malloc�����û�̬�ڴ棬������ͳ��
 *******************************************************************************/
 
/* ========================================================================== */
/*                             ͷ�ļ���                                       */
/* ========================================================================== */
#include <sal.h>
#include <malloc.h>
#include <sal_memList.h>

/* ========================================================================== */
/*                           ������Ͷ�����                                   */
/* ========================================================================== */

/* ========================================================================== */
/*                            ȫ�ֱ���������                                  */
/* ========================================================================== */

static UINT32 mallocLen = 0;

/* ========================================================================== */
/*                          ����������                                        */
/* ========================================================================== */

/*******************************************************************************
* ������  : SAL_memUAllocAlign
* ��  ��  : �ú���������linux�û�̬�������ڴ�
*
* ��  ��  : - size:  Ҫ������ڴ��С
*           - align: �û��Զ��������ֽ���, ��Ϊ0��ʾ�������Զ������
*                    �ò�������Ϊ4��������, ������������NULL
*        - modId: ģ������
* ��  ��  : �ޡ�
* ����ֵ  : ��NULL: ����ɹ�
*           NULL:   ����ʧ��
*******************************************************************************/
static Ptr SAL_memUAllocAlign(UINT32 size, UINT32 align, const char* modId)
{
    UINT32 allocSize = 0;
    UINT32 totalSize = 0;
    UINT32 headSize = 0;
    UINT32 endSize  = 0;
    Ptr pBufBase = NULL;
    Ptr pBufData = NULL;
    SAL_MemBlockObject *pBufHead = NULL;
    SAL_MemBlockObject *pBufEnd = NULL;
    if(0 == size)
    {
        SAL_ERROR("size must > 0\n");
        return NULL;
    }

    /* ���align ����Ϊ4�ı��� */
    if(0 != (align & 3))
    {
        SAL_ERROR("invalid align:%d\n", align);
        return NULL;
    }

    /* ����ͷ��С */
    headSize = sizeof(SAL_MemBlockObject);

    /* ����β��С */
    endSize  = sizeof(SAL_MemBlockObject);

    /* �ܵ�Ҫ������ڴ��С */
    totalSize = size + align + headSize + endSize;

    allocSize = SAL_align(totalSize, align);

    /* �����ڴ� */
    pBufBase = malloc(allocSize);
    if(NULL == pBufBase)
    {
        SAL_ERROR("alloc failed, size:%d, align:%d\n", size, align);
        return NULL;
    }

    SAL_memSetSafe(pBufBase, 0, allocSize);
    /* �������ݶε�λ�ã��ڴ��ǰ�����ڴ�ʹ�õ����ͣ�������������� */
    pBufData = (void *)((PhysAddr)pBufBase + headSize);
    if(0 != align)
    {
        pBufData = (Ptr)SAL_align((PhysAddr)pBufData, align);
    }

    /* ��ȡͷ��λ�� */
    pBufHead = SAL_memDataToHead(pBufData);

    /* ���ͷ */
    pBufHead->nMgicNum  = SAL_MEM_MAGIC;
    pBufHead->size      = allocSize;
    pBufHead->pBufBase  = pBufBase;

    /* ��ȡβ��λ�� */
    pBufEnd = (SAL_MemBlockObject *)((PhysAddr)pBufBase + (allocSize - endSize));

    /* ���β*/
    pBufEnd->nMgicNum  = SAL_MEM_MAGIC;
    pBufEnd->size      = allocSize;
    pBufEnd->pBufBase  = pBufBase;
    
    #ifdef MEMLISTON
    if (SAL_FAIL == SAL_memListAdd(pBufHead, pBufEnd, size, modId))
    {
        SAL_ERROR("SAL_memListAdd error\n");
    }
    #endif

    mallocLen += (allocSize);

    /*�������ݶ�ָ��*/
    return pBufData;
}

/*******************************************************************************
* ������  : SAL_memAlloc
* ��  ��  : ��������ͳһ���ڴ����뺯��
* ��  ��  : - size: �ڴ��С
*        - modId: ģ������
* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
Ptr SAL_memAlloc(UINT32 size, const char *modId)
{
    return SAL_memUAllocAlign((size), 4, modId);
}

/*******************************************************************************
* ������  : SAL_memCalloc
* ��  ��  : �ú�����SAL_memUAllocAlign�Ļ�����,���Ӷ��ڴ���0�Ķ���
* ��  ��  : - size:  Ҫ������ڴ��С
*        - modId: ģ������
* ��  ��  : �ޡ�
* ����ֵ  : ��NULL:  ����ɹ�
*           NULL:    ����ʧ��
*******************************************************************************/
Ptr SAL_memCalloc(UINT32 size)
{
    Ptr pPtr = SAL_memUAllocAlign(size, 4, "Sal");
    if(pPtr)
    {
        SAL_clearSize(pPtr, size);
    }
    return pPtr;
}

/*******************************************************************************
* ������  : SAL_allocMemFree
* ��  ��  : ϵͳ������µ��ڴ������װ�������������ͷ��ڴ沢ͳ�Ƹ��µ�ǰϵͳ�ܼ�ʹ�õ��ڴ�
* ��  ��  : - pPtr:    Ҫ�ͷŵ��ڴ��ַ
* ��  ��  : ��
* ����ֵ  : SAL_SOK:   �ɹ�,�ڴ����ͷ�
*           SAL_FAIL: ʧ��, �ڴ�δ�ͷ�
*******************************************************************************/
INT32 SAL_allocMemFree(Ptr pPtr)
{
    SAL_MemBlockObject *pBufHead = NULL;

    if(NULL == pPtr)
    {
        return SAL_FAIL;
    }

    pBufHead = SAL_memDataToHead(pPtr);

    /* У��ħ�� */
    if(SAL_MEM_MAGIC != pBufHead->nMgicNum)
    {
        SAL_ERROR("invalid magic:0x%x\n", pBufHead->nMgicNum);
        return SAL_FAIL;
    }

    /* �ͷ��ڴ��ʱ��ħ����Ϊ-1 */
    pBufHead->nMgicNum = (UINT32) - 1;

    mallocLen -= (pBufHead->size);
    #ifdef MEMLISTON
    
    if (SAL_FAIL == SAL_memListDel(pBufHead))
    {
        SAL_ERROR("SAL_memListDel ERROR!!!\n");
    }
    #endif
    
    /* �ͷ��ڴ� */
    free(pBufHead->pBufBase);

    return SAL_SOK;
}

/*******************************************************************************
* ������  : SAL_memAlign
* ��  ��  : ϵͳ������µ��ڴ������װ���������������ڴ沢ͳ���������ܹ���֤������ڴ����ֽڶ����Ч��
* ��  ��  : - alignSize: ǿ���ֽڶ���ĳ���
*         : - size     : �����С
*         : - modId    : ģ���ʶ
* ��  ��  : ��
* ����ֵ  : ptr  : ����ɹ����ڴ��ַ
*           NULL : ����ʧ��
*******************************************************************************/
void *SAL_memAlign(UINT32 size, UINT32 alignSize, const char *modId)
{
    UINT32 allocSize = 0;
    UINT32 totalSize = 0;
    UINT32 headSize = 0;
    UINT32 endSize  = 0;
    Ptr pBufBase = NULL;
    Ptr pBufData = NULL;
    Ptr pBufDataAlign = NULL;
    SAL_MemBlockObject *pBufHead = NULL;
    SAL_MemBlockObject *pBaseHead = NULL;
    SAL_MemBlockObject *pBufEnd = NULL;

    if(0 == size)
    {
        SAL_ERROR("size must > 0\n");
        return NULL;
    }

    /* ���align ����Ϊ4�ı��� */
    if(0 != (alignSize & 3))
    {
        SAL_ERROR("invalid align:%d\n", alignSize);
        return NULL;
    }

    /* ����ͷ��С */
    headSize = sizeof(SAL_MemBlockObject);


    /* ����β��С */
    endSize  = sizeof(SAL_MemBlockObject);


    /* �ܵ�Ҫ������ڴ��С */
    totalSize = size + alignSize + headSize*2 + endSize;
    allocSize = SAL_align(totalSize, alignSize);

    /* �����ڴ� */
    pBufBase = memalign(alignSize, allocSize);
    if(NULL == pBufBase)
    {
        SAL_ERROR("alloc failed, size:%d, align:%d\n", size, alignSize);
        return NULL;
    }
    SAL_memSetSafe(pBufBase, 0, allocSize);
    
    /* �������ݶε�λ�ã��ڴ��ǰ�����ڴ�ʹ�õ����ͣ�������������� */
    pBufData = (void *)((PhysAddr)pBufBase + headSize*2);
    if(0 != alignSize)
    {
        pBufDataAlign = (Ptr)SAL_align((PhysAddr)pBufData, alignSize);
    }

    /* ��ȡͷ��λ�� */
    pBufHead = SAL_memDataToHead(pBufDataAlign);

    /* ���ͷ */
    pBufHead->nMgicNum  = SAL_MEM_MAGIC;
    pBufHead->size      = allocSize;
    pBufHead->pBufBase  = pBufBase;

    pBaseHead = (SAL_MemBlockObject *)pBufBase;
    pBaseHead->nMgicNum  = SAL_MEM_MAGIC;
    pBaseHead->size      = allocSize;
    pBaseHead->pBufBase  = pBufBase;
    
    /* ��ȡβ��λ�� */
    //pBufEnd = (SAL_MemBlockObject *)( (unsigned long)(pBufData) + size);
    pBufEnd = (SAL_MemBlockObject *)((PhysAddr)pBufData + (size + alignSize) + (allocSize - totalSize));

    /* ���β*/
    pBufEnd->nMgicNum  = SAL_MEM_MAGIC;
    pBufEnd->size      = allocSize;
    pBufEnd->pBufBase  = pBufBase;

    #ifdef MEMLISTON
    if (SAL_FAIL == SAL_memListAdd(pBaseHead, pBufEnd, size, modId))
    {
        SAL_ERROR("SAL_memListAdd error\n");
    }
    
    #endif
    mallocLen += (allocSize);

    /*�������ݶ�ָ��*/
    return pBufDataAlign;
}

UINT32 SAL_memTotalLen(void){return mallocLen;}

/*******************************************************************************
* ������  : SAL_memPageSize
* ��  ��  : ��ȡϵͳҳ���С
* ��  ��  : - void:
* ��  ��  : ��
* ����ֵ  : ҳ��С
*******************************************************************************/
UINT32 SAL_memPageSize(void)
{
    return (UINT32)getpagesize();
}


/*******************************************************************************
* ������  : SAL_memArray2D
* ��  ��  : ����ά���������ڴ�
* ��  ��  : - row: ��
*         : - col: ��
* ��  ��  : ��
* ����ֵ  : ��ά�����ڴ�
*******************************************************************************/
UINT32** SAL_memArray2D(UINT32 row, UINT32 col)
{
    UINT32 size = sizeof(UINT32);
    UINT32 point_size = sizeof(UINT32*);

    /* �������ڴ棬����point_size * row��ʾ���row����ָ�� */
    UINT32 **arr = (UINT32 **) SAL_memAlloc(point_size * row + size * row * col, "Sal");
    if (arr != NULL)
    {
        SAL_memSetSafe(arr, 0, point_size * row + size * row * col);
        UINT32 *head = (UINT32*)((PhysAddr)arr + point_size * row);
        while (row--)
            arr[row] = (UINT32*)((PhysAddr)head + row * col * size);
    }
    return (UINT32**)arr;
}


/*******************************************************************************
* Function      : SAL_memCpy
* Description   : ��װ memcpy �������������ֽڶ���ʽ�Ŀ��ٿ���
* Input         : - dest  : Ŀ���ַ
*               : - src   : Դ
*               : - nBytes: ����
* Output        : NONE
* Return        : HIK_SOK  : Success
*                 HIK_FAIL : Fail
*******************************************************************************/
void SAL_memCpy(INT8 *dest, INT8 *src, UINT32 nBytes)
{
    PhysAddr desRes, srcRes;

    desRes = (PhysAddr)dest & 0x3;
    srcRes = (PhysAddr)src & 0x3;
    if (srcRes == 0 && desRes != 0)
    {
        SAL_memCpySafe(dest, src, nBytes);
    }
    else if (srcRes != 0 && desRes == 0)
    {
        SAL_memCpySafe(dest, src, nBytes);
    }
    else if ((srcRes != 0 && desRes != 0) && (srcRes == desRes))
    {
        while (srcRes > 0)
        {
            *dest++ = *src++;
            srcRes--;
        }
    }
    else if ((srcRes != 0 && desRes != 0) && (srcRes != desRes))
    {
        SAL_memCpySafe(dest, src, nBytes);
    }

    if ((((PhysAddr)dest & 0x3) == 0) && (((PhysAddr)src & 0x3) == 0))
    {

        while (nBytes >= 4)
        {
            *((UINT32 *)dest) = *((UINT32 *)src);
            dest += 4;
            src += 4;
            nBytes -= 4;
        }
    }

    while (nBytes > 0)
    {
        *dest++ = *src++;
        nBytes--;
    }
}


