package com.drchip.projectdeadman;

import android.bluetooth.BluetoothDevice;

public class MybluetoothDevice {
    public BluetoothDevice device;
    public String deviceType;

    public MybluetoothDevice(BluetoothDevice device, String deviceType) {
        this.device = device;
        this.deviceType = deviceType;
    }
}
