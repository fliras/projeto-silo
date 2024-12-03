#include "LeitorTelemetria.h"

LeitorTelemetria::LeitorTelemetria(char *IDPlaca, bool deveMockarLDR) : IDPlaca(IDPlaca), deveMockarLDR(deveMockarLDR) {};

String LeitorTelemetria::obtemLeitura()
{
  float porcentagem = obtemPorcentagemDoSensor();
  return montaPayload(porcentagem);
}

float LeitorTelemetria::obtemPorcentagemDoSensor()
{
  int valorDoSensor = deveMockarLDR ? mockaValorDoSensor() : analogRead(PINO_ANALOGICO_DO_LDR);
  return map(valorDoSensor, VALOR_MIN_DO_LDR, VALOR_MAX_DO_LDR, 0, 100);
}

int LeitorTelemetria::mockaValorDoSensor() {
  srand((unsigned) time(NULL));
  return VALOR_MIN_DO_LDR + (rand() % VALOR_MAX_DO_LDR);
}

String LeitorTelemetria::montaPayload(float medicao)
{
  return "{\"medicao\": " + String(medicao) + "}";
}