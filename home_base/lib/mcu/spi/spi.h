
void spi_init();

char spi_receive();

// Call spi_send(0) if waiting for a response
void spi_send(char byte);

