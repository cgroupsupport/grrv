package com.cgroup.androidcontrolinterface;

import android.app.IntentService;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.media.MediaPlayer;
import android.os.PowerManager;
import android.util.Log;

import java.io.IOException;
import java.net.SocketException;

/**
 * Created by Teng on 2015/9/21.
 */

/** Background Intent Service is a service which is used for the principal function as
 * udp message sending and receiving
 * managing telephone sensors
 *
 * This service will keep sending and receiving udp messages per 5 ms before continuing losing 10 udp (timeout = 200 ms) feedback from raspberry pi in 2 s,
 * which means that we lose the connection.
 */
public class BackgroundIntentService extends IntentService implements SensorEventListener {
    /** Variables Declaration */
    androidcontrolinterface androidcontrolinterfaceVariable;

    /** ConnectActivitu Intent */
    Intent connectActivity;

    /** isRunning is a boolean which is used for start and stop the service */
    protected boolean isRunning  = false;
    protected boolean standardIsRunning  = false;
    protected boolean myoIsRunning  = false;

    /** isRunning is a boolean which is used for udp errors */
    protected boolean errorUDP = false;

    /** Class UDP Client */
    protected UDPClientClass udpClientClass;

    /** a package of bytes is defined to be 3 bytes, #1 byte is the identificati2on, #2 byte and #3 byte is the data */
    protected byte[] bytes = new byte[3];

    /** Class MediaPlayer which will launch the alert.mp3 in the src/res/raw/alert.mp3 */
    protected MediaPlayer alertPlayer;

    /** Raspberry Pi IP address */
    String ipAddress = "10.0.0.1";//"10.214.12.218";//"192.168.1.17";//"192.168.43.161"//"10.0.0.1";
    int port = 2015;

    /** Counter for times of timeout */
    int counter = 0;

    /** Declaration of the package */
    byte id = (byte)0x01;
    byte RunStopReverse = 0;
    byte direction = 0;

    /** Power Manager to keep CPU On*/
    PowerManager powerManager;
    PowerManager.WakeLock wakeLock;

    /** Sensor Manager Declaration*/
    protected SensorManager mSensorManager;
    protected Sensor mRotationSensor;
    protected float[] rotationMatrix, orientation, rotation;

    protected int stDirection;
    protected int stRunStopReverse;
    protected int stRunStopReverseInit;

    /** The constructor */
    public BackgroundIntentService() {
        super("BackgroundIntentService");
    }

    @Override
    public void onCreate() {
        super.onCreate();
        try {
            udpClientClass = new UDPClientClass();
        } catch (SocketException e) {
            e.printStackTrace();
        }

    /** Power Manager Initialization */
        powerManager = (PowerManager) getSystemService(POWER_SERVICE);
        wakeLock = powerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "MyWakelockTag");
    /** Alert MP3 Initialization */
        alertPlayer = MediaPlayer.create(getApplicationContext(), R.raw.alert);
    /** Get the application context */
        androidcontrolinterfaceVariable = (androidcontrolinterface) getApplication();

    /** Sensor Manager Initialization */
        mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        mRotationSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR);
        mSensorManager.registerListener(this, mRotationSensor, SensorManager.SENSOR_DELAY_NORMAL);
        rotationMatrix = new float[9];
        orientation = new float[3];
        rotation = new float[3];
        androidcontrolinterfaceVariable.setIsSensorInit(false);
    }

    /** The principal function of the background intentService
     * @param intent
     */
    @Override
    protected void onHandleIntent(Intent intent) {
        androidcontrolinterfaceVariable = (androidcontrolinterface) getApplication();
        androidcontrolinterfaceVariable.setIsToastDisconnected(false);
        /*register the sensor listener to listen to the gyroscope sensor, use the
        callbacks defined in this class, and gather the sensor information as quick
        as possible*/
    /** Set Timeout  200ms for reception */
        try {
            udpClientClass.setSoTimeout(200);
        } catch (SocketException e) {
            e.printStackTrace();
        }

    /** If isRunning is false, we quit the loop and close the service */
        while((androidcontrolinterfaceVariable.getIsRunning() || wakeLock.isHeld())) {
            isRunning = androidcontrolinterfaceVariable.getIsRunning();
            standardIsRunning = androidcontrolinterfaceVariable.getStandardIsRunning();
            myoIsRunning = androidcontrolinterfaceVariable.getMyoIsRunning();

    /** Per 5ms, we send a message */
            if (myoIsRunning || standardIsRunning || wakeLock.isHeld()) {
                if (standardIsRunning) {
                    if(wakeLock.isHeld()) {
                        wakeLock.release();
                        Log.d("WakeLock", "Off");
                    }
                    Log.d("BackgroundService", "standardIsRunning == true");
                }
                if (myoIsRunning) {
                    if(!wakeLock.isHeld()) {
                        wakeLock.acquire();
                        Log.d("WakeLock", "On");
                    }
                    Log.d("BackgroundService", "myoIsRunning == true");
                }

                try {
                    Thread.sleep(5);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                /** Data Treatment for StandardControl */
                if (standardIsRunning) {
                    RunStopReverse = (byte)stRunStopReverse;
                    direction = (byte)stDirection;
                }

                if (myoIsRunning) {
                    RunStopReverse = androidcontrolinterfaceVariable.getMyoRunStopReverse();
                    direction = androidcontrolinterfaceVariable.getMyoDirection();
                }

                if(androidcontrolinterfaceVariable.getForceToStop()) {
                    RunStopReverse = (byte) 0x00;
                    direction = (byte) 0x00;
                }

                try {
    /** Make the package */
                    bytes = udpClientClass.intToByteArray(id, RunStopReverse, direction);
                    udpClientClass.setSendData(bytes);
                    udpClientClass.send(ipAddress, port);
                    Log.d("DEBUG", "Sent");
                } catch (IOException e) {
                    e.printStackTrace();
                }
    /** If android continue not to receive the response for 10 times, we break the loop, close the socket and stop the service */

                try {
                    if (!udpClientClass.receive(ipAddress, port)) {
                        Log.d("DBackgroundService", "Intent Service Failure for the reception");
                        counter++;
                        Log.d("BackgroundService", "Counter = "+counter);
                        if (counter == 10) {
                            Log.d("BackgroundService", "Counter = "+10);
                            errorUDP = true;
                            break;
                        }
                    } else {
                        Log.d("BackgroundService", "Counter reinitialisation");
                        counter = 0;
                        androidcontrolinterfaceVariable.setReceiveData(udpClientClass.getReceiveData());
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
                Log.d("BackgroundService", "Intent Service Success for the reception");
            }
        }

    /** If wee have to stop the intent service, the alert will be launched
     * and we will inform
     * */
        if(errorUDP) {
            try {
            udpClientClass.closeSocket();
            } catch (IOException e) {
                e.printStackTrace();
            }
    /** alert Part for Myo */
            if(myoIsRunning){
                alertPlayer.start();
                Log.d("ALERT", "Start");
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                if (alertPlayer.isPlaying()) {
                    alertPlayer.stop();
                    Log.d("ALERT", "Stop");
                }
            }
            /** If it has errors of connection, we will return to the interface ConnectActivity */
            connectActivity = new Intent(this, ConnectActivity.class);
            connectActivity.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            startActivity(connectActivity);
            androidcontrolinterfaceVariable.setIsToastDisconnected(true);
        }
    /** Stop the intent service */
        stopSelf();
        //unregister the sensor listener
        mSensorManager.unregisterListener(this);
        Log.d("Intent Service", "Stop");
        if(wakeLock.isHeld()) {
            wakeLock.release();
            Log.d("BackgroundService", "WakeLock Off");
            Log.d("BackgroundService", "MyoControlActivity onDestroy");
        }
    }

    @Override
    //If the task is finished, the intentService will be closed
    public void onDestroy() {
        super.onDestroy();
        Log.d("BackgroundService", "onDestroy");
        super.onDestroy();
        isRunning = false;
    }

    /** The function will execute automatically when the sensor get the changes of the events
     * @param event SensorEvent
     */
    @Override
    public void onSensorChanged(SensorEvent event){
        /** Get the orientation */
        System.arraycopy(event.values, 0, rotation, 0, 3);
        SensorManager.getRotationMatrixFromVector(rotationMatrix, rotation);
        SensorManager.getOrientation(rotationMatrix, orientation);

        int stRunStopReverseLocal = Math.round((float)Math.toDegrees(orientation[2])/90*256);
        int stDirectionLocal = -Math.round((float) Math.toDegrees(orientation[1]) / 180 * 256);

        androidcontrolinterfaceVariable = (androidcontrolinterface) getApplication();
        if(androidcontrolinterfaceVariable.getIsSensorInit()){
            stRunStopReverseInit = Math.round((float) Math.toDegrees(orientation[2])/90*256);
            androidcontrolinterfaceVariable.setIsSensorInit(false);
        }

        /** RunStopReverse Treatment */
        if(stRunStopReverseLocal - stRunStopReverseInit>=30 && stRunStopReverseLocal - stRunStopReverseInit <=254) stRunStopReverse = stRunStopReverseLocal - stRunStopReverseInit;
        else if(stRunStopReverseLocal - stRunStopReverseInit<-50 && stRunStopReverseLocal - stRunStopReverseInit >=-254) stRunStopReverse = 255;
        else stRunStopReverse = 0;

        /** Direction Traiment */
        if(stDirectionLocal<=128 && stDirectionLocal>-128) stDirection = stDirectionLocal;
        else stDirection = 0;
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        //Do nothing
    }
}


