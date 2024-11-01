#ifndef GPU_USAGE_HPP
#define GPU_USAGE_HPP

#include <iostream>
#include <fstream>

std::string gpu_use_info {"/sys/class/drm/card1/device/gpu_busy_percent"}; // GPU use
std::string gpu_power_info {"/sys/class/drm/card1/device/hwmon/hwmon4/power1_average"}; // GPU power

void get_gpu_info(double& use, int& pow) {
    std::ifstream gpu_busy_file(gpu_use_info);
    std::ifstream gpu_power_file(gpu_power_info);

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