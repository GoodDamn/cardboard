package good.damn.sdk2.models.mesh.distortion;

import androidx.annotation.NonNull;
import good.damn.sdk2.matrix.SDKMatrix4x4;

public final class SDKMeshDistortionMatrix {

    @NonNull
    public final float[] mFov = new float[4]; // L, R, B, T

    @NonNull
    public final SDKMatrix4x4 mEyeFromHeadMatrix = new SDKMatrix4x4();

    public final void getHeadMatrix(
        @NonNull final float[][] eyeFromHeadMatrix
    ) {
        mEyeFromHeadMatrix.toArray(
            eyeFromHeadMatrix
        );
    }

    public final void getProjectionMatrix(
        float zNear,
        float zFar,
        @NonNull final SDKMatrix4x4 outMatrix
    ) {
        SDKMatrix4x4.writePerspective(
            mFov,
            zNear,
            zFar,
            outMatrix
        );
    }

    public final void getFov(
        @NonNull final float[] out
    ) {
        System.arraycopy(
            mFov,
            0,
            out,
            0,
            mFov.length
        );
    }
}
