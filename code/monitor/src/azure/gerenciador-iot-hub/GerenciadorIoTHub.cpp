#include "GerenciadorIoTHub.h"

GerenciadorIoTHub::GerenciadorIoTHub(char* fqdn, char* uriMQTT, char* deviceID, char* deviceKey)
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

void GerenciadorIoTHub::enviaTelemetria(String mensagem)
{
  clientMQTT->publica(gerenciadorDoClient->obtemTopicoDaTelemetria(), mensagem);
}