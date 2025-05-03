#include "LeitorTelemetria.h"

LeitorTelemetria::LeitorTelemetria(char *IDPlaca, bool deveMockarLDR) : IDPlaca(IDPlaca), deveMockarLDR(deveMockarLDR) {};

int LeitorTelemetria::obtemLeitura()
{
  return deveMockarLDR ? mockaValorDoSensor() : analogRead(PINO_ANALOGICO_DO_LDR);
}

// método que gera um valor aleatório entre os valores mínimos e máximos do sensor
int LeitorTelemetria::mockaValorDoSensor() {
  srand((unsigned) time(NULL));
  return VALOR_MIN_DO_LDR + ((rand() % (VALOR_MAX_DO_LDR - VALOR_MIN_DO_LDR)));
}