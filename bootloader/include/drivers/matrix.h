// Driver for the MATRIX controller

#ifndef MATRIX_H
#define MATRIX_H

#include <mint/types.h>

enum matrix {
    MATRIX32,
    MATRIX64
};

enum matrix get_matrix(u32 pid);

#endif
