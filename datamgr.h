#ifndef DATAMGR_H
#define DATAMGR_H

typedef struct {
    int sensor_id;
    double temperature;
    long timestamp;
} sensor_data_t;

void process_sensor_data(int sensor_id, double temperature, long timestamp);
void check_temperature_range(double temperature);

#endif
