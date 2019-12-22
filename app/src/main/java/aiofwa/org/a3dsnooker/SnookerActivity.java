package aiofwa.org.a3dsnooker;

import android.graphics.Color;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.util.concurrent.Executor;

import aiofwa.org.a3dsnooker.game.Engine;


public class SnookerActivity extends AppCompatActivity {
    // === METHODS === //
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        LinearLayout.LayoutParams matchParent = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT,
                1.0f
        );
        ViewGroup.LayoutParams wrapContent = new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT
        );
        engineWrapper = new LinearLayout(this);
        engineWrapper.setOrientation(LinearLayout.VERTICAL);
        engineWrapper.setLayoutParams(matchParent);
        gameEngine = new Engine(this);
        gameEngine.setLayoutParams(matchParent);
        engineWrapper.addView(gameEngine);

        gameInfo = new TextView(this);
        gameInfo.setLayoutParams(wrapContent);
        gameInfo.setBackgroundColor(Color.rgb(0, 0, 0));
        gameInfo.setTextColor(Color.WHITE);
        gameInfo.setText(getString(R.string.test));
        gameInfo.setGravity(Gravity.CENTER);
        engineWrapper.addView(gameInfo);

        new Thread() {
            @Override
            public void run() {
                while (true) {
                    try {
                        sleep(100);
                        String display = "";
                        switch (gameEngine.getSnookerWinner()) {
                            case 0:
                                switch (gameEngine.getSnookerControllingController()) {
                                    case -1:
                                        break;

                                    case 0:
                                        display = getString(R.string.your_turn);
                                        break;

                                    case 1:
                                        display = getString(R.string.npc_turn);
                                        break;
                                }
                                break;

                            case 1:
                                display = getString(R.string.you_win);
                                break;

                            case 2:
                                display = getString(R.string.npc_win);
                                break;
                        }
                        final String finalizedDisplay = display;
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                gameInfo.setText(finalizedDisplay);
                            }
                        });
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }.start();

        setContentView(engineWrapper);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        return super.onGenericMotionEvent(event);
    }

    @Override
    public void onBackPressed() {
        gameEngine.backToMainMenu();
    }

    // === VARIABLES === //
    Engine gameEngine;
    LinearLayout engineWrapper;
    TextView gameInfo;
}
