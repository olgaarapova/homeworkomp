
/* СЛОЖЕНИЕ ДВУХ МАТРИЦ */

#include <iostream>
#include <omp.h>
#include "math.h"
#include "time.h"

class Matrix {
    friend void fillMatrixRandom(Matrix &);
    friend void showMatrix(Matrix &);
public:
    int height;
    int width;
    float **content;
    Matrix(int h, int w) {
        height = h;
        width = w;
        content = new float*[height];
    }
    ~Matrix() {
        for (int i = 0; i < height; i++) {
            delete [] content[i];
        }
        delete [] content;
    }
};

void fillMatrixRandom(Matrix &matrix) {

    for (int i = 0; i < matrix.height; i++) {
        matrix.content[i] = new float[matrix.width];
        for (int j = 0; j < matrix.width; j++) {
            matrix.content[i][j] = rand() % 10 + 1;
        }
    }

}

void showMatrix(Matrix &matrix) {
    for (int i = 0; i < matrix.height; i++) {
        for (int j = 0; j < matrix.height; j++) {
            std::cout << matrix.content[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}

void dotprod(Matrix &A, Matrix &B, Matrix &C, int interval) {
    int beginHeight, endHeight;

    // это просто магия
    beginHeight = omp_get_thread_num() * interval;
    endHeight = (beginHeight + interval - 1 < A.height) ?
            beginHeight + interval : beginHeight + interval - 1;
    endHeight = (A.height % omp_get_num_threads()) ?
            endHeight + A.height % omp_get_num_threads() : endHeight;

    for (int i = beginHeight; i < endHeight; i++) {
        for (int j = 0; j < A.width; j++) {
            C.content[i][j] = A.content[i][j] + B.content[i][j];
        }
    }

}

int main() {
    int h, w;
    std::cout << "Input heigth: ";
    std::cin >> h;

    std::cout << "Input width: ";
    std::cin >> w;
    double beginTime, endTime, totalTime;

    Matrix *A = new Matrix(h, w);
    Matrix *B = new Matrix(h, w);
    fillMatrixRandom(*A);
    fillMatrixRandom(*B);

    Matrix *C = new Matrix(A->height, A->width);
    fillMatrixRandom(*C);

    std::cout << "/* ADDITION FOR MATRIX */" << std::endl;

    beginTime = omp_get_wtime();

#pragma omp parallel
    dotprod(*A, *B, *C, A->height / omp_get_num_threads());

    endTime = omp_get_wtime();
    totalTime = endTime - beginTime;
    std::cout << std::endl << "TOTAL MULTYTHREAD TIME " << std::endl << totalTime;

    showMatrix(*A);
    std::cout << "+" << std::endl;
    showMatrix(*B);
    std::cout << "=" << std::endl;
    showMatrix(*C);


    beginTime = omp_get_wtime();
    for (int i = 0; i < A->height; i++) {
        for (int j = 0; j < A->width; j++) {
            C->content[i][j] = A->content[i][j] + B->content[i][j];
        }
    }
    endTime = omp_get_wtime();
    totalTime = endTime - beginTime;
    std::cout << std::endl << "TOTAL ONE THREAD TIME " << std::endl << totalTime;

    return 0;

}

