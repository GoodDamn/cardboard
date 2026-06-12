/*
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "lens_distortion.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <memory>
#include <vector>

#include "include/cardboard.h"

namespace cardboard {

    constexpr float kDefaultBorderSizeMeters = 0.003f;

    // All values in tanangle units.
    struct LensDistortion::ViewportParams {
        float width;
        float height;
        float x_eye_offset;
        float y_eye_offset;
    };

    LensDistortion::LensDistortion(
            float screenWidthMeters,
            float screenHeightMeters,
            std::vector<CBDistortionMesh>& meshes
    ) : screen_width_meters_(
            screenWidthMeters
    ), screen_height_meters_(
            screenHeightMeters
    ), meshes_(meshes) {

        meshes_[kLeft].matrix.eye_from_head_matrix_ = cardboard::Matrix4x4::Translation(
                device_params_.inter_lens_distance() * 0.5f, 0.f, 0.f);

        meshes_[kRight].matrix.eye_from_head_matrix_ = cardboard::Matrix4x4::Translation(
                -device_params_.inter_lens_distance() * 0.5f, 0.f, 0.f);

        std::vector<float> distortion_coefficients(
                device_params_.distortion_coefficients_size(),
                0.0f
        );

        for (int i = 0; i < device_params_.distortion_coefficients_size(); i++) {
            distortion_coefficients.at(i) = device_params_.distortion_coefficients(i);
        }

        distortion_ = std::unique_ptr<PolynomialRadialDistortion>(
                new PolynomialRadialDistortion(distortion_coefficients)
        );

        UpdateParams();
    }

    void LensDistortion::UpdateParams() {

        meshes_[kLeft].matrix.fov = CalculateFov();
        // Mirror fov for right eye.
        meshes_[kRight].matrix.fov =    meshes_[kLeft].matrix.fov;
        meshes_[kRight].matrix.fov[0] = meshes_[kLeft].matrix.fov[1];
        meshes_[kRight].matrix.fov[1] = meshes_[kLeft].matrix.fov[0];

        meshes_[kLeft].mesh = std::unique_ptr<DistortionMesh>(
            CreateDistortionMesh(kLeft)
        );

        meshes_[kRight].mesh = std::unique_ptr<DistortionMesh>(
            CreateDistortionMesh(kRight)
        );
    }

    std::array<float, 2> LensDistortion::DistortedUvForUndistortedUv(
            std::array<float, 2> &in,
            CardboardEye eye
    ) {
        if (screen_width_meters_ == 0 || screen_height_meters_ == 0) {
            return {0, 0};
        }

        ViewportParams screen_params, texture_params;

        calculateScreenParams(
                eye,
                &screen_params
        );

        calculateTextureParams(
                meshes_[eye].matrix.fov,
                &texture_params
        );
        // Convert input from normalized [0, 1] screen coordinates to eye-centered
        // tanangle units.
        std::array<float, 2> undistorted_uv_tanangle = {
                in[0] * screen_params.width - screen_params.x_eye_offset,
                in[1] * screen_params.height - screen_params.y_eye_offset};

        std::array<float, 2> distorted_uv_tanangle =
                distortion_->Distort(undistorted_uv_tanangle);

        // Convert output from tanangle units to normalized [0, 1] pre distort texture
        // space.
        return {(distorted_uv_tanangle[0] + texture_params.x_eye_offset) /
                texture_params.width,
                (distorted_uv_tanangle[1] + texture_params.y_eye_offset) /
                texture_params.height};
    }

    std::array<float, 2> LensDistortion::UndistortedUvForDistortedUv(
            std::array<float, 2> &in,
            CardboardEye eye
    ) {
        if (screen_width_meters_ == 0 || screen_height_meters_ == 0) {
            return {0, 0};
        }

        ViewportParams screen_params, texture_params;

        calculateScreenParams(
                eye,
                &screen_params
        );

        calculateTextureParams(
                meshes_[eye].matrix.fov,
                &texture_params
        );
        // Convert input from normalized [0, 1] pre distort texture space to
        // eye-centered tanangle units.
        std::array<float, 2> distorted_uv_tanangle = {
                in[0] * texture_params.width - texture_params.x_eye_offset,
                in[1] * texture_params.height - texture_params.y_eye_offset};

        std::array<float, 2> undistorted_uv_tanangle =
                distortion_->DistortInverse(distorted_uv_tanangle);

        // Convert output from tanangle units to normalized [0, 1] screen coordinates.
        return {(undistorted_uv_tanangle[0] + screen_params.x_eye_offset) /
                screen_params.width,
                (undistorted_uv_tanangle[1] + screen_params.y_eye_offset) /
                screen_params.height};
    }

    std::array<float, 4> LensDistortion::CalculateFov() {
        // FOV angles in device parameters are in degrees so they are converted
        // to radians for posterior use.
        std::array<float, 4> device_fov = {
                DegreesToRadians(device_params_.left_eye_field_of_view_angles(0)),
                DegreesToRadians(device_params_.left_eye_field_of_view_angles(1)),
                DegreesToRadians(device_params_.left_eye_field_of_view_angles(2)),
                DegreesToRadians(device_params_.left_eye_field_of_view_angles(3)),
        };

        const float eye_to_screen_distance = device_params_.screen_to_lens_distance();
        const float outer_distance =
                (screen_width_meters_ - device_params_.inter_lens_distance()) / 2.0f;
        const float inner_distance = device_params_.inter_lens_distance() / 2.0f;
        const float bottom_distance = GetYEyeOffsetMeters();
        const float top_distance = screen_height_meters_ - bottom_distance;

        const float outer_angle =
                atan((*distortion_).Distort({outer_distance / eye_to_screen_distance, 0})[0]);
        const float inner_angle =
                atan((*distortion_).Distort({inner_distance / eye_to_screen_distance, 0})[0]);
        const float bottom_angle = atan(
                (*distortion_).Distort({0, bottom_distance / eye_to_screen_distance})[1]);
        const float top_angle =
                atan((*distortion_).Distort({0, top_distance / eye_to_screen_distance})[1]);

        return {
                std::min(outer_angle, device_fov[0]),
                std::min(inner_angle, device_fov[1]),
                std::min(bottom_angle, device_fov[2]),
                std::min(top_angle, device_fov[3]),
        };
    }

    float LensDistortion::GetYEyeOffsetMeters() {
        switch (device_params_.vertical_alignment()) {
            case DeviceParams::CENTER:
            default:
                return screen_height_meters_ / 2.0f;
            case DeviceParams::BOTTOM:
                return device_params_.tray_to_lens_distance() - kDefaultBorderSizeMeters;
            case DeviceParams::TOP:
                return screen_height_meters_ - device_params_.tray_to_lens_distance() -
                       kDefaultBorderSizeMeters;
        }
    }

    DistortionMesh *LensDistortion::CreateDistortionMesh(
            CardboardEye eye
    ) {
        ViewportParams screen_params, texture_params;

        calculateScreenParams(
                eye,
                &screen_params
        );

        calculateTextureParams(
                meshes_[eye].matrix.fov,
                &texture_params
        );

        return new DistortionMesh(
                *distortion_,
                screen_params.width,
                screen_params.height,
                screen_params.x_eye_offset,
                screen_params.y_eye_offset,
                texture_params.width,
                texture_params.height,
                texture_params.x_eye_offset,
                texture_params.y_eye_offset
        );
    }

    void LensDistortion::calculateScreenParams(
            CardboardEye eye,
            ViewportParams *screen_params
    ) {
        float interLensDistance = device_params_.inter_lens_distance();
        float screenLensDistance = device_params_.screen_to_lens_distance();

        screen_params->width = screen_width_meters_ / screenLensDistance;
        screen_params->height = screen_height_meters_ / screenLensDistance;

        screen_params->x_eye_offset = eye == kLeft ?
                                      ((screen_width_meters_ - interLensDistance) / 2) / screenLensDistance
                                                   : ((screen_width_meters_ + interLensDistance) / 2) / screenLensDistance;

        screen_params->y_eye_offset = GetYEyeOffsetMeters() / screenLensDistance;
    }

    void LensDistortion::calculateTextureParams(
            std::array<float, 4> &fov,
            ViewportParams *texture_params
    ) {
        texture_params->width = tan(fov[0]) + tan(fov[1]);
        texture_params->height = tan(fov[2]) + tan(fov[3]);

        texture_params->x_eye_offset = tan(fov[0]);
        texture_params->y_eye_offset = tan(fov[2]);
    }

    constexpr float LensDistortion::DegreesToRadians(float angle) {
        return angle * M_PI / 180.0f;
    }
}  // namespace cardboard
