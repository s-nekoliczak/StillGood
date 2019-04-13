
#include <stdio.h>
#include "../../mcu/i2c/i2c.h"
#include "rtc_macros.h"

// This array starts at index 1 and ends at index 12.
// Index 0 should be avoided.
const uint8_t __rtc_month_max_date[13] = {
    0,
    RTC_MONTH_DAYS_JAN,
    RTC_MONTH_DAYS_FEB,
    RTC_MONTH_DAYS_MAR,
    RTC_MONTH_DAYS_APR,
    RTC_MONTH_DAYS_MAY,
    RTC_MONTH_DAYS_JUN,
    RTC_MONTH_DAYS_JUL,
    RTC_MONTH_DAYS_AUG,
    RTC_MONTH_DAYS_SEP,
    RTC_MONTH_DAYS_OCT,
    RTC_MONTH_DAYS_NOV,
    RTC_MONTH_DAYS_DEC
};

// Records what the error code of the last I2C NACK was.
uint8_t __rtc_last_err = 0;

uint8_t rtc_get_last_err() {
    return __rtc_last_err;
}

uint8_t rtc_send_data(uint8_t addr, uint8_t* data, uint8_t data_len,
                                uint8_t close_ctn) {

    uint8_t st = i2c_start_trans();

    if (st != I2C_MSTR_TRANS_START && st != I2C_MSTR_TRANS_RPTD_START) {
        i2c_stop_send();
        __rtc_last_err = st;
        return st;
    }

    st = i2c_send_addr(RTC_I2C_ADDR, 0);
    if (st != I2C_MSTR_TRANS_SLA_W_ACK) {
        i2c_stop_send();
        __rtc_last_err = st;
        return st;
    }

    st = i2c_send_dbyte(addr);
    if (st != I2C_MSTR_TRANS_SEND_BYTE_ACK) {
        i2c_stop_send();
        __rtc_last_err = st;
        return st;
    }

    for (uint8_t i = 0; i < data_len; ++i) {
        st = i2c_send_dbyte(data[i]);
        if (st != I2C_MSTR_TRANS_SEND_BYTE_ACK) {
            i2c_stop_send();
            __rtc_last_err = st;
            return st;
        }
    }

    if (close_ctn == 1) {
        i2c_stop_send();
    }

    return st;
}

uint8_t rtc_rcv_data(uint8_t addr, uint8_t* data, uint8_t data_len, uint8_t close_ctn) {

    rtc_send_data(addr, 0, 0, 0);

    uint8_t st = i2c_start_trans();
    if (st != I2C_MSTR_RCV_START && st != I2C_MSTR_RCV_RPTD_START) {
        i2c_stop_send();
        __rtc_last_err = st;
        return st;
    }

    st = i2c_send_addr(RTC_I2C_ADDR, 1);
    if (st != I2C_MSTR_RCV_SLA_R_ACK) {
        i2c_stop_send();
        __rtc_last_err = st;
        return st;
    }

    for (uint8_t i = 0; i < data_len; ++i) {
        st = i2c_rcv_dbyte();
        if (st == I2C_MSTR_RCV_RCV_BYTE_ACK) {
            data[i] = i2c_read_data();
        } else {
            i2c_stop_rcv();
            __rtc_last_err = st;
            return st;
        }
    }
    
    if (close_ctn == 1) {
        i2c_stop_rcv();
    }

    return st;
}

uint16_t __rtc_rtcyear_to_year(uint8_t rtcyear) {
    return ((RTC_YEAR_BIAS + ((rtcyear & 0xF0) >> 4) * 10) + (rtcyear & 0x0F));
}

uint8_t __rtc_year_to_rtcyear(uint16_t year) {
    return ((((year - RTC_YEAR_BIAS) / 10) << 4) | ((year - RTC_YEAR_BIAS) % 10));
}

uint8_t rtc_send_year(uint16_t year, uint8_t close_ctn) {
    if (year < RTC_YEAR_BIAS) {
        year = RTC_YEAR_BIAS;
    } else if (year - RTC_YEAR_BIAS > RTC_MAX_YEAR) {
        year = RTC_MAX_YEAR + RTC_YEAR_BIAS;
    }
    uint8_t year_arr[1] = { __rtc_year_to_rtcyear(year) };

    uint8_t st = rtc_send_data(RTC_ADDR_YEAR, year_arr, 1, close_ctn);

    if (st != I2C_MSTR_TRANS_SEND_BYTE_ACK) {
        i2c_stop_send();
        return 0;
    }

    return 1;
}

int16_t rtc_rcv_year(uint8_t close_ctn) {
    uint8_t rtcyear_arr[1];
    uint8_t st = rtc_rcv_data(RTC_ADDR_YEAR, rtcyear_arr, 1, close_ctn);
    if (st != I2C_MSTR_RCV_RCV_BYTE_ACK) {
        return -1;
    }

    return __rtc_rtcyear_to_year(rtcyear_arr[0]);
}

uint8_t __rtc_rtcmonth_to_month(uint8_t rtcmonth) {
    return ((((rtcmonth & 0x10) >> 4) * 10) + (rtcmonth & 0x0F));
}

uint8_t __rtc_month_to_rtcmonth(uint8_t month) {
    return (((month / 10) << 4) | (month % 10));
}

uint8_t rtc_send_month(uint8_t month, uint8_t close_ctn) {
    if (month < RTC_MIN_MONTH) {
        month = RTC_MIN_MONTH;
    } else if (month > RTC_MAX_MONTH) {
        month = RTC_MAX_MONTH;
    }
    uint8_t month_arr[1] = { __rtc_month_to_rtcmonth(month) };

    uint8_t st = rtc_send_data(RTC_ADDR_MONTH, month_arr, 1, close_ctn);

    if (st != I2C_MSTR_TRANS_SEND_BYTE_ACK) {
        i2c_stop_send();
        return 0;
    }

    return 1;
}

int8_t rtc_rcv_month(uint8_t close_ctn) {
    uint8_t rtcmonth_arr[1];
    uint8_t st = rtc_rcv_data(RTC_ADDR_MONTH, rtcmonth_arr, 1, close_ctn);
    if (st != I2C_MSTR_RCV_RCV_BYTE_ACK) {
        return -1;
    }

    return __rtc_rtcmonth_to_month(rtcmonth_arr[0]);
}

int8_t rtc_is_leap_year() {
    int16_t year = rtc_rcv_year(1);
    if (year == -1) {
        return -1;
    }
    return !(year % 4);
}


uint8_t __rtc_rtcdate_to_date(uint8_t rtcdate) {
    return ((((rtcdate & 0x30) >> 4) * 10) + (rtcdate & 0x0F));
}

uint8_t __rtc_date_to_rtcdate(uint8_t date) {
    return (((date / 10) << 4) | (date % 10));
}

uint8_t rtc_send_date(uint8_t date, uint8_t close_ctn) {

    uint8_t month = rtc_rcv_month(0);
    uint8_t max_date = __rtc_month_max_date[month];
    if (month == RTC_MONTH_FEB) {
        if (rtc_is_leap_year() == 1) {
            max_date = RTC_MONTH_DAYS_FEB_LPYR;
        }
    }

    if (date < RTC_MIN_DATE) {
        date = RTC_MIN_DATE;
    } else if (date > max_date) {
        date = max_date;
    }

    uint8_t date_arr[1] = { __rtc_date_to_rtcdate(date) };

    uint8_t st = rtc_send_data(RTC_ADDR_DATE, date_arr, 1, close_ctn);

    if (st != I2C_MSTR_TRANS_SEND_BYTE_ACK) {
        i2c_stop_send();
        return 0;
    }

    return 1;
}

int8_t rtc_rcv_date(uint8_t close_ctn) {
    uint8_t rtcdate_arr[1];
    uint8_t st = rtc_rcv_data(RTC_ADDR_DATE, rtcdate_arr, 1, close_ctn);
    if (st != I2C_MSTR_RCV_RCV_BYTE_ACK) {
        return -1;
    }

    return __rtc_rtcdate_to_date(rtcdate_arr[0]);
}

uint8_t __rtc_rtcday_to_day(uint8_t rtcday) {
    return (rtcday & 0x07);
}

uint8_t __rtc_day_to_rtcday(uint8_t day) {
    return (day & 0x07);
}

uint8_t rtc_send_day(uint8_t day, uint8_t close_ctn) {

    if (day < RTC_MIN_DAYS) {
        day = RTC_MIN_DAYS;
    } else if (day > RTC_MAX_DAYS) {
        day = RTC_MAX_DAYS;
    }

    uint8_t day_arr[1] = { __rtc_day_to_rtcday(day) };

    uint8_t st = rtc_send_data(RTC_ADDR_DAY, day_arr, 1, close_ctn);

    if (st != I2C_MSTR_TRANS_SEND_BYTE_ACK) {
        i2c_stop_send();
        return 0;
    }

    return 1;
}

int8_t rtc_rcv_day(uint8_t close_ctn) {
    uint8_t rtcday_arr[1];
    uint8_t st = rtc_rcv_data(RTC_ADDR_DAY, rtcday_arr, 1, close_ctn);
    if (st != I2C_MSTR_RCV_RCV_BYTE_ACK) {
        return -1;
    }

    return __rtc_rtcday_to_day(rtcday_arr[0]);
}




// Only works for 24-hour clock.
uint8_t __rtc_rtchour_to_hour(uint8_t rtchour) {
    return ((((rtchour & 0x30) >> 4) * 10) + (rtchour & 0x0F));
}

uint8_t __rtc_hour_to_rtchour(uint8_t hour) {
    // 0 & 0x40 enforces 24-hour format.
    return ((0 & 0x40) | ((hour / 10) << 4) | (hour % 10));
}

uint8_t rtc_send_hour(uint8_t hour, uint8_t close_ctn) {

    if (hour < RTC_MIN_HOURS) {
        hour = RTC_MIN_HOURS;
    } else if (hour > RTC_MAX_HOURS) {
        hour = RTC_MAX_HOURS;
    }

    uint8_t hour_arr[1] = { __rtc_hour_to_rtchour(hour) };

    uint8_t st = rtc_send_data(RTC_ADDR_HOUR, hour_arr, 1, close_ctn);

    if (st != I2C_MSTR_TRANS_SEND_BYTE_ACK) {
        i2c_stop_send();
        return 0;
    }

    return 1;
}

int8_t rtc_rcv_hour(uint8_t close_ctn) {
    uint8_t rtchour_arr[1];
    uint8_t st = rtc_rcv_data(RTC_ADDR_HOUR, rtchour_arr, 1, close_ctn);
    if (st != I2C_MSTR_RCV_RCV_BYTE_ACK) {
        return -1;
    }

    return __rtc_rtchour_to_hour(rtchour_arr[0]);
}



uint8_t __rtc_rtcmnt_to_mnt(uint8_t rtcmnt) {
    return ((((rtcmnt & 0x70) >> 4) * 10) + (rtcmnt & 0x0F));
}

uint8_t __rtc_mnt_to_rtcmnt(uint8_t mnt) {
    return (((mnt / 10) << 4) | (mnt % 10));
}

uint8_t rtc_send_mnt(uint8_t mnt, uint8_t close_ctn) {

    if (mnt < RTC_MIN_MINUTES) {
        mnt = RTC_MIN_MINUTES;
    } else if (mnt > RTC_MAX_MINUTES) {
        mnt = RTC_MAX_MINUTES;
    }

    uint8_t mnt_arr[1] = { __rtc_mnt_to_rtcmnt(mnt) };

    uint8_t st = rtc_send_data(RTC_ADDR_MINUTE, mnt_arr, 1, close_ctn);

    if (st != I2C_MSTR_TRANS_SEND_BYTE_ACK) {
        i2c_stop_send();
        return 0;
    }

    return 1;
}

int8_t rtc_rcv_mnt(uint8_t close_ctn) {
    uint8_t rtcmnt_arr[1];
    uint8_t st = rtc_rcv_data(RTC_ADDR_MINUTE, rtcmnt_arr, 1, close_ctn);
    if (st != I2C_MSTR_RCV_RCV_BYTE_ACK) {
        return -1;
    }

    return __rtc_rtcmnt_to_mnt(rtcmnt_arr[0]);
}


uint8_t rtc_clock_is_running(uint8_t close_ctn) {
    uint8_t rtcsec_arr[1];
    uint8_t st = rtc_rcv_data(RTC_ADDR_SECOND, rtcsec_arr, 1, close_ctn);
    if (st != I2C_MSTR_RCV_RCV_BYTE_ACK) {
        return 2;
    }

    return !(rtcsec_arr[0] & 0x80);
}

uint8_t __rtc_rtcsec_to_sec(uint8_t rtcsec) {
    return ((((rtcsec & 0x70) >> 4) * 10) + (rtcsec & 0x0F));
}

uint8_t __rtc_sec_to_rtcsec(uint8_t sec, uint8_t start_clk) {
    return ((!start_clk << 7) | ((sec / 10) << 4) | (sec % 10));
}

uint8_t rtc_send_sec(uint8_t sec, uint8_t close_ctn) {

    if (sec < RTC_MIN_SECONDS) {
        sec = RTC_MIN_SECONDS;
    } else if (sec > RTC_MAX_SECONDS) {
        sec = RTC_MAX_SECONDS;
    }

    uint8_t is_running = rtc_clock_is_running(1);
    uint8_t sec_arr[1] = { __rtc_sec_to_rtcsec(sec, is_running) };

    uint8_t st = rtc_send_data(RTC_ADDR_SECOND, sec_arr, 1, close_ctn);

    if (st != I2C_MSTR_TRANS_SEND_BYTE_ACK) {
        i2c_stop_send();
        return 0;
    }

    return 1;
}

int8_t rtc_rcv_sec(uint8_t close_ctn) {
    uint8_t rtcsec_arr[1];
    uint8_t st = rtc_rcv_data(RTC_ADDR_SECOND, rtcsec_arr, 1, close_ctn);
    if (st != I2C_MSTR_RCV_RCV_BYTE_ACK) {
        return -1;
    }

    return __rtc_rtcsec_to_sec(rtcsec_arr[0]);
}

uint8_t rtc_clock_start(uint8_t close_ctn) {
    uint8_t clk_arr[1] = { (0 << 7) | (0x7F & rtc_rcv_sec(0)) };

    uint8_t st = rtc_send_data(RTC_ADDR_SECOND, clk_arr, 1, close_ctn);

    if (st != I2C_MSTR_TRANS_SEND_BYTE_ACK) {
        i2c_stop_send();
        return 0;
    }

    return 1;
}

uint8_t rtc_clock_stop(uint8_t close_ctn) {
    uint8_t clk_arr[1] = { (1 << 7) | (0x7F & rtc_rcv_sec(0))};

    uint8_t st = rtc_send_data(RTC_ADDR_SECOND, clk_arr, 1, close_ctn);

    if (st != I2C_MSTR_TRANS_SEND_BYTE_ACK) {
        i2c_stop_send();
        return 0;
    }

    return 1;
}

