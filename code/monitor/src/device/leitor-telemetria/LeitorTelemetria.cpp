#include "LeitorTelemetria.h"

LeitorTelemetria::LeitorTelemetria(char *IDPlaca, bool deveMockarSensor) : IDPlaca(IDPlaca), deveMockarSensor(deveMockarSensor) {};

int LeitorTelemetria::obtemLeitura()
{
  return deveMockarSensor ? mockaValorDoSensor() : analogRead(PINO_ANALOGICO_DO_SENSOR);
}

// método que gera um valor aleatório entre os valores mínimos e máximos do sensor
int LeitorTelemetria::mockaValorDoSensor() {
  srand((unsigned) time(NULL));
  return VALOR_MIN_DO_SENSOR + ((rand() % (VALOR_MAX_DO_SENSOR - VALOR_MIN_DO_SENSOR)));
}