import { create } from 'zustand'
import { getUser, postUser, updateUserGPS, updateUserStatus } from '../utils/supabase'
import { User } from '../types/user'

interface UserState {
  user: User | null
  loading: boolean
  error: string | null
  
  startUser: (name: string) => Promise<void>
  updateLocation: (latitude: number, longitude: number) => Promise<void>
  updateStatus: (isTempGood: boolean) => Promise<void>
}

export const useUserStore = create<UserState>((set) => ({
  user: null,
  loading: false,
  error: null,
  
  startUser: async (name: string) => {
    set({ loading: true, error: null })
    
    const userData = await getUser(name)
    
    if (!userData) {
      await postUser(name)
      const userData = await getUser(name)
      
      if (!userData) {
        set({ error: 'Failed to create user', loading: false })
        return
      }
    }
    
    set({ user: userData, loading: false })
  },
  
  updateLocation: async (latitude: number, longitude: number) => {
    set({ loading: true, error: null })
    
    const { user } = useUserStore.getState()
    
    if (!user) {
      set({ error: 'No user found', loading: false })
      return
    }
    
    await updateUserGPS(user.id, latitude, longitude)
    
    set(state => ({
      user: state.user ? { ...state.user, latitude, longitude } : null,
      loading: false
    }))
  },
  
  updateStatus: async (isTempGood: boolean) => {
    set({ loading: true, error: null })
    
    const { user } = useUserStore.getState()
    
    if (!user) {
      set({ error: 'No user found', loading: false })
      return
    }
    
    await updateUserStatus(user.id, isTempGood)
    
    set(state => ({
      user: state.user ? { ...state.user, is_temp_good: isTempGood } : null,
      loading: false
    }))
  }
}))