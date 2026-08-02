package good.damn.sdk2;

import good.damn.sdk2.enums.CBEnumVerticalAlignment;

public interface SDKIParamsDevice {
    public float getDistanceScreenToLens();
    public float getDistanceInterLens();
    public float getDistanceTrayToLens();
    public CBEnumVerticalAlignment getVerticalAlignment();
    public float getDistortionCoefficient(
        int index
    );
    public int getDistortionCoefficientsSize();
    public float getLeftEyeFovAngles(
        int index
    );
}
