//
// Support for the DHT22 temperature and humidity sensor
//

#include "Arduino.h"
#include "DHT22.h"

DHT22::DHT22()
  : _gpio(-1)
{}

int DHT22::begin(int gpio)
{
  _gpio = gpio;
  pinMode(_gpio, INPUT);            // default quiescent state
  return 0;
}

int DHT22::read(float *temp_p, float *hum_p)
{
  if (_gpio < 0)
    return DHT22_ERR_NOT_INITIALISED;

  // Pull pin low for 20ms to activate

  pinMode(_gpio, OUTPUT);
  digitalWrite(_gpio, 0);
  delay(START_SIGNAL_MS);

  // Pull pin high and wait 20uS for responses

  digitalWrite(_gpio, 1);
  delayMicroseconds(START_WAIT_US);
  pinMode(_gpio, INPUT);

  // Wait for 80uS low, 80uS high response

  int i;
  int poll_count;
  for (i = 0 ; i < 3 ; ++i) {
    poll_count = MAX_LOOP_POLLS;
    int val = (i + 1) & 1;
    while (digitalRead(_gpio) == val && poll_count-- > 0)
      delayMicroseconds(1);
  }

  if (i < 3 || poll_count <= 0) {
    Serial.println("Error waiting for DHT22 start response");
    return DHT22_ERR_START;
  }

  //
  // Read 40 bits of humidity and temperature data and checksum
  //
  
  int last_val = 0; 
  uint8_t times[NUM_TIME_PERIODS];
  for (i = 0 ; i < NUM_TIME_PERIODS ; ++i) {
    times[i] = 0;
    poll_count = MAX_LOOP_POLLS;
    for (;;) {
      int val = digitalRead(_gpio);
      if (val != last_val) {
        last_val = val;
        times[i] = MAX_LOOP_POLLS - poll_count;
        break;
      }
      if (poll_count-- <= 0) {
        break;
      }
      delayMicroseconds(1);
    }
  }

  //
  // The above sequence could take upto ~5ms, so yield
  // 
  yield();
  
  if (i < NUM_TIME_PERIODS || poll_count <= 0) {
    Serial.println("Error reading DHT22 response bits");
    return DHT22_ERR_RESPONSE;
  }

  //
  // Take average of the 50uS low periods between bits
  //

  int n = 0;
  for (i = 0 ; i < NUM_TIME_PERIODS ; i += 2) {
    n += times[i];
  }
  n /= (NUM_TIME_PERIODS / 2);

  //
  // Decode bits according to whether the high periods are greater than or less than the 50uS average.
  // TODO: If we are too close to the average, we should really return an error.
  //

  uint8_t bytes[5] = { 0 };
  for (int i = 1 ; i < NUM_TIME_PERIODS ; i += 2) {
    int index = i >> 4;
    bytes[index] <<= 1;
    if (times[i] > n)
      bytes[index] |= 1;
  }

  //
  // Calculate and verify the checksum
  //
  uint8_t sum = (bytes[0] + bytes[1] + bytes[2] + bytes[3]) & 0xff;
  if (sum != bytes[4]) {
    Serial.printf("DHT22 checksum failure - %d != %d\n", sum, bytes[4]);
    return DHT22_ERR_CHECKSUM;
  }

  //
  // Return the values
  //
  if (hum_p)
    *hum_p = static_cast<float>(((bytes[0] << 8) | bytes[1])) / 10;
  if (temp_p)
    *temp_p = static_cast<float>(((bytes[2] << 8) | bytes[3])) / 10;
  return 0;
}

