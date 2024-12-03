#include "GerenciadorIoTHub.h"

GerenciadorIoTHub::GerenciadorIoTHub(char *fqdn, char *uriMQTT, char *deviceID, char *deviceKey)
{
  this->gerenciadorDoClient = new GerenciadorClientIoTHub(fqdn, deviceID);
  this->tokenManager = new AzIoTHubTokenManager(gerenciadorDoClient->obtemClient());
  this->clientMQTT = new ClientMQTTAzure(uriMQTT);
}

void GerenciadorIoTHub::configura()
{
  if (!gerenciadorDoClient->foiInicializado())
    gerenciadorDoClient->inicializa();

  if (!sessaoEstaValidada())
  {
    if (tokenManager->Generate(DURACAO_SAS_TOKEN_EM_MINUTOS) != 0)
    {
      Logger.Error("Falha ao gerar um novo token");
      return;
    }
  }

  if (clientMQTT->verificaSeEstaConfigurado())
    clientMQTT->encerra();
  (void)clientMQTT->configura(gerenciadorDoClient->obtemID(), gerenciadorDoClient->obtemUsername(), tokenManager->Get());
}

bool GerenciadorIoTHub::sessaoEstaValidada()
{
  return tokenManager->IsCreated() && !tokenManager->IsExpired();
}

void GerenciadorIoTHub::enviaTelemetria(String telemetria)
{
  String mensagem = montaMensagem("MEDICAO", telemetria);
  clientMQTT->publica(gerenciadorDoClient->obtemTopicoDaTelemetria(), mensagem);
}

void GerenciadorIoTHub::enviaPing()
{
  String mensagem = montaMensagem("PING", "{}");
  clientMQTT->publica(gerenciadorDoClient->obtemTopicoDaTelemetria(), mensagem);
}

String GerenciadorIoTHub::montaMensagem(String comando, String dados)
{
  // {"idPlaca": "", "comando": "", "timestamp": "", dados: {}}
  return "{\"idPlaca\":\""+ String(gerenciadorDoClient->obtemID()) + "\",\"comando\":\"" + comando + "\",\"timestamp\":\"" + GerenciadorDataHora::obtemTimestampAtual() + "\",\"dados\":" + dados + "}";
}