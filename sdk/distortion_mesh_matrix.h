//
// Created by gooddamn on 6/12/26.
//

#ifndef CARDBOARD_DISTORTION_MESH_MATRIX_H
#define CARDBOARD_DISTORTION_MESH_MATRIX_H

#endif //CARDBOARD_DISTORTION_MESH_MATRIX_H

#include <array>
#include "include/cardboard.h"
#include "util/matrix_4x4.h"
#include "distortion_mesh.h"

namespace cardboard {
    struct CBDistortionMeshMatrix {
        std::array<float, 4> fov; // L, R, B, T
        Matrix4x4 eye_from_head_matrix_;

        void getHeadMatrix(
                float *eye_from_head_matrix
        ) const;

        void getProjectionMatrix(
                float z_near,
                float z_far,
                float *projection_matrix
        ) const;

        void getFov(
                float *field_of_view
        ) const;
    };

    struct CBDistortionMesh {
        CBDistortionMeshMatrix matrix;
        std::unique_ptr<DistortionMesh> mesh;
        CardboardMesh getDistortionMesh() const;
    };

}