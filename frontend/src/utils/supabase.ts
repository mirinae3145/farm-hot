import { createClient } from "@supabase/supabase-js";

const supabase = createClient(process.env.REACT_APP_SUPABASE_URL ?? '', process.env.REACT_APP_SUPABASE_ANON_KEY ?? '');

export const postUser = async (name: string) => {
  const { data, error } = await supabase
    .from('user')
    .insert([
      { 
        is_temp_good: true,
        name,
        latitude: 0,
        longitude: 0,
        region_id: 1,
      }
    ]);

  if (error) {
      console.error(error);
      return null;
  }

  return data;
}

export const getUser = async (name: string) => {
    const { data, error } = await supabase
      .from('user')
      .select('*')
      .eq('name', name)
      .maybeSingle();

    if (error) {
        console.error(error);
        return null;
    }

    return data;
}

export const updateUserGPS = async (id: string, latitude: number, longitude: number) => {
    const { data, error } = await supabase
      .from('user')
      .update({
        latitude,
        longitude
      })
      .eq('id', id);

    if (error) {
        console.error(error);
        return null;
    }

    return data;
}

export const updateUserStatus = async (id: string, is_temp_good: boolean) => {
    const { data, error } = await supabase
      .from('user')
      .update({
        is_temp_good
      })
      .eq('id', id);

    if (error) {
        console.error(error);
        return null;
    }

    return data;
}
    