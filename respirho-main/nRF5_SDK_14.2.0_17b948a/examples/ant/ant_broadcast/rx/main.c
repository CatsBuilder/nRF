/**
 * This software is subject to the ANT+ Shared Source License
 * www.thisisant.com/swlicenses
 * Copyright (c) Dynastream Innovations, Inc. 2014
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 * 1) Redistributions of source code must retain the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer.
 * 
 * 2) Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 * 
 * 3) Neither the name of Dynastream nor the names of its
 *    contributors may be used to endorse or promote products
 *    derived from this software without specific prior
 *    written permission.
 * 
 * The following actions are prohibited:
 * 1) Redistribution of source code containing the ANT+ Network
 *    Key. The ANT+ Network Key is available to ANT+ Adopters.
 *    Please refer to http://thisisant.com to become an ANT+
 *    Adopter and access the key.
 * 
 * 2) Reverse engineering, decompilation, and/or disassembly of
 *    software provided in binary form under this license.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE HEREBY
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; DAMAGE TO ANY DEVICE, LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. SOME STATES DO NOT ALLOW
 * THE EXCLUSION OF INCIDENTAL OR CONSEQUENTIAL DAMAGES, SO THE
 * ABOVE LIMITATIONS MAY NOT APPLY TO YOU.
 * 
 */
/**@file
 * @defgroup ant_broadcast_rx_example ANT Broadcast RX Example
 * @{
 * @ingroup nrf_ant_broadcast
 *
 * @brief Example of basic ANT Broadcast RX.
 *
 * Before compiling this example for NRF52, complete the following steps:
 * - Download the S212 SoftDevice from <a href="https://www.thisisant.com/developer/components/nrf52832" target="_blank">thisisant.com</a>.
 * - Extract the downloaded zip file and copy the S212 SoftDevice headers to <tt>\<InstallFolder\>/components/softdevice/s212/headers</tt>.
 * If you are using Keil packs, copy the files into a @c headers folder in your example folder.
 * - Make sure that @ref ANT_LICENSE_KEY in @c nrf_sdm.h is uncommented.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "nrf.h"
#include "bsp.h"
#include "hardfault.h"
#include "app_error.h"
#include "app_timer.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ant.h"
#include "nrf_pwr_mgmt.h"
#include "ant_interface.h"
#include "ant_parameters.h"
#include "ant_channel_config.h"
#include "asc_slave.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_delay.h"

#define APP_ANT_OBSERVER_PRIO   1    /**< Application's ANT observer priority. You shouldn't need to modify this value. */

int count=0;
int i=0;
int stato=0;
int var=0;
int b=0;
/**@brief Function for handling a ANT stack event.
 *
 * @param[in] p_ant_evt  ANT stack event.
 * @param[in] p_context  Context.
 */
 
 /*void ant_message_send(void)
{
    uint8_t         message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE];
    static uint8_t  counter = 1u;

    memset(message_payload, 0, ANT_STANDARD_DATA_PAYLOAD_SIZE);
    // Assign a new value to the broadcast data.
	  message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE - 8] = 2;
	  message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE - 7] = 0;
    message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE - 1] = counter;

    // Broadcast the data.
    ret_code_t err_code = sd_ant_broadcast_message_tx(BROADCAST_CHANNEL_NUMBER,
                                                      ANT_STANDARD_DATA_PAYLOAD_SIZE,
                                                      message_payload);
    APP_ERROR_CHECK(err_code);

    counter++;
}*/

void ant_message_set(void)
{
    uint8_t         message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE];

    memset(message_payload, 0, ANT_STANDARD_DATA_PAYLOAD_SIZE);
    // Assign a new value to the broadcast data.
	  message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE - 8] = 1;
	  message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE - 7] = 0;
    message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE - 6] = 9;
    ret_code_t err_code = sd_ant_broadcast_message_tx(BROADCAST_CHANNEL_NUMBER,
                                                      ANT_STANDARD_DATA_PAYLOAD_SIZE,
                                                      message_payload);
    APP_ERROR_CHECK(err_code);
}

void ant_message_send(int counter, int counter2)
{
    uint8_t         message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE];

    memset(message_payload, 0, ANT_STANDARD_DATA_PAYLOAD_SIZE);
    // Assign a new value to the broadcast data.
	  message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE - 8] = 3;
	  message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE - 7] = 0;
	  message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE - 3] = counter2;
    message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE - 1] = counter;

    // Broadcast the data.
    ret_code_t err_code = sd_ant_broadcast_message_tx(BROADCAST_CHANNEL_NUMBER,
                                                      ANT_STANDARD_DATA_PAYLOAD_SIZE,
                                                      message_payload);
    APP_ERROR_CHECK(err_code);


}

void ant_evt_handler(ant_evt_t * p_ant_evt, void * p_context)
{
    ret_code_t err_code;
	  static uint8_t                  m_channel_number=0; 
	  
	  uint8_t         message_addr[ANT_STANDARD_DATA_PAYLOAD_SIZE];
	  memset(message_addr, 2, ANT_STANDARD_DATA_PAYLOAD_SIZE);

    if (p_ant_evt->channel == BROADCAST_CHANNEL_NUMBER)
    {
        switch (p_ant_evt->event)
        {
            case EVENT_RX:
                /*if (p_ant_evt->message.ANT_MESSAGE_ucMesgID == MESG_BROADCAST_DATA_ID
                 || p_ant_evt->message.ANT_MESSAGE_ucMesgID == MESG_ACKNOWLEDGED_DATA_ID
                 || p_ant_evt->message.ANT_MESSAGE_ucMesgID == MESG_BURST_DATA_ID)
                {
						*/
						{
                if (p_ant_evt->message.ANT_MESSAGE_ucMesgID == MESG_BROADCAST_DATA_ID)
             
                {
                    if (p_ant_evt->message.ANT_MESSAGE_aucPayload [0x00] == 0x00)
									 {
										 count=0;
										 i=0;
										 if (stato==0)
										 {
										   stato=1;
										 }
								    }
									 if (stato==1)
									 {
									   
										   var=1;
										 
									 }
                }
                break;

            default:
                break;
							}
						}
        }
    }


NRF_SDH_ANT_OBSERVER(m_ant_observer, APP_ANT_OBSERVER_PRIO, ant_evt_handler, NULL);

/**@brief Function for the Timer and BSP initialization.
 */
static void utils_setup(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
	
	  NRF_LOG_DEFAULT_BACKENDS_INIT();

    err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    err_code = bsp_init(BSP_INIT_LED,
                        NULL);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for ANT stack initialization.
 */
static void softdevice_setup(void)
{
    ret_code_t err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    ASSERT(nrf_sdh_is_enabled());

    err_code = nrf_sdh_ant_enable();
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for setting up the ANT module to be ready for RX broadcast.
 */
static void ant_channel_rx_broadcast_setup(void)
{
    ant_channel_config_t broadcast_channel_config =
    {
        .channel_number    = BROADCAST_CHANNEL_NUMBER,
        .channel_type      = CHANNEL_TYPE_SHARED_SLAVE ,
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

/**@brief Function for application main entry. Does not return.
 */
int main(void)
{

	//ret_code_t err_code;
	  static uint8_t                  m_channel_number=0; 
	  uint8_t         message_addr[ANT_STANDARD_DATA_PAYLOAD_SIZE];
	  memset(message_addr, 3, ANT_STANDARD_DATA_PAYLOAD_SIZE);

	  utils_setup();
    softdevice_setup();
    ant_channel_rx_broadcast_setup();
	  ret_code_t err_code = sd_ant_broadcast_message_tx(BROADCAST_CHANNEL_NUMBER,
                                                      ANT_STANDARD_DATA_PAYLOAD_SIZE,
                                                      message_addr);	
	  NRF_LOG_INFO("Lor? tu mi fai divent? matto");
    // Main loop.
    for (;;)
    {
        /*if (stato==0)
				{
					//for (i=0; i<1; i++)
					//{
					ret_code_t err_code = sd_ant_broadcast_message_tx(BROADCAST_CHANNEL_NUMBER,
                                                      ANT_STANDARD_DATA_PAYLOAD_SIZE,
                                                      message_addr);	
					//ant_message_send();
				    
					stato=1;
				}*/
			  /*i++;
			  if (i==5)
				{		
				  count++;
					i=0;
				}*/
			  //if (stato==1)
				//{
			    //if (i==0)
					//{
						//nrf_delay_ms(1000);
						//i=1;
					//}
					//if (i==1)
					//{
					//count++;
					//ant_message_send(count);
			    //nrf_delay_ms(250);
					//ant_message_send(count);
									 // err_code = bsp_indication_set(BSP_INDICATE_RCV_OK);
                   // APP_ERROR_CHECK(err_code);
					//i++;
					//if (i==5)
				  //{		
				    //count++;
					  //i=0;
						//i++;
						/*if (stato==1)
						{
							i++;
							if (i==5)
							{
								b++;
								i=0;
							}*/
							if (stato==1)
						  {
							  //i++;
								   if (i==2)
									 {
										 i=0;
									 }
									//count++;
										count++;
									  i++;
								    nrf_delay_ms(50);
								    //ant_message_send(count,b);
								    //nrf_delay_ms(500);
								  if (var==1 & i==2)
									{
									//count++;
								  //nrf_delay_ms(250);
							    ant_message_send(count,b);
								  //nrf_delay_ms(250);
									}
							    //var=0;
								  //i=0;
										//b=0;
									//}
								
						  
							/*if (var==0)
							{
								i++;
								nrf_delay_ms(50);
							}*/
						}
						
				 }
				//}
			  NRF_LOG_FLUSH();
        nrf_pwr_mgmt_run();
    }
   
 

/**
 *@}
 **/
