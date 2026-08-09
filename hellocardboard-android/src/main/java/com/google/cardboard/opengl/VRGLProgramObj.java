package com.google.cardboard.opengl;

import android.opengl.GLES30;

import com.google.cardboard.utils.VRUtilsShaderCode;

import androidx.annotation.NonNull;

public final class VRGLProgramObj {

    @NonNull
    private final VRGLProgram mProgram = new VRGLProgram();

    private int mAttrPosition;
    private int mAttrUv;
    private int mUniformModelViewProjection;

    public final void create() {
        mProgram.compile(
            VRUtilsShaderCode.CODE_SHADER_VERTEX_OBJ,
            VRUtilsShaderCode.CODE_SHADER_FRAGMENT_OBJ
        );

        mAttrPosition = mProgram.getAttribLocation(
            "a_Position"
        );

        mAttrUv = mProgram.getAttribLocation(
            "a_UV"
        );

        mUniformModelViewProjection = mProgram.getUniformLocation(
            "u_MVP"
        );
    }

    public final int getAttrPosition() {
        return mAttrPosition;
    }

    public final int getAttrUv() {
        return mAttrUv;
    }

    public final int getUniformMVP() {
        return mUniformModelViewProjection;
    }

}
