import { useEffect, useState } from "react";
import { useBleDeviceStore } from "../hooks/bleDeviceStore";
import styles from "./settingPage.module.css";
import { useNavigate } from "react-router-dom";

export default function SettingPage() {
  const { device, setDevice } = useBleDeviceStore();
  const [gpsStatus, setGpsStatus] = useState<boolean>(false);

  const navigate = useNavigate();

  useEffect(() => {
    if (!device || ! gpsStatus) return;
    navigate('/main');
  }, [device, gpsStatus]);

  const requestBluetoothConnect = async () => {
    try {
      if (!navigator.bluetooth) {
        alert('Web Bluetooth 지원 안 되는 브라우저입니다.');
        return;
      }

      // 디바이스 선택 다이얼로그 (여기서만 호출 가능)
      const selected = await navigator.bluetooth.requestDevice({
        filters: [{ services: [process.env.SERVICE_UUID ?? 'battery_service'] }],
        //acceptAllDevices: true,
      });
      setDevice(selected);
    } catch (err: any) {
      alert('블루투스 권한이 필요합니다.');
      setDevice(null);
    }
  };

  const requestGpsConnect = () => {
    if (!navigator.geolocation) {
      alert('Geolocation 지원 안 되는 브라우저입니다.');
      return;
    }

    navigator.geolocation.getCurrentPosition(
      _ => {
        setGpsStatus(true);
      },
      _ => {
        alert('GPS 권한이 필요합니다.');
        setGpsStatus(false);
      },
      {
        enableHighAccuracy: true,
        timeout: 5000,
        maximumAge: 60_000,
      }
    );
  };

  const onNextClick = () => {
    navigate('/main');
  };

  return (
    <div className={styles.div}>
      <div className={styles.image1Parent}>
        <img className={styles.image1Icon} alt="" src="Logo.png" />
        <div className={styles.gps}>팜덥니?</div>
      </div>
      <div className={styles.frameParent}>
        <div className={styles.image2Parent}>
          <img className={styles.image2Icon} alt="" src="Bluetooth.png" />
          <div className={styles.gps}>블루투스로 아두이노와 연결해주세요</div>
        </div>
        <div className={styles.image2Parent}>
          <img className={styles.image2Icon} alt="" src="GPS.png" />
          <div className={styles.gps}>GPS를 허용해주세요</div>
        </div>
      </div>
      {(device && gpsStatus) && <NextButton onClick={onNextClick} />}
      {!device && <ConnectBleButton onClick={requestBluetoothConnect} />}
      {!gpsStatus && <ConnectGPSButton onClick={requestGpsConnect} />}
    </div>
  );
}

const NextButton = ({onClick}: {onClick: () => void}) => {
  return (
    <button className={styles.wrapper} onClick={onClick}>
      <div className={styles.gps}>확인</div>
    </button>
  );
}

const ConnectBleButton = ({onClick}: {onClick: () => Promise<void>}) => {
  return (
    <button className={styles.connectBleWrapper} onClick={onClick}>
      <div className={styles.connectDiv}>블루투스 연결</div>
    </button>
  );
}

const ConnectGPSButton = ({onClick}: {onClick: () => void}) => {
  return (
    <button className={styles.connectGpsWrapper} onClick={onClick}>
      <div className={styles.connectDiv}>GPS 연결</div>
    </button>
  );
}