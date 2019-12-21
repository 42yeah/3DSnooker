package aiofwa.org.a3dsnooker.game;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


public class EngineRenderer implements GLSurfaceView.Renderer {
    // === CONSTRUCTORS === //
    public EngineRenderer(Engine engine, Context context) {
        this.engine = engine;
        this.context = context;
    }

    // === METHODS === //
    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        setAssetManager(context.getAssets());
        surfaceCreated(engine.getWidth(), engine.getHeight());
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int i, int i1) {
        surfaceChanged(i, i1);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        render();
    }

    // === NATIVE === //
    static {
        System.loadLibrary("native-lib");
    }

    public native void setAssetManager(AssetManager manager);
    public native void surfaceCreated(int initialW, int initialH);
    public native void surfaceChanged(int w, int h);
    public native void render();

    // === VARIABLES === //
    Context context;
    Engine engine;
}
