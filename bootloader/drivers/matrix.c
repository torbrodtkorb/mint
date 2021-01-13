// Driver for the MATRIX contoller

#include<drivers/matrix.h>

static const u8 pid_matrix_map[] = 
    { 2, 6, 7, 9, 10, 12, 13, 15, 31, 32, 45, 46, 52, 53, 63 };

// Returns the MATRIX connected to the given peripheral
enum matrix get_matrix(u32 pid) {
    for (u32 i = 0; i < sizeof(pid_matrix_map); i ++) {
        if (pid == pid_matrix_map[i]) {
            return MATRIX64;
        }
    }
    return MATRIX32;
}
