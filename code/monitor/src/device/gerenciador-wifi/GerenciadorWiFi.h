#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include "src/device/logger-serial/LoggerSerial.h"

class GerenciadorWiFi
{
  private:
    WiFiManager wifiManager;
    char* ssidAP;
    char* senhaAP;

  public:
    GerenciadorWiFi(char* ssidAP, char* senhaAP);
    void conecta();
    void desconecta();
    bool estaConectado();
    String obtemIP();
};