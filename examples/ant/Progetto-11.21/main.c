#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_drv_rtc.h"
#include "nrf_drv_saadc.h"
#include "nrf_drv_clock.h"
#include "boards.h"
#include "bsp.h"
#include "hardfault.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_delay.h"
#include "app_error.h"
#include <stdint.h>
#include <stdbool.h>
#include "dma.h"
#include "ant.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ant.h"
#include "nrf_sdh_soc.h"
#include "rtc.h"
#include "adc.h"
#include "ppi.h"
#include "dma.h"    
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_uart.h"
#include "app_uart.h"
#include "app_fifo.h"  
#include "nrf_drv_uart.h"                                 /**< Get Compare event COMPARE_TIME seconds after the counter starts from 0. */
#define LED 7
#define UART_HWFC APP_UART_FLOW_CONTROL_DISABLED
#define MAX_TEST_DATA_BYTES     (15U)                /**< max number of test bytes to be used for tx and rx. */
#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 256                         /**< UART RX buffer size. */

void log_init(void){
  ret_code_t err_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(err_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
 }
void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}



void softdevice_setup(void){
  ret_code_t err_code;
  err_code = nrf_sdh_enable_request();
  APP_ERROR_CHECK(err_code);

  ASSERT(nrf_sdh_is_enabled());

  err_code = nrf_sdh_ant_enable();
  APP_ERROR_CHECK(err_code);
}



void main(void){
  log_init();
  nrf_gpio_cfg_output(LED);
  nrf_gpio_pin_set(LED);
  ppi_init();
  softdevice_setup();
  nrf_delay_ms(5);
  ant_channel_tx_broadcast_setup();
  rtc_config(); 

  lfclk_config();
  rtc_start();
  saadc_init();
  


  while(1){
    __WFE();
    __SEV();
    __WFE();
  }
}