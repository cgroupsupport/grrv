package com.cgroup.androidcontrolinterface;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

public class InterfaceSelectionActivity extends Activity {

    private androidcontrolinterface androidcontrolinterfaceVariable;
    private Intent backgroundIntentService;
    private Intent myoBackgroundService;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_interface_selection);
    }

    public void connectStandard(View view) throws InterruptedException {
        Intent intent = new Intent(this, StandardControlActivity.class);
        androidcontrolinterfaceVariable = (androidcontrolinterface) getApplication();
        androidcontrolinterfaceVariable.setIsRunning(true);
        myoBackgroundService = new Intent(this, MyoBackgroundService.class);
        stopService(myoBackgroundService);
        startActivity(intent);
        finish();
    }

    public void connectMyo(View view) throws InterruptedException {
        Intent intent = new Intent(this, MyoControlActivity.class);
        androidcontrolinterfaceVariable = (androidcontrolinterface) getApplication();
        androidcontrolinterfaceVariable.setIsRunning(true);
        startActivity(intent);
        finish();
    }

    @Override
    public void onResume() {
        super.onResume();
        androidcontrolinterfaceVariable = (androidcontrolinterface) getApplication();
        androidcontrolinterfaceVariable.setStandardIsRunning(false);
        androidcontrolinterfaceVariable.setMyoIsRunning(false);
        backgroundIntentService = new Intent(this, BackgroundIntentService.class);
        stopService(backgroundIntentService);
        myoBackgroundService  = new Intent(this, MyoBackgroundService.class);
        stopService(myoBackgroundService);
        Log.d("Interface", "InterfaceSelectionActivity onResume");
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
    }

    @Override
    public void onPause() {
        super.onPause();
        Log.d("Interface", "InterfaceSelectionActivity onPause");
        finish();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.d("Interface", "InterfaceSelectionActivity onDestroy");
        finish();
    }

    /** Quit Button, by clicking on the button, user can stop the services, stop the vehicle and close the application
     * @param view
     */
    public void quit(View view) {
        // TODO: Liberate resources and quit wifi
        myoBackgroundService = new Intent(this, MyoBackgroundService.class);
        stopService(myoBackgroundService);
        backgroundIntentService = new Intent(this, BackgroundIntentService.class);
        stopService(backgroundIntentService);
        moveTaskToBack(true);
        android.os.Process.killProcess(android.os.Process.myPid());
    }

    /** Back Button Pressed, by pressing the back button, user can stop the services, stop the vehicle and close the application
     */
    @Override
    public void onBackPressed() {
        super.onBackPressed();
        myoBackgroundService = new Intent(this, MyoBackgroundService.class);
        stopService(myoBackgroundService);
        backgroundIntentService = new Intent(this, BackgroundIntentService.class);
        stopService(backgroundIntentService);
        moveTaskToBack(true);
        android.os.Process.killProcess(android.os.Process.myPid());
    }
}
