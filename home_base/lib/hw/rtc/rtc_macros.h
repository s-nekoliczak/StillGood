
#define RTC_I2C_ADDR        0x68

#define RTC_YEAR_BIAS       2000

// RTC internal addresses
#define RTC_ADDR_SECOND     0x00
#define RTC_ADDR_MINUTE     0x01
#define RTC_ADDR_HOUR       0x02
#define RTC_ADDR_DAY        0x03
#define RTC_ADDR_DATE       0x04
#define RTC_ADDR_MONTH      0x05
#define RTC_ADDR_YEAR       0x06
#define RTC_ADDR_CONTROL    0x07

#define RTC_MIN_YEAR        0
#define RTC_MIN_MONTH       1
#define RTC_MIN_DATE        1
#define RTC_MIN_DAYS        1
#define RTC_MIN_HOURS       0
#define RTC_MIN_MINUTES     0
#define RTC_MIN_SECONDS     0

#define RTC_MAX_YEAR        99
#define RTC_MAX_MONTH       12
// Use __rtc_month_max_date for max date for a particular month.
#define RTC_MAX_DAYS        7
#define RTC_MAX_HOURS       23
#define RTC_MAX_MINUTES     59
#define RTC_MAX_SECONDS     59

#define RTC_MONTH_JAN       1
#define RTC_MONTH_FEB       2
#define RTC_MONTH_MAR       3
#define RTC_MONTH_APR       4
#define RTC_MONTH_MAY       5
#define RTC_MONTH_JUN       6
#define RTC_MONTH_JUL       7
#define RTC_MONTH_AUG       8
#define RTC_MONTH_SEP       9
#define RTC_MONTH_OCT       10
#define RTC_MONTH_NOV       11
#define RTC_MONTH_DEC       12

#define RTC_MONTH_DAYS_JAN      31
#define RTC_MONTH_DAYS_FEB      28
#define RTC_MONTH_DAYS_FEB_LPYR 29
#define RTC_MONTH_DAYS_MAR      31
#define RTC_MONTH_DAYS_APR      30
#define RTC_MONTH_DAYS_MAY      31
#define RTC_MONTH_DAYS_JUN      30
#define RTC_MONTH_DAYS_JUL      31
#define RTC_MONTH_DAYS_AUG      31
#define RTC_MONTH_DAYS_SEP      30
#define RTC_MONTH_DAYS_OCT      31
#define RTC_MONTH_DAYS_NOV      30
#define RTC_MONTH_DAYS_DEC      31

