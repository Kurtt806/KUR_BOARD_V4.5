#include <Arduino.h>
#define KUR_DEBUG

/*###################################################################*/
#define MANAGER_WIFI_NAME "KUR_DEV_1"
#define MANAGER_WIFI_PASS "12345678"

const int MIN_US    = 500;
const int MID_US    = 1500;
const int MAX_US    = 2500;
const int Hz        = 50;
int POS_SERVO_1     = 90;
int POS_SERVO_2     = 90;
int POS_SERVO_3     = 90;
int POS_SERVO_4     = 90;
#include <Kuredge.h>

void setup()
{
  KUR.begin();
}
void loop()
{
   KUR.run();
}