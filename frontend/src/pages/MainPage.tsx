import { useEffect } from 'react';
import styles from './mainPage.module.css';
import { useBleDeviceStore } from '../hooks/bleDeviceStore';
import { useNavigate } from 'react-router-dom';
import addBluetoothListener from '../utils/addBluetoothListener';
import { useUserStore } from '../hooks/userStore';
import { useState } from 'react';
import { UserStatus } from '../types/userStatus';

export default function MainPage() {
  const { device } = useBleDeviceStore();
  const { user } = useUserStore();
  const [ isSafe, setIsSafe ] = useState<UserStatus>(UserStatus.NORMAL);
  const navigate = useNavigate();

  useEffect(() => {
    if (!user) {
      navigate('/');
      return;
    }
  }, [user]);

  useEffect(() => {
    if (!device) {
      alert('블루투스 연결이 필요합니다.');
      navigate('/setting');
      return;
    }

    try {
      addBluetoothListener(device, (data: UserStatus) => {
        setIsSafe(data);
      });
    } catch (error) {
      console.error(error);
      alert('블루투스 연결이 필요합니다.');
      navigate('/setting');
    }
    
  }, [device]);

  return (
    <div className={styles.div}>
      <div className={styles.image1Parent}>
        <img className={styles.image1Icon} alt="" src="Logo.png" />
        <div className={styles.div1}>팜덥니?</div>
      </div>
      {isSafe === UserStatus.NORMAL && <SafeLayout />}
    </div>
  );
}

const SafeLayout = () => {
  return (
    <>
      <div className={styles.parent}>
        <div className={styles.div2}>안전</div>
        <div className={styles.div3}>
          <p className={styles.p}>현재는 안전한 상태입니다</p>
          <p className={styles.p}>안전하게 농업 생활하시길 바랍니다</p>
        </div>
      </div>
      <div className={styles.wrapper}>
        <div className={styles.div1}>현재 괜찮습니다.</div>
      </div>
    </>
  )
}