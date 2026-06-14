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
#ifndef CARDBOARD_SDK_LENSDISTORTION_H_
#define CARDBOARD_SDK_LENSDISTORTION_H_

#include <array>
#include <memory>

#ifdef __ANDROID__
#include "device_params/android/device_params.h"
#else
#include "cardboard_device.pb.h"
#endif

#include "distortion_mesh_matrix.h"
#include "include/cardboard.h"
#include "polynomial_radial_distortion.h"
#include "util/matrix_4x4.h"

namespace cardboard {

class LensDistortion {
 public:
  LensDistortion(
    float screenWidthMeters,
    float screenHeightMeters,
    std::vector<std::unique_ptr<CBDistortionMesh>>& meshes
  );

  // Tan angle units. "DistortedUvForUndistoredUv" goes through the forward
  // distort function. I.e. the lens. UndistortedUvForDistortedUv uses the
  // inverse distort function.
  std::array<float, 2> DistortedUvForUndistortedUv(
      std::unique_ptr<CBDistortionMesh>& mesh,
      std::array<float, 2>& in
  );

  std::array<float, 2> UndistortedUvForDistortedUv(
      std::unique_ptr<CBDistortionMesh>& mesh,
      std::array<float, 2>& in
  );
 private:

  float GetYEyeOffsetMeters();

  std::array<float, 4> CalculateFov();

  static constexpr float DegreesToRadians(float angle);

  DeviceParams device_params_;

  float screen_width_meters_;
  float screen_height_meters_;

  std::unique_ptr<PolynomialRadialDistortion> distortion_;
};

}  // namespace cardboard

#endif  // CARDBOARD_SDK_LENSDISTORTION_H_
