package com.drchip.projectdeadman.ui.userConfigs;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProviders;

import com.drchip.projectdeadman.ApplicationClass;
import com.drchip.projectdeadman.R;
import com.drchip.projectdeadman.Register;
import com.drchip.projectdeadman.UserInstructionsRASP;
import com.drchip.projectdeadman.UserInstuctionsSTM;

import java.util.ArrayList;

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
                    receive_phone_number(readMessage);
                    receive_message(readMessage);
                    receive_repeatTime(readMessage);
                    receive_platform(readMessage);
                    receive_password(readMessage);
                    receive_email(readMessage);

                    Toast.makeText(getContext(), "Message Reciebed:"+readMessage, Toast.LENGTH_SHORT).show();
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
    ImageView ivTop, ivBot,ivPhone,ivRepeat,ivEmail,ivEmailPassword,ivMessage,ivPlatform;

    ImageView[] check= {
            ivPhone,  ivRepeat,ivEmail,ivEmailPassword,ivMessage,ivPlatform
    };

    Animation animTop, animBot;
    Button btnUpdate,btnAddImages;
    Animation rotate,fade_in,fade_out,fade_out2;
    ArrayList<ImageView> toUpdate;

    ProgressDialog progressDialog;

    @SuppressLint("ClickableViewAccessibility")
    @RequiresApi(api = Build.VERSION_CODES.O)
    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        ApplicationClass.mBluetoothConnectionService.updateHandlerContex(mHandler);

        galleryViewModel =
                ViewModelProviders.of(this).get(UserConfigsViewModel.class);
        View root = inflater.inflate(R.layout.fragment_userconfigs, container, false);
        toUpdate = new ArrayList<>();
        etPlatform= root.findViewById(R.id.etPlatform);
        etPhoneNumber= root.findViewById(R.id.etPhoneNumber);
        etRepeatTime= root.findViewById(R.id.etRepeatTime);
        etMail = root.findViewById(R.id.etEmail);
        etMailPassword= root.findViewById(R.id.etEmailPassword);
        etMessage = root.findViewById(R.id.etMessage);
        ivTop = root.findViewById(R.id.ivTop);
        ivBot = root.findViewById(R.id.ivBot);
        ivPhone = root.findViewById(R.id.ivPhone1);
        ivRepeat= root.findViewById(R.id.ivRepeat);
        ivEmail= root.findViewById(R.id.ivEmail);
        ivEmailPassword= root.findViewById(R.id.ivEmailPassword);
        ivMessage = root.findViewById(R.id.ivMessage);
        ivPlatform = root.findViewById(R.id.ivPlatform);
        btnUpdate = root.findViewById(R.id.btnUpdate);
        btnAddImages=root.findViewById(R.id.btnAddImages);

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

        rotate = AnimationUtils.loadAnimation(getContext(), R.anim.rotate);
        rotate.setStartTime(10);
        fade_in = AnimationUtils.loadAnimation(getContext(), R.anim.fade_in);
        fade_in.setStartOffset(1);
        fade_out = AnimationUtils.loadAnimation(getContext(), R.anim.fade_out);
        fade_out2= AnimationUtils.loadAnimation(getContext(), R.anim.fade_out2);


        etMessage.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {

                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    AlertDialog.Builder message = new AlertDialog.Builder(getContext());
                    LayoutInflater inflater = getLayoutInflater();
                    final View dialogView = inflater.inflate(R.layout.message_dialog, null);
                    final EditText etReleaseMessage = dialogView.findViewById(R.id.etReleaseMessage);

                    message.setView(dialogView);
                    message.setTitle("Message to Release");
                    message.setMessage("Enter the message that you want to release here!!");

                    etReleaseMessage.setText(etMessage.getText());
                    message.setPositiveButton("Confirm", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                            if(!etMessage.getText().toString().equals(etReleaseMessage.getText().toString()))
                            {
                                toUpdate.add(ivMessage);
                                ivMessage.startAnimation(fade_out);
                                }

                            etMessage.setText(etReleaseMessage.getText());
                        }
                    });
                    message.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                        }
                    });
                    message.show();
                }

                return false;
            }
        });
        fade_in.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {

            }

            @Override
            public void onAnimationEnd(Animation animation) {
                for(int i=0; i<toUpdate.size();i++)
                {
                    toUpdate.get(i).clearAnimation();
                }
                toUpdate= new ArrayList<>();
                toUpdate.clear();
                fade_in.cancel();
                fade_in.reset();
                fade_out.cancel();
                fade_out.reset();

            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }
        });

        fade_out.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {  }
            @Override
            public void onAnimationEnd(Animation animation) {

          
                for(int i=0; i<toUpdate.size();i++)
                {
                    toUpdate.get(i).clearAnimation();
                    toUpdate.get(i).setImageResource(R.drawable.error);
                    toUpdate.get(i).startAnimation(fade_in);

                }


            }
            @Override
            public void onAnimationRepeat(Animation animation) { }
        });


        fade_out2.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {

            }

            @Override
            public void onAnimationEnd(Animation animation) {
                for(int i=0; i<toUpdate.size();i++)
                {
                    toUpdate.get(i).setImageResource(R.drawable.correct);
                    toUpdate.get(i).startAnimation(fade_in);

                }
//                for(int i=0; i<toUpdate.size();i++)
//                {
//                    toUpdate.get(i).clearAnimation();
//                }
//                toUpdate= new ArrayList<>();
//                toUpdate.clear();
            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }
        });

        btnAddImages.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

            }
        });

        check[0]=ivPhone;
        check[1]=ivRepeat;
        check[2]=ivEmail;
        check[3]=ivEmailPassword;
        check[4]= ivMessage;
        check[5]= ivPlatform;
        etPlatform.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {

                if (event.getAction() == MotionEvent.ACTION_DOWN) {


                    final AlertDialog.Builder message = new AlertDialog.Builder(getContext());
                    LayoutInflater inflater = getLayoutInflater();
                    final View dialogView = inflater.inflate(R.layout.platform_dialog, null);
                    //   final EditText etReleaseMessage = dialogView.findViewById(R.id.etReleaseMessage);
                    final RadioButton rbSMS = dialogView.findViewById(R.id.rbSMS);
                    final RadioButton rbEmail = dialogView.findViewById(R.id.rbEmail);
                    final RadioButton rbTwitter = dialogView.findViewById(R.id.rbTwitter);
                    final EditText etEmail = dialogView.findViewById(R.id.etMail);
                    final EditText etPhone = dialogView.findViewById(R.id.etPhone);
                    final EditText etKey = dialogView.findViewById(R.id.etkey);
                    final TextView tvKey= dialogView.findViewById(R.id.tvKey);
                    final LinearLayout llTwiter= dialogView.findViewById(R.id.llTwitter);


                    message.setView(dialogView);
                    message.setTitle("Platform to Release");
                    message.setMessage("Enter the platform that you want to release here!!");

                    if(ApplicationClass.deviceType.contains("STM"))
                        rbTwitter.setVisibility(View.VISIBLE);
                    else rbTwitter.setVisibility(View.GONE);

                    rbTwitter.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            etPhone.setVisibility(View.GONE);
                            etEmail.setVisibility(View.GONE);
                            llTwiter.setVisibility(View.VISIBLE);
                        }
                    });

                    rbSMS.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            etPhone.setVisibility(View.VISIBLE);
                            etEmail.setVisibility(View.GONE);
                        }
                    });


                    rbEmail.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            etEmail.setVisibility(View.VISIBLE);
                            etPhone.setVisibility(View.GONE);

                        }
                    });
                    tvKey.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            Uri uriUrl = Uri.parse("https://thingspeak.com/apps/thingtweets");
                            Intent launchBrowser = new Intent(Intent.ACTION_VIEW, uriUrl);
                            startActivity(launchBrowser);                        }
                    });

                    message.setPositiveButton("Confirm", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                            if (rbEmail.isChecked()) {
                                if (etEmail.getText().toString().isEmpty()) {
                                    Toast.makeText(getContext(), "Please put an email!", Toast.LENGTH_SHORT).show();
                                    dialog.cancel();
                                } else {
                                    etPlatform.setText("Email " + etEmail.getText().toString().trim());
                                }
                            } else if (rbSMS.isChecked()) {
                                if (etPhone.getText().toString().isEmpty()) {
                                    Toast.makeText(getContext(), "Please put an phone number!", Toast.LENGTH_SHORT).show();
                                    dialog.cancel();

                                } else {
                                    etPlatform.setText("SMS " + etPhone.getText().toString().trim());
                                }
                            }
                            else if(rbTwitter.isChecked())
                            {
                                if(etKey.getText().toString().isEmpty())
                                {
                                    Toast.makeText(getContext(), "Please put an twitter KEY number!", Toast.LENGTH_SHORT).show();
                                    dialog.cancel();
                                }
                                else etPlatform.setText("TWITTER "+etKey.getText().toString().trim());

                            }
                            else {
                                Toast.makeText(getContext(), "Please select an option!!", Toast.LENGTH_SHORT).show();
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

                    return false;
                }
                return false;

            }
        });



        btnUpdate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                for(ImageView img: check)
                {
                    img.setImageResource(R.drawable.loading);
                    img.setAnimation(rotate);
                }

                if(etPhoneNumber.getText().toString().isEmpty())
                {
                    AlertDialog.Builder message = new AlertDialog.Builder(getContext());

                    message.setTitle("Error!");
                    message.setMessage("Please make sure you choose a PHONE NUMBER to receive the sms messages!!!");

                    message.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                        }
                    });
                    message.show();
                }else
                if(etMessage.getText().toString().isEmpty())
                {
                    AlertDialog.Builder message = new AlertDialog.Builder(getContext());

                    message.setTitle("Error!");
                    message.setMessage("Please make sure you choose an MESSAGE to release!!!");

                    message.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                        }
                    });
                    message.show();
                }else
                if(etRepeatTime.getText().toString().isEmpty())
                {
                    AlertDialog.Builder message = new AlertDialog.Builder(getContext());

                    message.setTitle("Error!");
                    message.setMessage("Please make sure you choose an REPEAT TIME to do the presence check!!!");

                    message.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                        }
                    });
                    message.show();
                } else
                if(etPlatform.getText().toString().isEmpty())
                {
                    AlertDialog.Builder message = new AlertDialog.Builder(getContext());

                    message.setTitle("Error!");
                    message.setMessage("Please make sure you choose an PLATFORM to do the release!!!");

                    message.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                        }
                    });
                    message.show();
                }else if((etPhoneNumber.getText().length()<9 && !etPhoneNumber.getText().toString().contains("+"))|| (etPhoneNumber.getText().length()<13 && etPhoneNumber.getText().toString().contains("+")))
                {
                    AlertDialog.Builder message = new AlertDialog.Builder(getContext());

                    message.setTitle("Error!");
                    message.setMessage("Please make sure you enter an correct phone number!!!");

                    message.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int whicaccch) {

                        }
                    });
                    message.show();
                }else
                ApplicationClass.sendMessage("<M"+etMail.getText().toString().trim()+">", getContext());


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

                    etPlatform.addTextChangedListener(new TextWatcher() {
                        @Override
                        public void beforeTextChanged(CharSequence s, int start, int count, int after) {

                        }

                        @Override
                        public void onTextChanged(CharSequence s, int start, int before, int count) {

                        }

                        @Override
                        public void afterTextChanged(Editable s) {
                            toUpdate.add(ivPlatform);
                            ivPlatform.startAnimation(fade_out);
                        }
                    });
                    etMail.addTextChangedListener(new TextWatcher() {
                        @Override
                        public void beforeTextChanged(CharSequence s, int start, int count, int after) {

                        }

                        @Override
                        public void onTextChanged(CharSequence s, int start, int before, int count) {

                        }

                        @Override
                        public void afterTextChanged(Editable s) {

                            toUpdate.add(ivEmail);
                            ivEmail.startAnimation(fade_out);
                        }
                    });
                    etPhoneNumber.addTextChangedListener(new TextWatcher() {
                        @Override
                        public void beforeTextChanged(CharSequence s, int start, int count, int after) {

                        }

                        @Override
                        public void onTextChanged(CharSequence s, int start, int before, int count) {

                        }

                        @Override
                        public void afterTextChanged(Editable s) {

                            toUpdate.add(ivPhone);
                            ivPhone.startAnimation(fade_out);
                        }
                    });
                    etMailPassword.addTextChangedListener(new TextWatcher() {
                        @Override
                        public void beforeTextChanged(CharSequence s, int start, int count, int after) {

                        }

                        @Override
                        public void onTextChanged(CharSequence s, int start, int before, int count) {

                        }

                        @Override
                        public void afterTextChanged(Editable s) {

                            toUpdate.add(ivEmailPassword);
                            ivEmailPassword.startAnimation(fade_out);
                        }
                    });
                    etRepeatTime.addTextChangedListener(new TextWatcher() {
                        @Override
                        public void beforeTextChanged(CharSequence s, int start, int count, int after) {

                        }

                        @Override
                        public void onTextChanged(CharSequence s, int start, int before, int count) {

                        }

                        @Override
                        public void afterTextChanged(Editable s) {
                            toUpdate.add(ivRepeat);
                            ivRepeat.startAnimation(fade_out);
                        }
                    });
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
    void receive_email(String readMessage)
    {
        if(readMessage.contains("<M") && readMessage.contains((">"))) {

            StringBuilder aux = new StringBuilder();
            for (int i = 2; i < readMessage.length() - 1; i++) {
                aux.append(readMessage.charAt(i));
            }
            int lengh = 0;
            try {
                lengh = Integer.parseInt(aux.toString());
            } catch (NumberFormatException nfe) {
                Toast.makeText(getContext(), "Error Parsing Number: " + nfe.getMessage(), Toast.LENGTH_SHORT).show();
            }
            if (lengh != etMail.getText().toString().trim().length()) {
                Toast.makeText(getContext(), "<Error sending email, receibed: " + lengh + " instead of:" + etMail.getText().toString().trim().length(), Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<M" + etMail.getText().toString().trim() + ">", getContext());

            } else {
                toUpdate.add(ivEmail);
                ivEmail.startAnimation(fade_out2);

                ApplicationClass.sendMessage("<X"+etMailPassword.getText().toString().trim()+">", getContext());

            }
        }
    }

    void receive_password(String readMessage)
    {
        if(readMessage.contains("<X") && readMessage.contains((">"))) {

            StringBuilder aux = new StringBuilder();
            for (int i = 2; i < readMessage.length() - 1; i++) {
                aux.append(readMessage.charAt(i));
            }
            int lengh = 0;
            try {
                lengh = Integer.parseInt(aux.toString());
            } catch (NumberFormatException nfe) {
                Toast.makeText(getContext(), "Error Parsing Number: " + nfe.getMessage(), Toast.LENGTH_SHORT).show();
            }
            if (lengh != etMailPassword.getText().toString().trim().length()) {
                Toast.makeText(getContext(), "<Error sending email password, receibed: " + lengh + " instead of:" + etMailPassword.getText().toString().trim().length(), Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<X" + etMailPassword.getText().toString().trim() + ">", getContext());

            } else {
                toUpdate.add(ivEmailPassword);
                ivEmailPassword.startAnimation(fade_out2);
                ApplicationClass.sendMessage("<T"+etPhoneNumber.getText().toString().trim()+">", getContext());

            }
        }
    }
    void receive_phone_number(String readMessage)
    {
        if(readMessage.contains("<T") && readMessage.contains((">"))) {

            StringBuilder aux = new StringBuilder();
            for (int i = 2; i < readMessage.length() - 1; i++) {
                aux.append(readMessage.charAt(i));
            }
            int lengh = 0;
            try {
                lengh = Integer.parseInt(aux.toString());
            } catch (NumberFormatException nfe) {
                Toast.makeText(getContext(), "Error Parsing Number: " + nfe.getMessage(), Toast.LENGTH_SHORT).show();
            }
            if (lengh != etPhoneNumber.getText().toString().trim().length()) {
                ApplicationClass.sendMessage("<T" + etPhoneNumber.getText().toString().trim() + ">", getContext());

            } else {
                Toast.makeText(getContext(), "Sended phone number", Toast.LENGTH_SHORT).show();
                toUpdate.add(ivPhone);
                ivPhone.startAnimation(fade_out2);
                ApplicationClass.sendMessage("<R"+etMessage.getText().toString().trim()+">", getContext());

            }
        }
    }

    void receive_message(String readMessage) {
        if (readMessage.contains("<R") && readMessage.contains((">"))) {

            StringBuilder aux = new StringBuilder();
            for (int i = 2; i < readMessage.length() - 1; i++) {
                aux.append(readMessage.charAt(i));
            }
            int lengh = 0;
            try {
                lengh = Integer.parseInt(aux.toString());
            } catch (NumberFormatException nfe) {
                Toast.makeText(getContext(), "Error Parsing Number: " + nfe.getMessage(), Toast.LENGTH_SHORT).show();
            }
            if (lengh != etMessage.getText().toString().trim().length()) {
                Toast.makeText(getContext(), "<Error sending message to release, receibed: " + lengh + " instead of:" + etMessage.getText().toString().trim().length(), Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<R" + etMessage.getText().toString().trim() + ">", getContext());

            } else {
                toUpdate.add(ivMessage);
                ivMessage.startAnimation(fade_out2);
                ApplicationClass.sendMessage("<O"+etRepeatTime.getText().toString().trim()+">", getContext());

            }
        }
    }

    void receive_repeatTime(String readMessage)
    {
        if (readMessage.contains("<O") && readMessage.contains((">"))) {

            StringBuilder aux = new StringBuilder();
            for (int i = 2; i < readMessage.length() - 1; i++) {
                aux.append(readMessage.charAt(i));
            }
            int lengh = 0;
            try {
                lengh = Integer.parseInt(aux.toString());
            } catch (NumberFormatException nfe) {
                Toast.makeText(getContext(), "Error Parsing Number: " + nfe.getMessage(), Toast.LENGTH_SHORT).show();
            }
            if (lengh != etRepeatTime.getText().toString().trim().length()) {
                Toast.makeText(getContext(), "Error sending repeat time, receibed: " + lengh + " instead of:" + etRepeatTime.getText().toString().trim().length(), Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<O" + etRepeatTime.getText().toString().trim() + ">", getContext());

            } else {
                toUpdate.add(ivRepeat);
                ivRepeat.startAnimation(fade_out2);
                ApplicationClass.sendMessage("<A"+etPlatform.getText().toString().trim()+">", getContext());

            }
        }
    }
    void receive_platform(String readMessage)
    {
        if (readMessage.contains("<A") && readMessage.contains((">"))) {

            StringBuilder aux = new StringBuilder();
            for (int i = 2; i < readMessage.length() - 1; i++) {
                aux.append(readMessage.charAt(i));
            }
            int lengh = 0;
            try {
                lengh = Integer.parseInt(aux.toString());
            } catch (NumberFormatException nfe) {
                Toast.makeText(getContext(), "Error Parsing Number: " + nfe.getMessage(), Toast.LENGTH_SHORT).show();
            }
            if (lengh != etPlatform.getText().toString().trim().length()) {
                Toast.makeText(getContext(), "<Error sending Date to start, receibed: " + lengh + " instead of:" + etPlatform.getText().toString().trim().length(), Toast.LENGTH_SHORT).show();
                ApplicationClass.sendMessage("<A" + etPlatform.getText().toString().trim() + ">", getContext());

            } else {
                ApplicationClass.sendMessage("<U>",getContext());
                toUpdate.add(ivPlatform);
                ivPlatform.startAnimation(fade_out2);

                        //startActivityForResult(new Intent(Register.this, UserInstuctionsSTM.class), 1);



            }
        }
    }
}