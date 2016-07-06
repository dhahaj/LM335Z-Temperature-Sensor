/**
   LM335Z Temperature Sensor Sketch
    -- datasheet located at: http://www.ti.com/lit/ds/symlink/lm335.pdf
*/

#include <PrintEx.h>

#define ANALOG_PIN A0
#define DELAY 3000

StreamEx serial = Serial;

void setup()
{
  analogReference(EXTERNAL); // 3V3 Reference Voltage
  Serial.begin(9600);
  serial.print("Sketch Starting...\n");
}

void loop()
{
  int reading = analogRead(ANALOG_PIN);

  float volts = (reading / 1024.0) * 3300.0;
  float kelvin = (volts / 10);
  float tempC = kelvin - 273.15;
  float tempF = ((tempC * 9.0) / 5.0 + 32);

  serial.printf("  Analog Reading = %d, Voltage = %.3f\n", reading, volts / 1000.0);
  serial.printf("  Temperature = %fK, %.1f(C), %.1f(F)\n\n", kelvin, tempC, tempF);

  delay(DELAY);
}
