/*
* ClientMQTTAzure.cpp:
* Implementação das funcionalidades definidas em ClientMQTTAzure.h.
*/


// Inclusão dos Headers.
#include "ClientMQTTAzure.h"
#include <azure_ca.h>


// Declaração de objetos utilizados no fluxo de execução da classe.
static esp_mqtt_client_handle_t clientMQTT;
static char DADOS_RECEBIDOS[QTDE_BYTES_BUFFER_DE_DADOS_MQTT];


/// @brief Implementação do construtor, que inicializa os atributos da classe.
/// @param uriMQTT endereço MQTT do Azure, ao qual o Esp32 deve se conectar.
ClientMQTTAzure::ClientMQTTAzure(char* uriMQTT): uriMQTT(uriMQTT), estaConfigurado(estaConfigurado) {}


/// @brief Método utilizado para configurar o client MQTT que será utilizado para comunicação com o Azure.
/// @param clientID ID do Client do IoT Hub.
/// @param username Username do Client do IoT Hub.
/// @param sasToken Token de autenticação do Client do IoT Hub.
/// @return 0: Client configurado sem erros; 1: Erros na configuração do client.
int ClientMQTTAzure::configura(char* clientID, char* username, const char* sasToken)
{
  // limpa a struct com as configurações do client e a remonta com as novas informações.
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

  // tenta inicializar o client já configurado.
  clientMQTT = esp_mqtt_client_init(&configsMQTT);
  if (clientMQTT == NULL)
  {
    Logger.Error("Falha na criacao do client MQTT!");
    return 1;
  }

  // tenta registrar os eventos que irá monitorar junto ao Azure.
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


/// @brief Implementação do método que publica dados no Azure via MQTT em um determinado tópico.
/// @param topicoMQTT Tópico MQTT ao qual a mensagem está relacionada.
/// @param mensagem Mensagem que será enviada ao Azure.
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


/// @brief Implementação do método que finaliza o funcionamento do client MQTT, uma espécie de "Dispose".
void ClientMQTTAzure::encerra()
{
  (void)esp_mqtt_client_destroy(clientMQTT);
  estaConfigurado = false;
}


/// @brief Implementação do método que verifica se o Client MQTT já foi configurado.
/// @return true: Client configurado; false: Client ainda não configurado.
bool ClientMQTTAzure::verificaSeEstaConfigurado()
{
  return estaConfigurado;
}


/// @brief Método que trata os eventos MQTT disparados pelo Azure, executando uma determinada ação de acordo
/// com o contexto do evento.
/// @param args parâmetro recebido com o disparo do evento (sem utilidade).
/// @param base parâmetro recebido com o disparo do evento (sem utilidade).
/// @param idDoEvento parâmetro recebido com o disparo do evento (sem utilidade).
/// @param dadosDoEvento dados associados ao evento retornados pelo Azure.
void ClientMQTTAzure::trataEvento(void* args, esp_event_base_t base, int32_t idDoEvento, void* dadosDoEvento)
{
  // Esvazia estes parâmetros recebidos pelo evento, não possuem utilidade.
  (void)args;
  (void)base;
  (void)idDoEvento;

  // carrega o evento e executa a ação respectiva ao tipo dele.
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


/// @brief Implementação do método que inscreve o Client MQTT nos tópicos necessários assim que a conexão é estabelecida.
void ClientMQTTAzure::trataEventoDeConexaoEstabelecida()
{
  int subscriptionID = esp_mqtt_client_subscribe(clientMQTT, AZ_IOT_HUB_CLIENT_C2D_SUBSCRIBE_TOPIC, MQTT_QOS1);
  if (subscriptionID == -1)
    Logger.Error("Nao foi possível inscrever o dispositivo para receber mensagens do Azure!");
}


/// @brief Implementação do método que processa os dados recebidos do Azure.
/// @param evento Evento associado a mensagem recebida.
void ClientMQTTAzure::trataEventoDeMensagemRecebida(esp_mqtt_event_handle_t evento)
{
  int i;

  // Carrega e exibe o tópico MQTT associado a mensagem recebida.
  for (i = 0; i < (QTDE_BYTES_BUFFER_DE_DADOS_MQTT - 1) && i < evento->topic_len; i++)
    DADOS_RECEBIDOS[i] = evento->topic[i];
  DADOS_RECEBIDOS[i] = '\0';
  Logger.Info("Topico: " + String(DADOS_RECEBIDOS));

  // Carrega e exibe o conteúdo da mensagem recebida.
  for (i = 0; i < (QTDE_BYTES_BUFFER_DE_DADOS_MQTT - 1) && i < evento->data_len; i++)
    DADOS_RECEBIDOS[i] = evento->data[i];
  DADOS_RECEBIDOS[i] = '\0';
  Logger.Info("Dados: " + String(DADOS_RECEBIDOS));
}