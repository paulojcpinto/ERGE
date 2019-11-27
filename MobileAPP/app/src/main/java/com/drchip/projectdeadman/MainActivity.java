package com.drchip.projectdeadman;

import android.bluetooth.BluetoothDevice;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.Toast;

import com.google.android.material.navigation.NavigationView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.navigation.ui.NavigationUI;

public class MainActivity extends AppCompatActivity {

    private AppBarConfiguration mAppBarConfiguration;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        DrawerLayout drawer = findViewById(R.id.drawer_layout);
        NavigationView navigationView = findViewById(R.id.nav_view);
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        mAppBarConfiguration = new AppBarConfiguration.Builder(
                R.id.nav_home, R.id.nav_user_configs, R.id.nav_user_activity,
                R.id.nav_btconfigs, R.id.nav_terminal)
                .setDrawerLayout(drawer)
                .build();
        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment);
        NavigationUI.setupActionBarWithNavController(this, navController, mAppBarConfiguration);
        NavigationUI.setupWithNavController(navigationView, navController);

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.main, menu);
        ApplicationClass.playMenu = menu.findItem(R.id.BluetoothState);
        ApplicationClass.DeviceType = menu.findItem(R.id.DeviceTyepe);
        if (ApplicationClass.deviceConnected) {
            if (ApplicationClass.deviceType.contains("STM"))
                ApplicationClass.DeviceType.setIcon(R.drawable.stm);
            else if (ApplicationClass.deviceType.contains("RASP"))
                ApplicationClass.DeviceType.setIcon(R.drawable.rasp);
            else ApplicationClass.DeviceType.setIcon(R.drawable.not_knowned);
            ApplicationClass.playMenu.setIcon(R.drawable.bluetooth_on);
        } else {
            ApplicationClass.playMenu.setIcon(R.drawable.bluetooth_off);
        }
        if (ApplicationClass.deviceConnected) {
            ApplicationClass.playMenu.setIcon(R.drawable.bluetooth_on);
        } else {
            ApplicationClass.playMenu.setIcon(R.drawable.bluetooth_off);
        }

        return super.onCreateOptionsMenu(menu);

    }

    public boolean onOptionsItemSelected(@NonNull MenuItem item) {   // override metodos em code !!!

        switch (item.getItemId()) {

            case R.id.BluetoothState:
                if (ApplicationClass.deviceConnected) {
                    Toast.makeText(this, "Device is already connected", Toast.LENGTH_SHORT).show();
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

    @Override
    public boolean onSupportNavigateUp() {
        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment);
        return NavigationUI.navigateUp(navController, mAppBarConfiguration)
                || super.onSupportNavigateUp();
    }
}
