package com.example.self_balancebotapplication;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class StatefeedbackControlFragment extends Fragment {
    MainActivity mainActivity;

    // state feedback
    private EditText editTextK1;
    private EditText editTextK2;
    private EditText editTextK3;
    private EditText editTextK4;
    private Button buttonSendK;
    private Button buttonSendStop;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_statefeedback_control, container, false);

        editTextK1 = (EditText) view.findViewById(R.id.editText_k1);
        editTextK2 = (EditText) view.findViewById(R.id.editText_k2);
        editTextK3 = (EditText) view.findViewById(R.id.editText_k3);
        editTextK4 = (EditText) view.findViewById(R.id.editText_k4);

        buttonSendK = (Button) view.findViewById(R.id.button_send_k);
        buttonSendK.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendK();
            }
        });

        buttonSendStop = (Button) view.findViewById(R.id.button_send_statefeedback_stop);
        buttonSendStop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendStop();
            }
        });

        return view;
    }

    public void sendK(){
        String k1 = editTextK1.getText().toString();
        String k2 = editTextK2.getText().toString();
        String k3 = editTextK3.getText().toString();
        String k4 = editTextK4.getText().toString();

        if( k1.isEmpty() || k2.isEmpty() || k3.isEmpty() || k4.isEmpty() ){
            Toast.makeText(mainActivity, "Please Input All Parameters", Toast.LENGTH_LONG).show();
        }
        else{
            String data = "~2,"                                              // send mode 2
                    + "3,"                                                  // control mode 3
                    + k1 + "," + k2 + "," + k3 + "," + k4 + "#";             // state feedback
            mainActivity.bluetoothFragment.bluetoothSendData(data);
        }
    }

    public void sendStop(){
        String data = "~2,"                                              // send mode 2
                + "0#";                                             // control mode 0
        mainActivity.bluetoothFragment.bluetoothSendData(data);
    }

    public void setActivity(MainActivity mainActivity) {
        this.mainActivity = mainActivity;
    }
}
