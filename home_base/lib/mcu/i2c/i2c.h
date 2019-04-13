
// Master transmitter error codes: pg 273, table 26-3
#define I2C_MSTR_TRANS_START            0x08
#define I2C_MSTR_TRANS_RPTD_START       0x10
#define I2C_MSTR_TRANS_SLA_W_ACK        0x18
#define I2C_MSTR_TRANS_SLA_W_NACK       0x20
#define I2C_MSTR_TRANS_SEND_BYTE_ACK    0x28
#define I2C_MSTR_TRANS_SEND_BYTE_NACK   0x30
#define I2C_MSTR_TRANS_ARB_LOST         0x38

// Master receiver error codes: pg 277, table 26-4
#define I2C_MSTR_RCV_START              0x08
#define I2C_MSTR_RCV_RPTD_START         0x10
#define I2C_MSTR_RCV_ARB_LOST           0x38
#define I2C_MSTR_RCV_SLA_R_ACK          0x40
#define I2C_MSTR_RCV_SLA_R_NACK         0x48
#define I2C_MSTR_RCV_RCV_BYTE_ACK       0x50
#define I2C_MSTR_RCV_RCV_BYTE_NACK      0x58


void i2c_init();

void i2c_wait_for_hdwr_rspn();

uint8_t i2c_status();

// Write is 0, read is 1.
void i2c_set_addr(uint8_t addr, uint8_t w_r);

void i2c_set_dbyte(uint8_t data);

uint8_t i2c_read_data();

uint8_t i2c_start_trans();

// Write is 0, read is 1.
uint8_t i2c_send_addr(uint8_t addr, uint8_t w_r);

uint8_t i2c_send_dbyte(uint8_t byte);

uint8_t i2c_rcv_dbyte();

uint8_t i2c_stop_send();

uint8_t i2c_stop_rcv();


