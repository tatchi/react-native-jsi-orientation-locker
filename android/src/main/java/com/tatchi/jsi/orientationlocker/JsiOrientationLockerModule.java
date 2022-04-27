package com.tatchi.jsi.orientationlocker;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.hardware.SensorManager;
import android.os.Build;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.Display;
import android.view.OrientationEventListener;
import android.view.Surface;
import android.view.WindowManager;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.JavaScriptContextHolder;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.module.annotations.ReactModule;
import com.facebook.react.bridge.ReactMethod;

import com.facebook.react.turbomodule.core.CallInvokerHolderImpl;

@ReactModule(name = JsiOrientationLockerModule.NAME)
public class JsiOrientationLockerModule extends ReactContextBaseJavaModule {
  public static final String NAME = "JsiOrientationLocker";

  public JsiOrientationLockerModule(ReactApplicationContext reactContext) {
    super(reactContext);
  }

  @Override
  @NonNull
  public String getName() {
    return NAME;
  }

  @ReactMethod(isBlockingSynchronousMethod = true)
  public void install() {
    try {
      System.loadLibrary("react-native-jsi-orientation-locker");
      JsiBridge.instance.install(getReactApplicationContext());
    } catch (Exception exception) {
      Log.e(NAME, "Failed to install JSI Bindings!", exception);
    }
  }
}
