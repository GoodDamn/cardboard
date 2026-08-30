package com.google.cardboard.opengl;

import android.opengl.GLES30;

import androidx.annotation.NonNull;

public final class VRGLProgram {

    private int mDescriptorProgram = 0;

    public final int getUniformLocation(
        @NonNull final String uniform
    ) {
        return GLES30.glGetUniformLocation(
            mDescriptorProgram,
            uniform
        );
    }
    public final int getAttribLocation(
        @NonNull final String attribute
    ) {
        return GLES30.glGetAttribLocation(
            mDescriptorProgram,
            attribute
        );
    }

    public final void compile(
        @NonNull final String codeShaderVertex,
        @NonNull final String codeShaderFragment
    ) {
        mDescriptorProgram = GLES30.glCreateProgram();
        final int shaderVertex = createCompiledShader(
            codeShaderVertex,
            GLES30.GL_VERTEX_SHADER
        );

        final int shaderFragment = createCompiledShader(
            codeShaderFragment,
            GLES30.GL_FRAGMENT_SHADER
        );

        GLES30.glAttachShader(
            mDescriptorProgram,
            shaderVertex
        );

        GLES30.glAttachShader(
            mDescriptorProgram,
            shaderFragment
        );

        GLES30.glLinkProgram(
            mDescriptorProgram
        );
    }

    public final void use() {
        GLES30.glUseProgram(
            mDescriptorProgram
        );
    }

    private final int createCompiledShader(
        @NonNull final String code,
        @NonNull final int shaderType
    ) {
        final int shader = GLES30.glCreateShader(
            shaderType
        );

        GLES30.glShaderSource(
            shader,
            code
        );

        GLES30.glCompileShader(
            shader
        );

        return shader;
    }
}
