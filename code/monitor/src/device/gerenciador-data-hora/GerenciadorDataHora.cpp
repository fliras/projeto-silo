#include "GerenciadorDataHora.h"

GerenciadorDataHora::GerenciadorDataHora() {}

void GerenciadorDataHora::defineTimeZone()
{
  // o zero representa ausência de horário de verão
  configTime(GMT_OFFSET_SECS, 0, SERVIDORES_NTP);
  time_t timestampAtual = time(NULL);
  while (timestampAtual < DIA_13_NOV_2017_EM_UNIX_TIME)
  {
    delay(500);
    timestampAtual = time(nullptr);
  }
}

String GerenciadorDataHora::obtemTimestampAtual()
{
  struct tm tmstruct;
  getLocalTime(&tmstruct);
  String timestampString = "";

  timestampString += String(tmstruct.tm_year + ANO_INICIAL_EPOCH_UNIX);
  timestampString += "-" + String(tmstruct.tm_mon + 1);
  timestampString += "-" + String(tmstruct.tm_mday) + " ";

  if (tmstruct.tm_hour < 10)
    timestampString += "0";
  timestampString += String(tmstruct.tm_hour) + ":";

  if (tmstruct.tm_min < 10)
    timestampString += "0";
  timestampString += String(tmstruct.tm_min) + ":";

  if (tmstruct.tm_sec < 10)
    timestampString += "0";
  timestampString += String(tmstruct.tm_sec);

  return timestampString;
}