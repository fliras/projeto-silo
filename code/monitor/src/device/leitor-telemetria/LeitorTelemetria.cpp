/*
* LeitorTelemetria.cpp:
* Implementa as funcionalidades definidas no LeitorTelemetria.h
*/

// Inclusão dos Headers
#include "LeitorTelemetria.h"


/// @brief Implementação do construtor, que define se os dados devem ser lidos do sensor ou gerados aleatóriamente.
/// @param deveMockarSensor Indica se as leituras devem retornar dados reais do sensor ou valores aleatórios.
LeitorTelemetria::LeitorTelemetria(bool deveMockarSensor) : deveMockarSensor(deveMockarSensor) {};


/// @brief Implementação do método que coleta dados do sensor ou um valor gerado aleatoriamente.
/// @return Valor que corresponde a medição atual. 
int LeitorTelemetria::obtemLeitura()
{
  return deveMockarSensor ? mockaValorDoSensor() : analogRead(PINO_ANALOGICO_DO_SENSOR);
}


/// @brief Implementação do método que gera um valor aleatório entre os valores mínimos e máximos do sensor, definidos no Header.
/// @return Número gerado aleatoriamente.
int LeitorTelemetria::mockaValorDoSensor() {
  srand((unsigned) time(NULL));
  return VALOR_MIN_DO_SENSOR + ((rand() % (VALOR_MAX_DO_SENSOR - VALOR_MIN_DO_SENSOR)));
}