/*
* GerenciadorDataHora.cpp:
* Implementa as funcionalidades definidas no GerenciadorDataHora.h
*/

// Inclusão dos Headers
#include "GerenciadorDataHora.h"


/// @brief Implementação do Construtor
GerenciadorDataHora::GerenciadorDataHora() {}


/// @brief Implementação do método que define o TimeZone utilizado no Esp32,
/// por meio de servidores NTP.
// OBS: Como a configuração de Data e Hora é feita via internet, o fluxo
// de inicialização do software pode ficar preso aqui caso a rede não tenha uma conexão estável.
void GerenciadorDataHora::defineTimeZone()
{
  // o zero representa ausência de horário de verão
  configTime(GMT_OFFSET_SECS, 0, SERVIDORES_NTP);

  // enquanto o TimeZone estiver incorreto, continuará tentando configurar.
  time_t timestampAtual = time(NULL);
  while (timestampAtual < DIA_13_NOV_2017_EM_UNIX_TIME)
  {
    delay(500);
    timestampAtual = time(nullptr);
  }
}


/// @brief Implementação do método que obtém a data e hora atual no Esp32.
/// @return data e hora atuais no seguinte formato: "aaaa-mm-dd hh:mm:ss"
String GerenciadorDataHora::obtemTimestampAtual()
{
  struct tm timestampBruto;
  getLocalTime(&timestampBruto);

  String timestampFormatado = "";
  timestampFormatado += String(timestampBruto.tm_year + ANO_INICIAL_EPOCH_UNIX);
  timestampFormatado += "-" + String(timestampBruto.tm_mon + 1);
  timestampFormatado += "-" + String(timestampBruto.tm_mday) + " ";

  if (timestampBruto.tm_hour < 10)
    timestampFormatado += "0";
  timestampFormatado += String(timestampBruto.tm_hour) + ":";

  if (timestampBruto.tm_min < 10)
    timestampFormatado += "0";
  timestampFormatado += String(timestampBruto.tm_min) + ":";

  if (timestampBruto.tm_sec < 10)
    timestampFormatado += "0";
  timestampFormatado += String(timestampBruto.tm_sec);

  return timestampFormatado;
}