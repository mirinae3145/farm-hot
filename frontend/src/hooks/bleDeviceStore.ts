import { create } from "zustand";

interface BleDeviceState {
  device: BluetoothDevice | null
  setDevice: (device: BluetoothDevice | null) => void
}

export const useBleDeviceStore = create<BleDeviceState>((set) => ({
  device: null,
  setDevice: (device: BluetoothDevice | null) => set({ device }),
}))