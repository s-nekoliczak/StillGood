
#define SPI_PORT                    PORTB
#define SPI_DDR                     DDRB

#define SPI_SS                      PB2
#define SPI_MOSI                    PB3
#define SPI_MISO                    PB4
#define SPI_SCK                     PB5

#define SPI_SS_SELECT               SPI_PORT &= ~(1 << SPI_SS)
#define SPI_SS_DESELECT             SPI_PORT |= (1 << SPI_SS)

