package aiofwa.org.a3dsnooker.db.dao;

import androidx.room.Dao;
import androidx.room.Delete;
import androidx.room.Insert;
import androidx.room.Query;
import androidx.room.Update;

import java.util.List;
import aiofwa.org.a3dsnooker.db.entity.Achievement;


@Dao
public interface AchievementDao {
    @Query("SELECT * FROM achievement")
    List<Achievement> getAll();

    @Query("SELECT * FROM achievement WHERE finished=0")
    List<Achievement> getUnfinishedAchievements();

    @Query("SELECT * FROM achievement WHERE finished=1")
    List<Achievement> getFinishedAchievements();

    @Insert
    void insert(Achievement... achievement);

    @Update
    void update(Achievement... achievement);

    @Delete
    void drop(Achievement ...achievements);

    @Query("DELETE FROM achievement")
    void deleteEverything();
}
