#pragma once

#include "cl_common.h"

typedef enum{
    CL_BigEndian,
    CL_LittleEndian,
} CL_Endian_t;

//----------INT32--------------
int32_t CL_BytesToInt32(const uint8_t* input, CL_Endian_t endian);
void CL_Int32ToBytes(int32_t number, uint8_t* output, CL_Endian_t endian);

//------------UINT32---------------
uint32_t CL_BytesToUint32(const uint8_t *input, CL_Endian_t endian);
void CL_Uint32ToBytes(uint32_t number, uint8_t *output, CL_Endian_t endian);

//--------------INT16----------------
int16_t CL_BytesToInt16(const uint8_t *input, CL_Endian_t endian);
void CL_Int16ToBytes(int16_t number, uint8_t *output, CL_Endian_t endian);

//---------------UINT16------------------
uint16_t CL_BytesToUint16(const uint8_t *input, CL_Endian_t endian);
void CL_Uint16ToBytes(uint16_t number, uint8_t *output, CL_Endian_t endian);


