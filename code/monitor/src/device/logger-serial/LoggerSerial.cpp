/*
* LoggerSerial.cpp:
* Este é o arquivo que implementa as funcionalidades definidas em LoggerSerial.h
*/

// Importação dos Headers utilizados
#include "LoggerSerial.h"


/// @brief Implementação do construtor, que inicializa a comunicação via serial.
LoggerSerial::LoggerSerial()
{
  Serial.begin(BAUD_RATE_DO_SERIAL);
}

/// @brief Implementação do método que escreve informações via serial.
/// @param mensagem Conteúdo do log que será escrito.
void LoggerSerial::Info(String mensagem)
{
  Serial.print(GerenciadorDataHora::obtemTimestampAtual());
  Serial.print(" [INFO] ");
  Serial.println(mensagem);
}

/// @brief Implementação do método que escreve mensagens de erro via serial.
/// @param mensagem Conteúdo do log que será escrito.
void LoggerSerial::Error(String mensagem)
{
  Serial.print(GerenciadorDataHora::obtemTimestampAtual());
  Serial.print(" [ERROR] ");
  Serial.println(mensagem);
}

/// @brief Instância global exportada ao importar o Header do LoggerSerial
LoggerSerial Logger;
