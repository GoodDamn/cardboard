package good.damn.sdk2;

import java.util.concurrent.ConcurrentLinkedDeque;

import androidx.annotation.NonNull;

public final class SDKFilterMean {

    @NonNull
    private final ConcurrentLinkedDeque<
        SDVector3
    > mDeque = new ConcurrentLinkedDeque<>();

    private final int mFilterSize;

    public SDKFilterMean(
        final int filterSize
    ) {
       mFilterSize = filterSize;
    }

    public final void addSample(
        @NonNull final SDVector3 sample
    ) {
        mDeque.add(
            sample
        );

        if (mDeque.size() > mFilterSize) {
            mDeque.removeFirst();
        }
    }

    @NonNull
    public final SDVector3 getFilteredData() {
        @NonNull final SDVector3 mean = SDVector3.zeros();

        for (final SDVector3 sample : mDeque) {
            mean.add(sample);
        }

        return mean.divide((float)mFilterSize);
    }

}
