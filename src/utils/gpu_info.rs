// GPU info
use std::{
    fs,
    collections::HashMap
};

const GPU_USAGE: &str = "/sys/class/drm/card1/device/gpu_busy_percent";
const GPU_POWER: &str = "/sys/class/drm/card1/device/hwmon/hwmon4/power1_average";

fn read_gpu_usage() -> Option<f32> {
    let usage: u64 = fs::read_to_string(GPU_USAGE)
        .ok()?
        .trim()
        .parse::<u64>()
        .ok()?;

    Some(usage as f32)
}

fn read_gpu_ppt_watts() -> Option<f32> {
    let power: u64 = fs::read_to_string(GPU_POWER)
        .ok()?
        .trim()
        .parse::<u64>()
        .ok()?;

    Some(power as f32 / 1e6)
}

pub fn get_gpu_info(data: &mut HashMap<&str, f32>) {
    data.insert("gpu_usage", read_gpu_usage().unwrap_or(0.0));
    data.insert("gpu_power", read_gpu_ppt_watts().unwrap_or(0.0));
}