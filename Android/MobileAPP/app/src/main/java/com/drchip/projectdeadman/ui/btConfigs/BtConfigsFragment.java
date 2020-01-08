package com.drchip.projectdeadman.ui.btConfigs;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothDevice;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.drchip.projectdeadman.ApplicationClass;
import com.drchip.projectdeadman.DevicesAdapter;
import com.drchip.projectdeadman.MybluetoothDevice;
import com.drchip.projectdeadman.R;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProviders;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import java.util.Objects;
import java.util.Set;
import java.util.StringTokenizer;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Set;
import java.util.StringTokenizer;

public class BtConfigsFragment extends Fragment {

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
    private BtConfigViewModel btConfigViewModel;
    RecyclerView recyclerView;
    RecyclerView.Adapter myAdapter;
    RecyclerView.LayoutManager layoutManager;
    ArrayList<MybluetoothDevice> devices;

    TextView tvDeviceName, tvDeviceType,tvTime;
    Button btnDisconnect, btnReconnect;
    ImageView ivClock, ivType;
    Animation blink;


    Handler timerHandler = new Handler();

    Runnable timerRunnable = new Runnable() {

        @Override
        public void run() {
            //tvTime.setText(((SystemClock.elapsedRealtime()-ApplicationClass.conectedTime )/1000)/60+"min");

            //long millis = System.currentTimeMillis() - startTime;
            long millis =SystemClock.elapsedRealtime()-ApplicationClass.conectedTime;

            int seconds = (int) (millis / 1000);
            int minutes = seconds / 60;
            seconds = seconds % 60;
         tvTime.setText(String.format("%d:%02d", minutes, seconds));

            timerHandler.postDelayed(this, 500);
        }
    };

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        ApplicationClass.mBluetoothConnectionService.updateHandlerContex(mHandler);

        btConfigViewModel =
                ViewModelProviders.of(this).get(BtConfigViewModel.class);
        View root = inflater.inflate(R.layout.fragment_btconfigs, container, false);

      tvDeviceName = root.findViewById(R.id.tvDeviceName);
      tvDeviceType = root.findViewById(R.id.tvDeviceType);
      tvTime = root.findViewById(R.id.tvTime);
      btnDisconnect = root.findViewById(R.id.btnDisconect);
      btnReconnect= root.findViewById(R.id.btnReconnect);
      ivClock = root.findViewById(R.id.ivClock);
      ivType = root.findViewById(R.id.ivType);

      if(ApplicationClass.deviceType.equals("STM"))
          ivType.setImageResource(R.drawable.stm);
      else if(ApplicationClass.deviceType.equals("RASP"))
          ivType.setImageResource(R.drawable.rasp);
      else ivType.setImageResource(R.drawable.not_knowned);


        recyclerView= (RecyclerView) root.findViewById(R.id.list);
        recyclerView.setHasFixedSize(true);
        layoutManager = new LinearLayoutManager(getActivity());
        recyclerView.setLayoutManager(layoutManager);
        devices = new ArrayList<>();
        loadData();

        tvDeviceName.setText(ApplicationClass.target.getName());
        tvDeviceType.setText(ApplicationClass.deviceType);

        blink = AnimationUtils.loadAnimation(getContext(), R.anim.blink);
        tvTime.setText(((SystemClock.elapsedRealtime()-ApplicationClass.conectedTime )/1000)/60+"min");

        ivClock.setAnimation(blink);

        blink.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {

            }

            @Override
            public void onAnimationEnd(Animation animation) {

            }

            @Override
            public void onAnimationRepeat(Animation animation) {

                tvTime.setText(((SystemClock.elapsedRealtime()-ApplicationClass.conectedTime )/1000)/60+"min");

            }
        });
        timerHandler.postDelayed(timerRunnable, 0);
        myAdapter = new DevicesAdapter(getContext(),devices);
        recyclerView.setAdapter(myAdapter);

        return root;
    }
    public void loadData() {
        devices.clear();
        File file = Objects.requireNonNull(getContext()).getFileStreamPath("bluetooth.txt");
        String linefromFile;
        if (file.exists()) {
            try {

                BufferedReader reader = new BufferedReader(new InputStreamReader(Objects.requireNonNull(getActivity()).openFileInput("bluetooth.txt")));  //abre p ficheiro Data.txt para leitura!
                while ((linefromFile = reader.readLine()) != null) {
                    StringTokenizer tokens = new StringTokenizer(linefromFile, ",");
                    String DeviceMac = tokens.nextToken();
                    String type = tokens.nextToken();
                    Set<BluetoothDevice> auxDevices = ApplicationClass.BA.getBondedDevices();
                    for (BluetoothDevice test : auxDevices) {

                        if (test.getAddress().equals(DeviceMac)) {
                            devices.add(new MybluetoothDevice(test, type));
                        }


                    }

                }

            } catch (IOException e) {
                Toast.makeText(getContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
            }
        }
    }

}