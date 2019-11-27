package com.drchip.projectdeadman;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

public class Login extends AppCompatActivity {

    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;
    public static final String TOAST = "toast";

    public static final int CONNECTED_SUCCESS = 6;
    private MenuItem playMenu;
    private MenuItem DeviceType;


    @SuppressLint("HandlerLeak")
    private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {

                case MESSAGE_READ:
                    byte[] readBuf = (byte[]) msg.obj;
                    String readMessage = new String(readBuf, 0, msg.arg1);
                    if (readMessage.contains("STM") || readMessage.contains("RASP")) {

                        playMenu.setIcon(R.drawable.bluetooth_on);

                    }


                    Toast.makeText(Login.this, "Receibed " + readMessage, Toast.LENGTH_SHORT).show();
                    break;

                case MESSAGE_TOAST:
                    if (msg.getData().getString(TOAST).contains("Device connection was lost")) {
                        playMenu.setIcon(R.drawable.bluetooth_off);

                        Toast.makeText(Login.this, "Device was lost", Toast.LENGTH_SHORT).show();
                    }
                    Toast.makeText(getApplicationContext(), msg.getData().getString(TOAST),
                            Toast.LENGTH_SHORT).show();
                    break;
                case CONNECTED_SUCCESS:
                    ApplicationClass.sendMessage("O", Login.this);

                    ApplicationClass.sendMessage("<L>", Login.this);
                    break;
            }
        }
    };


    AutoCompleteTextView etNick;
    Button btnCancel, btnLogin;
    ImageView ivType;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);   //todo send signal bluetooth to rasp or stm and see who is connected.

        ActionBar actionBar = getSupportActionBar();
        actionBar.setTitle("Login");

        ApplicationClass.mBluetoothConnectionService.updateHandlerContex(mHandler);

        etNick =  findViewById(R.id.etNick);
        btnCancel = findViewById(R.id.btnCancel);
        btnLogin = findViewById(R.id.btnLogin);
        ivType = findViewById(R.id.ivType);

        if(ApplicationClass.deviceType.contains("STM"))
            ivType.setImageResource(R.drawable.stm);
        else if (ApplicationClass.deviceType.contains("RASP"))
            ivType.setImageResource(R.drawable.rasp);
        else ivType.setImageResource(R.drawable.not_knowned);

        String [] names = {"James","John", "Jenny","Pila","Jenine","Jack","PauloGay"};  // lista de nomes para aparecer na ajuda

        ArrayAdapter<String> adapter
                = new ArrayAdapter<String>(this, R.layout.custom_design_autocomlete,names);

        etNick.setThreshold(1);  //Numero de caraters que o utilizador percisa de por para começar a aparecer a funcao de autocomplete
        etNick.setAdapter(adapter);

        btnLogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //todo implement bluetooth method

                startActivity(new Intent(Login.this, MainActivity.class));

            }
        });

        btnCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                startActivity(new Intent(Login.this,Enter.class));
                Login.this.finish();
            }
        });
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();

        startActivity(new Intent(Login.this, Enter.class));
        Login.this.finish();
    }
@Override
    public boolean onCreateOptionsMenu( Menu menu ) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate( R.menu.login_register_menu, menu );
        playMenu = menu.findItem(R.id.BluetoothState);
        DeviceType = menu.findItem(R.id.DeviceTyepe);
        if(ApplicationClass.deviceConnected)
        {
            if(ApplicationClass.deviceType.contains("STM"))
                DeviceType.setIcon(R.drawable.stm);
            else if (ApplicationClass.deviceType.contains("RASP"))
                DeviceType.setIcon(R.drawable.rasp);
            else DeviceType.setIcon(R.drawable.not_knowned);
            playMenu.setIcon(R.drawable.bluetooth_on);
        }
        else
        {
            playMenu.setIcon(R.drawable.bluetooth_off);
        }
        return super.onCreateOptionsMenu(menu) ;
    }

    public boolean onOptionsItemSelected(@NonNull MenuItem item) {   // override metodos em code !!!

        switch (item.getItemId()) {

            case R.id.BluetoothState:
                if(ApplicationClass.deviceConnected)
                {
                    Toast.makeText(this, "Device is already connected", Toast.LENGTH_SHORT).show();
                }
                else
                {
                    BluetoothDevice device = ApplicationClass.BA.getRemoteDevice(ApplicationClass.target.getAddress());
                    // Attempt to connect to the device
                    ApplicationClass.mBluetoothConnectionService.connect(device);

                }
                break;
            case R.id.DeviceTyepe:

                Toast.makeText(this, "You are connected to an " + ApplicationClass.deviceType + " by the name " + ApplicationClass.target.getName(), Toast.LENGTH_SHORT).show();

                break;

        }
        return super.onOptionsItemSelected(item);

    }
}
