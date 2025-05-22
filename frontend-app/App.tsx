import { NavigationContainer } from '@react-navigation/native';
import { createNativeStackNavigator } from '@react-navigation/native-stack';
import MainPage from './pages/MainPage';
import IntroPage from './pages/IntroPage';
import { useUserStore } from './hooks/userStore';
import { useEffect } from 'react';
import { Alert } from 'react-native';
import { SafeAreaProvider } from 'react-native-safe-area-context';

const Stack = createNativeStackNavigator();

export default function App() {
  const { error } = useUserStore();
  useEffect(() => {
    if (error) {
      Alert.alert(error);
    }
  }, [error]);

  return (
    <SafeAreaProvider>
    <NavigationContainer>
      <Stack.Navigator>
        <Stack.Screen name="IntroPage" component={IntroPage} />
        <Stack.Screen name="MainPage" component={MainPage} />
      </Stack.Navigator>
    </NavigationContainer>
    </SafeAreaProvider>
  );
}
