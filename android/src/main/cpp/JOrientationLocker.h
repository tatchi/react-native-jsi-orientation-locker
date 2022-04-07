//
// Created by Marc Rousavy on 30.09.21.
//

#pragma once

#include <jni.h>
#include <ReactCommon/CallInvokerHolder.h>
#include <fbjni/fbjni.h>
#include <jsi/jsi.h>
#include <string>

using namespace facebook;
using namespace jni;

class JOrientationLocker : public jni::HybridClass<JOrientationLocker>
{
public:
  static constexpr auto kJavaDescriptor = "Lcom/tatchi/jsi/orientationlocker/OrientationLocker;";

  static jni::local_ref<jhybriddata> initHybrid(
      jni::alias_ref<jhybridobject> jThis,
      jlong jsContext,
      jni::alias_ref<react::CallInvokerHolder::javaobject> jsCallInvokerHolder);

  static void registerNatives();

  void lockToLandscape();
  void lockToLandscapeLeft();
  void lockToLandscapeRight();
  void lockToPortrait();
  void lockToPortraitUpsideDown();

  void listenToOrientationChanges(std::shared_ptr<jsi::Function> onChange);

  local_ref<jstring> getCurrentOrientation();

  void onOrientationChangedCallback(int value);

  void removeCallback(std::shared_ptr<facebook::jsi::Function> cb);

private:
  friend HybridBase;
  jni::global_ref<JOrientationLocker::javaobject> javaPart_;
  jsi::Runtime *runtime_;
  std::shared_ptr<react::CallInvoker> jsCallInvoker_;
  std::vector<std::shared_ptr<facebook::jsi::Function> > jsCallbacks_;

  explicit JOrientationLocker(
      jni::alias_ref<JOrientationLocker::jhybridobject> jThis,
      jsi::Runtime *rt,
      std::shared_ptr<react::CallInvoker> jsCallInvoker);
};
