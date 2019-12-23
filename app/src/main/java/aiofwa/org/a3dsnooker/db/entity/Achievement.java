package aiofwa.org.a3dsnooker.db.entity;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.room.Entity;
import androidx.room.PrimaryKey;


@Entity
public class Achievement {
    public Achievement(String achievement) {
        this.achievement = achievement;
        this.finished = false;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getAchievement() {
        return achievement;
    }

    public void setAchievement(String achievement) {
        this.achievement = achievement;
    }

    public boolean isFinished() {
        return finished;
    }

    public void setFinished(boolean finished) {
        this.finished = finished;
    }

    @Override
    public boolean equals(@Nullable Object obj) {
        if (obj == null) { return false; }
        if (!(obj instanceof Achievement)) { return false; }
        return id == ((Achievement) obj).getId();
    }

    @PrimaryKey(autoGenerate = true)
    private int id;

    private String achievement;

    private boolean finished;
}
