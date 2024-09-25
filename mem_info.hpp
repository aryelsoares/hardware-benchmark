#ifndef MEM_INFO_HPP
#define MEM_INFO_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

void get_memory_info(double& memory) {
    std::ifstream meminfoFile("/proc/meminfo");
    if (!meminfoFile.is_open()) {
        std::cerr << "Failed to open /proc/meminfo" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string line;
    long total{};
    long available{};
    while (std::getline(meminfoFile, line)) {
        std::istringstream iss(line);
        std::string key;
        long value;
        std::string unit;

        iss >> key >> value >> unit;

        if (key == "MemTotal:") {
            total = value;
        } else if (key == "MemAvailable:") {
            available = value;
        }

        const long kB_to_GB = 1024L * 1024L; // 1 GB = 1024L^2 kB
        memory = static_cast<double>(total - available) / kB_to_GB;
    }
}

#endif