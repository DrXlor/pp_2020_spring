// Copyright 2020 Kurakin Mikhail
#include <random>
#include "../../../modules/task_1/kurakin_m_multiply_matrix_fox/multiply_matrix_fox.h"
bool canMultiplicate(const matrix &a, const matrix &b) {
    if (a.empty() || b.empty()) return false;
    for (uint i = 0; i < a.size(); i++) {
        if (a[i].size() != b.size()) return false;
    }
    return true;
}

void prepareOutMatrix(matrix *matrixOut, uint sizeRow, uint sizeCol) {
    if ((*matrixOut).size() != sizeRow) (*matrixOut).resize(sizeRow);
    for (uint i = 0; i < (*matrixOut).size(); i++) {
        if ((*matrixOut)[i].size() != sizeCol) (*matrixOut)[i].resize(sizeCol);
        for (uint j = 0; j < (*matrixOut)[i].size(); j++) {
            (*matrixOut)[i][j] = 0;
        }
    }
}

bool simpleMaxtrixMultiply(const matrix &a, const matrix &b, matrix *out) {
    if (!canMultiplicate(a, b)) return false;
    prepareOutMatrix(out, a.size(), b.size());
    for (uint i = 0; i < a.size(); i++) {  // ixj j*k
        for (uint k = 0; k < b[0].size(); k++) {
            for (uint j = 0; j < b.size(); j++) {
                (*out)[i][k] += a[i][j] * b[j][k];
            }
        }
    }
    return true;
}

bool initMatrixRand(matrix *a, uint sizeRow, uint sizeCol) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1000.0, 1000.0);

    if ((*a).size() != sizeRow) (*a).resize(sizeRow);

    for (uint i = 0; i < sizeRow; i++) {
        if ((*a)[i].size() != sizeCol) (*a)[i].resize(sizeCol);

        for (uint j = 0; j < sizeCol; j++) {
            (*a)[i][j] = dis(gen);
        }
    }
    return true;
}
