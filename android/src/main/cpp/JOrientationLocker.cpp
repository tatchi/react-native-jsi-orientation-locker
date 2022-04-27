#include "JOrientationLocker.h"

#include <jni.h>
#include <fbjni/fbjni.h>

using namespace facebook;
using namespace react;
using namespace jni;

using TSelf = local_ref<HybridClass<JOrientationLocker>::jhybriddata>;

JOrientationLocker::JOrientationLocker(
    jni::alias_ref<JOrientationLocker::javaobject> jThis,
    jsi::Runtime *rt,
    std::shared_ptr<react::CallInvoker> jsCallInvoker)
    : javaPart_(jni::make_global(jThis)),
      runtime_(rt),
      jsCallInvoker_(std::move(jsCallInvoker)) {}

// JNI init
TSelf JOrientationLocker::initHybrid(
    alias_ref<jhybridobject> jThis,
    jlong jsContext,
    jni::alias_ref<react::CallInvokerHolder::javaobject>
        jsCallInvokerHolder)
{
  __android_log_write(ANDROID_LOG_INFO, "🥲", "initHybrid...");
  auto jsCallInvoker = jsCallInvokerHolder->cthis()->getCallInvoker();
  return makeCxxInstance(jThis, (jsi::Runtime *)jsContext, jsCallInvoker);
}

// JNI binding
void JOrientationLocker::registerNatives()
{
  __android_log_print(ANDROID_LOG_VERBOSE, "😇", "JOrientationLocker - registerNatives");
  registerHybrid({
      makeNativeMethod("initHybrid",
                       JOrientationLocker::initHybrid),
      makeNativeMethod("onOrientationChangedCallback",
                       JOrientationLocker::onOrientationChangedCallback),
  });
}

local_ref<jstring> JOrientationLocker::getCurrentOrientation()
{
  auto getCurrentOrientationMethod = javaPart_->getClass()->getMethod<jstring()>("getCurrentOrientation");

  return getCurrentOrientationMethod(javaPart_.get());
}

void JOrientationLocker::onOrientationChangedCallback(int value)
{
  __android_log_print(ANDROID_LOG_INFO, "COCO TAG", "IN callValue %d", value);

  jsCallInvoker_->invokeAsync([=]()
                              {
            __android_log_print(ANDROID_LOG_INFO, "COCO TAG", "in invoke async, nb: %d", jsCallbacks_.size());
            for(auto & elem : jsCallbacks_)
            {
                elem->call(*runtime_, jsi::Value(value));
            } });
}

void JOrientationLocker::lockToLandscape()
{
  auto lockToLandscapeMethod = javaPart_->getClass()->getMethod<void()>("lockToLandscape");

  lockToLandscapeMethod(javaPart_.get());
}

void JOrientationLocker::lockToLandscapeLeft()
{
  auto lockToLandscapeMethod = javaPart_->getClass()->getMethod<void()>("lockToLandscapeLeft");

  lockToLandscapeMethod(javaPart_.get());
}

void JOrientationLocker::lockToLandscapeRight()
{
  auto lockToLandscapeMethod = javaPart_->getClass()->getMethod<void()>("lockToLandscapeRight");

  lockToLandscapeMethod(javaPart_.get());
}

void JOrientationLocker::lockToPortrait()
{
  auto lockToLandscapeMethod = javaPart_->getClass()->getMethod<void()>("lockToPortrait");

  lockToLandscapeMethod(javaPart_.get());
}


void JOrientationLocker::lockToPortraitUpsideDown()
{
  auto lockToLandscapeMethod = javaPart_->getClass()->getMethod<void()>("lockToPortraitUpsideDown");

  lockToLandscapeMethod(javaPart_.get());
}

void JOrientationLocker::removeCallback(std::shared_ptr<facebook::jsi::Function> cb)
{
    auto it = std::find(jsCallbacks_.begin(), jsCallbacks_.end(), cb);

    // If element was found
    if (it != jsCallbacks_.end())
    {
        __android_log_write(ANDROID_LOG_INFO, "COCO TAG", "FOUND CALLBACK %i");
        auto index = it - jsCallbacks_.begin();
        jsCallbacks_.erase(jsCallbacks_.begin() + index);
    }
    else
    {
        __android_log_write(ANDROID_LOG_INFO, "COCO TAG", "CALLBACK NOT FOUND");
    }
}


void JOrientationLocker::listenToOrientationChanges(std::shared_ptr<jsi::Function> onChange)
{

  jsCallbacks_.push_back(onChange);

  auto listenToOrientationChangesMethod = javaPart_->getClass()->getMethod<void()>("listenToOrientationChanges");

  listenToOrientationChangesMethod(javaPart_.get());



}
