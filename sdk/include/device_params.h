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
#ifndef CARDBOARD_SDK_INCLUDE_DEVICE_PARAMS_H_
#define CARDBOARD_SDK_INCLUDE_DEVICE_PARAMS_H_

#include <jni.h>

#include <cstdint>

namespace cardboard {

    enum CBEnumVerticalAlignment {
        BOTTOM = 0,
        CENTER = 1,
        TOP = 2
    };

    class CBParamsDevice {
    public:
        virtual float screen_to_lens_distance() = 0;
        virtual float inter_lens_distance() = 0;
        virtual float tray_to_lens_distance() = 0;
        virtual CBEnumVerticalAlignment vertical_alignment() = 0;
        virtual float distortion_coefficients(int index) = 0;
        virtual int distortion_coefficients_size() = 0;
        virtual float left_eye_field_of_view_angles(int index) = 0;
        virtual ~CBParamsDevice() = default;
    };

}  // namespace cardboard

#endif  // CARDBOARD_SDK_INCLUDE_DEVICE_PARAMS_H_
