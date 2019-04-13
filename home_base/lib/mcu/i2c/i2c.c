

#include <avr/io.h>
#include <avr/interrupt.h>


/*
I2C notes
use 4.7k ohm resistors to pullup SCL/SDA

RTC notes
connect VBAT to GND if not using
*/

#define I2C_TRANSMIT_START      TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN)
#define I2C_TRANSMIT_STOP       TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN)
#define I2C_SEND_DATA           TWCR = (1<<TWINT) | (1<<TWEN)
#define I2C_RCV_DATA_ACK        TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN)
#define I2C_RCV_DATA_NACK       TWCR = (1<<TWINT) | (0<<TWEA) | (1<<TWEN)

void i2c_init() {
    sei();

    TWSR |=
            (1 << TWPS1)    // 16x prescaler
        |   (0 << TWPS0);

    TWCR |= (1 << TWIE);    // Enable interrupt
}

void i2c_wait_for_hdwr_rspn() {
    while (!(TWCR & (1<<TWINT)))
        ;
}

uint8_t i2c_status() {
    return (TWSR & 0xF8);
}

// Write is 0, read is 1.
void i2c_set_addr(uint8_t addr, uint8_t w_r) {
    // most-significant 7 bits are addr
    // lsb is read/write; w=0; r=1
    TWDR = (addr << 1) | (0x01 & w_r);
}

void i2c_set_dbyte(uint8_t byte) {
    TWDR = byte;
}

uint8_t i2c_read_data() {
    return (uint8_t)TWDR;
}


uint8_t i2c_start_trans() {
    I2C_TRANSMIT_START;
    i2c_wait_for_hdwr_rspn();
    return i2c_status();
}

uint8_t i2c_send_addr(uint8_t addr, uint8_t w_r) {
    i2c_set_addr(addr, w_r);
    I2C_SEND_DATA;
    i2c_wait_for_hdwr_rspn();
    return i2c_status();
}

uint8_t i2c_send_dbyte(uint8_t byte) {
    i2c_set_dbyte(byte);
    I2C_SEND_DATA;
    i2c_wait_for_hdwr_rspn();
    return i2c_status();
}

uint8_t i2c_rcv_dbyte() {
    I2C_RCV_DATA_ACK;
    i2c_wait_for_hdwr_rspn();
    return i2c_status();
}

void i2c_stop_send() {
    I2C_TRANSMIT_STOP;
}

uint8_t i2c_stop_rcv() {
    I2C_RCV_DATA_NACK;
    i2c_wait_for_hdwr_rspn();
    I2C_TRANSMIT_STOP;
    return i2c_status();
}

