package good.damn.sdk2.matrix;

import androidx.annotation.NonNull;

public final class SDKMatrix4x4 {

    @NonNull
    private final float[][] m = new float[4][4];

    public static void writeIdentity(
        @NonNull final SDKMatrix4x4 out
    ) {
        for (int j = 0; j < 4; ++j) {
            for (int i = 0; i < 4; ++i) {
                out.m[j][i] = (i == j) ? 1 : 0;
            }
        }
    }


    public static void writeZeros(
        @NonNull final SDKMatrix4x4 out
    ) {
        for (int j = 0; j < 4; ++j) {
            for (int i = 0; i < 4; ++i) {
                out.m[j][i] = 0;
            }
        }
    }

    // @brief Constructs a translation matrix from [@p x, @p y, @p z] position.
    // @param x The x position coordinate.
    // @param y The y position coordinate.
    // @param z The z position coordinate.
    // @returns A translation matrix.
    public static void writeTranslation(
        float x,
        float y,
        float z,
        @NonNull final SDKMatrix4x4 out
    ) {
        writeIdentity(out);
        out.m[3][0] = x;
        out.m[3][1] = y;
        out.m[3][2] = z;
    }

    // @brief Constructs a projection matrix from the field of view half angles
    //        and the z-coordinate of the near and far clipping planes.
    // @param fov An array with the half angles of the field of view.
    // @param zNear The z coordinate of the near clipping plane.
    // @param zFar The z coordinate of the far clipping plane.
    // @return A projection matrix.
    public static void writePerspective(
        @NonNull final float[] fov,
        float zNear,
        float zFar,
        @NonNull final SDKMatrix4x4 out
    ) {
        writeZeros(out);

        final float xLeft = (float) (-Math.tan(fov[0]) * zNear);
        final float xRight = (float) (Math.tan(fov[1]) * zNear);
        final float yBottom = (float) (-Math.tan(fov[2]) * zNear);
        final float yTop = (float) (Math.tan(fov[3]) * zNear);

        final float X = (2 * zNear) / (xRight - xLeft);
        final float Y = (2 * zNear) / (yTop - yBottom);
        final float A = (xRight + xLeft) / (xRight - xLeft);
        final float B = (yTop + yBottom) / (yTop - yBottom);
        final float C = (zNear + zFar) / (zNear - zFar);
        final float D = (2 * zNear * zFar) / (zNear - zFar);

        out.m[0][0] = X;
        out.m[2][0] = A;
        out.m[1][1] = Y;
        out.m[2][1] = B;
        out.m[2][2] = C;
        out.m[3][2] = D;
        out.m[2][3] = -1;
    }

    // @brief Copies into @p array the contents of `this` matrix.
    // @param[out] array A pointer to a float array of size 16.

    public final void toArray(
        @NonNull final float[] outMatrix16
    ) {
        int index = 0;
        for (int j = 0; j < 4; ++j) {
            for (int i = 0; i < 4; ++i) {
                outMatrix16[index++] = m[i][j];
            }
        }
    }

    public void toArray(
        @NonNull final float[][] outMatrix16
    ) {
        for (int j = 0; j < 4; ++j) {
            for (int i = 0; i < 4; ++i) {
                outMatrix16[j][i] = m[i][j];
            }
        }
    }
}
