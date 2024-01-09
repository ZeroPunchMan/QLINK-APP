#include "cl_list.h"

void CL_ListInit(CL_List_t *list)
{
    list->head.preNode = &list->head;
    list->head.nextNode = &list->head;
    list->head.list = list;
    list->size = 0;
}

CL_Result_t CL_ListAddLast(CL_List_t *list, CL_ListNode_t *newNode)
{
    if (list == CL_NULL || newNode == CL_NULL)
        return CL_ResInvalidParam;

    CL_ListNode_t *head = &list->head;
    CL_ListNode_t *last = head->preNode;
    last->nextNode = newNode;
    newNode->preNode = last;
    newNode->nextNode = head;
    head->preNode = newNode;

    newNode->list = list;
    list->size++;

    return CL_ResSuccess;
}

CL_Result_t CL_ListAddFirst(CL_List_t *list, CL_ListNode_t *newNode)
{
    if (list == CL_NULL || newNode == CL_NULL)
        return CL_ResInvalidParam;

    CL_ListNode_t *head = &list->head;
    CL_ListNode_t *next = head->nextNode;
    head->nextNode = newNode;
    newNode->preNode = head;
    next->preNode = newNode;
    newNode->nextNode = next;

    newNode->list = list;
    list->size++;
    return CL_ResSuccess;
}

CL_Result_t CL_ListAddAfter(CL_ListNode_t *node, CL_ListNode_t *newNode)
{
    if (node == CL_NULL || newNode == CL_NULL)
        return CL_ResInvalidParam;

    if (node->preNode == CL_NULL || node->nextNode == CL_NULL || node->list == CL_NULL)
        return CL_ResInvalidParam;

    CL_ListNode_t *next = node->nextNode;
    node->nextNode = newNode;
    newNode->preNode = node;
    newNode->nextNode = next;
    next->preNode = newNode;

    newNode->list = node->list;
    newNode->list->size++;
    return CL_ResSuccess;
}

CL_Result_t CL_ListAddBefore(CL_ListNode_t *node, CL_ListNode_t *newNode)
{
    if (node == CL_NULL || newNode == CL_NULL)
        return CL_ResInvalidParam;

    if (node->preNode == CL_NULL || node->nextNode == CL_NULL || node->list == CL_NULL)
        return CL_ResInvalidParam;

    CL_ListNode_t *previous = node->preNode;
    previous->nextNode = newNode;
    newNode->preNode = previous;
    newNode->nextNode = node;
    node->preNode = newNode;

    newNode->list = node->list;
    newNode->list->size++;
    return CL_ResSuccess;
}

CL_Result_t CL_ListRemove(CL_List_t *list, CL_ListNode_t *node)
{
    if (list == CL_NULL || node == CL_NULL || list != node->list)
        return CL_ResInvalidParam;

    CL_ListNode_t *preNode;
    CL_ListNode_t *nextNode;
    preNode = node->preNode;
    nextNode = node->nextNode;

    preNode->nextNode = nextNode;
    nextNode->preNode = preNode;

    node->nextNode = CL_NULL;
    node->preNode = CL_NULL;
    node->list = CL_NULL;
    list->size--;

    return CL_ResSuccess;
}

bool CL_ListContain(CL_List_t *list, CL_ListNode_t *node)
{
    CL_ListNode_t *itr;
    CL_LIST_FOR_EACH(list, itr)
    {
        if (itr == node)
        {
            return true;
        }
    }
    return false;
}

CL_ListNode_t *CL_ListGetByIdx(CL_List_t *list, int idx)
{
    if (idx >= list->size)
        return CL_NULL;

    CL_ListNode_t *itr;
    int count = 0;
    CL_LIST_FOR_EACH(list, itr)
    {
        if (idx == count)
        {
            return itr;
        }
        count++;
    }

    return CL_NULL; //should not happen
}

CL_ListNode_t *CL_ListMoveNext(CL_ListNode_t *node, bool loop)
{
    CL_ListNode_t *nextNode = node->nextNode;
    if (nextNode == &node->list->head)
    {
        if (loop)
        {
            nextNode = nextNode->nextNode;
        }
        else
        {
            nextNode = CL_NULL;
        }
    }

    return nextNode;
}

CL_ListNode_t *CL_ListMovePrev(CL_ListNode_t *node, bool loop)
{
    CL_ListNode_t* preNode = node->preNode;
    if(preNode == &node->list->head)
    {
        if(loop)
        {
            preNode = preNode->preNode;
        }
        else
        {
            preNode = CL_NULL;
        }
    }
    return preNode;
}
