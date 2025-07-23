/*
* GerenciadorWiFi.cpp:
* Implementa as funcionalidades definidas no GerenciadorWiFi.h
*/

// Inclusão dos Headers
#include "GerenciadorWiFi.h"


/// @brief Implementação do construtor, que inicializa os atributos da classe.
/// @param ssidAP SSID do AccessPoint que o Esp32 abre para que se configure sua conexão.
/// @param senhaAP Senha do AccessPoint que o Esp32 abre para que se configure sua conexão.
GerenciadorWiFi::GerenciadorWiFi(char *ssidAP, char *senhaAP) : ssidAP(ssidAP), senhaAP(senhaAP) {}


/// @brief Implementação do método que conecta em uma rede WiFi, por meio de um componente externo.
void GerenciadorWiFi::conecta()
{

  // O componente externo tenta se conectar a uma rede a qual já se conectara anteriormente. Caso não consiga,
  // abre um AccessPoint com SSID e Senha especificados, para que um usuário se conecte e especifique, em uma
  // página Web, com qual rede Wi-Fi o Esp32 deve se conectar. Após reinicialização, as novas definições passam a valer.
  Logger.Info("Conectando-se ao Wi-Fi...");
  wifiManager.autoConnect(ssidAP, senhaAP);

  while (!estaConectado())
    delay(500);

  Logger.Info("Conectado! IP disponibilizado: " + obtemIP());
}


/// @brief Implementação do método que desconecta de uma rede WiFi, por meio de um componente externo.
void GerenciadorWiFi::desconecta()
{
  WiFi.disconnect();
  delay(100);
}


/// @brief Implementação do método que indica o status atual de conexão com a rede WiFi.
/// @return true: Esp32 Conectado; false: Esp32 Desconectado.
bool GerenciadorWiFi::estaConectado()
{
  return WiFi.status() == WL_CONNECTED;
}


/// @brief Implementação do método que indica o IP atribuído ao Esp32, quando conectado a uma rede WiFi
/// @return Endereço IP atribuído.
String GerenciadorWiFi::obtemIP()
{
  return WiFi.localIP().toString();
}