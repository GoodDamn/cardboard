package good.damn.sdk2;

import androidx.annotation.NonNull;

public final class SDKPolynomialRadialDistortion {

    private static final float FLOAT_EPSILON = 1.1920929E-7f;

    @NonNull
    private final float[] mCoefficients;

    public SDKPolynomialRadialDistortion(
        @NonNull final float[] coefficients
    ) {
        mCoefficients = coefficients;
    }

    @NonNull
    public final void calculateDistortion(
        @NonNull final float[] p2,
        @NonNull final float[] outResult
    ) {
        final float distortionFactor = calculateDistortionFactor(
            p2[0] * p2[0] + p2[1] * p2[1]
        );

        outResult[0] = distortionFactor * p2[0];
        outResult[1] = distortionFactor * p2[1];
    }

    @NonNull
    public final void calculateDistortionInversed(
        @NonNull final float[] p2,
        @NonNull final float[] outResult
    ) {
        final float radius = (float) Math.sqrt(
            p2[0] * p2[0] + p2[1] * p2[1]
        );

        if (Math.abs(radius - 0.0f) < FLOAT_EPSILON) {
            outResult[0] = 0.0f;
            outResult[1] = 0.0f;
            return;
        }

        float r0 = radius / 2.0f;
        float r1 = radius / 3.0f;
        float r2;
        float dr0 = radius - calculateDistortionRadius(r0);
        float dr1;
        while (Math.abs(r1 - r0) > 0.0001f /** 0.1mm */) {
            dr1 = radius - calculateDistortionRadius(r1);
            r2 = r1 - dr1 * ((r1 - r0) / (dr1 - dr0));
            r0 = r1;
            r1 = r2;
            dr0 = dr1;
        }

        outResult[0] = (r1 / radius) * p2[0];
        outResult[1] = (r1 / radius) * p2[1];
    }

    private float calculateDistortionFactor(
        final float rSquared
    ) {
        float rFactor = 1.0f;
        float distortionFactor = 1.0f;
        for (float ki : mCoefficients) {
            rFactor *= rSquared;
            distortionFactor += ki * rFactor;
        }

        return distortionFactor;
    }

    private float calculateDistortionRadius(
        final float radius
    ) {
        return radius * calculateDistortionFactor(
            radius * radius
        );
    }

}
