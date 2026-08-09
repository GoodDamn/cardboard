package com.google.cardboard.utils;

import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.StringTokenizer;
import java.util.Vector;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

public final class VRUtilsLoaderObj {

    private static final String TAG = "VRUtilsLoaderObj";

    public static final boolean loadObjFile(
        @NonNull final AssetManager assets,
        @NonNull final String fileName,
        @NonNull final Vector<Float> outPosition,
        @NonNull final Vector<Float> outNormals,
        @NonNull final Vector<Float> outUv,
        @NonNull final Vector<Short> outIndices
    ) throws IOException {
        final Vector<Float> temp_positions = new Vector<>();
        final Vector<Float> temp_normals = new Vector<>();
        final Vector<Float> temp_uvs = new Vector<>();
        final Vector<Short> vertex_indices = new Vector<>();
        final Vector<Short> normal_indices = new Vector<>();
        final Vector<Short> uv_indices = new Vector<>();

        // If the file hasn't been uncompressed, load it to the internal storage.
        // Note that AAsset_openFileDescriptor doesn't support compressed
        // files (.obj).

        @NonNull final BufferedReader stream = new BufferedReader(
            new InputStreamReader(
                assets.open(
                    fileName,
                    AssetManager.ACCESS_STREAMING
                )
            )
        );

        @Nullable
        String readed = null;
        while (true) {
            readed = stream.readLine();

            if (readed == null) {
                break;
            }

            if (readed.charAt(0) == '\0') {
                continue;
            }

            if (readed.charAt(0) == 'v' && readed.charAt(1) == 'n') {
                // Parse vertex normal.
                @NonNull final String[] splitted = readed.split("\\s+");

                temp_normals.add(
                    Float.parseFloat(splitted[1])
                );

                temp_normals.add(
                    Float.parseFloat(splitted[2])
                );

                temp_normals.add(
                    Float.parseFloat(splitted[3])
                );
                continue;
            }

            if (readed.charAt(0) == 'v' && readed.charAt(1) == 't') {
                // Parse texture uv.
                @NonNull final String[] splitted = readed.split("\\s+");

                temp_uvs.add(
                    Float.parseFloat(splitted[1])
                );

                temp_uvs.add(
                    Float.parseFloat(splitted[2])
                );

                continue;
            }

            if (readed.charAt(0) == 'v') {
                // Parse vertex.
                @NonNull final String[] splitted = readed.split("\\s+");

                temp_positions.add(
                    Float.parseFloat(splitted[1])
                );

                temp_positions.add(
                    Float.parseFloat(splitted[2])
                );

                temp_positions.add(
                    Float.parseFloat(splitted[3])
                );
                continue;
            }

            if (readed.charAt(0) == 'f') {
                // Actual faces information starts from the second character.

                int[] vertex_index = new int[4];
                int[] normal_index = new int[4];
                int[] texture_index = new int[4];

                @NonNull
                final String[] per_vertex_info_list = readed.substring(2).split("\\s+");

                /*Vector<Character> per_vertex_info_list = new Vector<>();
                char *per_vertex_info_list_c_str;
                char *face_line_iter = face_line;
                while ((per_vertex_info_list_c_str = strtok_r(face_line_iter, " ", &face_line_iter))) {
                    // Divide each faces information into individual positions.
                    per_vertex_info_list.push_back(per_vertex_info_list_c_str);
                }*/

                boolean is_normal_available = false;
                boolean is_uv_available = false;
                for (int i = 0; i < per_vertex_info_list.length; ++i) {
                    int per_vertex_info_count = 0;

                    final boolean is_vertex_normal_only_face = per_vertex_info_list[i].contains("//");

                    @NonNull final StringTokenizer tokenizer = new StringTokenizer(
                        per_vertex_info_list[i],
                        "/"
                    );

                    while (tokenizer.hasMoreTokens()) {
                        @NonNull final String per_vertex_info = tokenizer.nextToken();

                        Log.d(TAG, "loadObjFile: PER_VERTEX_INFO: " + per_vertex_info + ";;; PER_VERTEX_INFO_LIST: " + Arrays.toString(per_vertex_info_list));
                        switch (per_vertex_info_count) {
                            case 0:
                                // Write to vertex indices.
                                vertex_index[i] = Integer.parseInt(per_vertex_info);  // NOLINT
                                break;
                            case 1:
                                // Write to texture indices.
                                if (is_vertex_normal_only_face) {
                                    normal_index[i] = Integer.parseInt(per_vertex_info);  // NOLINT
                                    is_normal_available = true;
                                } else {
                                    texture_index[i] = Integer.parseInt(per_vertex_info);  // NOLINT
                                    is_uv_available = true;
                                }
                                break;
                            case 2:
                                // Write to normal indices.
                                if (!is_vertex_normal_only_face) {
                                    normal_index[i] = Integer.parseInt(per_vertex_info);  // NOLINT
                                    is_normal_available = true;
                                    break;
                                }
                                //   [[clang::fallthrough]];
                                // Fallthrough to error case because if there's no texture coords,
                                // there should only be 2 indices per vertex (position and
                                // normal).
                            default:
                                // Error formatting.
                                LOGE(
                                    "Format of 'f int/int/int int/int/int int/int/int " +
                                    "(int/int/int)' " +
                                    "or 'f int//int int//int int//int (int//int)' required for " +
                                    "each face"
                                );
                                return false;
                        }
                        per_vertex_info_count++;
                    }
                }

                int vertices_count = per_vertex_info_list.length;
                for (int i = 2; i < vertices_count; ++i) {
                    vertex_indices.add((short) (vertex_index[0] - 1));
                    vertex_indices.add((short) (vertex_index[i - 1] - 1));
                    vertex_indices.add((short) (vertex_index[i] - 1));

                    if (is_normal_available) {
                        normal_indices.add((short) (normal_index[0] - 1));
                        normal_indices.add((short) (normal_index[i - 1] - 1));
                        normal_indices.add((short) (normal_index[i] - 1));
                    }

                    if (is_uv_available) {
                        uv_indices.add((short) (texture_index[0] - 1));
                        uv_indices.add((short) (texture_index[i - 1] - 1));
                        uv_indices.add((short) (texture_index[i] - 1));
                    }
                }
            }
        }

        final boolean is_normal_available = !normal_indices.isEmpty();
        final boolean is_uv_available = !uv_indices.isEmpty();

        if (is_normal_available && normal_indices.size() != vertex_indices.size()) {
            LOGE("Obj normal indices does not equal to vertex indices.");
            return false;
        }

        if (is_uv_available && uv_indices.size() != vertex_indices.size()) {
            LOGE("Obj UV indices does not equal to vertex indices.");
            return false;
        }

        for (int i = 0; i < vertex_indices.size(); i++) {
            int vertex_index = ((int)vertex_indices.get(i)) & 0xffff;
            outPosition.add(temp_positions.get(vertex_index * 3));
            outPosition.add(temp_positions.get(vertex_index * 3 + 1));
            outPosition.add(temp_positions.get(vertex_index * 3 + 2));
            outIndices.add((short) i);

            if (is_normal_available) {
                int normal_index = normal_indices.get(i);
                outNormals.add(temp_normals.get(normal_index * 3));
                outNormals.add(temp_normals.get(normal_index * 3 + 1));
                outNormals.add(temp_normals.get(normal_index * 3 + 2));
            }

            if (is_uv_available) {
                int uv_index = uv_indices.get(i);
                outUv.add(temp_uvs.get(uv_index * 2));
                outUv.add(temp_uvs.get(uv_index * 2 + 1));
            }
        }

        return true;
    }

    private static final void LOGE(
        @NonNull final String str
    ) {
        Log.d(TAG, "loadObjFile: " + str);
    }
}
