/*
* AzIoTHubTokenManager.h:
* Header do AzIoTHubTokenManager, utilizado para gerenciar os tokens criados
* para autenticar o Esp32 junto ao Azure IoT Hub.
*
* OBS: A maior parte deste código é externo, disponibilizado pela Microsoft.
* O código original está disponível no repositório oficial:
* https://github.com/Azure/azure-sdk-for-c-arduino/tree/main/examples/Azure_IoT_Hub_ESP32
*/

#ifndef AZIOTSASTOKEN_H
#define AZIOTSASTOKEN_H


// Inclusão dos headers
#include <Arduino.h>
#include <az_iot_hub_client.h>
#include <az_span.h>
#include "parametros.h"


/// @brief Protótipo do AzIoTHubTokenManager que deve criar e gerenciar tokens utilizados no Azure IoT Hub
class AzIoTHubTokenManager
{
public:
  AzIoTHubTokenManager(az_iot_hub_client* client);
  int geraToken(unsigned int expiryTimeInMinutes);
  bool tokenExpirou();
  bool tokenFoiCriado();
  const char* obtemToken();

private:
  az_iot_hub_client* client;
  az_span deviceKey;
  az_span signatureBuffer;
  az_span sasTokenBuffer;
  az_span sasToken;
  uint32_t expirationUnixTime;
};

#endif
