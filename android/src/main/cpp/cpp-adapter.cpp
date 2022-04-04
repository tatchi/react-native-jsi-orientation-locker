#include <jni.h>
#include <sys/types.h>
#include "pthread.h"
#include <jsi/jsi.h>
#include <android/log.h>

#include <fbjni/fbjni.h>
#include <ReactCommon/CallInvokerHolder.h>
#include <ReactCommon/CallInvoker.h>

#include <jsi/jsilib.h>

using namespace std;
using namespace facebook;
using namespace facebook::jsi;

JavaVM *java_vm;
jclass java_class;
jobject java_object;

/**
 * A simple callback function that allows us to detach current JNI Environment
 * when the thread
 * See https://stackoverflow.com/a/30026231 for detailed explanation
 */

void DeferThreadDetach(JNIEnv *env)
{
    static pthread_key_t thread_key;

    // Set up a Thread Specific Data key, and a callback that
    // will be executed when a thread is destroyed.
    // This is only done once, across all threads, and the value
    // associated with the key for any given thread will initially
    // be NULL.
    static auto run_once = []
    {
        const auto err = pthread_key_create(&thread_key, [](void *ts_env)
                                            {
            if (ts_env) {
                java_vm->DetachCurrentThread();
            } });
        if (err)
        {
            // Failed to create TSD key. Throw an exception if you want to.
        }
        return 0;
    }();

    // For the callback to actually be executed when a thread exits
    // we need to associate a non-NULL value with the key on that thread.
    // We can use the JNIEnv* as that value.
    const auto ts_env = pthread_getspecific(thread_key);
    if (!ts_env)
    {
        if (pthread_setspecific(thread_key, env))
        {
            // Failed to set thread-specific value for key. Throw an exception if you want to.
        }
    }
}

/**
 * Get a JNIEnv* valid for this thread, regardless of whether
 * we're on a native thread or a Java thread.
 * If the calling thread is not currently attached to the JVM
 * it will be attached, and then automatically detached when the
 * thread is destroyed.
 *
 * See https://stackoverflow.com/a/30026231 for detailed explanation
 */
JNIEnv *GetJniEnv()
{
    JNIEnv *env = nullptr;
    // We still call GetEnv first to detect if the thread already
    // is attached. This is done to avoid setting up a DetachCurrentThread
    // call on a Java thread.

    // g_vm is a global.
    auto get_env_result = java_vm->GetEnv((void **)&env, JNI_VERSION_1_6);
    if (get_env_result == JNI_EDETACHED)
    {
        if (java_vm->AttachCurrentThread(&env, NULL) == JNI_OK)
        {
            DeferThreadDetach(env);
        }
        else
        {
            // Failed to attach thread. Throw an exception if you want to.
        }
    }
    else if (get_env_result == JNI_EVERSION)
    {
        // Unsupported JNI version. Throw an exception if you want to.
    }
    return env;
}

static jstring string2jstring(JNIEnv *env, const string &str)
{
    return (*env).NewStringUTF(str.c_str());
}

std::vector<std::shared_ptr<facebook::jsi::Function> > jsCallbacks;
std::shared_ptr<react::CallInvoker> invoker;

jsi::Runtime *rt;

extern "C" JNIEXPORT int JNICALL
Java_com_tatchi_jsi_orientationlocker_JsiOrientationLockerModule_callValue(JNIEnv *env, jclass clazz, jint param)
{

    __android_log_write(ANDROID_LOG_INFO, "COCO TAG", "callValue got called in C++");

    invoker->invokeAsync([=]()
                         {
            __android_log_print(ANDROID_LOG_INFO, "COCO TAG", "in invoke async, nb: %d", jsCallbacks.size());
            for(auto & elem : jsCallbacks)
            {
                elem->call(*rt, jsi::Value(param));
            } });

    int result = param + 10;
    return result;
}

void install(facebook::jsi::Runtime &jsiRuntime, std::shared_ptr<react::CallInvoker> jsCallInvoker)
{
    //  docPathStr = std::string(docPath);
    //   auto pool = std::make_shared<ThreadPool>();
    invoker = jsCallInvoker;
    rt = &jsiRuntime;

    auto getDeviceName = Function::createFromHostFunction(jsiRuntime,
                                                          PropNameID::forAscii(jsiRuntime,
                                                                               "getDeviceName"),
                                                          0,
                                                          [](Runtime &runtime,
                                                             const Value &thisValue,
                                                             const Value *arguments,
                                                             size_t count) -> Value
                                                          {
                                                              JNIEnv *jniEnv = GetJniEnv();

                                                              java_class = jniEnv->GetObjectClass(
                                                                  java_object);
                                                              jmethodID getModel = jniEnv->GetMethodID(
                                                                  java_class, "getModel",
                                                                  "()Ljava/lang/String;");
                                                              jobject result = jniEnv->CallObjectMethod(
                                                                  java_object, getModel);
                                                              const char *str = jniEnv->GetStringUTFChars(
                                                                  (jstring)result, NULL);

                                                              return Value(runtime,
                                                                           String::createFromUtf8(
                                                                               runtime, str));
                                                          });

    jsiRuntime.global().setProperty(jsiRuntime, "getDeviceName", move(getDeviceName));

    auto getCurrentOrientation = Function::createFromHostFunction(jsiRuntime,
                                                                  PropNameID::forAscii(jsiRuntime,
                                                                                       "getCurrentOrientation"),
                                                                  0,
                                                                  [](Runtime &runtime,
                                                                     const Value &thisValue,
                                                                     const Value *arguments,
                                                                     size_t count) -> Value
                                                                  {
                                                                      JNIEnv *jniEnv = GetJniEnv();

                                                                      java_class = jniEnv->GetObjectClass(
                                                                          java_object);
                                                                      jmethodID getCurrentOrientation = jniEnv->GetMethodID(
                                                                          java_class, "getCurrentOrientation",
                                                                          "()Ljava/lang/String;");
                                                                      jobject result = jniEnv->CallObjectMethod(
                                                                          java_object, getCurrentOrientation);
                                                                      const char *str = jniEnv->GetStringUTFChars(
                                                                          (jstring)result, NULL);

                                                                      return Value(runtime,
                                                                                   String::createFromUtf8(
                                                                                       runtime, str));
                                                                  });

    jsiRuntime.global().setProperty(jsiRuntime, "getCurrentOrientation", move(getCurrentOrientation));

    auto lockToLandscape = Function::createFromHostFunction(jsiRuntime,
                                                            PropNameID::forAscii(jsiRuntime,
                                                                                 "lockToLandscape"),
                                                            0,
                                                            [](Runtime &runtime,
                                                               const Value &thisValue,
                                                               const Value *arguments,
                                                               size_t count) -> Value
                                                            {
                                                                JNIEnv *jniEnv = GetJniEnv();

                                                                java_class = jniEnv->GetObjectClass(
                                                                    java_object);
                                                                jmethodID lockToLandscape = jniEnv->GetMethodID(
                                                                    java_class, "lockToLandscape",
                                                                    "()V");
                                                                jniEnv->CallVoidMethod(
                                                                    java_object, lockToLandscape);
                                                                return Value::undefined();
                                                            });

    jsiRuntime.global().setProperty(jsiRuntime, "lockToLandscape", move(lockToLandscape));

    auto lockToLandscapeLeft = Function::createFromHostFunction(jsiRuntime,
                                                                PropNameID::forAscii(jsiRuntime,
                                                                                     "lockToLandscapeLeft"),
                                                                0,
                                                                [](Runtime &runtime,
                                                                   const Value &thisValue,
                                                                   const Value *arguments,
                                                                   size_t count) -> Value
                                                                {
                                                                    JNIEnv *jniEnv = GetJniEnv();

                                                                    java_class = jniEnv->GetObjectClass(
                                                                        java_object);
                                                                    jmethodID lockToLandscapeLeft = jniEnv->GetMethodID(
                                                                        java_class, "lockToLandscapeLeft",
                                                                        "()V");
                                                                    jniEnv->CallVoidMethod(
                                                                        java_object, lockToLandscapeLeft);
                                                                    return Value::undefined();
                                                                });

    jsiRuntime.global().setProperty(jsiRuntime, "lockToLandscapeLeft", move(lockToLandscapeLeft));

    auto lockToLandscapeRight = Function::createFromHostFunction(jsiRuntime,
                                                                 PropNameID::forAscii(jsiRuntime,
                                                                                      "lockToLandscapeRight"),
                                                                 0,
                                                                 [](Runtime &runtime,
                                                                    const Value &thisValue,
                                                                    const Value *arguments,
                                                                    size_t count) -> Value
                                                                 {
                                                                     JNIEnv *jniEnv = GetJniEnv();

                                                                     java_class = jniEnv->GetObjectClass(
                                                                         java_object);
                                                                     jmethodID lockToLandscapeRight = jniEnv->GetMethodID(
                                                                         java_class, "lockToLandscapeRight",
                                                                         "()V");
                                                                     jniEnv->CallVoidMethod(
                                                                         java_object, lockToLandscapeRight);
                                                                     return Value::undefined();
                                                                 });

    jsiRuntime.global().setProperty(jsiRuntime, "lockToLandscapeRight", move(lockToLandscapeRight));

    auto lockToPortrait = Function::createFromHostFunction(jsiRuntime,
                                                           PropNameID::forAscii(jsiRuntime,
                                                                                "lockToPortrait"),
                                                           0,
                                                           [](Runtime &runtime,
                                                              const Value &thisValue,
                                                              const Value *arguments,
                                                              size_t count) -> Value
                                                           {
                                                               JNIEnv *jniEnv = GetJniEnv();

                                                               java_class = jniEnv->GetObjectClass(
                                                                   java_object);
                                                               jmethodID lockToPortrait = jniEnv->GetMethodID(
                                                                   java_class, "lockToPortrait",
                                                                   "()V");
                                                               jniEnv->CallVoidMethod(
                                                                   java_object, lockToPortrait);
                                                               return Value();
                                                           });

    jsiRuntime.global().setProperty(jsiRuntime, "lockToPortrait", move(lockToPortrait));

    auto lockToPortraitUpsideDown = Function::createFromHostFunction(jsiRuntime,
                                                                     PropNameID::forAscii(jsiRuntime,
                                                                                          "lockToPortraitUpsideDown"),
                                                                     0,
                                                                     [](Runtime &runtime,
                                                                        const Value &thisValue,
                                                                        const Value *arguments,
                                                                        size_t count) -> Value
                                                                     {
                                                                         JNIEnv *jniEnv = GetJniEnv();

                                                                         java_class = jniEnv->GetObjectClass(
                                                                             java_object);
                                                                         jmethodID lockToPortraitUpsideDown = jniEnv->GetMethodID(
                                                                             java_class, "lockToPortraitUpsideDown",
                                                                             "()V");
                                                                         jniEnv->CallVoidMethod(
                                                                             java_object, lockToPortraitUpsideDown);
                                                                         return Value();
                                                                     });

    jsiRuntime.global().setProperty(jsiRuntime, "lockToPortraitUpsideDown", move(lockToPortraitUpsideDown));

    auto listenToOrientationChanges = Function::createFromHostFunction(jsiRuntime,
                                                                       PropNameID::forAscii(jsiRuntime,
                                                                                            "listenToOrientationChanges"),
                                                                       1,
                                                                       [](Runtime &runtime,
                                                                          const Value &thisValue,
                                                                          const Value *arguments,
                                                                          size_t count) -> Value
                                                                       {
                                                                           __android_log_write(ANDROID_LOG_INFO, "COCO TAG", "IN listenToOrientationChanges from C++");

                                                                           auto callback = arguments[0].asObject(runtime).asFunction(runtime);
                                                                           auto jsCallback = std::make_shared<jsi::Function>(std::move(callback));
                                                                           jsCallbacks.push_back(jsCallback);

                                                                           JNIEnv *jniEnv = GetJniEnv();

                                                                           java_class = jniEnv->GetObjectClass(
                                                                               java_object);
                                                                           jmethodID listenToOrientationChanges = jniEnv->GetMethodID(
                                                                               java_class, "listenToOrientationChanges",
                                                                               "()V");
                                                                           jniEnv->CallVoidMethod(
                                                                               java_object, listenToOrientationChanges);

                                                                           auto close = [jsCallback](jsi::Runtime &runtime, const jsi::Value &, const jsi::Value *, size_t) -> jsi::Value
                                                                           {
                                                                               auto it = std::find(jsCallbacks.begin(), jsCallbacks.end(), jsCallback);

                                                                               // If element was found
                                                                               if (it != jsCallbacks.end())
                                                                               {
                                                                                   __android_log_write(ANDROID_LOG_INFO, "COCO TAG", "FOUND CALLBACK %i");
                                                                                   auto index = it - jsCallbacks.begin();
                                                                                   jsCallbacks.erase(jsCallbacks.begin() + index);
                                                                               }
                                                                               else
                                                                               {
                                                                                   __android_log_write(ANDROID_LOG_INFO, "COCO TAG", "CALLBACK NOT FOUND");
                                                                               }

                                                                               return jsi::Value::undefined();
                                                                           };
                                                                           return jsi::Function::createFromHostFunction(runtime, jsi::PropNameID::forUtf8(runtime, "close"), 0, close);
                                                                       });

    jsiRuntime.global().setProperty(jsiRuntime, "listenToOrientationChanges", move(listenToOrientationChanges));

    auto setItem = Function::createFromHostFunction(jsiRuntime,
                                                    PropNameID::forAscii(jsiRuntime,
                                                                         "setItem"),
                                                    2,
                                                    [](Runtime &runtime,
                                                       const Value &thisValue,
                                                       const Value *arguments,
                                                       size_t count) -> Value
                                                    {
                                                        string key = arguments[0].getString(
                                                                                     runtime)
                                                                         .utf8(runtime);
                                                        string value = arguments[1].getString(
                                                                                       runtime)
                                                                           .utf8(runtime);

                                                        JNIEnv *jniEnv = GetJniEnv();

                                                        java_class = jniEnv->GetObjectClass(
                                                            java_object);

                                                        jmethodID set = jniEnv->GetMethodID(
                                                            java_class, "setItem",
                                                            "(Ljava/lang/String;Ljava/lang/String;)V");

                                                        jstring jstr1 = string2jstring(jniEnv,
                                                                                       key);
                                                        jstring jstr2 = string2jstring(jniEnv,
                                                                                       value);
                                                        jvalue params[2];
                                                        params[0].l = jstr1;
                                                        params[1].l = jstr2;

                                                        jniEnv->CallVoidMethodA(
                                                            java_object, set, params);

                                                        return Value(true);
                                                    });

    jsiRuntime.global().setProperty(jsiRuntime, "setItem", move(setItem));

    auto getItem = Function::createFromHostFunction(jsiRuntime,
                                                    PropNameID::forAscii(jsiRuntime,
                                                                         "getItem"),
                                                    1,
                                                    [](Runtime &runtime,
                                                       const Value &thisValue,
                                                       const Value *arguments,
                                                       size_t count) -> Value
                                                    {
                                                        string key = arguments[0].getString(
                                                                                     runtime)
                                                                         .utf8(
                                                                             runtime);

                                                        JNIEnv *jniEnv = GetJniEnv();

                                                        java_class = jniEnv->GetObjectClass(
                                                            java_object);
                                                        jmethodID get = jniEnv->GetMethodID(
                                                            java_class, "getItem",
                                                            "(Ljava/lang/String;)Ljava/lang/String;");

                                                        jstring jstr1 = string2jstring(jniEnv,
                                                                                       key);
                                                        jvalue params[2];
                                                        params[0].l = jstr1;

                                                        jobject result = jniEnv->CallObjectMethodA(
                                                            java_object, get, params);
                                                        const char *str = jniEnv->GetStringUTFChars(
                                                            (jstring)result, NULL);

                                                        return Value(runtime,
                                                                     String::createFromUtf8(
                                                                         runtime, str));
                                                    });

    jsiRuntime.global().setProperty(jsiRuntime, "getItem", move(getItem));
}

extern "C" JNIEXPORT void JNICALL
Java_com_tatchi_jsi_orientationlocker_JsiOrientationLockerModule_nativeInstall(JNIEnv *env, jobject thiz,
                                                                               jlong jsiRuntimePtr,
                                                                               jobject boxedCallInvokerHolder)
{

    auto jsiRuntime = reinterpret_cast<jsi::Runtime *>(jsiRuntimePtr);

    auto boxedCallInvokerRef = jni::make_local(boxedCallInvokerHolder);
    auto callInvokerHolder = jni::dynamic_ref_cast<react::CallInvokerHolder::javaobject>(boxedCallInvokerRef);
    auto jsCallInvoker = callInvokerHolder->cthis()->getCallInvoker();
    if (jsiRuntime)
    {
        install(*jsiRuntime, jsCallInvoker);
    }
    // TODO: needed?
    env->GetJavaVM(&java_vm);
    java_object = env->NewGlobalRef(thiz);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *)
{
    return facebook::jni::initialize(vm, [] {});
}
