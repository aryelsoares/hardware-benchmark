mod utils;

use crate::utils::cpu_info::{CpuEnergyState, get_cpu_info};
use crate::utils::gpu_info::get_gpu_info;
use crate::utils::temp_info::get_temp_info;
use crate::utils::ram_info::get_ram_info;

use std::{collections::HashMap};
use tokio::time::{sleep, Duration};
use tokio::signal::unix::{signal, SignalKind};
use futures::prelude::*;
use influxdb2::{Client, models::Status, models::DataPoint};
use sysinfo::System;

// Data extraction delay
static INTERVAL: Duration = Duration::from_secs(1);

// Wait for InfluxDB to be ready
async fn wait_for_influx(client: &Client) {
    loop {
        match client.health().await {
            Ok(h) if h.status == Status::Pass => {
                println!("InfluxDB ready");
                break;
            }
            _ => {
                println!("Waiting for InfluxDB...");
                sleep(Duration::from_secs(2)).await;
            }
        }
    }
}

// Main
#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    // Environment variables
    let host: String = std::env::var("INFLUXDB_HOST").unwrap();
    let org: String = std::env::var("INFLUXDB_ORG").unwrap();
    let token: String = std::env::var("INFLUXDB_TOKEN").unwrap();
    let bucket: String = std::env::var("INFLUXDB_BUCKET").unwrap();

    let client: Client = Client::new(host, org, token);

    // Wait for InfluxDB
    wait_for_influx(&client).await;
    println!("Starting metrics collection...");

    let mut sys: System = System::new_all();
    let mut cpu_energy_state: CpuEnergyState = CpuEnergyState {
        last_energy: None,
        last_time: None
    };

    let mut sigterm = signal(SignalKind::terminate())?;
    let mut sigint = signal(SignalKind::interrupt())?;

    loop {
        tokio::select! {
            // Container-safe shutdown (SIGTERM / docker stop)
            _ = sigterm.recv() => {
                println!("SIGTERM received. Shutting down...");
                break;
            }

            _ = sigint.recv() => {
                println!("SIGINT received. Shutting down...");
                break;
            }

            // Metrics collection interval
            _ = sleep(INTERVAL) => {
                let mut data: HashMap<&str, f32> = HashMap::new();

                get_cpu_info(&mut sys, &mut cpu_energy_state, &mut data);
                get_gpu_info(&mut data);
                get_ram_info(&mut data);
                get_temp_info(&mut data);

                let cpu: DataPoint = DataPoint::builder("CPU")
                    .field("usage", *data.get("cpu_usage").unwrap_or(&0.0) as f64)
                    .field("power", *data.get("cpu_power").unwrap() as f64)
                    .field("temp", *data.get("cpu_temp").unwrap() as f64)
                    .build()?;

                let gpu: DataPoint = DataPoint::builder("GPU")
                    .field("usage", *data.get("gpu_usage").unwrap() as f64)
                    .field("power", *data.get("gpu_power").unwrap() as f64)
                    .field("temp_edge", *data.get("gpu_temp_edge").unwrap() as f64)
                    .field("temp_junction", *data.get("gpu_temp_junction").unwrap() as f64)
                    .field("temp_mem", *data.get("gpu_temp_mem").unwrap() as f64)
                    .build()?;

                let ram: DataPoint = DataPoint::builder("RAM")
                    .field("usage", *data.get("ram_usage").unwrap() as f64)
                    .build()?;

                if let Err(e) = client
                    .write(&bucket, stream::iter(vec![cpu, gpu, ram]))
                    .await
                {
                    eprintln!("Failed to write metrics: {e}");
                }
            }
        }
    }

    println!("Collector stopped gracefully.");
    Ok(())
}