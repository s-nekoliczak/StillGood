
extern unsigned char* __uart_rcv_buf;
extern uint16_t __uart_rcv_buf_i;

void init_uart(uint16_t baud, uint16_t wait_ms);

// Taken from atmega328p datasheet example
void __uart_transmit(unsigned char data);

// Taken from atmega328p datasheet example
unsigned char __uart_receive(void);

uint16_t uart_rcv_buf_size();

void uart_transmit_string(unsigned char* str, uint16_t len);

void __uart_clear_rcv_buf();

void uart_clear();

uint16_t uart_copy_clear(unsigned char* dest);

uint8_t uart_have_reply();

