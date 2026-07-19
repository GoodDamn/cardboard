//
// Created by gooddamn on 6/13/26.
//

#ifndef CARDBOARD_DISTORTION_EYES_H
#define CARDBOARD_DISTORTION_EYES_H

#include "distortion_mesh_matrix.h"
#include "distortion_mesh.h"
#include "device_params.h"
#include "viewport_params.h"

class CBDistortionMeshEyeLeft
: public cardboard::CBDistortionMesh {

    void calculateFov(
        std::array<float, 4>
    ) override;

    void calculateHeadMatrix(
        float interLensDistance
    ) override;

    void calculateScreenParams(
        cardboard::DeviceParams* deviceParams,
        float screenWidthMeters,
        float screenHeightMeters,
        float yOffsetMeters,
        cardboard::ViewportParams* out
    ) override;

    void calculateTextureParams(
        cardboard::ViewportParams* out
    ) override;
};

class CBDistortionMeshEyeRight
: public cardboard::CBDistortionMesh {
    void calculateFov(
        std::array<float, 4>
    ) override;

    void calculateHeadMatrix(
        float interLensDistance
    ) override;

    void calculateScreenParams(
        cardboard::DeviceParams* deviceParams,
        float screenWidthMeters,
        float screenHeightMeters,
        float yOffsetMeters,
        cardboard::ViewportParams* out
    ) override;

    void calculateTextureParams(
        cardboard::ViewportParams* out
    ) override;
};

#endif //CARDBOARD_DISTORTION_EYES_H