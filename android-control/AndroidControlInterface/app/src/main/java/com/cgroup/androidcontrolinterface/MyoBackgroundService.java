package com.cgroup.androidcontrolinterface;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;
import android.widget.Toast;

import com.thalmic.myo.AbstractDeviceListener;
import com.thalmic.myo.Arm;
import com.thalmic.myo.DeviceListener;
import com.thalmic.myo.Hub;
import com.thalmic.myo.Myo;
import com.thalmic.myo.Pose;
import com.thalmic.myo.Quaternion;
import com.thalmic.myo.XDirection;

/**
 * Created by Teng on 2015/9/21.
 */

/** Myo Background Service is a service which is used to get the messages from the Myo
 *  and it will send the command to Background Intent Service to control the vehicle
 */
public class MyoBackgroundService extends Service {
    /** Variable Declaration */
    androidcontrolinterface androidcontrolinterfaceVariable;
    protected boolean isRunning = false; //true for run and false for stop

    /** When the service is created, we will initialize the Myo hub and try to connect to the Myo
     */
    @Override
    public void onCreate() {
        super.onCreate();
        Log.d("MyoListener", "onCreate");
        /** Initialization of HUB */
        // First, we initialize the Hub singleton with an application identifier.
        Hub hub = Hub.getInstance();

        Log.d("MyoListener", "Hub.getInstance");
        if (!hub.init(this, getPackageName())) {
            Log.d("MyoListener", "Couldn't initialize Hub");
            Toast.makeText(getApplicationContext(), "Couldn't initialize Hub", Toast.LENGTH_SHORT).show();
            stopSelf();
            return;
        }

        // Disable standard Myo locking policy. All poses will be delivered.
        hub.setLockingPolicy(Hub.LockingPolicy.NONE);

        // Next, register for DeviceListener callbacks.
        hub.addListener(mListener);

        // Finally, scan for Myo devices and connect to the first one found that is very near.
        Log.d("MyoListener", "attachToAdjacentMyo start");

        Toast.makeText(getApplicationContext(), "Start to connet your Myo", Toast.LENGTH_SHORT).show();
        hub.attachToAdjacentMyo();
    }

    /** Myo Listener */
    // Classes that inherit from AbstractDeviceListener can be used to receive events from Myo devices.
    // If you do not override an event, the default behavior is to do nothing.
    private DeviceListener mListener = new AbstractDeviceListener() {
        // onConnect() is called whenever a Myo has been connected.
        @Override
        public void onConnect(Myo myo, long timestamp) {
            // Set the text color of the text view to cyan when a Myo connects.
            Log.d("MyoListener","onConnect");
            Toast.makeText(getApplicationContext(), "Myo Connected", Toast.LENGTH_SHORT).show();
        }

        // onDisconnect() is called whenever a Myo has been disconnected.
        @Override
        public void onDisconnect(Myo myo, long timestamp) {
            // Set the text color of the text view to red when a Myo disconnects.
            Log.d("MyoListener", "onDisconnect");
            Toast.makeText(getApplicationContext(), "Myo Disconnected", Toast.LENGTH_SHORT).show();
        }

        // onArmSync() is called whenever Myo has recognized a Sync Gesture after someone has put it on their
        // arm. This lets Myo know which arm it's on and which way it's facing.
        @Override
        public void onArmSync(Myo myo, long timestamp, Arm arm, XDirection xDirection) {
            Log.d("onArmSync", "onConnect");
//            if(myo.getArm() == Arm.LEFT){
//                Toast.makeText(getApplicationContext(), "Arm Left", Toast.LENGTH_SHORT).show();
//            }
//            else Toast.makeText(getApplicationContext(), "Arm Right", Toast.LENGTH_SHORT).show();
        }

        // onArmUnsync() is called whenever Myo has detected that it was moved from a stable position on a person's arm after
        // it recognized the arm. Typically this happens when someone takes Myo off of their arm, but it can also happen
        // when Myo is moved around on the arm.
        @Override
        public void onArmUnsync(Myo myo, long timestamp) {
            Log.d("MyoListener", "onArmUnsync");
            //Toast.makeText(getApplicationContext(), "onArmUnsync", Toast.LENGTH_SHORT).show();
        }

        // onUnlock() is called whenever a synced Myo has been unlocked. Under the standard locking
        // policy, that means poses will now be delivered to the listener.
        @Override
        public void onUnlock(Myo myo, long timestamp) {
            Log.d("MyoListener", "onUnlock");
            //Toast.makeText(getApplicationContext(), "onUnlock", Toast.LENGTH_SHORT).show();
        }

        // onLock() is called whenever a synced Myo has been locked. Under the standard locking
        // policy, that means poses will no longer be delivered to the listener.
        @Override
        public void onLock(Myo myo, long timestamp) {
            Log.d("MyoListener", "onLock");
            //Toast.makeText(getApplicationContext(), "onLock", Toast.LENGTH_SHORT).show();
        }

        // onOrientationData() is called whenever a Myo provides its current orientation,
        // represented as a quaternion.
        @Override
        public void onOrientationData(Myo myo, long timestamp, Quaternion rotation) {

                androidcontrolinterfaceVariable = (androidcontrolinterface) getApplication();
                // Calculate Euler angles (roll, pitch, and yaw) from the quaternion.
                float roll_float = (float) Math.toDegrees(Quaternion.roll(rotation))/180*256;
                float pitch_float = (float) Math.toDegrees(Quaternion.pitch(rotation))/90*256;

                //Transformation
                int roll = Math.round(roll_float);
                int pitch = Math.round(pitch_float);

                Log.d("roll", "roll = " + roll);
                Log.d("pitch","pitch = "+pitch);

            //if((pitch>=255 && pitch<510) || (pitch < -255 && pitch >= -510)) isRunning = true; // If the Fingerspread doesn't work for starting the car.
            if(isRunning) {
                /** For roll */
                if(roll <= 127 && roll >= -127) androidcontrolinterfaceVariable.setMyoDirection((byte) -roll);
                else if(roll > 128 && roll < 255) androidcontrolinterfaceVariable.setMyoDirection((byte) -127); //right
                else if(roll < -127 && roll > -255) androidcontrolinterfaceVariable.setMyoDirection((byte)127);//left

                /** For pitch */
                if (pitch >= -30*256/90 && pitch <= 30*256/90) androidcontrolinterfaceVariable.setMyoRunStopReverse((byte)(155+pitch)); // Speed from 0 to 254
                else if(pitch<-60*256/90 && pitch>=-180*256/90) androidcontrolinterfaceVariable.setMyoRunStopReverse((byte)255); //Reverse = 255
                else androidcontrolinterfaceVariable.setMyoRunStopReverse((byte) 0); //Stop the vehicle
            }
            else {
                androidcontrolinterfaceVariable.setMyoRunStopReverse((byte) 0);
                androidcontrolinterfaceVariable.setMyoDirection((byte) 0);
            }
        }

        // onPose() is called whenever a Myo provides a new pose.
        @Override
        public void onPose(Myo myo, long timestamp, Pose pose) {
            androidcontrolinterfaceVariable = (androidcontrolinterface) getApplication();
            Log.d("MyoListener","onPose");
            // Handle the cases of the Pose enumeration, and change the text of the text view
            // based on the pose we receive.
            switch (pose) {
                case UNKNOWN:
                    //Toast.makeText(getApplicationContext(), "UNHNOWN", Toast.LENGTH_SHORT).show();
                    break;
                case REST:
                case DOUBLE_TAP:
                    switch (myo.getArm()) {
                        case LEFT:
                            //Toast.makeText(getApplicationContext(), "Arm Left", Toast.LENGTH_SHORT).show();
                            break;
                        case RIGHT:
                            //Toast.makeText(getApplicationContext(), "Arm Right", Toast.LENGTH_SHORT).show();
                            break;
                    }
                    break;
                case FIST:
//                    isRunning = false;
//                    androidcontrolinterfaceVariable.setMyoRunStopReverse((byte) 0);
//                    androidcontrolinterfaceVariable.setMyoDirection((byte) 0);
                    //Toast.makeText(getApplicationContext(), "FIST", Toast.LENGTH_SHORT).show();
                    break;
                case WAVE_IN:
                    isRunning = !isRunning;
                    if(!isRunning){
                        androidcontrolinterfaceVariable.setMyoRunStopReverse((byte) 0);
                        androidcontrolinterfaceVariable.setMyoDirection((byte) 0);
                        Log.d("Myo Control","Myo Stop");
                    }
                    //Toast.makeText(getApplicationContext(), "WAVE IN", Toast.LENGTH_SHORT).show();
                    break;
                case WAVE_OUT:
                    //Toast.makeText(getApplicationContext(), "WAVE OUT", Toast.LENGTH_SHORT).show();
                    break;
                case FINGERS_SPREAD:
                    isRunning = true;
                    //Toast.makeText(getApplicationContext(), "FINGERS_SPREAD", Toast.LENGTH_SHORT).show();
                    break;
            }

            if (pose != Pose.UNKNOWN && pose != Pose.REST) {
                // Tell the Myo to stay unlocked until told otherwise. We do that here so you can
                // hold the poses without the Myo becoming locked.
                myo.unlock(Myo.UnlockType.HOLD);

                // Notify the Myo that the pose has resulted in an action, in this case changing
                // the text on the screen. The Myo will vibrate.
                myo.notifyUserAction();
            } else {
                // Tell the Myo to stay unlocked only for a short period. This allows the Myo to
                // stay unlocked while poses are being performed, but lock after inactivity.
                myo.unlock(Myo.UnlockType.TIMED);
            }
        }
    };

    @Override
    public IBinder onBind(Intent intent) {
        //TODO: Nothng
        return null;
    }

    /** When the Myo service is destroyed, we will shutdown the Hub Listener
     */
    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.d("MyoListener", "onDestroy");
        // We don't want any callbacks when the Service is gone, so unregister the listener.
        Hub.getInstance().removeListener(mListener);
        Hub.getInstance().shutdown();
        stopSelf();
    }
}
