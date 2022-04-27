package com.tatchi.jsi.orientationlocker;

import android.util.Log;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.turbomodule.core.CallInvokerHolderImpl;

public class JsiBridge {
  private native void installNativeJsi(long jsContextNativePointer,
      CallInvokerHolderImpl jsCallInvokerHolder, OrientationLocker orientationLocker);

  public static final JsiBridge instance = new JsiBridge();

  public void install(ReactApplicationContext context) {
    long jsContextPointer = context.getJavaScriptContextHolder().get();
    CallInvokerHolderImpl jsCallInvokerHolder = (CallInvokerHolderImpl) context.getCatalystInstance()
        .getJSCallInvokerHolder();

    Log.i("JsiBridge", "IN INSTALL");

    OrientationLocker orientationLocker = new OrientationLocker(context);

    installNativeJsi(
        jsContextPointer,
        jsCallInvokerHolder,
        orientationLocker);
  }
}
