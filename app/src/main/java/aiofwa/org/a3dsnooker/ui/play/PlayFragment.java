package aiofwa.org.a3dsnooker.ui.play;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.graphics.Color;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProviders;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.util.List;
import java.util.PriorityQueue;
import java.util.Queue;

import aiofwa.org.a3dsnooker.NotificationBlabber;
import aiofwa.org.a3dsnooker.R;
import aiofwa.org.a3dsnooker.GlobalViewModel;
import aiofwa.org.a3dsnooker.SlideyActivity;
import aiofwa.org.a3dsnooker.db.dao.AchievementDao;
import aiofwa.org.a3dsnooker.db.database.AppDatabase;
import aiofwa.org.a3dsnooker.db.entity.Achievement;
import aiofwa.org.a3dsnooker.db.entity.Record;
import aiofwa.org.a3dsnooker.game.Engine;


public class PlayFragment extends Fragment {

    private GlobalViewModel globalViewModel;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        assert getActivity() != null;
        assert getContext() != null;

        activity = (SlideyActivity) getActivity();
        
        globalViewModel = ViewModelProviders.of(getActivity()).get(GlobalViewModel.class);
        LinearLayout root = (LinearLayout) inflater.inflate(R.layout.fragment_play, container, false);

        LinearLayout.LayoutParams matchParent = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT,
                1.0f
        );
        ViewGroup.LayoutParams wrapContent = new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT
        );
        engineWrapper = new LinearLayout(getContext());
        engineWrapper.setOrientation(LinearLayout.VERTICAL);
        engineWrapper.setLayoutParams(matchParent);

        gameInfo = new TextView(getContext());
        gameInfo.setLayoutParams(wrapContent);
        gameInfo.setBackgroundColor(Color.rgb(0, 0, 0));
        gameInfo.setTextColor(Color.WHITE);
        gameInfo.setText(activity.getString(R.string.test));
        gameInfo.setGravity(Gravity.CENTER);
        engineWrapper.addView(gameInfo);

        gameEngine = new Engine(activity);
        gameEngine.setLayoutParams(matchParent);
        engineWrapper.addView(gameEngine);

        achievements = null;
        running = true;
        anyoneMoved = false;

        new Thread() {
            @Override
            public void run() {
                syncAchievements();
                while (running) {
                    try {
                        sleep(100);
                        String display = "";
                        switch (gameEngine.getSnookerWinner()) {
                            case 0:
                                switch (gameEngine.getSnookerControllingController()) {
                                    case -1:
                                        break;

                                    case 0:
                                        if (!anyoneMoved) {
                                            startTime = System.currentTimeMillis();
                                        }
                                        anyoneMoved = true;
                                        display = activity.getString(R.string.your_turn);
                                        break;

                                    case 1:
                                        anyoneMoved = true;
                                        display = activity.getString(R.string.npc_turn);
                                        break;
                                }
                                break;

                            case 1:
                                if (!anyoneMoved) {
                                    break;
                                }
                                if (startTime != 0) {
                                    lapse = 180.0f - (System.currentTimeMillis() - startTime) / 1000.0f;
                                    AppDatabase.getInstance(activity).getRecordDao().insertRecord(new Record(lapse, 0));
                                    startTime = 0;
                                }
                                display = activity.getString(R.string.you_win);
                                checkAchievement(activity.getString(R.string.you_win));
                                break;

                            case 2:
                                if (!anyoneMoved) {
                                    break;
                                }
                                if (startTime != 0) {
                                    lapse = 180.0f - (System.currentTimeMillis() - startTime) / 1000.0f;
                                    AppDatabase.getInstance(activity).getRecordDao().insertRecord(new Record(lapse, 1));
                                    startTime = 0;
                                }
                                display = activity.getString(R.string.npc_win);
                                checkAchievement(activity.getString(R.string.achievement_lose));
                                break;
                        }
                        final String finalizedDisplay = display;
                        activity.runOnUiThread(new Runnable() {
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

        // === NOTIFICATION SETUP === //
        Intent notificationBlabberIntent = new Intent(getContext(), NotificationBlabber.class);
        blabber = new ServiceConnection() {
            @Override
            public void onServiceConnected(ComponentName name, IBinder service) {
                Log.i("Snooker3D", "Blabber is ready!");
                NotificationBlabber.BlabberBinder blabberBinder = (NotificationBlabber.BlabberBinder) service;
                PlayFragment.this.blabberInstance = blabberBinder.getService();
                while (!blabQueue.isEmpty()) {
                    blabberInstance.blab(blabQueue.remove());
                }
            }

            @Override
            public void onServiceDisconnected(ComponentName name) {

            }
        };
        activity.bindService(notificationBlabberIntent, blabber, Context.BIND_AUTO_CREATE);
        blabQueue = new PriorityQueue<>();

        root.addView(engineWrapper);

        activity.setOnBackPressedListener(new Runnable() {
            @Override
            public void run() {
                onBackPressed();
            }
        });

        return root;
    }

    @Override
    public void onDestroy() {
        running = false;
        if (blabber != null) {
            activity.unbindService(blabber);
        }
        super.onDestroy();
    }

    // === METHODS === //
    public void onBackPressed() {
        checkAchievement(activity.getString(R.string.achievement_usable_back));
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
        AchievementDao dao = AppDatabase.getInstance(getContext()).achievementDao();
        List<Achievement> achievements = dao.getAll();
        if (achievements.size() == 4) {
            this.achievements = achievements;
            return;
        }
        if (achievements.size() != 0) {
            dao.deleteEverything();
        }
        dao.insert(new Achievement(activity.getString(R.string.achievement_win)),
                new Achievement(activity.getString(R.string.achievement_lose)),
                new Achievement(activity.getString(R.string.achievement_usable_back)),
                new Achievement(activity.getString(R.string.achievement_draw)));
        this.achievements = dao.getAll();
    }

    private void updateAchievements() {
        new Thread() {
            @Override
            public void run() {
                AchievementDao dao = AppDatabase.getInstance(getContext()).achievementDao();
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
    boolean running;
    SlideyActivity activity;

    List<Achievement> achievements;

    // === BLABBER SETUP === //
    NotificationBlabber blabberInstance;
    Queue<String> blabQueue;
    ServiceConnection blabber;

    // === HARD DEBUG === //
    boolean anyoneMoved;
    long startTime;
    float lapse;
}
