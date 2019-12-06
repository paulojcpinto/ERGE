package com.cubitzo.bluetoothterminal;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothDevice;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.os.Message;
import android.text.method.ScrollingMovementMethod;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.initialization.InitializationStatus;
import com.google.android.gms.ads.initialization.OnInitializationCompleteListener;

import java.nio.charset.StandardCharsets;
import java.util.Timer;

public class MainActivity extends AppCompatActivity {
    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;
    public static final int CONNECTED_SUCCESS = 6;
    public static final String DEVICE_NAME = "device_name";
    public static final String TOAST = "toast";
    String message;
    TextView tvDisplay;
    EditText etMessage;
    Button btnSend;
    ImageView ivLoading;
    Animation rotate;
    Timer timer;
    Animation fade_in;
    CountDownTimer waitTimer;
    private AdView mAdView;
    @SuppressLint("HandlerLeak")
    private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {

                case MESSAGE_READ:
                    byte[] readBuf = (byte[]) msg.obj;
                    String readMessage = new String(readBuf, StandardCharsets.UTF_8);

                        //tvDisplay.append(readMessage);
                            tvDisplay.append(readMessage);
                            //tvDisplay.append("\n");

                    Toast.makeText(MainActivity.this, "Receibed " + readMessage, Toast.LENGTH_SHORT).show();
                    break;
                case MESSAGE_WRITE:
                    ivLoading.setImageResource(R.drawable.correct);
                    ivLoading.clearAnimation();
                    ivLoading.startAnimation(fade_in);

                    break;
                case MESSAGE_TOAST:
                    if (msg.getData().getString(TOAST).contains("Device connection was lost")) {
                        ivLoading.setImageResource(R.drawable.error);
                        ivLoading.clearAnimation();
                        ivLoading.startAnimation(fade_in);
                        ApplicationClass.playMenu.setIcon(R.drawable.bluetooth_off);

                        Toast.makeText(MainActivity.this, "Device was lost", Toast.LENGTH_SHORT).show();
                    }
                    Toast.makeText(MainActivity.this, msg.getData().getString(TOAST),
                            Toast.LENGTH_SHORT).show();
                    break;
                case CONNECTED_SUCCESS:
                    ApplicationClass.playMenu.setIcon(R.drawable.bluetooth_on);


                    break;


            }
        }
    };
    Animation fade_out;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ApplicationClass.mBluetoothConnectionService.updateHandlerContex(mHandler);
        tvDisplay = findViewById(R.id.tvDisplay);
        etMessage = findViewById(R.id.etMessage);
        btnSend = findViewById(R.id.btnSend);
        ivLoading = findViewById(R.id.ivloading);

        ivLoading.setVisibility(View.GONE);

        tvDisplay.setMovementMethod(new ScrollingMovementMethod());
        rotate = AnimationUtils.loadAnimation(this, R.anim.rotate);
        rotate.setStartTime(10);
        fade_in = AnimationUtils.loadAnimation(this, R.anim.fade_in);
        fade_in.setStartOffset(1);
        fade_out = AnimationUtils.loadAnimation(this, R.anim.fade_out);

        MobileAds.initialize(this, new OnInitializationCompleteListener() {
            @Override
            public void onInitializationComplete(InitializationStatus initializationStatus) {
            }
        });
        mAdView = findViewById(R.id.adView);
        AdRequest adRequest = new AdRequest.Builder().build();
        mAdView.loadAd(adRequest);


        btnSend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ivLoading.setImageResource(R.drawable.loading);
                ivLoading.setVisibility(View.VISIBLE);

                ivLoading.clearAnimation();
                ivLoading.startAnimation(rotate);
                //  ApplicationClass.sendMessage("O", getContext());

                ApplicationClass.sendMessage( etMessage.getText().toString().trim() , MainActivity.this);
            }
        });

        fade_in.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {

            }

            @Override
            public void onAnimationEnd(Animation animation) {

                ivLoading.clearAnimation();
                ivLoading.startAnimation(fade_out);
            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }
        });



    }
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.main, menu);
        ApplicationClass.playMenu = menu.findItem(R.id.BluetoothState);
        ApplicationClass.DeviceType = menu.findItem(R.id.DeviceTyepe);
        if (ApplicationClass.deviceConnected) {
            ApplicationClass.playMenu.setIcon(R.drawable.bluetooth_on);
        } else {
            ApplicationClass.playMenu.setIcon(R.drawable.bluetooth_off);
        }
        if (ApplicationClass.deviceConnected) {
            ApplicationClass.playMenu.setIcon(R.drawable.bluetooth_on);
        } else {
            ApplicationClass.playMenu.setIcon(R.drawable.bluetooth_off);
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
            case R.id.Clear:
                tvDisplay.setText("");
                break;

        }
        return super.onOptionsItemSelected(item);

    }

}
