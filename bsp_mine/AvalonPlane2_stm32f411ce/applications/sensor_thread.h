#ifndef __SENSOR_THREAD_H__
#define __SENSOR_THREAD_H__

#include "mpu6xxx.h"
struct sensor_data{
    struct mpu6xxx_3axes    dataAccel;
};

typedef struct sensor_data* sensor_data_t;




void read_sensor_enter(void *para);
void process_sensor_enter(void *para);
    

#endif
