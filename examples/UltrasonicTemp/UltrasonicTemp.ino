/*
 * HCSR04Ultrasonic/examples/UltrasonicTemp/UltrasonicTemp.ino
 *
 * SVN Keywords
 * ----------------------------------
 * $Author$
 * $Date$
 * $Revision$
 * ----------------------------------
 */

#include <Ultrasonic.h>
#include <OneWire.h>


typedef enum {
  SUCCESS,
  SEARCH_ERROR,
  CRC_ERROR,
  DEVICE_ERROR,
  UNKNOWN_ERROR
} TemperatureCodes;


#define TEMP_PIN    10
#define DS18S20_ID  0x10
#define DS18B20_ID  0x28

float temp;
OneWire ds(TEMP_PIN);


int getTemperature(void) {
  byte present = 0;
  byte data[12];
  byte addr[8];
  TemperatureCodes result = SUCCESS;

  // Find a device
  if(ds.search(addr)) {
    if(OneWire::crc8(addr, 7) == addr[7]) {
      if(addr[0] == DS18S20_ID || addr[0] == DS18B20_ID) {
        ds.reset();
        ds.select(addr);
        // Start conversion
        ds.write(0x44, 1);
        // Wait some time...
        delay(850);
        present = ds.reset();
        ds.select(addr);
        // Issue Read scratchpad command
        ds.write(0xBE);

        // Receive 9 bytes
        for(byte i = 0; i < 9; i++) {
          data[i] = ds.read();
        }

        // Calculate temperature value
        temp = ((data[1] << 8) + data[0]) * 0.0625;
      } else {
        result = DEVICE_ERROR;
      }
    } else {
      result = CRC_ERROR;
    }
  } else {
    result = SEARCH_ERROR;
  }

  ds.reset_search();
  return result;
}


#define TRIGGER_PIN     12
#define ECHO_PIN        13

#define NUMBER_BUFFERS  3
#define BUFFER_SIZE     3

#define BUFFER_01       0
#define BUFFER_02       1
#define BUFFER_03       2
// The max distance to read.
#define MAX_CM_DISTANCE	400
// Set multiplier, used to correct the non-linear result from the sensor.
#define MULTIPLIER      1.1017    // Default to 55 CM

/*
 * The constructors needs to be passed arguments for the trigger pin and
 * echo pin.
 */
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
bool disableSD = false;

// Only run 50 time so we can re-burn the code easily.
#define CYCLES         50
size_t count = 0;

void setup()
  {
  Serial.begin(115200);
  Serial.println("Starting Ultrasonic Test using standard deviation ...");
  // The multiplier must be a float.
  ultrasonic.setMultiplier(MULTIPLIER);
  // The maximum distance that will be detected.
  ultrasonic.setMaxDistance(MAX_CM_DISTANCE);

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
  if(!ultrasonic.sampleCreate(NUMBER_BUFFERS, BUFFER_SIZE, BUFFER_SIZE,
      BUFFER_SIZE))
    {
    disableSD = true;
    Serial.println("Could not allocate memory.");
    }

  // Give time for settings to take effect before starting the loop.
  delay(100);
  }

void loop()
  {
  float cmMsec, inMsec;
  float msStdDev, cmStdDev, inStdDev;
  long microsec = ultrasonic.timing();
  int code = UNKNOWN_ERROR;

  if((code = getTemperature()) != SUCCESS)
    {
    Serial.print("Could not read temperature, return code: ");
    Serial.println(code);
    }

  // The temperature value must be in Celsius.
  ultrasonic.setTemperature(temp);

  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);

  if(count < CYCLES)
    {
    if(disableSD)
      {
      Serial.print("CM: ");
      Serial.print(cmMsec);
      Serial.print(", IN: ");
      Serial.println(inMsec);
      }
    else
      {
      msStdDev = ultrasonic.unbiasedStdDev((float) microsec, BUFFER_01);
      cmStdDev = ultrasonic.unbiasedStdDev(cmMsec, BUFFER_02);
      inStdDev = ultrasonic.unbiasedStdDev(inMsec, BUFFER_03);
      Serial.print(count + 1);
      Serial.print(") MS: ");
      Serial.print(microsec);
      Serial.print(", SD: ");
      Serial.print(msStdDev);
      Serial.print(", CM: ");
      Serial.print(cmMsec);
      Serial.print(", SD: ");
      Serial.print(cmStdDev, 2);
      Serial.print(", IN: ");
      Serial.print(inMsec);
      Serial.print(", SD: ");
      Serial.print(inStdDev, 2);
      Serial.print(", C: ");
      Serial.println(temp);
      }

    count++;
    }
  }
