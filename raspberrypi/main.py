
# Supabase 기본 설정
import time
import threading
import asyncio
from supabase import AsyncClient, create_client, Client, create_async_client
import RPi.GPIO as GPIO
import os

# Supabase 설정
SUPABASE_URL = os.getenv("SUPABASE_URL")
SUPABASE_KEY = os.getenv("SUPABASE_KEY")

# GPIO 핀 설정 (밸브 제어용) TODO: 실제 사용할 때는 조절
# 지역 ID에 따른 GPIO 핀 매핑
VALVE_PINS = {
    1: 17,
    2: 18,
    3: 27,
    4: 22
}

# GPIO 초기화
GPIO.setmode(GPIO.BCM)
for pin in VALVE_PINS.values():
    GPIO.setup(pin, GPIO.OUT)
    GPIO.output(pin, GPIO.LOW)  # 기본값: 밸브 닫힘


# 밸브 제어 함수
def control_valve(region_id, is_active):
    """특정 지역의 밸브를 제어합니다."""
    if region_id in VALVE_PINS:
        pin = VALVE_PINS[region_id]
        
        if is_active:
            print(f"지역 ID {region_id}의 밸브를 엽니다.")
            #GPIO.output(pin, GPIO.HIGH)  # 밸브 열기
        else:
            print(f"지역 ID {region_id}의 밸브를 닫습니다.")
            #GPIO.output(pin, GPIO.LOW)   # 밸브 닫기
    else:
        print(f"알 수 없는 지역 ID: {region_id}")


# Supabase 데이터 변경 이벤트 처리 함수
def handle_region_change(payload):
    print(f"새로운 변경사항 감지: {payload}")
    
    try:
        # 변경된 데이터 가져오기
        region_data = payload['data']['record']
        region_id = region_data.get('id')
        is_active = not region_data.get('is_safe', True)
        
        # 해당 지역의 밸브 제어
        control_valve(region_id, is_active)
    except Exception as e:
        print(f"오류 발생: {e}")
        print(e.__str__())


# 현재 지역 상태를 가져와서 밸브 설정하는 함수
def fetch_and_setup_regions(supabase):
    try:
        # 현재 지역 상태 가져오기
        response = supabase.table("region").select("*").execute()
        
        print(f"총 {len(response.data)}개의 지역 상태를 가져왔습니다.")
        
        # 초기 상태에 따라 밸브 설정
        for region in response.data:
            region_id = region.get('id')
            is_active = not region.get('is_safe', True)
            control_valve(region_id, is_active)
            
        return True
    except Exception as e:
        print(f"지역 상태 가져오기 오류: {e}")
        return False


# Supabase region 리스너 설정 (비동기 함수)
async def subscribe_to_region_changes(supabase: AsyncClient):
    try:
        print("리스너 연결 시작")
        # Realtime 클라이언트 설정
        channel = supabase.channel('public:region')
        
        # INSERT, UPDATE, DELETE 이벤트 리스닝
        channel = channel.on_postgres_changes(
            "UPDATE", schema="public", table="region", callback=handle_region_change
        )

        # 채널 구독
        await channel.subscribe()

        print("Supabase 실시간 리스너가 시작되었습니다.")
        
        # 연결 유지
        while True:
            await asyncio.sleep(1)
            
    except Exception as e:
        print(f"리스너 오류 발생: {e}")


# 비동기 함수를 스레드에서 실행하는 함수
def run_async_listener():
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)
    
    try:
        # 비동기 클라이언트 생성
        async_supabase = loop.run_until_complete(create_async_client(SUPABASE_URL, SUPABASE_KEY))
        
        # 리스너 시작
        print("스레드에서 리스너 시작 중...")
        loop.run_until_complete(subscribe_to_region_changes(async_supabase))
    except Exception as e:
        print(f"비동기 스레드 오류: {e}")
    finally:
        loop.close()


# 메인 함수
if __name__ == "__main__":
    try:
        # 동기 클라이언트 생성 및 초기값 설정
        supabase = create_client(SUPABASE_URL, SUPABASE_KEY)
        fetch_and_setup_regions(supabase)
        print("지역 초기 설정 완료")
        
        # 비동기 리스너를 위한 별도 스레드 시작
        listener_thread = threading.Thread(target=run_async_listener, daemon=True)
        listener_thread.start()
        print("리스너 스레드 시작됨")
        
        # 메인 스레드에서 프로그램 유지
        while True:
            time.sleep(1)
            
    except KeyboardInterrupt:
        print("프로그램을 종료합니다.")