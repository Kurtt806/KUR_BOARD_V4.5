
#include "SPIFFS.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include "SSD1306Wire.h"
#include "Ticker.h"
#include <ESP32Servo.h>
//---WIFI------------------------------------
AsyncWebServer serverOTA(80);
WiFiServer serverCAR(80);
WiFiClient client;
const char *PARAM_INPUT_1 = "ssid";
const char *PARAM_INPUT_2 = "pass";
const char *PARAM_INPUT_3 = "ip";
const char *PARAM_INPUT_4 = "gateway";
String ssid;
String pass;
String ip;
String gateway;
const char *ssidPath = "/ssid.txt";
const char *passPath = "/pass.txt";
const char *ipPath = "/ip.txt";
const char *gatewayPath = "/gateway.txt";
IPAddress localIP;
IPAddress subnet(255, 255, 0, 0);
IPAddress localGateway;
//---SERVO-----------------------------------
String C1;
String C2;
String C3;
String C4;
const char *C1Path = "/C1.txt";
const char *C2Path = "/C2.txt";
const char *C3Path = "/C3.txt";
const char *C4Path = "/C4.txt";
const char *w_ssid = MANAGER_WIFI_NAME;
const char *w_pass = MANAGER_WIFI_PASS;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
int POS_SERVO_1;
int POS_SERVO_2;
int POS_SERVO_3;
int POS_SERVO_4;

SSD1306Wire display(0x3c, SDA, SCL);

unsigned long previousMillis = 0;
const int interval = 6000;
unsigned long time_DATA = 0;
unsigned long time_CONNECT = 0;
unsigned long time_TEST = 0;
unsigned long currentMillis = millis();
char cmd[100];
int cmdIndex;
unsigned long lastCmdTime = 0;
unsigned long aliveSentTime = 0;

int PROTECT_KEY[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

bool OLED = false;

void initSPIFFS()
{
    if (!SPIFFS.begin(true))
    {
        DEBUG_PRINTLN("[STORE]--- NOT FOUND FLASH");
    }
    DEBUG_PRINTLN("[STORE]--- FLASH DONE");
}
String readFile(fs::FS &fs, const char *path)
{
    File file = fs.open(path);
    if (!file || file.isDirectory())
    {
        return String();
    }
    String fileContent;
    while (file.available())
    {
        fileContent = file.readStringUntil('\n');
        break;
    }
    return fileContent;
}
void writeFile(fs::FS &fs, const char *path, const char *message)
{
    File file = fs.open(path, FILE_WRITE);

    if (!file)
    {
        return;
    }
    if (file.print(message))
    {
        DEBUG_PRINTLN("[STORE]--- WRITE DONE");
    }
    else
    {
        DEBUG_PRINTLN("[STORE]--- WRITE FAIL");
    }
}
void wifimanager()
{
    DEBUG_PRINTLN("[STORE]--- WIFI AP START");
    WiFi.softAP(w_ssid, w_pass);
    IPAddress IP = WiFi.softAPIP();
    DEBUG_PRINT("[STORE]--- IP: ");
    DEBUG_PRINTLN(IP);
    // Web Server Root URL
    serverOTA.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(SPIFFS, "/wifimanager.html", "text/html"); });
    serverOTA.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(SPIFFS, "/style.css", "text/css"); });
    serverOTA.on("/", HTTP_POST, [](AsyncWebServerRequest *request)
                 {
                  int params = request->params();
                  for (int i = 0; i < params; i++)
                  {
                      AsyncWebParameter *p = request->getParam(i);
                      if (p->isPost())
                      {
                          // HTTP POST ssid value
                          if (p->name() == PARAM_INPUT_1)
                          {
                              ssid = p->value().c_str();
                              writeFile(SPIFFS, ssidPath, ssid.c_str());
                              DEBUG_PRINTLN();
                              DEBUG_PRINTLN("[STORE]--- WRITE TO SSID");
                              DEBUG_PRINTLN(ssid);
                          }
                          // HTTP POST pass value
                          if (p->name() == PARAM_INPUT_2)
                          {
                              pass = p->value().c_str();
                              writeFile(SPIFFS, passPath, pass.c_str());
                              DEBUG_PRINTLN("[STORE]--- WRITE TO PASS");
                              DEBUG_PRINTLN(pass);
                          }
                          // HTTP POST ip value
                          if (p->name() == PARAM_INPUT_3)
                          {
                              ip = p->value().c_str();
                              writeFile(SPIFFS, ipPath, ip.c_str());
                              DEBUG_PRINTLN("[STORE]--- WRITE TO IP");
                          }
                          // HTTP POST gateway value
                          if (p->name() == PARAM_INPUT_4)
                          {
                              gateway = p->value().c_str();
                              writeFile(SPIFFS, gatewayPath, gateway.c_str());
                              DEBUG_PRINTLN("[STORE]--- WRITE TO GW");
                          }
                      }
                  }
                  request->send(200, "text/plain", "Done. ESP will restart");
                  delay(3000);
                  ESP.restart(); });
    serverOTA.begin();
}

void initservo()
{
    DEBUG_PRINTLN("[STORE]---initservo");
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    servo1.setPeriodHertz(Hz); // Standard 50hz servo
    servo2.setPeriodHertz(Hz); // Standard 50hz servo
    servo3.setPeriodHertz(Hz); // Standard 50hz servo
    servo4.setPeriodHertz(Hz); // Standard 50hz servo
    servo1.attach(BOARD_PWM_PIN_1, MIN_US, MAX_US);
    servo2.attach(BOARD_PWM_PIN_2, MIN_US, MAX_US);
    servo3.attach(BOARD_PWM_PIN_3, MIN_US, MAX_US);
    servo4.attach(BOARD_PWM_PIN_4, MIN_US, MAX_US);
}

void initconfig()
{
    DEBUG_PRINTLN("[STORE]---initconfig");
    pinMode(BOARD_LED_PIN, OUTPUT);
    digitalWrite(BOARD_LED_PIN, LOW);
    initSPIFFS();
    initservo();
    C1 = readFile(SPIFFS, C1Path);
    C2 = readFile(SPIFFS, C2Path);
    C3 = readFile(SPIFFS, C3Path);
    C4 = readFile(SPIFFS, C4Path);
    ssid = readFile(SPIFFS, ssidPath);
    pass = readFile(SPIFFS, passPath);
    ip = readFile(SPIFFS, ipPath);
    gateway = readFile(SPIFFS, gatewayPath);
    localIP.fromString(ip.c_str());
    localGateway.fromString(gateway.c_str());
    DEBUG_PRINT("[1]");
    DEBUG_PRINTLN(ssid);
    DEBUG_PRINT("[2]");
    DEBUG_PRINTLN(pass);
    DEBUG_PRINT("[3]");
    DEBUG_PRINTLN(ip);
    DEBUG_PRINT("[4]");
    DEBUG_PRINTLN(gateway);
}

void Servo_Control()
{
    servo1.write(POS_SERVO_1);
    servo2.write(POS_SERVO_2);
    servo3.write(POS_SERVO_3);
    servo4.write(POS_SERVO_4);
}

void draw_INFO_ESP()
{
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(2, 0, "KUR 4.5");
    display.drawString(50, 0, StateStr[KURState::get()]);
    display.setFont(ArialMT_Plain_10);
}
void draw_LINEandTEXT()
{
    // display.drawXbm(0, 54, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
    display.drawHorizontalLine(0, 12, 128); /*  _  */
    display.drawHorizontalLine(50, 40, 90); /*  _  */
    display.drawVerticalLine(48, 0, 120);   /*  |  */

    display.drawHorizontalLine(0, 30, 48); /*  _  */
    display.drawHorizontalLine(0, 42, 48); /*  _  */
    display.drawHorizontalLine(0, 54, 48); /*  _  */
    display.drawVerticalLine(24, 30, 82);  /*  |  */
}

boolean cmdStartsWith(const char *st)
{
    for (int i = 0;; i++)
    {
        if (st[i] == 0)
            return true;
        if (cmd[i] == 0)
            return false;
        if (cmd[i] != st[i])
            return false;
        ;
    }
    return false;
}

void thuchienlenh()
{
    lastCmdTime = millis();
    if (cmdStartsWith("CH1"))
    {
        int ch = cmd[2] - '0';
        if (ch >= 0 && ch <= 9 && cmd[3] == ' ')
        {
            POS_SERVO_1 = (int)atof(cmd + 4);
            DEBUG_PRINT("[CH1]--- ");
            DEBUG_PRINTLN(POS_SERVO_1);
        }
    }
    if (cmdStartsWith("CH2"))
    {
        int ch = cmd[2] - '0';
        if (ch >= 0 && ch <= 9 && cmd[3] == ' ')
        {
            POS_SERVO_2 = (int)atof(cmd + 4);
            DEBUG_PRINT("[CH2]--- ");
            DEBUG_PRINTLN(POS_SERVO_2);
        }
    }
    if (cmdStartsWith("CH3"))
    {
        int ch = cmd[2] - '0';
        if (ch >= 0 && ch <= 9 && cmd[3] == ' ')
        {
            POS_SERVO_3 = (int)atof(cmd + 4);
            DEBUG_PRINT("[CH3]--- ");
            DEBUG_PRINTLN(POS_SERVO_3);
        }
    }
    if (cmdStartsWith("CH4"))
    {
        int ch = cmd[2] - '0';
        if (ch >= 0 && ch <= 9 && cmd[3] == ' ')
        {
            POS_SERVO_4 = (int)atof(cmd + 4);
            DEBUG_PRINT("[CH4]--- ");
            DEBUG_PRINTLN(POS_SERVO_4);
        }
    }
    if (cmdStartsWith("POS1"))
    {
            PROTECT_KEY[4] = 0;
            C1 = POS_SERVO_1;
            writeFile(SPIFFS, C1Path, C1.c_str());
    }
    if (cmdStartsWith("POS2"))
    {
            C2 = POS_SERVO_2;
            writeFile(SPIFFS, C2Path, C2.c_str());
    }
    if (cmdStartsWith("reset"))
    {
        PROTECT_KEY[1] = PROTECT_KEY[1] + 1;
        if (PROTECT_KEY[1] >= 5)
        {
            PROTECT_KEY[1] = 0;
            KURState::set(MODE_RESET);
        }
    }
    if (cmdStartsWith("manager"))
    {
        PROTECT_KEY[2] = PROTECT_KEY[2] + 1;
        if (PROTECT_KEY[2] >= 5)
        {
            PROTECT_KEY[2] = 0;
            KURState::set(MODE_CONFIG);
        }
    }
    if (cmdStartsWith("ota"))
    {
        PROTECT_KEY[3] = PROTECT_KEY[3] + 1;
        if (PROTECT_KEY[3] >= 5)
        {
            PROTECT_KEY[3] = 0;
            KURState::set(MODE_OTA);
        }
    }
    if (cmdStartsWith("alive"))
    {
        aliveSentTime = millis();
    }
}

void Phone_ESP()
{
    char c = (char)client.read();
    if (c == '\n')
    {
        cmd[cmdIndex] = 0;
        thuchienlenh();
        cmdIndex = 0;
    }
    else
    {
        cmd[cmdIndex] = c;
        if (cmdIndex < 10)
        {
            cmdIndex++;
        }
    }
}

void ESP_Phone()
{
    //     if (millis() - time_DATA > REFRESH_DURATION_DATA)
    //     {
    //         char CH1[4];
    //         itoa(POS_SERVO_1, CH1, 10);
    //         client.write("C1 ");
    //         client.write(CH1);
    //         client.write("\n");
    //         char CH2[4];
    //         itoa(POS_SERVO_2, CH2, 10);
    //         client.write("C2 ");
    //         client.write(CH2);
    //         client.write("\n");
    //         time_DATA = millis();
    //     }
    if (millis() - time_CONNECT > REFRESH_DURATION_CONNECT)
    {
        int ssi = WiFi.RSSI();
        char ssi2[4];
        itoa(ssi, ssi2, 10);
        client.write("x ");
        client.write(ssi2);
        client.write("\n");
        client.write("ON on\n");
        time_CONNECT = millis();
    }
}

static inline String macToString(byte mac[6])
{
    char buff[20];
    snprintf(buff, sizeof(buff), "%02x:%02x:%02x:%02x:%02x:%02x",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(buff);
}
static String getWiFiMacAddress()
{
    return WiFi.macAddress();
}