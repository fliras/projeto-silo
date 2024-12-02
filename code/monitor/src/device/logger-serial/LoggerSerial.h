// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#ifndef SERIALLOGGER_H
#define SERIALLOGGER_H

#include <Arduino.h>
#include "src/device/gerenciador-data-hora/GerenciadorDataHora.h"

#define BAUD_RATE_DO_SERIAL 115200

class LoggerSerial
{
public:
  LoggerSerial();
  void Info(String mensagem);
  void Error(String mensagem);
};

extern LoggerSerial Logger;

#endif // SERIALLOGGER_H
