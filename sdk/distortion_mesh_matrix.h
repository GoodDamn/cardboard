//
// Created by gooddamn on 6/12/26.
//

#ifndef CARDBOARD_DISTORTION_MESH_MATRIX_H
#define CARDBOARD_DISTORTION_MESH_MATRIX_H

#endif //CARDBOARD_DISTORTION_MESH_MATRIX_H

#include <array>
#include "include/cardboard.h"
#include "util/matrix_4x4.h"

namespace cardboard {
    struct CBDistortionMeshMatrix {
        std::array<float, 4> fov; // L, R, B, T
        Matrix4x4 eye_from_head_matrix_;
    };

}