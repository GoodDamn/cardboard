package good.damn.sdk2.device;

import androidx.annotation.NonNull;
import good.damn.sdk2.enums.CBEnumVerticalAlignment;
import good.damn.sdk2.models.SDKMParamsDevice;

public final class SDKParamsDeviceImpl
implements SDKIParamsDevice {

    @NonNull
    private final SDKMParamsDevice mParams;

    public SDKParamsDeviceImpl(
        @NonNull final SDKMParamsDevice params
    ) {
        mParams = params;
    }

    @Override
    public float getDistanceScreenToLens() {
        return mParams.getDistanceScreenToLens();
    }

    @Override
    public float getDistanceInterLens() {
        return mParams.getDistanceInterLens();
    }

    @Override
    public float getDistanceTrayToLens() {
        return mParams.getDistanceTrayToLens();
    }

    @Override
    public CBEnumVerticalAlignment getVerticalAlignment() {
        return CBEnumVerticalAlignment.BOTTOM;
    }

    @Override
    public float getDistortionCoefficient(int index) {
        return mParams.getDistortionCoeffs()[index];
    }

    @Override
    public int getDistortionCoefficientsSize() {
        return mParams.getDistortionCoeffs().length;
    }

    @Override
    public float getLeftEyeFovAngles(int index) {
        return mParams.getFov()[index];
    }
}
