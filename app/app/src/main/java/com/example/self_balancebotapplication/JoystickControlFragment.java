package com.example.self_balancebotapplication;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.jmedeisis.bugstick.Joystick;
import com.jmedeisis.bugstick.JoystickListener;

public class JoystickControlFragment extends Fragment {
    MainActivity mainActivity;

    TextView yTextView ;
    TextView xTextView ;

    String xNoneString;
    String xValueString;
    String yNoneString;
    String yValueString;

    Joystick joystick_r;
    Joystick joystick_l;

    private int joy_value[];

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_joystick_control, container, false);

        joy_value = new int[2];
        joy_value[0] = 0;
        joy_value[1] = 0;

        yTextView = (TextView) view.findViewById(R.id.y_value);
        xTextView = (TextView) view.findViewById(R.id.x_value);

        xNoneString = "0 : X";
        xValueString = "%1$.0f : X";
        yNoneString = "Y : 0";
        yValueString = "Y : %1$.0f";

        joystick_r = (Joystick) view.findViewById(R.id.joystick_r);
        joystick_l = (Joystick) view.findViewById(R.id.joystick_l);


        joystick_r.setJoystickListener(new JoystickListener() {
            @Override
            public void onDown() {

            }

            @Override
            public void onDrag(float degrees, float offset) {
                float value = (float) (offset * 100 * Math.cos(Math.toRadians(degrees)));
                xTextView.setText(String.format(xValueString, value));
                joy_value[1] = (int)value;

                sendValue();
            }

            @Override
            public void onUp() {
                xTextView.setText(xNoneString);
                joy_value[1] = 0;

                sendValue();
            }
        });


        joystick_l.setJoystickListener(new JoystickListener() {
            @Override
            public void onDown() {
            }

            @Override
            public void onDrag(float degrees, float offset) {
                float value = (float) (offset * 100 * Math.sin(Math.toRadians(degrees)));
                yTextView.setText(String.format(yValueString, value));
                joy_value[0] = (int)value;

                sendValue();
            }

            @Override
            public void onUp() {
                yTextView.setText(yNoneString);
                joy_value[0] = 0;

                sendValue();
            }
        });

        return view;
    }

    public int[] getJoy_value() {
        return joy_value;
    }

    public void setActivity(MainActivity mainActivity) {
        this.mainActivity = mainActivity;
    }

    public void sendValue(){
        String data;
        int common = joy_value[0];
        int diff = joy_value[1];
        float left_motor, right_motor;

        if(joy_value[0] >= 0){
            left_motor = (float) (common + diff / 2.0);
            right_motor = (float) (common - diff / 2.0);
        }else{
            left_motor = (float) (common - diff / 2.0);
            right_motor = (float) (common + diff / 2.0);
        }
//        System.out.println(left_motor + ", " + right_motor);

        //mapping
        if(left_motor > 0) left_motor = (float) (left_motor * (231/150.0) + 25);
        if(left_motor < 0) left_motor = (float) (left_motor * (231/150.0) - 25);
        if(right_motor > 0) right_motor = (float) (right_motor * (231/150.0) + 25);
        if(right_motor < 0) right_motor = (float) (right_motor * (231/150.0) - 25);

        data = "~" + (int)left_motor + "," + (int)right_motor + "#";
        System.out.println(data);

        mainActivity.bluetoothFragment.bluetoothSendData(data);
    }
}
