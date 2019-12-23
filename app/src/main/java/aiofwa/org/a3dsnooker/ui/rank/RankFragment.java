package aiofwa.org.a3dsnooker.ui.rank;

import android.app.Activity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.AnimationUtils;
import android.view.animation.LayoutAnimationController;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProviders;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import java.util.concurrent.Executors;

import aiofwa.org.a3dsnooker.R;
import aiofwa.org.a3dsnooker.GlobalViewModel;
import aiofwa.org.a3dsnooker.SlideyActivity;
import aiofwa.org.a3dsnooker.db.database.AppDatabase;


public class RankFragment extends Fragment {

    private GlobalViewModel globalViewModel;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        assert getActivity() != null;
        context = getActivity();

        globalViewModel =
                ViewModelProviders.of(getActivity()).get(GlobalViewModel.class);
        View root = inflater.inflate(R.layout.fragment_rank, container, false);

        final RecyclerView rankRecyclerView = root.findViewById(R.id.rankRecyclerView);
        final LayoutAnimationController layoutAnimation = AnimationUtils.loadLayoutAnimation(this.getActivity(),R.anim.layout_animation_fall_down);

        new Thread() {
            @Override
            public void run() {
                final RankAdapter adapter = new RankAdapter(context, AppDatabase.getInstance(getActivity()).getRecordDao().getRecords());
                context.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        rankRecyclerView.setAdapter(adapter);
                        rankRecyclerView.setLayoutManager(new LinearLayoutManager(context));
                        rankRecyclerView.setLayoutAnimation(layoutAnimation);
                    }
                });
            }
        }.start();

        return root;
    }

    private Activity context;
}
