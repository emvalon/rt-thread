/*
 * Copyright (c) 2006-2018, valon shen(valonshen@foxmail.com)
 *
 * Change Logs:
*/
#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>
#include "sensor_thread.h"


#define SENSOR_MAIL_SIZE    200u
#define MPU_INT_PIN         2u
#define MPU_READY_FLAG      1ul


static rt_mailbox_t sensorMailbox;
static rt_event_t   sensorEvent;

static void setReadyFlag(void *arg)
{
    if(rt_event_send(sensorEvent,MPU_READY_FLAG) != RT_EOK){
        rt_kprintf("sensor data ready set fault\r\n");
    }
}

int create_communicate_queue(void)
{
    //create sensor mailbox
    sensorMailbox = rt_mb_create("sensorM",SENSOR_MAIL_SIZE,RT_IPC_FLAG_FIFO);
    if(sensorMailbox == RT_NULL){
        rt_kprintf("sensor mb create fault\r\n");
        while(1);
    }
    //create sensor event
    sensorEvent = rt_event_create("sensorE",0);
    if(sensorEvent == RT_NULL){
        rt_kprintf("sensor event create fault\r\n");
        while(1);
    }
    //init mpu interrupt pin
    rt_pin_mode(MPU_INT_PIN,PIN_MODE_INPUT_PULLDOWN);
    rt_pin_attach_irq(MPU_INT_PIN,PIN_IRQ_MODE_RISING,setReadyFlag,RT_NULL);
    rt_pin_irq_enable(MPU_INT_PIN,PIN_IRQ_ENABLE);
    return 0;
}
INIT_APP_EXPORT(create_communicate_queue);



/***********************************
 read sensor thread enter
************************************/
rt_err_t mpu6xxx_write_reg(struct mpu6xxx_device *dev, rt_uint8_t reg, rt_uint8_t data);
void read_sensor_enter(void *para)
{
    struct mpu6xxx_device*  mpu6050;
    uint32_t flag;
    sensor_data_t  pData;
    rt_err_t    res;
   
    mpu6050 = mpu6xxx_init("i2c1",0x68);
    mpu6xxx_write_reg(mpu6050,25,3);
    mpu6xxx_write_reg(mpu6050,26,4);
    mpu6xxx_write_reg(mpu6050,56,1);
    if(mpu6050 == RT_NULL){
        rt_kprintf("mpu6050 not find\r\n");
        return;
    }
    while(1){
        res = rt_event_recv( sensorEvent,           MPU_READY_FLAG, 
                             RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR,
                             RT_WAITING_FOREVER,    &flag       );
        if(res != RT_EOK){
            rt_kprintf("recv sensor ready flag fault\r\n");
        }
        else{
            if(flag & MPU_READY_FLAG){
                pData = (sensor_data_t)rt_malloc(sizeof(struct sensor_data));
                mpu6xxx_get_accel(mpu6050,&(pData->dataAccel));
                //send data mailbox
                res = rt_mb_send(sensorMailbox,(rt_ubase_t)pData);
                if(res != RT_EOK){
                    rt_kprintf("sensor send fault:%d  entry:%d\r\n",res,sensorMailbox->entry );  
                    rt_thread_delay(1);
                }
            }
        }
        flag = 0;
    }
}


/***********************************
 process sensor data thread enter
************************************/
void process_sensor_enter(void *para)
{
    rt_err_t    res; 
    sensor_data_t pData;
    uint32_t tick,preTick,cnt=0;
    preTick = rt_tick_get();
    while(1){ 
        res = rt_mb_recv(sensorMailbox,(rt_ubase_t*)&pData,RT_WAITING_FOREVER);
        if(res == RT_EOK){            
            //rt_kprintf("x:%d\ty:%d\tz:%d\r\n",pData->dataAccel.x,pData->dataAccel.y,pData->dataAccel.z);
            rt_free(pData);
            cnt++;
            
        }
        tick = rt_tick_get();
        if( tick - preTick >=1000 ){
            rt_kprintf("read cnt:%d\r\n",cnt);
            preTick = tick;
            cnt=0;
        }
    }
}




















