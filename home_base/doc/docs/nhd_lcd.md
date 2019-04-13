# NHD LCD

This article describes the functions available in the NHD (New Haven Display) LCD library in nhd_lcd.c, how to use them, and examples.

This library is for use with the ATMega328P MCU and the NHD-C12864A1Z-FS(RGB)-FBW-HT1-ND LCD.


---
# Hardware Notes

TODO

---
# Overview Notes

Each row has a maximum length of 22 characters.

There are 8 rows total. They are addressed between row 7 (top of screen) and row 0 (bottom of screen).

Only upper-case versions of characters will be displayed. Functions can still be called with either upper- or lower-case characters, though.

There is an option when printing rows to make a row selectable. This means that the first character will be empty. If a row is selected, then that first character will turn into a right-pointing arrow. This also means the maximum length of the row is the reserved space for selection plus 21 other characters.  

---
# Typical Workflow

1. `nhd_lcd_base_init()`  
Initializes the MCU on the LCD and turns on the backlight.  
2. `nhd_lcd_init_screen()`  
Sets various settings on the LCD MCU.

---
# Functions

---
# `nhd_lcd_init()`

`void nhd_lcd_init_screen()`

**Description**  
This function initializes various settings on the LCD MCU. These settings allow text to be visible on the LCD.  

---
# `nhd_lcd_light_on()`

`void nhd_lcd_light_on()`

**Description**  
This function turns on the backlight for the NHD LCD. The contents of the screen will remain untouched.

---
# `nhd_lcd_light_off()`

`void nhd_lcd_light_off()`

**Description**  
This function turns off the backlight for the NHD LCD. The contents of the screen will remain untouched.

---
# `nhd_lcd_wipe_screen()`

`void nhd_lcd_wipe_screen()`

**Description**  
This function turns off all pixels on the LCD screen.

---
# `nhd_lcd_print_row_str()`

`void nhd_lcd_print_row_str(uint8_t row, char* str, uint8_t str_len, uint8_t is_selectable, uint8_t is_selected)`

**Description**  
This function prints a string on a row on the LCD screen. The row can also be made selectable. If the row is selectable, then the row can also be selected, which is signified with a right-pointing arrow character in the first position of the row.

**Parameters**  
1. `uint8_t row`  
The row to write `str` to. Row 7 is the top of the screen and row 0 is the bottom of the screen.  
2. `char* str`  
A string to write to the specified row. The maximum length string that can be written to the LCD is 22.  
3. `uint8_t str_len`  
The length of the `str` argument. Maximum length the LCD can write to one row is 22.  
4. `uint8_t is_selectable`  
An option to make the row selectable.  
If set to `1`, then the first character in the row is reserved for a potential arrow character, signifying the row is selected. If the row is not selected, then the first character position is blank.  
If set to `0`, then the row will not be selectable and the first character position will not be reserved for a potential arrow character.  
5. `uint8_t is_selected`  
An option to select the row with an arrow character. This option is ignored if the `is_selectable` argument is set to `0`.  
If set to `1`, then an arrow character will be displayed in the first character position of the row.  
If set to `0`, then a space character will be displayed in the first character position of the row.  


**Examples**
```C
nhd_lcd_print_row_str(7, "hello", 5, 1, 1);
```  
This command will output the string `"hello"` on the top row (row 7) of the LCD screen. `"hello"` is 5 characters long, so the value `5` is sent into the `str_len` parameter.
This row is also made selectable, so the `"hello"` string starts at the second character position.
And, since this row is selected (with the last argument set to 1), the first position will have the right-pointing arrow character displayed.

---
# `nhd_lcd_sel_row()`

`void nhd_lcd_sel_row(uint8_t row, uint8_t select)`

**Description**  
If the row is selectable, then the row can also be selected, which is signified with a right-pointing arrow character in the first position of the row.  
Note that this function does not determine if a row is selectable or not and will overwrite any character that may be present in the first position. The application programmer should account for this and keep track of which rows are selectable in their application.  

**Parameters**  
1. `uint8_t row`  
The row select of deselect. Row 7 is the top of the screen and row 0 is the bottom of the screen.  
2. `uint8_t select`  
This determines if the row should be selected or not.
If `1`, then this row is selected and a right-pointing arrow is displayed in the first position of the row.
If `0`, then a blank space is displayed in the first position of the row.


**Examples**
```C
nhd_lcd_sel_row(5, 0);
```  
This command will deselect row 5 (3rd row from the top) by blanking the character in the first position.
```C
nhd_lcd_sel_row(0, 1);
```  
This command will select row 0 (bottom row) by setting the character in the first position to the right-pointint arrow character.

