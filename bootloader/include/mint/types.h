#ifndef TYPES_H
#define TYPES_H


#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;

// Define for register definition
#define _rw volatile 
#define __r volatile const
#define __w volatile

#endif
