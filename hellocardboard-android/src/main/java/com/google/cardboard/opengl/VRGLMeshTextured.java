package com.google.cardboard.opengl;

import android.content.res.AssetManager;
import android.opengl.GLES30;
import android.util.Log;

import com.google.cardboard.utils.VRUtilsLoaderObj;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;
import java.util.Vector;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

public final class VRGLMeshTextured {

    private static final String TAG = "VRGLMeshTextured";

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

        @NonNull
        final Vector<Float> positions = new Vector<>();

        @NonNull
        final Vector<Float> uvs = new Vector<>();

        @NonNull
        final Vector<Short> indices = new Vector<>();

        @NonNull
        final Vector<Float> normals = new Vector<>();

        try {
            VRUtilsLoaderObj.loadObjFile(
                assets,
                objFilePath,
                positions,
                normals,
                uvs,
                indices
            );

            mBufferPosition = createVectorBufferFloat(
                positions
            );

            mBufferUv = createVectorBufferFloat(
                uvs
            );

            mBufferIndices = createVectorBufferShort(
                indices
            );

            Log.d(TAG, "initialize: LAST_VALUES: POSITION: " + positions.lastElement() + " UV: " + uvs.lastElement() + " INDICES: " + indices.lastElement());
        } catch (IOException e) {
            Log.d(TAG, "initialize: " + e.getMessage());
        }
    }

    public final void draw() {
        if (mBufferIndices == null) {
            return;
        }

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

    @NonNull
    private final ShortBuffer createVectorBufferShort(
        @NonNull final Vector<Short> input
    ) {
        @NonNull
        final ShortBuffer buffer = ByteBuffer.allocateDirect(
            input.size() * 2
        ).order(
            ByteOrder.nativeOrder()
        ).asShortBuffer();

        for (int i = 0; i < input.size(); i++) {
            buffer.put(
                i,
                input.get(i)
            );
        }

        buffer.position(0);
        return buffer;
    }

    @NonNull
    private final FloatBuffer createVectorBufferFloat(
        @NonNull final Vector<Float> input
    ) {
        @NonNull
        final FloatBuffer buffer = ByteBuffer.allocateDirect(
            input.size() * 4
        ).order(
            ByteOrder.nativeOrder()
        ).asFloatBuffer();

        for (int i = 0; i < input.size(); i++) {
            buffer.put(
                i,
                input.get(i)
            );
        }

        buffer.position(0);
        return buffer;
    }

}
