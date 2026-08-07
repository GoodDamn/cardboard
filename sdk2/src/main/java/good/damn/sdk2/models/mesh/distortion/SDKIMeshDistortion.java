package good.damn.sdk2.models.mesh.distortion;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import good.damn.sdk2.device.SDKIParamsDevice;
import good.damn.sdk2.models.SDKMParamsViewport;

public abstract class SDKIMeshDistortion {

    @NonNull
    public final SDKMeshDistortionMatrix matrix = new SDKMeshDistortionMatrix();
    @Nullable
    public SDKDistortion distortion = null;

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
