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

#include "hello_cardboard_app.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

#include <array>
#include <cmath>
#include <fstream>

#include "cardboard.h"
#include "device_params_impl.h"

namespace ndk_hello_cardboard {

    namespace {

// 6 Hz cutoff frequency for the velocity filter of the head tracker.
        constexpr int kVelocityFilterCutoffFrequency = 6;

        constexpr uint64_t kPredictionTimeWithoutVsyncNanos = 50000000;

        constexpr float kMetersPerInch = 0.0254f;

    }  // anonymous namespace

    template<std::size_t SIZE>
    void convertJniToFloatArray(
        JNIEnv *env,
        jfloatArray *inp,
        std::array<float, SIZE> &output
    ) {
        jfloat *elements = env->GetFloatArrayElements(
            *inp,
            nullptr
        );

        for (unsigned int i = 0; i < SIZE; i++) {
            output[i] = elements[i];
        }

        env->ReleaseFloatArrayElements(
            *inp,
            elements,
            JNI_ABORT
        );
    }

    HelloCardboardApp::HelloCardboardApp(
        JavaVM *vm,
        jobject obj,
        jfloat interLensDistance,
        jfloat trayToLensDistance,
        jfloat screenToLensDistance,
        jfloatArray fovHalfDegrees,
        jfloatArray distortionCoeffs,
        jobject instanceDrawer
    ) : head_tracker_(nullptr),
        lens_distortion_(nullptr),
        distortion_renderer_(nullptr),
        screen_params_changed_(false),
        device_params_changed_(false),
        screen_width_(0),
        screen_height_(0),
        depthRenderBuffer_(0),
        framebuffer_(0),
        texture_(0) {
        JNIEnv *env;
        vm->GetEnv((void **) &env, JNI_VERSION_1_6);

        instanceDrawer_ = env->NewGlobalRef(
            instanceDrawer
        );

        mDeviceParams.setDistanceInterLens(
            interLensDistance
        );

        mDeviceParams.setDistanceTrayToLens(
            trayToLensDistance
        );

        mDeviceParams.setDistanceScreenToLens(
            screenToLensDistance
        );

        std::array<float, 4ul> outFov{};
        convertJniToFloatArray(
            env,
            &fovHalfDegrees,
            outFov
        );

        mDeviceParams.setFovHalfDegrees(
            outFov
        );

        std::array<float, 2> distCoef;
        convertJniToFloatArray(
            env,
            &distortionCoeffs,
            distCoef
        );

        mDeviceParams.setDistortionCoeffs(
            distCoef
        );

        meshes.clear();

        auto *meshLeft = new CBMesh();
        auto *meshRight = new CBMesh();

        meshLeft->meshDistortion = std::make_unique<CBDistortionMeshEyeLeft>();
        meshRight->meshDistortion = std::make_unique<CBDistortionMeshEyeRight>();

        meshLeft->meshRender = new CardboardMesh();
        meshRight->meshRender = new CardboardMesh();

        meshes.push_back(
            meshLeft
        );

        meshes.push_back(
            meshRight
        );

        Cardboard_initializeAndroid(vm, obj);
        head_tracker_ = CardboardHeadTracker_create();
        CardboardHeadTracker_setLowPassFilter(
            head_tracker_,
            kVelocityFilterCutoffFrequency
        );
    }

    HelloCardboardApp::~HelloCardboardApp() {
        CardboardHeadTracker_destroy(head_tracker_);
        CardboardLensDistortion_destroy(lens_distortion_);
        CardboardDistortionRenderer_destroy(distortion_renderer_);
    }

    void HelloCardboardApp::SetScreenParams(
        int width,
        int height,
        float xdpi,
        float ydpi
    ) {
        screen_width_ = width;
        screen_height_ = height;
        mScreenWidthMeters = (static_cast<float>(width) / xdpi) * kMetersPerInch;
        mScreenHeightMeters = (static_cast<float>(height) / ydpi) * kMetersPerInch;
        screen_params_changed_ = true;
    }


    void HelloCardboardApp::OnDrawFrame(
        JNIEnv* env
    ) {
        if (!UpdateDeviceParams() || instanceDrawer_ == nullptr) {
            return;
        }

        // get drawer instance
        jclass interfaceDrawer = env->GetObjectClass(
            instanceDrawer_
        );

        if (interfaceDrawer == nullptr) {
            LOGD("interfaceDrawer == nullptr");
            throw std::exception();
            return;
        }

        jmethodID instanceDrawerMethod_ = env->GetMethodID(
            interfaceDrawer,
            "onDraw",
            "(I)V"
        );

        if (instanceDrawerMethod_ == nullptr) {
            LOGD("methodId == nullptr");
            throw std::exception();
            return;
        }

        // Update Head Pose.
        matrixPose = GetPose();

        // Bind buffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glDisable(GL_SCISSOR_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // LeftEye
        glViewport(
            0,
            0,
            screen_width_ / 2,
            screen_height_
        );

        jint indexEye = 0;
        // Draw room and target
        env->CallVoidMethod(
            instanceDrawer_,
            instanceDrawerMethod_,
            indexEye
        );

        // RightEye
        glViewport(
            screen_width_ / 2,
            0,
            screen_width_ / 2,
            screen_height_
        );

        indexEye = 1;
        // Draw room and target
        env->CallVoidMethod(
            instanceDrawer_,
            instanceDrawerMethod_,
            indexEye
        );

        env->DeleteLocalRef(
            interfaceDrawer
        );

        // Render
        CardboardDistortionRenderer_renderEyeToDisplay(
            distortion_renderer_,
            /* target_display = */ 0,
            /* x = */ 0,
            /* y = */ 0,
            screen_width_,
            screen_height_,
            meshes[0]->meshRender,
            meshes[1]->meshRender
        );

        CHECKGLERROR("onDrawFrame");
    }

    void HelloCardboardApp::OnPause() { CardboardHeadTracker_pause(head_tracker_); }

    void HelloCardboardApp::getPose(
        JNIEnv* env,
        jfloatArray matrixOut,
        jint index_eye,
        jfloat positionX,
        jfloat positionY,
        jfloat positionZ
    ) {
        if (meshes.empty()) {
            return;
        }

        CardboardMesh* mesh = meshes[
            index_eye
        ]->meshRender;

        Matrix4x4 eye_matrix = GetMatrixFromGlArray(
            mesh->eye_matrix
        );

        Matrix4x4 projection_matrix = GetMatrixFromGlArray(
            mesh->projection_matrix
        );

        Matrix4x4 b = projection_matrix *
               eye_matrix *
               matrixPose *
               GetTranslationMatrix({positionX, positionY, positionZ});

        env->SetFloatArrayRegion(
            matrixOut,
            0,
            16,
            b.m[0]
        );
    }

    void HelloCardboardApp::OnResume() {
        CardboardHeadTracker_resume(head_tracker_);

        // Parameters may have changed.
        device_params_changed_ = true;
    }

    bool HelloCardboardApp::UpdateDeviceParams() {
        // Checks if screen or device parameters changed
        if (!screen_params_changed_ && !device_params_changed_) {
            return true;
        }

        CardboardLensDistortion_destroy(lens_distortion_);

        lens_distortion_ = CardboardLensDistortion_create(
            mScreenWidthMeters,
            mScreenHeightMeters,
            &meshes,
            &mDeviceParams
        );

        GlSetup();
        CardboardDistortionRenderer_destroy(distortion_renderer_);
        const CardboardOpenGlEsDistortionRendererConfig config{kGlTexture2D};
        distortion_renderer_ = CardboardOpenGlEs3DistortionRenderer_create(&config);

        for (uint32_t i = 0; i < meshes.size(); i++) {
            CardboardMesh *meshRender = meshes[i]->meshRender;
            meshRender->id = i;
            CardboardLensDistortion_getDistortionMesh(
                lens_distortion_,
                meshRender
            );

            CardboardDistortionRenderer_setMesh(
                distortion_renderer_,
                meshRender
            );

            CardboardLensDistortion_getEyeFromHeadMatrix(
                lens_distortion_,
                meshRender
            );

            CardboardLensDistortion_getProjectionMatrix(
                lens_distortion_,
                meshRender,
                kZNear,
                kZFar
            );
        }

        screen_params_changed_ = false;
        device_params_changed_ = false;

        CHECKGLERROR("UpdateDeviceParams");

        return true;
    }

    void HelloCardboardApp::GlSetup() {
        LOGD("GL SETUP");

        if (framebuffer_ != 0) {
            GlTeardown();
        }

        // Create render texture.
        glGenTextures(1, &texture_);
        glBindTexture(GL_TEXTURE_2D, texture_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_width_, screen_height_, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, 0);

        CardboardEyeTextureDescription *leftEye = &meshes[0]->meshRender->textureDescription;
        CardboardEyeTextureDescription *rightEye = &meshes[1]->meshRender->textureDescription;

        leftEye->texture = texture_;
        leftEye->left_u = 0.0f;
        leftEye->right_u = 0.5f;
        leftEye->top_v = 1.0f;
        leftEye->bottom_v = 0.0f;

        rightEye->texture = texture_;
        rightEye->left_u = 0.5f;
        rightEye->right_u = 1.0f;
        rightEye->top_v = 1.0f;
        rightEye->bottom_v = 0.0f;

        // Generate depth buffer to perform depth test.
        glGenRenderbuffers(1, &depthRenderBuffer_);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, screen_width_,
                              screen_height_);
        CHECKGLERROR("Create Render buffer");

        // Create render target.
        glGenFramebuffers(1, &framebuffer_);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                               texture_, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER, depthRenderBuffer_);

        CHECKGLERROR("GlSetup");
    }

    void HelloCardboardApp::GlTeardown() {
        if (framebuffer_ == 0) {
            return;
        }
        glDeleteRenderbuffers(1, &depthRenderBuffer_);
        depthRenderBuffer_ = 0;
        glDeleteFramebuffers(1, &framebuffer_);
        framebuffer_ = 0;
        glDeleteTextures(1, &texture_);
        texture_ = 0;

        CHECKGLERROR("GlTeardown");
    }

    Matrix4x4 HelloCardboardApp::GetPose() {
        std::array<float, 4> out_orientation;
        std::array<float, 3> out_position;
        CardboardHeadTracker_getPose(
            head_tracker_, GetBootTimeNano() + kPredictionTimeWithoutVsyncNanos,
            kLandscapeLeft, &out_position[0], &out_orientation[0]);
        return GetTranslationMatrix(out_position) *
               Quatf::FromXYZW(&out_orientation[0]).ToMatrix();
    }

    /*bool HelloCardboardApp::IsPointingAtTarget() {
        // Compute vectors pointing towards the reticle and towards the target object
        // in head space.
        Matrix4x4 head_from_target = matrixPose * model_target_;

        const std::array<float, 4> unit_quaternion = {0.f, 0.f, 0.f, 1.f};
        const std::array<float, 4> target_vector = head_from_target * unit_quaternion;

        const std::array<float, 4> point_vector = {0.f, 0.f, -1.f, 0.f};
        float angle = AngleBetweenVectors(point_vector, target_vector);
        return angle < 0.2f;
    }*/

}  // namespace ndk_hello_cardboard
