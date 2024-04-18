#ifndef SHT40_H
#define SHT40_H

#include <Arduino.h>
#include <Wire.h>

#define NO_ERROR 0

enum Mode
{
    TEMPERATURE = 0,
    HUMIDITY = 1
};

class Sht40
{
private:
    const int8_t buffer = 6;
    const int8_t i2c_address = 0x44;
    Mode mode;
    int rx_bytes[6];

public:
    Sht40(Mode mode);
    ~Sht40();
    bool run(float &current_num);
    float getTemperature();
    float getHumidity();
};

Sht40::Sht40(Mode mode)
{
    this->mode = mode;
}

Sht40::~Sht40()
{
}

bool Sht40::run(float &current_num)
{
    Wire.beginTransmission(i2c_address);
    Wire.write(0xFD);
    if (Wire.endTransmission() != 0)
    {
        Serial.println("Error: I2C communication failed");
        return false;
    }

    delay(10);

    Wire.requestFrom(i2c_address, buffer);
    if (Wire.available() != buffer)
    {
        Serial.println("Error: Data read failed");
        return false;
    }

    for (size_t i = 0; i < buffer; i++)
    {
        rx_bytes[i] = Wire.read();
        Serial.print(rx_bytes[i]);
        Serial.print(",");
    }
    Serial.println();

    if (this->mode == TEMPERATURE)
        current_num = getTemperature();
    else if (this->mode == HUMIDITY)
        current_num = getHumidity();

    return true;
}

float Sht40::getTemperature()
{
    float t_ticks = rx_bytes[0] * 256 + rx_bytes[1];
    float result = 175 * t_ticks / 65535 - 45;
    return result;
}

float Sht40::getHumidity()
{
    float h_ticks = rx_bytes[3] * 256 + rx_bytes[4];
    float result = 125 * h_ticks / 65535 - 6;
    if (result <= 0.0)
        return 0.0;
    if (result >= 100.0)
        return 100.0;
    return result;
}

#endif
