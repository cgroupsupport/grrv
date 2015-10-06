package com.cgroup.androidcontrolinterface;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

/** Standard Activity, in which, user can control the car by using the rotation system of the smartphone
 */
public class StandardControlActivity extends Activity {
    /** Variable Declaration */
    private Intent backgroundIntentService;
    private Intent myoBackgroundService;

    // the application which takes those global variables
    private androidcontrolinterface androidcontrolinterfaceVariable;
    private boolean toMyo = false;
    private boolean isLongClick = false;
    private boolean isStop = true;
    private Button runStopButton;
    private TextView textRcvData;
    private boolean isTextRcvData = false;
    private byte[] rcvData = new byte[3];

    /** When the Standard Control Activity is created, we will create the application variables
     * and initialize thevariables.
     * @param savedInstanceState Bundle
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_standard_control);
        runStopButton = (Button) findViewById(R.id.buttonRunStop);
        if(isStop) {
            androidcontrolinterfaceVariable = (androidcontrolinterface) getApplication();
            androidcontrolinterfaceVariable.setForceToStop(true);
            runStopButton.setBackgroundResource(R.drawable.run);
        }

        /** Touch Listener, when user touches the stop button, he will stop the car and initialize the global reference.
         * When user release the stop button, the global reference will be initialized and the car will run again.
         */
        runStopButton.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                androidcontrolinterfaceVariable = (androidcontrolinterface) getApplication();
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    androidcontrolinterfaceVariable.setForceToStop(true);
                    InitSensorThread();
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    if (isLongClick) {
                        androidcontrolinterfaceVariable.setForceToStop(false);
                        isLongClick = false;
                    } else {
                        isStop = !isStop;
                        androidcontrolinterfaceVariable.setForceToStop(isStop);
                        if (!isStop) runStopButton.setBackgroundResource(R.drawable.stop);
                        else runStopButton.setBackgroundResource(R.drawable.run);
                    }
                }
                return false;
            }
        });

        /** Long Click Listener, if we find that the user didn't touch the stop button for a long time,
         * we will change the car state (run or stop) when he releases the stop button.
         */
         runStopButton.setOnLongClickListener(new View.OnLongClickListener() {
                @Override
                public boolean onLongClick(View v) {
                if(!isStop) isLongClick = true;
                return false;
            }
        });

        textRcvData = (TextView) findViewById(R.id.textReceiveData);
        isTextRcvData = true;
    }

    /** The activity is resumed
     */
    @Override
    public void onResume() {
        super.onResume();
        androidcontrolinterfaceVariable = (androidcontrolinterface) getApplication();
        androidcontrolinterfaceVariable.setStandardIsRunning(true);
        androidcontrolinterfaceVariable.setMyoIsRunning(false);
        androidcontrolinterfaceVariable.setIsRunning(true);
        backgroundIntentService  = new Intent(this, BackgroundIntentService.class);
        startService(backgroundIntentService);
        Log.d("Interface", "StandardControlActivity onResume");
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

    /** When the activity is paused, we will stop the car
     */
    @Override
    public void onPause() {
        super.onPause();
        androidcontrolinterfaceVariable = (androidcontrolinterface) getApplication();
        Log.d("Interface", "StandardControlActivity onPause");
        if(!toMyo) {
            androidcontrolinterfaceVariable = (androidcontrolinterface) getApplication();
            androidcontrolinterfaceVariable.setStandardIsRunning(false);
            androidcontrolinterfaceVariable.setMyoIsRunning(false);
            androidcontrolinterfaceVariable.setIsRunning(true);
            backgroundIntentService  = new Intent(this, BackgroundIntentService.class);
            stopService(backgroundIntentService);
            Intent intent = new Intent(this, ConnectActivity.class);
            startActivity(intent);
            finish();
        }
        else toMyo = false;
    }

    /** When the activity is destroyed, we will close the activity
     */
    @Override
    public void onDestroy() {
        super.onDestroy();
        isTextRcvData = false;
        Log.d("Interface", "StandardControlActivity onDestroy");
        finish();
    }

    /** Connect Myo Button, by clicking on the button, user can enter the Myo interface
     * @param view
     * @throws InterruptedException
     */
    public void connectMyo(View view) throws InterruptedException {
        toMyo = true;
        Intent intent = new Intent(this, MyoControlActivity.class);
        startActivity(intent);
        finish();
    }

    /** Turn Left Button, this command turns the camera to the left side.
     * This thread will send the command per 100 ms*/
    public void InitSensorThread() {
        new Thread(new Runnable() {
            public void run() {
                do {
                    androidcontrolinterfaceVariable = (androidcontrolinterface) getApplication();
                    androidcontrolinterfaceVariable.setIsSensorInit(true);
                }while(androidcontrolinterfaceVariable.getForceToStop());
            }
        }).start();
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