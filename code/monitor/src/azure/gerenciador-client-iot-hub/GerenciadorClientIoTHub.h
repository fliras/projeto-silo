#ifndef IOT_HUB_CLIENT_H
#define IOT_HUB_CLIENT_H

#include "src/device/logger-serial/LoggerSerial.h"
#include <az_core.h>
#include <az_iot.h>

#define AZURE_SDK_CLIENT_USER_AGENT "c%2F" AZ_SDK_VERSION_STRING "(ard;esp32)"
#define obtemTamanhoDeArray(a) (sizeof(a) / sizeof(a[0]))

class GerenciadorClientIoTHub
{  
  public:
    GerenciadorClientIoTHub(char* host, char* deviceID);
    void inicializa();
    az_iot_hub_client* obtemClient();
    char* obtemID();
    char* obtemUsername();
    char* obtemTopicoDaTelemetria();
    bool foiInicializado();

  private:
    az_iot_hub_client client;
    char* host;
    char* deviceID;
    char id[128];
    char username[128];
    char topicoDaTelemetria[128];
    bool inicializado;
    bool carregaClient();
    bool carregaIdDoClient();
    bool carregaUsername();
    bool carregaTopicoDaTelemetria();
};

#endif