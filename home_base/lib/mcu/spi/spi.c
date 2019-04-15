
#include <stdio.h>
#include <avr/io.h>
#include "spi_macros.h"

void spi_init() {

    SPI_DDR |= (1 << SPI_MOSI) | (1 << SPI_SCK);

    SPCR =
            (1 << SPE)      // Enable SPI
        |   (1 << MSTR)     // Set to master
        |   (1 << SPR1)
        |   (1 << SPR0);
}

char spi_receive() {

    while (!(SPSR & (1<<SPIF)))
        ;

    return SPDR;
}

// Call spi_send(0) if waiting for a response
void spi_send(char byte) {

    SPDR = byte;

    while (!(SPSR & (1<<SPIF)))
        ;
}

