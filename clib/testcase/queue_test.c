#include "test_common.h"
#include "cl_queue.h"
#include "pthread.h"
#include "unistd.h"
#include <sys/select.h>


typedef struct
{
    uint8_t a;
    int16_t b;
    uint32_t c;
} TestStruct;

CL_QUEUE_DEF_INIT(foreach_q, 256, TestStruct, static);
//*************for each test*************************
static CL_Result_t ForeachTest(void)
{
    TestStruct testStruct;
    testStruct.a = 1;
    testStruct.b = 2;
    testStruct.c = 3;

    TestStruct temp;

    //************************
    //add something
    for (int i = 0; i < (CL_QueueCapacity(&foreach_q) * 2 + 100); i++)
    {

        if (CL_QueueFull(&foreach_q))
            CL_QueuePoll(&foreach_q, &temp);

        CL_QueueAdd(&foreach_q, &testStruct);
    }

    //flush with regular data
    for (int i = 0; i < CL_QueueCapacity(&foreach_q); i++)
    {
        testStruct.a = 0;
        testStruct.b = i;
        testStruct.c = i;

        if (CL_QueueFull(&foreach_q))
            CL_QueuePoll(&foreach_q, &temp);

        CL_QueueAdd(&foreach_q, &testStruct);
    }

    //verify regular data
    TestStruct *pTS = CL_NULL;
    int count = 0;
    CL_QUEUE_FOR_EACH(&foreach_q, pTS, TestStruct)
    {
        if ((pTS->a) != 0 || pTS->b != count || pTS->c != count)
            return CL_ResFailed;

        // printf("%d, %d, %d\n", pTS->a, pTS->b, pTS->c);
        count++;
    }

    //***************************
    //add regular data
    CL_QueueClear(&foreach_q);
    for (int i = 0; i < CL_QueueCapacity(&foreach_q) - 10; i++)
    {
        testStruct.a = 0;
        testStruct.b = i;
        testStruct.c = i;

        CL_QueueAdd(&foreach_q, &testStruct);
    }

    //verify regular data
    count = 0;
    CL_QUEUE_FOR_EACH(&foreach_q, pTS, TestStruct)
    {
        if ((pTS->a) != 0 || pTS->b != count || pTS->c != count)
            return CL_ResFailed;

        // printf("%d, %d, %d\n", pTS->a, pTS->b, pTS->c);
        count++;
    }

    return CL_ResSuccess;
}

//****************length etc.*************************************
#define LEN_TEST_CAP    (1024)
CL_QUEUE_DEF_INIT(len_q, LEN_TEST_CAP, int8_t, static);
CL_Result_t LengthTest(void)
{
    int8_t data;
    //cap
    if(CL_QueueCapacity(&len_q) != LEN_TEST_CAP)
    {
        return CL_ResFailed;
    }
    // printf("cap: %d\n", CL_QueueCapacity(&len_q));

    //full
    for (int i = 0; i < CL_QueueCapacity(&len_q); i++)
    {
        CL_QueueAdd(&len_q, &data);
    }

    if (CL_QueueLength(&len_q) != CL_QueueCapacity(&len_q) || CL_QueueFreeSpace(&len_q) != 0)
    {
        return CL_ResFailed;
    }
    // printf("full len: %d, free: %d\n", CL_QueueLength(&len_q), CL_QueueFreeSpace(&len_q));

    //half
    uint16_t halfCap = CL_QueueCapacity(&len_q) / 2;
    for (int i = 0; i < halfCap; i++)
    {
        CL_QueuePoll(&len_q, &data);
    }

    if (CL_QueueLength(&len_q) != halfCap || CL_QueueFreeSpace(&len_q) != halfCap)
    {
        return CL_ResFailed;
    }
    // printf("half len: %d, free: %d\n", CL_QueueLength(&len_q), CL_QueueFreeSpace(&len_q));    

    //not full
    for (int i = 0; i < CL_QueueCapacity(&len_q) * 2 + 30; i++)
    {
        if (CL_QueueFull(&len_q))
        {
            CL_QueuePoll(&len_q, &data);
        }

        CL_QueueAdd(&len_q, &data);
    }

    uint16_t pollCount = CL_QueueCapacity(&len_q) / 10;
    for (int i = 0; i < pollCount; i++)
    {
        CL_QueuePoll(&len_q, &data);
    }

    if (CL_QueueLength(&len_q) != (CL_QueueCapacity(&len_q) - pollCount) || CL_QueueFreeSpace(&len_q) != pollCount)
    {
        return CL_ResFailed;
    }
    // printf("part len: %d, free: %d\n", CL_QueueLength(&len_q), CL_QueueFreeSpace(&len_q));    


    return CL_ResSuccess;
}

//********************async test*******************************
void select_sleep(time_t sec, suseconds_t us)
{
    struct timeval timeout;

    timeout.tv_sec = sec;
    timeout.tv_usec = us;

    select(0, NULL, NULL, NULL, &timeout);
}

static bool slowPoll = false;
static CL_Result_t asyncTestResult = CL_ResSuccess;
#define LOOP_MAX_VAL 10
CL_QUEUE_DEF_INIT(async_q, 100, int, static);
static void *PollThread(void *arg)
{
    int lastVal = -1;
    while (1)
    {
        int pollVal;
        if (CL_QueuePoll(&async_q, &pollVal) == CL_ResSuccess)
        {
            int nextVal = (lastVal + 1) % LOOP_MAX_VAL;
            if (pollVal != nextVal)
            {
                asyncTestResult = CL_ResFailed;
                pthread_exit(NULL);
                return NULL;
            }
            lastVal = pollVal;

            // if(nextVal == 0)
            //     printf("\n");

            // printf("%d ", pollVal);
            // fflush(stdout);
        }

        if (slowPoll)
            select_sleep(0, 10000);
        else
            select_sleep(0, 1000);
    }
}

static CL_Result_t AsyncTest(void)
{
    pthread_t pollThr;
    pthread_create(&pollThr, NULL, PollThread, NULL);
    const int addTimes = 30000;
    int count = 0;
    while (1)
    {
        for (int i = 0; i < LOOP_MAX_VAL; i++)
        {
            while (CL_QueueAdd(&async_q, &i) != CL_ResSuccess)
            {
                select_sleep(0, 5000);
                if (asyncTestResult != CL_ResSuccess)
                {
                    goto out;
                }   
            }

            count++;
            if(count % 1000 == 0)
            {
                printf("add %d times\n", count);
            }
            if (count > addTimes / 2)
            {
                slowPoll = true;
            }
            if (asyncTestResult != CL_ResSuccess || count > addTimes)
            {
                goto out;
            }  
        }
    }

out: 
    pthread_cancel(pollThr);
    return asyncTestResult;
}


TestCase_t testCases[] = {
    {ForeachTest, "for each"},
    {LengthTest, "length"},
    {AsyncTest, "async"}
};

int main(int argc, char **argv)
{
    TEST_CASE_PROC(testCases, "queue");
    return 0;
}
