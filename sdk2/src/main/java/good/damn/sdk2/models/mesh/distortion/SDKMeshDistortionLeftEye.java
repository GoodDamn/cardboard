package good.damn.sdk2.models.mesh.distortion;

import androidx.annotation.NonNull;
import good.damn.sdk2.device.SDKIParamsDevice;
import good.damn.sdk2.matrix.SDKMatrix4x4;
import good.damn.sdk2.models.SDKMParamsViewport;

import static java.lang.Math.tan;

public final class SDKMeshDistortionLeftEye
extends SDKIMeshDistortion {


    @Override
    public void calculateFov(
            @NonNull float[] fov4
    ) {
        System.arraycopy(
                fov4,
                0,
                matrix.mFov,
                0,
                fov4.length
        );
    }

    @Override
    public void calculateHeadMatrix(
            float interLensDistance
    ) {
        SDKMatrix4x4.writeTranslation(
                interLensDistance * 0.5f,
                0.f,
                0.f,
                matrix.mEyeFromHeadMatrix
        );
    }

    @Override
    public void calculateScreenParams(
            @NonNull SDKIParamsDevice paramsDevice,
            float screenWidthMeters,
            float screenHeightMeters,
            float yOffsetMeters,
            @NonNull final SDKMParamsViewport screenParams
    ) {
        float interLensDistance = paramsDevice.getDistanceInterLens();
        float screenLensDistance = paramsDevice.getDistanceScreenToLens();
        screenParams.width = screenWidthMeters / screenLensDistance;
        screenParams.height = screenHeightMeters / screenLensDistance;
        screenParams.x_eye_offset = (
                (screenWidthMeters - interLensDistance) / 2
        ) / screenLensDistance;
        screenParams.y_eye_offset = yOffsetMeters / screenLensDistance;
    }

    @Override
    public void calculateTextureParams(
            @NonNull SDKMParamsViewport textureParams
    ) {
        textureParams.width = (float) (tan(matrix.mFov[0]) + tan(matrix.mFov[1]));
        textureParams.height = (float) (tan(matrix.mFov[2]) + tan(matrix.mFov[3]));
        textureParams.x_eye_offset = (float) tan(matrix.mFov[0]);
        textureParams.y_eye_offset = (float) tan(matrix.mFov[2]);
    }
}
