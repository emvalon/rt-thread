/*
 * Copyright (c) 2006-2018, valon shen(valonshen@foxmail.com)
 *
 * Change Logs:
*/
#include <rtthread.h>
#include <board.h>
#include "sensor_thread.h"


#define SENSOR_MAIL_SIZE    20


static rt_mailbox_t sensorM;

int create_communicate_queue(void)
{
    sensorM = rt_mb_create("sensorM",SENSOR_MAIL_SIZE,RT_IPC_FLAG_FIFO);
    if(sensorM == RT_NULL){
        rt_kprintf("sensor mb create fault\r\n");
        while(1);
    }
    return 0;
}
INIT_APP_EXPORT(create_communicate_queue);



/***********************************
 read sensor thread enter
************************************/
void read_sensor_enter(void *para)
{
    struct mpu6xxx_device*  mpu6050;
    sensor_data_t  pData;
    rt_err_t    res;
   
    mpu6050 = mpu6xxx_init("i2c1",0x68);
    if(mpu6050 == RT_NULL){
        rt_kprintf("mpu6050 not find\r\n");
        return;
    }
    while(1){
        pData = (sensor_data_t)rt_malloc(sizeof(struct sensor_data));
        mpu6xxx_get_accel(mpu6050,&(pData->dataAccel));
        //send data mailbox
        res = rt_mb_send(sensorM,(rt_ubase_t)pData);
        if(res != RT_EOK){
            rt_kprintf("sensor send fault:%d\r\n",res);
        }
        rt_thread_delay(5000);
    }
}


/***********************************
 process sensor data thread enter
************************************/
void process_sensor_enter(void *para)
{
   
    rt_err_t    res; 
    sensor_data_t pData;
    while(1){
        res = rt_mb_recv(sensorM,(rt_ubase_t*)&pData,RT_WAITING_FOREVER);
        if(res == RT_EOK){            
            rt_kprintf("x:%d\ty:%d\tz:%d\r\n",pData->dataAccel.x,pData->dataAccel.y,pData->dataAccel.z);
            rt_free(pData);
        }
    }
}




















