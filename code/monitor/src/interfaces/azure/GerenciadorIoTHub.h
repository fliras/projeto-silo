#ifndef AZ_IOT_HUB_SERVICE_H
#define AZ_IOT_HUB_SERVICE_H

#include "Arduino.h"
#include "src/interfaces/azure/GerenciadorClientIoTHub.h"
#include "src/interfaces/azure/ClientMQTTAzure.h"
#include "src/interfaces/azure/AzIoTHubTokenManager.h"
#include "src/interfaces/esp32/LoggerSerial.h"
#include "src/interfaces/esp32/GerenciadorDataHora.h"

#define DURACAO_SAS_TOKEN_EM_MINUTOS 60

class GerenciadorIoTHub
{
  private:
    GerenciadorClientIoTHub* gerenciadorDoClient;
    AzIoTHubTokenManager* tokenManager;
    ClientMQTTAzure* clientMQTT;
    String montaMensagem(String comando, String dados);
  
  public:
    GerenciadorIoTHub(char* fqdn, char* uriMQTT, char* deviceID, char* deviceKey);
    void configura();
    bool sessaoEstaValidada();
    void enviaTelemetria(String telemetria);
    void enviaPing();
};

#endif