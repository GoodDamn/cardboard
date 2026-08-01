package good.damn.sdk2.models.mesh.eye;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

public final class SDKMMeshEye {

    @Nullable
    public int[] indices;

    /// Vertices buffer. 2 floats per vertex: x, y.
    @Nullable
    public float[] vertices;

    /// UV coordinates buffer. 2 floats per uv: u, v.
    @Nullable
    public float[] uvs;

    public byte id;

    @NonNull
    public final float[] projectionMatrix = new float[16];
    @NonNull
    public final float[] eyeMatrix = new float[16];

    @NonNull
    public final SDKMMeshEyeTextureDescription textureDescription = new SDKMMeshEyeTextureDescription();
}
