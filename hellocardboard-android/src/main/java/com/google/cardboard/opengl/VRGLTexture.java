package com.google.cardboard.opengl;

import android.content.res.AssetManager;
import android.graphics.BitmapFactory;
import android.opengl.GLES30;
import android.opengl.GLUtils;
import android.util.Log;

import java.io.IOException;

import androidx.annotation.NonNull;

public final class VRGLTexture {

    private static final String TAG = "VRGLTexture";
    private int[] mDescriptor = new int[1];

    public final void initialize(
        @NonNull final AssetManager assets,
        @NonNull final String texturePath
    ) {
        GLES30.glGenTextures(
            1,
            mDescriptor,
            0
        );

        bind();
        GLES30.glTexParameteri(
            GLES30.GL_TEXTURE_2D,
            GLES30.GL_TEXTURE_WRAP_S,
            GLES30.GL_CLAMP_TO_EDGE
        );
        GLES30.glTexParameteri(
            GLES30.GL_TEXTURE_2D,
            GLES30.GL_TEXTURE_WRAP_T,
            GLES30.GL_CLAMP_TO_EDGE
        );

        GLES30.glTexParameteri(
            GLES30.GL_TEXTURE_2D,
            GLES30.GL_TEXTURE_MIN_FILTER,
            GLES30.GL_LINEAR_MIPMAP_NEAREST
        );

        GLES30.glTexParameteri(
            GLES30.GL_TEXTURE_2D,
            GLES30.GL_TEXTURE_MAG_FILTER,
            GLES30.GL_LINEAR
        );

        try {
            GLUtils.texImage2D(
                GLES30.GL_TEXTURE_2D,
                0,
                BitmapFactory.decodeStream(
                    assets.open(texturePath)
                ),
                0
            );
        } catch (IOException e) {
            Log.d(TAG, "initialize: Couldn't load texture: " + e.getMessage());
        }

        GLES30.glGenerateMipmap(
            GLES30.GL_TEXTURE_2D
        );
    }

    public final void bind() {
        GLES30.glActiveTexture(
            GLES30.GL_TEXTURE0
        );

        GLES30.glBindTexture(
            GLES30.GL_TEXTURE_2D,
            mDescriptor[0]
        );
    }

}
