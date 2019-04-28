/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2015-07-29     Arda.Fu      first implementation
 */
#include <rtthread.h>
#include <board.h>
#include "sensor_thread.h"



// read sensor thread
#define READ_SENSOR_STACK_SIZE      800
#define READ_SENSOR_PRO             5
#define READ_SENSOR_SLICE           2
// process sensor data thread
#define PROCESS_SENSOR_STACK_SIZE   800
#define PROCESS_SENSOR_PRO          6
#define PROCESS_SENSOR_SLICE        2



int main(void)
{
    
    rt_thread_t readSensorThread;
    rt_thread_t processSensorThread;
    /* user app entry */
    
    readSensorThread    = rt_thread_create(   
                            "readSen",      read_sensor_enter,  
                            RT_NULL,            READ_SENSOR_STACK_SIZE, 
                            READ_SENSOR_PRO,    READ_SENSOR_SLICE   );
    
    processSensorThread = rt_thread_create(   
                            "process",   process_sensor_enter,  
                            RT_NULL,            PROCESS_SENSOR_STACK_SIZE, 
                            PROCESS_SENSOR_PRO, PROCESS_SENSOR_SLICE   );
    rt_thread_startup(readSensorThread);
    rt_thread_startup(processSensorThread);
    return 0;
}


















