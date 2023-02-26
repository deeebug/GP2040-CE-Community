#ifndef _ENUMS_H_
#define _ENUMS_H_

typedef enum
{
    STATICSPLASH,
    CLOSEIN,
    CLOSEINCUSTOM,
    NOSPLASH,
} SplashMode;

typedef enum
{
    MAIN,
    X,
    Y,
    Z,
	CUSTOM,
	LEGACY
} SplashChoice;

typedef enum
{
	BOARD_LED_OFF,
	MODE_INDICATOR,
	INPUT_TEST
} OnBoardLedMode;

typedef enum
{
	CONFIG_TYPE_WEB = 0,
	CONFIG_TYPE_SERIAL,
	CONFIG_TYPE_DISPLAY
} ConfigType;

#endif
