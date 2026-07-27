//
// Created by gooddamn on 6/12/26.
//

#ifndef CARDBOARD_DISTORTION_MESH_MATRIX_H
#define CARDBOARD_DISTORTION_MESH_MATRIX_H

#endif //CARDBOARD_DISTORTION_MESH_MATRIX_H

#include <array>
#include "include/matrix_4x4.h"
#include "include/distortion_mesh_matrix.h"
#include "include/distortion_mesh.h"

namespace cardboard {

    void CBDistortionMeshMatrix::getProjectionMatrix(
            float z_near,
            float z_far,
            float *projection_matrix
    ) const {
        Matrix4x4::Perspective(
            fov,
            z_near,
            z_far
        ).ToArray(
            projection_matrix
        );
    }

    void CBDistortionMeshMatrix::getHeadMatrix(
            float *eye_from_head_matrix
    ) const {
        eye_from_head_matrix_.ToArray(
            eye_from_head_matrix
        );
    }

    void CBDistortionMeshMatrix::getFov(
            float *field_of_view
    ) const {
        std::memcpy(
            field_of_view,
            fov.data(),
            sizeof(float) * 4
        );
    }
}