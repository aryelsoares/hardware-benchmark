#ifndef CPU_USAGE_HPP
#define CPU_USAGE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

struct CpuUsage {
    long user, nice, system, idle, iowait, irq, softirq, steal;
};

CpuUsage cpu_use() {
    std::ifstream proc_stat("/proc/stat");
    std::string cpu;
    CpuUsage usage;

    proc_stat >> cpu >> usage.user >> usage.nice >> usage.system >> usage.idle
              >> usage.iowait >> usage.irq >> usage.softirq >> usage.steal;
    
    return usage;
}

double calculate_cpu_use(CpuUsage start, CpuUsage end) {
    long idle_time = end.idle - start.idle;
    long total_time = (end.user - start.user) + (end.nice - start.nice)
                    + (end.system - start.system) + (end.idle - start.idle)
                    + (end.iowait - start.iowait) + (end.irq - start.irq)
                    + (end.softirq - start.softirq) + (end.steal - start.steal);

    if (total_time == 0) {
        return 0.0;
    }
    return (static_cast<double>(total_time - idle_time) / static_cast<double>(total_time)) * 100.0;
}

double cpu_pow() {
    std::ifstream cpu_power_file("/sys/class/powercap/intel-rapl:0/energy_uj");
    long long pow{};

    if (cpu_power_file.is_open()) {
        cpu_power_file >> pow;
        cpu_power_file.close();
    } else {
        std::cerr << "Failed to read CPU power" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return pow;
}

double calculate_power_watts(long long start_energy, long long end_energy) {
    long long energy_diff_uj = end_energy - start_energy; // microjoules (uJ)
    double energy_joules = static_cast<int>(energy_diff_uj) * 1e-6; // Convert to J

    return energy_joules / 0.5; // Watts (J/s)
}

void get_cpu_info(double& use, int& pow) {
    CpuUsage start_use = cpu_use();
    long long start_pow = cpu_pow();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    CpuUsage end_use = cpu_use();
    long long end_pow = cpu_pow();

    use = calculate_cpu_use(start_use, end_use);
    pow = calculate_power_watts(start_pow, end_pow);
}

#endif