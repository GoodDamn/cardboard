//
// Created by gooddamn on 6/13/26.
//

#include "distortion_eyes.h"
#include "device_params/android/device_params.h"
#include "viewport_params.h"

namespace cardboard {

    ///
    /// Left eye
    ///
    void CBDistortionMeshEyeLeft::calculateFov(
        std::array<float, 4> fov
    ) {
        matrix.fov = fov;
    }

    void CBDistortionMeshEyeLeft::calculateHeadMatrix(
        float interLensDistance
    ) {
        matrix.eye_from_head_matrix_ = cardboard::Matrix4x4::Translation(
            interLensDistance * 0.5f,
            0.f,
            0.f
        );
    }

    void CBDistortionMeshEyeLeft::calculateTextureParams(
        ViewportParams* textureParams
    ) {
        textureParams->width = tan(matrix.fov[0]) + tan(matrix.fov[1]);
        textureParams->height = tan(matrix.fov[2]) + tan(matrix.fov[3]);

        textureParams->x_eye_offset = tan(matrix.fov[0]);
        textureParams->y_eye_offset = tan(matrix.fov[2]);
    }

    void CBDistortionMeshEyeLeft::calculateScreenParams(
        DeviceParams *deviceParams,
        float screenWidthMeters,
        float screenHeightMeters,
        float yOffsetMeters,
        ViewportParams* screenParams
    ) {
        float interLensDistance = deviceParams->inter_lens_distance();
        float screenLensDistance = deviceParams->screen_to_lens_distance();
        screenParams->width = screenWidthMeters / screenLensDistance;
        screenParams->height = screenHeightMeters / screenLensDistance;
        screenParams->x_eye_offset = (
            (screenWidthMeters - interLensDistance) / 2
        ) / screenLensDistance;
        screenParams->y_eye_offset = yOffsetMeters / screenLensDistance;
    }

    ///
    /// Right eye
    ///
    void CBDistortionMeshEyeRight::calculateFov(
        std::array<float, 4> fov
    ) {
        // Mirror fov for right eye.
        matrix.fov = fov;
        matrix.fov[0] = fov[1];
        matrix.fov[1] = fov[0];
    }

    void CBDistortionMeshEyeRight::calculateHeadMatrix(
        float interLensDistance
    ) {
        matrix.eye_from_head_matrix_ = cardboard::Matrix4x4::Translation(
            -interLensDistance * 0.5f,
            0.f,
            0.f
        );
    }

    void CBDistortionMeshEyeRight::calculateTextureParams(
        ViewportParams* textureParams
    ) {
        textureParams->width = tan(matrix.fov[0]) + tan(matrix.fov[1]);
        textureParams->height = tan(matrix.fov[2]) + tan(matrix.fov[3]);

        textureParams->x_eye_offset = tan(matrix.fov[0]);
        textureParams->y_eye_offset = tan(matrix.fov[2]);
    }

    void CBDistortionMeshEyeRight::calculateScreenParams(
        DeviceParams *deviceParams,
        float screenWidthMeters,
        float screenHeightMeters,
        float yOffsetMeters,
        ViewportParams* screenParams
    ) {
        float interLensDistance = deviceParams->inter_lens_distance();
        float screenLensDistance = deviceParams->screen_to_lens_distance();
        screenParams->width = screenWidthMeters / screenLensDistance;
        screenParams->height = screenHeightMeters / screenLensDistance;
        screenParams->x_eye_offset = (
            (screenWidthMeters + interLensDistance) / 2
        ) / screenLensDistance;
        screenParams->y_eye_offset = yOffsetMeters / screenLensDistance;
    }
}