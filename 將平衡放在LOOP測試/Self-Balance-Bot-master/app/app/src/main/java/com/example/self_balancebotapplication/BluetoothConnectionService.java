package com.example.self_balancebotapplication;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.nio.charset.Charset;
import java.util.UUID;

/**
 * Created by User on 12/21/2016.
 */

public class BluetoothConnectionService {
    private static final String TAG = "BluetoothConnectionServ";

    private static final UUID MY_UUID_SECURE =
            UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private final BluetoothAdapter mBluetoothAdapter;
    Context mContext;

    private BluetoothDevice mmDevice;
    ProgressDialog mProgressDialog;

    private ConnectedThread mConnectedThread;
    private TextView connectedText;

    // for ui control
    BluetoothFragment bluetoothFragment;
    MainActivity mainActivity;

    public BluetoothConnectionService(Context context, BluetoothDevice device, TextView textView, BluetoothFragment bluetoothFragment, MainActivity mainActivity) {
        mContext = context;
        connectedText = textView;
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        Log.d(TAG, "startClient: Started.");
        mmDevice = device;
        new ConnectBT().execute();

        this.bluetoothFragment = bluetoothFragment;
        this.mainActivity = mainActivity;
    }

    private class ConnectBT extends AsyncTask<Void, Void, Void> {
        private boolean ConnectSuccess = true;
        private BluetoothSocket btSocket;
        private BluetoothDevice dispositivo;
        @Override
        protected  void onPreExecute () {
            mProgressDialog = ProgressDialog.show(mContext,"Connecting Bluetooth"
                    ,"Please Wait...",true);
        }

        @Override
        protected Void doInBackground (Void... devices) {
            try {
                dispositivo = mBluetoothAdapter.getRemoteDevice(mmDevice.getAddress());
                // for bluetooth 4.1
                btSocket = dispositivo.createRfcommSocketToServiceRecord(MY_UUID_SECURE);

                BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                btSocket.connect();

            } catch (IOException e) {
                ConnectSuccess = false;
                e.printStackTrace();
            }
            return null;
        }

        @Override
        protected void onPostExecute (Void result) {
            super.onPostExecute(result);

            //dismiss the progressdialog when connection is established
            try{
                mProgressDialog.dismiss();
            }catch (NullPointerException e){
                e.printStackTrace();
            }

            if (!ConnectSuccess) {
                Toast.makeText(bluetoothFragment.mainActivity, "Fail to connect", Toast.LENGTH_LONG).show();
                connectedText.setText("Unconnected");
                Log.d(TAG,"Connection Failed. Is it a SPP Bluetooth? Try again.");
            } else {
                bluetoothFragment.bluetoothConnect = true;
                startCommunication(btSocket,dispositivo);
                connectedText.setText("Connected to : " + mmDevice.getName()+"\n"+ mmDevice.getAddress());

                // request for information of car
                // 1. psi controller kp, ki, kd, reference
                String data = "~2,"
                            + "1#";
                mainActivity.bluetoothFragment.bluetoothSendData(data);
            }
        }
    }

    /**
     Finally the ConnectedThread which is responsible for maintaining the BTConnection, Sending the data, and
     receiving incoming data through input/output streams respectively.
     **/
    private class ConnectedThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedThread(BluetoothSocket socket) {
            Log.d(TAG, "ConnectedThread: Starting.");

            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            try {
                tmpIn = mmSocket.getInputStream();
                tmpOut = mmSocket.getOutputStream();
            } catch (IOException e) {
                e.printStackTrace();
            }
            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        public void run(){
            byte[] buffer = new byte[1024];  // buffer store for the stream

            int bytes; // bytes returned from read()

            // Keep listening to the InputStream until an exception occurs
            while (true) {
                // Read from the InputStream
                try {
                    //bytes = mmInStream.read(buffer);
                    //String temp = new String(buffer, 0, bytes);
                    BufferedReader r = new BufferedReader(new InputStreamReader(mmInStream));
                    String rawData = r.readLine();
                    Log.d(TAG, "InputStream: " + rawData);

                    // receive data
                    if( rawData.contains("~") && rawData.contains("#") ){
                        int beginIndex  = rawData.indexOf("~");
                        int endIndex = rawData.indexOf("#");
                        String data = rawData.substring(beginIndex+1, endIndex);

                        System.out.println("data =  "+ data);
                        mainActivity.processData(data);
                    }
                } catch (IOException e) {
                    Log.e(TAG, "write: Error reading Input Stream. " + e.getMessage() );
                    break;
                }
            }
        }

        //Call this from the main activity to send data to the remote device
        public void write(byte[] bytes) {
            String text = new String(bytes, Charset.defaultCharset());
            Log.d(TAG, "write: Writing to outputstream: " + text);
            try {
                mmOutStream.write(bytes);
            } catch (IOException e) {
                Log.e(TAG, "write: Error writing to output stream. " + e.getMessage() );
            }
        }

        /* Call this from the main activity to shutdown the connection */
        public void cancel() {
            try {
                connectedText.setText("Unconnected");
                bluetoothFragment.bluetoothConnect = false;
                mmSocket.close();
            } catch (IOException e) { }
        }
    }

    private void startCommunication(BluetoothSocket mmSocket, BluetoothDevice mmDevice) {
        Log.d(TAG, "connected: Starting.");
        bluetoothFragment.bluetoothConnect = true;
        // Start the thread to manage the connection and perform transmissions
        mConnectedThread = new ConnectedThread(mmSocket);
        mConnectedThread.start();
    }

    public void write(String string) {
        byte[] bytes = string.getBytes(Charset.defaultCharset());

        Log.d(TAG, "write: Write Called.");
        //perform the write
        if(mConnectedThread!=null)
            mConnectedThread.write(bytes);
    }
}

/*
Send mode:
1: Send parameter to car, such as kp, ki, kd, etc.
2: Request for information of car state, such as kp, ki, kd in car.

Item mode:

 */