package mangoproject.mango.android;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.content.pm.ApplicationInfo;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends Activity {
    protected static final String TAG = "MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        loadNativeLibraries();
        nativeMain();
        cppMain();
    }

    private native void nativeMain();
    private native void cppMain();

    private void loadNativeLibraries() {
        String nativeLibraryPath = getApplicationInfo().nativeLibraryDir;
        File nativeLibraryDir = new File(nativeLibraryPath);

        if (nativeLibraryDir.exists() && nativeLibraryDir.isDirectory()) {
            String[] libFileNames = nativeLibraryDir.list();

            if (libFileNames != null) {
                for (String libFileName : libFileNames) {
                    // TODO: Filter libraries
                    if (libFileName.endsWith(".so") && !libFileName.startsWith("libWidgets")) {
                        String libBaseName = libFileName.substring(3, libFileName.length() - 3);
                        Log.d(TAG, "Loading library: " + libBaseName);
                        try {
                            System.loadLibrary(libBaseName);
                        } catch (UnsatisfiedLinkError e) {
                            Log.e(TAG, "Failed to load library: " + libBaseName, e);
                        }
                    }
                }
            } else {
                Log.d(TAG, "No libraries found in the specified directory");
            }
        } else {
            Log.d(TAG, "Specified directory does not exist or is not a directory");
        }
    }
}