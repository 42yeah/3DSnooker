package aiofwa.org.a3dsnooker.db.entity;

import androidx.room.Entity;
import androidx.room.Ignore;
import androidx.room.PrimaryKey;


@Entity
public class Record {
    @Ignore
    Record() {}

    /**
     * Record is set when a game is either win or lose.
     * @param score is time in seconds, divided by 3 minutes.
     * @param controller is who ever wins. 0 for human, 1 for NPC. If the game is a draw, there should be no record.
     */
    public Record(float score, int controller) {
        this.score = score;
        this.controller = controller;
    }

    public float getScore() {
        return score;
    }

    public void setScore(float score) {
        this.score = score;
    }

    public int getController() {
        return controller;
    }

    public void setController(int controller) {
        this.controller = controller;
    }

    public int getRecordId() {
        return recordId;
    }

    public void setRecordId(int recordId) {
        this.recordId = recordId;
    }

    @PrimaryKey(autoGenerate = true)
    private int recordId;

    private float score;
    private int controller;
}
