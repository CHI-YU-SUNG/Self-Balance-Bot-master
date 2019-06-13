package com.example.self_balancebotapplication;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.UUID;

public class BluetoothFragment extends Fragment implements AdapterView.OnItemClickListener{
    private static final String TAG = " fragment_bluetooth";
    MainActivity mainActivity;

    // bluetooth
    // part1: enable bluetooth
    BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    Switch switchBluetooth;

    // part2: search bluetooth device
    Button buttonSearch;
    public ArrayList<BluetoothDevice> bluetoothDeviceArrayList;
    public DeviceListAdapter mDeviceListAdapter;
    ListView listViewShowDevice;

    // part3: connect to device
    BluetoothConnectionService mBluetoothConnection;
    BluetoothDevice mBTDevice;
    TextView textViewShowBluetooth;
    public boolean bluetoothConnect = false;

    // Create a BroadcastReceiver for ACTION_FOUND
    private final BroadcastReceiver mBroadcastReceiver1 = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            // When discovery finds a device
            if (action.equals(mBluetoothAdapter.ACTION_STATE_CHANGED)) {
                final int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, mBluetoothAdapter.ERROR);
                switch(state){
                    case BluetoothAdapter.STATE_OFF:
                        Log.d(TAG, "mBroadcastReceiver1: STATE OFF");
                        switchBluetooth.setChecked((false));
                        break;
                    case BluetoothAdapter.STATE_TURNING_OFF:
                        Log.d(TAG, "mBroadcastReceiver1: STATE TURNING OFF");
                        break;
                    case BluetoothAdapter.STATE_ON:
                        Log.d(TAG, "mBroadcastReceiver1: STATE ON");
                        switchBluetooth.setChecked((true));
                        break;
                    case BluetoothAdapter.STATE_TURNING_ON:
                        Log.d(TAG, "mBroadcastReceiver1: STATE TURNING ON");
                        break;
                }
            }
        }
    };

    /**
     * Broadcast Receiver for listing devices that are not yet paired
     * -Executed by btnDiscover() method.
     */
    private BroadcastReceiver mBroadcastReceiver2 = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            Log.d(TAG, "onReceive: ACTION FOUND.");

            if (action.equals(BluetoothDevice.ACTION_FOUND)){
                BluetoothDevice device = intent.getParcelableExtra (BluetoothDevice.EXTRA_DEVICE);
                bluetoothDeviceArrayList.add(device);
                Log.d(TAG, "onReceive: " + device.getName() + ": " + device.getAddress());

                mDeviceListAdapter = new DeviceListAdapter(context, R.layout.device_adapter_view, bluetoothDeviceArrayList);
                listViewShowDevice.setAdapter(mDeviceListAdapter);
            }
        }
    };

    /**
     * Broadcast Receiver that detects bond state changes (Pairing status changes)
     */
    private final BroadcastReceiver mBroadcastReceiver3 = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();

            if(action.equals(BluetoothDevice.ACTION_BOND_STATE_CHANGED)){
                BluetoothDevice mDevice = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                //3 cases:
                //case1: bonded already
                if (mDevice.getBondState() == BluetoothDevice.BOND_BONDED){
                    Log.d(TAG, "BroadcastReceiver: BOND_BONDED.");
                    //inside BroadcastReceiver4
                    mBTDevice = mDevice;
                }
                //case2: creating a bone
                if (mDevice.getBondState() == BluetoothDevice.BOND_BONDING) {
                    Log.d(TAG, "BroadcastReceiver: BOND_BONDING.");
                }
                //case3: breaking a bond
                if (mDevice.getBondState() == BluetoothDevice.BOND_NONE) {
                    Log.d(TAG, "BroadcastReceiver: BOND_NONE.");
                }
            }
        }
    };

    @Override
    public void onDestroy() {
        Log.d(TAG, "onDestroy: called.");
        super.onDestroy();
        mainActivity.unregisterReceiver(mBroadcastReceiver1);
        mainActivity.unregisterReceiver(mBroadcastReceiver2);
        mainActivity.unregisterReceiver(mBroadcastReceiver3);
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_bluetooth, container, false);
        switchBluetooth = (Switch) view.findViewById(R.id.switch_bluetooth);
        textViewShowBluetooth = (TextView) view.findViewById(R.id.textView_show_bluetooth);
        buttonSearch = (Button) view.findViewById(R.id.button_search);
        listViewShowDevice = (ListView) view.findViewById(R.id.listView_show_device);

        // bluetooth
        // part1: enable bluetooth
        if (mBluetoothAdapter.isEnabled()) {
            switchBluetooth.setChecked(true);
            buttonSearch.setEnabled(true);
        }
        switchBluetooth.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.d(TAG, "onClick: enabling/disabling bluetooth.");
                if (switchBluetooth.isChecked()) {
                    enableBluetooth(true);
                    buttonSearch.setEnabled(true);
                } else {
                    enableBluetooth(false);
                    buttonSearch.setEnabled(false);
                    textViewShowBluetooth.setText("Unconnected");
                    bluetoothConnect = false;
                }
            }
        });

        // part2: search bluetooth device
        buttonSearch.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                bluetoothDeviceArrayList.clear();
                listViewShowDevice.setAdapter(null);
                searchDivice();
                bluetoothConnect = false;
            }
        });
        bluetoothDeviceArrayList = new ArrayList<>();


        // part3: connect to device
        // Broadcasts when bond state changes (ie:pairing)
        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_BOND_STATE_CHANGED);
        mainActivity.registerReceiver(mBroadcastReceiver3, filter);
        listViewShowDevice.setOnItemClickListener(BluetoothFragment.this);

        return view;
    }

    // bluetooth
    // part1: enable bluetooth
    // for switchBluetooth
    public void enableBluetooth(boolean open) {
        if(mBluetoothAdapter == null){
            Log.d(TAG, "enableDisableBT: Does not have BT capabilities.");
        }

        // bluetooth is disabled and want to be enabled
        if (!mBluetoothAdapter.isEnabled() && open) {
            Log.d(TAG, "enableDisableBT: enabling BT.");
            Intent enableBTIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivity(enableBTIntent);

            IntentFilter BTIntent = new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED);
            mainActivity.registerReceiver(mBroadcastReceiver1, BTIntent);
        }

        if (mBluetoothAdapter.isEnabled() && !open) {
            Log.d(TAG, "enableDisableBT: disabling BT.");
            mBluetoothAdapter.disable();

            IntentFilter BTIntent = new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED);
            mainActivity.registerReceiver(mBroadcastReceiver1, BTIntent);
        }
    }

    // part2: search bluetooth device
    // for buttonSearch
    public void searchDivice() {
        Log.d(TAG, "btnDiscover: Looking for unpaired devices.");

        if(mBluetoothAdapter.isDiscovering()){
            mBluetoothAdapter.cancelDiscovery();
            Log.d(TAG, "btnDiscover: Canceling discovery.");

            //check BT permissions in manifest
            checkBTPermissions();

            mBluetoothAdapter.startDiscovery();
            IntentFilter discoverDevicesIntent = new IntentFilter(BluetoothDevice.ACTION_FOUND);
            mainActivity.registerReceiver(mBroadcastReceiver2, discoverDevicesIntent);
        }

        if(!mBluetoothAdapter.isDiscovering()){
            //check BT permissions in manifest
            checkBTPermissions();

            mBluetoothAdapter.startDiscovery();
            IntentFilter discoverDevicesIntent = new IntentFilter(BluetoothDevice.ACTION_FOUND);
            mainActivity.registerReceiver(mBroadcastReceiver2, discoverDevicesIntent);
        }
    }

    /*
     *  This method is required for all devices running API23+
     *  Android must programmatically check the permissions for bluetooth. Putting the proper permissions
     *  in the manifest is not enough.
     *
     **NOTE: This will only execute on versions > LOLLIPOP because it is not needed otherwise.
     */
    private void checkBTPermissions() {
        this.requestPermissions(new String[]{Manifest.permission.ACCESS_FINE_LOCATION, Manifest.permission.ACCESS_COARSE_LOCATION}, 1001); //Any number
    }

    // part3: connect to device
    @Override
    public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
        //first cancel discovery because its very memory intensive.
        mBluetoothAdapter.cancelDiscovery();

        Log.d(TAG, "onItemClick: You Clicked on a device.");
        String deviceName = bluetoothDeviceArrayList.get(i).getName();
        String deviceAddress = bluetoothDeviceArrayList.get(i).getAddress();
        Log.d(TAG, "onItemClick: deviceName = " + deviceName);
        Log.d(TAG, "onItemClick: deviceAddress = " + deviceAddress);

        // connect to device
        mBTDevice = mBluetoothAdapter.getRemoteDevice(deviceAddress);
        mBluetoothConnection = new BluetoothConnectionService(mainActivity, mBTDevice, textViewShowBluetooth, this, mainActivity);
        bluetoothDeviceArrayList.clear();
        listViewShowDevice.setAdapter(null);
    }

    // part4: send Message
    // send mode:
    // 1: Joystick Control Fragment
    // 2: PID Control Fragment & State feedback Control Fragment
    public void bluetoothSendData(String data){
        if( this.mBluetoothConnection!=null && bluetoothConnect) {
            try {
                this.mBluetoothConnection.write(data);
            } catch (NullPointerException except) {
                Toast.makeText(mainActivity, "Fial to Send Data", Toast.LENGTH_LONG).show();
            }
        } else{
            Toast.makeText(mainActivity, "Blutooth is Not Connected", Toast.LENGTH_LONG).show();
        }
    }


    public void setActivity(MainActivity mainActivity) {
        this.mainActivity = mainActivity;
    }
}