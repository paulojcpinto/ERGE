package com.drchip.projectdeadman.ui.userConfigs;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class UserConfigsViewModel extends ViewModel {

    private MutableLiveData<String> mText;

    public UserConfigsViewModel() {
        mText = new MutableLiveData<>();
        mText.setValue("This is gallery fragment");
    }

    public LiveData<String> getText() {
        return mText;
    }
}