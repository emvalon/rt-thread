/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-01-01     aozima       first implementation.
 * 2012-07-27     aozima       fixed variable uninitialized.
 */
#include <board.h>
#include "drv_qspi.h"
#include "spi_flash.h"
#include "spi_flash_sfud.h"

#include <rthw.h>
#include <finsh.h>


static void enter_qspi_mode()
{

    struct rt_qspi_message message;
    struct rt_qspi_device* qspi_flash_bus_dev;
    uint8_t s;
    qspi_flash_bus_dev = (struct rt_qspi_device*)rt_device_find("qspi10");
    
    memset(&message,0,sizeof(message));
    
    message.parent.cs_release   = 1;
    message.parent.cs_take      = 1;
    message.parent.length       = 1;
    message.parent.next         = RT_NULL;
    message.parent.send_buf     = 0;
    message.parent.recv_buf     = &s;
    
    message.instruction.content     = 0x35;
    message.instruction.qspi_lines  = 1;

    message.address.qspi_lines      = 0;  
    message.address.content         = 0;
    message.address.size            = 0;
    
    
    message.alternate_bytes.qspi_lines=0;
    message.alternate_bytes.content     = 0;
    message.alternate_bytes.size        = 0;
    
    message.dummy_cycles            = 0;
    
    message.qspi_data_lines         = 1;
    
    rt_qspi_transfer_message(qspi_flash_bus_dev,&message);
    rt_kprintf("read w25q cs2:%02x\r\n",s);
    
    //------------------------------------------------------------   
   
    
    message.parent.cs_release   = 1;
    message.parent.cs_take      = 1;
    message.parent.length       = 1;
    message.parent.next         = RT_NULL;
    message.parent.send_buf     = &s;
    message.parent.recv_buf     = 0;
    
    message.instruction.content     = 0x06;
    message.instruction.qspi_lines  = 1;

    message.address.qspi_lines      = 0;   
    message.alternate_bytes.qspi_lines=0;
    message.dummy_cycles            = 0;
    message.qspi_data_lines         = 0;
    
    rt_qspi_transfer_message(qspi_flash_bus_dev,&message);
    
    
 //------------------------------------------------------------   
    s   |= 0x02;
    
     message.parent.cs_release   = 1;
    message.parent.cs_take      = 1;
    message.parent.length       = 1;
    message.parent.next         = RT_NULL;
    message.parent.send_buf     = &s;
    message.parent.recv_buf     = 0;
    
    message.instruction.content     = 0x31;
    message.instruction.qspi_lines  = 1;

    message.address.qspi_lines      = 0;   
    message.alternate_bytes.qspi_lines=0;
    message.dummy_cycles            = 0;
    message.qspi_data_lines         = 1;
    
    rt_qspi_transfer_message(qspi_flash_bus_dev,&message);
    // ----------------------------------------------------
    
     message.parent.cs_release   = 1;
    message.parent.cs_take      = 1;
    message.parent.length       = 1;
    message.parent.next         = RT_NULL;
    message.parent.send_buf     = 0;
    message.parent.recv_buf     = &s;
    
    message.instruction.content     = 0x35;
    message.instruction.qspi_lines  = 1;

    message.address.qspi_lines      = 0;   
    message.alternate_bytes.qspi_lines=0;
    message.dummy_cycles            = 0;
    message.qspi_data_lines         = 1;
    
    rt_qspi_transfer_message(qspi_flash_bus_dev,&message);
    rt_kprintf("read w25q cs2:%02x\r\n",s);
 //------------------------------------------------------------  
    
  
    
    
    
    message.parent.cs_release   = 1;
    message.parent.cs_take      = 1;
    message.parent.length       = 0;
    message.parent.next         = RT_NULL;
    message.parent.send_buf     = (void*)1;
    message.parent.recv_buf     = 0;
    
    message.instruction.content     = 0x38;
    message.instruction.qspi_lines  = 1;

    message.address.qspi_lines      = 0;   
    message.alternate_bytes.qspi_lines=0;
    message.dummy_cycles            = 0;
    message.qspi_data_lines         = 0;
    
    rt_qspi_transfer_message(qspi_flash_bus_dev,&message);
    
//    QSPI_CommandTypeDef Cmdhandler;
//    
//    Cmdhandler.AddressMode

}

static int rt_hw_spi_device_init(void)
{
    /* attach cs */
    {
 //       static struct rt_spi_device spi_device;
//        static struct stm32_hw_spi_cs  spi_cs;
		rt_err_t result;
//
//        spi_cs.GPIOx = GPIOB;
//        spi_cs.GPIO_Pin = GPIO_PIN_6;
//      
//    
//        HAL_GPIO_WritePin(spi_cs.GPIOx, spi_cs.GPIO_Pin, GPIO_PIN_SET);
        
        result = stm32_qspi_bus_attach_device("qspi1","qspi10",GET_PIN(B,6) ,1,enter_qspi_mode,enter_qspi_mode);
//        result = rt_spi_bus_attach_device(&spi_device, "qspi10", "qspi1", (void*)&spi_cs);
		if (result != RT_EOK)
		{
			return result;
		}
    }

	return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_hw_spi_device_init);

static int rt_hw_spi_flash_with_sfud_init(void)
{
    enter_qspi_mode();
    if (RT_NULL == rt_sfud_flash_probe("W25Q256", "qspi10"))
    {
        return RT_ERROR;
    };

	return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_spi_flash_with_sfud_init);
