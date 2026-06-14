//
// Created by gooddamn on 6/13/26.
//

#ifndef CARDBOARD_SDK_DISTORTION_EYES_H
#define CARDBOARD_SDK_DISTORTION_EYES_H

#include "include/cardboard.h"
#include "distortion_mesh_matrix.h"
#include "device_params/android/device_params.h"
#include "viewport_params.h"

namespace cardboard {
    class CBDistortionMeshEyeLeft
        : public CBDistortionMesh {

        void calculateFov(
            std::array<float, 4>
        ) override;

        void calculateHeadMatrix(
            float interLensDistance
        ) override;

        void calculateScreenParams(
            DeviceParams* deviceParams,
            float screenWidthMeters,
            float screenHeightMeters,
            float yOffsetMeters,
            ViewportParams* out
        ) override;

        void calculateTextureParams(
            ViewportParams* out
        ) override;
    };

    class CBDistortionMeshEyeRight
        : public CBDistortionMesh {
        void calculateFov(
            std::array<float, 4>
        ) override;

        void calculateHeadMatrix(
            float interLensDistance
        ) override;

        void calculateScreenParams(
            DeviceParams* deviceParams,
            float screenWidthMeters,
            float screenHeightMeters,
            float yOffsetMeters,
            ViewportParams* out
        ) override;

        void calculateTextureParams(
            ViewportParams* out
        ) override;
    };

}

#endif //CARDBOARD_SDK_DISTORTION_EYES_H