// Temperature info
use std::collections::HashMap;
use sysinfo::Components;

const CPU_TEMP: &str = "tctl";
const GPU_TEMP_EDGE: &str = "edge";
const GPU_TEMP_JUNCTION: &str = "junction";
const GPU_TEMP_MEM: &str = "mem";

fn read_temp(sensor: &str) -> Option<f32> {
    let components: Components = Components::new_with_refreshed_list();

    for component in components.iter() {
        let label: String = component.label().to_lowercase();

        if label.contains(sensor) {
            return component.temperature();
        }
    }

    None
}

pub fn get_temp_info(data: &mut HashMap<&str, f32>) {
    let sensor_list: Vec<&str> = vec![CPU_TEMP, GPU_TEMP_EDGE, GPU_TEMP_JUNCTION, GPU_TEMP_MEM];
    let data_list: Vec<&str> = vec!["cpu_temp", "gpu_temp_edge", "gpu_temp_junction", "gpu_temp_mem"];

    for (sensor, key) in sensor_list.iter().zip(data_list.iter()) {
        let temp: f32 = read_temp(sensor).unwrap_or(0.0);
        data.insert(*key, temp);
    }
}