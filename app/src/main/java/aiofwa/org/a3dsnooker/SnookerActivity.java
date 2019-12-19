package aiofwa.org.a3dsnooker;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;

import aiofwa.org.a3dsnooker.game.Engine;


public class SnookerActivity extends AppCompatActivity {
    // === METHODS === //
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        gameEngine = new Engine(this);
        setContentView(gameEngine);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        return super.onGenericMotionEvent(event);
    }

    // === VARIABLES === //
    Engine gameEngine;
}
