#include "adc.h"


// Handle the events once the samples are received in the buffer
void saadc_callback_handler(nrf_drv_saadc_evt_t const * p_event){
    float val; // a float variable to hold the arithmetic calculations data.

    if(p_event -> type ==  NRF_DRV_SAADC_EVT_DONE) // check if the sampling is done and we are ready to take these samples for processing
    {
      ret_code_t err_code; // a variable to hold errors code

// A function to take the samples (which are in the buffer in the form of 2's complement), and convert them to 16-bit interger values
      err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, SAMPLE_BUFFER_LEN);
      APP_ERROR_CHECK(err_code); // check for errors



// simple log message to show some event occured
      NRF_LOG_INFO("ADC Event Occurred!!");

// For loop is used to read and process each variable in the buffer, if the buffer size is 1, we don't need for loop
      for(int i = 0; i<SAMPLE_BUFFER_LEN; i++)
      {
        NRF_LOG_INFO("Sample Value Read: %d", p_event->data.done.p_buffer[i]); // read the variable and print it

// Perform some calculations to convert this value back to the voltage
        val = p_event->data.done.p_buffer[i] * 3.6 / 4096;
		
		// use NRF log special marker to output the floating point values.
        NRF_LOG_INFO("Voltage Read: " NRF_LOG_FLOAT_MARKER "\r\n", NRF_LOG_FLOAT(val));

      
        }
       


    }
}


// Create a function to initialize the saadc 
void saadc_init(void){
  ret_code_t err_code; // variable to store the error code

// Create a struct to hold the default configurations which will be used to initialize the adc module.
// make sure to use the right pins
  nrf_saadc_channel_config_t channel_config =  NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN0);

// Initialize the adc module Null is for configurations, they can be configured via CMSIS Configuration wizard so we don't need to pass anything here
  err_code = nrf_drv_saadc_init(NULL, saadc_callback_handler);
  APP_ERROR_CHECK(err_code);


// allocate first buffer where the values will be stored once sampled
  err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[0], SAMPLE_BUFFER_LEN);
  APP_ERROR_CHECK(err_code);

// allocate second buffer where the values will be stored if overwritten on first before reading
  err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[1], SAMPLE_BUFFER_LEN);
  APP_ERROR_CHECK(err_code);

  nrf_saadc_int_enable(NRF_SAADC_INT_END);

}





  
