#include "LoggerSerial.h"

LoggerSerial::LoggerSerial()
{
  Serial.begin(BAUD_RATE_DO_SERIAL);
}

void LoggerSerial::Info(String mensagem)
{
  Serial.print(GerenciadorDataHora::obtemTimestampAtual());
  Serial.print(" [INFO] ");
  Serial.println(mensagem);
}

void LoggerSerial::Error(String mensagem)
{
  Serial.print(GerenciadorDataHora::obtemTimestampAtual());
  Serial.print(" [ERROR] ");
  Serial.println(mensagem);
}

LoggerSerial Logger;
