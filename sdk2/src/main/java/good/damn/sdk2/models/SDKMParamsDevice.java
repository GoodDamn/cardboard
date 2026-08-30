package good.damn.sdk2.models;

import androidx.annotation.NonNull;

public final class SDKMParamsDevice {

    private final float mDistanceInterLens;
    private final float mDistanceTrayToLens;
    private final float mDistanceScreenToLens;

    @NonNull
    private final float[] mFov;

    @NonNull
    private final float[] mDistortionCoeffs;

    public SDKMParamsDevice(
            final float distanceInterLens,
            final float distanceTrayToLens,
            final float distanceScreenToLens,
            @NonNull final float[] fov,
            @NonNull final float[] distortionCoeffs
    ) {
        mDistanceInterLens = distanceInterLens;
        mDistanceTrayToLens = distanceTrayToLens;
        mDistanceScreenToLens = distanceScreenToLens;
        mFov = fov;
        mDistortionCoeffs = distortionCoeffs;
    }

    public float getDistanceTrayToLens() {
        return mDistanceTrayToLens;
    }

    public float getDistanceScreenToLens() {
        return mDistanceScreenToLens;
    }

    @NonNull
    public float[] getFov() {
        return mFov;
    }

    @NonNull
    public float[] getDistortionCoeffs() {
        return mDistortionCoeffs;
    }

    public float getDistanceInterLens() {
        return mDistanceInterLens;
    }
}
