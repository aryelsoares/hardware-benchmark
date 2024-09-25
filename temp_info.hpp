#ifndef TEMP_HPP
#define TEMP_HPP

#include <iostream>
#include <sensors/sensors.h>

/* === SENSOR LIST ===
0: CPU (tctl)
1: SSD (composite)
2: SSD (sensor 1)
3: SSD (sensor 2)
4: GPU (edge)
5: GPU (junction)
6: GPU (mem)
7: SSD (composite) */

void get_temp_info(int& tctl, int& edge, int& junction, int& mem) {
    if (sensors_init(nullptr) != 0) {
        std::cerr << "Failed to initialize sensors library.\n";
    }

    int temp_list[8]; // sensor amount
    size_t temp_idx{};

    // Check running chips
    const sensors_chip_name *chip;
    int chip_nr = 0;

    while ((chip = sensors_get_detected_chips(nullptr, &chip_nr)) != nullptr) {
        const char *chip_name = sensors_get_adapter_name(&chip->bus);
        const sensors_feature *feature;
        int feature_nr = 0;

        // Check features
        while ((feature = sensors_get_features(chip, &feature_nr)) != nullptr) {
            const sensors_subfeature *subfeature;
            int subfeature_nr = 0;

            // Verify subfeatures as temperature
            while ((subfeature = sensors_get_all_subfeatures(chip, feature, &subfeature_nr)) != nullptr) {
                if (subfeature->type == SENSORS_SUBFEATURE_TEMP_INPUT) {
                    double temp;
                    if (sensors_get_value(chip, subfeature->number, &temp) == 0) {
                        temp_list[temp_idx++] = temp;
                    } else {
                        std::cerr << "Failed to read temperature for subfeature.\n";
                        std::exit(EXIT_FAILURE);
                    }
                }
            }
        }
    }

    sensors_cleanup();

    // Temp info
    tctl = temp_list[0];
    edge = temp_list[4];
    junction = temp_list[5];
    mem = temp_list[6];
}

#endif