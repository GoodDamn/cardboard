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

#ifndef HELLO_CARDBOARD_ANDROID_SRC_MAIN_JNI_HELLO_CARDBOARD_APP_H_
#define HELLO_CARDBOARD_ANDROID_SRC_MAIN_JNI_HELLO_CARDBOARD_APP_H_

#include <android/asset_manager.h>
#include <jni.h>

#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <GLES2/gl2.h>
#include "cardboard.h"
#include "util.h"
#include "distortion_eyes.h"
#include "device_params_impl.h"

namespace ndk_hello_cardboard {

/**
 * This is a sample app for the Cardboard SDK. It loads a simple environment and
 * objects that you can click on.
 */
class HelloCardboardApp {
    std::vector<CBMesh*> meshes;
    APParamsDeviceMutable mDeviceParams;
 public:
  /**
   * Creates a HelloCardboardApp.
   *
   * @param vm JavaVM pointer.
   * @param obj Android activity object.
   * @param asset_mgr_obj The asset manager object.
   */
  HelloCardboardApp(
      JavaVM* vm,
      jobject obj,
      jfloat interLensDistance,
      jfloat trayToLensDistance,
      jfloat screenToLensDistance,
      jfloatArray fovHalfDegrees,
      jfloatArray distortionCoeffs,
      jobject instanceDrawer);

  ~HelloCardboardApp();

  /**
   * Sets screen parameters.
   *
   * @param width Screen width
   * @param height Screen height
   */
  void SetScreenParams(
    int width,
    int height,
    float xdpi,
    float ydpi
  );

  /**
   * Draws the scene. This should be called on the rendering thread.
   */
  void OnDrawFrame(
      JNIEnv* env
  );

  /**
   * Pauses head tracking.
   */
  void OnPause();

  /**
   * Resumes head tracking.
   */
  void OnResume();

  void getPose(
      JNIEnv* env,
      jfloatArray matrixOut,
      jint index_eye,
      jfloat positionX,
      jfloat positionY,
      jfloat positionZ
  );

  /**
   * Allows user to switch viewer.
   */
  void SwitchViewer();

 private:
  /**
   * Default near clip plane z-axis coordinate.
   */
  static constexpr float kZNear = 0.1f;

  /**
   * Default far clip plane z-axis coordinate.
   */
  static constexpr float kZFar = 100.f;

  /**
   * Updates device parameters, if necessary.
   *
   * @return true if device parameters were successfully updated.
   */
  bool UpdateDeviceParams();

  /**
   * Initializes GL environment.
   */
  void GlSetup();

  /**
   * Deletes GL environment.
   */
  void GlTeardown();

  /**
   * Gets head's pose as a 4x4 matrix.
   *
   * @return matrix containing head's pose.
   */
  Matrix4x4 GetPose();

  CardboardHeadTracker* head_tracker_;
  CardboardLensDistortion* lens_distortion_;
  CardboardDistortionRenderer* distortion_renderer_;

  bool screen_params_changed_;
  bool device_params_changed_;
  int screen_width_;
  int screen_height_;
  float mScreenWidthMeters;
  float mScreenHeightMeters;

  GLuint depthRenderBuffer_;  // depth buffer
  GLuint framebuffer_;        // framebuffer object
  GLuint texture_;            // distortion texture

  jobject instanceDrawer_;

  Matrix4x4 matrixPose;
};

}  // namespace ndk_hello_cardboard

#endif  // HELLO_CARDBOARD_ANDROID_SRC_MAIN_JNI_HELLO_CARDBOARD_APP_H_
