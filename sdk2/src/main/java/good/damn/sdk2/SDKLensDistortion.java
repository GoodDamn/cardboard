package good.damn.sdk2;

import androidx.annotation.NonNull;

public final class SDKLensDistortion {

    @NonNull
    private final SDKPolynomialRadialDistortion mDistortion;

    @NonNull
    private final SDKParamsDevice mParamsDevice;

    private final float mScreenWidthMeters;
    private final float mScreenHeightMeters;

    public SDKLensDistortion(
        @NonNull final SDKParamsDevice paramsDevice,
        final float screenWidthMeters,
        final float screenHeightMeters
    ) {
        mParamsDevice = paramsDevice;
        mScreenWidthMeters = screenWidthMeters;
        mScreenHeightMeters = screenHeightMeters;

        mDistortion = new SDKPolynomialRadialDistortion();
    }


    private float GetYEyeOffsetMeters() {

    }

    private void calculateFov(
        @NonNull final float[] outResult
    ) {

    }


    private static final float degreesToRadians(
            float angle
    ) {

    }
}
