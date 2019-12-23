package aiofwa.org.a3dsnooker.ui.achievement;

import android.content.Context;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.ViewModel;
import androidx.paging.LivePagedListBuilder;
import androidx.paging.PagedList;

import aiofwa.org.a3dsnooker.db.database.AppDatabase;
import aiofwa.org.a3dsnooker.db.entity.Achievement;


public class AchievementViewModel extends ViewModel {
    private LiveData achievements;

    public AchievementViewModel() {
    }

    public LiveData<PagedList<Achievement> > getAchievements(Context context) {
        achievements =
                new LivePagedListBuilder<Integer, Achievement>(AppDatabase.getInstance(context).achievementDao().loadAll(),
                        new PagedList.Config.Builder().setPageSize(10).build()
                ).build();
        return achievements;
    }
}
