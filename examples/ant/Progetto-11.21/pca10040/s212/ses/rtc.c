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
void rtc_config(void){
    uint32_t err_code=NRF_SUCCESS;
    nrf_rtc_task_trigger(rtc.p_reg, NRF_RTC_TASK_STOP);
    //Initialize RTC instance
    nrf_drv_rtc_config_t config;
    config.interrupt_priority=RTC_DEFAULT_CONFIG_IRQ_PRIORITY;
    config.tick_latency= RTC_US_TO_TICKS(NRF_MAXIMUM_LATENCY_US, RTC_DEFAULT_CONFIG_FREQUENCY);
    config.prescaler =  73; //this prescaler has to be setted in order to have the interrupt at YOUR desired frequency, for ref. see at page 242 of the nrf datasheet
    config.reliable=RTC_DEFAULT_CONFIG_RELIABLE;
    err_code = nrf_drv_rtc_init(&rtc, &config, rtc_handler);
    APP_ERROR_CHECK(err_code);



}

void rtc_start(void){
 //Enable tick event & interrupt
 nrf_drv_rtc_tick_enable(&rtc,false);
 //Power on RTC instance, timer starts here
 nrf_drv_rtc_enable(&rtc);
}