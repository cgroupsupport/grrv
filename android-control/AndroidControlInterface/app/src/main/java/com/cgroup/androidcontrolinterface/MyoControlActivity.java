package com.cgroup.androidcontrolinterface;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

/** Myo Activity, in which, user can control the car by using the Myo.
 */
public class MyoControlActivity extends Activity {
    private androidcontrolinterface androidcontrolinterfaceVariable;
    private Intent backgroundIntentService;
    private Intent myoBackgroundService;
    private TextView textRcvData;
    private boolean isTextRcvData = false;
    private byte[] rcvData = new byte[3];

    /** When the activity is created, we do nothing
     * @param savedInstanceState Bundle
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_myo_control);
        textRcvData = (TextView) findViewById(R.id.textReceiveData);
        isTextRcvData = true;
    }

    /** When the activity is resumed, we will start the Myo control service
     */
    @Override
    public void onResume() {
        super.onResume();
        androidcontrolinterfaceVariable = (androidcontrolinterface) getApplication();
        androidcontrolinterfaceVariable.setStandardIsRunning(false);
        androidcontrolinterfaceVariable.setMyoIsRunning(true);
        androidcontrolinterfaceVariable.setIsRunning(true);
        backgroundIntentService  = new Intent(this, BackgroundIntentService.class);
        startService(backgroundIntentService);
        myoBackgroundService = new Intent(this, MyoBackgroundService.class);
        startService(myoBackgroundService);
        Log.d("Interface", "MyoControlActivity onResume");
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);

        textRcvDataThread();
    }

    /** When the activity is paused, we will do nothing.
     */
    @Override
    public void onPause() {
        super.onPause();
        Log.d("Interface", "MyoControlActivity onPause");
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
        isTextRcvData = false;
        moveTaskToBack(true);
        android.os.Process.killProcess(android.os.Process.myPid());
    }

    /** When the activity is destroyed, we will close the activity.
     */
    @Override
    public void onDestroy() {
        super.onDestroy();
        finish();
    }

    /** The connect Standard button, by clicking on the button, we will enter the standard interface
     * and switch to the standard mode
     * @param view
     * @throws InterruptedException
     */

    public void connectStandard(View view) throws InterruptedException {
        Intent intent = new Intent(this, StandardControlActivity.class);
        myoBackgroundService = new Intent(this, MyoBackgroundService.class);
        stopService(myoBackgroundService);
        startActivity(intent);
        finish();
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
        isTextRcvData = false;
        moveTaskToBack(true);
        android.os.Process.killProcess(android.os.Process.myPid());
    }

    /** It shows the response from Raspberry Pi in the activity
     */
    public void textRcvDataThread() {
        new Thread() {
            public void run() {
                while(isTextRcvData) {
                    androidcontrolinterfaceVariable = (androidcontrolinterface) getApplication();
                    rcvData = androidcontrolinterfaceVariable.getReceiveData();
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            textRcvData.setText((int) rcvData[1] + " " + (int) rcvData[2]);
                        }
                    });
                    try {
                        Thread.sleep(100);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }.start();
    }
}
