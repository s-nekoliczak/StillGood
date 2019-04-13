
uint8_t rtc_send_data(uint8_t addr, uint8_t* data, uint8_t data_len,
                                uint8_t close_ctn);

uint8_t rtc_rcv_data(uint8_t addr, uint8_t* data, uint8_t data_len, uint8_t close_ctn);

// Misc cmds
uint8_t rtc_get_last_err();
int8_t rtc_is_leap_year();

// Clock cmds
uint8_t rtc_clock_is_running(uint8_t close_ctn);
uint8_t rtc_clock_start(uint8_t close_ctn);
uint8_t rtc_clock_stop(uint8_t close_ctn);


// Year cmds
uint16_t __rtc_rtcyear_to_year(uint8_t rtcyear);
uint8_t __rtc_year_to_rtcyear(uint16_t year);
uint8_t rtc_send_year(uint16_t years, uint8_t close_ctn);
int16_t rtc_rcv_year(uint8_t close_ctn);

// Month cmds
uint8_t __rtc_rtcmonth_to_month(uint8_t rtcmonth);
uint8_t __rtc_month_to_rtcmonth(uint8_t month);
uint8_t rtc_send_month(uint8_t month, uint8_t close_ctn);
int8_t rtc_rcv_month(uint8_t close_ctn);

// Date cmds
uint8_t __rtc_rtcdate_to_date(uint8_t rtcdate);
uint8_t __rtc_date_to_rtcdate(uint8_t date);
uint8_t rtc_send_date(uint8_t date, uint8_t close_ctn);
int8_t rtc_rcv_date(uint8_t close_ctn);

// Day cmds
uint8_t __rtc_rtcday_to_day(uint8_t rtcday);
uint8_t __rtc_day_to_rtcday(uint8_t day);
uint8_t rtc_send_day(uint8_t day, uint8_t close_ctn);
int8_t rtc_rcv_day(uint8_t close_ctn);

// Hour cmds
uint8_t __rtc_rtchour_to_hour(uint8_t rtchour);
uint8_t __rtc_hour_to_rtchour(uint8_t hour);
uint8_t rtc_send_hour(uint8_t hour, uint8_t close_ctn);
int8_t rtc_rcv_hour(uint8_t close_ctn);

// Minute cmds
uint8_t __rtc_rtcmnt_to_mnt(uint8_t rtcmnt);
uint8_t __rtc_mnt_to_rtcmnt(uint8_t mnt);
uint8_t rtc_send_mnt(uint8_t mnt, uint8_t close_ctn);
int8_t rtc_rcv_mnt(uint8_t close_ctn);

// Second cmds
uint8_t __rtc_rtcsec_to_sec(uint8_t rtcsec);
uint8_t __rtc_sec_to_rtcsec(uint8_t sec, uint8_t start_clk);
uint8_t rtc_send_sec(uint8_t sec, uint8_t close_ctn);
int8_t rtc_rcv_sec(uint8_t close_ctn);

