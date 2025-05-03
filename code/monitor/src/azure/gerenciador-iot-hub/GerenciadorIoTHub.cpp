#include "GerenciadorIoTHub.h"

GerenciadorIoTHub::GerenciadorIoTHub(char *fqdn, char *uriMQTT, char *deviceID, char *deviceKey)
{
  this->gerenciadorDoClient = new GerenciadorClientIoTHub(fqdn, deviceID);
  this->clientMQTT = new ClientMQTTAzure(uriMQTT);
}

void GerenciadorIoTHub::configura()
{
  if (!gerenciadorDoClient->foiInicializado())
    gerenciadorDoClient->inicializa();

  if (!sessaoEstaValidada())
  {
    if (!gerenciadorDoClient->geraToken(DURACAO_SAS_TOKEN_EM_MINUTOS))
    {
      Logger.Error("Falha ao gerar um novo token");
      return;
    }
  }

  if (clientMQTT->verificaSeEstaConfigurado())
    clientMQTT->encerra();
  (void)clientMQTT->configura(gerenciadorDoClient->obtemID(), gerenciadorDoClient->obtemUsername(), gerenciadorDoClient->obtemToken());
}

bool GerenciadorIoTHub::sessaoEstaValidada()
{
  return gerenciadorDoClient->tokenValido();
}

void GerenciadorIoTHub::enviaTelemetria(int valorTelemetria)
{
  String payloadTelemetria = "{\"medicao\": " + String(valorTelemetria) + "}";
  String mensagem = montaMensagem("MEDICAO", payloadTelemetria);
  clientMQTT->publica(gerenciadorDoClient->obtemTopicoDaTelemetria(), mensagem);
}

void GerenciadorIoTHub::enviaPing()
{
  String mensagem = montaMensagem("PING", "{}");
  clientMQTT->publica(gerenciadorDoClient->obtemTopicoDaTelemetria(), mensagem);
}

String GerenciadorIoTHub::montaMensagem(String comando, String payload)
{
  // {"idPlaca": "", "comando": "", "timestamp": "", dados: {}}
  return "{\"idPlaca\":\""+ String(gerenciadorDoClient->obtemID()) + "\",\"comando\":\"" + comando + "\",\"timestamp\":\"" + GerenciadorDataHora::obtemTimestampAtual() + "\",\"dados\":" + payload + "}";
}