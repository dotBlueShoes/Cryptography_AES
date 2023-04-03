#pragma once
#include "Framework.hpp"

namespace Matrix {

    template<typename T>
    struct Matrix4x4 {
        T value[16];
    };

    template<typename T>
    block multiply(const Matrix4x4<T> matrixA, const Matrix4x4<T> matrixB) {
        Matrix4x4<T> result;

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

        Matrix4x4<uint8> matrixA {
            2, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };

        Matrix4x4<uint8> matrixB {
            3, 4, 2, 5,
            3, 4, 2, 5,
            3, 4, 2, 5,
            3, 4, 2, 5
        };

        Matrix4x4<uint16> matrixC {
            2, 3, 4, 7,
            4, 6, 2, 15,
            3, 6, 4, 3,
            8, 7, 8, 5
        };

        Matrix4x4<uint16> matrixD {
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

}