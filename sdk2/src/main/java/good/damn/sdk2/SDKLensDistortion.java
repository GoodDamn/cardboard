package good.damn.sdk2;

import androidx.annotation.NonNull;
import good.damn.sdk2.device.SDKIParamsDevice;
import good.damn.sdk2.models.SDKMParamsViewport;
import good.damn.sdk2.models.mesh.SDKMMesh;
import good.damn.sdk2.models.mesh.distortion.SDKDistortion;

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
            mesh.meshDistortion.calculateHeadMatrix(
                    paramsDevice.getDistanceInterLens()
            );
        }

        @NonNull
        final float[] distortionCoefficients = new float[
            paramsDevice.getDistortionCoefficientsSize()
        ];

        for (int i = 0; i < distortionCoefficients.length; i++) {
            distortionCoefficients[i] = paramsDevice.getDistortionCoefficient(i);
        }

        mDistortion = new SDKPolynomialRadialDistortion(
            distortionCoefficients
        );

        @NonNull
        final float[] calculatedFov = new float[4];
        calculateFov(calculatedFov);

        final float yOffsetMeters = getYEyeOffsetMeters();
        for (@NonNull final SDKMMesh mesh: meshes) {
            mesh.meshDistortion.calculateFov(
                calculatedFov
            );

            @NonNull
            final SDKMParamsViewport paramsScreen = new SDKMParamsViewport(),
                    paramsTexture = new SDKMParamsViewport();

            mesh.meshDistortion.calculateScreenParams(
                    mParamsDevice,
                    mScreenWidthMeters,
                    mScreenHeightMeters,
                    yOffsetMeters,
                    paramsScreen
            );

            mesh.meshDistortion.calculateTextureParams(
                    paramsTexture
            );

            mesh.meshDistortion.distortion = new SDKDistortion(
                    mDistortion,
                    paramsScreen,
                    paramsTexture
            );
        }
    }


    private float getYEyeOffsetMeters() {
        switch (mParamsDevice.getVerticalAlignment()) {
            case BOTTOM:
                return mParamsDevice.getDistanceTrayToLens() - DEFAULT_BORDER_SIZE_METERS;
            case TOP:
                return mScreenHeightMeters - mParamsDevice.getDistanceTrayToLens() - DEFAULT_BORDER_SIZE_METERS;
            case CENTER:
            default:
                return mScreenHeightMeters / 2.0f;
        }
    }

    private void calculateFov(
        @NonNull final float[] outResult
    ) {
        // FOV angles in device parameters are in degrees so they are converted
        // to radians for posterior use.

        final float[] deviceFov = new float[] {
                degreesToRadians(mParamsDevice.getLeftEyeFovAngles(0)),
                degreesToRadians(mParamsDevice.getLeftEyeFovAngles(3)),
                degreesToRadians(mParamsDevice.getLeftEyeFovAngles(1)),
                degreesToRadians(mParamsDevice.getLeftEyeFovAngles(2)),
        };

        final float interLensDistance = mParamsDevice.getDistanceInterLens();
        final float eye_to_screen_distance = mParamsDevice.getDistanceScreenToLens();
        final float outer_distance = (mScreenWidthMeters - interLensDistance) / 2.0f;
        final float inner_distance = interLensDistance / 2.0f;
        final float bottom_distance = getYEyeOffsetMeters();
        final float top_distance = mScreenHeightMeters - bottom_distance;

        final float[] out = new float[2];


        mDistortion.calculateDistortion(
                new float[]{outer_distance / eye_to_screen_distance, 0},
                out
        );
        final float outer_angle = (float) Math.atan(out[0]);

        mDistortion.calculateDistortion(
                new float[]{inner_distance / eye_to_screen_distance, 0},
                out
        );
        final float inner_angle = (float) Math.atan(out[0]);


        mDistortion.calculateDistortion(
                new float[]{0, bottom_distance / eye_to_screen_distance},
                out
        );
        final float bottom_angle = (float) Math.atan(out[1]);

        mDistortion.calculateDistortion(
                new float[]{0, top_distance / eye_to_screen_distance},
                out
        );
        final float top_angle = (float) Math.atan(out[1]);

        outResult[0] = Math.min(outer_angle, deviceFov[0]);
        outResult[1] = Math.min(inner_angle, deviceFov[1]);
        outResult[2] = Math.min(bottom_angle, deviceFov[2]);
        outResult[3] = Math.min(top_angle, deviceFov[3]);
    }


    private static final float degreesToRadians(
        float angle
    ) {
        return (float) Math.toRadians(angle);
    }
}
