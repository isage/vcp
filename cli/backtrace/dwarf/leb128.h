// 
// File: uleb128.h
// Author: GrandChris
// Date: 2021-03-18
// Brief: Decodes LEB128 numbers
//

#pragma once

#include <cstdint>
#include <cstddef>
#include <span>

/// 
/// \brief   Decodes an unsigned Little Endian Base 128 (LEB128) encoded number
/// \author  GrandChris
/// \date    2021-03-18
/// \param data Byte array
/// \param result The decoded number
/// \return  The number of bytes read from the data
/// \details https://en.wikipedia.org/wiki/LEB128
///
inline size_t decodeUleb128(std::span<uint8_t const> const data, uint64_t & result) 
{
    size_t n = 0;
    size_t shift = 0;

    if(data.empty()) {
        result = 0;
        return 0;
    }

    while(true) {
        uint8_t const byte = data[n++];
        result |= (byte & 0x7F) << shift;
        shift += 7;

        if((byte & 0x80) == 0) 
        {   // success
            return n;
        }
        else if(n >= data.size()) 
        {   // decoding failed
            result = 0;
            return 0;
        }
    }    
}

/// 
/// \brief   Decodes a signed Little Endian Base 128 (LEB128) encoded number
/// \author  GrandChris
/// \date    2021-03-18
/// \param data Byte array
/// \param result The decoded number
/// \return  The number of bytes read from the data
/// \details https://en.wikipedia.org/wiki/LEB128
///
inline size_t decodeSleb128(std::span<uint8_t const> const data, uint64_t & result) 
{
    size_t n = 0;
    size_t shift = 0;

    if(data.empty()) {
        result = 0;
        return 0;
    }

    while(true) {
        uint8_t const byte = data[n++];
        result |= (byte & 0x7F) << shift;
        shift += 7;

        if((byte & 0x80) == 0) 
        {   // success
            if(shift < 32 && (byte & 0x40) != 0) {
                result |= (~0 << shift);
            }

            return n;
        }
        else if(n >= data.size()) 
        {   // decoding failed
            result = 0;
            return 0;
        }
    }    
}
