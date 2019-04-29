/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-03-05     whj4674672   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

/* defined the LED0 pin: PB1 */
#define LED1_PIN    GET_PIN(B, 0)


 struct rt_device_pwm *pwm_dev;
    

void huxideng_enter(void* p)
{
    uint32_t plus=0;
    uint8_t dir=0;
    pwm_dev = (struct rt_device_pwm*)rt_device_find("pwm3");
    rt_pwm_set(pwm_dev,4,100,0);
    rt_pwm_enable(pwm_dev,4);
    while(1){
        if(dir){
          if(--plus == 0){
              dir = 0;
          }
        }
        else{
          if(++plus >= 100){
              dir = 1;
          }
        }
        rt_pwm_set(pwm_dev,4,100,plus);
        rt_thread_delay(15);
    }
}


int main(void)
{
    int count = 1;
    /* set LED0 pin mode to output */	
    rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);	
	
    
    
    rt_thread_startup(rt_thread_create("huxideng",huxideng_enter,RT_NULL,500,5,1));
    
    
    while (count++)
    {
        rt_pin_write(LED1_PIN, PIN_HIGH);
        rt_thread_mdelay(1500);
        rt_pin_write(LED1_PIN, PIN_LOW);
        rt_thread_mdelay(1500);
    }
    return RT_EOK;
}
