
#include <stdio.h>
#include <string.h>

#include <avr/io.h>
#include <util/delay.h>

#include "../../mcu/spi/spi.h"
#include "../../mcu/spi/spi_macros.h"
#include "eeprom_macros.h"

void eeprom_read_bytes(uint16_t addr, char * str, uint16_t len) {

    SPI_SS_SELECT;

    memset(str, 0, strlen(str));

    spi_send(EEPROM_CMD_READ);
    spi_send(0xFF & (addr>>8));
    spi_send(0xFF & (addr));

    for (uint16_t i = 0; i < len; ++i) {
        spi_send(0);
        str[i] = spi_receive();
    }

    SPI_SS_DESELECT;
}

/*
Only writes to one page. This _WON'T_ start writing to a subsequent page if the
length is long enough. It will rather wrap around and write to the beginning
of the current page per the datasheet's notes. Call eeprom_write_bytes to avoid
that wrapping issue and to write bytes sequentially regardless of page breaks.
*/
void eeprom_write_page(uint16_t addr, char * str, uint16_t len) {

    _delay_ms(1);

    SPI_SS_SELECT;

    spi_send(EEPROM_CMD_WREN);

    SPI_SS_DESELECT;
    SPI_SS_SELECT;

    spi_send(EEPROM_CMD_WRITE);
    spi_send(0xFF & (addr>>8));
    spi_send(0xFF & (addr));

    for (uint16_t i = 0; i < len; ++i) {
        spi_send(str[i]);
    }

    SPI_SS_DESELECT;

    _delay_ms(1);
}

void eeprom_write_bytes(uint16_t addr, char * str, uint16_t len) {

    uint16_t pages = ((addr + len) / EEPROM_PAGE_SIZE) + 1
                        - (addr / EEPROM_PAGE_SIZE);

    uint16_t curr_addr = addr;
    uint16_t bytes_left = len;

    for (uint16_t i = 0; i < pages; ++i) {

        uint8_t write_len = EEPROM_PAGE_SIZE
                            - (EEPROM_PAGE_SIZE_MASK & curr_addr);

        if (bytes_left < write_len) {
            write_len = bytes_left;
        }

        // Go back to first addr if we hit the last address.
        if (curr_addr >= EEPROM_SIZE) {
            curr_addr = EEPROM_FIRST_ADDR;
        }

        eeprom_write_page(curr_addr, str+(len - bytes_left), write_len);

        curr_addr += write_len;
        bytes_left -= write_len;
    }
}

