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

#ifndef HELLO_CARDBOARD_ANDROID_SRC_MAIN_JNI_UTIL_H_
#define HELLO_CARDBOARD_ANDROID_SRC_MAIN_JNI_UTIL_H_

#include <android/asset_manager.h>
#include <jni.h>
#include <array>
#include <vector>

#include <GLES2/gl2.h>

#define LOG_TAG "HelloCardboardApp"
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define HELLOCARDBOARD_CHECK(condition)                                    \
  if (!(condition)) {                                                      \
    LOGE("*** CHECK FAILED at %s:%d: %s", __FILE__, __LINE__, #condition); \
    abort();                                                               \
  }

namespace ndk_hello_cardboard {

class Matrix4x4 {
 public:
  float m[4][4];

  // Multiplies two matrices.
  Matrix4x4 operator*(const Matrix4x4& right);

  // Multiplies a matrix with a vector.
  std::array<float, 4> operator*(const std::array<float, 4>& vec);

  // Converts a matrix to an array of floats suitable for passing to OpenGL.
  std::array<float, 16> ToGlArray();
};

struct Quatf {
  float x;
  float y;
  float z;
  float w;

  Quatf(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}

  Quatf() : x(0), y(0), z(0), w(1) {}

  static Quatf FromXYZW(float q[4]) { return Quatf(q[0], q[1], q[2], q[3]); }

  Matrix4x4 ToMatrix();
};

/**
 * Converts an array of floats to a matrix.
 *
 * @param vec GL array
 * @return Obtained matrix
 */
Matrix4x4 GetMatrixFromGlArray(float* vec);

/**
 * Construct a translation matrix.
 *
 * @param translation Translation array
 * @return Obtained matrix
 */
Matrix4x4 GetTranslationMatrix(const std::array<float, 3>& translation);

/**
 * Computes the angle between two vectors.
 *
 * @param vec1 First vector
 * @param vec2 Second vector
 * @return Angle between the vectors
 */
float AngleBetweenVectors(const std::array<float, 4>& vec1,
                          const std::array<float, 4>& vec2);

/**
 * Gets system boot time in nanoseconds.
 *
 * @return System boot time in nanoseconds
 */
int64_t GetBootTimeNano();

/**
 * Checks for OpenGL errors, and crashes if one has occurred.  Note that this
 * can be an expensive call, so real applications should call this rarely.
 *
 * @param file File name
 * @param line Line number
 * @param label Error label
 */
void CheckGlError(const char* file, int line, const char* label);

#define CHECKGLERROR(label) CheckGlError(__FILE__, __LINE__, label)

/**
 * Converts a string into an OpenGL ES shader.
 *
 * @param type The type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER).
 * @param shader_source The source code of the shader.
 * @return The shader object handler, or 0 if there's an error.
 */
GLuint LoadGLShader(GLenum type, const char* shader_source);

}  // namespace ndk_hello_cardboard

#endif  // HELLO_CARDBOARD_ANDROID_SRC_MAIN_JNI_UTIL_H_
