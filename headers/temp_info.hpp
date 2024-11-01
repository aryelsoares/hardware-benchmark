#ifndef TEMP_HPP
#define TEMP_HPP

#include <iostream>
#include <sensors/sensors.h>
#include <string>

std::string cpu_chip_prefix {"k10temp"}; // CPU chip prefix
std::string gpu_chip_prefix {"amdgpu"}; // GPU chip prefix

void get_temp_info(int& tctl, int& edge, int& junction, int& mem) {
    if (sensors_init(nullptr) != 0) {
        std::cerr << "Failed to initialize sensors library.\n";
        std::exit(EXIT_FAILURE);
    }

    const sensors_chip_name *chip;
    int chip_nr = 0;

    // iterates through detected chips
    while ((chip = sensors_get_detected_chips(nullptr, &chip_nr)) != nullptr) {
        std::string chip_prefix = chip->prefix;

        // CPU chip verify
        if (chip_prefix == cpu_chip_prefix) {
            const sensors_feature *feature;
            int feature_nr = 0;

            while ((feature = sensors_get_features(chip, &feature_nr)) != nullptr) {
                std::string feature_name = sensors_get_label(chip, feature);

                const sensors_subfeature *subfeature;
                int subfeature_nr = 0;

                while ((subfeature = sensors_get_all_subfeatures(chip, feature, &subfeature_nr)) != nullptr) {
                    if (subfeature->type == SENSORS_SUBFEATURE_TEMP_INPUT) {
                        double temp;
                        if (sensors_get_value(chip, subfeature->number, &temp) == 0) {
                            if (feature_name == "Tctl") {
                                tctl = static_cast<int>(temp);
                            }
                        }
                    }
                }
            }
        }

        // GPU chip verify
        else if (chip_prefix == gpu_chip_prefix) {
            const sensors_feature *feature;
            int feature_nr = 0;

            while ((feature = sensors_get_features(chip, &feature_nr)) != nullptr) {
                std::string feature_name = sensors_get_label(chip, feature);

                const sensors_subfeature *subfeature;
                int subfeature_nr = 0;

                while ((subfeature = sensors_get_all_subfeatures(chip, feature, &subfeature_nr)) != nullptr) {
                    if (subfeature->type == SENSORS_SUBFEATURE_TEMP_INPUT) {
                        double temp;
                        if (sensors_get_value(chip, subfeature->number, &temp) == 0) {
                            if (feature_name == "edge") edge = static_cast<int>(temp);
                            else if (feature_name == "junction") junction = static_cast<int>(temp);
                            else if (feature_name == "mem") mem = static_cast<int>(temp);
                        }
                    }
                }
            }
        }
    }

    sensors_cleanup();
}

#endif