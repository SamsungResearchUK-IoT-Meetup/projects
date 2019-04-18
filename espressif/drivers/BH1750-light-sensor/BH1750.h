
constexpr uint8_t BH1750_ADDR = 0x23;  // Default I2C address

#define BH1750_ONE_TIME_H_RES_MODE    0x20  // One time measurement at 1 lx resolution (120ms typ)
#define BH1750_ONE_TIME_H_RES_MODE2   0x21  // One time measurement at 0.5 lx resolution (120ms typ)
#define BH1750_ONE_TIME_H_RES_MODE    0x23  // One time measurement at 4 lx resolution (16ms typ)

#define BH1750_ERR_NOT_INITIALISED    -1
#define BH1750_ERR_NO_DATA            -2

class BH1750 {
public:
  BH1750();
  int begin(uint8_t addr);
  int read(int *level);
private:
  uint8_t  _addr;
};

