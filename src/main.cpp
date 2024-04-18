#include <Arduino.h>
#include "Sht40.h"

Sht40 *sensor1;
Sht40 *sensor2;
float current_num;

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  sensor1 = new Sht40(Mode::HUMIDITY);
  sensor2 = new Sht40(Mode::TEMPERATURE);
}

void loop()
{
  if (sensor1->run(current_num))
    Serial.println(current_num);
  else
    Serial.println("Error1");
  delay(500);
  if (sensor2->run(current_num))
    Serial.println(current_num);
  else
    Serial.println("Error2");
}
