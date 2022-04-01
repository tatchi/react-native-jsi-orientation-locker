import { NativeModules, Platform } from 'react-native';

declare global {
  var getCurrentOrientation: () => string;
  var lockToLandscape: () => void;
  var lockToPortrait: () => void;
  var activateListener: (cb: (orientation: number) => void) => void;
}

const OrientationNativeModule = NativeModules.JsiOrientationLocker;

if (OrientationNativeModule) {
  if (typeof OrientationNativeModule.install === 'function') {
    OrientationNativeModule.install();
  }
}

function noop() {
  if (__DEV__) {
    console.warn(
      'react-native-jsi-orientation-locker is not available on this platform'
    );
  }
}

export const getCurrentOrientation = (): string => {
  return global.getCurrentOrientation();
};

export const lockToLandscape = (): void => {
  return global.lockToLandscape();
};
export const lockToPortrait = (): void => {
  return global.lockToPortrait();
};
export const listenToOrientationChanges = (
  cb: (orientation: number) => void
): void => {
  return global.activateListener(cb);
};

const orientationModuleAndroid = {
  getCurrentOrientation,
  lockToLandscape,
  lockToPortrait,
  listenToOrientationChanges,
};

const orientationModuleiOS = {
  getCurrentOrientation: noop as () => string,
  lockToLandscape: noop,
  lockToPortrait: noop,
  listenToOrientationChanges: noop,
};

export default Platform.select({
  android: orientationModuleAndroid,
  ios: orientationModuleiOS,
});
