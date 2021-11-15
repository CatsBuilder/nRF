#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_drv_rtc.h"
#include "nrf_drv_saadc.h"
#include "nrf_drv_clock.h"
#include "boards.h"
#include "bsp.h"
//#include "hardfault.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_delay.h"
#include "app_error.h"
#include <stdint.h>
#include <stdbool.h>
#include "dma.h"
#include "ant_channel_config.h"
#include "ant_parameters.h"
#include "ant_interface.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ant.h"
#include "rtc.h"
#include "adc.h"
#include "ppi.h"
#include "dma.h"    
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


#define COMPARE_COUNTERTIME  (3UL)                                        /**< Get Compare event COMPARE_TIME seconds after the counter starts from 0. */
#define LED 11
#ifdef BSP_LED_0
    #define TICK_EVENT_OUTPUT     BSP_LED_0                                 /**< Pin number for indicating tick event. */
#endif
#ifndef TICK_EVENT_OUTPUT
    #error "Please indicate output pin"
#endif
#ifdef BSP_LED_1
    #define COMPARE_EVENT_OUTPUT   BSP_LED_1                                /**< Pin number for indicating compare event. */
#endif
#ifndef COMPARE_EVENT_OUTPUT
    #error "Please indicate output pin"
#endif

void log_init(void){
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

void softdevice_setup(void){
    ret_code_t err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    ASSERT(nrf_sdh_is_enabled());

    err_code = nrf_sdh_ant_enable();
    APP_ERROR_CHECK(err_code);
}

void ant_message_send(void)
{
    uint8_t         message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE];
    static uint8_t  counter = 1u;

    memset(message_payload, 0, ANT_STANDARD_DATA_PAYLOAD_SIZE);
    // Assign a new value to the broadcast data.
    message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE - 1] = counter;

    // Broadcast the data.
    ret_code_t err_code = sd_ant_broadcast_message_tx(BROADCAST_CHANNEL_NUMBER,
                                                      ANT_STANDARD_DATA_PAYLOAD_SIZE,
                                                      message_payload);
    APP_ERROR_CHECK(err_code);

    counter++;
}


static void ant_channel_tx_broadcast_setup(void)
{
    ant_channel_config_t broadcast_channel_config =
    {
        .channel_number    = BROADCAST_CHANNEL_NUMBER,
        .channel_type      = CHANNEL_TYPE_SLAVE,
        .ext_assign        = 0x00,
        .rf_freq           = RF_FREQ,
        .transmission_type = CHAN_ID_TRANS_TYPE,
        .device_type       = CHAN_ID_DEV_TYPE,
        .device_number     = CHAN_ID_DEV_NUM,
        .channel_period    = CHAN_PERIOD,
        .network_number    = ANT_NETWORK_NUM,
    };

    ret_code_t err_code = ant_channel_init(&broadcast_channel_config);
    APP_ERROR_CHECK(err_code);

    // Fill tx buffer for the first frame.
    ant_message_send();

    // Open channel.
    err_code = sd_ant_channel_open(BROADCAST_CHANNEL_NUMBER);
    APP_ERROR_CHECK(err_code);
}



void main(void){
  log_init();
  softdevice_setup();
  nrf_gpio_cfg_output(LED);
  nrf_gpio_pin_set(LED);
  ppi_init();
  saadc_init();
  lfclk_config();
  rtc_config();
  ant_channel_tx_broadcast_setup();
  while(1){
    __WFE();
    __SEV();
    __WFE();
  }
}