package com.example.self_balancebotapplication;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

public class CarStateFragment extends Fragment {
    MainActivity mainActivity;

    private TextView textViewInclination;
    private TextView textViewLeftWheel;
    private TextView textViewRightWheel;

    private double psi = 0;
    private double leftAngle = 0;
    private double rightAngle = 0;

    private String phi = "0";
    private String x = "0";
    private String y = "0";
    private String samplingTime = "0";

    private TextView textViewPhi;
    private TextView textViewX;
    private TextView textViewY;
    private TextView textViewSamplingTime;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_car_state, container, false);

        textViewInclination = view.findViewById(R.id.textView_state_inclination);
        textViewInclination.setText("0");
        textViewLeftWheel = view.findViewById(R.id.textView_state_left_wheel);
        textViewLeftWheel.setText("0");
        textViewRightWheel = view.findViewById(R.id.textView_state_right_wheel);
        textViewRightWheel.setText("0");

        textViewX = view.findViewById(R.id.textView_x);
        textViewY = view.findViewById(R.id.textView_y);
        textViewPhi = view.findViewById(R.id.textView_phi);
        textViewSamplingTime = view.findViewById(R.id.textView_samplingTime);

        return view;
    }

    public void receiveData(double[] data){
        if( data.length==9 ){
            psi = Math.toDegrees(data[2]);
            leftAngle = Math.toDegrees(data[3]);
            rightAngle = Math.toDegrees(data[4]);

            phi = String.format("%f", data[5]);
            x = String.format("%f", data[6]);
            y = String.format("%f", data[7]);
            samplingTime = String.format("%f", data[8]);
        }
    }

    public void updateInformation(){
        if( (textViewInclination!=null) && (textViewLeftWheel!=null) && (textViewRightWheel!=null) && (textViewPhi!=null) && (textViewX!=null) && (textViewY!=null) ){
            textViewInclination.setText(String.format("%.02f (degree)", psi));
            textViewLeftWheel.setText(String.format("%.02f (degree)", leftAngle));
            textViewRightWheel.setText(String.format("%.02f (degree)", rightAngle));

            textViewPhi.setText(phi);
            textViewX.setText(x);
            textViewY.setText(y);
            textViewSamplingTime.setText(samplingTime);
        }
    }

    public void setActivity(MainActivity mainActivity) {
        this.mainActivity = mainActivity;
    }
}
