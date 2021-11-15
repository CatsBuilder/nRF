#include "rtc.h"


/**  Function for handling the RTC0 interrupts.
 * Triggered on TICK. */
 
void rtc_handler(nrf_drv_rtc_int_type_t int_type){
    if (int_type == NRF_DRV_RTC_INT_TICK)
    {
        //null, used by the init function but unused by the algorithm
    }
}



/** Function starting the internal LFCLK XTAL oscillator.*/
void lfclk_config(void){
    ret_code_t err_code;
    err_code= nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_clock_lfclk_request(NULL);
}

/** initialization and configuration of RTC driver instance. */
void rtc_config(void)
{
    uint32_t err_code;

    //Initialize RTC instance
    nrf_drv_rtc_config_t config;
    config.interrupt_priority=4;
    config.tick_latency=RTC_US_TO_TICKS(1000,32768);
    config.prescaler = 108; //this prescaler has to be setted in order to have the interrupt at YOUR desired frequency, for ref. see at page 242 of the nrf datasheet
    config.reliable=false;
    err_code = nrf_drv_rtc_init(&rtc, &config, rtc_handler);
    APP_ERROR_CHECK(err_code);

    //Enable tick event & interrupt
    nrf_drv_rtc_tick_enable(&rtc,false);

    //Power on RTC instance, timer starts here
    nrf_drv_rtc_enable(&rtc);
}
