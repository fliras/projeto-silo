/*
* monitor.ino:
* Este é o arquivo central da aplicação, no qual todos os componentes são montados e inicializados.
*/

// importação dos componentes e constantes
#include "parametros.h"
#include "src/device/logger-serial/LoggerSerial.h"
#include "src/device/gerenciador-wifi/GerenciadorWiFi.h"
#include "src/device/gerenciador-data-hora/GerenciadorDataHora.h"
#include "src/device/leitor-telemetria/LeitorTelemetria.h"
#include "src/azure/gerenciador-iot-hub/GerenciadorIoTHub.h"


// instanciação dos componentes utilizados para gerenciar Wi-Fi, conexão com o Azure IoT Hub e coleta de telemetrias (medições).
static GerenciadorWiFi gerenciadorWiFi(CFG_WIFI_SSID_AP, CFG_WIFI_SENHA_AP);
static GerenciadorIoTHub gerenciadorIoTHub(CFG_IOTHUB_FQDN, CFG_IOTHUB_URI_MQTT, CFG_IOTHUB_DEVICE_ID, CFG_IOTHUB_DEVICE_KEY);
static LeitorTelemetria leitorTelemetria(CFG_IOTHUB_DEVICE_ID, CFG_DEVE_MOCKAR_VALOR_DO_SENSOR);


// variáveis de controle do fluxo de execução.
static unsigned long ultimaContagemDeMilissegundos = 0;
static int contadorDeEnvio = CFG_MULTIPLO_DE_ENVIO_DE_TELEMETRIA;


/// @brief Método de inicialização do Esp32, que conecta na rede Wi-Fi, configura data e hora, conecta-se ao IoT Hub e inicia
/// o primeiro ciclo de execução.
void setup()
{
  gerenciadorWiFi.conecta();
  GerenciadorDataHora::defineTimeZone();
  gerenciadorIoTHub.configura();
  executaCiclo();
}


/// @brief Método principal do ciclo de vida da aplicação, que a cada execução verifica a estabilidade da conexão e executa
/// um ciclo de extração/envio de dados periodicamente.
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


/// @brief Método que representa um ciclo de execução do monitor, que coleta dados do sensor e os envia ao Azure quando necessário.
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

/// @brief Método que efetivamente envia dados ao Azure.
/// @param valorTelemetria valor extraído do sensor que será enviado ao Azure.
void trataEnvioDeTelemetria(int valorTelemetria)
{
  Logger.Info("Envio de telemetria! Contador: " + String(contadorDeEnvio));
  gerenciadorIoTHub.enviaTelemetria(valorTelemetria);
  contadorDeEnvio = 1; // Com o envio de uma medição, a contagem é resetada para dar lugar às mensagens de Ping.
}

/// @brief Método que envia mensagens de Ping ao Azure, para indicar que o Esp32 está operante.
void trataEnvioDePing()
{
  Logger.Info("Envio de ping! Contador: " + String(contadorDeEnvio));
  gerenciadorIoTHub.enviaPing();
  contadorDeEnvio++; // A cada ping o contador é aumentado para que, em algum momento, uma medição seja enviada.
}