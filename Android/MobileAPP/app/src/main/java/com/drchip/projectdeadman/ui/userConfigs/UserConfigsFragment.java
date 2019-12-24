package com.drchip.projectdeadman.ui.userConfigs;

import android.annotation.SuppressLint;
import android.app.ProgressDialog;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProviders;

import com.drchip.projectdeadman.ApplicationClass;
import com.drchip.projectdeadman.R;

public class UserConfigsFragment extends Fragment {

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

                    }
                    bt_parsing(readMessage);


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

    private UserConfigsViewModel galleryViewModel;
    EditText etPlatform,etPhoneNumber,etRepeatTime,etMail,etMailPassword,etMessage;
    ImageView ivTop, ivBot;
    Animation animTop, animBot;
    int itBot;


    ProgressDialog progressDialog;

    @RequiresApi(api = Build.VERSION_CODES.O)
    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        ApplicationClass.mBluetoothConnectionService.updateHandlerContex(mHandler);

        galleryViewModel =
                ViewModelProviders.of(this).get(UserConfigsViewModel.class);
        View root = inflater.inflate(R.layout.fragment_userconfigs, container, false);

        etPlatform= root.findViewById(R.id.etPlatform);
        etPhoneNumber= root.findViewById(R.id.etPhoneNumber);
        etRepeatTime= root.findViewById(R.id.etRepeatTime);
        etMail = root.findViewById(R.id.etEmail);
        etMailPassword= root.findViewById(R.id.etEmailPassword);
        etMessage = root.findViewById(R.id.etMessage);
        ivTop = root.findViewById(R.id.ivTop);
        ivBot = root.findViewById(R.id.ivBot);
        progressDialog = new ProgressDialog(getContext());
        progressDialog.setTitle("Communicating");
        progressDialog.setMessage("Loading user data!...");
        progressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
        progressDialog.setMax(6);
        progressDialog.setCancelable(true);
        progressDialog.show();
        animTop = AnimationUtils.loadAnimation(getContext(), R.anim.update_anim);
        animBot = AnimationUtils.loadAnimation(getContext(), R.anim.update_anim);
        animBot.setStartOffset(1000);
        ivTop.setAnimation(animTop);
        ivBot.setAnimation(animBot);
        ApplicationClass.sendMessage("<G" + ApplicationClass.userNickname + ">", getContext());
        etMessage.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getButtonState() == MotionEvent.ACTION_POINTER_DOWN)
                    return false;
            }
        });


        return root;
    }


    private void bt_parsing(String message) {
        if (message.contains("<") && message.contains(">")) {
            char c;
            c = message.charAt(1);
            switch (c) {
                case 't':
                    etPhoneNumber.setText(parsing(message).substring(message.indexOf("<t") + 2, message.lastIndexOf(">")));
                    ApplicationClass.sendMessage("<o>", getContext());
                    progressDialog.incrementProgressBy(1);
                    break;
                case 'o':
                    etRepeatTime.setText(parsing(message).substring(message.indexOf("<o") + 2, message.lastIndexOf(">")));
                    ApplicationClass.sendMessage("<m>", getContext());
                    progressDialog.incrementProgressBy(1);
                    break;

                case 'm':
                    etMail.setText(parsing(message).substring(message.indexOf("<m") + 2, message.lastIndexOf(">")));
                    ApplicationClass.sendMessage("<x>", getContext());
                    progressDialog.incrementProgressBy(1);
                    break;

                case 'x':
                    etMailPassword.setText(parsing(message).substring(message.indexOf("<x") + 2, message.lastIndexOf(">")));
                    ApplicationClass.sendMessage("<r>", getContext());
                    progressDialog.incrementProgressBy(1);
                    break;
                case 'r':
                    etMessage.setText(parsing(message).substring(message.indexOf("<r") + 2, message.lastIndexOf(">")));
                    ApplicationClass.sendMessage("<a>", getContext());
                    progressDialog.incrementProgressBy(1);
                    break;
                case 'a':
                    etPlatform.setText(parsing(message).substring(message.indexOf("<a") + 2, message.lastIndexOf(">")));
                    progressDialog.dismiss();
                    break;

            }
        }
    }

    private String parsing(String message) {
        StringBuilder result = new StringBuilder();
        for (int i = message.indexOf('<') + 2; i < message.indexOf('>') - 1; i++) {
            result.append(message.charAt(i));

        }
        return message;
    }
}