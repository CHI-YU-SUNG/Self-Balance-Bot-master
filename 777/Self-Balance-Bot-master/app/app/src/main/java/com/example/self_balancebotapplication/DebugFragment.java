package com.example.self_balancebotapplication;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

public class DebugFragment extends Fragment {
    MainActivity mainActivity;

    private String leftErrorIntegral = "0";
    private String rightErrorIntegral = "0";

    private TextView textViewLeftErrorIntegral;
    private TextView textViewRightErrorIntegral;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_debug, container, false);

        textViewLeftErrorIntegral = view.findViewById(R.id.textView_x);
        textViewRightErrorIntegral = view.findViewById(R.id.textView_y);

        return view;
    }

    public void receiveData(String[] data){
        if( data.length==4 ){
            leftErrorIntegral = data[2];
            rightErrorIntegral = data[3];
        }
    }

    public void updateInformation(){
        if( (textViewLeftErrorIntegral!=null) && (textViewRightErrorIntegral!=null) ){
            textViewLeftErrorIntegral.setText(leftErrorIntegral);
            textViewRightErrorIntegral.setText(rightErrorIntegral);
        }
    }

    public void setActivity(MainActivity mainActivity) {
        this.mainActivity = mainActivity;
    }
}
