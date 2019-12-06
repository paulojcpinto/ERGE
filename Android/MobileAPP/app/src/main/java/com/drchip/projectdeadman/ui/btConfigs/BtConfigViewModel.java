package com.drchip.projectdeadman.ui.btConfigs;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class BtConfigViewModel extends ViewModel {

    private MutableLiveData<String> mText;

    public BtConfigViewModel() {
        mText = new MutableLiveData<>();
        mText.setValue("This is share fragment");
    }

    public LiveData<String> getText() {
        return mText;
    }
}