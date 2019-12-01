package com.cubitzo.bluetoothterminal;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.app.Application;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.RadioButton;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.material.snackbar.Snackbar;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import java.util.Set;
import java.util.StringTokenizer;

public class Enter extends AppCompatActivity {

    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;
    public static final int CONNECTED_SUCCESS = 6;
    public static final String DEVICE_NAME = "device_name";
    public static final String TOAST = "toast";
    Animation rotate;
    ArrayList<MybluetoothDevice> loadedDevices;
    ArrayList<BluetoothDevice> listb;
    TextView tvDevice,tvMacAdress, tvType;
    Button btnMac, btnTerminal, btnConnect;
    ImageView ivStatus;
    private Set<BluetoothDevice> pairedDevices;
    boolean choosed, connected;
    Animation fade_in;
    Animation fade_out;
    String name;

    @SuppressLint("HandlerLeak")
    private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {

                case MESSAGE_READ:
                    byte[] readBuf = (byte[]) msg.obj;
                    String readMessage = new String(readBuf, 0, msg.arg1);

                    Toast.makeText(Enter.this, "Receibed " + readMessage, Toast.LENGTH_SHORT).show();
                    break;

                case MESSAGE_TOAST:
                    if (msg.getData().getString(TOAST).contains("Device connection was lost")) {
                        connected = false;
                        // ivStatus.setImageResource(R.drawable.error);
                        ivStatus.clearAnimation();
                        ivStatus.startAnimation(fade_out);
                        btnTerminal.setVisibility(View.GONE);
                    }
                    Toast.makeText(getApplicationContext(), msg.getData().getString(TOAST),
                            Toast.LENGTH_SHORT).show();
                    break;
                case CONNECTED_SUCCESS:

                    connected = true;
                    ivStatus.clearAnimation();
                    ivStatus.setImageResource(R.drawable.done);
                    ivStatus.startAnimation(fade_in);
                    btnTerminal.startAnimation(fade_in);
                    btnTerminal.setVisibility(View.VISIBLE);
                    boolean canSave = true;
                    for (int i = 0; i < loadedDevices.size(); i++) {
                        if (loadedDevices.get(i).device.getAddress().equals(ApplicationClass.target.getAddress())) {
                            canSave = false;
                        }
                    }
                    if (canSave) {
                        loadedDevices.add(new MybluetoothDevice(ApplicationClass.target, ApplicationClass.deviceType));
                        saveDevice();
                    }
                    break;
            }
        }
    };


    @Override
       protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_enter);

        tvDevice = findViewById(R.id.tvDevice);
        tvMacAdress = findViewById(R.id.tvMacAdress);
        btnMac = findViewById(R.id.btnMac);
        btnTerminal = findViewById(R.id.btnTerminal);
        ivStatus = findViewById(R.id.ivStatus);
        tvType = findViewById(R.id.tvType);
        btnConnect = findViewById(R.id.btnConnect);

        rotate = AnimationUtils.loadAnimation(this, R.anim.rotate);
        rotate.setStartTime(10);
        fade_in = AnimationUtils.loadAnimation(this, R.anim.fade_in);
        fade_in.setStartOffset(1);
        fade_out = AnimationUtils.loadAnimation(this, R.anim.fade_out);
        name = "";

        connected = false;
        listb = new ArrayList<BluetoothDevice>();
        loadedDevices = new ArrayList<>();
        ApplicationClass.mBluetoothConnectionService.updateHandlerContex(mHandler);
        loadData();

        btnTerminal.setVisibility(View.GONE);

        if (loadedDevices.size() >= 1) {
            tvDevice.setText(loadedDevices.get(loadedDevices.size() - 1).device.getName());
            tvMacAdress.setText(loadedDevices.get(loadedDevices.size()-1).device.getAddress());
            tvType.setText(loadedDevices.get(loadedDevices.size()-1).deviceType);
            ApplicationClass.deviceType = loadedDevices.get(loadedDevices.size()-1).deviceType;
            ApplicationClass.target = loadedDevices.get(loadedDevices.size() - 1).device;
        }
        if (!ApplicationClass.BA.isEnabled()) {
            Intent turnOn = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            Snackbar.make(findViewById(R.id.activity_enter), "Please turn on bluetooth", Snackbar.LENGTH_SHORT).show();

            Toast.makeText(getApplicationContext(), "Turned on", Toast.LENGTH_LONG).show();
            startActivityForResult(turnOn, 0);
        }

        if (ApplicationClass.deviceConnected) {
            connected = true;
            tvDevice.setText(ApplicationClass.target.getName());
            ivStatus.setImageResource(R.drawable.done);
        }

        fade_out.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {

            }

            @Override
            public void onAnimationEnd(Animation animation) {
                ivStatus.clearAnimation();

                ivStatus.setImageResource(R.drawable.error);
                ivStatus.startAnimation(fade_in);

            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }
        });

        ivStatus.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                if (!connected) {

                    // Get the BLuetoothDevice object
                    //String address = data.getExtras().getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
                    //
                    if(tvType.getText().toString().contains("Click HERE!!!")){

                        Snackbar.make(findViewById(android.R.id.content), "Please enter an Device type", Snackbar.LENGTH_SHORT).show();


                    }
                    else {
                        ivStatus.setImageResource(R.drawable.loading2);
                        ivStatus.clearAnimation();
                        ivStatus.startAnimation(rotate);
                        BluetoothDevice device = ApplicationClass.BA.getRemoteDevice(ApplicationClass.target.getAddress());
                        // Attempt to connect to the device
                        ApplicationClass.mBluetoothConnectionService.connect(device);
                    }




                } else {

                    ApplicationClass.mBluetoothConnectionService.stop();
                    connected = false;
                    //  ivStatus.setImageResource(R.drawable.error);
                    //Toast.makeText(Enter.this, "You closed the connection", Toast.LENGTH_SHORT).show();


                }
            }
        });
        tvType.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {

                AlertDialog.Builder message = new AlertDialog.Builder(Enter.this);
                LayoutInflater inflater = getLayoutInflater();
                final View dialogView = inflater.inflate(R.layout.device_type_dialog, null);
                final EditText etType = dialogView.findViewById(R.id.etType);

                message.setView(dialogView);
                message.setTitle("Device type");
                message.setMessage("Enter the device type for better organization");

                message.setPositiveButton("Confirm", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                        if(etType.getText().toString().isEmpty()) {
                            Toast.makeText(Enter.this, "Please select a device type", Toast.LENGTH_SHORT).show();

                        }
                        else {

                            tvType.setText(etType.getText().toString().trim());
                            ApplicationClass.deviceType = etType.getText().toString().trim();
                        }
                    }
                });
                message.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                    }
                });
                message.show();


                //if(connected)
                //{
                  //  loadedDevices.remove(loadedDevices.size());
                //  loadedDevices.add(new MybluetoothDevice(ApplicationClass.target,))
                //}
            }
        });

        btnConnect.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!connected) {


                    if(tvType.getText().toString().contains("Click HERE!!!")){

                        Snackbar.make(findViewById(android.R.id.content), "Please enter an Device type", Snackbar.LENGTH_SHORT).show();


                    }
                    else {
                        ivStatus.setImageResource(R.drawable.loading2);
                        ivStatus.clearAnimation();
                        ivStatus.startAnimation(rotate);
                        BluetoothDevice device = ApplicationClass.BA.getRemoteDevice(ApplicationClass.target.getAddress());
                        // Attempt to connect to the device
                        ApplicationClass.mBluetoothConnectionService.connect(device);
                    }


                    // for (int i = 0; i < 10000000; i++) for (int j = 0; j < 115; j++) ;
//                    ApplicationClass.sendMessage("O", Enter.this);
//
//                    ApplicationClass.sendMessage("<L>", Enter.this);

                } else {

                    ApplicationClass.mBluetoothConnectionService.stop();
                    connected = false;
                }
            }
        });

        btnMac.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                final AlertDialog.Builder message = new AlertDialog.Builder(Enter.this);
                LayoutInflater inflater = getLayoutInflater();
                final View dialogView = inflater.inflate(R.layout.bt_mac_adress, null);
                final EditText etMac = dialogView.findViewById(R.id.etMac);

                message.setView(dialogView);
                message.setTitle("Device Mac");
                message.setMessage("Enter the device mac address to connect with him");

                etMac.addTextChangedListener(new TextWatcher() {
                    @Override
                    public void beforeTextChanged(CharSequence s, int start, int count, int after) {

                    }

                    @Override
                    public void onTextChanged(CharSequence s, int start, int before, int count) {
                        if(count ==1)
                        if((etMac.getText().toString().length()+1)%3==0)
                        //if(etMac.getText().toString().length() == 2 ||etMac.getText().toString().length() == 5||etMac.getText().toString().length() ==7 )
                        {
                            //SystemClock.sleep(1); //pause and wait for rest of data. Adjust this depending on your sending speed.

                            //etMac.setAllCaps(true);
                            etMac.append(":");
                        }
                    }

                    @Override
                    public void afterTextChanged(Editable s) {



                    }
                });

                message.setPositiveButton("Confirm", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                        if(etMac.getText().toString().length() != 17)
                            Snackbar.make(findViewById(android.R.id.content), "Please enter a valid mac", Snackbar.LENGTH_SHORT).show();
                        else
                        {
                            tvMacAdress.setText(etMac.getText().toString().trim());
                            ivStatus.setImageResource(R.drawable.loading2);
                            ivStatus.clearAnimation();
                            ivStatus.startAnimation(rotate);
                            BluetoothDevice device = ApplicationClass.BA.getRemoteDevice(tvMacAdress.getText().toString());
                            // Attempt to connect to the device
                            ApplicationClass.mBluetoothConnectionService.connect(device);
                        }



                    }
                });

                message.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                    }
                });                message.show();
            }
        });

        tvMacAdress.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                ClipboardManager clipboard = (ClipboardManager)v.getContext().getSystemService(Context.CLIPBOARD_SERVICE);
                ClipData clip = ClipData.newPlainText("MacAddress", tvMacAdress.getText().toString());
                clipboard.setPrimaryClip(clip);
                Snackbar.make(v,"Mac Address copied",Snackbar.LENGTH_SHORT).show();
            }
        });
        btnTerminal.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                if(connected)
                startActivity(new Intent(Enter.this,MainActivity.class));
                else                 Snackbar.make(v,"Please Connect to a device first",Snackbar.LENGTH_SHORT).show();

                //Enter.this.finish();
            }
        });
    }
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {

        getMenuInflater().inflate(R.menu.enter_menu, menu);


        return super.onCreateOptionsMenu(menu);
    }
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {   // override metodos em code !!!

        switch (item.getItemId()) {

            case R.id.addDevice:

                final AlertDialog.Builder message = new AlertDialog.Builder(Enter.this);
                LayoutInflater inflater = getLayoutInflater();
                final View dialogView = inflater.inflate(R.layout.paired_devices, null);
                final ListView lv = dialogView.findViewById(R.id.lvPairedDevices);

                final TextView tvBname = dialogView.findViewById(R.id.tvBName);
                final TextView tvBmac = dialogView.findViewById(R.id.tvBmac);
                final ImageView ivType = dialogView.findViewById(R.id.ivType);
                final LinearLayout llSelected = dialogView.findViewById(R.id.llSelected);
                choosed = false;
                llSelected.setVisibility(View.GONE);



                message.setView(dialogView);
                message.setTitle("Bluetooth Devices");
                message.setMessage("Select the device that you want to add!!");

                pairedDevices = ApplicationClass.BA.getBondedDevices();
                listb.addAll(pairedDevices);
                MyBluetoothAdapter adapter = new MyBluetoothAdapter(getApplicationContext(), listb, 3);
                lv.setAdapter(adapter);
                llSelected.setVisibility(View.GONE);
                lv.setVisibility(View.VISIBLE);


                lv.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                    @Override
                    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                        ApplicationClass.target = listb.get(position);

                        //  Toast.makeText(Enter.this, "Device added succesfully " + listb.get(position).getName(), Toast.LENGTH_SHORT).show();
                        tvBname.setText(listb.get(position).getName());
                        tvBmac.setText(listb.get(position).getAddress());

                        lv.setVisibility(View.GONE);

                        llSelected.setVisibility(View.VISIBLE);
                        choosed = true;

                    }
                });
                message.setPositiveButton("Comfirm", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                        if (choosed) {
                            Toast.makeText(Enter.this, "Selected succesfuly", Toast.LENGTH_SHORT).show();
                            tvDevice.setText(ApplicationClass.target.getName());
                            tvMacAdress.setText(ApplicationClass.target.getAddress());
                            Enter.this.tvType.setText("Click HERE!!!");

                        } else {
                            Toast.makeText(Enter.this, "Please select one device", Toast.LENGTH_SHORT).show();
                            dialog.cancel();
                        }

                    }
                });
                message.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                    }
                });
                message.show();


                break;

            case R.id.SelectDevice:

                final AlertDialog.Builder message1 = new AlertDialog.Builder(Enter.this);
                LayoutInflater inflater1 = getLayoutInflater();
                final View dialogView1 = inflater1.inflate(R.layout.known_devices, null);
                //   final EditText etReleaseMessage = dialogView.findViewById(R.id.etReleaseMessage);
                final ListView lv1 = dialogView1.findViewById(R.id.lvPairedDevices);

                final TextView tvBname1 = dialogView1.findViewById(R.id.tvBName);
                final TextView tvBmac1 = dialogView1.findViewById(R.id.tvBmac);
                final ImageView ivType1 = dialogView1.findViewById(R.id.ivType);
                final TextView tvType = dialogView1.findViewById(R.id.tvType);
                final LinearLayout llSelected1 = dialogView1.findViewById(R.id.llSelected);
                choosed = false;
                llSelected1.setVisibility(View.GONE);

                final ArrayList<BluetoothDevice> auxBluetooth = new ArrayList<>();


                // final ArrayAdapter adapter = new ArrayAdapter(this, android.R.layout.simple_list_item_1, list);


                message1.setView(dialogView1);
                message1.setTitle("Bluetooth Devices");
                message1.setMessage("Select the device that you want to select!!");




                        auxBluetooth.clear();

                        for (int i = 0; i < loadedDevices.size(); i++) {
                            if (loadedDevices.get(i).deviceType.contains("STM"))
                                auxBluetooth.add(loadedDevices.get(i).device);
                        }

                        MyBluetoothAdapterType adapter1 = new MyBluetoothAdapterType(getApplicationContext(), loadedDevices);

                        choosed = false;
                        lv1.setAdapter(adapter1);

                        llSelected1.setVisibility(View.GONE);
                        lv1.setVisibility(View.VISIBLE);





                lv1.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                    @Override
                    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                        ApplicationClass.target = listb.get(position);

                        //  Toast.makeText(Enter.this, "Device added succesfully " + listb.get(position).getName(), Toast.LENGTH_SHORT).show();
                        tvBname1.setText(listb.get(position).getName());
                        tvBmac1.setText(listb.get(position).getAddress());
                        tvType.setText(loadedDevices.get(position).deviceType);

                        lv1.setVisibility(View.GONE);

                        llSelected1.setVisibility(View.VISIBLE);
                        choosed = true;

                    }
                });


                message1.setPositiveButton("Comfirm", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                        if (choosed) {
                            Toast.makeText(Enter.this, "Selected succesfuly", Toast.LENGTH_SHORT).show();
                            tvDevice.setText(ApplicationClass.target.getName());
                            tvMacAdress.setText(ApplicationClass.target.getAddress());
                            Enter.this.tvType.setText("Click HERE!!!");


                        } else {
                            Toast.makeText(Enter.this, "Please select one device", Toast.LENGTH_SHORT).show();
                            dialog.cancel();
                        }

                    }
                });
                message1.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                    }
                });
                message1.show();


                break;
//            case  R.id.refresh:
//                Toast.makeText(this, "Refresh Selected", Toast.LENGTH_SHORT).show();
//                break;
//            case R.id.send:
//                Toast.makeText(this, "Send Clicked", Toast.LENGTH_SHORT).show();
//                break;


        }
        return super.onOptionsItemSelected(item);

    }
    public void saveDevice() {

        try {
            FileOutputStream file = openFileOutput("bluetooth.txt", MODE_PRIVATE);  //cria o ficheiro caso nao exitsa, e define a permisao do ficheiro para so o nossa aplicaçao
            OutputStreamWriter outputFile = new OutputStreamWriter(file);  //cria a connecao com o ficheiro que vamos escrever

            for (int i = 0; i < loadedDevices.size(); i++) {
                outputFile.write(loadedDevices.get(i).device.getName() + "," + loadedDevices.get(i).device.getAddress() + "," + loadedDevices.get(i).deviceType);


            }
            outputFile.flush();
            outputFile.close();
           // Toast.makeText(this, "Sucessfully savedes eyeyeyyeyeyeyeyeyeye!", Toast.LENGTH_LONG).show();

        } catch (IOException e) {
            Toast.makeText(this, e.getMessage(), Toast.LENGTH_SHORT).show();
        }

    }

    public void loadData() {
        loadedDevices.clear();
        File file = getApplicationContext().getFileStreamPath("bluetooth.txt");
        String linefromFile;
        if (file.exists()) {
            try {

                BufferedReader reader = new BufferedReader(new InputStreamReader(openFileInput("bluetooth.txt")));  //abre p ficheiro Data.txt para leitura!
                while ((linefromFile = reader.readLine()) != null) {
                    StringTokenizer tokens = new StringTokenizer(linefromFile, ",");
                    String DeviceName = tokens.nextToken();
                    String DeviceMac = tokens.nextToken();
                    String type = tokens.nextToken();
                    Set<BluetoothDevice> auxDevices = ApplicationClass.BA.getBondedDevices();
                    for (BluetoothDevice test : auxDevices) {
                        if (test.getName().equals(DeviceName)) {
                            if (test.getAddress().equals(DeviceMac)) {
                                loadedDevices.add(new MybluetoothDevice(test, type));
                            }


                        }
                    }

                }

            } catch (IOException e) {
                Toast.makeText(this, e.getMessage(), Toast.LENGTH_SHORT).show();
            }
        } else

            tvDevice.setText("No device selected!");

    }

}
