package good.damn.sdk2;

public final class SDVector3 {

    private final float[] mData = new float[3];

    public final void setX(
        final float x
    ) { mData[0] = x; }

    public final void setY(
        final float x
    ) { mData[1] = x; }

    public final void setZ(
        final float x
    ) { mData[2] = x; }

    public final float getX() {
        return mData[0];
    }

    public final float getY() {
        return mData[1];
    }

    public final float getZ() {
        return mData[2];
    }



    public final void zero() {
        mData[0] = 0f;
        mData[1] = 0f;
        mData[2] = 0f;
    }
}
