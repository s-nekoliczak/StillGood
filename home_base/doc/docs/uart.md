# UART

This article describes the functions available in the UART library in uart.c, how to use them, and examples.

This library is for use with the ATMega328P MCU.

---
# Typical Workflow

1. `init_uart()`  
Initialize the MCU for UART transmissions.
2. `uart_transmit_string()`  
Send a string through UART.
3. `uart_wait_for_reply()`  
Allow the receiver of `uart_transmit_string()` to send a reply.
4. `uart_copy_clear()`  
Copy the receiver's reply to a local array and clear the receive buffer for future use.
---
# Notes On Receiving Data

The interrupt service routine `ISR(USART_RX_vect)` copies incoming data into the internal receive buffer `__uart_rcv_buf[]`. The size of `__uart_rcv_buf[]` is tracked with the `__uart_rcv_buf_i` internal variable.

Try to avoid using `__uart_rcv_buf[]` and `__uart_rcv_buf_i` directly, as misusing these variables might produce some unwanted results. Instead, try using the functions `uart_copy_clear()` (to access the receive buffer `__uart_rcv_buf[]`) and `uart_rcv_buf_size()` (to access `__uart_rcv_buf_i`).

---
# Functions
---
# `init_uart()`
`void init_uart(uint16_t baud)`

**Description**  
Initialize the UART on the MCU.

The double speed feature (`U2Xn`) is automatically disabled in this library.

**Parameters**  
1. `uint16_t baud`  
The register `UBRRn` on the ATMega328P is set to the value of `baud`, so consult ATMega328P's datasheet to determine the appropriate integer for your desired baud rate.

**Examples**
```C
init_uart(0x01);  
```
In this example, the ATMega328P is running with a 3.6864MHz crystal with a 115.2k baud rate. This creates a 0.0% error rate.

---
# `uart_transmit_string()`

`void uart_transmit_string(unsigned char* str, uint16_t len)`

**Description**  
`uart_transmit_string()` sends the contents of `str[]` through the UART.

**Parameters**  
1. `unsigned char* str`  
An array of type `unsigned char` that contains the data to send through the UART.  
2. `uint16_t len`  
The length of the other arg `str`.

**Examples**
```C
unsigned char my_cmd[3] = {0x01, 0x02, 0x03};
uart_transmit_string(my_cmd, 3);
```
In this example, `0x01 0x02 0x03` is sent over the UART.

---
# `uart_wait_for_reply()`
`void uart_wait_for_reply()`

**Description**  
Wait for a reply from the other end of the UART connection.  
Use this when waiting for a reply after sending a command through `uart_transmit_string()`

---
# `uart_copy_clear()`

`uint16_t uart_copy_clear(unsigned char* dest)`

**Description**  
`uart_copy_clear()` copies the data received from the UART to `dest[]`.
`dest[]` should be sent in blank, as all data in the array will be wiped.  
After that copy, all data in `__uart_rcv_buf[]` will be set to 0 and `__uart_rcv_buf_i` will be set to 0.

**Parameters**  
1. `unsigned char* dest`  
An array that receives a copy of the data from `__uart_rcv_buf[]`.

**Return Values**  
This function returns `__uart_rcv_buf_i`, which is the size of the receive buffer `__uart_rcv_buf[]`.

Data from `__uart_rcv_buf[]` is copied to `dest[]`.

**Examples**
```C
/*
Assume the following:
*__uart_rcv_buf = {0x0A, 0x0B, 0x0C};
__uart_rcv_buf_i = 3;
*/
unsigned char my_data[16];
uint16_t len = uart_copy_clear(my_data);
```
`my_data` will contain `{0x0A, 0x0B, 0x0C}` and `len` will be `3`. All data in `__uart_rcv_buf[]` will be set to 0 and `__uart_rcv_buf_i` will be set to 0.

