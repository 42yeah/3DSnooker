package aiofwa.org.a3dsnooker.ui.achievement;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.animation.LayoutAnimationController;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProviders;
import androidx.paging.PagedList;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import aiofwa.org.a3dsnooker.R;
import aiofwa.org.a3dsnooker.GlobalViewModel;
import aiofwa.org.a3dsnooker.SlideyActivity;
import aiofwa.org.a3dsnooker.db.entity.Achievement;


public class AchievementFragment extends Fragment {

    private GlobalViewModel globalViewModel;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        assert getActivity() != null;
        context = getActivity();

        globalViewModel =
                ViewModelProviders.of(getActivity()).get(GlobalViewModel.class);
        View root = inflater.inflate(R.layout.fragment_achievement, container, false);

        achievementViewModel= ViewModelProviders.of(this).get(AchievementViewModel.class);
        recyclerView = root.findViewById(R.id.rv);
        recyclerView.setLayoutManager(new LinearLayoutManager(context));

        Animation animation = AnimationUtils.loadAnimation(this.getActivity(),R.anim.item_animation_fall_down);
        recyclerView.setAnimation(animation);
        final LayoutAnimationController layoutAnimation = AnimationUtils.loadLayoutAnimation(this.getActivity(),R.anim.layout_animation_fall_down);

        // 初始化适配器
        achievementAdapter = new AchievementAdapter();
        // 设置适配器
        recyclerView.setAdapter(achievementAdapter);

        achievementViewModel.getAchievements(getActivity()).observe(getActivity(), new Observer<PagedList<Achievement>>() {
            @Override
            public void onChanged(PagedList<Achievement> achievements) {
                achievementAdapter.submitList(achievements);
                recyclerView.setLayoutAnimation(layoutAnimation);
            }
        });
        return root;
    }

    private AchievementViewModel achievementViewModel;
    private RecyclerView recyclerView;
    private AchievementAdapter achievementAdapter;
    private Activity context;
}