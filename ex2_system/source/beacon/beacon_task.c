/*
 * Copyright (C) 2020  University of Alberta
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
 * @file beacon_task.c
 * @author Andrew R. Rooney, Grace Yi
 * @date Mar. 6, 2021
 */

#include "beacon_task.h"

#define SCW_BCN_FLAG 5
#define SCW_BCN_ON 1
#define BEACON_PACKET_LENGTH 97
#define BEACON_FREQ_DELAY pdMS_TO_TICKS(30 * 1000) // pdMS_TO_TICKS(1000) converts 1000 ms to number of ticks
#define BEACON_UPDATE_DELAY pdMS_TO_TICKS(3 * 1000)

static void *beacon_daemon(All_systems_housekeeping *all_hk_data);
SAT_returnState start_beacon_daemon();

/**
 * Construct and send out the system beacon at the required frequency.
 *
 * @param pvParameters
 *    task parameters (not used)
 */
static void *beacon_daemon(All_systems_housekeeping *all_hk_data) {

    for (;;) {
        int8_t uhf_status = -1;
        /* Constructing the system beacon content */
        // Refer to table 3 of MOP
        UHF_configStruct beacon_msg;

        // Testing purposes only, passing 1s to the UHF
        beacon_packet_1_t beacon_packet_one = {// define each element as 1 for testing purposes
                                               1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                               1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

        update_beacon(&all_hk_data, &beacon_packet_one, &beacon_packet_two);

        // Send first beacon packet
        memcpy(&(beacon_msg.message), &beacon_packet_one, sizeof(beacon_packet_1_t));
        beacon_msg.len = sizeof(beacon_packet_1_t);
        // memcpy(&(beacon_msg.message), &beacon_packet_2, sizeof(beacon_packet_2));
        //  TODO: call the appropriate HAL functions to get the most updated or
        //  cached information of the components + state machine, RTC, etc.
        //  Then uncomment the next line:
        uhf_status = HAL_UHF_setBeaconMsg(beacon_msg);
        /* Sending the beacon */
        // The beacon transmission period is configurable through comms service
        // by the operator or here through HAL_UHF_getBeaconT().
        uint8_t scw[SCW_LEN];
        scw[5] = 1;
        HAL_UHF_setSCW(scw);

        // wait 3s before updating the second beacon packet to allow time for sending the first beacon
        vTaskDelay(BEACON_UPDATE_DELAY);

        // Send second beacon packet
        memcpy(&(beacon_msg.message), &beacon_packet_two, sizeof(beacon_packet_2_t));
        beacon_msg.len = sizeof(beacon_packet_2_t);
        // memcpy(&(beacon_msg.message), &beacon_packet_2, sizeof(beacon_packet_2));
        //  TODO: call the appropriate HAL functions to get the most updated or
        //  cached information of the components + state machine, RTC, etc.
        //  Then uncomment the next line:
        uhf_status = HAL_UHF_setBeaconMsg(beacon_msg);
        /* Sending the beacon */
        // The beacon transmission period is configurable through comms service
        // by the operator or here through HAL_UHF_getBeaconT().
        HAL_UHF_setSCW(scw);

#if UHF_IS_STUBBED == 0
        uhf_status = HAL_UHF_getSCW(scw);

        if (uhf_status == U_GOOD_CONFIG) {
            scw[SCW_BCN_FLAG] = SCW_BCN_ON;
            uhf_status = HAL_UHF_setSCW(scw);
        }
#endif
#if EPS_IS_STUBBED == 0
        if (uhf_status != U_GOOD_CONFIG) {

            if (eps_get_pwr_chnl(UHF_5V0_PWR_CHNL) == 1 && gioGetBit(UHF_GIO_PORT, UHF_GIO_PIN) == 1) {
                printf("Beacon failed");
            } else
                printf("UHF is off.");
        }
#endif

        vTaskDelay(BEACON_FREQ_DELAY);
    }
}

/**
 * Start the beacon daemon
 *
 * @returns status
 *   error report of task creation
 */
SAT_returnState start_beacon_daemon(void) {
    if (xTaskCreate((TaskFunction_t)beacon_daemon, "beacon_daemon", 2048, NULL, BEACON_TASK_PRIO, NULL) !=
        pdPASS) {
        ex2_log("FAILED TO CREATE TASK coordinate_management_daemon\n");
        return SATR_ERROR;
    }
    ex2_log("Coordinate management started\n");
    return SATR_OK;
}
