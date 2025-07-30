#ifndef MATRIX_PALETTE_H
#define MATRIX_PALETTE_H

#include "math.h"

const size_t MAX_SKELETON_BONES = 96;

struct MatrixPalette {
    Matrix4 entry[MAX_SKELETON_BONES];
};

#endif