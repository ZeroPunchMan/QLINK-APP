#pragma once

#include "cl_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uint16_t cellSize;
    uint16_t capacity;
    void* memBuff;
    uint16_t* idxBuff;
    uint16_t freeCount;
} CL_Pool_t;

#define CL_POOL_DEFINE(name, capacity, cell_type, modifier) \
static cell_type name##_poolBuf[capacity]; \
static uint16_t name##_idxBuff[capacity]; \
modifier CL_Pool_t name = {(uint16_t)sizeof(cell_type), capacity, (void*)(name##_poolBuf), name##_idxBuff, capacity};

#define CL_POOL_DECL(name) \
extern CL_Pool_t name;


#define CL_POOL_ALLOC(pool_ptr, cell_type)  ((cell_type*)CL_PoolAlloc(pool_ptr, sizeof(cell_type)))
#define CL_POOL_FREE(pool_ptr, cell_ptr)    (CL_PoolFree(pool_ptr, (void*)cell_ptr))

void CL_PoolInit(CL_Pool_t* pool);
void *CL_PoolAlloc(CL_Pool_t *pool, uint16_t cellSize);
CL_Result_t CL_PoolFree(CL_Pool_t* pool, void* pCell);


#define USE_POOL_CHECK

#ifdef USE_POOL_CHECK
CL_Result_t CL_PoolFullCheck(CL_Pool_t *pool);
#endif

#ifdef __cplusplus
}
#endif
