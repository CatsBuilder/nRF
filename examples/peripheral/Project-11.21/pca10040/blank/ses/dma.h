#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "nrf.h"
#include "nrf_drv_clock.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_power.h"
#include "nrf_serial.h"
#include "app_timer.h"


#include "app_error.h"
#include "app_util.h"
#include "boards.h"


#define DATASIZE 14
// A sleep handler which will be called when there is no communication taking place

void sleep_handler(void);