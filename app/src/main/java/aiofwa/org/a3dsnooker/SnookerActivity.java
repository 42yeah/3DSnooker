package aiofwa.org.a3dsnooker;

import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.graphics.Color;
import android.os.IBinder;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.util.List;
import java.util.PriorityQueue;
import java.util.Queue;

import aiofwa.org.a3dsnooker.db.dao.AchievementDao;
import aiofwa.org.a3dsnooker.db.database.AppDatabase;
import aiofwa.org.a3dsnooker.db.entity.Achievement;
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
                syncAchievements();
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
                                checkAchievement(getString(R.string.you_win));
                                break;

                            case 2:
                                display = getString(R.string.npc_win);
                                checkAchievement(getString(R.string.achievement_lose));
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

        achievements = null;


        // Set up notification
        Intent notificationBlabberIntent = new Intent(this, NotificationBlabber.class);
        ServiceConnection blabber = new ServiceConnection() {
            @Override
            public void onServiceConnected(ComponentName name, IBinder service) {
                Log.i("Snooker3D", "Blabber is ready!");
                NotificationBlabber.BlabberBinder blabberBinder = (NotificationBlabber.BlabberBinder) service;
                SnookerActivity.this.blabberInstance = blabberBinder.getService();
                while (!blabQueue.isEmpty()) {
                    blabberInstance.blab(blabQueue.remove());
                }
            }

            @Override
            public void onServiceDisconnected(ComponentName name) {

            }
        };
        bindService(notificationBlabberIntent, blabber, BIND_AUTO_CREATE);
        blabQueue = new PriorityQueue<>();

        setContentView(engineWrapper);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        return super.onGenericMotionEvent(event);
    }

    @Override
    public void onBackPressed() {
        checkAchievement(getString(R.string.achievement_usable_back));
        gameEngine.backToMainMenu();
    }

    private void notifyAchievement(Achievement achievement) {
        blab("成就解锁：" + achievement.getAchievement() + "！");
    }

    private void checkAchievement(String keyword) {
        Achievement achievement = getAchievement(keyword);
        if (achievement == null) {
            Log.e("Snooker3D", "No such achievement: " + keyword);
            return;
        }
        if (!achievement.isFinished()) {
            notifyAchievement(achievement);
            achievement.setFinished(true);
            updateAchievements();
        } else {}
    }

    /**
     * syncAchievements is used to bootstrap achievement.
     */
    private void syncAchievements() {
        AchievementDao dao = AppDatabase.getInstance(this).achievementDao();
        List<Achievement> achievements = dao.getAll();
        if (achievements.size() == 4) {
            this.achievements = achievements;
            return;
        }
        if (achievements.size() != 0) {
            dao.deleteEverything();
        }
        dao.insert(new Achievement(getString(R.string.achievement_win)),
                new Achievement(getString(R.string.achievement_lose)),
                new Achievement(getString(R.string.achievement_usable_back)),
                new Achievement(getString(R.string.achievement_draw)));
        this.achievements = dao.getAll();
    }

    private void updateAchievements() {
        new Thread() {
            @Override
            public void run() {
                AchievementDao dao = AppDatabase.getInstance(SnookerActivity.this).achievementDao();
                for (Achievement achievement : achievements) {
                    dao.update(achievement);
                }
            }
        }.start();
    }

    Achievement getAchievement(String keyword) {
        for (Achievement achievement : achievements) {
            if (achievement.getAchievement().equals(keyword)) {
                return achievement;
            }
        }
        return null;
    }

    // === BLABBER === //
    public void blab(String content) {
        blabQueue.add(content);
        if (blabberInstance != null) {
            while (!blabQueue.isEmpty()) {
                blabberInstance.blab(blabQueue.remove());
            }
        }
    }

    // === VARIABLES === //
    Engine gameEngine;
    LinearLayout engineWrapper;
    TextView gameInfo;

    List<Achievement> achievements;

    // === BLABBER SETUP === //
    NotificationBlabber blabberInstance;
    Queue<String> blabQueue;
}
