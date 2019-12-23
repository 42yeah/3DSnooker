package aiofwa.org.a3dsnooker.db.dao;

import androidx.room.Dao;
import androidx.room.Insert;
import androidx.room.Query;

import java.util.List;

import aiofwa.org.a3dsnooker.db.entity.Record;


@Dao
public interface RecordDao {
    @Query("SELECT * FROM record ORDER BY score DESC")
    List<Record> getRecords();

    @Insert
    void insertRecord(Record ...records);
}
