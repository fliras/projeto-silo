#include "GerenciadorClientIoTHub.h"

GerenciadorClientIoTHub::GerenciadorClientIoTHub(char* host, char* deviceID) {
  this->inicializado = false;
  this->host = host;
  this->deviceID = deviceID;
}

void GerenciadorClientIoTHub::inicializa()
{
  if (!carregaClient())
  {
    Logger.Error("Falha ao iniciar o Client do Azure IOT Hub");
    return;
  }

  if (!carregaIdDoClient())
  {
    Logger.Error("Falha ao obter o Client ID");
    return;
  }

  if (!carregaUsername())
  {
    Logger.Error("Falha ao obter o Username");
    return;
  }

  if (!carregaTopicoDaTelemetria())
  {
    Logger.Error("Falha ao obter o topico MQTT de telemetria");
    return;
  }

  Logger.Info("Client IOT Hub inicializado!");
  Logger.Info("ID: " + String(obtemID()));
  Logger.Info("Username: " + String(obtemUsername()));
  Logger.Info("Topico da telemetria: " + String(obtemTopicoDaTelemetria()));
  inicializado = true;
}

bool GerenciadorClientIoTHub::carregaClient()
{
  az_iot_hub_client_options parametrosDoClient = az_iot_hub_client_options_default();
  parametrosDoClient.user_agent = AZ_SPAN_FROM_STR(AZURE_SDK_CLIENT_USER_AGENT);
  az_result resultado = az_iot_hub_client_init(
    this->obtemClient(),
    az_span_create((uint8_t *)host, strlen(host)),
    az_span_create((uint8_t *)deviceID, strlen(deviceID)),
    &parametrosDoClient);
  return !az_result_failed(resultado);
}

bool GerenciadorClientIoTHub::carregaIdDoClient()
{
  size_t tamanhoIdDoClient;
  az_result resultado = az_iot_hub_client_get_client_id(
    this->obtemClient(),
    id,
    sizeof(id) - 1,
    &tamanhoIdDoClient);
  return !az_result_failed(resultado);
}

bool GerenciadorClientIoTHub::carregaUsername()
{
  az_result resultado = az_iot_hub_client_get_user_name(
    this->obtemClient(),
    username,
    obtemTamanhoDeArray(username),
    NULL);
  return !az_result_failed(resultado);
}

bool GerenciadorClientIoTHub::carregaTopicoDaTelemetria()
{
  az_result resultado = az_iot_hub_client_telemetry_get_publish_topic(
    this->obtemClient(),
    NULL,
    topicoDaTelemetria,
    sizeof(topicoDaTelemetria),
    NULL);
  return !az_result_failed(resultado);
}

az_iot_hub_client* GerenciadorClientIoTHub::obtemClient()
{
  return &client;
}

char* GerenciadorClientIoTHub::obtemID()
{
  return this->id;
}

char* GerenciadorClientIoTHub::obtemUsername()
{
  return this->username;
}

char* GerenciadorClientIoTHub::obtemTopicoDaTelemetria()
{
  return this->topicoDaTelemetria;
}

bool GerenciadorClientIoTHub::foiInicializado()
{
  return inicializado;
}