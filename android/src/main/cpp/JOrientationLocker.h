//
// Created by Marc Rousavy on 30.09.21.
//

#pragma once

#include <jni.h>
#include <fbjni/fbjni.h>
#include <string>

using namespace facebook;
using namespace jni;

struct JOrientationLocker : public JavaClass<JOrientationLocker>
{
  static constexpr auto kJavaDescriptor = "Lcom/tatchi/jsi/orientationlocker/OrientationLocker;";

public:
    void lockToLandscape() const;

  local_ref<jstring> getCurrentOrientation() const;
};
