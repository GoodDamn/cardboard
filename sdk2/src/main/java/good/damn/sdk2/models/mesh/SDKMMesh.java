package good.damn.sdk2.models.mesh;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import good.damn.sdk2.models.mesh.distortion.SDKIMeshDistortion;
import good.damn.sdk2.models.mesh.eye.SDKMMeshEye;

public final class SDKMMesh {

    @Nullable
    public SDKIMeshDistortion distortion;

    @NonNull
    public SDKMMeshEye meshRender = new SDKMMeshEye();
}
