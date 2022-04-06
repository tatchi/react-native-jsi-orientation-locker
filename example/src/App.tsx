import * as React from 'react';
import {StyleSheet, View, Text, TouchableOpacity} from 'react-native';
import Orientation from 'react-native-jsi-orientation-locker';

export default function App() {
  const [orientation, setOrientation] = React.useState(() =>
    Orientation.getCurrentOrientation(),
  );

  return (
    <View style={styles.container}>
      <TouchableOpacity
        onPress={() => {
          Orientation.listenToOrientationChanges(orientation => {
            console.log(`got orientation: ${orientation}`);
          })
          let value = Orientation.getCurrentOrientation();
          setOrientation(value);
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>Orientation: {orientation}</Text>
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
