/*
* GerenciadorIoTHub.cpp:
* Implementação das funcionalidades definidas em GerenciadorIoTHub.h
*/

// Inclusão dos Headers
#include "GerenciadorIoTHub.h"


/// @brief Implementação do construtor, que inicializa os atributos da classe.
/// @param fqdn Domínio FQDN do serviço IoT Hub configurado no Azure.
/// @param uriMQTT endereço MQTT configurado no Azure.
/// @param deviceID ID do dispositivo IoT criado no Azure.
/// @param deviceKey chave do dispositivo IoT criado no Azure.
GerenciadorIoTHub::GerenciadorIoTHub(char *fqdn, char *uriMQTT, char *deviceID, char *deviceKey)
{
  this->gerenciadorDoClient = new GerenciadorClientIoTHub(fqdn, deviceID);
  this->clientMQTT = new ClientMQTTAzure(uriMQTT);
}


/// @brief Implementação do método que configura os componentes que interagem com partes específicas do Azure.
void GerenciadorIoTHub::configura()
{
  // inicia o client do IoT Hub caso não esteja em funcionamento.
  if (!gerenciadorDoClient->foiInicializado())
    gerenciadorDoClient->inicializa();

  // caso a sessão não esteja em vigor, tenta gerar outro token.
  if (!sessaoEstaValidada() && !gerenciadorDoClient->geraToken(DURACAO_SAS_TOKEN_EM_MINUTOS))
  {
    Logger.Error("Falha ao gerar um novo token ao revalidar a sessao");
    return;
  }

  // caso o client MQTT já esteja operando, ele é encerrado para que seja iniciado novamente.
  if (clientMQTT->verificaSeEstaConfigurado())
    clientMQTT->encerra();
  (void)clientMQTT->configura(gerenciadorDoClient->obtemID(), gerenciadorDoClient->obtemUsername(), gerenciadorDoClient->obtemToken());
}


/// @brief Implementação do método que verifica se a sessão entre Esp32 e Azure está em vigor, checando a situação do token.
/// @return true: sessão validada; false: sessão não validada.
bool GerenciadorIoTHub::sessaoEstaValidada()
{
  return gerenciadorDoClient->tokenValido();
}


/// @brief Implementação do método que envia ao Azure uma mensagem em JSON com dados de medição.
/// @param valorTelemetria Valor coletado pelo sensor.
void GerenciadorIoTHub::enviaTelemetria(int valorTelemetria)
{
  String payloadTelemetria = "{\"medicao\": " + String(valorTelemetria) + "}"; // Ex: {"medicao": 123}
  String mensagem = montaMensagem("MEDICAO", payloadTelemetria);
  clientMQTT->publica(gerenciadorDoClient->obtemTopicoDaTelemetria(), mensagem);
}


/// @brief Implementação do método que envia pings ao Azure, para indicar que o Esp32 está em atividade.
void GerenciadorIoTHub::enviaPing()
{
  String mensagem = montaMensagem("PING", "{}");
  clientMQTT->publica(gerenciadorDoClient->obtemTopicoDaTelemetria(), mensagem);
}


/// @brief Método para montar mensagens em JSON, em um formato esperado pelo Azure.
/// @param comando Tipo da mensagem, podendo ser "MEDICAO" ou "PING".
/// @param payload Objeto JSON em String com o conteúdo da mensagem que será enviada ao Azure.
/// @return 
String GerenciadorIoTHub::montaMensagem(String comando, String payload)
{
  // Ex: {"idPlaca": "", "comando": "", "timestamp": "", dados: {}}
  return "{\"idPlaca\":\""+ String(gerenciadorDoClient->obtemID()) + "\",\"comando\":\"" + comando + "\",\"timestamp\":\"" + GerenciadorDataHora::obtemTimestampAtual() + "\",\"dados\":" + payload + "}";
}