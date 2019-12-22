package aiofwa.org.a3dsnooker.db.database;

import android.content.Context;

import androidx.room.Database;
import androidx.room.Room;
import androidx.room.RoomDatabase;

import aiofwa.org.a3dsnooker.db.dao.AchievementDao;
import aiofwa.org.a3dsnooker.db.entity.Achievement;


@Database(entities = { Achievement.class }, version = 1, exportSchema = false)
public abstract class AppDatabase extends RoomDatabase {
    public AppDatabase() {
        database = null;
    }

    public abstract AchievementDao achievementDao();

    public static AppDatabase getInstance(Context context) {
        if (database == null) {
            database = Room.databaseBuilder(context, AppDatabase.class, "achievements").build();
        }
        return database;
    }

    private static AppDatabase database;
}
