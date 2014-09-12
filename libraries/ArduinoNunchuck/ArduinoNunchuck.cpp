/*
 * ArduinoNunchuck.cpp - Improved Wii Nunchuck library for Arduino
 *
 * Copyright 2011-2013 Gabriel Bianconi, http://www.gabrielbianconi.com/
 *
 * Project URL: http://www.gabrielbianconi.com/projects/arduinoNunchuck/
 *
 * Based on the following resources:
 *   http://www.windmeadow.com/node/42
 *   http://todbot.com/blog/2008/02/18/wiichuck-wii-nunchuck-adapter-available/
 *   http://wiibrew.org/wiki/Wiimote/Extension_Controllers
 *
 */

#include <Arduino.h>
#include <Wire.h>
#include "ArduinoNunchuck.h"

#define ADDRESS 0x52

void ArduinoNunchuck::init()
{
  Wire.begin();
  ArduinoNunchuck::_sendByte(0x55, 0xF0);
  ArduinoNunchuck::_sendByte(0x00, 0xFB);
  ArduinoNunchuck::update();
}

void ArduinoNunchuck::update()
{
  int count = 0;
  int values[6];

  Wire.requestFrom(ADDRESS, 6);

  while(Wire.available())
  {
    values[count] = Wire.read();
    count++;
  }

  ArduinoNunchuck::analogX = values[0];
  ArduinoNunchuck::analogY = values[1];
  ArduinoNunchuck::accelX = (values[2] << 2) | ((values[5] >> 2) & 3);
  ArduinoNunchuck::accelY = (values[3] << 2) | ((values[5] >> 4) & 3);
  ArduinoNunchuck::accelZ = (values[4] << 2) | ((values[5] >> 6) & 3);
  ArduinoNunchuck::zButton = !((values[5] >> 0) & 1);
  ArduinoNunchuck::cButton = !((values[5] >> 1) & 1);

  ArduinoNunchuck::_sendByte(0x00, 0x00);
}

void ArduinoNunchuck::_sendByte(byte data, byte location)
{
  Wire.beginTransmission(ADDRESS);
  Wire.write(location);
  Wire.write(data);
  Wire.endTransmission();
  delay(10);
}