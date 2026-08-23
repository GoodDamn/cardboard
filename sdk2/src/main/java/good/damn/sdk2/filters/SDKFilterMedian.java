package good.damn.sdk2.filters;

import java.util.concurrent.ConcurrentLinkedDeque;

import androidx.annotation.NonNull;
import good.damn.sdk2.SDVector3;

public final class SDKFilterMedian {

    @NonNull
    private final ConcurrentLinkedDeque<
        SDVector3
    > mDeque = new ConcurrentLinkedDeque<>();

    @NonNull
    private final ConcurrentLinkedDeque<
        Double
    > mDequeNormals = new ConcurrentLinkedDeque<>();


    private final int mFilterSize;

    public SDKFilterMedian(
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

        mDequeNormals.add(
            sample.length()
        );

        if (mDeque.size() > mFilterSize) {
            mDeque.removeFirst();
            mDequeNormals.removeFirst();
        }
    }

    public final boolean isValid() {
        return mDeque.size() == mFilterSize;
    }

    /*@NonNull
    public final SDVector3 getFilteredData() {
        std::vector<float> norms(norms_.begin(), norms_.end());

        // Get median of value of the norms.
        std::nth_element(norms.begin(), norms.begin() + filter_size_ / 2,
            norms.end());
        const float median_norm = norms[filter_size_ / 2];

        // Get median value based on their norm.
        auto median_it = buffer_.begin();

        for (final Double norm: mDequeNormals) {
            if (norm == median_norm) {
                break;
            }

            ++median_it;
        }

        return *median_it;
    }*/

    public final void reset() {
        mDeque.clear();
        mDequeNormals.clear();
    }
}
