package com.google.cardboard;

import android.content.res.AssetManager;
import android.opengl.GLES30;
import android.opengl.Matrix;

import com.google.cardboard.opengl.VRGLMeshTextured;
import com.google.cardboard.opengl.VRGLProgram;
import com.google.cardboard.opengl.VRGLProgramObj;
import com.google.cardboard.opengl.VRGLTexture;
import com.google.cardboard.utils.VRUtilsShaderCode;

import androidx.annotation.Nullable;
import good.damn.sdk2.SDKLensDistortion;
import good.damn.sdk2.device.SDKParamsDeviceImpl;
import good.damn.sdk2.matrix.SDKMatrix4x4;
import good.damn.sdk2.models.SDKMParamsDevice;

import androidx.annotation.NonNull;
import good.damn.sdk2.models.mesh.SDKMMesh;
import good.damn.sdk2.models.mesh.distortion.SDKMeshDistortionLeftEye;
import good.damn.sdk2.models.mesh.distortion.SDKMeshDistortionRightEye;
import good.damn.sdk2.models.mesh.eye.SDKMMeshEye;

public final class VRApplication {

    private static final float METERS_PER_INCH = 0.0254f;

    @NonNull
    private final float[] mvp = new float[16];
    @NonNull
    private final VRGLProgramObj mProgramObj = new VRGLProgramObj();

    @NonNull
    private final VRGLMeshTextured meshRoom = new VRGLMeshTextured();

    @NonNull
    private final VRGLTexture mTextureRoom = new VRGLTexture();

    @NonNull
    private SDKParamsDeviceImpl mParamsDevice;

    @NonNull
    private SDKMMesh[] meshes;


    @NonNull
    private SDKLensDistortion mLensDistortion;

    @Nullable
    private AssetManager mAssets = null;

    private float mScreenWidthMeters = 0.0f;
    private float mScreenHeightMeters = 0.0f;

    private int mScreenWidth = 0;
    private int mScreenHeight = 0;

    public final void destroy() {

    }

    public final void create(
            @NonNull final AssetManager assets,
            @NonNull final SDKMParamsDevice params
    ) {
        mAssets = assets;
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

    public final void surfaceCreated() {
        mProgramObj.create();

        meshRoom.initialize(
            mProgramObj.getAttrPosition(),
            mProgramObj.getAttrUv(),
            "CubeRoom.obj",
            mAssets
        );

        mTextureRoom.initialize(
            mAssets,
            "CubeRoom_BakedDiffuse.png"
        );

        GLES30.glEnable(
            GLES30.GL_DEPTH_TEST
        );
    }

    public final void draw() {
        GLES30.glViewport(
            0,
            0,
            mScreenWidth,
            mScreenHeight
        );

        GLES30.glClearColor(
            0.0f,
            1.0f,
            0.0f,
            1.0f
        );

        GLES30.glClear(
            GLES30.GL_COLOR_BUFFER_BIT |
                GLES30.GL_DEPTH_BUFFER_BIT
        );

        mProgramObj.use();
        GLES30.glUniformMatrix4fv(
            mProgramObj.getUniformMVP(),
            1,
            false,
            mvp,
            0
        );
        mTextureRoom.bind();
        meshRoom.draw();
    }

    public final void setScreenParams(
            final int width,
            final int height,
            final float xdpi,
            final float ydpi
    ) {
        mScreenWidthMeters = ((width) / xdpi) * METERS_PER_INCH;
        mScreenHeightMeters = ((height) / ydpi) * METERS_PER_INCH;
        mScreenWidth = width;
        mScreenHeight = height;
        Matrix.perspectiveM(
            mvp,
            0,
            80f,
            (float)mScreenWidth / mScreenHeight,
            0.001f,
            500f
        );

        float[] view = new float[16];
        Matrix.setIdentityM(view, 0);
        Matrix.setLookAtM(
            view,
            0,
            0f,
            3f,
            3f,
            0f,
            3f,
            0f,
            0f,
            1.0f,
            0.0f
        );

        Matrix.multiplyMM(
            mvp,
            0,
            mvp,
            0,
            view,
            0
        );
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

        return true;
    }

}
