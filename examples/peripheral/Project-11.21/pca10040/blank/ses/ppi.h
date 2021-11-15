#ifndef __PPI_H__
  #define __PPI_H__
  
  #include "nrf_drv_ppi.h"
  #include "nrf_drv_saadc.h"
  #include "nrf_drv_rtc.h"
  
  static nrf_ppi_channel_t ppi_channel_adc;// A struct to hold ppi channel values
  extern nrf_drv_rtc_t rtc;

  static void ppi_init(void);
  #endif