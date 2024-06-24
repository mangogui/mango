package mangoproject.mango.android;

import android.content.Context;
import android.graphics.Color;
import android.view.MotionEvent;
import android.view.View;

public class WidgetView extends View {
    public WidgetView(Context context) {
        super(context);
        this.setBackgroundColor(Color.parseColor("#FF0000"));
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int action = event.getAction();
        float x = event.getX();
        float y = event.getY();

        return nativeOnTouchEvent(action, x, y);
    }

    // Native method declaration
    private native boolean nativeOnTouchEvent(int action, float x, float y);
}
