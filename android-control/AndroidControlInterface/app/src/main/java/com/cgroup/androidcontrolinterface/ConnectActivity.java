package com.cgroup.androidcontrolinterface;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

/** The first interface that user will see, by clicking on the connect button, user enter the Interface Selection Activity
 */
public class ConnectActivity extends Activity {
    /**Variable Declaration */
    private androidcontrolinterface androidcontrolinterfaceVariable;
    private Intent backgroundIntentService;
    private Intent myoBackgroundService;
    private WifiManager mWifiManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_connect);
    }

    /** When the activity is resumed, we verify if it's because we lose the connection
     */
    @Override
    public void onResume() {
        super.onResume();
        androidcontrolinterfaceVariable = (androidcontrolinterface) getApplication();
        androidcontrolinterfaceVariable.setStandardIsRunning(false);
        androidcontrolinterfaceVariable.setMyoIsRunning(false);
        androidcontrolinterfaceVariable.setIsRunning(false);
        backgroundIntentService  = new Intent(this, BackgroundIntentService.class);
        stopService(backgroundIntentService);
        myoBackgroundService  = new Intent(this, MyoBackgroundService.class);
        stopService(myoBackgroundService);

        if(androidcontrolinterfaceVariable.getIsToastDisconnected()) {
            Toast.makeText(getApplicationContext(), "Connection lost", Toast.LENGTH_SHORT).show();
        }

        Log.d("Interface", "ConnectActivity onResume");
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);

        mWifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
    }

    @Override
    public void onPause() {
        super.onPause();
        Log.d("Interface", "ConnectActivity onPause");
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.d("Interface", "ConnectActivity onDestroy");
        finish();
    }

    /** Quit Button, by clicking on the button, user can stop the services, stop the vehicle and close the application
     * @param view
     */
    public void quit(View view) {
        myoBackgroundService = new Intent(this, MyoBackgroundService.class);
        stopService(myoBackgroundService);
        backgroundIntentService = new Intent(this, BackgroundIntentService.class);
        stopService(backgroundIntentService);
        moveTaskToBack(true);
        android.os.Process.killProcess(android.os.Process.myPid());
    }

    public void connect(View view){
        if (mWifiManager.getConnectionInfo().getSSID().equals("\"CgroupRasPi\"")) {
            launchSelection();
        } else {
            Toast.makeText(getApplicationContext(), "Choose the CgroupRasPi network", Toast.LENGTH_SHORT).show();
            startActivityForResult(new Intent(WifiManager.ACTION_PICK_WIFI_NETWORK), 1);
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (mWifiManager.getConnectionInfo().getSSID().equals("\"CgroupRasPi\"")) { // si le bon wifi
            launchSelection();
        } else {
            Toast.makeText(getApplicationContext(), "Select the correct network", Toast.LENGTH_SHORT).show();
        }
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
    }

    public void launchSelection() {
        Intent intentActivity = new Intent(this, InterfaceSelectionActivity.class);
        startActivity(intentActivity);//Enter the InterfaceSelectionActivity
        androidcontrolinterfaceVariable = (androidcontrolinterface) getApplication();
        androidcontrolinterfaceVariable.setIsRunning(true);
        finish();
    }
}
