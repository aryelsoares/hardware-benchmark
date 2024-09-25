#ifndef GPU_USAGE_HPP
#define GPU_USAGE_HPP

#include <iostream>
#include <fstream>

void get_gpu_info(double& use, int& pow) {
    std::ifstream gpu_busy_file("/sys/class/drm/card1/device/gpu_busy_percent");
    std::ifstream gpu_power_file("/sys/class/drm/card1/device/hwmon/hwmon5/power1_average");

    if (gpu_busy_file.is_open()) {
        gpu_busy_file >> use;
        gpu_busy_file.close();
    } else {
        std::cerr << "Failed to read GPU usage" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if (gpu_power_file.is_open()) {
        long value{};
        gpu_power_file >> value;
        pow = static_cast<int>(value / 10e5); // Convert to watts
        gpu_power_file.close();
    } else {
        std::cerr << "Failed to read GPU power" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

#endif