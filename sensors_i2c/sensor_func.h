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
