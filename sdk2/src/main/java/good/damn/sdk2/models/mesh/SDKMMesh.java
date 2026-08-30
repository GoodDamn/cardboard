package good.damn.sdk2.models.mesh;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import good.damn.sdk2.models.mesh.distortion.SDKIMeshDistortion;
import good.damn.sdk2.models.mesh.eye.SDKMMeshEye;

public final class SDKMMesh {

    @NonNull
    public final SDKIMeshDistortion meshDistortion;

    @NonNull
    public final SDKMMeshEye meshRender;

    public SDKMMesh(
        @NonNull final SDKIMeshDistortion meshDistortion,
        @NonNull final SDKMMeshEye meshRender
    ) {
        this.meshDistortion = meshDistortion;
        this.meshRender = meshRender;
    }
}
