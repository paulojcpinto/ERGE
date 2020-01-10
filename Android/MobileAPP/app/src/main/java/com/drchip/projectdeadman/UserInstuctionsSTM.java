package com.drchip.projectdeadman;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

import java.util.ArrayList;

public class UserInstuctionsSTM extends AppCompatActivity {

    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;
    public static final int CONNECTED_SUCCESS = 6;
    public static final String TOAST = "toast";
    TextView tvFingerSTM, tvPinCode, tvFingerSTMDescription, tvPinCodeDescription, tvSuccess;
    ImageView ivFingerSTM, ivPinCode, ivSuccess;
    Button btnCancel, btnConfirm, btnTeste;
    LinearLayout linSuccess, linPincode;
    MenuItem playMenu;
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
                    if(readMessage.contains("<F>"))
                    {

                      //  ivFingerSTM.setImageResource(R.drawable.correct);
                       // ivFingerSTM.clearAnimation();
                        toUpdate.add(ivFingerSTM);
                        ivFingerSTM.startAnimation(fade_out2);
                        ivPinCode.startAnimation(rotate);
                        linPincode.startAnimation(fade_in2);
                        linPincode.setVisibility(View.VISIBLE);
                    }

                    if(readMessage.contains("<Q1>"))
                    {
                        toUpdate.add(ivPinCode);
                        ivPinCode.startAnimation(fade_out2);
                        linSuccess.setVisibility(View.VISIBLE);
                        linSuccess.startAnimation(fade_in2);
                        Animation fade_in1 = AnimationUtils.loadAnimation(UserInstuctionsSTM.this, R.anim.fade_in);
                        btnConfirm.setVisibility(View.VISIBLE);
                        btnConfirm.startAnimation(fade_in1);


                    }
                    if(readMessage.contains("<Q5"))
                    {
                        toUpdate.add(ivPinCode);
                        ivPinCode.startAnimation(fade_out);
                        ivSuccess.setImageResource(R.drawable.error);
                        tvSuccess.setText("Error: pincode invalid!!!");
                        linSuccess.setVisibility(View.VISIBLE);
                        linSuccess.startAnimation(fade_in2);
                        ivSuccess.startAnimation(fade_in);
                    }
                    if(readMessage.contains("<Y1>"))
                    {
                        linSuccess.setVisibility(View.VISIBLE);
                        linSuccess.startAnimation(fade_in);
                        Animation fade_in1 = AnimationUtils.loadAnimation(UserInstuctionsSTM.this, R.anim.fade_in);
                        btnConfirm.setVisibility(View.VISIBLE);
                        btnConfirm.startAnimation(fade_in1);


                    }
                    if(readMessage.contains("<Y2>"))
                    {
                        ivSuccess.setImageResource(R.drawable.error);
                        tvSuccess.setText("Error registering your fingerprint, timeout");
                        linSuccess.setVisibility(View.VISIBLE);
                        linSuccess.startAnimation(fade_in2);
                        ivSuccess.startAnimation(fade_in);

                    }



                    Toast.makeText(UserInstuctionsSTM.this, "Receibed " + readMessage, Toast.LENGTH_SHORT).show();
                    break;

                case MESSAGE_TOAST:
                    if (msg.getData().getString(TOAST).contains("Device connection was lost")) {
                        playMenu.setIcon(R.drawable.bluetooth_off);

                        Toast.makeText(UserInstuctionsSTM.this, "Device was lost", Toast.LENGTH_SHORT).show();
                    }

                    Toast.makeText(getApplicationContext(), msg.getData().getString(TOAST),
                            Toast.LENGTH_SHORT).show();
                    break;
                case CONNECTED_SUCCESS:
                    ApplicationClass.sendMessage("O", UserInstuctionsSTM.this);

                    ApplicationClass.sendMessage("<L>", UserInstuctionsSTM.this);
                    break;

            }
        }
    };
    MenuItem DeviceType;
    Animation rotate;
    Animation fade_in;
    Animation fade_in2;
    Animation fade_out;
    Animation fade_out2;
    ArrayList<ImageView> toUpdate;

    int aux = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user_instuctions_stm);
        ApplicationClass.mBluetoothConnectionService.updateHandlerContex(mHandler);

        tvFingerSTM = findViewById(R.id.tvFingerStm);
        tvPinCode = findViewById(R.id.tvPinCode);
        tvFingerSTMDescription = findViewById(R.id.tvFingeStmDescription);
        tvPinCodeDescription = findViewById(R.id.tvPinCodeDescription);
        ivFingerSTM = findViewById(R.id.ivFingerStm);
        ivPinCode = findViewById(R.id.ivPinCode);
        ivSuccess = findViewById(R.id.ivSuccess);
        btnCancel = findViewById(R.id.btnCancel);
        btnConfirm = findViewById(R.id.btnConfirm);
        linSuccess = findViewById(R.id.linSuccess);
        linPincode = findViewById(R.id.linPinCode);
        btnTeste = findViewById(R.id.btnTeste);
        toUpdate = new ArrayList<ImageView>();
        tvSuccess= findViewById(R.id.tvSuccess);
        linPincode.setVisibility(View.GONE);
        linSuccess.setVisibility(View.GONE);
        tvPinCodeDescription.setVisibility(View.GONE);
        tvFingerSTMDescription.setVisibility(View.GONE);
        btnConfirm.setVisibility(View.GONE);

        rotate = AnimationUtils.loadAnimation(this, R.anim.rotate);
        rotate.setStartTime(10);
        fade_in = AnimationUtils.loadAnimation(this, R.anim.fade_in);
        fade_in.setStartOffset(1);
        fade_in2 = AnimationUtils.loadAnimation(this, R.anim.fade_in);
        fade_in2.setStartOffset(1);
        fade_out = AnimationUtils.loadAnimation(this, R.anim.fade_out);
        fade_out2 = AnimationUtils.loadAnimation(this, R.anim.fade_out);

        ivFingerSTM.startAnimation(rotate);

        fade_out.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {

            }

            @Override
            public void onAnimationEnd(Animation animation) {

                for(int i=0; i<toUpdate.size();i++)
                {
                    toUpdate.get(i).clearAnimation();
                    toUpdate.get(i).setImageResource(R.drawable.error);
                    toUpdate.get(i).startAnimation(fade_in);

                }
                toUpdate= new ArrayList<>();
                toUpdate.clear();
            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }
        });

        fade_out2.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {

            }

            @Override
            public void onAnimationEnd(Animation animation) {

                for(int i=0; i<toUpdate.size();i++)
                {
                    toUpdate.get(i).clearAnimation();
                    toUpdate.get(i).setImageResource(R.drawable.correct);
                    toUpdate.get(i).startAnimation(fade_in);

                }
                toUpdate= new ArrayList<>();
                toUpdate.clear();
            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }
        });

        ActionBar actionBar = getSupportActionBar();
        actionBar.setTitle("Instructions");
        ApplicationClass.sendMessage("<Y>",this);


        tvFingerSTM.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (tvFingerSTMDescription.getVisibility() == View.GONE)
                    tvFingerSTMDescription.setVisibility(View.VISIBLE);
                else
                    tvFingerSTMDescription.setVisibility(View.GONE);
            }
        });

        tvPinCode.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (tvPinCodeDescription.getVisibility() == View.GONE)
                    tvPinCodeDescription.setVisibility(View.VISIBLE);
                else
                    tvPinCodeDescription.setVisibility(View.GONE);
            }
        });
        btnTeste.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                switch (aux) {
                    case 0:
                        ivFingerSTM.setImageResource(R.drawable.correct);
                        ivFingerSTM.clearAnimation();
                        ivPinCode.startAnimation(rotate);
                        linPincode.setVisibility(View.VISIBLE);


                        break;
                    case 1:
                        ivPinCode.setImageResource(R.drawable.correct);
                        ivPinCode.clearAnimation();
                        linSuccess.setVisibility(View.VISIBLE);
                        ivSuccess.startAnimation(fade_in);
                        Animation fade_in1 = AnimationUtils.loadAnimation(UserInstuctionsSTM.this, R.anim.fade_in);
                        btnConfirm.startAnimation(fade_in1);
                        btnConfirm.setVisibility(View.VISIBLE);
                        break;

                }


                aux++;

            }
        });
        btnCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(UserInstuctionsSTM.this, Enter.class));
                UserInstuctionsSTM.this.finish();
            }
        });
        btnConfirm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ApplicationClass.seccionTime = SystemClock.elapsedRealtime();
                startActivity(new Intent(UserInstuctionsSTM.this, MainActivity.class));
                UserInstuctionsSTM.this.finish();

            }
        });


    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.login_register_menu, menu);
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
        return super.onCreateOptionsMenu(menu);
    }

    public boolean onOptionsItemSelected(@NonNull MenuItem item) {   // override metodos em code !!!

        switch (item.getItemId()) {

            case R.id.BluetoothState:
                if (ApplicationClass.deviceConnected) {
                    ApplicationClass.mBluetoothConnectionService.stop();
                    Toast.makeText(this, "Closing connection", Toast.LENGTH_SHORT).show();
                } else {
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
