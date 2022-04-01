import { NativeModules } from 'react-native';

// interface OrientationModuleInterface {
//   getCurrentOrientation(): string;
//   lockToLandscape(): void;
//   lockToPortrait(): void;
//   activateListener(cb: (orientation: number) => void): void;
//   install(): void;
// }

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
  console.log('LISTEN!');
  return global.activateListener(cb);
};

const orientationModule = {
  getCurrentOrientation,
  lockToLandscape,
  lockToPortrait,
  listenToOrientationChanges,
};

export default orientationModule;
