package com.cgroup.androidvision;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

/**
 * Created by Teng on 2015/9/20.
 */

/** This is the UDP Client Class, by using this class, we can send and receive UDP data
 */
public class UDPClientClass{
    /** Variable Declaration */
    protected DatagramSocket clientSocket;
    protected byte[] receiveData = new byte[3];
    protected byte[] sendData = new byte[3];

    /** UDP Socket Constructor
     * @throws SocketException
     */
    public UDPClientClass() throws SocketException {
        clientSocket = new DatagramSocket();
    }

    /** By using the function client, we can sent a message
     * @param ip_address String
     * @param port  int
     * @throws IOException
     */
    public void send(String ip_address, int port) throws IOException {
        //send_data = sentence.getBytes();
        InetAddress IPAddress =  InetAddress.getByName(ip_address);

        DatagramPacket send_packet = new DatagramPacket(sendData,3, IPAddress, port);
        try {
            clientSocket.send(send_packet);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    /** By using the function receive, we can receive the message from the target
     * @param ip_address String
     * @param port  int
     * @return  boolean
     * @throws IOException
     */
    public boolean receive(String ip_address, int port) throws IOException {
        InetAddress IPAddress =  InetAddress.getByName(ip_address);
        DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
        boolean state;
        try {
            clientSocket.receive(receivePacket);
            state = true;
        } catch (IOException e) {
            e.printStackTrace();
            state = false;
        }
        return state;
    }

    /** Get the receive data
     * @return byte[]
     * @throws IOException
     */
    public byte[] getReceiveData() throws IOException{
        return receiveData;
    }

    /** Get the send data
     * @return byte[]
     * @throws IOException
     */
    public byte[] getSendData() throws IOException{
        return sendData;
    }

    /** Set the receive data
     * @param receiveData byte[]
     * @throws IOException
     */
    public void setReceiveData(byte[] receiveData) throws IOException{
        this.receiveData = receiveData;
    }

    /** Set the send data
     * @param sendData byte[]
     * @throws IOException
     */
    public void setSendData(byte[] sendData) throws IOException{
        this.sendData = sendData;
    }

    /** By using the function closeSocket, we can close the socket
     * @throws IOException
     */
    public void closeSocket() throws IOException {
        clientSocket.close();
    }

    /** By using this function we can build the package of 3 bytes
     * @param d1 int
     * @param d2 int
     * @param d3 int
     * @return byte[3]
     */
    public byte[] intToByteArray(int d1,int d2,int d3) {
        return new byte[]{
                (byte) d1,
                (byte) d2,
                (byte) d3};
    }

    /** Set the timeout for the UDP socket
     * @param time int
     * @throws SocketException
     */
    public void setSoTimeout(int time) throws SocketException {
        clientSocket.setSoTimeout(time);
    }
}


