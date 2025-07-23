/*
* GerenciadorClientIoTHub.cpp:
* Implementação das funcionalidades definidas em GerenciadorClientIoTHub.cpp.
*/

// Inclusão dos headers
#include "GerenciadorClientIoTHub.h"


/// @brief Implementação do construtor, que inicializa os atributos da classe.
/// @param host Host FQDN do serviço IoT Hub configurado no Azure.
/// @param deviceID ID do dispositivo IoT criado no Azure.
GerenciadorClientIoTHub::GerenciadorClientIoTHub(char* host, char* deviceID)
{
  this->inicializado = false;
  this->host = host;
  this->deviceID = deviceID;
  this->tokenManager = nullptr;
}


/// @brief Implementação do método que inicializa o client e obtém suas informações no Azure.
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

  this->tokenManager = new AzIoTHubTokenManager(&client);

  Logger.Info("Client IOT Hub inicializado!");
  Logger.Info("ID: " + String(obtemID()));
  Logger.Info("Username: " + String(obtemUsername()));
  Logger.Info("Topico da telemetria: " + String(obtemTopicoDaTelemetria()));
  inicializado = true;
}


/// @brief Implementação do método que monta a instância do client disponibilizado
/// pela biblioteca externa da Microsoft.
/// @return true: Client carregado corretamente; false: Falha no carregamento do Client.
bool GerenciadorClientIoTHub::carregaClient()
{
  az_iot_hub_client_options parametrosDoClient = az_iot_hub_client_options_default();
  parametrosDoClient.user_agent = AZ_SPAN_FROM_STR(AZURE_SDK_CLIENT_USER_AGENT);
  az_result resultado = az_iot_hub_client_init(
    &client,
    az_span_create((uint8_t *)host, strlen(host)),
    az_span_create((uint8_t *)deviceID, strlen(deviceID)),
    &parametrosDoClient);
  return !az_result_failed(resultado);
}


/// @brief Implementação do método que obtém o ID do client junto ao Azure IoT Hub.
/// @return true: Id do Client carregado corretamente; false: Falha no carregamento do Id do client.
bool GerenciadorClientIoTHub::carregaIdDoClient()
{
  size_t tamanhoIdDoClient;
  az_result resultado = az_iot_hub_client_get_client_id(
    &client,
    id,
    sizeof(id) - 1,
    &tamanhoIdDoClient);
  return !az_result_failed(resultado);
}


/// @brief Implementação do método que carrega o UserName do client junto ao Azure IoT Hub.
/// @return true: Username carregado corretamente; false: Falha no carregamento do username.
bool GerenciadorClientIoTHub::carregaUsername()
{
  az_result resultado = az_iot_hub_client_get_user_name(
    &client,
    username,
    obtemTamanhoDeArray(username),
    NULL);
  return !az_result_failed(resultado);
}


/// @brief Implementação do método que carrega o tópico MQTT para publicar as mensagens no Azure.
/// @return true: Tópico carregado corretamente; false: Falha no carregamento do tópico.
bool GerenciadorClientIoTHub::carregaTopicoDaTelemetria()
{
  az_result resultado = az_iot_hub_client_telemetry_get_publish_topic(
    &client,
    NULL,
    topicoDaTelemetria,
    sizeof(topicoDaTelemetria),
    NULL);
  return !az_result_failed(resultado);
}


/// @brief Implementação do tópico que retorna o token gerenciado pelo Client.
/// @return Token do IoTHub.
const char* GerenciadorClientIoTHub::obtemToken()
{
  return this->tokenManager->obtemToken();
}


/// @brief Implementação do método que cria um novo token do IoTHub.
/// @param expiracaoEmMinutos Tempo expiração do token, em minutos.
/// @return true: Token criado com sucesso; false: Falha na criação do token.
bool GerenciadorClientIoTHub::geraToken(unsigned int expiracaoEmMinutos)
{
  return tokenManager->geraToken(expiracaoEmMinutos) == 0;
}


/// @brief Implementação do método para checar a validade do token atual.
/// @return true: Token válido; false: Token inválido.
bool GerenciadorClientIoTHub::tokenValido()
{
  return tokenManager->tokenFoiCriado() && !tokenManager->tokenExpirou();
}


/// @brief Implementação do método para retornar o ID do Client.
/// @return Id do Client
char* GerenciadorClientIoTHub::obtemID()
{
  return this->id;
}


/// @brief Implementação do método para retornar o username do Client.
/// @return Username do Client
char* GerenciadorClientIoTHub::obtemUsername()
{
  return this->username;
}


/// @brief Implementação do método para retornar o tópico MQTT utilizado para publicar as mensagens no Azure.
/// @return Tópico MQTT
char* GerenciadorClientIoTHub::obtemTopicoDaTelemetria()
{
  return this->topicoDaTelemetria;
}


/// @brief Implementação do método utilizado para checar se o Client já foi inicializado.
/// @return true: Client inicializado; false: Client ainda não inicializado.
bool GerenciadorClientIoTHub::foiInicializado()
{
  return inicializado;
}