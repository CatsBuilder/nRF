#include "ant.h"

void ant_send(uint16_t *message){
  uint8_t message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE];
  memset(message_payload, 0, ANT_STANDARD_DATA_PAYLOAD_SIZE);
  // Assign a new value to the broadcast data.
  message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE - 8] = DEVICENUMBER;
  message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE - 7] = counter; 
  message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE - 6] = (uint8_t)(message[0]>>8);
  message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE - 5] = (uint8_t)(message[0]&&0xFF);
  message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE - 4] = (uint8_t)(message[1]>>8);
  message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE - 3] = (uint8_t)(message[1]&&0xFF);
  message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE - 2] = (uint8_t)(message[2]>>8);
  message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE - 1] = (uint8_t)(message[2]&&0xFF);

    // Broadcast the data.
  ret_code_t err_code = sd_ant_broadcast_message_tx(BROADCAST_CHANNEL_NUMBER,
                                                      ANT_STANDARD_DATA_PAYLOAD_SIZE,
                                                      message_payload);
    APP_ERROR_CHECK(err_code);
	//NRF_LOG_INFO("Messaggio numero %d, ret code ant broadcast: %d", counter, err_code);
        counter++;
}


void ant_channel_tx_broadcast_setup(void){

ant_channel_config_t broadcast_channel_config =
    {
        .channel_number    = BROADCAST_CHANNEL_NUMBER,
        .channel_type      = CHANNEL_TYPE_MASTER,
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

    // Open channel.
    err_code = sd_ant_channel_open(BROADCAST_CHANNEL_NUMBER);
    APP_ERROR_CHECK(err_code);
}


void ant_evt_handler(ant_evt_t * p_ant_evt, void * p_context){
                  ret_code_t err_code;

    if (p_ant_evt->channel == BROADCAST_CHANNEL_NUMBER )  //durante la calibrazione ignora tutti i messaggi che arrivano dal master
    {
      switch (p_ant_evt->event)
      {
        case EVENT_RX:
            if (p_ant_evt->message.ANT_MESSAGE_ucMesgID == MESG_BROADCAST_DATA_ID){
              if (p_ant_evt->message.ANT_MESSAGE_aucPayload [0x00] == 0x00 && p_ant_evt->message.ANT_MESSAGE_aucPayload [0x07] == 0x80 ){//se il primo byte del payload è zero e l'ultimo è 128 									
                //stato=0;									  //ferma l'acquisizione												
                nrf_gpio_pin_set(7);
                sd_ant_pending_transmit_clear (BROADCAST_CHANNEL_NUMBER, NULL); //svuota il buffer, utile per una seconda acquisizione
                NRF_LOG_INFO("Ricevuto messaggio di stop acquisizione");
                        
              }
               if (p_ant_evt->message.ANT_MESSAGE_aucPayload [0x00] == 0x00 && p_ant_evt->message.ANT_MESSAGE_aucPayload [0x07] == 0x00)   //se il primo byte del payload è zero avvia l'acquisizione
                {
                       NRF_LOG_INFO("Inizio acquisizione");	
                       sd_ant_pending_transmit_clear (BROADCAST_CHANNEL_NUMBER, NULL); //svuota il buffer, utile per una seconda acquisizione
                       nrf_gpio_pin_clear(7);
                       
                      //count=0;
                       //i=0;
                       //stato = 1;
          }
              if (p_ant_evt->message.ANT_MESSAGE_aucPayload [0x00] == 0x00 && p_ant_evt->message.ANT_MESSAGE_aucPayload [0x07] == 0xFF )   //messaggio di inizio calibrazione
                {
                       //cal_rec = 1;
                       //stato = 0;
                       //calibrazione();
                       //salva_calib_flash();
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
NRF_SDH_ANT_OBSERVER(m_ant_observer, APP_ANT_OBSERVER_PRIO, ant_evt_handler, NULL);

