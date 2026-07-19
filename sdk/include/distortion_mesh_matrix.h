//
// Created by gooddamn on 6/12/26.
//

#ifndef CARDBOARD_SDK_INCLUDE_DISTORTION_MESH_MATRIX_H
#define CARDBOARD_SDK_INCLUDE_DISTORTION_MESH_MATRIX_H

#include <array>
#include "matrix_4x4.h"
#include "distortion_mesh.h"
#include "viewport_params.h"
#include "device_params.h"

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

    class CBDistortionMesh {
    public:
        CBDistortionMeshMatrix matrix;
        std::unique_ptr<DistortionMesh> mesh;

        virtual ~CBDistortionMesh() = default;

        virtual void calculateFov(
            std::array<float, 4>
        ) = 0;

        virtual void calculateHeadMatrix(
            float interLensDistance
        ) = 0;

        virtual void calculateScreenParams(
            DeviceParams* deviceParams,
            float screenWidthMeters,
            float screenHeightMeters,
            float yOffsetMeters,
            ViewportParams* out
        ) = 0;

        virtual void calculateTextureParams(
            ViewportParams* out
        ) = 0;
    };

}

#endif //CARDBOARD_SDK_INCLUDE_DISTORTION_MESH_MATRIX_H