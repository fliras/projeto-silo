#include "GerenciadorWiFi.h"

GerenciadorWiFi::GerenciadorWiFi(char *ssidAP, char *senhaAP) : ssidAP(ssidAP), senhaAP(senhaAP) {}

void GerenciadorWiFi::conecta()
{
  Logger.Info("Conectando-se ao Wi-Fi...");
  wifiManager.autoConnect(ssidAP, senhaAP);

  while (!estaConectado())
    delay(500);

  Logger.Info("Conectado! IP disponibilizado: " + obtemIP());
}

void GerenciadorWiFi::desconecta()
{
  WiFi.disconnect();
  delay(100);
}

bool GerenciadorWiFi::estaConectado()
{
  return WiFi.status() == WL_CONNECTED;
}

String GerenciadorWiFi::obtemIP()
{
  return WiFi.localIP().toString();
}