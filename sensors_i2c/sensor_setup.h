Wire.begin(21,22);

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

Serial.println("Light Intensity (Lux)\tTemperature (C)");

//  alarm = digitalRead(2);
