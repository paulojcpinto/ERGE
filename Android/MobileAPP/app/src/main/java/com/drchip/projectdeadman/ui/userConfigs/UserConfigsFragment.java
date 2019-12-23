package com.drchip.projectdeadman.ui.userConfigs;

import android.annotation.SuppressLint;
import android.app.ProgressDialog;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.Toast;

import com.drchip.projectdeadman.ApplicationClass;
import com.drchip.projectdeadman.R;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProviders;

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

    ProgressDialog progressDialog;
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

        progressDialog = new ProgressDialog(getContext());
        progressDialog.setTitle("Communicating");
        progressDialog.setMessage("Loading user data!...");
        progressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
        progressDialog.setMax(6);
        progressDialog.setCancelable(false);
        progressDialog.show();




        return root;
    }
}