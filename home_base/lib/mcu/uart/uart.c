
#include <stdio.h>
#include <string.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/setbaud.h>
#include <util/delay.h>

#define G_RCV_SIZE          256
#define UART_MCU_FREQ       3686400

#define UART_ENABLE_TIMER       TIMSK1 |= (1  << OCIE1A)
#define UART_DISABLE_TIMER      TIMSK1 &= ~(1  << OCIE1A)

unsigned char __uart_rcv_buf[G_RCV_SIZE];
uint16_t __uart_rcv_buf_i;
int __uart_icr1_val;
uint8_t __uart_is_first_char;
uint8_t __uart_have_reply;

void init_uart(uint16_t baud, uint16_t wait_ms) {

    __uart_is_first_char = 1;
    __uart_have_reply = 0;

    __uart_rcv_buf_i = 0;

    UBRR0H = (baud>>8);
    UBRR0L = baud;

    UCSR0A = (0<<U2X0);         // Disable double speed

    sei();

    // Enable The receiver and transmitter
    UCSR0B =
            (1 << RXCIE0)       // enable receiver interrupt
        |   (1 << RXEN0)        // enable receiver
        |   (1 << TXEN0);       // enable transmitter

    UCSR0C =
            (0 << UMSEL01)      // asynchronous
        |   (0 << UMSEL00)      // asynchronous
        |   (0 << UPM01)        // no parity
        |   (0 << UPM00)        // no parity
        |   (0 << USBS0)        // 1 stop bit
        |   (1 << UCSZ01)       // 8-bit message size
        |   (1 << UCSZ00)       // 8-bit message size
        |   (0 << UCPOL0);      // Disable clock polarity for async mode

    // Enable timer for response waiting.
    TCCR1B =
            (1 << WGM13)        // CTC mode
        |   (1 << WGM12)
        |   (1 << CS12);        // 256x pre-scaler

    float ms = (UART_MCU_FREQ / 256) * (wait_ms / 1000);
    __uart_icr1_val = (int)ms;
}

ISR(USART_RX_vect) {
    __uart_rcv_buf[__uart_rcv_buf_i++] = UDR0;
    if (__uart_is_first_char) {
        ICR1 = __uart_icr1_val;
        __uart_is_first_char = 0;
        UART_ENABLE_TIMER;
    }
}

ISR(TIMER1_COMPA_vect) {
    UART_DISABLE_TIMER;
    __uart_have_reply = 1;
}

// Taken from atmega328p datasheet example
void __uart_transmit(unsigned char data)
{
    while (!( UCSR0A & (1<<UDRE0)))
        ;
    UDR0 = data;
}

// Taken from atmega328p datasheet example
unsigned char __uart_receive(void)
{
    while (!(UCSR0A & (1<<RXC0)))
        ;
    return UDR0;
}

uint16_t uart_rcv_buf_size() {
    return __uart_rcv_buf_i;
}

void uart_transmit_string(unsigned char* str, uint16_t len) {
    for (uint16_t i = 0; i < len; ++i) {
        __uart_transmit(str[i]);
    }
}

void __uart_clear_rcv_buf() {
    memset(__uart_rcv_buf, 0, G_RCV_SIZE);
    __uart_rcv_buf_i = 0;
}

void uart_clear() {
    __uart_is_first_char = 1;
    __uart_have_reply = 0;
    __uart_clear_rcv_buf();
}

// Copies contents to dest array and clears __uart_rcv_buf for future use.
// Always call this after calling uart_transmit_string to keep __uart_rcv_buf buffer
// clear.
uint16_t uart_copy_clear(unsigned char* dest) {
    memcpy(dest, __uart_rcv_buf, __uart_rcv_buf_i);
    uint16_t len = __uart_rcv_buf_i;
    uart_clear();
    return len;
}

uint8_t uart_have_reply() {
    return __uart_have_reply;
}






