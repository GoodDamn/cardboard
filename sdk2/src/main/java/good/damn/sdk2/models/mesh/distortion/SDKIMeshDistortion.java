package good.damn.sdk2.models.mesh.distortion;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import good.damn.sdk2.SDKIParamsDevice;
import good.damn.sdk2.models.SDKMParamsViewport;

public abstract class SDKIMeshDistortion {

    SDKMeshDistortionMatrix matrix;
    @Nullable
    public SDKDistortion distortion;

    public abstract void calculateFov(
        @NonNull final float[] fov4
    );

    public abstract void calculateHeadMatrix(
        float interLensDistance
    );

    public abstract void calculateScreenParams(
        @NonNull final SDKIParamsDevice paramsDevice,
        final float screenWidthMeters,
        final float screenHeightMeters,
        final float yOffsetMeters,
        @NonNull final SDKMParamsViewport out
    );

    public abstract void calculateTextureParams(
        @NonNull final SDKMParamsViewport out
    );

}
