/*
 * HCSR04Ultrasonic/examples/UltrasonicStdDev/UltrasonicStdDev.pde
 *
 * SVN Keywords
 * ----------------------------------
 * $Author$
 * $Date$
 * $Revision$
 * ----------------------------------
 */

#include <Ultrasonic.h>

#define TRIGGER_PIN    12
#define ECHO_PIN       13

#define NUMBER_BUFFERS 2
#define BUFFER_SIZE    3

#define BUFFER_01      0
#define BUFFER_02      1

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
bool disableSD = false;


void setup()
  {
  Serial.begin(9600);
  Serial.println("Starting Ultasonic Test using standard deviation ...");

  /*
   * If NUMBER_BUFFERS is 2 then it must be followed by two size variables
   * one for each buffer to be created. The size variables do not need to be
   * the same value.
   *
   * Example: ultrasonic.sampleCreate(3, 20, 10, 3) is valid.
   *
   * Note: The minimum size for any buffer is 2. Using less than 2 will waist
   *       resources and the buffer will be ignored.
   */
  if(!ultrasonic.sampleCreate(NUMBER_BUFFERS, BUFFER_SIZE, BUFFER_SIZE))
    {
    disableSD = true;
    Serial.println("Could not allocate memory.");
    }
  }

void loop()
  {
  long cmMsec = 0, inMsec = 0;
  float cmStdDev, inStdDev;
  long microsec = ultrasonic.timing();

  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);

  if(disableSD)
    {
    Serial.print("CM: ");
    Serial.print(cmMsec);
    Serial.print(", IN: ");
    Serial.println(inMsec);
    }
  else
    {
    cmStdDev = ultrasonic.unbiasedStdDev(cmMsec, BUFFER_01);
    inStdDev = ultrasonic.unbiasedStdDev(inMsec, BUFFER_02);
    Serial.print("CM: ");
    Serial.print(cmMsec);
    Serial.print(", SD: ");
    Serial.print(cmStdDev, 2);
    Serial.print(", IN: ");
    Serial.print(inMsec);
    Serial.print(", SD: ");
    Serial.println(inStdDev, 2);
    }

  delay(2000);
  }
