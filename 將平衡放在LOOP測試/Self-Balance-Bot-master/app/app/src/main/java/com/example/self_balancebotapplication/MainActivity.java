package com.example.self_balancebotapplication;

import android.content.pm.ActivityInfo;
import android.support.annotation.NonNull;
import android.support.design.widget.NavigationView;
import android.support.v4.app.Fragment;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.Toolbar;
import android.view.MenuItem;

import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends AppCompatActivity implements NavigationView.OnNavigationItemSelectedListener {
    private Toolbar toolbar;
    private DrawerLayout drawer;

    // update ui component
    private Timer timer;
    private TimerTask task;

    // fragment
    public HomeFragment homeFragment = new HomeFragment();
    public BluetoothFragment bluetoothFragment = new BluetoothFragment();
    public JoystickControlFragment joystickControlFragment = new JoystickControlFragment();
    public PIDControlFragment pidControlFragment = new PIDControlFragment();
    private StatefeedbackControlFragment statefeedbackControlFragment = new StatefeedbackControlFragment();
    private CarStateFragment carStateFragment = new CarStateFragment();
    private DebugFragment debugFragment = new DebugFragment();
    private Fragment lastFragment;
    private Fragment currentFragment;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

        toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        drawer = findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(this, drawer, toolbar,
                R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.addDrawerListener(toggle);
        toggle.syncState();

        // add navigation item select listener
        NavigationView navigationView = findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);

        // set the homeFragment as first fragment
        // ******* bug !!!!!
        getSupportFragmentManager().beginTransaction().add(R.id.fragment_container, homeFragment).commit();
        getSupportFragmentManager().beginTransaction().hide(homeFragment).commit();
        //getSupportFragmentManager().beginTransaction().show(homeFragment).commit();
        lastFragment = homeFragment;
        navigationView.setCheckedItem(R.id.nav_home);

        // set mainActivity to each fragment
        bluetoothFragment.setActivity(this);
        pidControlFragment.setActivity(this);
        joystickControlFragment.setActivity(this);
        statefeedbackControlFragment.setActivity(this);
        carStateFragment.setActivity(this);
        debugFragment.setActivity(this);

        task = new TimerTask() {
            @Override
            public void run() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        carStateFragment.updateInformation();
                        debugFragment.updateInformation();

                        if(pidControlFragment.needUpdate){
                            pidControlFragment.updateInformation();
                        }
                    }
                });
            }
        };
        timer = new Timer();
        timer.scheduleAtFixedRate(task, 1000, 250);
    }

    @Override
    public boolean onNavigationItemSelected(@NonNull MenuItem menuItem) { // which fragment is selected
        int id = menuItem.getItemId();
        switch(id){
            case R.id.nav_home:
                currentFragment = homeFragment;
                break;
            case R.id.nav_bluetooth:
                currentFragment = bluetoothFragment;
                break;
            case R.id.nav_joystick_control:
                currentFragment = joystickControlFragment;
                break;
            case R.id.nav_pid_control:
                currentFragment = pidControlFragment;
                break;
            case R.id.nav_statefeedback_control:
                currentFragment = statefeedbackControlFragment;
                break;
            case R.id.nav_car_state:
                currentFragment = carStateFragment;
                break;
            case R.id.nav_debug:
                currentFragment = debugFragment;
                break;
        }

        // use hide and show , instead of replace
        // hide, show: does not destroy fragment
        // replace: destroy old fragment and create an new fragment
        if(currentFragment != lastFragment){
            if(!currentFragment.isAdded()){
                getSupportFragmentManager().beginTransaction().add(R.id.fragment_container, currentFragment).commit();
                getSupportFragmentManager().beginTransaction().hide(lastFragment).commit();
                getSupportFragmentManager().beginTransaction().show(currentFragment).commit();
            } else {
                getSupportFragmentManager().beginTransaction().hide(lastFragment).commit();
                getSupportFragmentManager().beginTransaction().show(currentFragment).commit();
            }

            lastFragment = currentFragment;
        }

        drawer.closeDrawer(GravityCompat.START);
        return true;
    }

    @Override
    public void onBackPressed() {
        if(drawer.isDrawerOpen(GravityCompat.START)){
            drawer.closeDrawer(GravityCompat.START);
        } else{
            super.onBackPressed();
        }
    }

    public void processData(String rawData){
        String dataS[] = rawData.split(",");
        //System.out.println(temp.length);
        double dataD[] = new double[dataS.length];
        for(int i=0; i<dataS.length; i++){
            dataD[i] = Double.parseDouble(dataS[i]);
            //System.out.println(dataD[i]);
        }

        if( ((int)dataD[0])==1 ){
            switch( (int)dataD[1] ){
                case 1:
                    carStateFragment.receiveData(dataD);
                    break;
                case 2:
                    pidControlFragment.receiveData(dataS);
                    pidControlFragment.needUpdate = true;
                    break;
                case 3:
                    debugFragment.receiveData(dataS);
                    break;
            }
        } else if( ((int)dataD[0])==2 ){
            switch( (int)dataD[1] ){
                case 1:
                case 2:
                    pidControlFragment.checkInformation(dataD);
                    break;
            }
        }
    }
}