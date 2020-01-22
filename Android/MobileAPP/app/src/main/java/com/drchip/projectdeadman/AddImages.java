package com.drchip.projectdeadman;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

public class AddImages extends AppCompatActivity {

    TextView tvNumber, tvNumberImages;
    ImageView ivImageCount;
    Button btnConfirm;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_images);

        tvNumber= findViewById(R.id.tvNumber);
        tvNumberImages = findViewById(R.id.tvNumberIMages);
        ivImageCount = findViewById(R.id.ivImageCount);
        btnConfirm = findViewById(R.id.btnConfirm);


    }
}
