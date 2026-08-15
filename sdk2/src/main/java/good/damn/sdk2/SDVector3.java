package good.damn.sdk2;

import androidx.annotation.NonNull;

public final class SDVector3 {

    public static final SDVector3 zeros() {
        return new SDVector3();
    }

    private final double[] mData = new double[3];

    public SDVector3() {
        this(0.0f, 0.0f, 0.0f);
    }

    public SDVector3(
        double[] data
    ) {
        System.arraycopy(
            data,
            0,
            mData,
            0,
            data.length
        );
    }

    public SDVector3(
        float x,
        float y,
        float z
    ) {
        mData[0] = x;
        mData[1] = y;
        mData[2] = z;
    }

    public final void setX(
        final double x
    ) { mData[0] = x; }

    public final void setY(
        final double x
    ) { mData[1] = x; }

    public final void setZ(
        final double x
    ) { mData[2] = x; }

    public final double getX() {
        return mData[0];
    }

    public final double getY() {
        return mData[1];
    }

    public final double getZ() {
        return mData[2];
    }

    public final void zero() {
        mData[0] = 0f;
        mData[1] = 0f;
        mData[2] = 0f;
    }

    @NonNull
    public final SDVector3 add(
        @NonNull final SDVector3 v
    ) {
        for (int i = 0; i < mData.length; i++) {
            mData[i] += v.mData[i];
        }

        return this;
    }

    @NonNull
    public final SDVector3 divide(
        final double v
    ) {
        for (int i = 0; i < mData.length; i++) {
            mData[i] /= v;
        }

        return this;
    }


    @NonNull
    public final SDVector3 copy() {
        return new SDVector3(
            mData
        );
    }
}
