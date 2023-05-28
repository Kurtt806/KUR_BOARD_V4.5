void restartMCU()
{
    WiFi.mode(WIFI_OFF);
    ESP.restart();
    while (1)
    {
    };
}

void enterLeader()
{
    DEBUG_PRINTLN("[MODE]--- MODE_ANALYSIS");
    KURState::set(MODE_ANALYSIS);
    digitalWrite(BOARD_LED_PIN, LOW);
    initconfig();
    initservo();
    WiFi.mode(WIFI_STA);
    if (ssid == "" || ip == "")
    {
        DEBUG_PRINTLN("[404]--- NO CHANGE SSID OR IP");
        KURState::set(MODE_CONFIG);
    }
    else
    {
        if (!WiFi.config(localIP, localGateway, subnet))
        {
            KURState::set(MODE_CONFIG);
        }
        else
        {
            KURState::set(MODE_CONNECT);
        }
    }
}

void enterConnect()
{
    DEBUG_PRINTLN("[MODE]--- MODE_CONNECT");
    KURState::set(MODE_CONNECT);
    WiFi.begin(ssid.c_str(), pass.c_str());
    unsigned long currentMillis = millis();
    previousMillis = currentMillis;
    while (WiFi.status() != WL_CONNECTED && currentMillis - previousMillis < interval)
    {
        currentMillis = millis();
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        if (OTA == true)
        {
            DEBUG_PRINTLN("[MODE]--- MODE_WAIT_UPDATE");
            serverOTA.begin();
            AsyncElegantOTA.begin(&serverOTA);
            KURState::set(MODE_WAIT_UPDATE);
        }
        else
        {
            DEBUG_PRINTLN("[MODE]--- MODE_RUN");
            serverCAR.begin();
            KURState::set(MODE_RUN);
        }
    }
    else
    {
        DEBUG_PRINTLN("[MODE]--- MODE_CONFIG");
        KURState::set(MODE_CONFIG);
    }
}

void enterRun()
{
    KURState::set(MODE_RUN);
    if (client.connected())
    {
        ESP_Phone();
        if (client.available())
        {
            Phone_ESP();
        }
    }
    else
    {
        for(int i=0; i<5; i++)
        {
            POS_SERVO[i] = 90;
        }
        client = serverCAR.available();
    }
    if(OLED == true)
    {
        display.clear();
        draw_LINEandTEXT();
        draw_INFO_ESP();
        display.display();
    }
}

void enterConfig()
{
    KURState::set(MODE_CONFIG);
    serverCAR.close();
    WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_AP);
    wifimanager();
    DEBUG_PRINTLN("[MODE]--- MODE_WAIT_CONFIG");
    KURState::set(MODE_WAIT_CONFIG);
}

void enterWaitConfig()
{
    KURState::set(MODE_WAIT_CONFIG);
}

void enterOTA()
{
    DEBUG_PRINTLN("[MODE]--- MODE_OTA");
    KURState::set(MODE_OTA);
    serverCAR.close();
    WiFi.mode(WIFI_OFF);
    delay(100);
    OTA = true;
    DEBUG_PRINTLN("[MODE]--- MODE_CONNECT");
    KURState::set(MODE_CONNECT);
}

void enterWaitOTA()
{
    KURState::set(MODE_WAIT_UPDATE);
}

void enterReset()
{
    DEBUG_PRINTLN("[MODE]--- MODE_RESET");
    KURState::set(MODE_RESET);
    restartMCU();
}