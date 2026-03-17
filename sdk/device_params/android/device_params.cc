/*
 * Copyright 2020 Google LLC
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
#include "device_params/android/device_params.h"

#include <jni.h>

#include <cstdint>

#include "jni_utils/android/jni_utils.h"
#include "qrcode/cardboard_v1/cardboard_v1.h"
#include "util/logging.h"

namespace cardboard {

// TODO(b/181658993): Check if JNI "execution + exception check" could be
// wrapped within a reusable function or macro.
    float DeviceParams::screen_to_lens_distance() const {
      return qrcode::kCardboardV1ScreenToLensDistance;
    }

    float DeviceParams::inter_lens_distance() const {
      return qrcode::kCardboardV1InterLensDistance;
    }

    float DeviceParams::tray_to_lens_distance() const {
      return qrcode::kCardboardV1TrayToLensDistance;
    }

    int DeviceParams::vertical_alignment() const {
      return qrcode::kCardboardV1VerticalAlignmentType;
    }

    float DeviceParams::distortion_coefficients(int index) const {
      return qrcode::kCardboardV1DistortionCoeffs[index];
    }

    int DeviceParams::distortion_coefficients_size() const {
      return qrcode::kCardboardV1DistortionCoeffsSize;
    }

    float DeviceParams::left_eye_field_of_view_angles(int index) const {
      return qrcode::kCardboardV1FovHalfDegrees[index];
    }

}  // namespace cardboard
