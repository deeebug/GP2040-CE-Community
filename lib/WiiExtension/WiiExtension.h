// WiiNunchuk Library
// category=Signal Input/Output

#ifndef _WIIEXTENSION_H_
#define _WIIEXTENSION_H_

#include <BitBang_I2C.h>

#define WII_EXTENSION_NONE          -1
#define WII_EXTENSION_NUNCHUCK      0
#define WII_EXTENSION_CLASSIC       1
#define WII_EXTENSION_CLASSIC_PRO   2
#define WII_EXTENSION_DRAWSOME      3
#define WII_EXTENSION_GUITAR        4
#define WII_EXTENSION_DRUMS         5
#define WII_EXTENSION_TURNTABLE     6
#define WII_EXTENSION_TAIKO         7
#define WII_EXTENSION_UDRAW         8
#define WII_EXTENSION_BALANCE_BOARD 9
#define WII_EXTENSION_MOTION_PLUS   10

#define WII_DATA_TYPE_0             0
#define WII_DATA_TYPE_1             1
#define WII_DATA_TYPE_2             2
#define WII_DATA_TYPE_3             3

#define WII_ENCRYPTION false

#ifndef HAS_WII_EXTENSION
#define HAS_WII_EXTENSION 1
#endif

#ifndef WII_EXTENSION_DEBUG
#define WII_EXTENSION_DEBUG false
#endif

#ifndef WII_EXTENSION_DELAY
#define WII_EXTENSION_DELAY 100
#endif

#ifndef WII_EXTENSION_I2C_ADDR
#define WII_EXTENSION_I2C_ADDR 0x52
#endif

#ifndef WII_EXTENSION_I2C_SDA_PIN
#define WII_EXTENSION_I2C_SDA_PIN 16
#endif

#ifndef WII_EXTENSION_I2C_SCL_PIN
#define WII_EXTENSION_I2C_SCL_PIN 17
#endif

#ifndef WII_EXTENSION_I2C_BLOCK
#define WII_EXTENSION_I2C_BLOCK i2c0
#endif

#ifndef WII_EXTENSION_I2C_SPEED
#define WII_EXTENSION_I2C_SPEED 400000
#endif

class WiiExtension {
  protected:
	uint8_t address;
  public:
    int8_t extensionType = WII_EXTENSION_NONE;
    int8_t dataType = WII_DATA_TYPE_0;

    int16_t joy1X        = 0;
    int16_t joy1Y        = 0;
    int16_t joy2X        = 0;
    int16_t joy2Y        = 0;
    int16_t accelX       = 0;
    int16_t accelY       = 0;
    int16_t accelZ       = 0;

    bool buttonZ         = false;
    bool buttonC         = false;
    bool buttonZR        = false;
    bool buttonZL        = false;
    bool buttonA         = false;
    bool buttonB         = false;
    bool buttonX         = false;
    bool buttonY         = false;
    bool buttonPlus      = false;
    bool buttonHome      = false;
    bool buttonMinus     = false;
    bool buttonLT        = false;
    bool buttonRT        = false;

    bool directionUp     = false;
    bool directionDown   = false;
    bool directionLeft   = false;
    bool directionRight  = false;

    int16_t triggerLeft  = 0;
    int16_t triggerRight = 0;

    bool fretGreen       = false;
    bool fretRed         = false;
    bool fretYellow      = false;
    bool fretBlue        = false;
    bool fretOrange      = false;

    int16_t whammyBar    = 0;

    bool isReady         = false;

    // Constructor 
	WiiExtension(int bWire, int sda, int scl, i2c_inst_t *picoI2C, int32_t iSpeed, uint8_t addr = WII_EXTENSION_I2C_ADDR);

    // Methods
    void begin();
	void reset();
  	void start();
  	void poll();
  private:
	
	BBI2C bbi2c;
	int32_t iSpeed;
	uint8_t config;
	bool singleShot;
	int data_ready;
	unsigned char uc[128];

    int8_t doWait = 0;

    int8_t _minX;
    int8_t _maxX;
    int8_t _cenX;

    int8_t _minY;
    int8_t _maxY;
    int8_t _cenY;

    int8_t _accelX0G;
    int8_t _accelY0G;
    int8_t _accelZ0G;
    int8_t _accelX1G;
    int8_t _accelY1G;
    int8_t _accelZ1G;

    int8_t decodeByte(int8_t val);
};

#endif
