#include "parametros.h"
#include "src/device/logger-serial/LoggerSerial.h"
#include "src/device/gerenciador-wifi/GerenciadorWiFi.h"
#include "src/device/gerenciador-data-hora/GerenciadorDataHora.h"
#include "src/device/leitor-telemetria/LeitorTelemetria.h"
#include "src/azure/gerenciador-iot-hub/GerenciadorIoTHub.h"

static GerenciadorWiFi gerenciadorWiFi(CFG_WIFI_SSID_AP, CFG_WIFI_SENHA_AP);
static GerenciadorIoTHub gerenciadorIoTHub(CFG_IOTHUB_FQDN, CFG_IOTHUB_URI_MQTT, CFG_IOTHUB_DEVICE_ID, CFG_IOTHUB_DEVICE_KEY);
static LeitorTelemetria leitorTelemetria(CFG_IOTHUB_DEVICE_ID, CFG_DEVE_MOCKAR_VALOR_DO_LDR);

static unsigned long time_now = 0;
static bool primeiraExecucao = true;

void setup()
{
  gerenciadorWiFi.conecta();
  GerenciadorDataHora::defineTimeZone();
  gerenciadorIoTHub.configura();
}

void loop()
{
  if (!gerenciadorWiFi.estaConectado())
  {
    gerenciadorWiFi.conecta();
  }
  else if (!gerenciadorIoTHub.sessaoEstaValidada())
  {
    Logger.Info("Sessao do IOT Hub expirada. Reconectando...");
    gerenciadorIoTHub.configura();
  }
  else if (primeiraExecucao || ((unsigned long)(millis() - time_now) > CFG_INTERVALO_ENVIO_TELEMETRIA_EM_MILISSEGS))
  {
    Logger.Info("Iniciando proximo loop...");

    String telemetria = leitorTelemetria.obtemLeitura();
    Logger.Info("Telemetria coletada: " + telemetria);

    if (CFG_DEVE_ENVIAR_TELEMETRIA)
      gerenciadorIoTHub.enviaTelemetria(telemetria);
    
    time_now = millis();
    Logger.Info("Loop finalizado.");
    primeiraExecucao = false;
  }
}