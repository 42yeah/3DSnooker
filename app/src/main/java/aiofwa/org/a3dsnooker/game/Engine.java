package aiofwa.org.a3dsnooker.game;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;


public class Engine extends GLSurfaceView {
    // === CONSTRUCTORS === //
    public Engine(Context context) {
        super(context);
        this.context = context;
        this.engineRenderer = new EngineRenderer(this, context);
        this.setEGLContextClientVersion(2);
        this.setEGLConfigChooser(new EngineConfigChooser());
        setRenderer(this.engineRenderer);
//        setRenderMode(RENDERMODE_WHEN_DIRTY);
        this.setOnTouchListener(new OnTouchListener());
    }

    // === METHODS === //
    public void onMotionEvent(MotionEvent event) {
        float w = getWidth(), h = getHeight();
        motionEvent(event.getAction(), event.getX() / w, event.getY() / h);
    }

    @Override
    public void onPause() {
        shutdown();
        super.onPause();
    }

    // === NATIVE === //
    static {
        System.loadLibrary("native-lib");
    }

    public native void motionEvent(int action, float x, float y);
    public native int getSnookerWinner();
    public native int getSnookerControllingController();
    public native void backToMainMenu();
    public native void shutdown();

    // === VARIABLES === //
    Context context;
    EngineRenderer engineRenderer;

    // === HELPER CLASSES === //
    class OnTouchListener implements View.OnTouchListener {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            performClick();
            onMotionEvent(event);
            return true;
        }
    }
}
