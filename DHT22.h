
enum DHT22_errors {
  DHT22_ERR_START             = -1,
  DHT22_ERR_RESPONSE          = -2,
  DHT22_ERR_CHECKSUM          = -3,
  DHT22_ERR_NOT_INITIALISED   = -4
};

class DHT22 {
public:
  DHT22();
  int begin(int gpio);
  int read(float *temp_p, float *hum_p);
private:
  enum {
     START_SIGNAL_MS  = 20,
     START_WAIT_US    = 20,
     NUM_TIME_PERIODS = 80,
     MAX_LOOP_POLLS   = 200
  };
  int _gpio;
};

