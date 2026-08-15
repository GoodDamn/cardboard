package good.damn.sdk2;

import androidx.annotation.NonNull;

public final class SDKFilterLowPass {

    private static final double kSecondsFromNanoseconds = 1e-9;

    // Minimum time step between sensor updates. This corresponds to 1000 Hz.
    private static final double kMinTimestepS = 0.001f;

    // Maximum time step between sensor updates. This corresponds to 1 Hz.
    private static final double kMaxTimestepS = 1.00f;

    private final double mCutoffTimeConstant;
    private boolean mInitialized;

    @NonNull
    private final SDVector3 mFilteredData = new SDVector3();

    private long mCurrentTimestampNs;

    public SDKFilterLowPass(
        final double cutoffFrequencyHz
    ) {
        mCutoffTimeConstant = 1.0 / (2.0 * Math.PI * cutoffFrequencyHz);
        mInitialized = false;
        reset();
    }

    public final void addSample(
        @NonNull final SDVector3 sample,
        final long timestampNs
    ) {
        addWeightedSample(
            sample,
            timestampNs,
            1.0
        );
    }

    public final void addWeightedSample(
        @NonNull final SDVector3 sample,
        final long timestampNs,
        final double weight
    ) {
        if (!mInitialized) {
            // Initialize filter state
            mFilteredData.setX(
                sample.getX()
            );

            mFilteredData.setY(
                sample.getY()
            );

            mFilteredData.setY(
                sample.getY()
            );

            mCurrentTimestampNs = timestampNs;
            mInitialized = true;
            return;
        }

        if (timestampNs < mCurrentTimestampNs) {
            mCurrentTimestampNs = timestampNs;
            return;
        }

        final double delta_s = (timestampNs - mCurrentTimestampNs) * kSecondsFromNanoseconds;
        if (delta_s <= kMinTimestepS || delta_s > kMaxTimestepS) {
            mCurrentTimestampNs = timestampNs;
            return;
        }

        final double weighted_delta_secs = weight * delta_s;

        final double alpha = weighted_delta_secs / (
            mCutoffTimeConstant + weighted_delta_secs
        );

        mFilteredData.setX(
            (float) ((1.0 - alpha) * mFilteredData.getX() + alpha * sample.getX())
        );

        mFilteredData.setY(
            (float) ((1.0 - alpha) * mFilteredData.getY() + alpha * sample.getY())
        );

        mFilteredData.setZ(
            (float) ((1.0 - alpha) * mFilteredData.getZ() + alpha * sample.getZ())
        );

        mCurrentTimestampNs = timestampNs;
    }

    public final long getMostRecentTimestampNs() {
        return mCurrentTimestampNs;
    }

    @NonNull
    public final SDVector3 getFilteredData() {
        return mFilteredData.copy();
    }

    public final boolean isInitialized() {
        return mInitialized;
    }

    public final void reset() {
        mInitialized = false;
        mFilteredData.zero();
    }
}
