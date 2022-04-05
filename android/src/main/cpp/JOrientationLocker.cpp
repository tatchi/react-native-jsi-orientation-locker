//
// Created by Marc Rousavy on 30.09.21.
//

#include "JOrientationLocker.h"

#include <jni.h>
#include <fbjni/fbjni.h>

using namespace facebook;
using namespace jni;

void JOrientationLocker::lockToLandscape() const
{
  auto lockToLandscapeMethod = getClass()->getMethod<void()>("lockToLandscape");

  lockToLandscapeMethod(self());
}

local_ref<jstring> JOrientationLocker::getCurrentOrientation() const
{
  auto getCurrentOrientationMethod = getClass()->getMethod<jstring()>("getCurrentOrientation");

  return getCurrentOrientationMethod(self());
}
