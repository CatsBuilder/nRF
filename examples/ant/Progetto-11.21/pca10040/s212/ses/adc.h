#ifndef __ADC_H__
  #define __ADC_H__

  #include "nrf_drv_saadc.h"
  #include "nrf_log.h"

  // Samples are needed to be stored in a buffer, we define the length here
  #define SAMPLE_BUFFER_LEN 5 

  // Save the samples in double buffer which is  a two dimentional array
  static nrf_saadc_value_t m_buffer_pool[2][SAMPLE_BUFFER_LEN]; 

  void saadc_init(void);
  void saadc_callback_handler(nrf_drv_saadc_evt_t const *);
  #endif































   /* #include <stdbool.h>
  #include <stdint.h>
  #include <stdio.h>
  #include <string.h>
  #include "nrf.h"
  #include "nrf_log_ctrl.h"
  #include "nrf_log_default_backends.h"
  #include "rtc.h"
  #include "nrf_drv_ppi.h"
  #include "nrf_drv_rtc.h"
  #include "boards.h"
  #include "app_error.h"
  #include "nrf_delay.h"
  #include "app_util_platform.h"
  #include "nrf_pwr_mgmt.h"*/