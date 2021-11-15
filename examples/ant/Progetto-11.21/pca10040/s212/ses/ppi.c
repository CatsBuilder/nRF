#include "ppi.h"


// A function to initialize the PPI Module with all the configurations
void ppi_init(void)
{
// a variable to hold the error value
    uint32_t err_code = NRF_SUCCESS;

// Variables to hold the event address and task address
// they are needed to connect with the PPI TEP(Task End Point) & PPI EEP (Event End Point)
    uint32_t rtc_evt_addr;
    uint32_t adc_task_addr;

// Initialize the PPI module, make sure its only enabled once in your code
   err_code =  nrf_drv_ppi_init();
   APP_ERROR_CHECK(err_code);


// Allocate the channel from the available PPI channels
    err_code = nrf_drv_ppi_channel_alloc(&ppi_channel_adc);
    APP_ERROR_CHECK(err_code);


// Get the address of the respective event and tasks from the pins
    rtc_evt_addr =  nrf_drv_rtc_event_address_get(&rtc, NRF_RTC_EVENT_TICK); 
    adc_task_addr = nrf_drv_saadc_sample_task_get();
    

// connect the EEP & TEP with Peripheral Events & Tasks using their addresses and assign them to an allocated channel
    err_code = nrf_drv_ppi_channel_assign(ppi_channel_adc, rtc_evt_addr, adc_task_addr);
    APP_ERROR_CHECK(err_code);

// Enable the channel so that it can start receiving events and then route them to tasks
    err_code = nrf_drv_ppi_channel_enable(ppi_channel_adc);
    APP_ERROR_CHECK(err_code);

    
}