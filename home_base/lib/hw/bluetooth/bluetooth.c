
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../mcu/uart/uart.h"
#include <util/delay.h>

// Action commands
#define BT_CMD_ACT_START_CMD            "$$$"
#define BT_CMD_ACT_EXIT_CMD             "---"
#define BT_CMD_ACT_DISCONNECT           "K,"
#define BT_CMD_ACT_REBOOT               "R,1"

// Action command replies
#define BT_CMD_ACT_START_CMD_REP_OK     "CMD"
#define BT_CMD_ACT_END_CMD_REP_OK       "END"

// Set commands
#define BT_CMD_SET_FACTORY_DEFAULT      "SF,1"
#define BT_CMD_SET_MODE                 "SM,"
#define BT_CMD_SET_DEVICE_NAME          "SN,"
#define BT_CMD_SET_PIN                  "SP,"
#define BT_CMD_SET_SERIAL_NAME          "S-,"
#define BT_CMD_SET_LOW_POWER            "S|,"

// Set command replies
#define BT_CMD_SET_REP_OK               "AOK"

// Misc
#define BT_REP_LEN                      3
#define BT_MAX_MODE                     6
#define BT_LOW_POWER_ARG_LEN            5


void __bt_transmit(char* str) {
    uart_transmit_string((unsigned char*) str, strlen(str));
}

uint8_t __bt_cmd_transmit(char* cmd, char* arg, char* rep_ok) {
    
    // START cmd doesn't need \r\n
    if (strcmp(cmd, BT_CMD_ACT_START_CMD) == 0) {
        __bt_transmit(cmd);
    } else {
        // +2 for \r\0
        uint8_t len = strlen(cmd) + strlen(arg) + 2;
        char* cmd_str = (char*)calloc(len, sizeof(char));

        // \0 is implied in snprintf
        if (arg == 0) {
            snprintf(cmd_str, len, "%s\r", cmd);
        } else {
            snprintf(cmd_str, len, "%s%s\r", cmd, arg);
        }

        __bt_transmit(cmd_str);

        free(cmd_str);
    }

    while (!uart_have_reply()) {
        _delay_ms(10);
    }

    uint8_t retval = 0;
    uint16_t rep_len = uart_rcv_buf_size();
    char* rep = (char*)calloc(rep_len, sizeof(char));
    uart_copy_clear((unsigned char*)rep);

    if (rep_len >= BT_REP_LEN && strncmp(rep, rep_ok, BT_REP_LEN) == 0) {
        retval = 1;
    }

    free(rep);

    return retval;
}

/*
Action commands
*/
uint8_t bt_cmd_act_start_cmd() {
    return __bt_cmd_transmit(BT_CMD_ACT_START_CMD, 0, BT_CMD_ACT_START_CMD_REP_OK);
}

uint8_t bt_cmd_act_exit_cmd() {
    return __bt_cmd_transmit(BT_CMD_ACT_EXIT_CMD, 0, BT_CMD_ACT_END_CMD_REP_OK);
}

uint8_t bt_cmd_act_disconnect() {
    return __bt_cmd_transmit(BT_CMD_ACT_DISCONNECT, 0, BT_CMD_SET_REP_OK);
}

uint8_t bt_cmd_act_reboot() {
    return __bt_cmd_transmit(BT_CMD_ACT_REBOOT, 0, BT_CMD_SET_REP_OK);
}

/*
Set commands
*/
uint8_t bt_cmd_factory_default() {
    return __bt_cmd_transmit(BT_CMD_SET_FACTORY_DEFAULT, 0, BT_CMD_SET_REP_OK);
}

uint8_t bt_cmd_set_mode(uint8_t mode) {

    if (mode > BT_MAX_MODE) {
        mode = BT_MAX_MODE;
    }
    
    char mode_str[2] = { mode + '0' };
    return __bt_cmd_transmit(BT_CMD_SET_MODE, mode_str, BT_CMD_SET_REP_OK);
}

uint8_t bt_cmd_set_device_name(char* name) {
    return __bt_cmd_transmit(BT_CMD_SET_DEVICE_NAME, name, BT_CMD_SET_REP_OK);
}

uint8_t bt_cmd_set_pin(char* pin) {
    return __bt_cmd_transmit(BT_CMD_SET_PIN, pin, BT_CMD_SET_REP_OK);
}

uint8_t bt_cmd_set_serial_name(char* name) {
    return __bt_cmd_transmit(BT_CMD_SET_SERIAL_NAME, name, BT_CMD_SET_REP_OK);
}


uint8_t bt_cmd_power_cycle() {
    return __bt_cmd_transmit(BT_CMD_ACT_REBOOT, 0, BT_CMD_SET_REP_OK);
}

uint8_t bt_cmd_set_low_power_connect(uint8_t sec_off, uint8_t sec_on) {
    char mode_hex[BT_LOW_POWER_ARG_LEN];
    snprintf(mode_hex, BT_LOW_POWER_ARG_LEN, "%02x%02x", sec_off, sec_on);
    return __bt_cmd_transmit(BT_CMD_SET_LOW_POWER, mode_hex, BT_CMD_SET_REP_OK);
}


