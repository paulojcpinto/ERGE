package com.drchip.projectdeadman;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.app.Application;
import android.app.DatePickerDialog;
import android.bluetooth.BluetoothDevice;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.Editable;
import android.text.TextWatcher;
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

import com.google.android.material.snackbar.Snackbar;

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
                    receive_nick(readMessage);
                    receive_pincode(readMessage);
                    receive_email(readMessage);
                    receive_password(readMessage);
                    receive_phone_number(readMessage);
                    receive_message(readMessage);
                    receive_repeatTime(readMessage);
                    receive_dateStart(readMessage);
                    receive_platform(readMessage);
                   // Toast.makeText(Register.this, "Receibed " + readMessage, Toast.LENGTH_SHORT).show();
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
    EditText etDate, etMessage, etPlatform, etNickName,etPinCode,etEmail,etEmailPassword,etPhoneNumber,etRepeatTime;

    @SuppressLint("ClickableViewAccessibility")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register);
        ActionBar actionBar = getSupportActionBar();
        actionBar.setTitle("Register");



        etNickName = findViewById(R.id.etNickName);
        etPinCode = findViewById(R.id.etPinCode);
        etEmail = findViewById(R.id.etEmail);
        etEmailPassword = findViewById(R.id.etEmailPassword);
        etPhoneNumber = findViewById(R.id.etPhoneNumber);
        etRepeatTime = findViewById(R.id.etRepeatTime);
        etDate= findViewById(R.id.etDateToStart);
        etMessage = findViewById(R.id.etMessage);
        etPlatform = findViewById(R.id.etPlatform);
        btnCreate = findViewById(R.id.btnCreate);

        ApplicationClass.mBluetoothConnectionService.updateHandlerContex(mHandler);






        etPinCode.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {

                if(etPinCode.getText().length() >4 )
                {
                    StringBuilder aux = new StringBuilder();
                    for(int i =0; i<4 ;i++)
                        aux.append(etPinCode.getText().toString().toCharArray()[i]);
                    etPinCode.setText(aux);
                    Toast.makeText(Register.this, "The pin code must have 4 numbers only", Toast.LENGTH_SHORT).show();
                }


            }


            @Override
            public void afterTextChanged(Editable s) {
               // Toast.makeText(Register.this, "Worked", Toast.LENGTH_SHORT).show();

            }
        });
        etPhoneNumber.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                if(etPhoneNumber.getText().toString().contains("+")) {
                    if (etPhoneNumber.getText().length() > 13) {
                        StringBuilder aux = new StringBuilder();
                        for (int i = 0; i < 13; i++)
                            aux.append(etPhoneNumber.getText().toString().toCharArray()[i]);
                        etPhoneNumber.setText(aux);
                        Toast.makeText(Register.this, "The phone number is at max lengh", Toast.LENGTH_SHORT).show();
                    }
                }else {
                        if (etPhoneNumber.getText().length() > 9) {
                            StringBuilder aux = new StringBuilder();
                            for (int i = 0; i < 9; i++)
                                aux.append(etPhoneNumber.getText().toString().toCharArray()[i]);
                            etPhoneNumber.setText(aux);
                            Toast.makeText(Register.this, "The phone number is at max lengh", Toast.LENGTH_SHORT).show();
                        }
                    }

            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });
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
                /*if(etNickName.getText().toString().isEmpty()) {

                    AlertDialog.Builder message = new AlertDialog.Builder(Register.this);

                    message.setTitle("Error!");
                    message.setMessage("Please make sure you choose an NICKNAME!!!");

                    message.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                        }
                    });
                    message.show();
                }else
                if(etPinCode.getText().toString().isEmpty())
                {
                    AlertDialog.Builder message = new AlertDialog.Builder(Register.this);

                    message.setTitle("Error!");
                    message.setMessage("Please make sure you choose an PINCODE!!!");

                    message.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                        }
                    });
                    message.show();

                }else
                if(etEmail.getText().toString().isEmpty())
                {
                    AlertDialog.Builder message = new AlertDialog.Builder(Register.this);

                    message.setTitle("Error!");
                    message.setMessage("Please make sure you choose an EMAIL that will send the emails!!!");

                    message.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                        }
                    });
                    message.show();

                }else
                if(etEmailPassword.getText().toString().isEmpty())
                {
                    AlertDialog.Builder message = new AlertDialog.Builder(Register.this);

                    message.setTitle("Error!");
                    message.setMessage("Please make sure you enter the PASSWORD to the email enter before!!!");

                    message.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                        }
                    });
                    message.show();
                }else

                if(etPhoneNumber.getText().toString().isEmpty())
                {
                    AlertDialog.Builder message = new AlertDialog.Builder(Register.this);

                    message.setTitle("Error!");
                    message.setMessage("Please make sure you choose a PHONE NUMBER to receive the sms messages!!!");

                    message.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                        }
                    });
                    message.show();
                }else
                if(etMessage.getText().toString().isEmpty())
                {
                    AlertDialog.Builder message = new AlertDialog.Builder(Register.this);

                    message.setTitle("Error!");
                    message.setMessage("Please make sure you choose an MESSAGE to release!!!");

                    message.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                        }
                    });
                    message.show();
                }else
                if(etRepeatTime.getText().toString().isEmpty())
                {
                    AlertDialog.Builder message = new AlertDialog.Builder(Register.this);

                    message.setTitle("Error!");
                    message.setMessage("Please make sure you choose an REPEAT TIME to do the presence check!!!");

                    message.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                        }
                    });
                    message.show();
                }else

                if(etDate.getText().toString().isEmpty())
                {
                    AlertDialog.Builder message = new AlertDialog.Builder(Register.this);

                    message.setTitle("Error!");
                    message.setMessage("Please make sure you choose an DATE to start doing the presence check!!!");

                    message.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                        }
                    });
                    message.show();
                }
                else
                if(etPlatform.getText().toString().isEmpty())
                {
                    AlertDialog.Builder message = new AlertDialog.Builder(Register.this);

                    message.setTitle("Error!");
                    message.setMessage("Please make sure you choose an PLATFORM to do the release!!!");

                    message.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                        }
                    });
                    message.show();
                }
                  else if(etPinCode.getText().length()<4)
                {
                    AlertDialog.Builder message = new AlertDialog.Builder(Register.this);

                    message.setTitle("Error!");
                    message.setMessage("Please make sure your PINCODE have exactly 4 numbers!!!");

                    message.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                        }
                    });
                    message.show();
                }else
                    if(!etEmail.getText().toString().contains("@"))
                    {
                        AlertDialog.Builder message = new AlertDialog.Builder(Register.this);

                        message.setTitle("Error!");
                        message.setMessage("Your email is invalid, please write it correctly!!!!");

                        message.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {

                            }
                        });
                        message.show();
                    }

                    else if((etPhoneNumber.getText().length()<9 && !etPhoneNumber.getText().toString().contains("+"))|| (etPhoneNumber.getText().length()<13 && etPhoneNumber.getText().toString().contains("+")))
                    {
                        AlertDialog.Builder message = new AlertDialog.Builder(Register.this);

                        message.setTitle("Error!");
                        message.setMessage("Please make sure you enter an correct phone number!!!");

                        message.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int whicaccch) {

                            }
                        });
                        message.show();
                    }else*/

                ApplicationClass.sendMessage("<S"+etNickName.getText().toString().trim()+">", Register.this);
               // startActivityForResult(new Intent(Register.this, UserInstructionsRASP.class), 1);
                //startActivityForResult(new Intent(Register.this, UserInstuctionsSTM.class), 1);


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
                    ApplicationClass.mBluetoothConnectionService.stop();
                    Toast.makeText(this, "Closing connection", Toast.LENGTH_SHORT).show();
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
    void receive_nick(String readMessage)
    {
        if(readMessage.contains("<S") && readMessage.contains((">")))
        {
            StringBuilder aux = new StringBuilder();
            for (int i =2; i<readMessage.length()-1;i++)
            {
                aux.append(readMessage.charAt(i));
            }
            int lengh=0;
            try {
                lengh  = Integer.parseInt(aux.toString());
            }catch (NumberFormatException nfe)
            {
                Toast.makeText(this, "Error Parsing Number: "+nfe.getMessage(), Toast.LENGTH_SHORT).show();
            }
            if(lengh !=etNickName.getText().toString().trim().length())
            {
                Toast.makeText(Register.this, "Error Sending Nickname:"+ lengh + "Suposto:" + etNickName.getText().toString().trim().length() + "With message: "+ readMessage, Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<S"+etNickName.getText().toString().trim()+">", Register.this);

            }else
            {
                Toast.makeText(Register.this, "Sended nickname", Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<P"+etPinCode.getText().toString().trim()+">", Register.this);


            }

        }
    }
    void receive_pincode(String readMessage)
    {


        if(readMessage.contains("<P") && readMessage.contains((">"))) {
            StringBuilder aux = new StringBuilder();
            for (int i = 2; i < readMessage.length()-1; i++) {
                aux.append(readMessage.charAt(i));
            } int lengh=0;
            try {
              lengh  = Integer.parseInt(aux.toString());
            }catch (NumberFormatException nfe)
            {
                Toast.makeText(this, "Error Parsing Number: "+nfe.getMessage(), Toast.LENGTH_SHORT).show();
            }

            if (lengh != etPinCode.getText().toString().length()) {
                Toast.makeText(Register.this, "Error Sending pincode, Receibed: "+lengh+" suposed:"+etPinCode.length(), Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<P" + etPinCode.getText().toString().trim() + ">", Register.this);

            } else {
                Toast.makeText(Register.this, "Sended pincode", Toast.LENGTH_SHORT).show();
                 ApplicationClass.sendMessage("<M"+etEmail.getText().toString().trim()+">", Register.this);

            }

        }
    }

    void receive_email(String readMessage)
    {
        if(readMessage.contains("<M") && readMessage.contains((">"))) {

            StringBuilder aux = new StringBuilder();
            for (int i = 2; i < readMessage.length() - 1; i++) {
                aux.append(readMessage.charAt(i));
            }
            int lengh = 0;
            try {
                lengh = Integer.parseInt(aux.toString());
            } catch (NumberFormatException nfe) {
                Toast.makeText(this, "Error Parsing Number: " + nfe.getMessage(), Toast.LENGTH_SHORT).show();
            }
            if (lengh != etEmail.getText().toString().trim().length()) {
                Toast.makeText(Register.this, "<Error sending email, receibed: " + lengh + " instead of:" + etEmail.getText().toString().trim().length(), Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<M" + etEmail.getText().toString().trim() + ">", Register.this);

            } else {
                Toast.makeText(Register.this, "Sended email", Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<X"+etEmailPassword.getText().toString().trim()+">", Register.this);

            }
        }
    }

    void receive_password(String readMessage)
    {
        if(readMessage.contains("<X") && readMessage.contains((">"))) {

            StringBuilder aux = new StringBuilder();
            for (int i = 2; i < readMessage.length() - 1; i++) {
                aux.append(readMessage.charAt(i));
            }
            int lengh = 0;
            try {
                lengh = Integer.parseInt(aux.toString());
            } catch (NumberFormatException nfe) {
                Toast.makeText(this, "Error Parsing Number: " + nfe.getMessage(), Toast.LENGTH_SHORT).show();
            }
            if (lengh != etEmailPassword.getText().toString().trim().length()) {
                Toast.makeText(Register.this, "<Error sending email password, receibed: " + lengh + " instead of:" + etEmailPassword.getText().toString().trim().length(), Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<X" + etEmail.getText().toString().trim() + ">", Register.this);

            } else {
                Toast.makeText(Register.this, "Sended email email password", Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<T"+etPhoneNumber.getText().toString().trim()+">", Register.this);

            }
        }
    }

    void receive_phone_number(String readMessage)
    {
        if(readMessage.contains("<T") && readMessage.contains((">"))) {

            StringBuilder aux = new StringBuilder();
            for (int i = 2; i < readMessage.length() - 1; i++) {
                aux.append(readMessage.charAt(i));
            }
            int lengh = 0;
            try {
                lengh = Integer.parseInt(aux.toString());
            } catch (NumberFormatException nfe) {
                Toast.makeText(this, "Error Parsing Number: " + nfe.getMessage(), Toast.LENGTH_SHORT).show();
            }
            if (lengh != etPhoneNumber.getText().toString().trim().length()) {
                Toast.makeText(Register.this, "<Error sending phone number, receibed: " + lengh + " instead of:" + etPhoneNumber.getText().toString().trim().length(), Toast.LENGTH_SHORT).show();
                 ApplicationClass.sendMessage("<T" + etPhoneNumber.getText().toString().trim() + ">", Register.this);

            } else {
                Toast.makeText(Register.this, "Sended phone number", Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<R"+etMessage.getText().toString().trim()+">", Register.this);

            }
        }
    }

    void receive_message(String readMessage) {
        if (readMessage.contains("<R") && readMessage.contains((">"))) {

            StringBuilder aux = new StringBuilder();
            for (int i = 2; i < readMessage.length() - 1; i++) {
                aux.append(readMessage.charAt(i));
            }
            int lengh = 0;
            try {
                lengh = Integer.parseInt(aux.toString());
            } catch (NumberFormatException nfe) {
                Toast.makeText(this, "Error Parsing Number: " + nfe.getMessage(), Toast.LENGTH_SHORT).show();
            }
            if (lengh != etMessage.getText().toString().trim().length()) {
                Toast.makeText(Register.this, "<Error sending message to release, receibed: " + lengh + " instead of:" + etMessage.getText().toString().trim().length(), Toast.LENGTH_SHORT).show();
                 ApplicationClass.sendMessage("<R" + etMessage.getText().toString().trim() + ">", Register.this);

            } else {
                Toast.makeText(Register.this, "Sended message to release", Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<O"+etRepeatTime.getText().toString().trim()+">", Register.this);

            }
        }
    }

    void receive_repeatTime(String readMessage)
    {
        if (readMessage.contains("<O") && readMessage.contains((">"))) {

        StringBuilder aux = new StringBuilder();
        for (int i = 2; i < readMessage.length() - 1; i++) {
            aux.append(readMessage.charAt(i));
        }
        int lengh = 0;
        try {
            lengh = Integer.parseInt(aux.toString());
        } catch (NumberFormatException nfe) {
            Toast.makeText(this, "Error Parsing Number: " + nfe.getMessage(), Toast.LENGTH_SHORT).show();
        }
        if (lengh != etRepeatTime.getText().toString().trim().length()) {
            Toast.makeText(Register.this, "Error sending repeat time, receibed: " + lengh + " instead of:" + etRepeatTime.getText().toString().trim().length(), Toast.LENGTH_SHORT).show();
             ApplicationClass.sendMessage("<O" + etRepeatTime.getText().toString().trim() + ">", Register.this);

        } else {
            Toast.makeText(Register.this, "Sended repeat time", Toast.LENGTH_SHORT).show();
            ApplicationClass.sendMessage("<D"+etDate.getText().toString().trim()+">", Register.this);

        }
    }
    }
    void receive_dateStart(String readMessage)
    {
        if (readMessage.contains("<D") && readMessage.contains((">"))) {

            StringBuilder aux = new StringBuilder();
            for (int i = 2; i < readMessage.length() - 1; i++) {
                aux.append(readMessage.charAt(i));
            }
            int lengh = 0;
            try {
                lengh = Integer.parseInt(aux.toString());
            } catch (NumberFormatException nfe) {
                Toast.makeText(this, "Error Parsing Number: " + nfe.getMessage(), Toast.LENGTH_SHORT).show();
            }
            if (lengh != etDate.getText().toString().trim().length()) {
                Toast.makeText(Register.this, "Error sending Date to start, receibed: " + lengh + " instead of:" + etDate.getText().toString().trim().length(), Toast.LENGTH_SHORT).show();
                 ApplicationClass.sendMessage("<D" + etDate.getText().toString().trim() + ">", Register.this);

            } else {
                Toast.makeText(Register.this, "Sended Date to start!", Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<A"+etPlatform.getText().toString().trim()+">", Register.this);

            }
        }
    }
    void receive_platform(String readMessage)
    {
        if (readMessage.contains("<A") && readMessage.contains((">"))) {

            StringBuilder aux = new StringBuilder();
            for (int i = 2; i < readMessage.length() - 1; i++) {
                aux.append(readMessage.charAt(i));
            }
            int lengh = 0;
            try {
                lengh = Integer.parseInt(aux.toString());
            } catch (NumberFormatException nfe) {
                Toast.makeText(this, "Error Parsing Number: " + nfe.getMessage(), Toast.LENGTH_SHORT).show();
            }
            if (lengh != etPlatform.getText().toString().trim().length()) {
                Toast.makeText(Register.this, "<Error sending Date to start, receibed: " + lengh + " instead of:" + etPlatform.getText().toString().trim().length(), Toast.LENGTH_SHORT).show();
                 ApplicationClass.sendMessage("<A" + etPlatform.getText().toString().trim() + ">", Register.this);

            } else {
                Toast.makeText(Register.this, "Sended Platform to release!", Toast.LENGTH_SHORT).show();
                final AlertDialog.Builder message = new AlertDialog.Builder(Register.this);
                LayoutInflater inflater = getLayoutInflater();
                final View dialogView = inflater.inflate(R.layout.success_login_layout, null);
                //   final EditText etReleaseMessage = dialogView.findViewById(R.id.etReleaseMessage);
                message.setView(dialogView);
                message.setTitle("Register Completed");
                message.setMessage("You successfully register an new user!!");
                message.setPositiveButton("Nice", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        if(ApplicationClass.deviceType.contains("STM"))
                        startActivity(new Intent(Register.this, UserInstuctionsSTM.class));
                        else if(ApplicationClass.deviceType.contains("RASP"))
                            startActivity(new Intent(Register.this, UserInstructionsRASP.class));
                        Register.this.finish();
                        //startActivityForResult(new Intent(Register.this, UserInstuctionsSTM.class), 1);

                    }
                });
                message.show();

            }
        }
    }



}
