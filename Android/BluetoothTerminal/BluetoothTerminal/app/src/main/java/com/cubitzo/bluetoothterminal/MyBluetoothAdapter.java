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

public class MyBluetoothAdapter extends ArrayAdapter<BluetoothDevice> {
    private final Context context;
    private final ArrayList<BluetoothDevice> values;
    private final int stmrasp;

    public MyBluetoothAdapter(@NonNull Context context, ArrayList<BluetoothDevice> list, int stmrasp) {
        super(context, R.layout.row_layout, list);
        this.context = context;
        this.values = list;
        this.stmrasp = stmrasp;
    }

    @NonNull
    @Override
    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
        LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE); //para aceder aos dados de outros xmls
        View rowView = inflater.inflate(R.layout.row_layout, parent, false);   // funçao que desenha o cenas do xml
        TextView tvBName = rowView.findViewById(R.id.tvBName);
        TextView tvBMac = rowView.findViewById(R.id.tvBmac);

        ImageView ivType = rowView.findViewById(R.id.ivType);

        tvBName.setText(values.get(position).getName());
        tvBMac.setText(values.get(position).getAddress());
        ivType.setImageResource(R.drawable.bluetooth);




        return rowView;
    }

}

