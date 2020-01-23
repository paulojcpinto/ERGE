package com.drchip.projectdeadman;

import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Timer;
import java.util.TimerTask;

public class AddImages extends AppCompatActivity {
    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;
    public static final int CONNECTED_SUCCESS = 6;
    public static final String TOAST = "toast";

    Animation fade_in;
    Animation fade_out,fade_out2;

    Timer timer = new Timer();
    final Handler h = new Handler(new Handler.Callback() {

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


    @SuppressLint("HandlerLeak")
    private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {

                case MESSAGE_READ:
                    byte[] readBuf = (byte[]) msg.obj;
                    String readMessage = new String(readBuf, 0, msg.arg1);
                    if (readMessage.contains("STM") || readMessage.contains("RASP")) {

                        ApplicationClass.playMenu.setIcon(R.drawable.bluetooth_on);

                    }
                    //bt_parsing(readMessage);
                    //receive_phone_number(readMessage);
                    //receive_message(readMessage);
                    //receive_repeatTime(readMessage);
                    //receive_platform(readMessage);
                    //receive_password(readMessage);
                    //receive_email(readMessage);

                    if(readMessage.contains("<I>")) {


                        if (imagesCount == 0) {
                            Animation animation2 = AnimationUtils.loadAnimation(AddImages.this, R.anim.rotate);
                            animation2.setStartTime(10);
                            ivImageCount.startAnimation(animation2);
                        }
                        imagesCount++;
                        if (imagesCount < 15) {
                            ivImageCount.clearAnimation();
                            ivImageCount.startAnimation(fade_in);
                            tvNumberImages.setText(imagesCount + "");
                            ivImageCount.setImageResource(R.drawable.correct);

                            // ivImageCount.setVisibility(View.INVISIBLE);


                            //ivImageCount.startAnimation(fade_in);
                            //  timer = new Timer();
                            //timer.schedule(new firstTask(), 1000,9000);


                        } else {
                            Animation fade_in1 = AnimationUtils.loadAnimation(AddImages.this, R.anim.fade_in);
                            tvNumberImages.setText(imagesCount + "");
                            ivImageCount.setImageResource(R.drawable.correct);
                            btnConfirm.startAnimation(fade_in1);
                            btnConfirm.setVisibility(View.VISIBLE);
                        }
                    }

                        Toast.makeText(AddImages.this, "Message Reciebed:"+readMessage, Toast.LENGTH_SHORT).show();
                    break;

                case MESSAGE_TOAST:
                    if (msg.getData().getString(TOAST).contains("Device connection was lost")) {
                        ApplicationClass.playMenu.setIcon(R.drawable.bluetooth_off);

                        Toast.makeText(AddImages.this, "Device was lost", Toast.LENGTH_SHORT).show();
                    }
                    Toast.makeText(AddImages.this, msg.getData().getString(TOAST),
                            Toast.LENGTH_SHORT).show();
                    break;
                case CONNECTED_SUCCESS:
                    ApplicationClass.sendMessage("O", AddImages.this);

                    ApplicationClass.sendMessage("<L>", AddImages.this);
                    break;

            }
        }
    };

    TextView tvNumber, tvNumberImages;
    ImageView ivImageCount;
    Button btnConfirm;
    int imagesCount = 0;
    Animation rotate;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_images);
        ApplicationClass.mBluetoothConnectionService.updateHandlerContex(mHandler);

        tvNumber= findViewById(R.id.tvNumber);
        tvNumberImages = findViewById(R.id.tvNumberIMages);
        ivImageCount = findViewById(R.id.ivImageCount);
        btnConfirm = findViewById(R.id.btnConfirm);

        Animation animation2 = AnimationUtils.loadAnimation(AddImages.this, R.anim.rotate);
        animation2.setStartTime(10);
        ivImageCount.startAnimation(animation2);
        ApplicationClass.sendMessage("<F>",AddImages.this);
        ActionBar actionBar = getSupportActionBar();
        actionBar.hide();


        rotate = AnimationUtils.loadAnimation(this, R.anim.rotate);
        rotate.setStartTime(10);
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



    }
    void receive_recieveImages(String message)
    {
        if (message.contains("<F") && message.contains((">"))) {

            tvNumberImages.setText(parsing(message).substring(message.indexOf("<F") + 2, message.lastIndexOf(">")));


        }
    }
    private String parsing(String message) {
        StringBuilder result = new StringBuilder();
        for (int i = message.indexOf('<') + 2; i < message.indexOf('>') - 1; i++) {
            result.append(message.charAt(i));

        }
        return message;
    }


    class firstTask extends TimerTask {

        @Override
        public void run() {
            h.sendEmptyMessage(0);
        }
    }

}
