# NFC

This article describes the functions available in the NFC library in nfc.c, how to use them, and examples.

This library is for use with the ATMega328P MCU, NFC Module for Arduino (SKU:DFR0231 at https://dfrobot.com), and with ISO-14443A/Mifare NFC tags.

---
# Typical Workflow

1. `nfc_wakeup()`  
Wakeup the device from sleep.
2. `nfc_inlistpsvtarget()`  
Retrieve the logical number and UID of the nearby tag.
3. `nfc_authenticate()`  
Authenticate the nearby tag.
4. At this point, we can read from and write to the nearby tag.
    1. `nfc_read_block()`
    2. `nfc_write_block()`
5. `sleep()`  
Set the device to sleep.

---
# Functions

---
# `nfc_wakeup()`

`uint8_t nfc_wakeup()`

**Description**  
Wakeup the NFC module from sleep mode.

**Return Values**  
This function returns a `1` on success and a `0` on failure.  

---
# `nfc_inlistpsvtarget()`

`uint8_t nfc_inlistpsvtarget()`

**Description**  
Assign a nearby tag to a logical number. Since this library only supports one nearby tag, this logical number will always be 1.  
The tag's UID will be saved internally in the `__g_uid` array of type `unsigned char`. This UID will be addressed by the logical number 1 for the duration that the tag is in the reader's field.

**Return Values**  
This function returns a `1` on success and a `0` on failure.  

---
# `nfc_authenticate()`
`uint8_t nfc_authenticate()`

**Description**  
Authenticate the nearby NFC tag.

**Return Values**  
This function returns a `1` on success and a `0` on failure.  

---
# `nfc_read_block()`

`uint8_t nfc_read_block(unsigned char* payload, unsigned char addr)`

**Description**  
`nfc_read_block()` copies the block from `addr` on the nearby tag to the `payload` array on the MCU.

**Paramters**  
1. `unsigned char* payload`  
A 16-byte array of type `unsigned char` that should be sent in blank.  
2. `unsigned char addr`  
An address of type `unsigned char` that targets 16-byte blocks on the NFC tag's memory. Consult your NFC tag's datasheet for address limits.

**Return Values**  
This function returns a `1` on success and a `0` on failure.  

On success, the `payload` array will be populated with the data from 16-byte block at `addr` on the NFC tag.  
On failure, the `payload` array will be untouched.

**Examples**
```C
unsigned char my_payload[16];
int retval = nfc_read_block(my_payload, 0x04);  
```
On success, this copies the 16-byte block at NFC tag address `0x04` (bytes 64-79) to the `my_payload` array and sets `retval` to `1`.  
On failure, `my_payload` will be untouched and `retval` will be set to `0`.

---
# `nfc_write_block()`

`nfc_write_block(unsigned char* payload, unsigned char addr)`

**Description**  
`nfc_write_block()` copies the data from `payload` to the block at `addr` on the nearby tag.

**Parameters**  
1. `unsigned char* payload`  
A 16-byte array of type `unsigned char` that should be sent in blank.  
2. `unsigned char addr`  
An address of type `unsigned char` that targets 16-byte blocks on the NFC tag's memory. Consult your NFC tag's datasheet for address limits.

**Return Values**  
This function returns a `1` on success and a `0` on failure.  

On success, the 16-byte block at `addr` on the NFC tag will be set to the 16 bytes of data in `payload`.  
On failure, the data on the NFC tag will be untouched.

**Examples**
```C
unsigned char my_payload[16];
int retval = nfc_write_block(my_payload, 0x04);  
```
On success, this copies the data in `payload` to the block at address `0x04` (bytes 64-79) on the NFC tag and `retval` will be set to `1`.  
On failure, the NFC tag's data will be untouched and `retval` will be set to `0`.

