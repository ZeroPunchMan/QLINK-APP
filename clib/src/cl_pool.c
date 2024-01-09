#include "cl_pool.h"
#include "string.h"

void CL_PoolInit(CL_Pool_t *pool)
{
    for (uint16_t i = 0; i < pool->capacity; i++)
    {
        pool->idxBuff[i] = i;
    }
}

void *CL_PoolAlloc(CL_Pool_t *pool, uint16_t cellSize)
{
    void *result = CL_NULL;

    if(cellSize != pool->cellSize)
        return result;
        
    if (pool->freeCount > 0)
    {
        uint16_t cellIdx;
        pool->freeCount--;
        cellIdx = pool->idxBuff[pool->freeCount];

        result = (void *)((char*)pool->memBuff + cellIdx * pool->cellSize);
    }

    return result;
}

CL_Result_t CL_PoolFree(CL_Pool_t *pool, void *pCell)
{
    if(pCell < pool->memBuff || pool->freeCount >= pool->capacity)
        return CL_ResInvalidParam;

    uint32_t addrOffset = (char *)pCell - (char *)pool->memBuff;
    if((addrOffset % pool->cellSize) != 0)
        return CL_ResInvalidParam;

    uint16_t cellIdx = addrOffset / pool->cellSize;
    pool->idxBuff[pool->freeCount++] = cellIdx;
    return CL_ResSuccess;
}


#ifdef USE_POOL_CHECK
#include "stdlib.h"

CL_Result_t CL_PoolFullCheck(CL_Pool_t *pool)
{
    if(pool->capacity != pool->freeCount)
        return CL_ResFailed;

    uint8_t* flags = malloc(pool->capacity);
    memset(flags, 0, pool->capacity);
    for(int i = 0; i < pool->capacity; i++)
    {
        uint16_t idx = pool->idxBuff[i];
        flags[idx]++;
    }

    for(int i = 0; i < pool->capacity; i++)
    {
        if(flags[i] != 1)
        {
            return CL_ResFailed;
        }
        // printf("%d: %d\n", i, flags[i]);
    }
    return CL_ResSuccess;
}
#endif
