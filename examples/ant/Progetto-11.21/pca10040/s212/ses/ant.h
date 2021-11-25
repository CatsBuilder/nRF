
#ifndef __ANT_H__
  #define __ANT_H__
  #define DEVICENUMBER 5
  #define APP_ANT_OBSERVER_PRIO   1    /**< Application's ANT observer priority. You shouldn't need to modify this value. */
  #include "ant_channel_config.h"
  #include "ant_parameters.h"
  #include "ant_interface.h"
  #include "app_error.h"
  #include "nrf_log.h"
  #include "nrf_sdh_ant.h"
  #include "nrf_gpio.h"
  #include "sdk_errors.h"



  void ant_channel_tx_broadcast_setup(void);
  void ant_send(uint8_t *message);
  #endif

