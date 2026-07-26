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
#include "include/cardboard.h"

#include <cmath>

#include "distortion_renderer.h"
#include "head_tracker.h"
#include "lens_distortion.h"
#include "util/is_arg_null.h"
#include "util/is_initialized.h"
#include "util/logging.h"
#include "include/device_params.h"

#ifdef __ANDROID__
#include "jni_utils/android/jni_utils.h"
#endif

// TODO(b/134142617): Revisit struct/class hierarchy.
struct CardboardLensDistortion : cardboard::LensDistortion {
};
struct CardboardDistortionRenderer : cardboard::DistortionRenderer {
};
struct CardboardHeadTracker : cardboard::HeadTracker {
};

namespace {
    // Return default (identity) matrix.
    void GetDefaultMatrix(float *matrix) {
        if (matrix == nullptr) {
            return;
        }

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                matrix[i * 4 + j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    }

// Return default (all angles equal to 45 degrees) field of view.
    void GetDefaultEyeFieldOfView(float *field_of_view) {
        if (field_of_view == nullptr) {
            return;
        }

        float default_angle = 45.0f * M_PI / 180.0f;
        for (int i = 0; i < 4; ++i) {
            field_of_view[i] = default_angle;
        }
    }

// Return default (empty) distortion mesh.
    void GetDefaultDistortionMesh(CardboardMesh *mesh) {
        if (mesh == nullptr) {
            return;
        }

        mesh->indices = nullptr;
        mesh->n_indices = 0;
        mesh->vertices = nullptr;
        mesh->uvs = nullptr;
        mesh->n_vertices = 0;
    }

// Return default (zero) position.
    void GetDefaultPosition(float *position) {
        if (position == nullptr) {
            return;
        }
        position[0] = 0.0f;
        position[1] = 0.0f;
        position[2] = 0.0f;
    }

// Return default (identity quaternion) orientation.
    void GetDefaultOrientation(float *orientation) {
        if (orientation == nullptr) {
            return;
        }

        orientation[0] = 0.0f;
        orientation[1] = 0.0f;
        orientation[2] = 0.0f;
        orientation[3] = 1.0f;
    }

}  // anonymous namespace

extern "C" {

std::vector<CBMesh*>* _meshes;

#ifdef __ANDROID__
void Cardboard_initializeAndroid(JavaVM *vm, jobject context) {
    if (CARDBOARD_IS_ARG_NULL(vm) || CARDBOARD_IS_ARG_NULL(context)) {
        return;
    }
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    jobject global_context = env->NewGlobalRef(context);

    cardboard::jni::initializeAndroid(vm, global_context);

    cardboard::util::SetIsInitialized();
}
#endif

CardboardLensDistortion* CardboardLensDistortion_create(
    float screenWidthMeters,
    float screenHeightMeters,
    std::vector<CBMesh*>* meshes,
    cardboard::CBParamsDevice* deviceParams
) {
    if (CARDBOARD_IS_NOT_INITIALIZED()) {
        return nullptr;
    }
    _meshes = meshes;

    return reinterpret_cast<CardboardLensDistortion*>(
        new cardboard::LensDistortion(
            screenWidthMeters,
            screenHeightMeters,
            meshes,
            deviceParams
        )
    );
}

void CardboardLensDistortion_destroy(CardboardLensDistortion *lens_distortion) {
    if (CARDBOARD_IS_NOT_INITIALIZED() ||
        CARDBOARD_IS_ARG_NULL(lens_distortion)) {
        return;
    }
    delete lens_distortion;
}

void CardboardLensDistortion_getEyeFromHeadMatrix(
    CardboardLensDistortion *lens_distortion,
    CardboardMesh* mesh
) {
    if (CARDBOARD_IS_NOT_INITIALIZED() ||
        CARDBOARD_IS_ARG_NULL(lens_distortion) ||
        CARDBOARD_IS_ARG_NULL(mesh->eye_matrix)) {
        GetDefaultMatrix(mesh->eye_matrix);
        return;
    }

    _meshes->operator[](mesh->id)->meshDistortion->matrix.getHeadMatrix(
        mesh->eye_matrix
    );
}

void CardboardLensDistortion_getProjectionMatrix(
    CardboardLensDistortion *lens_distortion,
    CardboardMesh* mesh,
    float z_near,
    float z_far
) {
    if (CARDBOARD_IS_NOT_INITIALIZED() ||
        CARDBOARD_IS_ARG_NULL(lens_distortion) ||
        CARDBOARD_IS_ARG_NULL(mesh->projection_matrix)) {
        GetDefaultMatrix(mesh->projection_matrix);
        return;
    }

    _meshes->operator[](mesh->id)->meshDistortion->matrix.getProjectionMatrix(
        z_near,
        z_far,
        mesh->projection_matrix
    );
}

void CardboardLensDistortion_getFieldOfView(
    CardboardLensDistortion *lens_distortion,
    CardboardMesh* mesh,
    float *field_of_view
) {
    if (CARDBOARD_IS_NOT_INITIALIZED() ||
        CARDBOARD_IS_ARG_NULL(lens_distortion) ||
        CARDBOARD_IS_ARG_NULL(field_of_view)) {
        GetDefaultEyeFieldOfView(field_of_view);
        return;
    }

    _meshes->operator[](mesh->id)->meshDistortion->matrix.getFov(
        field_of_view
    );
}

void CardboardLensDistortion_getDistortionMesh(
    CardboardLensDistortion *lens_distortion,
    CardboardMesh* mesh
) {
    if (CARDBOARD_IS_NOT_INITIALIZED() ||
        CARDBOARD_IS_ARG_NULL(lens_distortion) || CARDBOARD_IS_ARG_NULL(mesh)) {
        GetDefaultDistortionMesh(mesh);
        return;
    }

    cardboard::DistortionMesh* distortionMesh = _meshes->operator[](
        mesh->id
    )->meshDistortion->mesh.get();

    std::vector<int>* indices = distortionMesh->getIndices();
    std::vector<float>* vertices = distortionMesh->getVertices();
    std::vector<float>* uvs = distortionMesh->getUv();

    mesh->indices = const_cast<int*>(indices->data());
    mesh->vertices = const_cast<float*>(vertices->data());
    mesh->uvs = const_cast<float*>(uvs->data());
    mesh->n_indices = static_cast<int>(indices->size());
    mesh->n_vertices = static_cast<int>(vertices->size() / 2);
}


void CardboardDistortionRenderer_destroy(
    CardboardDistortionRenderer *renderer) {
    if (CARDBOARD_IS_NOT_INITIALIZED() || CARDBOARD_IS_ARG_NULL(renderer)) {
        return;
    }
    delete renderer;
}

void CardboardDistortionRenderer_setMesh(
    CardboardDistortionRenderer *renderer,
    const CardboardMesh *mesh
) {
    if (CARDBOARD_IS_NOT_INITIALIZED() ||
        CARDBOARD_IS_ARG_NULL(renderer) ||
        CARDBOARD_IS_ARG_NULL(mesh)
    ) {
        return;
    }

    static_cast<cardboard::DistortionRenderer *>(renderer)->SetMesh(
        mesh
    );
}

void CardboardDistortionRenderer_renderEyeToDisplay(
    CardboardDistortionRenderer *renderer,
    uint64_t target,
    int x,
    int y,
    int width,
    int height,
    CardboardMesh *left_eye,
    CardboardMesh *right_eye
) {
    if (CARDBOARD_IS_NOT_INITIALIZED() ||
        CARDBOARD_IS_ARG_NULL(renderer) ||
        CARDBOARD_IS_ARG_NULL(left_eye) ||
        CARDBOARD_IS_ARG_NULL(right_eye)
    ) {
        return;
    }

    static_cast<cardboard::DistortionRenderer *>(
        renderer
    )->RenderEyeToDisplay(
        target,
        x,
        y,
        width,
        height,
        left_eye,
        right_eye
    );
}

CardboardHeadTracker *CardboardHeadTracker_create() {
    if (CARDBOARD_IS_NOT_INITIALIZED()) {
        return nullptr;
    }
    return reinterpret_cast<CardboardHeadTracker *>(new cardboard::HeadTracker());
}

void CardboardHeadTracker_setLowPassFilter(CardboardHeadTracker *head_tracker, const int cutoff_frequency) {
    if (CARDBOARD_IS_NOT_INITIALIZED()) {
        return;
    }
    head_tracker->SetLowPassFilter(cutoff_frequency);
}

void CardboardHeadTracker_destroy(CardboardHeadTracker *head_tracker) {
    if (CARDBOARD_IS_NOT_INITIALIZED() || CARDBOARD_IS_ARG_NULL(head_tracker)) {
        return;
    }
    delete head_tracker;
}

void CardboardHeadTracker_pause(CardboardHeadTracker *head_tracker) {
    if (CARDBOARD_IS_NOT_INITIALIZED() || CARDBOARD_IS_ARG_NULL(head_tracker)) {
        return;
    }
    static_cast<cardboard::HeadTracker *>(head_tracker)->Pause();
}

void CardboardHeadTracker_resume(CardboardHeadTracker *head_tracker) {
    if (CARDBOARD_IS_NOT_INITIALIZED() || CARDBOARD_IS_ARG_NULL(head_tracker)) {
        return;
    }
    static_cast<cardboard::HeadTracker *>(head_tracker)->Resume();
}

void CardboardHeadTracker_getPose(
    CardboardHeadTracker *head_tracker, int64_t timestamp_ns,
    CardboardViewportOrientation viewport_orientation, float *position,
    float *orientation
) {
    if (CARDBOARD_IS_NOT_INITIALIZED() || CARDBOARD_IS_ARG_NULL(head_tracker) ||
        CARDBOARD_IS_ARG_NULL(position) || CARDBOARD_IS_ARG_NULL(orientation)) {
        GetDefaultPosition(position);
        GetDefaultOrientation(orientation);
        return;
    }
    std::array<float, 3> out_position;
    std::array<float, 4> out_orientation;
    static_cast<cardboard::HeadTracker *>(head_tracker)
        ->GetPose(timestamp_ns, viewport_orientation, out_position, out_orientation);
    std::memcpy(position, &out_position[0], 3 * sizeof(float));
    std::memcpy(orientation, &out_orientation[0], 4 * sizeof(float));
}

void CardboardHeadTracker_recenter(CardboardHeadTracker *head_tracker) {
    if (CARDBOARD_IS_NOT_INITIALIZED() || CARDBOARD_IS_ARG_NULL(head_tracker)) {
        return;
    }
    static_cast<cardboard::HeadTracker *>(head_tracker)->Recenter();
}

/*CardboardUv CardboardLensDistortion_undistortedUvForDistortedUv(
    CardboardLensDistortion *lens_distortion,
    const CardboardUv *distorted_uv,
    CardboardEye eye
) {

    if (CARDBOARD_IS_NOT_INITIALIZED() ||
        CARDBOARD_IS_ARG_NULL(lens_distortion) ||
        CARDBOARD_IS_ARG_NULL(distorted_uv)) {
        return CardboardUv{-1, -1}; // u, v
    }

    std::array<float, 2> in = {
        distorted_uv->u,
        distorted_uv->v
    };

    std::array<float, 2> out = static_cast<
        cardboard::LensDistortion *
    >(lens_distortion)->UndistortedUvForDistortedUv(
        meshes[eye],
        in
    );

    CardboardUv ret;
    ret.u = out[0];
    ret.v = out[1];
    return ret;
}

CardboardUv CardboardLensDistortion_distortedUvForUndistortedUv(
    CardboardLensDistortion *lens_distortion, const CardboardUv *undistorted_uv,
    CardboardEye eye) {

    if (CARDBOARD_IS_NOT_INITIALIZED() ||
        CARDBOARD_IS_ARG_NULL(lens_distortion) ||
        CARDBOARD_IS_ARG_NULL(undistorted_uv)) {
        return CardboardUv{-1, -1}; // u, v
    }

    std::array<float, 2> in = {
        undistorted_uv->u,
        undistorted_uv->v
    };

    std::array<float, 2> out = static_cast<
        cardboard::LensDistortion *
        >(lens_distortion)->DistortedUvForUndistortedUv(
        meshes[eye],
        in
    );

    CardboardUv ret;
    ret.u = out[0];
    ret.v = out[1];
    return ret;
}*/


}  // extern "C"
