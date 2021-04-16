#include "rtc.h"

const nrf_drv_rtc_t rtc=NRF_DRV_RTC_INSTANCE(2);

void lfclk_config(void){
    ret_code_t err_code = nrf_drv_clock_init();                        //Initialize the clock source specified in the nrf_drv_config.h file, i.e. the CLOCK_CONFIG_LF_SRC constant
    APP_ERROR_CHECK(err_code);
    nrf_drv_clock_lfclk_request(NULL);
}

void rtc_handler(nrf_drv_rtc_int_type_t int_type){
  nrf_gpio_pin_toggle(LED);
}

void rtc_config(void){
  uint32_t err_code;
  nrf_drv_rtc_config_t rtc_config=NRF_DRV_RTC_DEFAULT_CONFIG;
  rtc_config.prescaler=93;
  err_code= nrf_drv_rtc_init(&rtc, &rtc_config, rtc_handler);
  APP_ERROR_CHECK(err_code);
  nrf_drv_rtc_tick_enable(&rtc, true);
  nrf_drv_rtc_enable(&rtc);


}