#include "cl_serialize.h"

#define FROM_32BITS(number, output, endian) \
    if (endian == CL_BigEndian)             \
    {                                       \
        output[0] = number >> 24;           \
        output[1] = (number >> 16) & 0xff;  \
        output[2] = (number >> 8) & 0xff;   \
        output[3] = number & 0xff;          \
    }                                       \
    else if (endian == CL_LittleEndian)     \
    {                                       \
        output[3] = number >> 24;           \
        output[2] = (number >> 16) & 0xff;  \
        output[1] = (number >> 8) & 0xff;   \
        output[0] = number & 0xff;          \
    }

#define TO_32BITS(type, input, endian)  \
    type result = 0;                        \
    if (endian == CL_BigEndian)         \
    {                                   \
        result = (type)input[0] << 24;  \
        result |= (type)input[1] << 16; \
        result |= (type)input[2] << 8;  \
        result |= (type)input[3];       \
    }                                   \
    else if (endian == CL_LittleEndian) \
    {                                   \
        result = (type)input[3] << 24;  \
        result |= (type)input[2] << 16; \
        result |= (type)input[1] << 8;  \
        result |= (type)input[0];       \
    }                                   \
    return result;

int32_t CL_BytesToInt32(const uint8_t *input, CL_Endian_t endian)
{
    TO_32BITS(int32_t, input, endian);
}

uint32_t CL_BytesToUint32(const uint8_t *input, CL_Endian_t endian)
{
    TO_32BITS(uint32_t, input, endian);
}

void CL_Int32ToBytes(int32_t number, uint8_t *output, CL_Endian_t endian)
{
    FROM_32BITS(number, output, endian);
}

void CL_Uint32ToBytes(uint32_t number, uint8_t *output, CL_Endian_t endian)
{
    FROM_32BITS(number, output, endian);
}

#define FROM_16BITS(number, output, endian) \
    if (endian == CL_BigEndian)             \
    {                                       \
        output[0] = number >> 8;            \
        output[1] = number & 0xff;          \
    }                                       \
    else if (endian == CL_LittleEndian)     \
    {                                       \
        output[1] = number >> 8;            \
        output[0] = number & 0xff;          \
    }

#define TO_16BITS(type, input, endian)  \
    type result = 0;                        \
    if (endian == CL_BigEndian)         \
    {                                   \
        result = (type)input[0] << 8;   \
        result |= (type)input[1];       \
    }                                   \
    else if (endian == CL_LittleEndian) \
    {                                   \
        result |= (type)input[1] << 8;  \
        result |= (type)input[0];       \
    }                                   \
    return result;

int16_t CL_BytesToInt16(const uint8_t *input, CL_Endian_t endian)
{
    TO_16BITS(int16_t, input, endian);
}

uint16_t CL_BytesToUint16(const uint8_t *input, CL_Endian_t endian)
{
    TO_16BITS(uint16_t, input, endian);
}

void CL_Int16ToBytes(int16_t number, uint8_t *output, CL_Endian_t endian)
{
    FROM_16BITS(number, output, endian);
}

void CL_Uint16ToBytes(uint16_t number, uint8_t *output, CL_Endian_t endian)
{
    FROM_16BITS(number, output, endian);
}
