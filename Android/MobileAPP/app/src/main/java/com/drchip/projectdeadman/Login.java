package com.drchip.projectdeadman;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.bluetooth.BluetoothDevice;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.ArrayList;

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
    private ArrayList<String> users;
int numbers=0;

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
                    receive_login(readMessage);
                    if(readMessage.contains("<K>"))
                    {
                        numbers++;
                        ivWait.clearAnimation();
                        ivWait.startAnimation(fade_in3);
                        tvNumberCounter.setText(numbers  + "");
                        ivWait.setImageResource(R.drawable.correct);

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
    EditText etPinCode;
    Button btnCancel, btnLogin;
    ImageView ivType;
    Animation rotate;
    ImageView ivWait;
    TextView tvNumberCounter;
    Animation fade_in3;
    Animation fade_out3;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);   //todo send signal bluetooth to rasp or stm and see who is connected.

        ActionBar actionBar = getSupportActionBar();
        actionBar.setTitle("Login");

        ApplicationClass.mBluetoothConnectionService.updateHandlerContex(mHandler);
        users = new ArrayList<>();
        loadData();
        etNick =  findViewById(R.id.etNick);
        etPinCode= findViewById(R.id.etPinCode);
        btnCancel = findViewById(R.id.btnCancel);
        btnLogin = findViewById(R.id.btnLogin);
        ivType = findViewById(R.id.ivType);
        fade_in3 = AnimationUtils.loadAnimation(this, R.anim.fade_in);
        fade_out3 = AnimationUtils.loadAnimation(this, R.anim.fade_out);


        fade_in3.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {

            }

            @Override
            public void onAnimationEnd(Animation animation) {
                ivWait.clearAnimation();
                ivWait.startAnimation(fade_out3);
            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }
        });

        fade_out3.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {

            }

            @Override
            public void onAnimationEnd(Animation animation) {

                ivWait.clearAnimation();
                ivWait.setImageResource(R.drawable.loading2);
                ivWait.clearAnimation();
                ivWait.startAnimation(rotate);
            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }
        });

        if(ApplicationClass.deviceType.contains("STM"))
            ivType.setImageResource(R.drawable.stm);
        else if (ApplicationClass.deviceType.contains("RASP"))
            ivType.setImageResource(R.drawable.rasp);
        else ivType.setImageResource(R.drawable.not_knowned);

       // String[] stringArray = users.toArray().copyOf(objectArray, objectArray.length, String[].class);
        String[] strArr = asStrings(users.toArray());
        ArrayAdapter<String> adapter
                = new ArrayAdapter<String>(this, R.layout.custom_design_autocomlete, strArr);
        rotate = AnimationUtils.loadAnimation(this, R.anim.rotate);
        rotate.setStartTime(10);


        etNick.setThreshold(1);
        etNick.setAdapter(new ArrayAdapter<String>(this, R.layout.custom_design_autocomlete, strArr));
        etNick.setDropDownAnchor(R.id.ivType);

        btnLogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //todo implement bluetooth method

                if(etNick.getText().toString().isEmpty() || etNick.getText().toString().isEmpty())
                {
                    Toast.makeText(Login.this, "Please make sure you enter all fields", Toast.LENGTH_SHORT).show();
                } else {

                    ApplicationClass.sendMessage("<S" + etNick.getText().toString().trim() + ">", Login.this);
                }

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
    public static String[] asStrings(Object... objArray) {
        String[] strArray = new String[objArray.length];
        for (int i = 0; i < objArray.length; i++)
            strArray[i] = String.valueOf(objArray[i]);
        return strArray;
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
                    ApplicationClass.mBluetoothConnectionService.stop();
                    Toast.makeText(this, "Closing connection", Toast.LENGTH_SHORT).show();                }
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
            if(lengh !=etNick.getText().toString().trim().length())
            {
                Toast.makeText(Login.this, "Error Sending Nickname:"+ lengh + "Suposto:" + etNick.getText().toString().trim().length() + "With message: "+ readMessage, Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<S"+etNick.getText().toString().trim()+">", Login.this);

            }else
            {
                Toast.makeText(Login.this, "Sended nickname", Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<P"+etPinCode.getText().toString().trim()+">", Login.this);


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
                Toast.makeText(Login.this, "Error Sending pincode, Receibed: "+lengh+" suposed:"+etPinCode.length(), Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<P" + etPinCode.getText().toString().trim() + ">", Login.this);

            } else {
                Toast.makeText(Login.this, "Sended pincode", Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<Q>", Login.this);

            }

        }
    }

    void receive_login(String readMessage) {


        if(readMessage.contains("<Q") && readMessage.contains((">"))) {
            StringBuilder aux = new StringBuilder();
            for (int i = 2; i < readMessage.length()-1; i++) {
                aux.append(readMessage.charAt(i));
            } int lengh=0;
            try {
                lengh  = Integer.parseInt(aux.toString());
            }catch (NumberFormatException nfe) {
                Toast.makeText(this, "Error Parsing Number: "+nfe.getMessage(), Toast.LENGTH_SHORT).show();
            }

            switch (lengh) {
                case ApplicationClass.BAD_CREDENTIALS:

                    final AlertDialog.Builder message = new AlertDialog.Builder(Login.this);
                    LayoutInflater inflater = getLayoutInflater();
                    final View dialogView = inflater.inflate(R.layout.error_login_layout, null);
                    //   final EditText etReleaseMessage = dialogView.findViewById(R.id.etReleaseMessage);
                    message.setView(dialogView);
                    message.setTitle("Error!!");
                    message.setMessage("You enter the wrong PinCode! please try again!!");
                    message.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                        }
                    });
                    message.show();
                    break;
                case ApplicationClass.USER_NOT_FOUND: {
                    final AlertDialog.Builder messageUserNotFound = new AlertDialog.Builder(Login.this);
                    LayoutInflater inflaterUserNotFound = getLayoutInflater();
                    final View dialogViewUserNotFound = inflaterUserNotFound.inflate(R.layout.login_not_knowed, null);
                    //   final EditText etReleaseMessage = dialogView.findViewById(R.id.etReleaseMessage);
                    messageUserNotFound.setView(dialogViewUserNotFound);
                    messageUserNotFound.setTitle("Error!!");
                    messageUserNotFound.setMessage("The user that you entered does not exists, please try again!!!");
                    messageUserNotFound.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {


                        }
                    });
                    messageUserNotFound.show();
                }
                break;
                case ApplicationClass.BAD_PINCODE: {
                    final AlertDialog.Builder messageUserNotFound = new AlertDialog.Builder(Login.this);
                    LayoutInflater inflaterUserNotFound = getLayoutInflater();
                    final View dialogViewUserNotFound = inflaterUserNotFound.inflate(R.layout.login_blocked, null);

                    numbers=0;
                    messageUserNotFound.setView(dialogViewUserNotFound);
                    messageUserNotFound.setTitle("Error!!");
                    messageUserNotFound.setMessage("The pinCode entered is invalid!!!!");
                    messageUserNotFound.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {


                        }
                    });
                    messageUserNotFound.show();

                    messageUserNotFound.setCancelable(false);
                }

                break;

                case ApplicationClass.USER_BLOCKED:
                {
                    final AlertDialog.Builder messageUserNotFound = new AlertDialog.Builder(Login.this);
                    LayoutInflater inflaterUserNotFound = getLayoutInflater();
                    final View dialogViewUserNotFound = inflaterUserNotFound.inflate(R.layout.login_codewait, null);
                    ivWait = dialogViewUserNotFound.findViewById(R.id.ivWait);
                    tvNumberCounter = dialogViewUserNotFound.findViewById(R.id.tvNumber);

                    messageUserNotFound.setView(dialogViewUserNotFound);
                    messageUserNotFound.setTitle("Next Step!!");
                    messageUserNotFound.setMessage("Now enter the pinCode on the system!!!!");
                    messageUserNotFound.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {


                        }
                    });
                    messageUserNotFound.show();

                    ivWait.setAnimation(rotate);

                    messageUserNotFound.setCancelable(false);


                }  break;
                case ApplicationClass.LOGIN_SUCCESS: {
                    final AlertDialog.Builder messageUserNotFound = new AlertDialog.Builder(Login.this);
                    LayoutInflater inflaterUserNotFound = getLayoutInflater();
                    final View dialogViewUserNotFound = inflaterUserNotFound.inflate(R.layout.success_login_layout, null);
                    messageUserNotFound.setView(dialogViewUserNotFound);
                    messageUserNotFound.setTitle("Success!!");
                    messageUserNotFound.setMessage("You can now use the mobile application!!!");
                    messageUserNotFound.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                            if (!users.contains(etNick.getText().toString()))
                                saveUser(etNick.getText().toString());


                            ApplicationClass.userNickname = etNick.getText().toString().trim();
                            ApplicationClass.seccionTime = SystemClock.elapsedRealtime();
                            if(ApplicationClass.deviceType.equals("STM"))
                            {
                                ivWait.clearAnimation();
                                ivWait.setImageResource(R.drawable.correct);
                            }


                            startActivity(new Intent(Login.this, MainActivity.class));
                            Login.this.finish();

                        }
                    });
                    messageUserNotFound.show();
                }break;


            }

        }
    }

    public void saveUser(String nickname) {
        try {
            FileOutputStream file = openFileOutput("users.txt", MODE_PRIVATE);  //cria o ficheiro caso nao exitsa, e define a permisao do ficheiro para so o nossa aplicaçao
            OutputStreamWriter outputFile = new OutputStreamWriter(file);  //cria a connecao com o ficheiro que vamos escrever
            outputFile.append(nickname).append("\r\n");
            outputFile.close();
            Toast.makeText(this, "Sucessfully saveded!", Toast.LENGTH_LONG).show();

        } catch (IOException e) {
            Toast.makeText(this, e.getMessage(), Toast.LENGTH_SHORT).show();
        }

    }

    public void loadData() {
        File file = getApplicationContext().getFileStreamPath("users.txt");
        String linefromFile;
        if (file.exists()) {
            try {

                BufferedReader reader = new BufferedReader(new InputStreamReader(openFileInput("users.txt")));  //abre p ficheiro Data.txt para leitura!
                while ((linefromFile = reader.readLine()) != null) {
                    users.add(linefromFile);
                }

            } catch (IOException e) {
                Toast.makeText(this, e.getMessage(), Toast.LENGTH_SHORT).show();
            }
        } else

            Toast.makeText(this, "No users saved", Toast.LENGTH_SHORT).show();

    }

}
