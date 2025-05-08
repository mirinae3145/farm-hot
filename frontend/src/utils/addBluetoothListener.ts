

const addBluetoothListener = async (device: BluetoothDevice, onValueGet: (value: number) => void) => {
    // GATT 서버 연결
    const server = await device.gatt!.connect();
    const service = await server.getPrimaryService(process.env.SERVICE_UUID ?? 'battery_service');
    const char = await service.getCharacteristic(process.env.CHAR_UUID ?? 'battery_level');

    // 알림 수신 시작
    await char.startNotifications();
    char.addEventListener('characteristicvaluechanged', (e: Event) => {
        const target = e.target as BluetoothRemoteGATTCharacteristic;
        const dv = target.value!;
        onValueGet(dv.getUint8(0));
    });
}

export default addBluetoothListener;
