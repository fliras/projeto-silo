#ifndef AZ_IOT_HUB_SERVICE_H
#define AZ_IOT_HUB_SERVICE_H

#include "Arduino.h"
#include "src/azure/gerenciador-client-iot-hub/GerenciadorClientIoTHub.h"
#include "src/azure/client-mqtt/ClientMQTTAzure.h"
#include "src/azure/iot-hub-token-manager/AzIoTHubTokenManager.h"
#include "src/device/logger-serial/LoggerSerial.h"
#include "src/device/gerenciador-data-hora/GerenciadorDataHora.h"

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