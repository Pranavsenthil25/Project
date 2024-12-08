#include <stdio.h>
#include "datamgr.h"

void process_sensor_data(int sensor_id, double temperature, long timestamp) {
    printf("Processing data: Sensor ID=%d, Temperature=%.2f, Timestamp=%ld\n", sensor_id, temperature, timestamp);
    check_temperature_range(temperature);
}

void check_temperature_range(double temperature) {
    if (temperature < 10.0 || temperature > 30.0) {
        printf("ALERT: Temperature out of range! (%.2f)\n", temperature);
    }
}
