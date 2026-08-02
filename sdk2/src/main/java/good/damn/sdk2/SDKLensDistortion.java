package good.damn.sdk2;

import androidx.annotation.NonNull;
import good.damn.sdk2.models.mesh.SDKMMesh;

public final class SDKLensDistortion {

    private static final float DEFAULT_BORDER_SIZE_METERS = 0.003f;

    @NonNull
    private final SDKPolynomialRadialDistortion mDistortion;

    @NonNull
    private final SDKIParamsDevice mParamsDevice;

    private final float mScreenWidthMeters;
    private final float mScreenHeightMeters;

    public SDKLensDistortion(
        @NonNull final SDKIParamsDevice paramsDevice,
        final float screenWidthMeters,
        final float screenHeightMeters,
        @NonNull final SDKMMesh[] meshes
    ) {
        mParamsDevice = paramsDevice;
        mScreenWidthMeters = screenWidthMeters;
        mScreenHeightMeters = screenHeightMeters;

        for (
            @NonNull final SDKMMesh mesh: meshes
        ) {
            mesh.distortion.calculateHeadMatrix(
                    paramsDevice.getDistanceInterLens()
            );
        }

        final float[] distortionCoefficients = new float[
            paramsDevice.getDistortionCoefficientsSize()
        ];

        for (int i = 0; i < distortionCoefficients.length; i++) {
            distortionCoefficients[i] = paramsDevice.getDistortionCoefficient(i);
        }

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
