
extern "C"
{
    void app_loop();
    void restartMCU();
}
#if defined(MANAGER_WIFI_NAME) && !defined(MANAGER_WIFI_PASS)
MANAGER_WIFI_PASS = NULL;
#endif
#include <KURsetting.h>
#include "KURresetbutton.h"
#include "KURstate.h"
#include <KURstore.h>
#include <KURmode.h>

inline void KURState::set(State m)
{
    if (state != m && m < MODE_MAX_VALUE)
    {
        state = m;
    }
}


class KUR
{
public:
    void begin()
    {
        Serial.begin(115200);
        if (display.init())
        {
            OLED = true;
        }
        else
        {
            OLED = false;
        }
    }
    void run()
    {
        void runloop();
        switch (KURState::get())
        {
        case MODE_ANALYSIS:
            enterLeader();
            break;
        case MODE_RUN:
            enterRun();
            break;
        case MODE_CONNECT:
            enterConnect();
            break;
        case MODE_CONFIG:
            enterConfig();
            break;
        case MODE_WAIT_CONFIG:
            enterWaitConfig();
            break;
        case MODE_OTA:
            enterOTA();
            break;
        case MODE_WAIT_UPDATE:
            enterWaitOTA();
            break;
        case MODE_RESET:
            enterReset();
            break;
        default:
            enterLeader();
            break;
        }
    }
} KUR;