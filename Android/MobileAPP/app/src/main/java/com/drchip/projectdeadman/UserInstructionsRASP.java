package com.drchip.projectdeadman;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Handler.Callback;
import android.os.Message;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Timer;
import java.util.TimerTask;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

public class UserInstructionsRASP extends AppCompatActivity {

    TextView tvFingerRasp, tvFingerRaspDescription, tvFaceReco, tvFaceRecoDescription, tvImageNumber;
    ImageView ivFingerRasp, ivFaceStart, ivImageCount;
    ProgressBar pbImageCount;
    public static final int MESSAGE_STATE_CHANGE = 1;
    LinearLayout linFaceDescription, linImageCount;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;
    public static final int TIMER_OVER = 7;
    public static final String TOAST = "toast";
    public static final int CONNECTED_SUCCESS = 6;
    Button btnCancel, btnConfirm, btnTeste;
    int aux = 0;
    long starttime = 0;
    Animation rotate;

    Animation to_start;
    Animation fade_in;
    Animation fade_out,fade_out2;
    Timer timer = new Timer();
    final Handler h = new Handler(new Callback() {

        @Override
        public boolean handleMessage(Message msg) {
            ivImageCount.clearAnimation();
            // ivImageCount.setImageResource(R.drawable.loading2);
            ivImageCount.startAnimation(fade_out);
            timer.cancel();
            timer.purge();
            return false;
        }
    });
    private MenuItem playMenu;
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
                        ivFingerRasp.setImageResource(R.drawable.correct);
                        ivFingerRasp.clearAnimation();
                        ivFaceStart.startAnimation(rotate);
                        linFaceDescription.setVisibility(View.VISIBLE);

                        aux=0;
                    }
                    if(readMessage.contains("<I>"))
                    {



                        if(aux==0)
                        {
                            ivFaceStart.setImageResource(R.drawable.correct);
                            ivFaceStart.clearAnimation();
                            linImageCount.setVisibility(View.VISIBLE);
                            Animation animation2 = AnimationUtils.loadAnimation(UserInstructionsRASP.this, R.anim.rotate);
                            animation2.setStartTime(10);
                            ivImageCount.startAnimation(animation2);
                        }
                        aux++;
                        if ( aux < 15) {
                            ivImageCount.clearAnimation();
                            ivImageCount.startAnimation(fade_in);
                            tvImageNumber.setText(aux  + "");
                            pbImageCount.setProgress(aux );
                            starttime = System.currentTimeMillis();
                            ivImageCount.setImageResource(R.drawable.correct);

                            // ivImageCount.setVisibility(View.INVISIBLE);


                            //ivImageCount.startAnimation(fade_in);
                            //  timer = new Timer();
                            //timer.schedule(new firstTask(), 1000,9000);


                        } else  {
                            Animation fade_in1 = AnimationUtils.loadAnimation(UserInstructionsRASP.this, R.anim.fade_in);
                            tvImageNumber.setText(aux  + "");
                            pbImageCount.setProgress(aux );
                            ivImageCount.setImageResource(R.drawable.correct);
                            btnConfirm.startAnimation(fade_in1);
                            btnConfirm.setVisibility(View.VISIBLE);
                        }

                    }

                    Toast.makeText(UserInstructionsRASP.this, "Receibed " + readMessage, Toast.LENGTH_SHORT).show();
                    break;

                case MESSAGE_TOAST:
                    if (msg.getData().getString(TOAST).contains("Device connection was lost")) {
                        playMenu.setIcon(R.drawable.bluetooth_off);

                        Toast.makeText(UserInstructionsRASP.this, "Device was lost", Toast.LENGTH_SHORT).show();
                    }
                    Toast.makeText(getApplicationContext(), msg.getData().getString(TOAST),
                            Toast.LENGTH_SHORT).show();
                    break;
                case CONNECTED_SUCCESS:
                    ApplicationClass.sendMessage("O", UserInstructionsRASP.this);

                    ApplicationClass.sendMessage("<L>", UserInstructionsRASP.this);
                    break;

            }
        }
    };
    private MenuItem DeviceType;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user_instructions_rasp);
        ApplicationClass.mBluetoothConnectionService.updateHandlerContex(mHandler);

        tvFingerRasp = findViewById(R.id.tvFingerRasp);
        tvFingerRaspDescription = findViewById(R.id.tvFingeRaspDescription);
        tvFaceReco = findViewById(R.id.tvFacereco);
        tvFaceRecoDescription = findViewById(R.id.tvFacerecDescription);
        tvImageNumber = findViewById(R.id.tvImageNumber);
        ivFingerRasp = findViewById(R.id.ivFingerRasp);
        ivFaceStart = findViewById(R.id.ivFaceStart);
        ivImageCount = findViewById(R.id.ivImageCount);
        pbImageCount = findViewById(R.id.pbImageCount);
        btnCancel = findViewById(R.id.btnCancel);
        btnConfirm = findViewById(R.id.btnConfirm);
        linFaceDescription = findViewById(R.id.linFaceDescripiton);
        linImageCount = findViewById(R.id.linImageCount);
        btnTeste = findViewById(R.id.btnTeste);



        tvFaceRecoDescription.setVisibility(View.GONE);
        tvFingerRaspDescription.setVisibility(View.GONE);
        linFaceDescription.setVisibility(View.GONE);
        linImageCount.setVisibility(View.GONE);
        btnConfirm.setVisibility(View.GONE);

        pbImageCount.setProgress(0);

        rotate = AnimationUtils.loadAnimation(this, R.anim.rotate);
        rotate.setStartTime(10);

        to_start = AnimationUtils.loadAnimation(this, R.anim.rotate_to_start);
        fade_in = AnimationUtils.loadAnimation(this, R.anim.fade_in);
        fade_in.setStartOffset(1);
        fade_out = AnimationUtils.loadAnimation(this, R.anim.fade_out);
        fade_out2 = AnimationUtils.loadAnimation(this, R.anim.fade_out);

        fade_in.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {

            }

            @Override
            public void onAnimationEnd(Animation animation) {


            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }
        });

        ApplicationClass.sendMessage("<Y>",this);

        ivFingerRasp.startAnimation(rotate);

        ActionBar actionBar = getSupportActionBar();
        actionBar.setTitle("Instructions");
        tvFingerRasp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (tvFingerRaspDescription.getVisibility() == View.GONE)
                    tvFingerRaspDescription.setVisibility(View.VISIBLE);
                else tvFingerRaspDescription.setVisibility(View.GONE);
            }
        });
        tvFaceReco.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (tvFaceRecoDescription.getVisibility() == View.GONE)
                    tvFaceRecoDescription.setVisibility(View.VISIBLE);
                else tvFaceRecoDescription.setVisibility(View.GONE);
            }
        });



        btnTeste.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                switch (aux) {
                    case 0:
                        ivFingerRasp.setImageResource(R.drawable.correct);
                        ivFingerRasp.clearAnimation();
                        ivFaceStart.startAnimation(rotate);
                        linFaceDescription.setVisibility(View.VISIBLE);


                        break;
                    case 1:
                        ivFaceStart.setImageResource(R.drawable.correct);
                        ivFaceStart.clearAnimation();
                        linImageCount.setVisibility(View.VISIBLE);
                        Animation animation2 = AnimationUtils.loadAnimation(UserInstructionsRASP.this, R.anim.rotate);
                        animation2.setStartTime(10);
                        ivImageCount.startAnimation(animation2);
                        break;

                }

                if (aux > 1 && aux < 18) {
                    ivImageCount.clearAnimation();
                    ivImageCount.startAnimation(fade_in);
                    tvImageNumber.setText(aux - 2 + "");
                    pbImageCount.setProgress(aux - 2);
                    starttime = System.currentTimeMillis();
                    ivImageCount.setImageResource(R.drawable.correct);

                    // ivImageCount.setVisibility(View.INVISIBLE);


                    //ivImageCount.startAnimation(fade_in);
                    //  timer = new Timer();
                    //timer.schedule(new firstTask(), 1000,9000);


                } else if (aux == 18) {
                    Animation fade_in1 = AnimationUtils.loadAnimation(UserInstructionsRASP.this, R.anim.fade_in);

                    btnConfirm.startAnimation(fade_in1);
                    btnConfirm.setVisibility(View.VISIBLE);
                }
                aux++;

            }
        });


        fade_in.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {

            }

            @Override
            public void onAnimationEnd(Animation animation) {

                timer = new Timer();
                timer.schedule(new firstTask(), 100, 9000);

            }

            @Override
            public void onAnimationRepeat(Animation animation) {


            }
        });
        fade_out.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {

            }

            @Override
            public void onAnimationEnd(Animation animation) {

                ivImageCount.setImageResource(R.drawable.loading2);
                ivImageCount.clearAnimation();
                ivImageCount.startAnimation(rotate);
            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }
        });
        btnCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(UserInstructionsRASP.this, Enter.class));
                UserInstructionsRASP.this.finish();
            }
        });
        btnConfirm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(UserInstructionsRASP.this, MainActivity.class));
                UserInstructionsRASP.this.finish();

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

    class firstTask extends TimerTask {

        @Override
        public void run() {
            h.sendEmptyMessage(0);
        }
    }



}


