package com.drchip.projectdeadman;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.app.DatePickerDialog;
import android.bluetooth.BluetoothDevice;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.Toast;

import java.util.Calendar;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

public class Register extends AppCompatActivity {

    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;
    public static final String TOAST = "toast";
    private MenuItem playMenu;
    private MenuItem DeviceType;

    public static final int CONNECTED_SUCCESS = 6;
    EditText etDate, etMessage, etPlatform;
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

                    Toast.makeText(Register.this, "Receibed " + readMessage, Toast.LENGTH_SHORT).show();
                    break;

                case MESSAGE_TOAST:
                    if (msg.getData().getString(TOAST).contains("Device connection was lost")) {
                        playMenu.setIcon(R.drawable.bluetooth_off);

                        Toast.makeText(Register.this, "Device was lost", Toast.LENGTH_SHORT).show();
                    }
                    Toast.makeText(getApplicationContext(), msg.getData().getString(TOAST),
                            Toast.LENGTH_SHORT).show();
                    break;
                case CONNECTED_SUCCESS:
                    ApplicationClass.sendMessage("O", Register.this);

                    ApplicationClass.sendMessage("<L>", Register.this);
                    break;
            }
        }
    };
    Button btnCreate;

    @Override
    public void onBackPressed() {
        super.onBackPressed();

        startActivity(new Intent(Register.this, Enter.class));
        Register.this.finish();
    }
    DatePickerDialog datePickerDialog;

    @SuppressLint("ClickableViewAccessibility")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register);
        ActionBar actionBar = getSupportActionBar();
        actionBar.setTitle("Register");



        etDate= findViewById(R.id.etDate);
        etMessage = findViewById(R.id.etMessage);
        etPlatform = findViewById(R.id.etPlatform);
        btnCreate = findViewById(R.id.btnCreate);

        ApplicationClass.mBluetoothConnectionService.updateHandlerContex(mHandler);






        etMessage.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {

                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    AlertDialog.Builder message = new AlertDialog.Builder(Register.this);
                    LayoutInflater inflater = getLayoutInflater();
                    final View dialogView = inflater.inflate(R.layout.message_dialog, null);
                    final EditText etReleaseMessage = dialogView.findViewById(R.id.etReleaseMessage);

                    message.setView(dialogView);
                    message.setTitle("Message to Release");
                    message.setMessage("Enter the message that you want to release here!!");

                    message.setPositiveButton("Confirm", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                            etMessage.setText(etReleaseMessage.getText());
                        }
                    });
                    message.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                        }
                    });
                    message.show();
                }

                return false;
            }
        });

        etDate.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    final Calendar c = Calendar.getInstance();
                    int mYear = c.get(Calendar.YEAR); // current year
                    int mMonth = c.get(Calendar.MONTH); // current month
                    int mDay = c.get(Calendar.DAY_OF_MONTH); // current day
                    // date picker dialog
                    datePickerDialog = new DatePickerDialog(Register.this,
                            new DatePickerDialog.OnDateSetListener() {

                                @Override
                                public void onDateSet(DatePicker view, int year,
                                                      int monthOfYear, int dayOfMonth) {
                                    // set day of month , month and year value in the edit text
                                    etDate.setText(dayOfMonth + "/"
                                            + (monthOfYear + 1) + "/" + year);

                                }
                            }, mYear, mMonth, mDay);
                    datePickerDialog.show();
                }


                return false;
            }
        });


        etPlatform.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {

                if (event.getAction() == MotionEvent.ACTION_DOWN) {


                    final AlertDialog.Builder message = new AlertDialog.Builder(Register.this);
                    LayoutInflater inflater = getLayoutInflater();
                    final View dialogView = inflater.inflate(R.layout.platform_dialog, null);
                    //   final EditText etReleaseMessage = dialogView.findViewById(R.id.etReleaseMessage);
                    final RadioButton rbSMS = dialogView.findViewById(R.id.rbSMS);
                    final RadioButton rbEmail = dialogView.findViewById(R.id.rbEmail);
                    final EditText etEmail = dialogView.findViewById(R.id.etMail);
                    final EditText etPhone = dialogView.findViewById(R.id.etPhone);


                    message.setView(dialogView);
                    message.setTitle("Platform to Release");
                    message.setMessage("Enter the platform that you want to release here!!");


                    rbSMS.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            etPhone.setVisibility(View.VISIBLE);
                            etEmail.setVisibility(View.GONE);
                        }
                    });

                    rbEmail.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            etEmail.setVisibility(View.VISIBLE);
                            etPhone.setVisibility(View.GONE);

                        }
                    });

                    message.setPositiveButton("Confirm", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                            if (rbEmail.isChecked()) {
                                if (etEmail.getText().toString().isEmpty()) {
                                    Toast.makeText(Register.this, "Please put an email!", Toast.LENGTH_SHORT).show();
                                    dialog.cancel();
                                } else {
                                    etPlatform.setText("Email " + etEmail.getText().toString().trim());
                                }
                            } else if (rbSMS.isChecked()) {
                                if (etPhone.getText().toString().isEmpty()) {
                                    Toast.makeText(Register.this, "Please put an phone number!", Toast.LENGTH_SHORT).show();
                                    dialog.cancel();

                                } else {
                                    etPlatform.setText("SMS " + etPhone.getText().toString().trim());
                                }
                            } else {
                                Toast.makeText(Register.this, "Please select an option!!", Toast.LENGTH_SHORT).show();
                                dialog.cancel();
                            }

                        }
                    });
                    message.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                        }
                    });
                    message.show();

                    return false;
                }
                return false;

            }
        });

        btnCreate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                //ApplicationClass.sendMessage("<L>", Register.this);
                //startActivityForResult(new Intent(Register.this, UserInstructionsRASP.class), 1);
                startActivityForResult(new Intent(Register.this, UserInstuctionsSTM.class), 1);


            }
        });


    }
    @Override
    public boolean onCreateOptionsMenu( Menu menu ) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate( R.menu.login_register_menu, menu );
        playMenu = menu.findItem(R.id.BluetoothState);
        DeviceType = menu.findItem(R.id.DeviceTyepe);
        if (ApplicationClass.deviceConnected) {
            if (ApplicationClass.deviceType.contains("STM"))
                DeviceType.setIcon(R.drawable.stm);
            else if (ApplicationClass.deviceType.contains("RASP"))
                DeviceType.setIcon(R.drawable.rasp);
            else DeviceType.setIcon(R.drawable.not_knowned);
            playMenu.setIcon(R.drawable.bluetooth_on);
        } else {
            playMenu.setIcon(R.drawable.bluetooth_off);
        }
        if(ApplicationClass.deviceConnected)
        {
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
