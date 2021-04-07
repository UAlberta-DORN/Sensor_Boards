#include <Wire.h>

// volatile int alarm;

// Device addresses
const int OPT3001_Address = 0x44;
const int TMP116_Address = 0x49;

// Hexadecimal addresses for various OPT3001 registers
const int OPT_Reg = 0x00;                // OPTical register
const int OPT_Config_Reg = 0x01;         // Configuration register
const int OPT_Low_Lim_Reg = 0x02;        // High limit register
const int OPT_High_Lim_Reg = 0x03;       // Low limit register
const int OPT_Device_ID_Reg = 0x7F;      // Device ID register

// Hexadecimal addresses for various TMP116 registers
const int Temp_Reg = 0x00;               // Temperature register
const int Temp_Config_Reg = 0x01;        // Configuration register
const int Temp_High_Lim_Reg = 0x02;      // High limit register
const int Temp_Low_Lim_Reg = 0x03;       // Low limit register
const int Temp_EEPROM_Unlock_Reg = 0x04; // EEPROM unlock register
const int Temp_Device_ID_Reg = 0x0F;     // Device ID register

// Set OPTical threshold
const uint8_t OPT_highlimH = B10111111;  // High byte of high lim
const uint8_t OPT_highlimL = B11111111;  // Low byte of high lim
const uint8_t OPT_lowlimH = B00000000;   // High byte of low lim
const uint8_t OPT_lowlimL = B00000001;   // Low byte of low lim

// Set temperature threshold
const uint8_t Temp_highlimH = B00001101; // High byte of high lim
const uint8_t Temp_highlimL = B10000000; // Low byte of high lim   - High 27 C
const uint8_t Temp_lowlimH = B00001100;  // High byte of low lim
const uint8_t Temp_lowlimL = B00000000;  // Low byte of low lim    - Low 24 C
