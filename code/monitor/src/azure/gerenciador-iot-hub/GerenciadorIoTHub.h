/*
* GerenciadorIoTHub.h:
* Header do GerenciadorIoTHub, classe central para mediar a interação entre Esp32 e Azure IoT Hub.
*/

#ifndef AZ_IOT_HUB_SERVICE_H
#define AZ_IOT_HUB_SERVICE_H

// Inclusão dos Headers
#include "Arduino.h"
#include "src/azure/gerenciador-client-iot-hub/GerenciadorClientIoTHub.h"
#include "src/azure/client-mqtt/ClientMQTTAzure.h"
#include "src/device/logger-serial/LoggerSerial.h"
#include "src/device/gerenciador-data-hora/GerenciadorDataHora.h"


// Constantes utilizadas
#define DURACAO_SAS_TOKEN_EM_MINUTOS 60


/// @brief Protótipo do GerenciadorIoTHub, que deve gerenciar a conexão com o Azure e enviar
/// mensagens com as medições extraídas ou mensagens de Ping, para indicar atividade.
class GerenciadorIoTHub
{
  private:
    GerenciadorClientIoTHub* gerenciadorDoClient;
    ClientMQTTAzure* clientMQTT;
    String montaMensagem(String comando, String payload);
  
  public:
    GerenciadorIoTHub(char* fqdn, char* uriMQTT, char* deviceID, char* deviceKey);
    void configura();
    bool sessaoEstaValidada();
    void enviaTelemetria(int valorTelemetria);
    void enviaPing();
};

#endif