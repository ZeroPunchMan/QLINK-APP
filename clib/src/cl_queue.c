#include "cl_queue.h"
#include "string.h"

#define DATA_CPY(DST, SRC, SIZE) memcpy(DST, SRC, SIZE)

static inline uint16_t NextPos(uint16_t pos, uint16_t step, uint16_t max)
{
    pos = (pos + step) % (max + 1);
    return pos;
}

CL_Result_t CL_QueueAdd(CL_Queue_t *q, void *data)
{
    if (CL_QueueFull(q))
        return CL_ResFailed;

    DATA_CPY((char *)q->buff + (q->tail * q->data_size), data, q->data_size);
    q->tail = NextPos(q->tail, 1, q->capacity);

    return CL_ResSuccess;
}

CL_Result_t CL_QueueMultiAdd(CL_Queue_t *q, void *data, uint16_t len)
{
    if (CL_QueueFreeSpace(q) < len)
        return CL_ResFailed;

    if (q->tail >= q->head)
    {
        uint16_t ltoEnd = q->capacity + 1 - q->tail;
        if (ltoEnd >= len)
        {
            DATA_CPY((char *)q->buff + (q->tail * q->data_size), data, q->data_size * len);
            q->tail = NextPos(q->tail, len, q->capacity);
        }
        else
        {
            DATA_CPY((char *)q->buff + (q->tail * q->data_size), data, q->data_size * ltoEnd);
            uint16_t lRem = len - ltoEnd;
            DATA_CPY((char *)q->buff, (char *)data + (q->data_size * ltoEnd), q->data_size * lRem);
            q->tail = lRem;
        }
    }
    else
    {
        DATA_CPY((char *)q->buff + (q->tail * q->data_size), data, q->data_size * len);
        q->tail = NextPos(q->tail, len, q->capacity);
    }
    return CL_ResSuccess;
}

CL_Result_t CL_QueuePoll(CL_Queue_t *q, void *data)
{
    if (CL_QueueEmpty(q))
        return CL_ResFailed;

    if (data != CL_NULL)
        DATA_CPY(data, (char *)q->buff + (q->head * q->data_size), q->data_size);

    q->head = NextPos(q->head, 1, q->capacity);

    return CL_ResSuccess;
}

CL_Result_t CL_QueueMultiPoll(CL_Queue_t *q, void *data, uint16_t len)
{
    if (CL_QueueLength(q) < len)
        return CL_ResFailed;

    if (q->tail >= q->head)
    {
        DATA_CPY(data, (char *)q->buff + (q->head * q->data_size), q->data_size * len);
        q->head = NextPos(q->head, len, q->capacity);
    }
    else
    {
        uint16_t ltoEnd = q->capacity + 1 - q->head;
        if (ltoEnd >= len)
        {
            DATA_CPY(data, (char *)q->buff + (q->head * q->data_size), q->data_size * len);
            q->head = NextPos(q->head, len, q->capacity);
        }
        else
        {
            DATA_CPY(data, (char *)q->buff + (q->head * q->data_size), q->data_size * ltoEnd);
            uint16_t lRem = len - ltoEnd;
            DATA_CPY((char *)data + (q->data_size * ltoEnd), (char *)q->buff, q->data_size * lRem);
            q->head = lRem;
        }
    }

    return CL_ResSuccess;
}

CL_Result_t CL_QueuePeek(CL_Queue_t *q, uint16_t index, void **pptr)
{
    if (CL_QueueEmpty(q) || index >= CL_QueueLength(q))
        return CL_ResFailed;

    index += q->head;
    index = index % (q->capacity + 1);

    *pptr = (char *)q->buff + (index * q->data_size);

    return CL_ResSuccess;
}

bool CL_QueueEmpty(CL_Queue_t *q)
{
    if (q->head == q->tail)
        return true;

    return false;
}

bool CL_QueueFull(CL_Queue_t *q)
{
    uint16_t nextTail = NextPos(q->tail, 1, q->capacity);
    if (nextTail == q->head)
        return true;

    return false;
}

uint16_t CL_QueueLength(CL_Queue_t *q)
{
    uint16_t head, tail;
    head = q->head;
    tail = q->tail;

    if (tail >= head)
    {
        return tail - head;
    }
    else
    {
        return q->capacity - head + 1 + tail;
    }
}

uint16_t CL_QueueFreeSpace(CL_Queue_t *q)
{
    return q->capacity - CL_QueueLength(q);
}
