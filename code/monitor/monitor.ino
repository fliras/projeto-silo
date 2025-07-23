#include "parametros.h"
#include "src/device/logger-serial/LoggerSerial.h"
#include "src/device/gerenciador-wifi/GerenciadorWiFi.h"
#include "src/device/gerenciador-data-hora/GerenciadorDataHora.h"
#include "src/device/leitor-telemetria/LeitorTelemetria.h"
#include "src/azure/gerenciador-iot-hub/GerenciadorIoTHub.h"

static GerenciadorWiFi gerenciadorWiFi(CFG_WIFI_SSID_AP, CFG_WIFI_SENHA_AP);
static GerenciadorIoTHub gerenciadorIoTHub(CFG_IOTHUB_FQDN, CFG_IOTHUB_URI_MQTT, CFG_IOTHUB_DEVICE_ID, CFG_IOTHUB_DEVICE_KEY);
static LeitorTelemetria leitorTelemetria(CFG_IOTHUB_DEVICE_ID, CFG_DEVE_MOCKAR_VALOR_DO_SENSOR);

static unsigned long ultimaContagemDeMilissegundos = 0;
static int contadorDeEnvio = CFG_MULTIPLO_DE_ENVIO_DE_TELEMETRIA;

void setup()
{
  gerenciadorWiFi.conecta();
  GerenciadorDataHora::defineTimeZone();
  gerenciadorIoTHub.configura();
  executaCiclo();
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

  unsigned long intervaloDesdeUltimoCicloEmMilissegundos = (unsigned long)(millis() - ultimaContagemDeMilissegundos);
  if (intervaloDesdeUltimoCicloEmMilissegundos >= CFG_INTERVALO_ENVIO_MSG_EM_MILISSEGS)
  {
    executaCiclo();
  }
}

void executaCiclo()
{
  Logger.Info("Iniciando ciclo...");
  int valorTelemetria = leitorTelemetria.obtemLeitura();
  Logger.Info("Valor lido do sensor: " + String(valorTelemetria));

  bool enviaTelemetriaNesseCiclo = contadorDeEnvio == CFG_MULTIPLO_DE_ENVIO_DE_TELEMETRIA;
  if (CFG_DEVE_ENVIAR_MSG && enviaTelemetriaNesseCiclo)
  {
    trataEnvioDeTelemetria(valorTelemetria);
  }
  else if (CFG_DEVE_ENVIAR_MSG)
  {
    trataEnvioDePing();
  }

  ultimaContagemDeMilissegundos = millis();
  Logger.Info("Ciclo finalizado.");
}

void trataEnvioDeTelemetria(int valorTelemetria)
{
  Logger.Info("Envio de telemetria! Contador: " + String(contadorDeEnvio));
  gerenciadorIoTHub.enviaTelemetria(valorTelemetria);
  contadorDeEnvio = 1;
}

void trataEnvioDePing()
{
  Logger.Info("Envio de ping! Contador: " + String(contadorDeEnvio));
  gerenciadorIoTHub.enviaPing();
  contadorDeEnvio++;
}