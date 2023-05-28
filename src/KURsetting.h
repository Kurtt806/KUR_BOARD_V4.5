

#define BOARD_LED_PIN               2
#define BOARD_BUTTON_PIN            0
#define BOARD_BUTTON_ACTIVE_LOW     true


#define BOARD_USER_PIN_1            15
#define BOARD_USER_PIN_2            13


#define BOARD_ADC_PIN_1             32
#define BOARD_ADC_PIN_2             33


#define BOARD_PWM_PIN_1             12
#define BOARD_PWM_PIN_2             14
#define BOARD_PWM_PIN_3             27
#define BOARD_PWM_PIN_4             26
               

#define BUTTON_HOLD_TIME_ACTION     2000
#define BUTTON_PRESS_TIME_ACTION    50

#define REFRESH_DURATION_CONNECT    2000
#define REFRESH_DURATION_DATA       200

#define WAIT_OTA_RESET              300



#define WiFi_Logo_width 16
#define WiFi_Logo_height 16
const uint8_t WiFi_Logo_bits[] PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x08, 0x10, 0xe4, 0x27, 0x10, 0x08,
   0xc0, 0x03, 0x20, 0x04, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01,
   0xc0, 0x03, 0xc0, 0x03, 0x80, 0x01, 0x00, 0x00 };

#define DEBUG_PRINTER       Serial 
   #ifdef KUR_DEBUG
#define DEBUG_PRINT(...)                  \
    {                                     \
        DEBUG_PRINTER.print(__VA_ARGS__); \
    }
#define DEBUG_PRINTLN(...)                  \
    {                                       \
        DEBUG_PRINTER.println(__VA_ARGS__); \
    }
#else
#define DEBUG_PRINT(...) \
    {                    \
    }
#define DEBUG_PRINTLN(...) \
    {                      \
    }
#endif