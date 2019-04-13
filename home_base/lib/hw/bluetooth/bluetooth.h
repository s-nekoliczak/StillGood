
void __bt_transmit(char* str);

uint8_t __bt_cmd_transmit(char* cmd, char* arg, char* rep_ok);

/*
Action commands
*/
uint8_t bt_cmd_act_start_cmd();

uint8_t bt_cmd_act_exit_cmd();

uint8_t bt_cmd_act_disconnect();

uint8_t bt_cmd_act_reboot();

/*
Set commands
*/
uint8_t bt_cmd_factory_default();

uint8_t bt_cmd_set_mode(uint8_t mode);

uint8_t bt_cmd_set_device_name(char* name);

uint8_t bt_cmd_set_pin(char* pin);

uint8_t bt_cmd_set_serial_name(char* name);

uint8_t bt_cmd_power_cycle();

uint8_t bt_cmd_set_low_power_connect(uint8_t sec_off, uint8_t sec_on);


