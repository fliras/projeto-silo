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
  Logger.Info("Valor coletado do LDR: " + String(valorDoSensor));
  return remapeiaLDRParaPorcentagem(valorDoSensor, VALOR_MIN_DO_LDR, VALOR_MAX_DO_LDR, 0, 100);
}

int LeitorTelemetria::mockaValorDoSensor() {
  srand((unsigned) time(NULL));
  return VALOR_MIN_DO_LDR + (rand() % VALOR_MAX_DO_LDR);
}

float LeitorTelemetria::remapeiaLDRParaPorcentagem(float x, float in_min, float in_max, float out_min, float out_max) {
  float percentual = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  float percentualComDuasCasas = round(percentual * 100) / 100;
  return percentualComDuasCasas;
}

String LeitorTelemetria::montaPayload(float medicao)
{
  return "{\"medicao\": " + String(medicao) + "}";
}