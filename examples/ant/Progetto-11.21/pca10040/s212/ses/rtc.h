#ifndef __RTC_H__
  #define __RTC_H__
  #include "nrf_drv_rtc.h"
  #include "nrf_drv_clock.h"

  static nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(0); //Declaring an instance of nrf_drv_rtc for RTC0.

  void rtc_handler(nrf_drv_rtc_int_type_t);
  void lfclk_config(void);
  void rtc_config(void);
  void rtc_start(void);
#endif


































  /*#include "boards.h"
  #include "app_error.h"
  #include <stdint.h>
  #include <stdbool.h>
  #include <stdio.h>
  #include <string.h>
  #include "nrf_drv_saadc.h"
  #include "nrf_drv_ppi.h"
  #include "nrf_drv_timer.h"
  #include "nrf_delay.h"
  #include "app_util_platform.h"
  #include "nrf_pwr_mgmt.h"
  #include "nrf_drv_power.h"
  #include "nrf_log.h"
  #include "nrf_log_ctrl.h"
  #include "nrf_log_default_backends.h"
  #include "nrf.h"
  */