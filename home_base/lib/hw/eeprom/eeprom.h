

void init_spi();

char spi_receive();

// Call spi_send(0) if waiting for a response
void spi_send(char byte);

void eeprom_read_bytes(uint16_t addr, char * str, uint16_t len);

/*
Only writes to one page. This _WON'T_ start writing to a subsequent page if the
length is long enough. It will rather wrap around and write to the beginning
of the current page per the datasheet's notes. Call eeprom_write_bytes to avoid
that wrapping issue and to write bytes sequentially regardless of page breaks.
*/
void eeprom_write_page(uint16_t addr, char * str, uint16_t len);

void eeprom_write_bytes(uint16_t addr, char * str, uint16_t len);

