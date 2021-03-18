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

void setup() {

  Wire.begin(21,22);
  Serial.begin(115200);

  // Write limits to OPT register
  I2Cwrite(OPT3001_Address, OPT_High_Lim_Reg, OPT_highlimH, OPT_highlimL);
  I2Cwrite(OPT3001_Address, OPT_Low_Lim_Reg, OPT_lowlimH, OPT_lowlimL);

  // Write limits to TMP register
  I2Cwrite(TMP116_Address, Temp_High_Lim_Reg, Temp_highlimH, Temp_highlimL);
  I2Cwrite(TMP116_Address, Temp_Low_Lim_Reg, Temp_lowlimH, Temp_lowlimL);

  // Sets Pin 13 as output, Pin 2 as input (active low)
  pinMode(13, OUTPUT);
  pinMode(2, INPUT_PULLUP);

  // Sets up Pin 2 to trigger "alert" ISR when pin changes H->L and L->H
  attachInterrupt(digitalPinToInterrupt(2), alert, CHANGE);

//  alarm = digitalRead(2);
}

void loop() {
  // One-shot setting sent to config register
  I2Cwrite(OPT3001_Address, OPT_Config_Reg, 0xC2, 0x00);
  // Request and calculate OPTical lux reading
  double OPT_data = ReadOPTSensor();
  Serial.println(OPT_data);

  delay(10);

  // One-shot setting sent to config register
  I2Cwrite(TMP116_Address, Temp_Config_Reg, 0x0C, 0x20);
  // Request and calculate temperature reading
  double temp_data = ReadTempSensor();
  Serial.println(temp_data);

  delay(500);
}

double I2Cwrite(int dev, int reg, int H, int L){

  // Takes in 4 variables:
  // device address, register address,
  // high and low bytes of data to transmit
  Wire.beginTransmission(dev);
  Wire.write(reg);
  Wire.write(H);
  Wire.write(L);
  Wire.endTransmission();
  delay(10);

}

void alert(){

//  alarm = digitalRead(2);

}

double ReadOPTSensor(void){

  // Device addresses
  //int OPT3001_Address = 0x48;

  // Data array to store 2-bytes from I2C line
  uint8_t data[2];
  // Combination of 2-byte data into 16-bit data
  int16_t datac;
  // Mantissa and exponent from register
  int16_t iExponent, iMantissa;
  // Calculated lux from sensor data
  float fLux;

  // Points to device & begins transmission
  Wire.beginTransmission(OPT3001_Address);
  // Points to OPTical register to read/write data
  Wire.write(OPT_Reg);
  // Ends data transfer and transmits data from register
  Wire.endTransmission();

  // Delay to allow sufficient conversion time
  delay(10);

  // Requests 2-byte temperature data from device
  Wire.requestFrom(OPT3001_Address,2);

  // Checks if data received matches the requested 2-bytes
  if(Wire.available() <= 2){
    // Stores each byte of data from temperature register
    data[0] = Wire.read();
    data[1] = Wire.read();

    // Combines data to make 16-bit binary number
    datac = ((data[0] << 8) | data[1]);

    // Extract mantissa and exponent
    iMantissa = datac & 0x0FFF;
    iExponent = (datac & 0xF000) >> 12;

    // Caculate lux based on (3) in datasheet
    return 0.01 * pow(2, iExponent) * iMantissa;

  }
}

double ReadTempSensor(void){

  // Device addresses
  //int TMP116_Address = 0x48;

  // Data array to store 2-bytes from I2C line
  uint8_t data[2];
  // Combination of 2-byte data into 16-bit data
  int16_t datac;

  // Points to device & begins transmission
  Wire.beginTransmission(TMP116_Address);
  // Points to temperature register to read/write data
  Wire.write(Temp_Reg);
  // Ends data transfer and transmits data from register
  Wire.endTransmission();

  // Delay to allow sufficient conversion time
  delay(10);

  // Requests 2-byte temperature data from device
  Wire.requestFrom(TMP116_Address,2);

  // Checks if data received matches the requested 2-bytes
  if(Wire.available() <= 2){
    // Stores each byte of data from temperature register
    data[0] = Wire.read();
    data[1] = Wire.read();

    // Combines data to make 16-bit binary number
    datac = ((data[0] << 8) | data[1]);

    // Convert to Celcius (7.8125 mC resolution) and return
    return datac*0.0078125;

  }
}
