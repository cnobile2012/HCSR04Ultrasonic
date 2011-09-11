/*
 * HCSR04Ultrasonic/examples/UltrasonicDemo/UltrasonicDemo.pde
 *
 * SVN Keywords
 * ----------------------------------
 * $Author$
 * $Date$
 * $Revision$
 * ----------------------------------
 */

#include <Ultrasonic.h>

#define TRIGGER_PIN  12
#define ECHO_PIN     13

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

void setup()
  {
  Serial.begin(9600);
  }

void loop()
  {
  long cmMsec = 0, inMsec = 0;

  cmMsec = ultrasonic.ranging(Ultrasonic::CM);
  inMsec = ultrasonic.ranging(Ultrasonic::IN);
  Serial.print("CM: ");
  Serial.print(cmMsec);
  Serial.print(", IN: ");
  Serial.println(inMsec);
  delay(1000);
  }
