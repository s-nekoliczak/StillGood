
// Write in process bit
#define EEPROM_BIT_WIP              0b00000001

// Write enabled latch bit
#define EEPROM_BIT_WEL              0b00000010

#define EEPROM_CMD_READ             0b00000011
#define EEPROM_CMD_WRITE            0b00000010

// Reset write enable latch (disable write operations)
#define EEPROM_CMD_WRDI             0b00000100

// Enable write latch (enable write operations)
#define EEPROM_CMD_WREN             0b00000110

// Read status register
#define EEPROM_CMD_RDSR             0b00000101

// Write status register
#define EEPROM_CMD_WRSR             0b00000001

#define EEPROM_PAGE_SIZE            0x40
#define EEPROM_PAGE_SIZE_MASK       0x3F
#define EEPROM_FIRST_ADDR           0x0000
// Size of EEPROM in bytes
#define EEPROM_SIZE                 0x8000

