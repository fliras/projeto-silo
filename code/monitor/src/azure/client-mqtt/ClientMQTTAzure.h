#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <Arduino.h>
#include <mqtt_client.h>
#include <az_iot_hub_client.h>
#include "src/device/logger-serial/LoggerSerial.h"

#define QTDE_BYTES_BUFFER_DE_DADOS_MQTT 128
#define MQTT_QOS1 1
#define INTERVALO_KEEP_ALIVE_EM_SEGUNDOS 30
#define DEVE_DESATIVAR_CLEAN_SESSION 0
#define DEVE_DESATIVAR_AUTO_RECONNECT false
#define NAO_DEVE_RETER_MENSAGEM 0

class ClientMQTTAzure
{
  private:
    char* uriMQTT;
    esp_mqtt_client_config_t configsMQTT;
    bool estaConfigurado;
    static void trataEvento(void* args, esp_event_base_t base, int32_t idDoEvento, void* dadosDoEvento);
    static void trataEventoDeConexaoEstabelecida();
    static void trataEventoDeMensagemRecebida(esp_mqtt_event_handle_t event);
  
  public:
    ClientMQTTAzure(char* uriMQTT);
    int configura(char* clientID, char* username, const char* sasToken);
    void encerra();
    bool verificaSeEstaConfigurado();
    void publica(char* topicoMQTT, String mensagem);
};

#endif