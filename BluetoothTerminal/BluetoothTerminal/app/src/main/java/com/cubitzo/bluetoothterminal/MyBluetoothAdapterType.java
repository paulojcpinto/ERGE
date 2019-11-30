package com.cubitzo.bluetoothterminal;

import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.ArrayList;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

public class MyBluetoothAdapterType extends ArrayAdapter<MybluetoothDevice> {
    private final Context context;
    private final ArrayList<MybluetoothDevice> values;

    public MyBluetoothAdapterType(@NonNull Context context, ArrayList<MybluetoothDevice> list) {
        super(context, R.layout.row_layout2, list);
        this.context = context;
        this.values = list;
    }

    @NonNull
    @Override
    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
        LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE); //para aceder aos dados de outros xmls
        View rowView = inflater.inflate(R.layout.row_layout2, parent, false);   // funçao que desenha o cenas do xml
        TextView tvBName = rowView.findViewById(R.id.tvBName);
        TextView tvBMac = rowView.findViewById(R.id.tvBmac);
        TextView tvType = rowView.findViewById(R.id.tvType);

        ImageView ivType = rowView.findViewById(R.id.ivType);

        tvBName.setText(values.get(position).device.getName());
        tvBMac.setText(values.get(position).device.getAddress());
        ivType.setImageResource(R.drawable.bluetooth);
        tvType.setText(values.get(position).deviceType);



        return rowView;
    }

}

