#include <iostream>
#include <csignal>
#include <string>
#include <thread>
#include <chrono>
#include "influxdb.hpp"
#include "cpu_info.hpp"
#include "gpu_info.hpp"
#include "mem_info.hpp"
#include "temp_info.hpp"

volatile bool keep_running = true;

void signal_handler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        std::cout << "\nClosing application..." << std::endl;
        keep_running = false;
    }
}

int main() {

    std::cout << "Starting to extract data. Ctrl + C to close it." << std::endl;

    std::signal(SIGINT, signal_handler);

    std::string token = "hardware_info";
    std::string org = "home_org";

    influxdb_cpp::server_info si("influxdb", 8086, org, token, "home");

    while (keep_running) {

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
        // CPU data
        double cpu_usage{};
        int cpu_power{};
        get_cpu_info(cpu_usage, cpu_power);

        // GPU data
        double gpu_usage{};
        int gpu_power{};
        get_gpu_info(gpu_usage, gpu_power);

        // Memory data
        double memory{};
        get_memory_info(memory);

        // Temp data
        int gpu_temp_edge{};
        int gpu_temp_junction{};
        int gpu_temp_mem{};
        int cpu_temp{};
        get_temp_info(cpu_temp, gpu_temp_edge, gpu_temp_junction, gpu_temp_mem);

        int res = influxdb_cpp::builder()
            .meas("CPU")
            .field("usage", cpu_usage)
            .field("power", cpu_power)
            .field("temp", cpu_temp)

            .meas("GPU")
            .field("usage", gpu_usage)
            .field("power", gpu_power)
            .field("temp_edge", gpu_temp_edge)
            .field("temp_junction", gpu_temp_junction)
            .field("temp_mem", gpu_temp_mem)

            .meas("RAM")
            .field("usage", memory)
            .post_http(si);
    
        if (res != 0) {
            std::cerr << "Failed to post data to InfluxDB. Error code: " << res << std::endl;
            return 1;
        }
    }

    std::cout << "Finished program.\n";
    return 0;
}
