package com.google.cardboard.utils;

public final class VRUtilsShaderCode {

    public static final String CODE_SHADER_VERTEX_OBJ =
        "uniform mat4 u_MVP;\n" +
        "    attribute vec4 a_Position;\n" +
        "    attribute vec2 a_UV;\n" +
        "    varying vec2 v_UV;\n" +
        "\n" +
        "    void main() {\n" +
        "      v_UV = a_UV;\n" +
        "      gl_Position = u_MVP * a_Position;\n" +
        "    }";


    public static final String CODE_SHADER_FRAGMENT_OBJ =
        "precision mediump float;\n" +
        "\n" +
        "    uniform sampler2D u_Texture;\n" +
        "    varying vec2 v_UV;\n" +
        "\n" +
        "    void main() {\n" +
        "      // The y coordinate of this sample's textures is reversed compared to\n" +
        "      // what OpenGL expects, so we invert the y coordinate.\n" +
        "      gl_FragColor = texture2D(u_Texture, vec2(v_UV.x, 1.0 - v_UV.y));\n" +
        "    }";

}
