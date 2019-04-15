


#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// in-house libraries
#include "./lib/mcu/i2c/i2c.h"
#include "./lib/mcu/spi/spi.h"
#include "./lib/mcu/uart/uart.h"
#include "./lib/hw/bluetooth/bluetooth.h"
#include "./lib/hw/eeprom/eeprom.h"
#include "./lib/hw/rtc/rtc.h"


#include <util/delay.h>


//temp defines
#include <avr/interrupt.h>


static void init_mcu(void) {
    // No clock divider.
    CLKPR =
            (1 << CLKPCE)
        |   (0 << CLKPS3)
        |   (0 << CLKPS2)
        |   (0 << CLKPS1)
        |   (0 << CLKPS0);

}



int main() {

    init_mcu();
    // 1/0x1 per the datasheet for 3.6864MHz/115200 baud
    //
    init_uart(0x1, 10);
    // Need to call eeprom_init() and epr_init() before spi_init.
    eeprom_init();
    spi_init();
    i2c_init();

    _delay_ms(500);

    char eeprom_buf[128] = "";
    eeprom_write_bytes(0x003C, "def", 3);
    eeprom_read_bytes(0x003C, eeprom_buf, 3);

    while(1) {
        _delay_ms(10);
        if (uart_have_reply()) {
            uint16_t rep_len = uart_rcv_buf_size();
            unsigned char* rep = (unsigned char*)calloc(rep_len, sizeof(unsigned char));
            uart_copy_clear(rep);
            // if (strcmp(rep, "abc") == 0) {
            if (rep[0] == 'a') {
                uart_transmit_string((unsigned char*)"got abc\n", 8);
            } else {
                uart_transmit_string((unsigned char*)"wrong\n", 6);
            }
            free(rep);
        }
    }

}

