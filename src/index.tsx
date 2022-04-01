import { NativeModules, Platform } from 'react-native';

declare global {
  var getCurrentOrientation: () => string;
  var lockToLandscape: () => void;
  var lockToLandscapeLeft: () => void;
  var lockToLandscapeRight: () => void;
  var lockToPortrait: () => void;
  var lockToPortraitUpsideDown: () => void;
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

export const lockToLandscapeLeft = (): void => {
  return global.lockToLandscapeLeft();
};

export const lockToLandscapeRight = (): void => {
  return global.lockToLandscapeRight();
};
export const lockToPortrait = (): void => {
  return global.lockToPortrait();
};

export const lockToPortraitUpsideDown = (): void => {
  return global.lockToPortraitUpsideDown();
};
export const listenToOrientationChanges = (
  cb: (orientation: number) => void
): void => {
  return global.activateListener(cb);
};

const orientationModuleAndroid = {
  getCurrentOrientation,
  lockToLandscape,
  lockToLandscapeLeft,
  lockToLandscapeRight,
  lockToPortrait,
  lockToPortraitUpsideDown,
  listenToOrientationChanges,
};

const orientationModuleiOS = {
  getCurrentOrientation: noop as () => string,
  lockToLandscape: noop,
  lockToLandscapeLeft: noop,
  lockToLandscapeRight: noop,
  lockToPortrait: noop,
  lockToPortraitUpsideDown: noop,
  listenToOrientationChanges: noop,
};

export default Platform.OS === 'android'
  ? orientationModuleAndroid
  : orientationModuleiOS;
