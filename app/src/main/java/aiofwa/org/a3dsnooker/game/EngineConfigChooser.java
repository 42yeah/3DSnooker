package aiofwa.org.a3dsnooker.game;

import android.opengl.GLSurfaceView;
import android.util.Log;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGL11;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;


public class EngineConfigChooser implements GLSurfaceView.EGLConfigChooser {
    // === CONSTANT === //
    final static int EGL_COVERAGE_SAMPLES_NV = 0x30E1;
    final static int EGL_COVERAGE_BUFFERS_NV = 0x30E0;

    // === METHODS === //
    @Override
    public EGLConfig chooseConfig(EGL10 egl10, EGLDisplay eglDisplay) {
        int[] attribs = new int[] {
                EGL10.EGL_COLOR_BUFFER_TYPE, EGL11.EGL_RGB_BUFFER,
                EGL10.EGL_RED_SIZE, 8,
                EGL10.EGL_GREEN_SIZE, 8,
                EGL10.EGL_BLUE_SIZE, 8,
                EGL10.EGL_DEPTH_SIZE, 16,
//                EGL10.EGL_SAMPLE_BUFFERS, 1,
//                EGL10.EGL_SAMPLES, 4, // 4x MXAA
                EGL10.EGL_NONE
        };
        EGLConfig[] configs = new EGLConfig[1];
        int[] configCounts = new int[1];
        egl10.eglChooseConfig(eglDisplay, attribs, configs, 1, configCounts);
        if (configCounts[0] == 0) {
            Log.e("Error!", "eGLChooseConfig was disastrous.");
            return null;
        }
        return configs[0];
    }
}
