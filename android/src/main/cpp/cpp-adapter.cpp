#include <jni.h>
#include <fbjni/fbjni.h>
#include <jsi/jsi.h>
#include <ReactCommon/CallInvokerHolder.h>
#include <typeinfo>
#include <android/log.h>

#include "JOrientationLocker.h"

using namespace std;
using namespace facebook;
using namespace facebook::jsi;

struct JsiBridge : JavaClass<JsiBridge>
{
    static constexpr auto kJavaDescriptor = "Lcom/tatchi/jsi/orientationlocker/JsiBridge;";

    static void registerNatives()
    {
        javaClassStatic()->registerNatives({// initialization for JSI
                                            makeNativeMethod("installNativeJsi", JsiBridge::installNativeJsi),
                                            makeNativeMethod("callValue", JsiBridge::callValue)

        });
    }

private:
    static void callValue(jni::alias_ref<jni::JClass> clazz)
    {
        __android_log_write(ANDROID_LOG_INFO, "COCO TAG", "IN callValue");
    }

    static void installNativeJsi(jni::alias_ref<jni::JObject> thiz,
                                 jlong jsiRuntimePtr,
                                 jni::alias_ref<react::CallInvokerHolder::javaobject> jsCallInvokerHolder,
                                 jni::alias_ref<JOrientationLocker::javaobject> orientationLocker)
    {
        auto jsCallInvoker = jsCallInvokerHolder->cthis()->getCallInvoker();
        auto jsiRuntime = reinterpret_cast<jsi::Runtime *>(jsiRuntimePtr);

        // auto boxedCallInvokerRef = jni::make_local(boxedCallInvokerHolder);
        // auto callInvokerHolder = jni::dynamic_ref_cast<react::CallInvokerHolder::javaobject>(boxedCallInvokerRef);
        // auto jsCallInvoker = callInvokerHolder->cthis()->getCallInvoker();

        auto boxedOrientationLockerRef = jni::make_global(orientationLocker);
        // auto orientationLocker = jni::dynamic_ref_cast<JOrientationLocker::javaobject>(boxedOrientationLockerRef);

        install(*jsiRuntime, jsCallInvoker, boxedOrientationLockerRef);
    }

    static void install(jsi::Runtime &jsiRuntime, std::shared_ptr<react::CallInvoker> jsCallInvoker, jni::global_ref<JOrientationLocker::javaobject> orientationLocker)
    {

        auto getCurrentOrientation = Function::createFromHostFunction(jsiRuntime,
                                                                      PropNameID::forAscii(jsiRuntime,
                                                                                           "getCurrentOrientation"),
                                                                      0,
                                                                      [orientationLocker](Runtime &runtime,
                                                                                          const Value &thisValue,
                                                                                          const Value *arguments,
                                                                                          size_t count) -> Value
                                                                      {
                                                                          auto orientation = orientationLocker->getCurrentOrientation();

                                                                          return Value(runtime,
                                                                                       String::createFromUtf8(
                                                                                           runtime, orientation->toString()));
                                                                      });

        jsiRuntime.global().setProperty(jsiRuntime, "getCurrentOrientation", move(getCurrentOrientation));

        auto listenToOrientationChanges = Function::createFromHostFunction(jsiRuntime,
                                                                           PropNameID::forAscii(jsiRuntime,
                                                                                                "listenToOrientationChanges"),
                                                                           0,
                                                                           [orientationLocker](Runtime &runtime,
                                                                                               const Value &thisValue,
                                                                                               const Value *arguments,
                                                                                               size_t count) -> Value
                                                                           {
                                                                               __android_log_write(ANDROID_LOG_INFO, "COCO TAG", "IN listenToOrientationChanges from C++");

                                                                               orientationLocker->listenToOrientationChanges();

                                                                               return jsi::Value::undefined();
                                                                           });

        jsiRuntime.global().setProperty(jsiRuntime, "listenToOrientationChanges", move(listenToOrientationChanges));
    }
};

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *)
{
    return jni::initialize(vm, []
                           { JsiBridge::registerNatives(); });
}

