package com.google.cardboard.misc;

import android.content.Context;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.lang.reflect.Array;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

public final class VRProviderParams {

    private static final String TAG = "VRProviderParams";
    @NonNull
    private static final String DIR = "misc";

    @Nullable
    public static final VRMParams extractPublicParams(
        @NonNull final Context context
    ) throws Exception {

        @Nullable
        final File dir = context.getExternalFilesDir(
            DIR
        );

        if (dir == null) {
            return null;
        }

        @NonNull
        final File file = new File(
                dir,
                "params.txt"
        );

        if (!file.exists() && file.createNewFile()) {
            Log.d(TAG, "extractPublicParams: file" + file.getName() + " is created");
        }

        byte[] buffer = new byte[512];

        FileInputStream inp = new FileInputStream(
                file
        );

        int readBytes = inp.read(buffer);
        inp.close();

        @NonNull
        final String[] strParams = new String(
                buffer,
                0,
                readBytes,
                StandardCharsets.UTF_8
        ).split("\n");

        Log.d(TAG, "extractPublicParams: " + Arrays.toString(strParams));
        return new VRMParams(
            Float.parseFloat(strParams[0]),
            Float.parseFloat(strParams[1]),
            Float.parseFloat(strParams[2]),
            new float[] {
                Float.parseFloat(strParams[3]),
                Float.parseFloat(strParams[4]),
                Float.parseFloat(strParams[5]),
                Float.parseFloat(strParams[6])
            },
            new float[] {
                Float.parseFloat(strParams[7]),
                Float.parseFloat(strParams[8]),
            }
        );
    }

}
