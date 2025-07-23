/*
* LoggerSerial.h:
* Header do LoggerSerial, utilizado para pré-declarar os componentes utilizados na escrita via Serial.
*/

#ifndef SERIALLOGGER_H
#define SERIALLOGGER_H


// Importação de bibliotecas e headers de outros componentes
#include <Arduino.h>
#include "src/device/gerenciador-data-hora/GerenciadorDataHora.h"


// Constantes utilizadas pelo componente
#define BAUD_RATE_DO_SERIAL 115200

/// @brief Protótipo da classe do Logger que será implementada, atuando na escrita de informações e erros via Serial.
class LoggerSerial
{
public:
  LoggerSerial();
  void Info(String mensagem);
  void Error(String mensagem);
};

// Já exporta uma instância criada do LoggerSerial assim que o Header é importado por outro componente
extern LoggerSerial Logger;

#endif // SERIALLOGGER_H
