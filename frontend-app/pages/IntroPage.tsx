import { useEffect, useState } from 'react';
import { useUserStore } from '../hooks/userStore';
import { useNavigation } from '@react-navigation/native';
import {Image, StyleSheet, Text, View, TextInput, TouchableOpacity, SafeAreaView, Alert} from "react-native";
import logo from "../assets/Logo.png";


export default function IntroPage() {
  const { user, startUser } = useUserStore();
  const navigate = useNavigation();

  const [name, setName] = useState('');

  useEffect(() => {
    if (user) {
      navigate.navigate('MainPage' as never);
    }
  }, [user]);

  const onChange = (e: string) => {
    setName(e);
  };

  const onClick = async () => {
    if (!name) {
      Alert.alert('이름을 입력해주세요.');
      return;
    }
    await startUser(name);
  };

  return (
    <SafeAreaView style={[styles.safeareaview, styles.safeareaviewFlexBox]}>
      <View style={[styles.image1Parent, styles.safeareaviewFlexBox]}>
        <Image style={styles.image1Icon} resizeMode="cover" source={logo} />
        <Text style={styles.text}>팜덥니?</Text>
      </View>
      <View style={styles.frameParent}>
        <View style={[styles.wrapper, styles.wrapperFlexBox]}>
          <TextInput style={[styles.text1, styles.textTypo]} placeholder="사용자 아이디(이름)" value={name} onChangeText={onChange} />
        </View>
        <TouchableOpacity style={[styles.container, styles.wrapperFlexBox]} onPress={onClick}>
          <Text style={[styles.text2, styles.textTypo]}>회원가입 혹은 로그인</Text>
        </TouchableOpacity>
      </View>
    </SafeAreaView>);
};

const styles = StyleSheet.create({
  safeareaviewFlexBox: {
    alignItems: "center",
    overflow: "hidden",
    justifyContent: "center"
  },
  wrapperFlexBox: {
    borderRadius: 7,
    alignSelf: "stretch",
    alignItems: "center",
    overflow: "hidden"
  },
  textTypo: {
    fontSize: 14,
    textAlign: "left"
  },
  image1Icon: {
    width: 110,
    height: 111
  },
  text: {
    fontSize: 35,
    color: "#46a653",
    textAlign: "left",
    fontFamily: "Inter-ExtraBold",
    fontWeight: "800"
  },
  image1Parent: {
    paddingHorizontal: 30,
    paddingVertical: 0,
    flexDirection: "row",
    justifyContent: "center"
  },
  text1: {
    fontFamily: "Inter-Regular",
    color: "#999797"
  },
  wrapper: {
    backgroundColor: "#fefefe",
    height: 42,
    paddingHorizontal: 7,
    paddingVertical: 5,
    flexDirection: "row"
  },
  text2: {
    color: "#396518",
    fontFamily: "Inter-ExtraBold",
    fontWeight: "800",
    fontSize: 14
  },
  container: {
    backgroundColor: "#8af13b",
    paddingHorizontal: 115,
    paddingVertical: 9,
    justifyContent: "center"
  },
  frameParent: {
    borderRadius: 10,
    backgroundColor: "#efefef",
    paddingHorizontal: 15,
    paddingVertical: 20,
    gap: 20,
    alignSelf: "stretch",
    alignItems: "center",
    overflow: "hidden"
  },
  safeareaview: {
    backgroundColor: "#fff",
    flex: 1,
    width: "100%",
    height: "100%",
    paddingHorizontal: 10,
    paddingVertical: 10,
    gap: 10,
    justifyContent: "center"
  }
});