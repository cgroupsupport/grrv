package com.cgroup.androidcontrolinterface;

import android.app.Application;

/**
 * Created by Teng on 2015/9/23.
 */

/** The application, with witch we read and write the global variable
 */
public class androidcontrolinterface extends Application {
    /** Variable Declaration */
    protected boolean standardIsRunning; //enable or disable standard mode
    protected boolean myoIsRunning; //enable or disable myo mode
    protected boolean isRunning; //run or stop the vehicle
    protected boolean forceToStop; //true: force the vehicle to stop
    protected byte myoRunStopReverse; //run, stop, reverse and speed control byte to be inserted in the package to send to the raspberry pi
    protected byte myoDirection; //direction control byte to be inserted in the package to send to the raspberry pi
    protected boolean isSensorInit; //enable or disable the sensor initialization before beginning the standard control mode
    protected boolean isToastDisconnected;
    protected byte[] receiveData = new byte[3];

    /** Get the standard mode state
     * @return boolean
     */
    public boolean getStandardIsRunning() {
        return standardIsRunning;
    }

    /** Enable or disable the standard mode
     * @param standardIsRunning boolean
     */
    public void setStandardIsRunning(boolean standardIsRunning) {
        this.standardIsRunning = standardIsRunning;
    }

    /** Get the Myo mode state
     * @return boolean
     */
    public boolean getMyoIsRunning() {
        return myoIsRunning;
    }

    /** Enable or disable the Myo mode
     * @param myoIsRunning boolean
     */
    public void setMyoIsRunning(boolean myoIsRunning) {
        this.myoIsRunning = myoIsRunning;
    }

    /** Get the BackgroundIntentService state
     * @return boolean
     */
    public boolean getIsRunning() {
        return isRunning;
    }

    /** Enable or disable the BackgroundIntentService
     * @param isRunning boolean
     */
    public void setIsRunning(boolean isRunning) {
        this.isRunning = isRunning;
    }

    /** Make sure if we stop the car
     * @return boolean
     */
    public boolean getForceToStop() {
        return forceToStop;
    }

    /** Take true to stop the car
     * @param forceToStop boolean
     */
    public void setForceToStop(boolean forceToStop) {
        this.forceToStop = forceToStop;
    }

    /** Get the Myo speed command byte
     * @return byte
     */
    public byte getMyoRunStopReverse() {
        return myoRunStopReverse;
    }

    /** Get the Myo direction command byte
     * @return byte
     */
    public byte getMyoDirection() {
        return myoDirection;
    }

    /** Set the Myo Speed command data
     * @param myoRunStopReverse byte
     */
    public void setMyoRunStopReverse(byte myoRunStopReverse) {
        this.myoRunStopReverse = myoRunStopReverse;
    }

    /** Set the Myo direction command data
     * @param myoDirection byte
     */
    public void setMyoDirection(byte myoDirection) {
        this.myoDirection = myoDirection;
    }

    /** Enable or diable the sensor reference initialization
     * @param isSensorInit boolean
     */
    public void setIsSensorInit(boolean isSensorInit) {
        this.isSensorInit = isSensorInit;
    }

    /** Make sure if we need to initialize the sensor reference
     * @return boolean
     */
    public boolean getIsSensorInit() {
        return isSensorInit;
    }

    /** Get the disconnected state
     * @return boolean
     */
    public boolean getIsToastDisconnected() {
        return isToastDisconnected;
    }

    /** Enable or disable the disconnected toast
     * @param isToastDisconnected boolean
     */
    public void setIsToastDisconnected(boolean isToastDisconnected) {
        this.isToastDisconnected = isToastDisconnected;
    }

    /** Get the receive data
     * @return receiveData byte[3]
     */
    public byte[] getReceiveData() {
        return receiveData;
    }

    /** Set the receive data
     * @param receiveData byte[3]
     */
    public void setReceiveData(byte[] receiveData) {
        this.receiveData = receiveData;
    }
}
