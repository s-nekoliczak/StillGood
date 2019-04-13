# RTC

This article describes the functions available in the RTC library in rtc.c, how to use them, and examples.

This library is for use with the ATMega328P MCU and the DS1307+-ND real time clock chip.

---
# Hardware Notes

1. The SCL and SDA lines each need to be pulled up with one 4.7k ohm resistor per line.
2. A 32.768kHz crystal is needed for the X1/X2 pins.
3. VBAT needs to be connected to ground if a battery is not used.

---
# Overview Notes

A repeated start can be used for sending consecutive `send` and `rcv` commands.  
Using repeated starts will be a bit faster than stopping/starting the connection.  

Internally, this RTC chip can only handle years 0 through 99, so the base year is set by the RTC_YEAR_BIAS macro with the value 2000. So any year should be expressed as 2000 + X, where X is the value of the year field on the RTC. For example, 2019 will be saved internally on the RTC chip as year 19. This library masks this logic, but it might be something the application programmer wants to know.  
This means that the minimum year is 2000 and the max year is 2099.

---
# Typical Workflow

The workflow for using the RTC chip is simple. The user just needs to initiate the i2c hardware on the MCU and use commands supplied in the RTC library.

1. `i2c_init()`  
Initialize the i2c hardware on the ATMega328p. This is from the i2c.c library.
2. Call `send` or `rcv` functions to set or get values from the RTC chip, respectively.

---
# A Note on Handling Errors

In case of an error on a function that interacts with the RTC chip, the i2c transmission error code will available via the `rtc_get_last_err()` function and the i2c connection will be closed.  
See the datasheet for ATMega328P for details on these i2c transmission result codes.  

If an error code is returned, then the command sent most likely did not process correctly on the RTC chip. The application code should handle this error and act accordingly.  
See the return values of individual functions below to see which return values are error codes.  

---
# Functions

---
# `rtc_clock_is_running()`

`int8_t rtc_clock_is_running(uint8_t close_ctn)`

**Description**  
`rtc_clock_is_running()` says whether or not the time-keeping clock is running on the RTC chip. If the clock is running, it keeps track of time as expected. If the clock is not running, time is paused and all time values will remain constant until the clock is started again.

**Parameters**  
1. `uint8_t close_ctn`  
A boolean value that determines if the i2c connection should be closed or kept open for a repeated start.  
A `1` value will close the connection.  
A `0` value will keep the connection open for a repeated start.

**Return Values**  
This function returns `1` if the clock is running and keeping track of time.  
This function returns `0` if the clock is stopped.  
This function returns `-1` on an i2c transmission error.

---
# `rtc_clock_start()`

`uint8_t rtc_clock_start(uint8_t close_ctn)`

**Description**  
`rtc_clock_start()` starts the time-keeping clock on the RTC chip.

**Parameters**  
1. `uint8_t close_ctn`  
A boolean value that determines if the i2c connection should be closed or kept open for a repeated start.  
A `1` value will close the connection.  
A `0` value will keep the connection open for a repeated start.

**Return Values**  
This function returns `1` on a successful send.  
This function returns `0` on an i2c transmission error.  

---
# `rtc_clock_stop()`

`uint8_t rtc_clock_stop(uint8_t close_ctn)`

**Description**  
`rtc_clock_stop()` stops the time-keeping clock on the RTC chip.

**Parameters**  
1. `uint8_t close_ctn`  
A boolean value that determines if the i2c connection should be closed or kept open for a repeated start.  
A `1` value will close the connection.  
A `0` value will keep the connection open for a repeated start.

**Return Values**  
This function returns `1` on a successful send.  
This function returns `0` on an i2c transmission error.  

---
# `rtc_send_year()`

`uint8_t rtc_send_year(uint16_t years, uint8_t close_ctn)`

**Description**  
`rtc_send_year()` sets the year on the RTC. The minimum year is `2000` and the maximum year is `2099`.  

**Parameters**  
1. `uint16_t years`  
A year in a human format that will be saved to the RTC.
If a value less than `2000` is sent, then the year will be `2000`. If a value greater than `2099` is sent, then the year will be `2099`.  
2. `uint8_t close_ctn`  
A boolean value that determines if the i2c connection should be closed or kept open for a repeated start.  
A `1` value will close the connection.  
A `0` value will keep the connection open for a repeated start.

**Return Values**  
This function returns `1` on a successful send.  
This function returns `0` on an i2c transmission error.  

**Examples**
```C
int status = rtc_send_year(2019, 1);  
```  
This command sets the year to 2019 and closes the i2c connection.
A `1` will be returned if the year is successfully set on the chip. A `0` will be returned if the transmission failed.

---
# `rtc_rcv_year()`

`int16_t rtc_rcv_year(uint8_t close_ctn)`

**Description**  
`rtc_rcv_year()` gets the year from the RTC chip. The minimum year is `2000` and the maximum year is `2099`.  

**Parameters**  
1. `uint8_t close_ctn`  
A boolean value that determines if the i2c connection should be closed or kept open for a repeated start.  
A `1` value will close the connection.  
A `0` value will keep the connection open for a repeated start.

**Return Values**  
This function returns the year as a 16-bit integer between `2000` and `2099` on a successful i2c transmission.  
This funcion returns `-1` on an i2c transmission error.  

**Examples**
```C
int status = rtc_rcv_year(0);  
```  
On i2c successful transmission, this command gets the year saved on the RTC chip and keeps the i2c connection open.
If the i2c transmission fails, then `-1` is returned and the i2c connection is closed.

---
# `rtc_send_month()`

`uint8_t rtc_send_month(uint8_t month, uint8_t close_ctn)`

**Description**  
`rtc_send_month()` sets the month on the RTC. The minimum month is `1` and the maximum month is `12`.  

**Parameters**  
1. `uint8_t month`  
The number month that will be saved to the RTC. Only values `1` through `12` inclusive are valid values to send in.  
If a value less than `1` is sent, then the month will be `1`. If a value greater than `12` is sent, then the month will be `12`.  
2. `uint8_t close_ctn`  
A boolean value that determines if the i2c connection should be closed or kept open for a repeated start.  
A `1` value will close the connection.  
A `0` value will keep the connection open for a repeated start.

**Return Values**  
This function returns `1` on a successful send.  
This function returns `0` on an i2c transmission error.  

**Examples**
```C
int status = rtc_send_month(4, 1);  
```  
This command sets the month to 4 (April) and closes the i2c connection.
A `1` will be returned if the month is successfully set on the chip. A `0` will be returned if the transmission failed.

---
# `rtc_rcv_month()`

`int8_t rtc_rcv_month(uint8_t close_ctn)`

**Description**  
`rtc_rcv_month()` gets the month from the RTC chip. The minimum month is `1` and the maximum month is `12`.  

**Parameters**  
1. `uint8_t close_ctn`  
A boolean value that determines if the i2c connection should be closed or kept open for a repeated start.  
A `1` value will close the connection.  
A `0` value will keep the connection open for a repeated start.

**Return Values**  
This function returns the month as a 8-bit integer between `1` and `12` on a successful i2c transmission.  
This function returns `-1` on an i2c transmission error.  

**Examples**
```C
int status = rtc_rcv_month(0);  
```  
On i2c successful transmission, this command gets the month saved on the RTC chip and keeps the i2c connection open.
If the i2c transmission fails, then `-1` is returned and the i2c connection is closed.

---
# `rtc_send_date()`

`uint8_t rtc_send_date(uint16_t date, uint8_t close_ctn)`

**Description**  
`rtc_send_date()` sets the dateon the RTC. The minimum date is `1` and the maximum date is dependent on which month is currently set.  

**Parameters**  
1. `uint8_t date`  
The number date that will be saved to the RTC. Only values 1 through the maximum date of the current month are valid.
If a value less than `1` is sent, then the date will be `1`. If a value greater than the maximum date of the month is sent, then the date will be that maximum date.  
This library also accounts for leap years.  
2. `uint8_t close_ctn`  
A boolean value that determines if the i2c connection should be closed or kept open for a repeated start.  
A `1` value will close the connection.  
A `0` value will keep the connection open for a repeated start.

**Return Values**  
This function returns `1` on a successful send.  
This function returns `0` on an i2c transmission error.  

**Examples**
```C
int status = rtc_send_date(14, 1);  
```  
This command sets the date to the 14th and closes the i2c connection.
A `1` will be returned if the date is successfully set on the chip. A `0` will be returned if the transmission failed.

---
# `rtc_rcv_date()`

`int8_t rtc_rcv_date(uint8_t close_ctn)`

**Description**  
`rtc_rcv_date()` gets the date from the RTC chip. The minimum date is `1` and the maximum date is dependent on which month is currently set.  

**Parameters**  
1. `uint8_t close_ctn`  
A boolean value that determines if the i2c connection should be closed or kept open for a repeated start.  
A `1` value will close the connection.  
A `0` value will keep the connection open for a repeated start.

**Return Values**  
This function returns the date as a 8-bit integer between `1` and the maximum date of the current month on a successful i2c transmission.  
This function returns `-1` on an i2c transmission error.  

**Examples**
```C
int status = rtc_rcv_date(0);  
```  
On i2c successful transmission, this command gets the date saved on the RTC chip and keeps the i2c connection open.
If the i2c transmission fails, then `-1` is returned and the i2c connection is closed.

---
# `rtc_send_day()`

`uint8_t rtc_send_day(uint8_t day, uint8_t close_ctn)`

**Description**  
`rtc_send_day()` sets the day of the week on the RTC. The minimum day is `1` (Sunday) and the maximum day is `7` (Saturday).  

**Parameters**  
1. `uint8_t day`  
The number day that will be saved to the RTC. Only values 1 through 7 inclusive are valid values to send in.  
If a value less than `1` is sent, then the day will be `1`. If a value greater than `7` is sent, then the day will be `7`.  
2. `uint8_t close_ctn`  
A boolean value that determines if the i2c connection should be closed or kept open for a repeated start.  
A `1` value will close the connection.  
A `0` value will keep the connection open for a repeated start.

**Return Values**  
This function returns `1` on a successful send.  
This function returns `0` on an i2c transmission error.  

**Examples**
```C
int status = rtc_send_day(5, 1);  
```  
This command sets the day to 5 (Thursday) and closes the i2c connection.
A `1` will be returned if the day is successfully set on the chip. A `0` will be returned if the transmission failed.

---
# `rtc_rcv_day()`

`int8_t rtc_rcv_day(uint8_t close_ctn)`

**Description**  
`rtc_rcv_day()` gets the day from the RTC chip. The minimum day is `1` (Sunday) and the maximum day is `7` (Saturday).  

**Parameters**  
1. `uint8_t close_ctn`  
A boolean value that determines if the i2c connection should be closed or kept open for a repeated start.  
A `1` value will close the connection.  
A `0` value will keep the connection open for a repeated start.

**Return Values**  
This function returns the day as a 8-bit integer between `1` and `7` on a successful i2c transmission.  
This function returns `-1` on an i2c transmission error.  

**Examples**
```C
int status = rtc_rcv_day(0);  
```  
On i2c successful transmission, this command gets the day saved on the RTC chip and keeps the i2c connection open.
If the i2c transmission fails, then `-1` is returned and the i2c connection is closed.

---
# `rtc_send_hour()`

`uint8_t rtc_send_hour(uint8_t hour, uint8_t close_ctn)`

**Description**  
`rtc_send_hour()` sets the hour on the RTC. The minimum hour is `0` (midnight/00:00) and the maximum hour is `23` (23:00).  
This library only operates on a 24-hour clock. The application programmer will need to convert this value to AM/PM if desired.  

**Parameters**  
1. `uint8_t hour`  
The hour that will be saved to the RTC. Only values `0` through `23` inclusive are valid values to send in.  
If a value greater than `23` is sent, then the hour will be `23`.  
2. `uint8_t close_ctn`  
A boolean value that determines if the i2c connection should be closed or kept open for a repeated start.  
A `1` value will close the connection.  
A `0` value will keep the connection open for a repeated start.

**Return Values**  
This function returns `1` on a successful send.  
This function returns `0` on an i2c transmission error.  

**Examples**
```C
int status = rtc_send_hour(15, 1);  
```  
This command sets the hour to 15 (15:00/03:00PM) and closes the i2c connection.
A `1` will be returned if the hour is successfully set on the chip. A `0` will be returned if the transmission failed.

---
# `rtc_rcv_hour()`

`int8_t rtc_rcv_hour(uint8_t close_ctn)`

**Description**  
`rtc_rcv_hour()` gets the hour from the RTC chip. The minimum hour is `0` (midnight/00:00) and the maximum hour is `23` (23:00).  
This library only operates on a 24-hour clock. The application programmer will need to convert this value to AM/PM if desired.  

**Parameters**  
1. `uint8_t close_ctn`  
A boolean value that determines if the i2c connection should be closed or kept open for a repeated start.  
A `1` value will close the connection.  
A `0` value will keep the connection open for a repeated start.

**Return Values**  
This function returns the hour as a 8-bit integer between `0` and `23` on a successful i2c transmission.  
This function returns `-1` on an i2c transmission error.  

**Examples**
```C
int status = rtc_rcv_hour(0);  
```  
On i2c successful transmission, this command gets the hour saved on the RTC chip and keeps the i2c connection open.
If the i2c transmission fails, then `-1` is returned and the i2c connection is closed.

---
# `rtc_send_mnt()`

`uint8_t rtc_send_mnt(uint8_t mnt, uint8_t close_ctn)`

**Description**  
`rtc_send_mnt()` sets the minute on the RTC. The minimum minute is `0` and the maximum minute is `59`.  

**Parameters**  
1. `uint8_t mnt`  
The minute that will be saved to the RTC. Only values `0` through `59` inclusive are valid values to send in.  
If a value greater than `59` is sent, then the minute will be `59`.  
2. `uint8_t close_ctn`  
A boolean value that determines if the i2c connection should be closed or kept open for a repeated start.  
A `1` value will close the connection.  
A `0` value will keep the connection open for a repeated start.

**Return Values**  
This function returns `1` on a successful send.  
This function returns `0` on an i2c transmission error.  

**Examples**
```C
int status = rtc_send_mnt(27, 1);  
```  
This command sets the minute to 27 (if hour is 15, then time will be 15:27/03:27PM) and closes the i2c connection.
A `1` will be returned if the minute is successfully set on the chip. A `0` will be returned if the transmission failed.

---
# `rtc_rcv_mnt()`

`int8_t rtc_rcv_mnt(uint8_t close_ctn)`

**Description**  
`rtc_rcv_mnt()` gets the minute from the RTC chip. The minimum minute is `0` and the maximum minute is `59`.  

**Parameters**  
1. `uint8_t close_ctn`  
A boolean value that determines if the i2c connection should be closed or kept open for a repeated start.  
A `1` value will close the connection.  
A `0` value will keep the connection open for a repeated start.

**Return Values**  
This function returns the minute as a 8-bit integer between `0` and `59` on a successful i2c transmission.  
This function returns `-1` on an i2c transmission error.  

**Examples**
```C
int status = rtc_rcv_mnt(0);  
```  
On i2c successful transmission, this command gets the minute saved on the RTC chip and keeps the i2c connection open.
If the i2c transmission fails, then `-1` is returned and the i2c connection is closed.

---
# `rtc_send_sec()`

`uint8_t rtc_send_sec(uint8_t sec, uint8_t close_ctn)`

**Description**  
`rtc_send_sec()` sets the second on the RTC. The minimum second is `0` and the maximum second is `59`.  

**Parameters**  
1. `uint8_t sec`  
The second that will be saved to the RTC. Only values `0` through `59` inclusive are valid values to send in.  
If a value greater than `59` is sent, then the second will be `59`.  
2. `uint8_t close_ctn`  
A boolean value that determines if the i2c connection should be closed or kept open for a repeated start.  
A `1` value will close the connection.  
A `0` value will keep the connection open for a repeated start.

**Return Values**  
This function returns `1` on a successful send.  
This function returns `0` on an i2c transmission error.  

**Examples**
```C
int status = rtc_send_sec(49, 1);  
```  
This command sets the second to 49 (if hour is 15 and minute is 27, then time will be 15:27:49/03:27:49PM) and closes the i2c connection.
A `1` will be returned if the second is successfully set on the chip. A `0` will be returned if the transmission failed.

---
# `rtc_rcv_sec()`

`int8_t rtc_rcv_sec(uint8_t close_ctn)`

**Description**  
`rtc_rcv_sec()` gets the second from the RTC chip. The minimum second is `0` and the maximum second is `59`.  

**Parameters**  
1. `uint8_t close_ctn`  
A boolean value that determines if the i2c connection should be closed or kept open for a repeated start.  
A `1` value will close the connection.  
A `0` value will keep the connection open for a repeated start.

**Return Values**  
This function returns the second as a 8-bit integer between `0` and `59` on a successful i2c transmission.  
This function returns `-1` on an i2c transmission error.  

**Examples**
```C
int status = rtc_rcv_sec(0);  
```  
On i2c successful transmission, this command gets the second saved on the RTC chip and keeps the i2c connection open.
If the i2c transmission fails, then `-1` is returned and the i2c connection is closed.

