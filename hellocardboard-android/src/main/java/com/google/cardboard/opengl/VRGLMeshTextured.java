package com.google.cardboard.opengl;

import android.content.res.AssetManager;
import android.opengl.GLES30;

import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

public final class VRGLMeshTextured {

    private int mAttrPosition = 0;
    private int mAttrUv = 0;

    @Nullable
    private FloatBuffer mBufferPosition;

    @Nullable
    private FloatBuffer mBufferUv;

    @Nullable
    private ShortBuffer mBufferIndices;

    public final void initialize(
        final int attrPosition,
        final int attrUv,
        @NonNull final String objFilePath,
        @NonNull final AssetManager assets
    ) {
        mAttrPosition = attrPosition;
        mAttrUv = attrUv;

    }

    public final void draw() {
        GLES30.glEnableVertexAttribArray(
            mAttrPosition
        );

        GLES30.glVertexAttribPointer(
            mAttrPosition,
            3,
            GLES30.GL_FLOAT,
            false,
            0,
            mBufferPosition
        );

        GLES30.glEnableVertexAttribArray(
            mAttrUv
        );

        GLES30.glVertexAttribPointer(
            mAttrUv,
            2,
            GLES30.GL_FLOAT,
            false,
            0,
            mBufferUv
        );

        GLES30.glDrawElements(
            GLES30.GL_TRIANGLES,
            mBufferIndices.capacity(),
            GLES30.GL_UNSIGNED_SHORT,
            mBufferIndices
        );
    }

}
