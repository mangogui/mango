package mangoproject.mango.android;

import android.os.Bundle;
import android.app.Activity;


public class MainActivity extends Activity {

    static {
        System.loadLibrary("Mango");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        nativeMain();
        cppMain();
    }

    private native void nativeMain();
    private native void cppMain();
}