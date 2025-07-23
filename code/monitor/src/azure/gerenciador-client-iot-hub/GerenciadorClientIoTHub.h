/*
* GerenciadorClientIoTHub.h:
* Header do GerenciadorClientIoTHub, que define as funcionalidades da classe responsável por interagir diretamente com o Azure como um client.
*/

#ifndef IOT_HUB_CLIENT_H
#define IOT_HUB_CLIENT_H


// Inclusão dos Headers
#include "src/device/logger-serial/LoggerSerial.h"
#include "src/azure/iot-hub-token-manager/AzIoTHubTokenManager.h"
#include <az_core.h>
#include <az_iot.h>


// Constantes utilizadas
#define AZURE_SDK_CLIENT_USER_AGENT "c%2F" AZ_SDK_VERSION_STRING "(ard;esp32)" // Constante utilizada pela biblioteca externa da Microsoft
#define obtemTamanhoDeArray(a) (sizeof(a) / sizeof(a[0])) // Método autilizar


/// @brief Protótipo do GerenciadorClientIoTHub.
class GerenciadorClientIoTHub
{  
  public:
    GerenciadorClientIoTHub(char* host, char* deviceID);
    void inicializa();
    const char* obtemToken();
    bool geraToken(unsigned int expiracaoEmMinutos);
    bool tokenValido();
    char* obtemID();
    char* obtemUsername();
    char* obtemTopicoDaTelemetria();
    bool foiInicializado();

  private:
    AzIoTHubTokenManager* tokenManager;
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