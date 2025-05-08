import { useState } from 'react';
import styles from './introPage.module.css';
import { useNavigate } from 'react-router-dom';
import { useUserStore } from '../hooks/userStore';

export default function IntroPage() {
  const { startUser } = useUserStore();
  const navigate = useNavigate();

  const [name, setName] = useState('');

  const onChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    setName(e.target.value);
  };

  const onClick = async () => {
    if (!name) {
      alert('이름을 입력해주세요.');
      return;
    }
    await startUser(name);
    navigate('/setting');
  };

  return (
    <div className={styles.div}>
      <div className={styles.image1Parent}>
        <img className={styles.image1Icon} alt="" src="Logo.png" />
        <div className={styles.div1}>팜덥니?</div>
      </div>
      <div className={styles.frameParent}>
        <div className={styles.wrapper}>
          <input className={styles.div2} type="text" placeholder="사용자 아이디(이름)" onChange={onChange} />
        </div>
        <div className={styles.container}>
          <div className={styles.div1} onClick={onClick}>회원가입 혹은 로그인</div>
        </div>
      </div>
    </div>
  );
}