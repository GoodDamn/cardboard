package com.google.cardboard;

import android.content.res.AssetManager;

import good.damn.sdk2.SDKLensDistortion;
import good.damn.sdk2.device.SDKParamsDeviceImpl;
import good.damn.sdk2.models.SDKMParamsDevice;

import androidx.annotation.NonNull;
import good.damn.sdk2.models.mesh.SDKMMesh;
import good.damn.sdk2.models.mesh.distortion.SDKMeshDistortionLeftEye;
import good.damn.sdk2.models.mesh.distortion.SDKMeshDistortionRightEye;
import good.damn.sdk2.models.mesh.eye.SDKMMeshEye;

public final class VRApplication {

    private static final float METERS_PER_INCH = 0.0254f;

    @NonNull
    private SDKParamsDeviceImpl mParamsDevice;

    @NonNull
    private SDKMMesh[] meshes;


    @NonNull
    private SDKLensDistortion mLensDistortion;

    private float mScreenWidthMeters = 0.0f;
    private float mScreenHeightMeters = 0.0f;

    public final void destroy() {

    }

    public final void create(
            @NonNull final AssetManager assets,
            @NonNull final SDKMParamsDevice params
    ) {
        mParamsDevice = new SDKParamsDeviceImpl(
            params
        );

        meshes = new SDKMMesh[2];

        @NonNull
        final SDKMMesh meshLeft = new SDKMMesh(
                new SDKMeshDistortionLeftEye(),
                new SDKMMeshEye()
        );

        @NonNull
        final SDKMMesh meshRight = new SDKMMesh(
                new SDKMeshDistortionRightEye(),
                new SDKMMeshEye()
        );

        meshes[0] = meshLeft;
        meshes[1] = meshRight;
    }

    public final void resume() {

    }

    public final void pause() {

    }

    public final void setScreenParams(
            final int width,
            final int height,
            final float xdpi,
            final float ydpi
    ) {
        mScreenWidthMeters = ((width) / xdpi) * METERS_PER_INCH;
        mScreenHeightMeters = ((height) / ydpi) * METERS_PER_INCH;
        // screen_params_changed_ = true;
    }

    private final boolean updateParams() {
        if (mLensDistortion != null) {
            return true;
        }

        mLensDistortion = new SDKLensDistortion(
                mParamsDevice,
                mScreenWidthMeters,
                mScreenHeightMeters,
                meshes
        );
    }

}
