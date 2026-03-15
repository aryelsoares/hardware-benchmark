// RAM info
use std::collections::HashMap;
use sysinfo::System;

pub fn get_ram_info(data: &mut HashMap<&str, f32>) {
    let mut sys: System = System::new_all();

    sys.refresh_memory();

    let usage: f32 = sys.used_memory() as f32 / 1e9;
    data.insert("ram_usage", usage);
}