package aiofwa.org.a3dsnooker.ui.achievement;

import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.paging.PagedListAdapter;
import androidx.recyclerview.widget.DiffUtil;
import androidx.recyclerview.widget.RecyclerView;

import aiofwa.org.a3dsnooker.R;
import aiofwa.org.a3dsnooker.db.entity.Achievement;


class AchievementAdapter extends PagedListAdapter<Achievement, AchievementAdapter.ItemHolder> {
    AchievementAdapter() {
        super(DIFF_CALLBACK);
    }

    @NonNull
    @Override
    public ItemHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        return new ItemHolder(LayoutInflater.from(parent.getContext())
                .inflate(R.layout.achievement_recycler_view_item, parent, false));
    }

    @Override
    public void onBindViewHolder(@NonNull ItemHolder holder, int position) {
        // 设置成就名称
        Achievement achievement = getItem(position);
        holder.achievementName.setText(achievement.getAchievement());
        // 设置成就状态
        if (achievement.isFinished()) {
            holder.achievementState.setTextColor(Color.rgb(0, 123, 0));
            holder.achievementState.setText("已达成");
        } else {
            holder.achievementState.setText("未达成");
        }
    }

    class ItemHolder extends RecyclerView.ViewHolder {
        TextView achievementName;
        TextView achievementState;
        ItemHolder(View item) {
            super(item);
            achievementName = item.findViewById(R.id.achievementName);
            achievementState = item.findViewById(R.id.achievementState);
        }
    }

    private static DiffUtil.ItemCallback<Achievement> DIFF_CALLBACK = new DiffUtil.ItemCallback<Achievement>() {
        @Override
        public boolean areItemsTheSame(Achievement oldAchievement, Achievement newAchievement) {
            return oldAchievement.getId() == newAchievement.getId();
        }

        @Override
        public boolean areContentsTheSame(Achievement oldAchievement, Achievement newAchievement) {
            return oldAchievement.equals(newAchievement);
        }
    };

}