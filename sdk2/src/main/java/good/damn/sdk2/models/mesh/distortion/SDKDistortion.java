package good.damn.sdk2.models.mesh.distortion;

import androidx.annotation.NonNull;
import good.damn.sdk2.SDKPolynomialRadialDistortion;
import good.damn.sdk2.matrix.SDKMatrix4x4;
import good.damn.sdk2.models.SDKMParamsViewport;

public final class SDKDistortion {

    private static final int RESOLUTION = 40;

    @NonNull
    private final int[] mIndices;

    @NonNull
    private final float[] mVertices;

    @NonNull
    private final float[] mUvs;

    @NonNull
    public final SDKMatrix4x4 mEyeFromHeadMatrix = new SDKMatrix4x4();

    @NonNull
    public final float[] mFov = new float[4];

    public SDKDistortion(
            @NonNull final SDKPolynomialRadialDistortion distortion,
            @NonNull final SDKMParamsViewport paramsScreen,
            @NonNull final SDKMParamsViewport paramsTexture
    ) {
        final float screen_width = paramsScreen.width;
        final float screen_height = paramsScreen.height;
        final float x_eye_offset_screen = paramsScreen.x_eye_offset;
        final float y_eye_offset_screen = paramsScreen.y_eye_offset;
        final float texture_width = paramsTexture.width;
        final float texture_height = paramsTexture.height;
        final float x_eye_offset_texture = paramsTexture.x_eye_offset;
        final float y_eye_offset_texture = paramsTexture.y_eye_offset;

        final int nComponents = RESOLUTION * RESOLUTION * 2; // 2 components per vertex
        mVertices = new float[nComponents];
        mUvs = new float[nComponents];// 2 components per uv

        float u_screen, v_screen, u_texture, v_texture;
        float[] p_texture = new float[2];
        float[] p_screen = new float[2];
        for (int row = 0; row < RESOLUTION; row++) {
            for (int col = 0; col < RESOLUTION; col++) {
                // Note that we warp the mesh vertices using the inverse of
                // the distortion function instead of warping the texture
                // coordinates by the distortion function so that the mesh
                // exactly covers the screen area that gets rendered to.
                // Helps avoid visible aliasing in the vignette.
                u_texture = ((float) (col) / (RESOLUTION - 1));
                v_texture = ((float) (row) / (RESOLUTION - 1));

                // texture position & radius relative to eye center in meters - I believe
                // this is tanangle
                p_texture[0] = u_texture * texture_width - x_eye_offset_texture;
                p_texture[1] = v_texture * texture_height - y_eye_offset_texture;

                distortion.calculateDistortionInversed(
                    p_texture,
                    p_screen
                );

                u_screen = (p_screen[0] + x_eye_offset_screen) / screen_width;
                v_screen = (p_screen[1] + y_eye_offset_screen) / screen_height;

                int index = (row * RESOLUTION + col) * 2;

                mVertices[index + 0] = 2 * u_screen - 1;
                mVertices[index + 1] = 2 * v_screen - 1;
                mUvs[index + 0] = u_texture;
                mUvs[index + 1] = v_texture;
            }
        }

        // Strip method described at:
        // http://dan.lecocq.us/wordpress/2009/12/25/triangle-strip-for-grids-a-construction/
        //
        // For a grid with 4 rows and 4 columns of vertices, the strip would
        // look like:
        //
        //     0  -  1  -  2  -  3
        //     ↓  ↗  ↓  ↗  ↓  ↗  ↓
        //     4  -  5  -  6  -  7 ↺
        //     ↓  ↖  ↓  ↖  ↓  ↖  ↓
        //   ↻ 8  -  9  - 10  - 11
        //     ↓  ↗  ↓  ↗  ↓  ↗  ↓
        //    12  - 13  - 14  - 15
        //
        // Note the little circular arrows next to 7 and 8 that indicate
        // repeating that vertex once so as to produce degenerate triangles.

        // Number of indices:
        //   1 vertex per triangle
        //   2 triangles per quad
        //   (rows - 1) * (cols - 1) quads
        //   2 vertices at the start of each row for the first triangle
        //   1 extra vertex per row (except first and last) for a
        //     degenerate triangle
        final int n_indices = 2 * (RESOLUTION - 1) * RESOLUTION + (RESOLUTION - 2);
        mIndices = new int[n_indices];
        int index_offset = 0;
        int vertex_offset = 0;
        for (int row = 0; row < RESOLUTION - 1; row++) {
            if (row > 0) {
                mIndices[index_offset] = mIndices[index_offset - 1];
                index_offset++;
            }
            for (int col = 0; col < RESOLUTION; col++) {
                if (col > 0) {
                    if (row % 2 == 0) {
                        // Move right on even rows.
                        vertex_offset++;
                    } else {
                        // Move left on odd rows.
                        vertex_offset--;
                    }
                }
                mIndices[index_offset++] = vertex_offset;
                mIndices[index_offset++] = vertex_offset + RESOLUTION;
            }
            vertex_offset = vertex_offset + RESOLUTION;
        }
    }

    @NonNull
    public float[] getVertices() {
        return mVertices;
    }

    @NonNull
    public float[] getUvs() {
        return mUvs;
    }

    @NonNull
    public int[] getIndices() {
        return mIndices;
    }

}
