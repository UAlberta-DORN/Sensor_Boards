#include "sensor_address.h"
#include "sensor_func.h"

void setup() {
  Serial.begin(115200);
  #include "sensor_setup.h"
}

void loop() {
  #include "sensor_loop.h"
  delay(500);
}


void alert(){

//  alarm = digitalRead(2);

}
