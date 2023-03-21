// Type your code here, or load an example.
#include <inttypes.h>
#include <stdio.h>

using uint16 = uint16_t;
using uint8 = uint8_t;

#define block inline auto

template<typename T>
struct matrix4x4 {
    T value[16];
};

template<typename T>
block multiply(const matrix4x4<T> matrixA, const matrix4x4<T> matrixB) {
    matrix4x4<T> result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.value[(i * 4) + j] = 0;
  
            for (int k = 0; k < 4; k++) {
                result.value[(i * 4) + j] += 
                    matrixA.value[(i * 4) + k] * 
                    matrixB.value[(k * 4) + j];
            }
        }
    }

    return result;
}

int main() {

    matrix4x4<uint8> matrixA { 
        2, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    matrix4x4<uint8> matrixB { 
        3, 4, 2, 5,
        3, 4, 2, 5,
        3, 4, 2, 5,
        3, 4, 2, 5
    };

    matrix4x4<uint16> matrixC { 
        2, 3, 4, 7,
        4, 6, 2, 15,
        3, 6, 4, 3,
        8, 7, 8, 5
    };

    matrix4x4<uint16> matrixD { 
        3, 4, 2, 5,
        3, 4, 2, 5,
        3, 4, 2, 5,
        3, 4, 2, 5
    };

    matrixC = multiply<uint16>(matrixC, matrixD);
    matrixB = multiply<uint8>(matrixA, matrixB);

    return matrixC.value[7];

    //char* message = "sample hello world";
    //printf(message);

    //return 0;
}
