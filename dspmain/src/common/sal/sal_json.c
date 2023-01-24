/*******************************************************************************
 * sal_json.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : zhangyulong8 <zhangyulong8@hikvision.com>
 * Version: V1.0.0  2020��9��17�� Create
 *
 * Description : ����������list�Ĳ���(�ɰ淢��δ����Ŀʹ�õ�list���ܣ�����ع���
                 ���ܣ�ʹ�ò������׶�����Щ)������������ɾ��Ĺ��ܣ��ĵĹ�����Ҫ
                 ʹ�����ڲ�ѯ��Ԫ��֮��֮��ֱ���޸�Ԫ�����ݼ��ɡ�list��ʵ����Ҫ
                 ʹ�����Լ��������ýӿڷ��̰߳�ȫ��ͬ����Ҫʹ�������ö��̹߳���
 * Modification:
 *******************************************************************************/

/* ========================================================================== */
/*                             ͷ�ļ���                                       */
/* ========================================================================== */
#include "sal_json.h"

#include "sal_trace.h"
#include "sal_macro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <limits.h>
#include <math.h>

/* ========================================================================== */
/*                           ������Ͷ�����                                   */
/* ========================================================================== */

/* ========================================================================== */
/*                          ���ݽṹ������                                    */
/* ========================================================================== */


/* ========================================================================== */
/*                          ����������                                        */
/* ========================================================================== */
static const INT8 *SAL_jsonParseValue(SAL_Json *item, const INT8 *str);
static INT8 *SAL_jsonPrintValue(const SAL_Json *item, INT32 depth, INT32 fmt);


/*******************************************************************************
* ������  : SAL_jsonSkip
* ��  ��  : ���������ַ����հ׵�
* ��  ��  : - in : �ַ�����ַ
* ��  ��  : ��
* ����ֵ  : ��NULL  : ���������ַ���ĵ�ַ
*******************************************************************************/
static const INT8 *SAL_jsonSkip(const INT8 *in) 
{
    if (NULL == in)
    {
        return in;
    }

    while (('\0' != *in) && (unsigned char)*in <= ' ')
    {
        in++;
    }

    return in;
}

/*******************************************************************************
* ������  : SAL_jsonMalloc
* ��  ��  : ר�Ŵ���JSON��Ҫ���ڴ�ռ�
* ��  ��  : - dataLen : �ռ䳤��
* ��  ��  : ��
* ����ֵ  : ��NULL  : �ɹ���������ڴ��ַ
*           NULL : ʧ��
*******************************************************************************/
static Ptr SAL_jsonMalloc(UINT32 dataLen)
{
    Ptr data = NULL;

    data = calloc(1, dataLen);

    return data;
}

/*******************************************************************************
* ������  : SAL_jsonFree
* ��  ��  : �ͷ�JSON�ĵ�ַ
* ��  ��  : - ptr : ��Ҫ�ͷŵ��ڴ����ַ
* ��  ��  : ��
* ����ֵ  : ��
*******************************************************************************/
VOID SAL_jsonFree(void* ptr)
{
    if (NULL != ptr)
    {
        free(ptr);
    }
}

/*******************************************************************************
* ������  : SAL_jsonCreateNewItem
* ��  ��  : ������JSON����
* ��  ��  : - str   : JSON�ַ���
* ��  ��  : ��
* ����ֵ  : ��NULL  : �ɹ�������ֵJSON����ʹ����Ϻ���Ҫ���ú����ͷ���Դ
*           NULL : ʧ��
*******************************************************************************/
static SAL_Json* SAL_jsonCreateNewItem(VOID)
{
	SAL_Json* item = NULL;

    item = (SAL_Json*)SAL_jsonMalloc(sizeof(SAL_Json));

	return item;
}

/*******************************************************************************
* ������  : SAL_jsonStrdup
* ��  ��  : JSON�����ַ�������
* ��  ��  : - str : Դ�ַ�����ַ
* ��  ��  : ��
* ����ֵ  : ��NULL  : �ɹ�����������ַ���ַ
*           NULL : ʧ��orԴ����NULL
*******************************************************************************/
INT8* SAL_jsonStrdup(const INT8* str)
{
    UINT32 len = 0;
    INT8* tmp = NULL;

    if (NULL == str)
    {
        return NULL;
    }

    len = strlen(str) + 1;
    tmp = (INT8*)SAL_jsonMalloc(len);
    if (NULL == tmp)
    {
        return tmp;
    }

    SAL_memCpySafe(tmp, str, len);
    
    return tmp;
}

static UINT32 SAL_jsonParseHex4(const INT8 *str)
{
    UINT32 h = 0;
    INT32 i = 0;

    if (NULL == str)
    {
        return 0;
    }

    for (i = 0; i < 4; ++i)
    {
        if (0 != i)
        {
            h = h << 4;
            str++;
        }

        if ('0' <= *str && '9' >= *str)
        {
            h += (*str) - '0';
        }
        else if ('A' <= *str && 'F' >= *str)
        {
            h += 10 + (*str) - 'A';
        }
        else if ('a' <= *str && 'f' >= *str)
        {
            h += 10 + (*str) - 'a';
        }
        else
        {
            return 0;
        }
    }

    return h;
}


// �����ַ���
/*******************************************************************************
* ������  : SAL_jsonStrdup
* ��  ��  : �����ַ���JSON����
* ��  ��  : - item : JSON���󣬱����������JSON����
*         : - str : Դ�ַ�����ַ
* ��  ��  : ��
* ����ֵ  : ��NULL  : �ɹ�������һ��JSON�ַ���ʣ����ַ���ַ
*           NULL : ʧ��
*******************************************************************************/

static const INT8 *SAL_jsonParseString(SAL_Json *item, const INT8 *str)
{
    const INT8 *ptr;
    INT8 *ptr2 = NULL;
    INT8 *out = NULL;
    INT32 len = 0;
    UINT32 uc = 0;
    UINT32 uc2 = 0;

    if (NULL == item || NULL == str)
    {
        return NULL;
    }

    ptr = str + 1;

    // �ַ����ж�
    if ('\"' != *str)
    {
        return NULL;
    }

    // ����ע��
    while ('\"' != *ptr && '\0' != *ptr && ++len)
    {
        if ('\\' == *ptr++)
        {
            ptr++;
        }
    }

    out = (INT8*)SAL_jsonMalloc(len + 1);
    if (NULL == out)
    {
        return NULL;
    }
	
	ptr = str+1;
    ptr2 = out;

    while ('\"' != *ptr && '\0' != *ptr)
    {
        if ('\\' != *ptr)
        {
            *ptr2++ = *ptr++;
        }
        else
        {
            ptr++;
            switch (*ptr)
            {
            case 'b':
            {
                *ptr2++ ='\b';
            }break;
            case 'f':
            {
                *ptr2++ ='\f';
            }break;
            case 'n':
            {
                *ptr2++ ='\n';
            }break;
            case 'r':
            {
                *ptr2++ ='\r';
            }break;
            case 't':
            {
                *ptr2++ ='\t';
            }break;
            case 'u':   // utf16 to utf8
            {
                // get unicode char
                uc = SAL_jsonParseHex4(ptr+1);
                ptr += 4;

                // invalid
                if ((0xDC00 <= uc && 0xDFFF >= uc) || uc == 0)
                {
                    break;
                }

                // UTF16 surrogate pairs.
                if (0xD800 <= uc && 0xDBFF >= uc)
                {
                    // missing second-half of surrogate.
                    if ('\\' != ptr[1] || 'u' != ptr[2])
                    {
                        break;
                    }
                    
                    uc2 = SAL_jsonParseHex4(ptr+3);
                    ptr += 6;

                    // invalid second-half of surrogate.
                    if (0xDC00 > uc2 || 0xDFFF < uc2)
                    {
                        break;
                    }
                    
                    uc = 0x10000 + (((uc & 0x3FF) << 10) | (uc2 & 0x3FF));

                    len = 4;
                    if (0x80 > uc)
                    {
                        len = 1;
                    }
                    else if (0x800 > uc)
                    {
                        len = 2;
                    }
                    else if (0x10000 > uc)
                    {
                        len = 3;
                        ptr2 += len;
                    }

                    switch (len)
                    {
                    case 4:
                    {
                        *--ptr2 = ((uc | 0x80) & 0xBF);
                        uc >>= 6;
                    }//break;
                    case 3:
                    {
                        *--ptr2 = ((uc | 0x80) & 0xBF);
                        uc >>= 6;
                    }//break;
                    case 2:
                    {
                        *--ptr2 = ((uc | 0x80) & 0xBF);
                        uc >>= 6;
                    }//break;
                    case 1:
                    {
                        *--ptr2 = (uc | 0x00);
                    }//break;
                    }
                    ptr2 += len;
                }
            }break;
            default:
            {
                *ptr2++ = *ptr;
            }break;
            }
            ptr++;
        }
    }

    *ptr2 = '\0';
    if ('\"' == *ptr)
    {
        ptr++;
    }

    item->strValue = out;
    item->jsonType = JSON_String;

    return ptr;
}

// ��װ�ַ�ֵ�ַ�
static INT8 *SAL_jsonPrintStringPtr(const INT8 *str)
{
    const INT8* ptr = NULL;
    INT8* ptr2 = NULL;
    INT8* out = NULL;
    INT32 len = 0;
    INT32 flag = 0;
    UINT8 token = 0;
	INT32 ptr2len = 0;
	
    if (NULL == str)
    {
        out = SAL_jsonMalloc(3);
        if (NULL == out)
        {
            return NULL;
        }

        SAL_strCpySafe(out, 3,"\"\"");
        return out;
    }
    
    for (ptr = str; '\0' != *ptr; ptr++)
    {
        if ((0 < *ptr && 32 > *ptr) || ('\"' == *ptr) || ('\\' == *ptr))
        {
            flag |= 1;
        }
        else
        {
            flag |= 0;
        }
    }

    if (!flag)
    {
        len = ptr - str;
        out = SAL_jsonMalloc(len + 3);
        if (NULL == out)
        {
            return NULL;
        }

        ptr2 = out;
        *ptr2++ = '\"';
        SAL_strCpySafe(ptr2,len + 2, str);
        ptr2[len] = '\"';
        ptr2[len+1] = '\0';
        return out;
    }

    ptr = str;
    while ((token = *ptr) && ++len)
    {
        if (strchr("\"\\\b\f\n\r\t", token))
        {
            len++;
        }
        else if (token < 32)
        {
            len += 5;
            ptr++;
        }
    }

    out = SAL_jsonMalloc(len + 3);
    if (NULL == out)
    {
        return NULL;
    }

    ptr2 = out;
	ptr2len = len +3;
    ptr = str;
    *ptr2++ = '\"';
	ptr2len = ptr2len -1;
    while (*ptr)
    {
        if (31 < (unsigned char)*ptr && '\"' != *ptr && '\\' != *ptr)
        {
            *ptr2++ = *ptr++;
			ptr2len = ptr2len -1;
        }
        else
        {
            *ptr2++ = '\\';
			ptr2len = ptr2len -1;
            token = *ptr++;
            switch (token)
            {
            case '\\':
            {
                *ptr2++ = '\\';
				ptr2len = ptr2len -1;
            }break;
            case '\"':
            {
                *ptr2++ = '\"';
				ptr2len = ptr2len -1;
            }break;
            case '\b':
            {
                *ptr2++ = 'b';
				ptr2len = ptr2len -1;
            }break;
            case '\f':
            {
                *ptr2++ = 'f';
				ptr2len = ptr2len -1;
            }break;
            case '\n':
            {
                *ptr2++ = 'n';
				ptr2len = ptr2len -1;
            }break;
            case '\r':
            {
                *ptr2++ = 'r';
				ptr2len = ptr2len -1;
            }break;
            case '\t':
            {
                *ptr2++ = 't';
				ptr2len = ptr2len -1;
            }break;
            default:
            {
                sprintf_s(ptr2,ptr2len, "u%04x", token);
                ptr2 += 5;
				ptr2len = ptr2len - 5;
            }break;
            }
        }
    }

    *ptr2++ = '\"';
	ptr2len = ptr2len -1;
    *ptr2++ = '\0';
	ptr2len = ptr2len -1;
    return out;
}

static INT8 *SAL_jsonPrintString(const SAL_Json* item)
{
    if (NULL == item)
    {
        return NULL;
    }

    return SAL_jsonPrintStringPtr(item->strValue);
}



// ��������
static const INT8 *SAL_jsonParseNumber(SAL_Json *item, const INT8 *num)
{
    FLOAT64 n = 0;
    FLOAT64 sign = 1;
    FLOAT64 scale = 0;

    INT32 subscale = 0;
    INT32 signsubscale = 1;
    if (NULL == item || NULL == num)
    {
        return 0;
    }

    if ('-' == *num)
    {
        sign = -1;
        num++;
    }

    if ('0' == *num)
    {
        num++;
    }

    if ('1' <= *num && '9' >= *num)
    {
        do{
            n = n * 10.0 + (*num - '0');
            num++;
        }while ('0' <= *num && '9' >= *num);
    }
    
    if ('.' == *num && '0' <= num[1] && '9' >= num[1])
    {
        num++;
        do{
            n = n * 10.0 + (*num++ - '0');
            scale--;
        }while ('0' <= *num && '9' >= *num);
    }

    if ('e' == *num || 'E' == *num)
    {
        num++;
        if ('+' == *num)
        {
            num++;
        }
        else if ('-' == *num)
        {
            signsubscale = -1;
            num++;
        }

        while ('0' <= *num && '9' >= *num)
        {
            subscale = subscale * 10 + (*num - '0');
            num++;
        }
    }

    n = sign * n * pow(10.0f, scale + subscale * signsubscale);

    item->doubleValue = n;
    item->intValue = (INT32)n;
    item->jsonType = JSON_Number;
    
    return num;
}

// ��װ����ֵ�ַ�
static INT8 *SAL_jsonPrintNumber(const SAL_Json *item)
{
    INT8* tmp = NULL;
    FLOAT64 d = 0.0f;

    if (NULL == item)
    {
        return NULL;
    }

    d = item->doubleValue;
    if (0.0f == d)
    {
        tmp = SAL_jsonStrdup("0");
    }
    // ������
    else if (fabs((FLOAT64)item->intValue - item->doubleValue) <= DBL_EPSILON && d <= INT_MAX && d >= INT_MIN)
    {
        tmp = SAL_jsonMalloc(21);
        if (NULL != tmp)
        {
            snprintf(tmp, 21, "%d", item->intValue);
        }
    }
    // ������
    else
    {
        tmp = SAL_jsonMalloc(64);
        if (tmp)
        {
            if (fabs(floor(d) - d) <= DBL_EPSILON && fabs(d) < 1.0e60)
            {
                snprintf(tmp, 64, "%.0f", d);
            }
            else if (fabs(d) < 1.0e-6 || fabs(d) > 1.0e9)
            {
                snprintf(tmp, 64, "%e", d);
            }
            else
            {
                snprintf(tmp, 64, "%f", d);
            }
        }
    }

    return tmp;
}

// ��������
static const INT8 *SAL_jsonParseArray(SAL_Json *item, const INT8 *str)
{
    SAL_Json* child = NULL;

    if (NULL == item || NULL == str)
    {
        return NULL;
    }

    if ('[' != *str)
    {
        return NULL;
    }
    
    item->jsonType = JSON_Array;
    str = SAL_jsonSkip(str + 1);

    // empty
    if (']' == *str)
    {
        return str + 1;
    }

    child = SAL_jsonCreateNewItem();
    item->child = child;
    if (NULL == item->child)
    {
        return NULL;
    }
    
    str = SAL_jsonSkip(SAL_jsonParseValue(child, SAL_jsonSkip(str)));
    if (NULL == str)
    {
        return NULL;
    }

    while (',' == *str)
    {
        SAL_Json* tmp = SAL_jsonCreateNewItem();
        if (NULL == tmp)
        {
            return NULL;
        }

        child->next = tmp;
        tmp->prev = child;
        child = tmp;
        str = SAL_jsonSkip(SAL_jsonParseValue(child, SAL_jsonSkip(str +1)));
        if (NULL == str)
        {
            return NULL;
        }
    }

    if (']' == *str)
    {
        return str + 1;
    }

    return NULL;
}

static INT8 *SAL_jsonPrintArray(const SAL_Json *item, INT32 depth, INT32 fmt)
{
    INT8 **entries = NULL;
    INT8 *out = NULL;
    INT8 *ptr = NULL;
    INT8 *ret = NULL;
    INT32 len = 5;
    SAL_Json *child = NULL;

    INT32 numentries = 0;
    INT32 i = 0;
    INT32 fail = 0;
    size_t tmplen = 0;

    if (NULL == item)
    {
        return NULL;
    }

    child = item->child;
    while (child)
    {
        numentries++;
        child = child->next;
    }

    if (0 == numentries)
    {
        out = SAL_jsonMalloc(3);
        if (NULL != out)
        {
            SAL_strCpySafe(out, 3,"[]");
        }
        return out;
    }

    entries = (INT8**)SAL_jsonMalloc(numentries * sizeof(INT8*));
    if (NULL == entries)
    {
        return NULL;
    }

    child = item->child;
    while (NULL != child && 0 == fail)
    {
        ret = SAL_jsonPrintValue(child, depth+1, fmt);
        entries[i++] = ret;
        if (NULL != ret)
        {
            len += strlen(ret) + 2 + (fmt ? 1 : 0);
        }
        else
        {
            fail = 1;
        }

        child = child->next;
    }

    if (0 == fail)
    {
        out = SAL_jsonMalloc(len);
    }

    if (NULL == out)
    {
        fail = 1;
    }

    // failed
    if (0 != fail)
    {
        for (i=0; i < numentries; i++)
        {
            if (NULL != entries[i])
            {
                SAL_jsonFree(entries[i]);
                entries[i] = NULL;
            }
        }
        SAL_jsonFree(entries);

        return NULL;
    }

    *out = '[';
    ptr = out+1;
    *ptr = '\0';
    for (i=0; i < numentries; i++)
    {
        tmplen = strlen(entries[i]);
        SAL_memCpySafe(ptr, entries[i], tmplen);
        ptr += tmplen;
        if (i != numentries - 1)
        {
            *ptr++ = ',';
            if (0 != fmt)
            {
                *ptr++ = ' ';
                *ptr = '\0';
            }
        }
        SAL_jsonFree(entries[i]);
        entries[i] = NULL;
    }
    SAL_jsonFree(entries);
    *ptr++ = ']';
    *ptr++ = '\0';

    return out;
}


static const INT8 *SAL_jsonParseObject(SAL_Json *item, const INT8 *str)
{
    SAL_Json* child = NULL;
    if (NULL == item || NULL == str)
    {
        return NULL;
    }

    if ('{' != *str)
    {
        return NULL;
    }

    item->jsonType = JSON_Object;
    str = SAL_jsonSkip(str + 1);

    if ('}' == *str)
    {
        return str + 1;
    }

    child = SAL_jsonCreateNewItem();
    item->child = child;
    if (NULL == child)
    {
        return NULL;
    }

    str = SAL_jsonSkip(SAL_jsonParseValue(child, SAL_jsonSkip(str)));
    if (NULL == str)
    {
        return NULL;
    }
    child->key = child->strValue;
    child->strValue = NULL;

    if (':' != *str)
    {
        return NULL;
    }

    str = SAL_jsonSkip(SAL_jsonParseValue(child, SAL_jsonSkip(str + 1)));
    if (NULL == str)
    {
        return NULL;
    }

    while (',' == *str)
    {
        SAL_Json* tmp = SAL_jsonCreateNewItem();
        if (NULL == tmp)
        {
            return NULL;
        }

        child->next = tmp;
        tmp->prev = child;
        child = tmp;
        str = SAL_jsonSkip(SAL_jsonParseString(child, SAL_jsonSkip(str + 1)));
        if (NULL == str)
        {
            return NULL;
        }
        child->key = child->strValue;
        child->strValue = NULL;

        if (':' != *str)
        {
            return NULL;
        }

        str = SAL_jsonSkip(SAL_jsonParseValue(child, SAL_jsonSkip(str +1)));
        if (NULL == str)
        {
            return NULL;
        }
    }

    if ('}' == *str)
    {
        return str + 1;
    }

    return NULL;
}

static INT8 *SAL_jsonPrintObject(const SAL_Json *item , INT32 depth, INT32 fmt)
{
    INT8 **entries = NULL;
    INT8 **names = NULL;
    INT8 *out = NULL;
    INT8 *ptr = NULL; 
    INT8 *ret = NULL;
    INT8 *str = NULL;

    INT32 len = 7;
    INT32 i = 0;
    INT32 j = 0;

    SAL_Json *child = NULL;

    INT32 numentries = 0;
    INT32 fail = 0;
    size_t tmplen = 0;
	size_t ptrlen = 0;

    if (NULL == item)
    {
        return NULL;
    }

    child = item->child;
    while (NULL != child)
    {
        numentries++;
        child = child->next;
    }

    if (0 == numentries)
    {
        out = SAL_jsonMalloc(0 != fmt ? depth+4 : 3);
        if (NULL == out)
        {
            return NULL;
        }
        
        ptr = out;
        *ptr++ = '{';

        if (0 != fmt)
        {
            *ptr++ = '\n';
            for (i = 0; i < depth - 1; i++)
            {
                *ptr++ = '\t';
            }
        }

        *ptr++ = '}';
        *ptr++ = '\0';

        return out;
    }

    entries = (INT8**)SAL_jsonMalloc(numentries * sizeof(INT8*));
    if (NULL == entries)
    {
        return NULL;
    }

    names = (INT8**)SAL_jsonMalloc(numentries * sizeof(INT8*));
    if (NULL == names)
    {
        SAL_jsonFree(entries);
        entries = NULL;
        return NULL;
    }

    child = item->child;
    depth++;
    i = 0;
    if (0 != fmt)
    {
        len += depth;
    }
    while (child)
    {
        str = SAL_jsonPrintStringPtr(child->key);
        names[i] = str;
        ret = SAL_jsonPrintValue(child, depth, fmt);
        entries[i] = ret;
        i++;
        
        if (NULL != str && NULL != ret)
        {
            len += strlen(ret) + strlen(str) + 2 + (0 != fmt ? 2 + depth : 0);
        }
        else
        {
            fail = 1;
        }

        child = child->next;
    }

    if (0 == fail)
    {
        out = (INT8*)SAL_jsonMalloc(len);
		ptrlen = len;
    }

    if (NULL == out)
    {
        fail = 1;
		ptrlen = 0;
    }

    // failed
    if (0 != fail)
    {
        for (i = 0; i < numentries; i++)
        {
            if (NULL != names[i])
            {
                SAL_jsonFree(names[i]);
                names[i] = NULL;
            }

            if (NULL != entries[i])
            {
                SAL_jsonFree(entries[i]);
                entries[i] = NULL;
            }
        }

        SAL_jsonFree(names);
        names = NULL;
        SAL_jsonFree(entries);
        entries = NULL;

        return NULL;
    }

    *out = '{';
    ptr = out+1;
	ptrlen = ptrlen -1;
    if (0 != fmt)
    {
        *ptr++ = '\n';
		ptrlen = ptrlen -1;
        *ptr = '\0';
    }
    for (i = 0; i < numentries; i++)
    {
        if (0 != fmt)
        {
            for (j = 0; j < depth; j++)
            {
                *ptr++ = '\t';
				ptrlen = ptrlen -1;
            }
        }

        tmplen = strlen(names[i]);
        SAL_memCpySafe(ptr, names[i], tmplen);
        ptr += tmplen;
		ptrlen = ptrlen -tmplen;

        *ptr++=':';
		ptrlen = ptrlen -1;
        if (0 != fmt)
        {
            *ptr++ = '\t';
        }

        SAL_strCpySafe(ptr,ptrlen, entries[i]);
        ptr += strlen(entries[i]);
		ptrlen = ptrlen -strlen(entries[i]);

        if (i != numentries - 1)
        {
            *ptr++ = ',';
			ptrlen = ptrlen -1;
        }

        if (0 != fmt)
        {
            *ptr++ = '\n';
			ptrlen = ptrlen -1;
            *ptr = '\0';
        }

        SAL_jsonFree(names[i]);
        names[i] = NULL;
        SAL_jsonFree(entries[i]);
        entries[i] = NULL;
    }

    SAL_jsonFree(names);
    names = NULL;
    SAL_jsonFree(entries);
    entries = NULL;

    if (0 != fmt)
    {
        for (i = 0; i < depth - 1; i++)
        {
            *ptr++ = '\t';
			ptrlen = ptrlen -1;
        }
    }

    *ptr++ = '}';
	ptrlen = ptrlen -1;
    *ptr++ = '\0';
	ptrlen = ptrlen -1;

    return out;
}




// JSON����
static const INT8 *SAL_jsonParseValue(SAL_Json *item, const INT8 *str)
{
    if (NULL == str || NULL == item)
    {
        return NULL;
    }

    if (0 == strncmp(str, "null", strlen("null")))
    {
        item->jsonType = JSON_NULL;
        return str + 4;
    }

    if (0 == strncmp(str, "false", strlen("false")))
    {
        item->jsonType = JSON_False;
        return str +5;
    }

    if (0 == strncmp(str, "true", strlen("true")))
    {
        item->jsonType = JSON_True;
        return str + 4;
    }

    // �ַ���
    if ('\"' == *str)
    {
        return SAL_jsonParseString(item, str);
    }
    // ����
    else if ('-' == *str || ('0' <= *str && '9' >= *str))
    {
        return SAL_jsonParseNumber(item, str);
    }
    // ����
    else if ('[' == *str)
    {
        return SAL_jsonParseArray(item, str);
    }
    // object
    else if ('{' == *str)
    {
        return SAL_jsonParseObject(item, str);
    }

    return NULL;
}

// Json��ʽ��
static INT8 *SAL_jsonPrintValue(const SAL_Json *item, INT32 depth, INT32 fmt)
{
    INT8 *out = NULL;

    if (NULL == item)
    {
        return NULL;
    }

    switch (item->jsonType)
    {
    case JSON_NULL:
    {
        out = SAL_jsonStrdup("null");
    }break;
    case JSON_False:
    {
        out = SAL_jsonStrdup("false");
    }break;
    case JSON_True:
    {
        out = SAL_jsonStrdup("true");
    }break;
    case JSON_Number:
    {
        out = SAL_jsonPrintNumber(item);
    }break;
    case JSON_String:
    {
        out = SAL_jsonPrintString(item);
    }break;
    case JSON_Array:
    {
        out = SAL_jsonPrintArray(item, depth, fmt);
    }break;
    case JSON_Object:
    {
        out = SAL_jsonPrintObject(item, depth, fmt);
    }break;
    }

    return out;
}

/*******************************************************************************
* ������  : SAL_jsonDelete
* ��  ��  : ����JSON����
* ��  ��  : - item   : JSON����
* ��  ��  : ��
* ����ֵ  : ��
*******************************************************************************/
VOID SAL_jsonDelete(SAL_Json* item)
{
    SAL_Json* tmp = NULL;
    if (NULL == item)
    {
        return;
    }

	while (NULL != item)
	{
	    tmp = item->next;

        // �����ӽڵ��ڴ�
        if (NULL != item->child)
        {
            SAL_jsonDelete(item->child);
            item->child = NULL;
        }

        // ����strValue�ڴ�
        if (NULL != item->strValue)
        {
            SAL_jsonFree(item->strValue);
            item->strValue = NULL;
        }

        // ����key�ڴ�
        if (NULL != item->key)
        {
            SAL_jsonFree(item->key);
            item->key = NULL;
        }

        SAL_jsonFree(item);
        item = tmp;
	}
}

/*******************************************************************************
* ������  : SAL_jsonRead
* ��  ��  : JSON�ַ�������ΪJSON����
* ��  ��  : - str   : JSON�ַ���
* ��  ��  : ��
* ����ֵ  : ��NULL  : �ɹ�������ֵJSON����
*           NULL : ʧ��
*******************************************************************************/
SAL_Json* SAL_jsonRead(const INT8 *str)
{
    SAL_Json* item = NULL;
    if (NULL == str)
    {
        SAL_ERROR("input str is NULL!\n");
        return NULL;
    }

    item = SAL_jsonCreateNewItem();
    if (NULL == item)
    {
        SAL_ERROR("SAL_jsonCreateNewItem error!\n");
        return NULL;
    }
    
    if (NULL == SAL_jsonParseValue(item, str))
    {
        SAL_ERROR("SAL_jsonParseValue error!\n");
        SAL_jsonDelete(item);
        return NULL;
    }

    return item;
}

/*******************************************************************************
* ������  : SAL_jsonWrite
* ��  ��  : JSON����תΪ�ַ���
* ��  ��  : - root   : JSON����
*         : - fmt : �Ƿ���Ҫ��ʽ�����(Json�ַ����пո�ͻ��У�����Ŀ������)��0��ʾ����ʽ�����(ʡ�ڴ�)����0��ʾ��Ҫ��ʽ�����
* ��  ��  : ��
* ����ֵ  : ��NULL  : �ɹ�������ֵ�ַ���ʹ����Ϻ���Ҫ���ú����ͷ���Դ
*           NULL : ʧ��
*******************************************************************************/
INT8* SAL_jsonWrite(const SAL_Json* root, INT32 fmt)
{
    INT8* jsonStr = NULL;
    if (NULL == root)
    {
        SAL_ERROR("input root is NULL!\n");
        return NULL;
    }

    jsonStr = SAL_jsonPrintValue(root, 0, fmt);
    if (NULL == jsonStr)
    {
        SAL_ERROR("SAL_jsonPrintValue error!\n");
    }

    return jsonStr;
}

static SAL_Json *SAL_jsonCreateNull(void)
{
    SAL_Json* item = NULL;

    item = SAL_jsonCreateNewItem();
    if (NULL == item)
    {
        return NULL;
    }

    item->jsonType = JSON_NULL;
    return item;
}

static SAL_Json *SAL_jsonCreateTrue(VOID)
{
    SAL_Json* item = NULL;

    item = SAL_jsonCreateNewItem();
    if (NULL == item)
    {
        return NULL;
    }

    item->jsonType = JSON_True;
    return item;
}

static SAL_Json *SAL_jsonCreateFalse(VOID)
{
    SAL_Json* item = NULL;

    item = SAL_jsonCreateNewItem();
    if (NULL == item)
    {
        return NULL;
    }

    item->jsonType = JSON_False;
    return item;
}

static SAL_Json *SAL_jsonCreateBool(BOOL b)
{
    SAL_Json* item = NULL;

    item = SAL_jsonCreateNewItem();
    if (NULL == item)
    {
        return NULL;
    }

    if (SAL_TRUE == b)
    {
        item->jsonType = JSON_True;
    }
    else
    {
        item->jsonType = JSON_False;
    }

    return item;
}

static SAL_Json *SAL_jsonCreateIntNumber(INT32 num)
{
    SAL_Json* item = NULL;

    item = SAL_jsonCreateNewItem();
    if (NULL == item)
    {
        return NULL;
    }

    item->intValue = num;
    item->doubleValue = (FLOAT64)num;
    item->jsonType = JSON_Number;
    return item;
}

static SAL_Json *SAL_jsonCreateDoubleNumber(FLOAT64 num)
{
    SAL_Json* item = NULL;

    item = SAL_jsonCreateNewItem();
    if (NULL == item)
    {
        return NULL;
    }

    item->intValue = (INT32)num;
    item->doubleValue = num;
    item->jsonType = JSON_Number;
    return item;
}

static SAL_Json *SAL_jsonCreateString(const INT8 *string)
{
    SAL_Json* item = NULL;

    if (NULL == string)
    {
        return NULL;
    }
    
    item = SAL_jsonCreateNewItem();
    if (NULL == item)
    {
        return NULL;
    }

    item->strValue = SAL_jsonStrdup(string);
    item->jsonType = JSON_String;
    return item;
}

/*******************************************************************************
* ������  : SAL_jsonCreateArray
* ��  ��  : ����һ��JSON�������Ϳ����ݶ���
* ��  ��  : - ��
* ��  ��  : ��
* ����ֵ  : ��NULL  : �ɹ�������ֵJSON�������Ϳ����ݶ���
*           NULL : ʧ��
*******************************************************************************/
SAL_Json *SAL_jsonCreateArray(VOID)
{
    SAL_Json* item = NULL;

    item = SAL_jsonCreateNewItem();
    if (NULL == item)
    {
        SAL_ERROR("SAL_jsonCreateNewItem error!\n");
        return NULL;
    }

    item->jsonType = JSON_Array;
    return item;
}

/*******************************************************************************
* ������  : SAL_jsonCreateObject
* ��  ��  : ����һ��JSON�������Ϳ����ݶ���
* ��  ��  : - ��
* ��  ��  : ��
* ����ֵ  : ��NULL  : �ɹ�������ֵJSON�������Ϳ����ݶ���
*           NULL : ʧ��
*******************************************************************************/
SAL_Json *SAL_jsonCreateObject(VOID)
{
    SAL_Json* item = NULL;

    item = SAL_jsonCreateNewItem();
    if (NULL == item)
    {
        SAL_ERROR("SAL_jsonCreateNewItem error!\n");
        return NULL;
    }

    item->jsonType = JSON_Object;
    return item;
}

/*******************************************************************************
* ������  : SAL_jsonCreateIntArray
* ��  ��  : ����һ��JSON�����������Ͷ���
* ��  ��  : - numbers : ������������
*         : - count : �������ݸ���
* ��  ��  : ��
* ����ֵ  : ��NULL  : �ɹ�������ֵJSON�����������Ͷ���
*           NULL : ʧ��
*******************************************************************************/
SAL_Json *SAL_jsonCreateIntArray(const INT32 *numbers, INT32 count)
{
    SAL_Json* item = NULL;
    SAL_Json* tmp = NULL;
    SAL_Json* swap = NULL;
    INT32 i = 0;

    if (NULL == numbers || 0 >= count)
    {
        SAL_ERROR("input args error! numbers(%p) count(%d)\n", numbers, count);
        return NULL;
    }

    item = SAL_jsonCreateArray();
    if (NULL == item)
    {
        SAL_ERROR("SAL_jsonCreateArray error!\n");
        return NULL;
    }

    for (i = 0; i < count; ++i)
    {
        tmp = SAL_jsonCreateIntNumber(numbers[i]);
        if (NULL == tmp)
        {
            SAL_ERROR("SAL_jsonCreateIntNumber[%d] error!\n", i);
            SAL_jsonDelete(item);
            swap = NULL;
            return NULL;
        }
        
        if (0 == i)
        {
            item->child = tmp;
        }
        else
        {
            if (NULL == swap)
            {
                swap = tmp;
            }
            else
            {
                swap->next = tmp;
                tmp->prev = swap;
                swap = tmp;
            }
        }
    }

    item->jsonType = JSON_Array;

    return item;
}

/*******************************************************************************
* ������  : SAL_jsonCreateDoubleArray
* ��  ��  : ����һ��JSON�����������Ͷ���
* ��  ��  : - numbers : ������������
*         : - count : �������ݸ���
* ��  ��  : ��
* ����ֵ  : ��NULL  : �ɹ�������ֵJSON�����������Ͷ���
*           NULL : ʧ��
*******************************************************************************/
SAL_Json *SAL_jsonCreateDoubleArray(const FLOAT64 *numbers, INT32 count)
{
    SAL_Json* item = NULL;
    SAL_Json* tmp = NULL;
    SAL_Json* swap = NULL;
    INT32 i = 0;

    if (NULL == numbers || 0 >= count)
    {
        SAL_ERROR("input args error! numbers(%p) count(%d)\n", numbers, count);
        return NULL;
    }

    item = SAL_jsonCreateArray();
    if (NULL == item)
    {
        SAL_ERROR("SAL_jsonCreateNewItem error!\n");
        return NULL;
    }

    for (i = 0; i < count; ++i)
    {
        tmp = SAL_jsonCreateDoubleNumber(numbers[i]);
        if (NULL == tmp)
        {
            SAL_ERROR("SAL_jsonCreateDoubleNumber[%d] error!\n", i);
            SAL_jsonDelete(item);
            return NULL;
        }
        
        if (0 == i)
        {
            item->child = tmp;
        }
        else
        {
            if (NULL == swap)
            {
                swap = tmp;
            }
            else
            {
                swap->next = tmp;
                tmp->prev = swap;
                swap = tmp;
            }
        }
    }

    item->jsonType = JSON_Array;

    return item;
}

/*******************************************************************************
* ������  : SAL_jsonCreateStringArray
* ��  ��  : ����һ��JSON�����������Ͷ���
* ��  ��  : - numbers : ������������
*         : - count : �������ݸ���
* ��  ��  : ��
* ����ֵ  : ��NULL  : �ɹ�������ֵJSON�����������Ͷ���
*           NULL : ʧ��
*******************************************************************************/
SAL_Json *SAL_jsonCreateStringArray(const INT8 **strings, INT32 count)
{
    SAL_Json* item = NULL;
    SAL_Json* tmp = NULL;
    SAL_Json* swap = NULL;
    INT32 i = 0;

    if (NULL == strings || 0 >= count)
    {
        SAL_ERROR("input args error! srings(%p) count(%d)\n", strings, count);
        return NULL;
    }

    item = SAL_jsonCreateArray();
    if (NULL == item)
    {
        return NULL;
    }

    for (i = 0; i < count; ++i)
    {
        tmp = SAL_jsonCreateString(strings[i]);
        if (NULL == tmp)
        {
            SAL_ERROR("SAL_jsonCreateString[%d] error!\n", i);
            SAL_jsonDelete(item);
            return NULL;
        }
        
        if (0 == i)
        {
            item->child = tmp;
            swap = item->child;
        }
        else
        {
            if (NULL != swap)
            {
                swap->next = tmp;
                tmp->prev = swap;
                swap = tmp;
            }
        }
    }

    item->jsonType = JSON_Array;

    return item;
}

/*******************************************************************************
* ������  : SAL_jsonAddItemToArray
* ��  ��  : JSON�������Ͷ������һ��Ԫ��
* ��  ��  : - array : JSON�������Ͷ���
*         : - item : JSON�������ݶ���
* ��  ��  : ��
* ����ֵ  : SAL_SOK : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_jsonAddItemToArray(SAL_Json *array, SAL_Json *item)
{
    SAL_Json* child = NULL;

    if (NULL == array || NULL == item)
    {
        SAL_ERROR("input args error! array(%p) item(%p)\n", array, item);
        return SAL_FAIL;
    }

    child = array->child;
    if (NULL == child)
    {
        array->child = item;
        return SAL_SOK;
    }

    while (NULL != child && NULL != child->next)
    {
        child = child->next;
    }
    child->next = item;
    item->prev = child;

    return SAL_SOK;
}

/*******************************************************************************
* ������  : SAL_jsonAddItemToObject
* ��  ��  : JSON�������Ͷ������һ��Ԫ��
* ��  ��  : - object : Ҫ�����Ӷ���ĸ�JSON����
*         : - name : ��Ҫ�����Ķ�������keyֵ
*         : - item : Ҫ��ӵ���JSON����
* ��  ��  : ��
* ����ֵ  : SAL_SOK : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_jsonAddItemToObject(SAL_Json *object, const INT8 *name, SAL_Json *item)
{
    if (NULL == object || NULL == name || NULL == item)
    {
        SAL_ERROR("input args error! object(%p) name(%p) item(%p)\n", object, name, item);
        return SAL_FAIL;
    }

    if (NULL != item->key)
    {
        SAL_jsonFree(item->key);
    }

    item->key = SAL_jsonStrdup(name);
    
    return SAL_jsonAddItemToArray(object, item);
}

/*******************************************************************************
* ������  : SAL_jsonAddNullToObject
* ��  ��  : JSON�������Ͷ������һ��Ԫ��
* ��  ��  : - object : JSON�������Ͷ���
*         : - name : ��Ҫ�����Ķ�������keyֵ
* ��  ��  : ��
* ����ֵ  : SAL_SOK : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_jsonAddNullToObject(SAL_Json *object, const INT8 *name)
{
    if (NULL == object || NULL == name)
    {
        SAL_ERROR("input args error! object(%p) name(%p)\n", object, name);
        return SAL_FAIL;
    }

    return SAL_jsonAddItemToObject(object, name, SAL_jsonCreateNull());
}

/*******************************************************************************
* ������  : SAL_jsonAddTrueToObject
* ��  ��  : JSON�������Ͷ������һ��Ԫ��
* ��  ��  : - object : JSON�������Ͷ���
*         : - name : ��Ҫ�����Ķ�������keyֵ
* ��  ��  : ��
* ����ֵ  : SAL_SOK : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_jsonAddTrueToObject(SAL_Json *object, const INT8 *name)
{
    if (NULL == object || NULL == name)
    {
        SAL_ERROR("input args error! object(%p) name(%p)\n", object, name);
        return SAL_FAIL;
    }
    
    return SAL_jsonAddItemToObject(object, name, SAL_jsonCreateTrue());
}

/*******************************************************************************
* ������  : SAL_jsonAddFalseToObject
* ��  ��  : JSON�������Ͷ������һ��Ԫ��
* ��  ��  : - object : JSON�������Ͷ���
*         : - name : ��Ҫ�����Ķ�������keyֵ
* ��  ��  : ��
* ����ֵ  : SAL_SOK : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_jsonAddFalseToObject(SAL_Json *object, const INT8 *name)
{
    if (NULL == object || NULL == name)
    {
        SAL_ERROR("input args error! object(%p) name(%p)\n", object, name);
        return SAL_FAIL;
    }

    return SAL_jsonAddItemToObject(object, name, SAL_jsonCreateFalse());
}

/*******************************************************************************
* ������  : SAL_jsonAddBoolToObject
* ��  ��  : JSON�������Ͷ������һ��Ԫ��
* ��  ��  : - object : JSON�������Ͷ���
*         : - name : ��Ҫ�����Ķ�������keyֵ
*         : - b : boolֵ
* ��  ��  : ��
* ����ֵ  : SAL_SOK : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_jsonAddBoolToObject(SAL_Json *object, const INT8 *name, BOOL b)
{
    if (NULL == object || NULL == name)
    {
        SAL_ERROR("input args error! object(%p) name(%p) b(%d)\n", object, name, b);
        return SAL_FAIL;
    }
    
    return SAL_jsonAddItemToObject(object, name, SAL_jsonCreateBool(b));
}

/*******************************************************************************
* ������  : SAL_jsonAddIntNumberToObject
* ��  ��  : JSON�������Ͷ������һ��������
* ��  ��  : - object : JSON�������Ͷ���
*         : - name : ��Ҫ�����Ķ�������keyֵ
*         : - n : ����ֵ
* ��  ��  : ��
* ����ֵ  : SAL_SOK : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_jsonAddIntNumberToObject(SAL_Json *object, const INT8 *name, INT32 n)
{
    if (NULL == object || NULL == name)
    {
        SAL_ERROR("input args error! object(%p) name(%p) n(%d)\n", object, name, n);
        return SAL_FAIL;
    }
    
    return SAL_jsonAddItemToObject(object, name, SAL_jsonCreateIntNumber(n));
}

/*******************************************************************************
* ������  : SAL_jsonAddDoubleNumberToObject
* ��  ��  : JSON�������Ͷ������һ��������
* ��  ��  : - object : JSON�������Ͷ���
*         : - name : ��Ҫ�����Ķ�������keyֵ
*         : - n : ����ֵ
* ��  ��  : ��
* ����ֵ  : SAL_SOK : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_jsonAddDoubleNumberToObject(SAL_Json *object, const INT8 *name, FLOAT64 n)
{
    if (NULL == object || NULL == name)
    {
        SAL_ERROR("input args error! object(%p) name(%p) n(%lf)\n", object, name, n);
        return SAL_FAIL;
    }
    
    return SAL_jsonAddItemToObject(object, name, SAL_jsonCreateDoubleNumber(n));
}

/*******************************************************************************
* ������  : SAL_jsonAddStringToObject
* ��  ��  : JSON�������Ͷ������һ���ַ�����
* ��  ��  : - object : JSON�������Ͷ���
*         : - name : ��Ҫ�����Ķ�������keyֵ
*         : - s : �ַ�����
* ��  ��  : ��
* ����ֵ  : SAL_SOK : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_jsonAddStringToObject(SAL_Json *object, const INT8 *name, const INT8 *s)
{
    if (NULL == object || NULL == name || NULL == s)
    {
        SAL_ERROR("input args error! object(%p) name(%p) s(%p)\n", object, name, s);
        return SAL_FAIL;
    }
    
    return SAL_jsonAddItemToObject(object, name, SAL_jsonCreateString(s));
}

/*******************************************************************************
* ������  : SAL_jsonDetachItemFromArray
* ��  ��  : ��JSON�������Ͷ����Ƴ�ָ��λ�õ�JSON����
* ��  ��  : - array : JSON�������Ͷ���
*         : - which : ��Ҫ�Ƴ�������λ��
* ��  ��  : ��
* ����ֵ  : ��NULL : �ɹ����Ƴ������ݣ���Ҫ�ֶ��ͷ���Դ
*           NULL : ʧ��
*******************************************************************************/
SAL_Json *SAL_jsonDetachItemFromArray(SAL_Json *array, INT32 which)
{
    SAL_Json* tmp = NULL;
    if (NULL == array || 0 > which)
    {
        SAL_ERROR("input args error! array(%p) which(%d)\n", array, which);
        return NULL;
    }

    tmp = array->child;
    while (NULL != tmp && 0 < which)
    {
        tmp = tmp->next;
        which--;
    }

    if (NULL == tmp)
    {
        return NULL;
    }

    if (NULL != tmp->prev)
    {
        tmp->prev->next = tmp->next;
    }

    if (NULL != tmp->next)
    {
        tmp->next->prev = tmp->prev;
    }

    if (tmp == array->child)
    {
        array->child = tmp->next;
    }

    tmp->prev = NULL;
    tmp->next = NULL;

    return tmp;
}

/*******************************************************************************
* ������  : SAL_jsonDeleteItemFromArray
* ��  ��  : ��JSON�������Ͷ���ɾ��ָ��λ�õ�JSON����
* ��  ��  : - array : JSON�������Ͷ���
*         : - which : ��Ҫ�Ƴ�������λ��
* ��  ��  : ��
* ����ֵ  : SAL_SOK : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_jsonDeleteItemFromArray(SAL_Json *array, INT32 which)
{
    SAL_Json* tmp = NULL;

    if (NULL == array || 0 > which)
    {
        SAL_WARN("input args error! array(%p) which(%d)\n", array, which);
        return SAL_FAIL;
    }

    tmp = SAL_jsonDetachItemFromArray(array, which);
    if (NULL == tmp)
    {
        return SAL_FAIL;
    }

    SAL_jsonDelete(tmp);
    tmp = NULL;

    return SAL_SOK;
}

/*******************************************************************************
* ������  : SAL_jsonDetachItemFromArray
* ��  ��  : ��JSON�������Ͷ����Ƴ�ָ��λ�õ�JSON����
* ��  ��  : - array : JSON�������Ͷ���
*         : - name : ��Ҫ�Ƴ�������keyֵ
* ��  ��  : ��
* ����ֵ  : ��NULL : �ɹ����Ƴ������ݣ���Ҫ�ֶ��ͷ���Դ
*           NULL : ʧ��
*******************************************************************************/
SAL_Json *SAL_jsonDetachItemFromObject(SAL_Json *object, const INT8 *name)
{
    SAL_Json* tmp = NULL;
    INT32 i = 0;

    if (NULL == object || NULL == name)
    {
        SAL_ERROR("input args error! object(%p) name(%p)\n", object, name);
        return NULL;
    }

    tmp = object->child;

    while (NULL != tmp && 0 != strcasecmp(tmp->key, name))
    {
        tmp = tmp->next;
        i++;
    }

    if (NULL == tmp)
    {
        return NULL;
    }

    return SAL_jsonDetachItemFromArray(object, i);
}

/*******************************************************************************
* ������  : SAL_jsonDeleteItemFromObject
* ��  ��  : ��JSON�������Ͷ���ɾ��ָ��λ�õ�JSON����
* ��  ��  : - array : JSON�������Ͷ���
*         : - name : ��Ҫɾ��������keyֵ
* ��  ��  : ��
* ����ֵ  : SAL_SOK : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_jsonDeleteItemFromObject(SAL_Json *object, const INT8 *name)
{
    SAL_Json* tmp = NULL;

    if (NULL == object || NULL == name)
    {
        SAL_WARN("input args error! object(%p) name(%p)\n", object, name);
        return SAL_FAIL;
    }

    tmp = SAL_jsonDetachItemFromObject(object, name);
    if (NULL == tmp)
    {
        return SAL_FAIL;
    }

    SAL_jsonDelete(tmp);
    tmp = NULL;

    return SAL_SOK;
}

/*******************************************************************************
* ������  : SAL_jsonGetItemFromObject
* ��     ��  : ��JSON�������ҵ�ָ�����Ӷ���
* ��     ��  : - *pObject : JSON����
*                   : - pKeyName : ��Ҫ��ȡ���Ӷ���keyֵ
* ��     ��  : - **ppItem:��ȡ�õ����Ӷ���
* ����ֵ  : SAL_SOK : �ɹ�
*                     SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_jsonGetItemFromObject(SAL_Json *pObject, const INT8 *pKeyName, SAL_Json **ppItem)
{
    INT8        *pKey  = NULL;
    SAL_Json    *pTmp  = NULL;
    
    if (NULL == pObject || NULL == pKeyName || NULL == ppItem)
    {
        SAL_ERROR("input args error! object(%p) name(%p) item(%p)\n", pObject, pKeyName, ppItem);
        return SAL_FAIL;
    }

    pTmp = pObject;
    pKey = pTmp->key;//params
    if(NULL == pKey)
    {
        SAL_ERROR("key %p\n", pKey);
        return SAL_FAIL;
    }

    do
    {
        if(JSON_Object == pTmp->jsonType && (0 == strncmp(pKey, pKeyName, strlen(pKeyName)))) //pKeyName=param
        {
            if(NULL == pTmp->child)
            {
                SAL_ERROR("key %s, child %p\n", pKey, pTmp->child);
                return SAL_FAIL;
            }
            *ppItem = pTmp->child;
            break;
        }
        if(NULL == pTmp->next)
        {
            SAL_ERROR("find object(key %s) failed!\n", pKeyName);
            return SAL_FAIL;
        }
        pTmp = pTmp->next;

        pKey = pTmp->key;
        if(NULL == pKey)
        {
            SAL_ERROR("key %p\n", pKey);
            return SAL_FAIL;
        }

    }while(1);

    return SAL_SOK;
}

/*******************************************************************************
* ������  : SAL_jsonGetBoolFromObject
* ��     ��  : ��JSON�������ҵ�ָ����BOOL���Ͷ���ֵ
* ��     ��  : - *pObject : JSON����
*                   : - pKeyName : ��Ҫ��ȡ���Ӷ���keyֵ
* ��     ��  : - **ppItem:��ȡ�õ����Ӷ���
* ����ֵ  : SAL_SOK : �ɹ�
*                     SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_jsonGetBoolFromObject(SAL_Json *pObject, const INT8 *pKeyName, BOOL *pValue)
{
    INT8        *pKey  = NULL;
    SAL_Json    *pTmp  = NULL;
    
    if (NULL == pObject || NULL == pKeyName || NULL == pValue)
    {
        SAL_ERROR("input args error! object(%p) name(%p) item(%p)\n", pObject, pKeyName, pValue);
        return SAL_FAIL;
    }

    pTmp = pObject;
    pKey = pTmp->key;
    if(NULL == pKey)
    {
        SAL_ERROR("key %p\n", pKey);
        return SAL_FAIL;
    }

    do
    {
        if((JSON_True == pTmp->jsonType || JSON_False == pTmp->jsonType) && (0 == strncmp(pKey, pKeyName, strlen(pKeyName))))
        {
            *pValue = (JSON_True == pTmp->jsonType) ? SAL_TRUE: SAL_FALSE;
            break;
        }
        if(NULL == pTmp->next)
        {
            SAL_ERROR("find object(key %s) failed!\n", pKeyName);
            return SAL_FAIL;
        }
        pTmp = pTmp->next;

        pKey = pTmp->key;
        if(NULL == pKey)
        {
            SAL_ERROR("key %p\n", pKey);
            return SAL_FAIL;
        }

    }while(1);

    return SAL_SOK;
}

/*******************************************************************************
* ������  : SAL_jsonGetIntNumberFromObject
* ��     ��  : ��JSON�������ҵ�ָ����INT���Ͷ���ֵ
* ��     ��  : - *pObject : JSON����
*                   : - pKeyName : ��Ҫ��ȡ���Ӷ���keyֵ
* ��     ��  : - **ppItem:��ȡ�õ����Ӷ���
* ����ֵ  : SAL_SOK : �ɹ�
*                     SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_jsonGetIntNumberFromObject(SAL_Json *pObject, const INT8 *pKeyName, INT32 *pValue)
{
    INT8        *pKey  = NULL;
    SAL_Json    *pTmp  = NULL;
    
    if (NULL == pObject || NULL == pKeyName || NULL == pValue)
    {
        SAL_ERROR("input args error! object(%p) name(%p) item(%p)\n", pObject, pKeyName, pValue);
        return SAL_FAIL;
    }

    pTmp = pObject;
    pKey = pTmp->key;
    if(NULL == pKey)
    {
        SAL_ERROR("key %p\n", pKey);
        return SAL_FAIL;
    }
    
    do
    {
        if(JSON_Number == pTmp->jsonType && (0 == strncmp(pKey, pKeyName, strlen(pKeyName))))
        {
            *pValue = pTmp->intValue;
            break;
        }
        
        if(NULL == pTmp->next)
        {
            SAL_ERROR("find object(key %s) failed!\n", pKeyName);
            return SAL_FAIL;
        }
        pTmp = pTmp->next;

        pKey = pTmp->key;
        if(NULL == pKey)
        {
            SAL_ERROR("key %p\n", pKey);
            return SAL_FAIL;
        }

    }while(1);

    return SAL_SOK;
}

/*******************************************************************************
* ������  : SAL_jsonGetIntNumberFromObject
* ��     ��  : ��JSON�������ҵ�ָ����Double���Ͷ���ֵ
* ��     ��  : - *pObject : JSON����
*                   : - pKeyName : ��Ҫ��ȡ���Ӷ���keyֵ
* ��     ��  : - **ppItem:��ȡ�õ����Ӷ���
* ����ֵ  : SAL_SOK : �ɹ�
*                     SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_jsonGetDoubleNumberFromObject(SAL_Json *pObject, const INT8 *pKeyName, FLOAT64 *pValue)
{
    INT8        *pKey  = NULL;
    SAL_Json    *pTmp  = NULL;
    
    if (NULL == pObject || NULL == pKeyName || NULL == pValue)
    {
        SAL_ERROR("input args error! object(%p) name(%p) item(%p)\n", pObject, pKeyName, pValue);
        return SAL_FAIL;
    }

    pTmp = pObject;
    pKey = pTmp->key;
    if(NULL == pKey)
    {
        SAL_ERROR("key %p\n", pKey);
        return SAL_FAIL;
    }

    do
    {
        if(JSON_Number == pTmp->jsonType && (0 == strncmp(pKey, pKeyName, strlen(pKeyName))))
        {
            *pValue = pTmp->doubleValue;
            break;
        }
        if(NULL == pTmp->next)
        {
            SAL_ERROR("find object(key %s) failed!\n", pKeyName);
            return SAL_FAIL;
        }
        pTmp = pTmp->next;

        pKey = pTmp->key;
        if(NULL == pKey)
        {
            SAL_ERROR("key %p\n", pKey);
            return SAL_FAIL;
        }
        
    }while(1);

    return SAL_SOK;
}

/*******************************************************************************
* ������  : SAL_jsonGetIntNumberFromObject
* ��     ��  : ��JSON�������ҵ�ָ����String���Ͷ���ֵ
* ��     ��  : - *pObject : JSON����
*                   : - pKeyName : ��Ҫ��ȡ���Ӷ���keyֵ
* ��     ��  : - **pValue:��ȡ�õ���String
* ����ֵ  : SAL_SOK : �ɹ�
*                     SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_jsonGetStringFromObject(SAL_Json *pObject, const INT8 *pKeyName, INT8 **ppString)
{
    INT8        *pKey  = NULL;
    SAL_Json    *pTmp  = NULL;
    
    if (NULL == pObject || NULL == pKeyName || NULL == ppString)
    {
        SAL_ERROR("input args error! object(%p) name(%p) item(%p)\n", pObject, pKeyName, ppString);
        return SAL_FAIL;
    }

    pTmp = pObject;
    pKey = pTmp->key;
    if(NULL == pKey)
    {
        SAL_ERROR("key %p\n", pKey);
        return SAL_FAIL;
    }

    do
    {
        if(JSON_String == pTmp->jsonType && (0 == strncmp(pKey, pKeyName, strlen(pKeyName))))
        {
            if(NULL == pTmp->strValue)
            {
                SAL_ERROR("key %s, strValue %p\n", pKey, pTmp->strValue);
            }
            *ppString = pTmp->strValue;
            break;
        }
        if(NULL == pTmp->next)
        {
            SAL_ERROR("find object(key %s) failed!\n", pKeyName);
            return SAL_FAIL;
        }
        pTmp = pTmp->next;

        pKey = pTmp->key;
        if(NULL == pKey)
        {
            SAL_ERROR("key %p\n", pKey);
            return SAL_FAIL;
        }
        
    }while(1);

    return SAL_SOK;
}

/*******************************************************************************
* ������  : SAL_jsonGetIntNumberFromObject
* ��     ��  : ��JSON�������ҵ�ָ����String���Ͷ���ֵ
* ��     ��  : - *pObject : JSON����
*                   : - pKeyName : ��Ҫ��ȡ���Ӷ���keyֵ
* ��     ��  : - **pValue:��ȡ�õ���String
* ����ֵ  : SAL_SOK : �ɹ�
*                     SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_jsonGetArrayFromObject(SAL_Json *pObject, const INT8 *pKeyName, SAL_Json **ppArrayItem)
{
    INT8        *pKey  = NULL;
    SAL_Json    *pTmp  = NULL;
    
    if (NULL == pObject || NULL == pKeyName || NULL == ppArrayItem)
    {
        SAL_ERROR("input args error! object(%p) name(%p) item(%p)\n", pObject, pKeyName, ppArrayItem);
        return SAL_FAIL;
    }

    pTmp = pObject;
    pKey = pTmp->key;
    if(NULL == pKey)
    {
        SAL_ERROR("key %p\n", pKey);
        return SAL_FAIL;
    }

    do
    {
        if(JSON_Array == pTmp->jsonType && (0 == strncmp(pKey, pKeyName, strlen(pKeyName))))
        {
            if(NULL == pTmp->child)
            {
                SAL_ERROR("key %s, child %p\n", pKey, pTmp->child);
                return SAL_FAIL;
            }
            *ppArrayItem = pTmp->child;
            break;
        }
        if(NULL == pTmp->next)
        {
            SAL_ERROR("find object(key %s) failed!\n", pKeyName);
            return SAL_FAIL;
        }
        pTmp = pTmp->next;

        pKey = pTmp->key;
        if(NULL == pKey)
        {
            SAL_ERROR("key %p\n", pKey);
            return SAL_FAIL;
        }
        
    }while(1);

    return SAL_SOK;
}


// Json�ַ���������Demo����data�н�Ϊ���ӵ�Json�ַ���Ϊ��
static int jsonReadTest(void)
{
    const INT8* data = \
        "{\n"
        "    \"name\": \"China\",\n"
        "    \"province\": [\n"
        "        {\n"
        "            \"name\": \"Heilongjiang\",\n"
        "            \"cities\": {\n"
        "                \"city\": [\n"
        "                    \"Haerbin\",\n"
        "                    \"Daqing\"\n"
        "                ]\n"
        "            }\n"
        "        },\n"
        "        {\n"
        "            \"name\": \"Guangdong\",\n"
        "            \"cities\": {\n"
        "                \"city\": [\n"
        "                    \"Guangzhou\",\n"
        "                    \"Shenzhen\",\n"
        "                    \"Zhuhai\"\n"
        "                ]\n"
        "            }\n"
        "        },\n"
        "        {\n"
        "            \"name\": \"Taiwan\",\n"
        "            \"cities\": {\n"
        "                \"city\": [\n"
        "                    \"Taibei\",\n"
        "                    \"Gaoxiong\"\n"
        "                ]\n"
        "            }\n"
        "        },\n"
        "        {\n"
        "            \"name\": \"Xinjiang\",\n"
        "            \"cities\": {\n"
        "                \"city\": [\n"
        "                    \"Wulumuqi\"\n"
        "                ]\n"
        "            }\n"
        "        }\n"
        "    ],\n"
        "    \"test1\": false,\n"
        "    \"test2\": true,\n"
        "    \"test3\": null\n"
        "}\"";
    long len = 0;
    INT8* out = NULL;
    SAL_Json* json = NULL;

    len = strlen(data);

    //
    json = SAL_jsonRead(data);
    if (NULL == json)
    {
        printf("--- %s:failed!\n", __FUNCTION__);
        return -1;
    }
    
    out = SAL_jsonWrite(json, 1);
    SAL_jsonDelete(json);
    printf("%s\n", out);
    printf("\n");

    if (NULL != out)
    {
        SAL_jsonFree(out);
        out = NULL;
    }

    return 0;
}

static int jsonWriteTest(void)
{
    BOOL createFailed = SAL_FALSE;
    SAL_Json* json = NULL;
    INT8* out = NULL;

    SAL_Json* tmp = NULL;
    
    SAL_Json* city = NULL;
    SAL_Json* array = NULL;

    INT32 i = 0;
    INT32 arrayCount = 0;
    SAL_Json* provinceArray[4] = {0};

    json = SAL_jsonCreateObject();
    if (NULL == json)
    {
        return -1;
    }

    createFailed = SAL_TRUE;
    do{
        {
            const INT8* cityName[] = {"Haerbin", "Daqing"};
            array = SAL_jsonCreateStringArray(cityName, sizeof(cityName) / sizeof(cityName[0]));
            if (NULL == array)
            {
                break;
            }

            city = SAL_jsonCreateObject();
            if (NULL == city)
            {
                SAL_jsonDelete(array);
                array = NULL;
                break;
            }
            
            if (SAL_SOK != SAL_jsonAddItemToObject(city, "city", array))
            {
                SAL_jsonDelete(array);
                array = NULL;

                SAL_jsonDelete(city);
                city = NULL;
                
                break;
            }

            tmp = SAL_jsonCreateObject();
            if (NULL == tmp)
            {
                SAL_jsonDelete(city);
                city = NULL;
                break;
            }
            
            if (SAL_SOK != SAL_jsonAddStringToObject(tmp, "name", "Heilongjiang"))
            {
                SAL_jsonDelete(city);
                city = NULL;

                SAL_jsonDelete(tmp);
                tmp = NULL;
                break;
            }
            
            if (SAL_SOK != SAL_jsonAddItemToObject(tmp, "cities", city))
            {
                SAL_jsonDelete(city);
                city = NULL;

                SAL_jsonDelete(tmp);
                tmp = NULL;
                break;
            }

            provinceArray[arrayCount] = tmp;
            arrayCount++;

            tmp = NULL;
            city = NULL;
            array = NULL;
        }


        {
            const INT8* cityName[] = {"Guangzhou", "Shenzhen", "Zhuhai"};
            array = SAL_jsonCreateStringArray(cityName, sizeof(cityName) / sizeof(cityName[0]));
            if (NULL == array)
            {
                break;
            }

            city = SAL_jsonCreateObject();
            if (NULL == city)
            {
                SAL_jsonDelete(array);
                array = NULL;
                break;
            }
            
            if (SAL_SOK != SAL_jsonAddItemToObject(city, "city", array))
            {
                SAL_jsonDelete(array);
                array = NULL;

                SAL_jsonDelete(city);
                city = NULL;
                
                break;
            }

            tmp = SAL_jsonCreateObject();
            if (NULL == tmp)
            {
                SAL_jsonDelete(city);
                city = NULL;
                break;
            }
            
            if (SAL_SOK != SAL_jsonAddStringToObject(tmp, "name", "Guangdong"))
            {
                SAL_jsonDelete(city);
                city = NULL;

                SAL_jsonDelete(tmp);
                tmp = NULL;
                break;
            }

            if (SAL_SOK != SAL_jsonAddItemToObject(tmp, "cities", city))
            {
                SAL_jsonDelete(city);
                city = NULL;

                SAL_jsonDelete(tmp);
                tmp = NULL;
                break;
            }

            provinceArray[arrayCount] = tmp;
            arrayCount++;
            
            tmp = NULL;
            city = NULL;
            array = NULL;
        }

        {
            const INT8* cityName[] = {"Taibei", "Gaoxiong"};
            array = SAL_jsonCreateStringArray(cityName, sizeof(cityName) / sizeof(cityName[0]));
            if (NULL == array)
            {
                break;
            }

            city = SAL_jsonCreateObject();
            if (NULL == city)
            {
                SAL_jsonDelete(array);
                array = NULL;
                break;
            }
            
            if (SAL_SOK != SAL_jsonAddItemToObject(city, "city", array))
            {
                SAL_jsonDelete(array);
                array = NULL;

                SAL_jsonDelete(city);
                city = NULL;
                
                break;
            }

            tmp = SAL_jsonCreateObject();
            if (NULL == tmp)
            {
                SAL_jsonDelete(city);
                city = NULL;
                break;
            }
            
            if (SAL_SOK != SAL_jsonAddStringToObject(tmp, "name", "Taiwan"))
            {
                SAL_jsonDelete(city);
                city = NULL;

                SAL_jsonDelete(tmp);
                tmp = NULL;
                break;
            }

            if (SAL_SOK != SAL_jsonAddItemToObject(tmp, "cities", city))
            {
                SAL_jsonDelete(city);
                city = NULL;

                SAL_jsonDelete(tmp);
                tmp = NULL;
                break;
            }

            provinceArray[arrayCount] = tmp;
            arrayCount++;
            
            tmp = NULL;
            city = NULL;
            array = NULL;
        }

        {
            const INT8* cityName[] = {"Wulumuqi"};
            array = SAL_jsonCreateStringArray(cityName, sizeof(cityName) / sizeof(cityName[0]));
            if (NULL == array)
            {
                break;
            }

            city = SAL_jsonCreateObject();
            if (NULL == city)
            {
                SAL_jsonDelete(array);
                array = NULL;
                break;
            }
            
            if (SAL_SOK != SAL_jsonAddItemToObject(city, "city", array))
            {
                SAL_jsonDelete(array);
                array = NULL;

                SAL_jsonDelete(city);
                city = NULL;
                
                break;
            }

            tmp = SAL_jsonCreateObject();
            if (NULL == tmp)
            {
                SAL_jsonDelete(city);
                city = NULL;
                break;
            }

            if (SAL_SOK != SAL_jsonAddStringToObject(tmp, "name", "Xinjiang"))
            {
                SAL_jsonDelete(city);
                city = NULL;

                SAL_jsonDelete(tmp);
                tmp = NULL;
                break;
            }
            
            if (SAL_SOK != SAL_jsonAddItemToObject(tmp, "cities", city))
            {
                SAL_jsonDelete(city);
                city = NULL;

                SAL_jsonDelete(tmp);
                tmp = NULL;
                break;
            }

            provinceArray[arrayCount] = tmp;
            arrayCount++;
            
            tmp = NULL;
            city = NULL;
            array = NULL;
        }

        createFailed = SAL_FALSE;
    } while(0);

    tmp = SAL_jsonCreateArray();
    for (i = 0; i < arrayCount; ++i)
    {
        if (SAL_SOK == SAL_jsonAddItemToArray(tmp, provinceArray[i]))
        {
            provinceArray[i] = NULL;
        }
        else
        {
            createFailed = SAL_TRUE;
            break;
        }
    }

    // �����ɹ�
    if (SAL_FALSE == createFailed)
    {
        if(SAL_SOK != SAL_jsonAddStringToObject(json, "name", "China"))
        {
            if (NULL != tmp)
                SAL_jsonDelete(tmp);    
            if (NULL != json)
                SAL_jsonDelete(json);
            for (i = 0; i < arrayCount; ++i)
                if (NULL != provinceArray[i])
                {
                    SAL_jsonDelete(provinceArray[i]);
                    provinceArray[i] = NULL;
                }
            return -1;
        }
        if(SAL_SOK != SAL_jsonAddItemToObject(json, "province", tmp))
        {
            if (NULL != tmp)
                SAL_jsonDelete(tmp);    
            if (NULL != json)
                SAL_jsonDelete(json);
            for (i = 0; i < arrayCount; ++i)
                if (NULL != provinceArray[i])
                {
                    SAL_jsonDelete(provinceArray[i]);
                    provinceArray[i] = NULL;
                }
            return -1;
        }
        if(SAL_SOK != SAL_jsonAddBoolToObject(json, "test1", SAL_TRUE))
        {
            if (NULL != tmp)
                SAL_jsonDelete(tmp);    
            if (NULL != json)
                SAL_jsonDelete(json);
            for (i = 0; i < arrayCount; ++i)
                if (NULL != provinceArray[i])
                {
                    SAL_jsonDelete(provinceArray[i]);
                    provinceArray[i] = NULL;
                }
            return -1;
        }
        if(SAL_SOK != SAL_jsonAddBoolToObject(json, "test2", SAL_FALSE))
        {
            if (NULL != tmp)
                SAL_jsonDelete(tmp);    
            if (NULL != json)
                SAL_jsonDelete(json);
            for (i = 0; i < arrayCount; ++i)
                if (NULL != provinceArray[i])
                {
                    SAL_jsonDelete(provinceArray[i]);
                    provinceArray[i] = NULL;
                }
            return -1;
        }
        if(SAL_SOK != SAL_jsonAddNullToObject(json, "test3"))
        {
            if (NULL != tmp)
                SAL_jsonDelete(tmp);    
            if (NULL != json)
                SAL_jsonDelete(json);
            for (i = 0; i < arrayCount; ++i)
                if (NULL != provinceArray[i])
                {
                    SAL_jsonDelete(provinceArray[i]);
                    provinceArray[i] = NULL;
                }
            return -1;
        }
    }
    // ���ڲ���ʧ��
    else
    {
        if (NULL != tmp)
        {
            SAL_jsonDelete(tmp);
        }

        if (NULL != json)
        {
            SAL_jsonDelete(json);
        }

        for (i = 0; i < arrayCount; ++i)
        {
            if (NULL != provinceArray[i])
            {
                SAL_jsonDelete(provinceArray[i]);
                provinceArray[i] = NULL;
            }
        }

        return -1;
    }

    out = SAL_jsonWrite(json, 1);
    if (NULL != out)
    {
        printf("%s\n", out);
        SAL_jsonFree(out);
    }

    SAL_jsonDelete(json);

    return 0;
}


