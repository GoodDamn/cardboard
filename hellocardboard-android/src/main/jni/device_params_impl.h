//
// Created by gooddamn on 7/24/26.
//

#ifndef CARDBOARD_DEVICE_PARAMS_IMPL_H
#define CARDBOARD_DEVICE_PARAMS_IMPL_H

#include <jni.h>
#include <array>
#include <cstdint>

#include "device_params.h"

class APParamsDeviceMutable
: public cardboard::CBParamsDevice {
private:

    float mDistanceInterLens;
    float mDistanceTrayToLens;
    float mDistanceScreenToLens;
    std::array<float, 4> mFovHalfDegrees;
    std::array<float, 2> mDistortionCoeffs;

public:

    void setDistanceScreenToLens(
        float d
    ) { mDistanceScreenToLens = d; }

    void setDistanceInterLens(
        float d
    ) { mDistanceInterLens = d; }

    void setDistanceTrayToLens(
        float d
    ) { mDistanceTrayToLens = d; }

    void setFovHalfDegrees(
        std::array<float, 4> d
    ) { mFovHalfDegrees = d; }

    void setDistortionCoeffs(
        std::array<float, 2> d
    ) { mDistortionCoeffs = d; }

    float screen_to_lens_distance() override {
        return mDistanceScreenToLens;
    }

    float inter_lens_distance() override {
        return mDistanceInterLens;
    }

    float tray_to_lens_distance() override {
        return mDistanceTrayToLens;
    }

    cardboard::CBEnumVerticalAlignment vertical_alignment() override {
        return cardboard::CBEnumVerticalAlignment::BOTTOM;
    }

    float distortion_coefficients(
        int index
    ) override {
        return mDistortionCoeffs[index];
    }

    int distortion_coefficients_size() override {
        return mDistortionCoeffs.size();
    }

    float left_eye_field_of_view_angles(
        int index
    ) override {
        return mFovHalfDegrees[index];
    }
};

class APParamsDeviceDefault
: public cardboard::CBParamsDevice {

private:
    static constexpr float kCardboardV1InterLensDistance = 0.06f;
    static constexpr float kCardboardV1TrayToLensDistance = 0.035f;
    static constexpr float kCardboardV1ScreenToLensDistance = 0.042f;
    static constexpr float kCardboardV1FovHalfDegrees[] = {40.0f, 40.0f, 40.0f, 40.0f};
    static constexpr float kCardboardV1DistortionCoeffs[] = {0.441f, 0.156f};
    static constexpr int kCardboardV1DistortionCoeffsSize = 2;
    static constexpr cardboard::CBEnumVerticalAlignment kCardboardV1VerticalAlignmentType =
        cardboard::CBEnumVerticalAlignment::BOTTOM;

    static constexpr char kCardboardV1Vendor[] = "Google, Inc.";
    static constexpr char kCardboardV1Model[] = "Cardboard v1";

public:

    float screen_to_lens_distance() override {
        return kCardboardV1ScreenToLensDistance;
    }

    float inter_lens_distance() override {
        return kCardboardV1InterLensDistance;
    }

    float tray_to_lens_distance() override {
        return kCardboardV1TrayToLensDistance;
    }

    cardboard::CBEnumVerticalAlignment vertical_alignment() override {
        return kCardboardV1VerticalAlignmentType;
    }

    float distortion_coefficients(int index) override {
        return kCardboardV1DistortionCoeffs[index];
    }

    int distortion_coefficients_size() override {
        return kCardboardV1DistortionCoeffsSize;
    }

    float left_eye_field_of_view_angles(int index) override {
        return kCardboardV1FovHalfDegrees[index];
    }
};
#endif //CARDBOARD_DEVICE_PARAMS_IMPL_H
