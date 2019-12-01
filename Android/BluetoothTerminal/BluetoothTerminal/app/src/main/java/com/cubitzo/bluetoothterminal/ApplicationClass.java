package com.cubitzo.bluetoothterminal;

import android.app.Application;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.os.Handler;
import android.view.MenuItem;
import android.widget.Toast;

public class ApplicationClass extends Application {

    public static int connectedTo;
    public static BluetoothAdapter BA;
    public static BluetoothDevice target;
    public static BluetoothConnectionService mBluetoothConnectionService;
    public static String deviceType;
    public static boolean deviceConnected;
    public static MenuItem DeviceType;
    public static MenuItem playMenu;

    private final Handler mHandler = new Handler();

    public static void sendMessage(String message, Context context) {

        // Check that we're actually connected before trying anything
        if (mBluetoothConnectionService.getState() != BluetoothConnectionService.STATE_CONNECTED) {
            Toast.makeText(context, "not connected", Toast.LENGTH_SHORT).show();
            return;
        }
        // Check that there's actually something to send
        if (message.length() > 0) {

            // Get the message bytes and tell the BluetoothChatService to write
            byte[] send = message.getBytes();
            mBluetoothConnectionService.write(send);
            // Reset out string buffer to zero and clear the edit text field
            // mOutStringBuffer.setLength(0);
            //mOutEditText.setText(mOutStringBuffer);
        }
    }

    public void onCreate() {
        super.onCreate();
        deviceConnected = false;
        BA = BluetoothAdapter.getDefaultAdapter();

        ApplicationClass.mBluetoothConnectionService = new BluetoothConnectionService(this, mHandler);

        ApplicationClass.mBluetoothConnectionService.start();


    }

}
