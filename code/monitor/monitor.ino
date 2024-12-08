#include "parametros.h"
#include "src/interfaces/esp32/LoggerSerial.h"
#include "src/interfaces/esp32/GerenciadorWiFi.h"
#include "src/interfaces/esp32/GerenciadorDataHora.h"
#include "src/interfaces/esp32/LeitorTelemetria.h"
#include "src/interfaces/azure/GerenciadorIoTHub.h"

static GerenciadorWiFi gerenciadorWiFi(CFG_WIFI_SSID_AP, CFG_WIFI_SENHA_AP);
static GerenciadorIoTHub gerenciadorIoTHub(CFG_IOTHUB_FQDN, CFG_IOTHUB_URI_MQTT, CFG_IOTHUB_DEVICE_ID, CFG_IOTHUB_DEVICE_KEY);
static LeitorTelemetria leitorTelemetria(CFG_IOTHUB_DEVICE_ID, CFG_DEVE_MOCKAR_VALOR_DO_LDR);

static unsigned long ultimaContagemDeMilissegundos = 0;
static bool primeiraExecucao = true;
static int contadorDeEnvio = CFG_MULTIPLO_DE_ENVIO_DE_TELEMETRIA;

void trataEnvioDeTelemetria();
void trataEnvioDePing();
void preparaProximoCicloDeEnvio();

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

  bool iniciouCicloDeEnvio = ((unsigned long)(millis() - ultimaContagemDeMilissegundos) > CFG_INTERVALO_ENVIO_MSG_EM_MILISSEGS);
  if (primeiraExecucao || iniciouCicloDeEnvio)
  {
    Logger.Info("Iniciando ciclo de envio...");

    bool deveEnviarTelemetria = CFG_DEVE_ENVIAR_MSG && contadorDeEnvio == CFG_MULTIPLO_DE_ENVIO_DE_TELEMETRIA;
    if (deveEnviarTelemetria)
    {
      trataEnvioDeTelemetria();
    }
    else if (CFG_DEVE_ENVIAR_MSG)
    {
      trataEnvioDePing();
    }
    
    preparaProximoCicloDeEnvio();
    Logger.Info("Ciclo finalizado.");
  }
}

void trataEnvioDeTelemetria()
{
  Logger.Info("Envio de telemetria! Contador: " + String(contadorDeEnvio));
  String telemetria = leitorTelemetria.obtemLeitura();
  Logger.Info("Telemetria coletada: " + telemetria);
  gerenciadorIoTHub.enviaTelemetria(telemetria);
  contadorDeEnvio = 1;
}

void trataEnvioDePing()
{
  Logger.Info("Envio de ping! Contador: " + String(contadorDeEnvio));
  gerenciadorIoTHub.enviaPing();
  contadorDeEnvio++;
}

void preparaProximoCicloDeEnvio()
{
  ultimaContagemDeMilissegundos = millis();
  primeiraExecucao = false;
}