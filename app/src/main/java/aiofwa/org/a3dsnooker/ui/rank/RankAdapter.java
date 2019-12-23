package aiofwa.org.a3dsnooker.ui.rank;

import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.List;

import aiofwa.org.a3dsnooker.R;
import aiofwa.org.a3dsnooker.db.entity.Record;


public class RankAdapter extends RecyclerView.Adapter {
    RankAdapter() {}
    RankAdapter(Activity activity, List<Record> records) {
        this.activity = activity;
        this.records = records;
    }

    @NonNull
    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        LinearLayout view = (LinearLayout) activity.getLayoutInflater().inflate(R.layout.rank_recycler_view_item, parent, false);
        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull RecyclerView.ViewHolder holder, int position) {
        ((ViewHolder) holder).setRecord(records.get(position));
    }

    @Override
    public int getItemCount() {
        return this.records.size();
    }

    // === VIEWHOLDER === //
    class ViewHolder extends RecyclerView.ViewHolder {
        public ViewHolder(@NonNull LinearLayout itemView) {
            super(itemView);
            this.layout = itemView;
        }

        void setRecord(Record record) {
            TextView winnerElem = layout.findViewById(R.id.winner);
            TextView scoreElem = layout.findViewById(R.id.score);
            assert winnerElem != null && scoreElem != null;
            switch (record.getController()) {
                case 0:
                    winnerElem.setText("玩家");
                    break;

                case 1:
                    winnerElem.setText("机器人");
                    winnerElem.setTextColor(Color.rgb(123, 0, 0));
                    break;
            }
            String concat = record.getScore() + " 分";
            scoreElem.setText(concat);
        }

        LinearLayout layout;
    }

    private Activity activity;
    private List<Record> records;
}
