/**
   LM335Z Temperature Sensor Sketch
    View the datasheet at: http://www.ti.com/lit/ds/symlink/lm335.pdf
*/

#include <PrintEx.h>

#define ANALOG_PIN A0
#define DELAY 5000
#define AVG_CNT 10
#define KELVIN 0
#define CELSIUS 1
#define FARENHEIT 2

StreamEx serial = Serial;
float min, max;
uint16_t lastReading = 0;

void setup()
{
  analogReference(EXTERNAL); // 3V3 Reference Voltage
  Serial.begin(115200);
  serial.printf("Sketch Starting...\n");
  min = calcTemp(calcVoltage(analogRead(ANALOG_PIN)), FARENHEIT);
  max = min;
  delay(10);
}

void loop()
{
  uint16_t reading = readAnalog(ANALOG_PIN);

  float volts   = calcVoltage(reading),
        kelvin  = calcTemp(volts, KELVIN),
        tempC   = calcTemp(volts, CELSIUS),
        tempF   = calcTemp(volts, FARENHEIT);

  if (lastReading != reading) {
    lastReading = reading;
    serial.printf("\n  Analog Reading = %d, Voltage = %.3f\n", reading, volts / 1000.0);
    serial.printf("  Temperature = %fK, %.1f(C), %.1f(F)\n", kelvin, tempC, tempF);
    serial.printf("  Minimum = %.1f, Maximum = %.1f\n", min, max);
  } else {
    serial.printf(".");
  }
  delay(DELAY);
}

void serialEvent(void)
{
  char c = (char)Serial.read();
  switch (c)
  {
    case '\n':
    case '\r':
      return;

    case 'r':
    case 'R':
      min = calcTemp(calcVoltage(analogRead(ANALOG_PIN)), FARENHEIT);
      max = min;
      break;
  }
}

uint16_t readAnalog(uint8_t analogPin)
{
  // serial.printf("  First reading = %i\n", analogRead(analogPin));
  uint16_t reading = 0.0;
  uint8_t i;
  for (i = 0; i < AVG_CNT; i++) {
    reading += analogRead(analogPin);
    delay(5);
  }
  uint16_t average = reading / i;
  return average;
}

float calcVoltage(uint16_t rawAnalog)
{
  return (rawAnalog / 1024.0) * 3300.0;
}

float calcTemp(uint16_t value, uint8_t unit)
{
  float rval = 0.0;
  switch (unit)
  {
    case KELVIN:
      rval = value / 10;
      break;
    case CELSIUS:
      rval = (value / 10) - 273.15;
      break;
    case FARENHEIT:
      rval = ((value / 10) - 273.15) * 9.0 / 5.0 + 32;
      recordMinMax(rval);
      break;
  }
  return rval;
}

void recordMinMax(float value)
{
  if (value < min) min = value;
  else if (value > max) max = value;
}
