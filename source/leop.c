/*
 * Copyright (C) 2015  University of Alberta
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
/**
 * @file leop.c
 * @author Grace Yi
 * @date Oct. 2021
 */

#include "leop.h"
#include "logger/logger.h"

static void *leop_daemon(void *pvParameters);
SAT_returnState start_leop_daemon(void);


static Deployable_t sw;

//LEOP Sequence

/**
 * @brief
 *      Check hardswitch status to ensure all deployables 
 *      have been successfully deployed
 * @return bool
 *      Returns TRUE if all deployables have been deployed
 *      Returns FALSE otherwise
 */
bool hard_switch_status() {
    TickType_t two_min_delay = pdMS_TO_TICKS(120 * 1000);
    TickType_t four_min_delay = pdMS_TO_TICKS(240 * 1000);
    TickType_t twenty_sec_delay = pdMS_TO_TICKS(20 * 1000);
    int getStatus_retries;
    int successful_deployment = 0;
    uint16_t burnwire_currents[7] = {0};

    //sw = {Port, UHF_P, UHF_Z, Payload, UHF_S, UHF_N, Starboard, DFGM};
    for (getStatus_retries = 0; getStatus_retries <= MAX_RETRIES; getStatus_retries++) {
        sw = 0;
        //Deploy DFGM
        if ((switchstatus(sw) != 1) && (getStatus_retries != MAX_RETRIES)) {
            ex2_log("Check #%d: %c not deployed\n", &getStatus_retries, sw);
            ex2_log("Activated %c\n", sw);
            activate(sw, &burnwire_currents[sw]);
            vTaskDelay(5000);
        }
        else if ((switchstatus(sw) != 1) && (getStatus_retries == MAX_RETRIES)) {
            ex2_log("Check #%d: %c not deployed, exiting the LEOP sequence.\n", &getStatus_retries, sw);
            return false;
        }
    }
    ex2_log("DFGM deployed, burnwire current = %d\n", burnwire_currents[0]);
    vTaskDelay(5000);
    //    vTaskDelay(two_min_delay);



    for (getStatus_retries = 0; getStatus_retries <= MAX_RETRIES; getStatus_retries++) {
        //Deploy UHF
        for (sw = 1; sw < 4 /* SHOULD BE A 5! */; sw++) {
            if ((switchstatus(sw) != 1) && (getStatus_retries != MAX_RETRIES)) {
                ex2_log("Check #%d: %c not deployed\n", &getStatus_retries, sw);
                ex2_log("Activated %c\n", sw);
                activate(sw, &burnwire_currents[sw]);
                vTaskDelay(5000);
            }
            else if ((switchstatus(sw) != 1) && (getStatus_retries == MAX_RETRIES)) {
                ex2_log("Check #%d: %c not deployed, exiting the LEOP sequence.\n", &getStatus_retries, sw);
                return false;
            }
            
        }
    }
    ex2_log("UHF Port deployed, burnwire current = %d\n", burnwire_currents[1]);
    ex2_log("UHF Zenith deployed, burnwire current = %d\n", burnwire_currents[2]);
    ex2_log("UHF Starboard deployed, burnwire current = %d\n", burnwire_currents[3]);
    ex2_log("UHF Nadir deployed, burnwire current = %d\n", burnwire_currents[4]);
    vTaskDelay(5000);
//    vTaskDelay(four_min_delay);


    for (getStatus_retries = 0; getStatus_retries <= MAX_RETRIES; getStatus_retries++) {
        //Deploy solar panels
        for (sw = 5; sw < 8; sw++) {
            if ((switchstatus(sw) != 1) && (getStatus_retries != MAX_RETRIES)) {
                ex2_log("Check #%d: %c not deployed\n", &getStatus_retries, sw);
                ex2_log("Activated %c\n", sw);
                activate(sw, &burnwire_currents[sw]);
                vTaskDelay(5000);
            }
            else if ((switchstatus(sw) != 1) && (getStatus_retries == MAX_RETRIES)) {
                ex2_log("Check #%d: %c not deployed, exiting the LEOP sequence.\n", &getStatus_retries, sw);
                return false;
            }
        }
        ex2_log("Port Deployable Panel deployed, burnwire current = %d\n", burnwire_currents[5]);
        ex2_log("Deployable Payload deployed, burnwire current = %d\n", burnwire_currents[6]);
        ex2_log("Starboard Deployable deployed, burnwire current = %d\n", burnwire_currents[7]);
    }
    return true;
} 

/**
 * @brief
 *      Set an eeprom flag so LEOP only gets executed once
 * @details
 *      Checks if LEOP sequence has been successfully executed 
 *      If not, execute LEOP sequence
 *      Otherwise, skip LEOP sequence
 * @return void
 */
bool leop_init() {
    bool eeprom_flag = false;
    if (eeprom_flag != true) {
//    if (eeprom_get_leop_status() != true) {
        //If leop sequence was never executed, check that all hard switches have been deployed
        if (hard_switch_status() == true) {
            //If all hard switch have been deployed, set eeprom flag to TRUE
            eeprom_set_leop_status();
            return true;
        }
        return false;
    }
//    else if (eeprom_get_leop_status() == true) {
    else if (eeprom_flag == true) {
        return true;
    }

    return false;
}


/**
 * @brief
 *    leop task
 *
 * @param pvParameters
 *    task parameters (not used)
 */
static void *leop_daemon(void *pvParameters) {
   
    //for (;;) {
        leop_init();
        vTaskDelete(0);
    //}
    return 0;
}

/**
 * @brief
 *      Start LEOP
 * @details
 *      Starts the FreeRTOS task responsible for LEOP
 * @param None
 * @return SAT_returnState
 *      success report
 */
SAT_returnState start_leop_daemon(void) {
  if (xTaskCreate((TaskFunction_t)leop_daemon,
                  "start_leop", 600, NULL, NORMAL_SERVICE_PRIO,
                  NULL) != pdPASS) {
    ex2_log("FAILED TO CREATE TASK start_leop\n");
    return SATR_ERROR;
  }
  ex2_log("leop started\n");
  return SATR_OK;
}
