// One-shot setting sent to config register
I2Cwrite(OPT3001_Address, OPT_Config_Reg, 0xC2, 0x00);
// Request and calculate OPTical lux reading
double OPT_data = ReadOPTSensor();
Serial.print(OPT_data);
Serial.print("\t\t\t");

delay(10);

// One-shot setting sent to config register
I2Cwrite(TMP116_Address, Temp_Config_Reg, 0x0C, 0x20);
// Request and calculate temperature reading
double temp_data = ReadTempSensor();
Serial.println(temp_data);
