/*
* GerenciadorWiFi.h:
* Header do GerenciadorWiFi, utilizado para pré-declarar os componentes que gerenciam a conexão de rede.
*/

// Inclusão dos Headers
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include "src/device/logger-serial/LoggerSerial.h"


/// @brief Protótipo do GerenciadorWiFi, que deve conectar e desconectar de uma rede WiFi, bem como
/// monitorar o status de conexão quando solicitado.
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