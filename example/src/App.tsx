import * as React from 'react';
import {StyleSheet, View, Text, TouchableOpacity} from 'react-native';
import Orientation from 'react-native-jsi-orientation-locker';

export default function App() {
  const [orientation, setOrientation] = React.useState<string>('');
  const [currentOrientation, setCurrentOrientation] = React.useState(() =>
    Orientation.getCurrentOrientation(),
  );

  const unregister1 = React.useRef();
  const unregister2 = React.useRef();

  React.useEffect(() => {
    unregister1.current = Orientation.listenToOrientationChanges(
      orientation => {
        console.log({first: orientation});
        setCurrentOrientation(orientation);
      },
    );
    unregister2.current = Orientation.listenToOrientationChanges(res => {
      console.log({second: res});
    });
  }, []);

  return (
    <View style={styles.container}>
      <Text>Current Orientation: {currentOrientation}</Text>
      <TouchableOpacity
        onPress={() => {
          let value = Orientation.getCurrentOrientation();
          setOrientation(value);
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>Orientation: {orientation}</Text>
      </TouchableOpacity>

      <TouchableOpacity
        onPress={() => {
          Orientation.lockToPortrait();
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>Lock Portrait</Text>
      </TouchableOpacity>

      <TouchableOpacity
        onPress={() => {
          Orientation.lockToPortraitUpsideDown();
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>Lock PortraitUpsideDown</Text>
      </TouchableOpacity>

      <TouchableOpacity
        onPress={() => {
          Orientation.lockToLandscape();
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>Lock Landscape</Text>
      </TouchableOpacity>

      <TouchableOpacity
        onPress={() => {
          Orientation.lockToLandscapeLeft();
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>Lock Landscape Left</Text>
      </TouchableOpacity>

      <TouchableOpacity
        onPress={() => {
          Orientation.lockToLandscapeRight();
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>Lock Landscape Right</Text>
      </TouchableOpacity>

      <TouchableOpacity
        onPress={() => {
          unregister1.current();
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>Unregister Listener 1</Text>
      </TouchableOpacity>
      <TouchableOpacity
        onPress={() => {
          unregister2.current();
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>Unregister Listener 2.</Text>
      </TouchableOpacity>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  box: {
    width: 60,
    height: 60,
    marginVertical: 20,
  },
  button: {
    width: '95%',
    alignSelf: 'center',
    height: 40,
    backgroundColor: 'green',
    alignItems: 'center',
    justifyContent: 'center',
    borderRadius: 5,
    marginVertical: 10,
  },
  buttonTxt: {
    color: 'white',
  },
});
