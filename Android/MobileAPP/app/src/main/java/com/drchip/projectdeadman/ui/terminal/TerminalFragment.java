package com.drchip.projectdeadman.ui.terminal;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.os.Message;
import android.text.method.ScrollingMovementMethod;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProviders;

import com.drchip.projectdeadman.ApplicationClass;
import com.drchip.projectdeadman.R;

import java.nio.charset.StandardCharsets;
import java.util.Timer;
import java.util.TimerTask;

public class TerminalFragment extends Fragment {

    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;
    public static final int CONNECTED_SUCCESS = 6;
    public static final int UPDATE_IMAGE_ERROR = 7;
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
    @SuppressLint("HandlerLeak")
    private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {

                case MESSAGE_READ:
                    byte[] readBuf = (byte[]) msg.obj;
                    String readMessage = new String(readBuf, StandardCharsets.UTF_8);
                    if (readMessage.contains("STM") || readMessage.contains("RASP")) {

                        ApplicationClass.playMenu.setIcon(R.drawable.bluetooth_on);
                        message = "";

                    } else {
                        //tvDisplay.append(readMessage);

                        message = readMessage;

                        if (message.contains(">") && message.contains("<")) {

                            if(waitTimer != null) {
                                waitTimer.cancel();
                                waitTimer = null;
                            }

                            ivLoading.setImageResource(R.drawable.correct);
                            ivLoading.clearAnimation();
                            ivLoading.startAnimation(fade_in);
                            tvDisplay.append(readMessage);
                            message = "";
                            tvDisplay.append("\n");
                        } else {
                            if(waitTimer==null)
                            waitTimer = new CountDownTimer(1000, 300) {

                                public void onTick(long millisUntilFinished) {
                                    //called every 300 milliseconds, which could be used to
                                    //send messages or some other action
                                }

                                public void onFinish() {
                                    //After 60000 milliseconds (60 sec) finish current
                                    //if you would like to execute something when time finishes
                                    ivLoading.clearAnimation();
                                    ivLoading.setImageResource(R.drawable.error);
                                    ivLoading.startAnimation(fade_in);
                                    //message = "";
                                    waitTimer.cancel();
                                    waitTimer = null;
                                   // ApplicationClass.sendMessage("<E"+4+">".trim().toString(), getContext());
                                }
                            }.start();
                        }
//                        else
//                        {
//                            tvDisplay.append("Error:"+readMessage+"\n");
//
//                        }


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
                case UPDATE_IMAGE_ERROR:

                    //ApplicationClass.sendMessage("<E"+4+">", getContext());

                    ivLoading.clearAnimation();
                    ivLoading.setImageResource(R.drawable.error);
                    ivLoading.startAnimation(fade_in);
                    message = "";


                    break;

            }
        }
    };
    Animation fade_out;
    private TerminalViewModel terminalViewModel;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        ApplicationClass.mBluetoothConnectionService.updateHandlerContex(mHandler);

        terminalViewModel =
                ViewModelProviders.of(this).get(TerminalViewModel.class);
        View root = inflater.inflate(R.layout.fragment_terminal, container, false);

        tvDisplay = root.findViewById(R.id.tvDisplay);
        etMessage = root.findViewById(R.id.etMessage);
        btnSend = root.findViewById(R.id.btnSend);
        ivLoading = root.findViewById(R.id.ivloading);

        ivLoading.setVisibility(View.GONE);

        tvDisplay.setMovementMethod(new ScrollingMovementMethod());
        message = "";

        rotate = AnimationUtils.loadAnimation(getContext(), R.anim.rotate);
        rotate.setStartTime(10);
        fade_in = AnimationUtils.loadAnimation(getContext(), R.anim.fade_in);
        fade_in.setStartOffset(1);
        fade_out = AnimationUtils.loadAnimation(getContext(), R.anim.fade_out);

        btnSend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ivLoading.setImageResource(R.drawable.loading);
                ivLoading.setVisibility(View.VISIBLE);

                ivLoading.clearAnimation();
                ivLoading.startAnimation(rotate);
              //  ApplicationClass.sendMessage("O", getContext());

                ApplicationClass.sendMessage( etMessage.getText().toString().trim() , getContext());
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







        return root;
    }


    class firstTask extends TimerTask {

        @Override
        public void run() {
            mHandler.obtainMessage(TerminalFragment.UPDATE_IMAGE_ERROR, 1, 0, null)
                    .sendToTarget();




        }
    }
}