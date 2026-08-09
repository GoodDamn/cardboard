package com.google.cardboard.renderer;

import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.util.DisplayMetrics;
import android.util.Log;

import com.google.cardboard.VRApplication;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import androidx.annotation.NonNull;

public final class VRRendererImpl
implements GLSurfaceView.Renderer {

    @NonNull
    private final VRApplication mApplication;

    private final float mDpiX;
    private final float mDpiY;

    public VRRendererImpl(
        @NonNull final VRApplication app,
        @NonNull final DisplayMetrics metrics
    ) {
        mApplication = app;
        mDpiX = metrics.xdpi;
        mDpiY = metrics.ydpi;
    }

    @Override
    public void onSurfaceCreated(
        @NonNull final GL10 g,
        @NonNull final EGLConfig config
    ) {

    }

    @Override
    public void onSurfaceChanged(
        @NonNull final GL10 g,
        final int width,
        final int height
    ) {
        mApplication.setScreenParams(
            width,
            height,
            mDpiX,
            mDpiY
        );
    }

    @Override
    public void onDrawFrame(
        @NonNull final GL10 g
    ) {
        mApplication.draw();
    }
}
