package com.drchip.projectdeadman;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.ArrayList;

public class DevicesAdapter extends RecyclerView.Adapter<DevicesAdapter.ViewHolder> {

    private ArrayList<MybluetoothDevice> devices;
    ItemClicked activity;

    public interface ItemClicked  // Interface serve para passar eventos e assim entre classes, ou seja criar uma interface entre classes!
    {
        void onItemClicked(int index);
    }

    public DevicesAdapter(Context context, ArrayList<MybluetoothDevice> list)
    {

        devices= list;
    }

    public  class ViewHolder extends RecyclerView.ViewHolder
    {

        ImageView ivType;
        TextView tvName, tvType, tvMac;
        public ViewHolder(@NonNull View itemView) {
            super(itemView);

           ivType= itemView.findViewById(R.id.ivType);
           tvName = itemView.findViewById(R.id.tvName);
           tvType = itemView.findViewById(R.id.tvType);
           tvMac = itemView.findViewById(R.id.tvMac);

        }
    }
    @NonNull
    @Override
    public DevicesAdapter.ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View v= LayoutInflater.from(parent.getContext()).inflate(R.layout.list_devices,parent,false);

        return new ViewHolder(v);         //muito importante!!!!para linkar com o ivPref por exemplo
    }


    @Override
    public void onBindViewHolder(@NonNull DevicesAdapter.ViewHolder holder, int position) {
        holder.itemView.setTag(devices.get(position));   // quando algem segura o cenas guarda o index!!!!

        holder.tvMac.setText(devices.get(position).device.getAddress());
        holder.tvName.setText(devices.get(position).device.getName());
        holder.tvType.setText(devices.get(position).deviceType);

        if(devices.get(position).deviceType.contains("STM"))
        {
            holder.ivType.setImageResource(R.drawable.stm);
        }
        else if(devices.get(position).deviceType.contains("RASP"))
        {
            holder.ivType.setImageResource(R.drawable.rasp);
        }
        else holder.ivType.setImageResource(R.drawable.not_knowned);

    }


    @Override
    public int getItemCount() {
        return devices.size();
    }
}
