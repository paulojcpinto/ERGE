package com.drchip.projectdeadman.ui.home;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.drchip.projectdeadman.ApplicationClass;
import com.drchip.projectdeadman.DevicesAdapter;
import com.drchip.projectdeadman.R;
import com.drchip.projectdeadman.Register;
import com.drchip.projectdeadman.UserInstructionsRASP;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;

import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;

public class HomeFragment extends Fragment {


    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;
    public static final int CONNECTED_SUCCESS = 6;
    public static final String TOAST = "toast";


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
                        ApplicationClass.conectedTime= SystemClock.elapsedRealtime();

                    }
                    receive_nick(readMessage);
                    if(readMessage.contains("<H1>"))
                    {
                        tvPresenceStatus.setText("Already Done");
                        toUpdateText.add(tvPresenceStatus);
                        toUpdateImage.add(ivPresenseStatus);
                        ivPresenseStatus.setImageResource(R.drawable.correct);
                        ivPresenseStatus.setAnimation(fade_out2);
                        tvPresenceStatus.setAnimation(fade_out);

                    }
                    if(readMessage.contains("<H2>"))
                    {
                        tvPresenceStatus.setText("User not unknown");
                        toUpdateText.add(tvPresenceStatus);
                        toUpdateImage.add(ivPresenseStatus);
                        ivPresenseStatus.setImageResource(R.drawable.not_knowned);
                        ivPresenseStatus.setAnimation(fade_out);
                        tvPresenceStatus.setAnimation(fade_out);
                    }
                    if(readMessage.contains("<H0>"))
                    {
                        tvPresenceStatus.setText("Lacking");
                        toUpdateText.add(tvPresenceStatus);
                        toUpdateImage.add(ivPresenseStatus);
                        ivPresenseStatus.setImageResource(R.drawable.correct);
                        ivPresenseStatus.setAnimation(fade_out);
                        tvPresenceStatus.setAnimation(fade_out);
                    }
                    if(readMessage.contains("<F1>"))
                    {
                        tvFingerStatus.setText("Finger Read with success");
                        ivFingerStatus.setImageResource(R.drawable.dedinho_green);
                        toUpdateLayout.add(lFingerStatus);
                        lFingerStatus.setAnimation(fade_in2);

                    }
                    if(readMessage.contains("<F2>"))
                    {
                        tvFingerStatus.setText("Error reading finger!");
                        ivFingerStatus.setImageResource(R.drawable.dedinho_red);
                        toUpdateLayout.add(lFingerStatus);
                        lFingerStatus.setAnimation(fade_in2);
                    }
                    if(readMessage.contains("<I1>"))
                    {
                        tvFaceStatus.setText("Successful Facial Recognition");
                        ivFaceStatus.setImageResource(R.drawable.facescansuccess);
                        toUpdateLayout.add(lFaceStatus);
                        lFaceStatus.setAnimation(fade_in2);
                    }
                    if(readMessage.contains("<I2>"))
                    {
                        tvFaceStatus.setText("Facial Recognition timed out");
                        ivFaceStatus.setImageResource(R.drawable.facescanerror);
                        toUpdateLayout.add(lFaceStatus);
                        lFaceStatus.setAnimation(fade_in2);
                    }





                    Toast.makeText(getContext(), "Receibed " + readMessage, Toast.LENGTH_SHORT).show();
                    break;

                case MESSAGE_TOAST:
                    if (msg.getData().getString(TOAST).contains("Device connection was lost")) {
                        ApplicationClass.playMenu.setIcon(R.drawable.bluetooth_off);

                        Toast.makeText(getContext(), "Device was lost", Toast.LENGTH_SHORT).show();
                    }
                    Toast.makeText(getContext(), msg.getData().getString(TOAST),
                            Toast.LENGTH_SHORT).show();
                    break;
                case CONNECTED_SUCCESS:
                    ApplicationClass.sendMessage("O", getContext());

                    ApplicationClass.sendMessage("<L>", getContext());
                    break;

            }
        }
    };


ImageView ivPresenseStatus,ivFingerStatus,ivFaceStatus,ivClock;
TextView tvPresenceStatus, tvFingerStatus, tvFaceStatus,tvTime;
LinearLayout lFingerStatus,lFaceStatus;
Animation blink;
    Animation rotate;
    Animation fade_in;
    Animation fade_in2;
    Animation fade_out;
    Animation fade_out2;
    ArrayList<ImageView> toUpdateImage;
    ArrayList<TextView> toUpdateText;
    ArrayList<LinearLayout> toUpdateLayout;



    Handler timerHandler = new Handler();

    Runnable timerRunnable = new Runnable() {

        @Override
        public void run() {
            //tvTime.setText(((SystemClock.elapsedRealtime()-ApplicationClass.conectedTime )/1000)/60+"min");

            //long millis = System.currentTimeMillis() - startTime;
            long millis = SystemClock.elapsedRealtime()-ApplicationClass.seccionTime;

            int seconds = (int) (millis / 1000);
            int minutes = seconds / 60;
            seconds = seconds % 60;
            tvTime.setText(String.format("%d:%02d", minutes, seconds));

            timerHandler.postDelayed(this, 500);
        }
    };
    Timer timer = new Timer();
    final Handler h = new Handler(new Handler.Callback() {

        @Override
        public boolean handleMessage(Message msg) {
            for(int i=0; i<toUpdateLayout.size();i++)
            {
                toUpdateLayout.get(i).clearAnimation();
                toUpdateLayout.get(i).startAnimation(fade_out);

            }
            toUpdateLayout= new ArrayList<>();
            toUpdateLayout.clear();
            timer.cancel();
            timer.purge();
            return false;
        }
    });

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {

        View root = inflater.inflate(R.layout.fragment_home, container, false);
        ApplicationClass.mBluetoothConnectionService.updateHandlerContex(mHandler);
         ivPresenseStatus = root.findViewById(R.id.ivPresenseStatus);
         ivFingerStatus = root.findViewById(R.id.ivFingerStatus);
         ivFaceStatus = root.findViewById(R.id.ivFaceStatus);
         tvPresenceStatus = root.findViewById(R.id.tvPresenceStatus);
         tvFingerStatus = root.findViewById(R.id.tvFingerStm);
         tvFaceStatus = root.findViewById(R.id.tvFaceStatus);
         lFingerStatus = root.findViewById(R.id.lFingerStatus);
         lFaceStatus = root.findViewById(R.id.lFaceStatus);
         ivClock = root.findViewById(R.id.ivClock);
         tvTime = root.findViewById(R.id.tvTime);
         toUpdateImage = new ArrayList<ImageView>();
         toUpdateText=  new ArrayList<>();
         toUpdateLayout= new ArrayList<>();


        blink = AnimationUtils.loadAnimation(getContext(), R.anim.blink);
        tvTime.setText(((SystemClock.elapsedRealtime()-ApplicationClass.seccionTime )/1000)/60+"min");

        ivClock.setAnimation(blink);
        timerHandler.postDelayed(timerRunnable, 0);

        ApplicationClass.sendMessage("<S"+ApplicationClass.userNickname+">",getContext());
       // ApplicationClass.sendMessage("<H>",getContext());


        fade_in = AnimationUtils.loadAnimation(getContext(), R.anim.fade_in);
        fade_in.setStartOffset(1);
        fade_in2 = AnimationUtils.loadAnimation(getContext(), R.anim.fade_in);
        fade_in2.setStartOffset(1);
        fade_out = AnimationUtils.loadAnimation(getContext(), R.anim.fade_out);
        fade_out2 = AnimationUtils.loadAnimation(getContext(), R.anim.fade_out);


        fade_out.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {

            }

            @Override
            public void onAnimationEnd(Animation animation) {

                for(int i=0; i<toUpdateImage.size();i++)
                {
                    toUpdateImage.get(i).clearAnimation();
                    toUpdateImage.get(i).setImageResource(R.drawable.error);
                    toUpdateImage.get(i).startAnimation(fade_in);

                }
                toUpdateImage= new ArrayList<>();
                toUpdateImage.clear();

            for(int i=0; i<toUpdateText.size();i++)
            {
                toUpdateText.get(i).clearAnimation();
                toUpdateText.get(i).startAnimation(fade_in);

            }
                toUpdateText= new ArrayList<>();
                toUpdateText.clear();

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

            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }
        });

        fade_in2.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {

            }

            @Override
            public void onAnimationEnd(Animation animation) {

                timer = new Timer();
                timer.schedule(new firstTask(), 1000, 90000);

            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }
        });




        return root;
    }
    class firstTask extends TimerTask {

        @Override
        public void run() {
            h.sendEmptyMessage(0);
        }
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
                Toast.makeText(getContext(), "Error Parsing Number: "+nfe.getMessage(), Toast.LENGTH_SHORT).show();
            }
            if(lengh !=ApplicationClass.userNickname.length())
            {
                Toast.makeText(getContext(), "Error Sending Nickname:"+ lengh + "Suposto:" + ApplicationClass.userNickname.length() + "With message: "+ readMessage, Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<S"+ApplicationClass.userNickname.length()+">", getContext());

            }else
            {
                ApplicationClass.sendMessage("<H>",getContext());

            }

        }
    }

}