#include "rtc.h"
#include "nrf_drv_saadc.h"
const nrf_drv_rtc_t rtc=NRF_DRV_RTC_INSTANCE(2);
nrf_ppi_channel_t m_ppi_channel;

void lfclk_config(void){
  ret_code_t err_code = nrf_drv_clock_init();                        //Initialize the clock source specified in the nrf_drv_config.h file, i.e. the CLOCK_CONFIG_LF_SRC constant
  APP_ERROR_CHECK(err_code);
  nrf_drv_clock_lfclk_request(NULL);
}

void rtc_handler(nrf_drv_rtc_int_type_t int_type){
//  nrf_gpio_pin_toggle(LED);
//  ret_code_t err_code;
//  err_code=nrf_drv_saadc_sample();
// APP_ERROR_CHECK(err_code);
//routine managed by the PPI, no CPU involvement
}

void rtc_config(void){
  ret_code_t err_code;
  nrf_drv_rtc_config_t rtc_config=NRF_DRV_RTC_DEFAULT_CONFIG;
  rtc_config.prescaler=93;
  err_code= nrf_drv_rtc_init(&rtc, &rtc_config, rtc_handler);
  APP_ERROR_CHECK(err_code);
  nrf_drv_rtc_tick_enable(&rtc, true);
  nrf_drv_rtc_enable(&rtc);
  uint32_t rtc_tick_event_address=nrf_drv_rtc_event_address_get(&rtc, NRF_RTC_EVENT_TICK);
  uint32_t saadc_sample_task_addr = nrf_drv_saadc_sample_task_get();
  err_code = nrf_drv_ppi_channel_alloc(&m_ppi_channel);
  APP_ERROR_CHECK(err_code);
  err_code = nrf_drv_ppi_channel_assign(m_ppi_channel, rtc_tick_event_address, saadc_sample_task_addr);
  APP_ERROR_CHECK(err_code);
}