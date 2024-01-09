#include "test_common.h"
#include "cl_list.h"
#include "cl_pool.h"

static CL_List_t testList;

typedef struct
{
    CL_ListNode_t node;
    int a, b;
} TestStruct_t;

#define TEST_POOL_SIZE (202) //this number should be even
CL_POOL_DEFINE(testPool, TEST_POOL_SIZE, TestStruct_t, static);
CL_Result_t ListTest(void)
{
    CL_PoolInit(&testPool);
    CL_ListInit(&testList);

    TestStruct_t *ts;
    CL_ListNode_t *pNode;
    TestStruct_t *pCtn;

    //add 0~200
    for (int i = 100; i < TEST_POOL_SIZE; i++)
    {
        ts = CL_POOL_ALLOC(&testPool, TestStruct_t);
        ts->a = i;
        ts->b = i;
        CL_ListAddLast(&testList, &ts->node);
    }
    if (testList.size != TEST_POOL_SIZE - 100)
        return CL_ResFailed;

    for (int i = 99; i >= 0; i--)
    {
        ts = CL_POOL_ALLOC(&testPool, TestStruct_t);
        ts->a = ts->b = i;
        CL_ListAddFirst(&testList, &ts->node);
    }
    if (testList.size != TEST_POOL_SIZE)
        return CL_ResFailed;

    for (int i = 0; i < TEST_POOL_SIZE; i++)
    {
        pNode = CL_ListGetByIdx(&testList, i);
        pCtn = CL_CONTAINER_OF(pNode, TestStruct_t, node);
        if(pCtn->a != i || pCtn->b != i)
        {
            return CL_ResFailed;
        }
    }

    int count = 0;
    CL_LIST_FOR_EACH(&testList, pNode)
    {
        pCtn = CL_CONTAINER_OF(pNode, TestStruct_t, node);
        if (pCtn->a != count || pCtn->b != count)
        {
            return CL_ResFailed;
        }
        // printf("%d\n", pCtn->a);
        count++;
    }

    //remove even number
    CL_LIST_FOR_EACH_SAFE(&testList, pNode)
    {
        pCtn = CL_CONTAINER_OF(pNode, TestStruct_t, node);
        if (pCtn->a % 2 == 0)
        {
            if (CL_ListRemove(&testList, pNode) != CL_ResSuccess)
            {
                return CL_ResFailed;
            }
            if (CL_POOL_FREE(&testPool, pCtn) != CL_ResSuccess)
            {
                return CL_ResFailed;
            }
            // printf("ctn: %d, node: %d\n", pCtn, pNode);
        }
    }
    if (testList.size != (TEST_POOL_SIZE / 2))
        return CL_ResFailed;

    count = 1;
    CL_LIST_FOR_EACH(&testList, pNode)
    {
        pCtn = CL_CONTAINER_OF(pNode, TestStruct_t, node);
        if (pCtn->a != count || pCtn->b != count)
        {
            return CL_ResFailed;
        }
        // printf("%d\n", pCtn->a);
        count += 2;
    }

    count = 1;
    pNode = CL_ListGetByIdx(&testList, 0);
    while(pNode != CL_NULL)
    {
        pCtn = CL_CONTAINER_OF(pNode, TestStruct_t, node);
        if(pCtn->a != count || pCtn->b != count)
        {
            return CL_ResFailed;
        }
        count += 2;
        pNode = CL_ListMoveNext(pNode, false);
    }
    if(count != TEST_POOL_SIZE + 1)
    {
        return CL_ResFailed;
    }


    CL_LIST_FOR_EACH_REVERSE_SAFE(&testList, pNode)
    {
        pCtn = CL_CONTAINER_OF(pNode, TestStruct_t, node);
        ts = CL_POOL_ALLOC(&testPool, TestStruct_t);
        ts->a = ts->b = pCtn->a - 1;
        CL_ListAddBefore(pNode, &ts->node);
        // DebugLog("add: %d\n", ts->a);
    }
    if (testList.size != TEST_POOL_SIZE)
        return CL_ResFailed;

    pNode = CL_ListGetByIdx(&testList, TEST_POOL_SIZE / 2);
    count = TEST_POOL_SIZE / 2;
    for(int i = 0; i < TEST_POOL_SIZE * 5; i++)
    {
        pCtn = CL_CONTAINER_OF(pNode, TestStruct_t, node);
        // printf("%d\n", pCtn->a);
        if(pCtn->a != count || pCtn->b != count)
        {
            return CL_ResFailed;
        }
        pNode = CL_ListMoveNext(pNode, true);
        if(++count >= TEST_POOL_SIZE)
        {
            count = 0;
        }
    }    

    count = TEST_POOL_SIZE - 1;
    CL_LIST_FOR_EACH_REVERSE(&testList, pNode)
    {
        pCtn = CL_CONTAINER_OF(pNode, TestStruct_t, node);
        if (pCtn->a != count || pCtn->b != count)
        {
            return CL_ResFailed;
        }
        // printf("%d\n", pCtn->a);
        count--;
    }
    if (count != -1)
    {
        return CL_ResFailed;
    }

    //remove odd number
    CL_LIST_FOR_EACH_ENTRY_REVERSE_SAFE(&testList, pCtn, TestStruct_t, node)
    {
        if (pCtn->a % 2 != 0)
        {
            if (CL_ListRemove(&testList, &pCtn->node) != CL_ResSuccess)
            {
                return CL_ResFailed;
            }
            if (CL_POOL_FREE(&testPool, pCtn) != CL_ResSuccess)
            {
                return CL_ResFailed;
            }
        }
    }
    if (testList.size != (TEST_POOL_SIZE / 2))
        return CL_ResFailed;

    count = 0;
    CL_LIST_FOR_EACH_ENTRY(&testList, pCtn, TestStruct_t, node)
    {
        if (pCtn->a != count || pCtn->b != count)
        {
            return CL_ResFailed;
        }
        count += 2;
        // printf("%d\n", pCtn->a);
    }

    CL_LIST_FOR_EACH_ENTRY_SAFE(&testList, pCtn, TestStruct_t, node)
    {
        ts = CL_POOL_ALLOC(&testPool, TestStruct_t);
        ts->a = ts->b = pCtn->a + 1;
        CL_ListAddAfter(&pCtn->node, &ts->node);
    }

    pNode = CL_ListGetByIdx(&testList, TEST_POOL_SIZE / 3);
    count = TEST_POOL_SIZE / 3;
    for(int i = 0; i < TEST_POOL_SIZE * 10; i++)
    {
        pCtn = CL_CONTAINER_OF(pNode, TestStruct_t, node);
        if(pCtn->a != count || pCtn->b != count)
        {
            return CL_ResFailed;
        }
        pNode = CL_ListMovePrev(pNode, true);
        if(count > 0)
        {
            count--;
        }
        else
        {
            count = TEST_POOL_SIZE - 1;
        }
    }

    count = TEST_POOL_SIZE - 1;
    CL_LIST_FOR_EACH_ENTRY_REVERSE(&testList, pCtn, TestStruct_t, node)
    {
        if (pCtn->a != count || pCtn->b != count)
        {
            return CL_ResFailed;
        }
        count--;
        // printf("%d\n", pCtn->a);
    }
    if (count != -1)
    {
        return CL_ResFailed;
    }

    pNode = CL_ListGetByIdx(&testList, testList.size - 1);
    count = TEST_POOL_SIZE - 1;
    while(pNode != CL_NULL)
    {
        pCtn = CL_CONTAINER_OF(pNode, TestStruct_t, node);
        if (pCtn->a != count || pCtn->b != count)
        {
            return CL_ResFailed;
        }
        count--;
        pNode = CL_ListMovePrev(pNode, false);
    }
    if (count != -1)
    {
        return CL_ResFailed;
    }
    if(testList.size != TEST_POOL_SIZE)
    {
        return CL_ResFailed;
    }

    //remove all
    CL_LIST_FOR_EACH_REVERSE_SAFE(&testList, pNode)
    {
        pCtn = CL_CONTAINER_OF(pNode, TestStruct_t, node);
        if (CL_ListRemove(&testList, &pCtn->node) != CL_ResSuccess)
        {
            return CL_ResFailed;
        }
        if (CL_POOL_FREE(&testPool, pCtn) != CL_ResSuccess)
        {
            return CL_ResFailed;
        }
    }

    if(testList.size != 0)
    {
        return CL_ResFailed;
    }
    //list should be empty
    CL_LIST_FOR_EACH_ENTRY(&testList, pCtn, TestStruct_t, node)
    {
        return CL_ResFailed;
    }

    CL_LIST_FOR_EACH_REVERSE(&testList, pNode)
    {
        return CL_ResFailed;
    }

    return CL_PoolFullCheck(&testPool);
}

CL_Result_t EasyTest(void)
{
}

TestCase_t testCases[] = {
    {ListTest, "odd-even"},
};

int main(int argc, char **argv)
{
    TEST_CASE_PROC(testCases, "list");
    return 0;
}
