/*
 * Copyright (C) 2022  University of Alberta
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
 * @file ns_payload.h
 * @author Thomas Ganley
 * @date 2022-06-12
 */

#include "ns_payload.h"

NS_return HAL_NS_upload_artwork(char *filename) {
#if PAYLOAD_IS_STUBBED == 0
    return NS_upload_artwork(filename);
#else
    return NS_IS_STUBBED;
#endif
}

NS_return HAL_NS_capture_image() {
#if PAYLOAD_IS_STUBBED == 0
    return NS_capture_image();
#else
    return NS_IS_STUBBED;
#endif
}

NS_return HAL_NS_get_image_file(uint32_t *img_size){
#if PAYLOAD_IS_STUBBED == 0
    return NS_get_image_file(img_size);
#else
    return NS_IS_STUBBED;
#endif
}

NS_return HAL_NS_confirm_downlink(uint8_t *conf) {
#if PAYLOAD_IS_STUBBED == 0
    return NS_confirm_downlink(conf);
#else
    return NS_IS_STUBBED;
#endif
}

NS_return HAL_NS_get_heartbeat(uint8_t *heartbeat) {
#if PAYLOAD_IS_STUBBED == 0
    return NS_get_heartbeat(heartbeat);
#else
    return NS_IS_STUBBED;
#endif
}

NS_return HAL_NS_get_flag(char flag, bool *stat) {
#if PAYLOAD_IS_STUBBED == 0
    return NS_get_flag(flag, stat);
#else
    return NS_IS_STUBBED;
#endif
}

NS_return HAL_NS_get_filename(char subcode, char *filename) {
#if PAYLOAD_IS_STUBBED == 0
    return NS_get_filename(subcode, filename);
#else
    return NS_IS_STUBBED;
#endif
}

NS_return HAL_NS_trim_log_file(){
#if PAYLOAD_IS_STUBBED == 0
    return NS_trim_log_file();
#else
    return NS_IS_STUBBED;
#endif
}

NS_return HAL_NS_get_log_file(uint32_t *log_size){
#if PAYLOAD_IS_STUBBED == 0
    return NS_get_log_file(log_size);
#else
    return NS_IS_STUBBED;
#endif
}

NS_return HAL_NS_get_telemetry(ns_telemetry *tlm) {
#if PAYLOAD_IS_STUBBED == 0
    return NS_get_telemetry(tlm);
#else
    return NS_IS_STUBBED;
#endif
}

NS_return HAL_NS_get_software_version(uint8_t *version) {
#if PAYLOAD_IS_STUBBED == 0
    return NS_get_software_version(version);
#else
    return NS_IS_STUBBED;
#endif
}

NS_return HAL_NVM_start_nvm(char *filename) {
#if UHF_IS_STUBBED == 0
    return NVM_start_northern_voices(filename);
#else
    return NS_IS_STUBBED;
#endif
}

NS_return HAL_NVM_stop_nvm() {
#if UHF_IS_STUBBED == 0
    NVM_stop_northern_voices();
    return NS_OK;
#else
    return NS_IS_STUBBED;
#endif
}

NS_return HAL_NVM_get_status(bool *stat){
#if UHF_IS_STUBBED == 0
    *stat = NVM_northern_voices_status();
    return NS_OK;
#else
    return NS_IS_STUBBED;
#endif
}

