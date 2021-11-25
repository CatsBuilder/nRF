#include "adc.h"
#include "nrf_drv_uart.h"
#include "nrf_gpio.h"
// Handle the events once the samples are received in the buffer
static uint16_t count=0;
uint16_t val[SAMPLE_BUFFER_LEN]; // a variable to hold the data.

void saadc_callback_handler(nrf_drv_saadc_evt_t const * p_event){
    
    
    
    if(p_event -> type ==  NRF_DRV_SAADC_EVT_DONE){   // check if the sampling is done and we are ready to take these samples for processing
      ret_code_t err_code; // a variable to hold errors code
    if (count==150){
      nrf_gpio_pin_toggle(7);
      count=0;
    }
        count++;
// A function to take the samples (which are in the buffer in the form of 2's complement), and convert them to 16-bit interger values
      err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, SAMPLE_BUFFER_LEN);
      APP_ERROR_CHECK(err_code); // check for errors

// For loop is used to read and process each variable in the buffer, if the buffer size is 1, we don't need for loop
      for(int i = 0; i<SAMPLE_BUFFER_LEN; i++){
//        NRF_LOG_INFO("Sample Value Read: %d", p_event->data.done.p_buffer[i]); // read the variable and print it

// put the values in the array used for transmission
        val[i] = p_event->data.done.p_buffer[i];

        }
    
    ant_send(val); 
      

    nrf_saadc_event_clear(NRF_DRV_SAADC_EVT_DONE);
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

  err_code = nrf_drv_saadc_channel_init(NRF_SAADC_INPUT_AIN0, &channel_config);
  APP_ERROR_CHECK(err_code);
	

// allocate first buffer where the values will be stored once sampled
  err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[0], SAMPLE_BUFFER_LEN);
  APP_ERROR_CHECK(err_code);

// allocate second buffer where the values will be stored if overwritten on first before reading
  err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[1], SAMPLE_BUFFER_LEN);
  APP_ERROR_CHECK(err_code);

  nrf_saadc_int_enable(NRF_SAADC_INT_DONE);

}


/*void ant_evt_handler(ant_evt_t * p_ant_evt, void * p_context){
  if (p_ant_evt->channel == BROADCAST_CHANNEL_NUMBER && flag_cal == 0){ //durante la calibrazione ignora tutti i messaggi che arrivano dal master
    switch (p_ant_evt->event){
      case EVENT_RX:
        if (p_ant_evt->message.ANT_MESSAGE_ucMesgID == MESG_BROADCAST_DATA_ID)
        {
        if (p_ant_evt->message.ANT_MESSAGE_aucPayload [0x00] == 0x00 && p_ant_evt->message.ANT_MESSAGE_aucPayload [0x07] == 0x80 ){ //se il primo byte del payload è zero e l'ultimo è 128									
          stato=0;									  //ferma l'acquisizione												
          nrf_gpio_pin_set(LED);
          sd_ant_pending_transmit_clear (BROADCAST_CHANNEL_NUMBER, NULL); //svuota il buffer, utile per una seconda acquisizione
          NRF_LOG_INFO("Ricevuto messaggio di stop acquisizione");
        }
        if (p_ant_evt->message.ANT_MESSAGE_aucPayload [0x00] == 0x00 && p_ant_evt->message.ANT_MESSAGE_aucPayload [0x07] == 0x00){ //se il primo byte del payload è zero avvia l'acquisizione
          NRF_LOG_INFO("Inizio acquisizione");	
          sd_ant_pending_transmit_clear (BROADCAST_CHANNEL_NUMBER, NULL); //svuota il buffer, utile per una seconda acquisizione
          nrf_gpio_pin_clear(LED);
          count=0;
          i=0;
          stato = 1;
        }
        if (p_ant_evt->message.ANT_MESSAGE_aucPayload [0x00] == 0x00 && p_ant_evt->message.ANT_MESSAGE_aucPayload [0x07] == 0xFF && cal_rec == 0){//messaggio di inizio calibrazione
          cal_rec = 1;
          stato = 0;
          calibrazione();
          salva_calib_flash();
          }		
        }
      break;

      case EVENT_RX_SEARCH_TIMEOUT:	   //in caso di timeout riapre il canale
        err_code = sd_ant_channel_open(BROADCAST_CHANNEL_NUMBER);
        APP_ERROR_CHECK(err_code); 
      break;							

      default:
      break;
    }
  }
}
NRF_SDH_ANT_OBSERVER(m_ant_observer, APP_ANT_OBSERVER_PRIO, ant_evt_handler, NULL);*/




  
