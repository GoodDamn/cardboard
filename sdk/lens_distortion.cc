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
#include "include/viewport_params.h"


#include "include/cardboard.h"

namespace cardboard {

    constexpr float kDefaultBorderSizeMeters = 0.003f;

    LensDistortion::LensDistortion(
        float screenWidthMeters,
        float screenHeightMeters,
        std::vector<CBMesh*>* meshes,
        CBParamsDevice* deviceParams
    ) : screen_width_meters_(
        screenWidthMeters
    ), screen_height_meters_(
        screenHeightMeters
    ) {
        device_params_ = deviceParams;
        for (const auto & meshe : *meshes) {
            meshe->meshDistortion->calculateHeadMatrix(
                device_params_->inter_lens_distance()
            );
        }

        std::vector<float> distortion_coefficients(
            device_params_->distortion_coefficients_size(),
            0.0f
        );

        for (int i = 0; i < device_params_->distortion_coefficients_size(); i++) {
            distortion_coefficients.at(i) = device_params_->distortion_coefficients(i);
        }

        distortion_ = std::make_unique<PolynomialRadialDistortion>(
            distortion_coefficients
        );

        std::array<float, 4> calculatedFov = CalculateFov();

        int i = 0;
        float yOffsetMeters = GetYEyeOffsetMeters();
        for (const auto & meshe : *meshes) {
            meshe->meshDistortion->calculateFov(
                calculatedFov
            );

            ViewportParams paramsScreen, paramsTexture;

            meshe->meshDistortion->calculateScreenParams(
                device_params_,
                screenWidthMeters,
                screenHeightMeters,
                yOffsetMeters,
                &paramsScreen
            );

            meshe->meshDistortion->calculateTextureParams(
                &paramsTexture
            );

            meshe->meshDistortion->mesh = std::make_unique<DistortionMesh>(
                *distortion_,
                paramsScreen.width,
                paramsScreen.height,
                paramsScreen.x_eye_offset,
                paramsScreen.y_eye_offset,
                paramsTexture.width,
                paramsTexture.height,
                paramsTexture.x_eye_offset,
                paramsTexture.y_eye_offset
            );
        }
    }

    std::array<float, 2> LensDistortion::DistortedUvForUndistortedUv(
        std::unique_ptr<CBDistortionMesh>& mesh,
        std::array<float, 2> &in
    ) {
        if (screen_width_meters_ == 0 || screen_height_meters_ == 0) {
            return {0, 0};
        }

        ViewportParams paramsScreen, paramsTexture;

        mesh->calculateScreenParams(
            device_params_,
            screen_width_meters_,
            screen_height_meters_,
            GetYEyeOffsetMeters(),
            &paramsScreen
        );

        mesh->calculateTextureParams(
            &paramsTexture
        );

        // Convert input from normalized [0, 1] screen coordinates to eye-centered
        // tanangle units.
        std::array<float, 2> undistorted_uv_tanangle = {
            in[0] * paramsScreen.width - paramsScreen.x_eye_offset,
            in[1] * paramsScreen.height - paramsScreen.y_eye_offset
        };

        std::array<float, 2> distorted_uv_tanangle = distortion_->Distort(
            undistorted_uv_tanangle
        );

        // Convert output from tanangle units to normalized [0, 1] pre distort texture
        // space.
        return {
            (distorted_uv_tanangle[0] + paramsTexture.x_eye_offset) / paramsTexture.width,
            (distorted_uv_tanangle[1] + paramsTexture.y_eye_offset) / paramsTexture.height
        };
    }

    std::array<float, 2> LensDistortion::UndistortedUvForDistortedUv(
        std::unique_ptr<CBDistortionMesh>& mesh,
        std::array<float, 2> &in
    ) {
        if (screen_width_meters_ == 0 || screen_height_meters_ == 0) {
            return {0, 0};
        }

        ViewportParams paramsScreen, paramsTexture;
        mesh->calculateScreenParams(
            device_params_,
            screen_width_meters_,
            screen_height_meters_,
            GetYEyeOffsetMeters(),
            &paramsScreen
        );

        mesh->calculateTextureParams(
            &paramsTexture
        );

        // Convert input from normalized [0, 1] pre distort texture space to
        // eye-centered tanangle units.
        std::array<float, 2> distorted_uv_tanangle = {
            in[0] * paramsTexture.width - paramsTexture.x_eye_offset,
            in[1] * paramsTexture.height - paramsTexture.y_eye_offset
        };

        std::array<float, 2> undistorted_uv_tanangle = distortion_->DistortInverse(
            distorted_uv_tanangle
        );

        // Convert output from tanangle units to normalized [0, 1] screen coordinates.
        return {
            (undistorted_uv_tanangle[0] + paramsScreen.x_eye_offset) / paramsScreen.width,
            (undistorted_uv_tanangle[1] + paramsScreen.y_eye_offset) / paramsScreen.height
        };
    }

    std::array<float, 4> LensDistortion::CalculateFov() {
        // FOV angles in device parameters are in degrees so they are converted
        // to radians for posterior use.
        std::array<float, 4> device_fov = {
                DegreesToRadians(device_params_->left_eye_field_of_view_angles(0)),
                DegreesToRadians(device_params_->left_eye_field_of_view_angles(1)),
                DegreesToRadians(device_params_->left_eye_field_of_view_angles(2)),
                DegreesToRadians(device_params_->left_eye_field_of_view_angles(3)),
        };

        const float eye_to_screen_distance = device_params_->screen_to_lens_distance();
        const float outer_distance =
                (screen_width_meters_ - device_params_->inter_lens_distance()) / 2.0f;
        const float inner_distance = device_params_->inter_lens_distance() / 2.0f;
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
        switch (device_params_->vertical_alignment()) {
            case CBEnumVerticalAlignment::CENTER:
            default:
                return screen_height_meters_ / 2.0f;
            case CBEnumVerticalAlignment::BOTTOM:
                return device_params_->tray_to_lens_distance() - kDefaultBorderSizeMeters;
            case CBEnumVerticalAlignment::TOP:
                return screen_height_meters_ - device_params_->tray_to_lens_distance() -
                       kDefaultBorderSizeMeters;
        }
    }

    constexpr float LensDistortion::DegreesToRadians(float angle) {
        return angle * M_PI / 180.0f;
    }
}  // namespace cardboard
