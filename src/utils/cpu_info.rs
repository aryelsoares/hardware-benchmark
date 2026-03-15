// CPU info
use std::{
    fs,
    time::Instant,
    collections::HashMap
};
use sysinfo::System;

const CPU_POWER: &str = "/sys/class/powercap/intel-rapl:0/energy_uj";

pub struct CpuEnergyState {
    pub last_energy: Option<f64>,
    pub last_time: Option<Instant>
}

fn read_cpu_energy_joules(path: &str) -> Option<f64> {
    let micro_joules: u64 = fs::read_to_string(path)
        .ok()?
        .trim()
        .parse::<u64>()
        .ok()?;
    
    Some(micro_joules as f64 / 1e6)
}

fn cpu_power_watts_step(path: &str, state: &mut CpuEnergyState) -> Option<f64> {
    let now: Instant = Instant::now();
    let energy: f64 = read_cpu_energy_joules(path)?;

    match (state.last_energy, state.last_time) {
        (Some(e_prev), Some(t_prev)) => {
            let delta_e: f64 = energy - e_prev;
            let delta_t: f64 = (now - t_prev).as_secs_f64();

            state.last_energy = Some(energy);
            state.last_time = Some(now);

            if delta_t > 0.0 {
                Some(delta_e / delta_t)
            } else {
                None
            }
        }
        _ => {
            state.last_energy = Some(energy);
            state.last_time = Some(now);
            None
        }
    }
}

pub fn get_cpu_info(sys: &mut System, cpu_energy_state: &mut CpuEnergyState, data: &mut HashMap<&str, f32>) {
    sys.refresh_cpu_usage();
    data.insert("cpu_usage", sys.global_cpu_usage());

    let power: Option<f64> = cpu_power_watts_step(CPU_POWER, cpu_energy_state);
    data.insert("cpu_power", power.unwrap_or(0.0) as f32);
}