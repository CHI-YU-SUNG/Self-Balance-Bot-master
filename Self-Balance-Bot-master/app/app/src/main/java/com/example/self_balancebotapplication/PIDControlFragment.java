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
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.Toast;

public class PIDControlFragment extends Fragment implements RadioGroup.OnCheckedChangeListener {
    MainActivity mainActivity;

    // motor
    private EditText editTextKp;
    private Button buttonKpDown;
    private Button buttonKpUp;

    private EditText editTextKi;
    private Button buttonKiDown;
    private Button buttonKiUp;

    private EditText editTextKd;
    private Button buttonKdDown;
    private Button buttonKdUp;

    private EditText editTextReference;
    private Button buttonReferenceDown;
    private Button buttonReferenceUp;

    // radio button
    private RadioButton radioButtonPsi;
    private RadioButton radioButtonTheta;
    public RadioGroup radioGroupController;

    // bottom
    private Button buttonSendPid;
    private Button buttonStopPid;

    // kp, ki, kd, reference
    String kp;
    String ki;
    String kd;
    String reference;

    // update information
    public boolean needUpdate = false;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_pid_control, container, false);

        // motor
        editTextKp = view.findViewById(R.id.editText_inclination_kp);
        buttonKpDown = view.findViewById(R.id.button_inclination_kp_down);
        buttonKpDown.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                editTextKp.setText(String.format("%.1f", Double.parseDouble(editTextKp.getText().toString()) - 0.1));
            }
        });
        buttonKpUp = view.findViewById(R.id.button_inclination_kp_up);
        buttonKpUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                editTextKp.setText(String.format("%.1f", Double.parseDouble(editTextKp.getText().toString()) + 0.1));
            }
        });

        editTextKi = view.findViewById(R.id.editText_inclination_ki);
        buttonKiDown = view.findViewById(R.id.button_inclination_ki_down);
        buttonKiDown.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                editTextKi.setText(String.format("%.0f", Double.parseDouble(editTextKi.getText().toString()) - 1));
            }
        });
        buttonKiUp = view.findViewById(R.id.button_inclination_ki_up);
        buttonKiUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                editTextKi.setText(String.format("%.0f", Double.parseDouble(editTextKi.getText().toString()) + 1));
            }
        });

        editTextKd = view.findViewById(R.id.editText_inclination_kd);
        buttonKdDown = view.findViewById(R.id.button_inclination_kd_down);
        buttonKdDown.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                editTextKd.setText(String.format("%.2f", Double.parseDouble(editTextKd.getText().toString()) - 0.01));
            }
        });
        buttonKdUp = view.findViewById(R.id.button_inclination_kd_up);
        buttonKdUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                editTextKd.setText(String.format("%.2f", Double.parseDouble(editTextKd.getText().toString()) + 0.01));
            }
        });

        editTextReference = view.findViewById(R.id.editText_inclination_reference);
        buttonReferenceDown = view.findViewById(R.id.button_inclination_reference_down);
        buttonReferenceDown.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                editTextReference.setText(String.format("%.3f", Double.parseDouble(editTextReference.getText().toString()) - 0.005));
            }
        });
        buttonReferenceUp = view.findViewById(R.id.button_inclination_reference_up);
        buttonReferenceUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                editTextReference.setText(String.format("%.3f", Double.parseDouble(editTextReference.getText().toString()) + 0.005));
            }
        });

        // radio button
        radioButtonPsi = view.findViewById(R.id.radioButton_psi);
        radioButtonTheta = view.findViewById(R.id.radioButton_theta);
        radioGroupController = view.findViewById(R.id.radioGroup_controller);
        radioGroupController.setOnCheckedChangeListener(this);


        // bottom
        buttonSendPid = view.findViewById(R.id.button_send_pid_inclination);
        buttonSendPid.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                send();
            }
        });
        buttonStopPid = view.findViewById(R.id.button_stop_pid_inclination);
        buttonStopPid.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                stop();
            }
        });

        return view;
    }

    @Override
    public void onCheckedChanged(RadioGroup group, int checkedId) {
        if (checkedId == R.id.radioButton_psi || checkedId == R.id.radioButton_theta){
            requestForInformaiton();
        }
    }

    private void send() {
        kp = editTextKp.getText().toString();
        ki = editTextKi.getText().toString();
        kd = editTextKd.getText().toString();
        reference = editTextReference.getText().toString();

        if (kp.isEmpty() || ki.isEmpty() || kd.isEmpty() || reference.isEmpty() ) {
            Toast.makeText(mainActivity, "Please Input All Parameters", Toast.LENGTH_LONG).show();
        } else {
            String data = "";
            if( radioButtonPsi.isChecked() ){
                data = "~1,"                                                              // send mode 1
                        + "2,"                                                               // item mode 2
                        + "1,"                                                               // control 1
                        + kp + "," + ki + "," + kd + "," + reference + "#";
            }else if( radioButtonTheta.isChecked() ){
                data = "~1,"                                                              // send mode 1
                        + "2,"                                                               // item mode 2
                        + "3,"                                                               // control 3
                        + kp + "," + ki + "," + kd + "," + reference + "#";
            }
            mainActivity.bluetoothFragment.bluetoothSendData(data);
        }
    }

    public void stop() {
        String data = "~2,"                                              // send mode 2
                + "0#";                                             // control mode 0
        System.out.println(data);
        mainActivity.bluetoothFragment.bluetoothSendData(data);
    }

    public void receiveData(String[] data){
        if( data.length==6 ){
            kp = data[2];
            ki = data[3];
            kd = data[4];
            reference = data[5];
        }
    }

    public void requestForInformaiton(){
        if (radioButtonPsi.isChecked()) {  // request for psi: kp, ki, kd, reference
            String data = "~2,"
                    + "1#";
            mainActivity.bluetoothFragment.bluetoothSendData(data);
        } else if (radioButtonTheta.isChecked()) {  // request for theta: kp, ki, kd, reference
            String data = "~2,"
                    + "2#";
            mainActivity.bluetoothFragment.bluetoothSendData(data);
        }
    }

    public void updateInformation(){
        if( editTextKp!=null && editTextKi!=null && editTextKd!=null && editTextReference!=null ){
            editTextKp.setText(kp);
            editTextKi.setText(ki);
            editTextKd.setText(kd);
            editTextReference.setText(reference);

            needUpdate = false;
        }
    }

    public void checkInformation(double []data){
        double error = 0.0001;
        if( Math.abs(data[2]-Double.parseDouble(kp))<error || Math.abs(data[3]-Double.parseDouble(ki))<error || Math.abs(data[4]-Double.parseDouble(kd))<error || Math.abs(data[5]-Double.parseDouble(reference))<error ){
            //Toast.makeText(mainActivity, "Successfully Set the Parameters", Toast.LENGTH_LONG).show();
            System.out.println("Right");
        } else {
            //Toast.makeText(mainActivity, "Fail to Set the Parameters! Please Try Again!", Toast.LENGTH_LONG).show();
            System.out.println("Error");
        }
    }

    public void setActivity(MainActivity mainActivity) {
        this.mainActivity = mainActivity;
    }
}