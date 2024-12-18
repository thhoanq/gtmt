/***************************************************************************//**
 * @file main.c
 * @brief main() function.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include "sl_component_catalog.h"
#include "sl_system_init.h"
#include "app.h"
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif
#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "sl_system_kernel.h"
#else // SL_CATALOG_KERNEL_PRESENT
#include "sl_system_process_action.h"
#endif // SL_CATALOG_KERNEL_PRESENT

#include <sl_udelay.h>
#include <app_lcd.h>
#include "em_cmu.h"
#include "em_gpio.h"
#include "sl_sleeptimer.h"
#include <DHT.h>
#include "app_log.h"

uint32_t Temperature, Humidity;

int main(void)
{
  // Initialize Silicon Labs device, system, service(s) and protocol stack(s).
  // Note that if the kernel is present, processing task(s) will be created by
  // this call.
  sl_system_init();

  // Initialize the application. For example, create periodic timer(s) or
  // task(s) if the kernel is present.
  app_init();
  memlcd_app_init();
  app_log_info("Test\n");
  DHT_DataTypedef DHT11_Data;

  CMU_ClockEnable(cmuClock_GPIO, true);


//  DHT_Name DHT11;
//  DHT_Init(&DHT11, gpioPortB, 0);

    char buffer_disp[20];

#if defined(SL_CATALOG_KERNEL_PRESENT)
  // Start the kernel. Task(s) created in app_init() will start running.
  sl_system_kernel_start();
#else // SL_CATALOG_KERNEL_PRESENT
  while (1) {
    // Do not remove this call: Silicon Labs components process action routine
    // must be called from the super loop.
    sl_system_process_action();

    // Application process.
    app_process_action();

    DHT_GetData(&DHT11_Data);
    Temperature = DHT11_Data.Temperature;
    Humidity = DHT11_Data.Humidity;

        app_log_info("Nhom DDCH\r\n");
        app_log_info("Temp: %d\r\n", Temperature);
        app_log_info("Humd: %d\r\n", Humidity);
    sl_sleeptimer_delay_millisecond(1000); //1s
    snprintf(buffer_disp, sizeof(buffer_disp), "Nhiet do: %d",Temperature );
    memlcd_display(2, buffer_disp);
    snprintf(buffer_disp, sizeof(buffer_disp), "Do am: %d", Humidity);
    memlcd_display(3, buffer_disp);

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
    // Let the CPU go to sleep if the system allows it.
    sl_power_manager_sleep();
#endif
  }
#endif // SL_CATALOG_KERNEL_PRESENT
}
