#include "ClientMQTTAzure.h"
#include <azure_ca.h>

static esp_mqtt_client_handle_t clientMQTT;
static char DADOS_RECEBIDOS[QTDE_BYTES_BUFFER_DE_DADOS_MQTT];

ClientMQTTAzure::ClientMQTTAzure(char* uriMQTT): uriMQTT(uriMQTT), estaConfigurado(estaConfigurado) {}

int ClientMQTTAzure::configura(char* clientID, char* username, const char* sasToken)
{
  memset(&configsMQTT, 0, sizeof(configsMQTT));
  configsMQTT.broker.address.uri = uriMQTT;
  configsMQTT.broker.address.port = AZ_IOT_DEFAULT_MQTT_CONNECT_PORT;
  configsMQTT.credentials.client_id = clientID;
  configsMQTT.credentials.username = username;
  configsMQTT.credentials.authentication.password = sasToken;
  configsMQTT.session.keepalive = INTERVALO_KEEP_ALIVE_EM_SEGUNDOS;
  configsMQTT.session.disable_clean_session = DEVE_DESATIVAR_CLEAN_SESSION;
  configsMQTT.network.disable_auto_reconnect = DEVE_DESATIVAR_AUTO_RECONNECT;
  configsMQTT.broker.verification.certificate = (const char *)ca_pem;
  configsMQTT.broker.verification.certificate_len = (size_t)ca_pem_len;

  clientMQTT = esp_mqtt_client_init(&configsMQTT);
  if (clientMQTT == NULL)
  {
    Logger.Error("Falha na criacao do client MQTT!");
    return 1;
  }

  esp_mqtt_client_register_event(clientMQTT, MQTT_EVENT_ANY, ClientMQTTAzure::trataEvento, NULL);
  esp_err_t setUpResult = esp_mqtt_client_start(clientMQTT);
  if (setUpResult != ESP_OK)
  {
    Logger.Error("Falha na inicializacao do client MQTT! Codigo de erro: " + setUpResult);
    return 1;
  }
  else
  {
    Logger.Info("Client MQTT inicializado!");
    estaConfigurado = true;
    return 0;
  }
}

void ClientMQTTAzure::publica(char* topicoMQTT, String mensagem)
{
  int houveSucesso = esp_mqtt_client_publish(
    clientMQTT,
    topicoMQTT,
    (const char*)mensagem.c_str(),
    mensagem.length(),
    MQTT_QOS1,
    NAO_DEVE_RETER_MENSAGEM);
  
  if (!houveSucesso)
    Logger.Error("Falha no envio dos dados!");
}

void ClientMQTTAzure::encerra()
{
  (void)esp_mqtt_client_destroy(clientMQTT);
  estaConfigurado = false;
}

bool ClientMQTTAzure::verificaSeEstaConfigurado()
{
  return estaConfigurado;
}

void ClientMQTTAzure::trataEvento(void* args, esp_event_base_t base, int32_t idDoEvento, void* dadosDoEvento)
{
  (void)args;
  (void)base;
  (void)idDoEvento;

  esp_mqtt_event_handle_t evento = (esp_mqtt_event_handle_t)dadosDoEvento;
  switch (evento->event_id)
  {
    case MQTT_EVENT_ERROR:
      Logger.Info("MQTT_EVENT_ERROR: Houve um problema na comunicacao");
      break;
    case MQTT_EVENT_CONNECTED:
      Logger.Info("MQTT_EVENT_CONNECTED: Conexao estabelecida!");
      ClientMQTTAzure::trataEventoDeConexaoEstabelecida();
      break;
    case MQTT_EVENT_DISCONNECTED:
      Logger.Info("MQTT_EVENT_DISCONNECTED: Conexao finalizada!");
      break;
    case MQTT_EVENT_SUBSCRIBED:
      Logger.Info("MQTT_EVENT_SUBSCRIBED: Client inscrito em um novo topico!");
      break;
    case MQTT_EVENT_UNSUBSCRIBED:
      Logger.Info("MQTT_EVENT_UNSUBSCRIBED: Inscricao do client em um topico cancelada!");
      break;
    case MQTT_EVENT_PUBLISHED:
      Logger.Info("MQTT_EVENT_PUBLISHED: Mensagem publicada com sucesso!");
      break;
    case MQTT_EVENT_DATA:
      Logger.Info("MQTT_EVENT_DATA: Dados recebidos do servidor!");
      ClientMQTTAzure::trataEventoDeMensagemRecebida(evento);
      break;
    default:
      Logger.Error("MQTT_UNKNOWN: Evento desconhecido!");
      break;
  }
}
void ClientMQTTAzure::trataEventoDeConexaoEstabelecida()
{
  int subscriptionID = esp_mqtt_client_subscribe(clientMQTT, AZ_IOT_HUB_CLIENT_C2D_SUBSCRIBE_TOPIC, MQTT_QOS1);
  if (subscriptionID == -1)
    Logger.Error("Nao foi possível inscrever o dispositivo para receber mensagens do Azure!");
}

void ClientMQTTAzure::trataEventoDeMensagemRecebida(esp_mqtt_event_handle_t evento)
{
  int i;

  for (i = 0; i < (QTDE_BYTES_BUFFER_DE_DADOS_MQTT - 1) && i < evento->topic_len; i++)
    DADOS_RECEBIDOS[i] = evento->topic[i];
  DADOS_RECEBIDOS[i] = '\0';
  Logger.Info("Topico: " + String(DADOS_RECEBIDOS));

  for (i = 0; i < (QTDE_BYTES_BUFFER_DE_DADOS_MQTT - 1) && i < evento->data_len; i++)
    DADOS_RECEBIDOS[i] = evento->data[i];
  DADOS_RECEBIDOS[i] = '\0';
  Logger.Info("Dados: " + String(DADOS_RECEBIDOS));
}