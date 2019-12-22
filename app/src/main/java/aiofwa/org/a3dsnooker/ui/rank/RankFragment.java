package aiofwa.org.a3dsnooker.ui.rank;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.support.annotation.NonNull;
import android.support.v4.app.Fragment;
import android.arch.lifecycle.ViewModelProviders;

import aiofwa.org.a3dsnooker.R;
import aiofwa.org.a3dsnooker.GlobalViewModel;


public class RankFragment extends Fragment {

    private GlobalViewModel globalViewModel;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        globalViewModel =
                ViewModelProviders.of(getActivity()).get(GlobalViewModel.class);
        View root = inflater.inflate(R.layout.fragment_rank, container, false);
        return root;
    }
}
