#include <Arduino.h>
//#define KUR_DEBUG

/*###################################################################*/
// #define MANAGER_WIFI_NAME "KUR_DEV (Binh)"
// #define MANAGER_WIFI_PASS "12345678"
/*###################################################################*/
// #define MANAGER_WIFI_NAME "KUR_DEV (Dinh)"
// #define MANAGER_WIFI_PASS "12345678"
// /*###################################################################*/
// #define MANAGER_WIFI_NAME "KUR_DEV (Toan)"
// #define MANAGER_WIFI_PASS "12345678"
// /*###################################################################*/
// #define MANAGER_WIFI_NAME "KUR_DEV (Hieu)"
// #define MANAGER_WIFI_PASS "12345678"
// /*###################################################################*/
#define MANAGER_WIFI_NAME "KUR_DEV (Nhiem)"
#define MANAGER_WIFI_PASS "11111111"
// /*###################################################################*/
// #define MANAGER_WIFI_NAME "KUR_DEV (Bao)"
// #define MANAGER_WIFI_PASS "12345678"
// /*###################################################################*/

const int MIN_US    = 500;
const int MAX_US    = 2500;
const int Hz        = 50;

#include <Kuredge.h>
void setup()
{
  KUR.begin();
}
void loop()
{           
   KUR.run();
}             




