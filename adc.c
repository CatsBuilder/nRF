/**

 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nrf.h"
#include "nrf_drv_saadc.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "boards.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "app_util_platform.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_drv_power.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

void saadc_init(void){
    ret_code_t err_code;
    nrf_saadc_channel_config_t channel_config=NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN0);
    err_code=nrf_drv_saadc_init(NULL, saadc_callback_handler);
    //not blocking mod
    APP_ERROR_CHECK(err_code);
    err_code=nrfx_saadc_channel_init(0, & )
}



/** @} */
